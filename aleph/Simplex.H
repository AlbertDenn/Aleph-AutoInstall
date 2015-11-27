
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath León
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met: 

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright 
     notice, this list of conditions and the following disclaimer in the 
     documentation and/or other materials provided with the distribution.

  3. All advertising materials mentioning features or use of this software
     must display the following acknowledgement:

     Copyright (c) 2002-2014 Leandro Rabindranath León. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath León ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Aleph-w is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.

  I request users of this software to return to 

  Leandro Rabindranath Leon
  CEMISID 
  Ed La Hechicera 
  3er piso, ala sur
  Facultad de Ingenieria 
  Universidad de Los Andes 
  Merida - REPÚBLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/


# ifndef SIMPLEX_H
# define SIMPLEX_H

# include <limits>
# include <fstream>
# include <tpl_dynMat.H>
# include <tpl_dynDlist.H>
# include <format.H>

namespace Aleph {


template <typename T> class Simplex
{
public:

  enum State { Not_Solved, Solving, Unbounded, Solved, Unfeasible };

private:

  
      // selecciona la celda de la función objetivo con menor
      // valor. Retorna -1 si todas las celdas son negativas

  int compute_pivot_col() const noexcept
  {
    T minimum = numeric_limits<T>::max();
    int p = -1;
    for (int i = 0, M = num_var + num_rest; i < M; i++)
      {
        const T & c = m->read(0, i);
        if (c < minimum)
          {
            p = i;
            minimum = c;
          }
      }
    return minimum >= 0 ? -1 : p;
  }

        // selecciona entre los elementos B el menor valor de división
        // entre el col_idx coeficiente de la función objetivo que
        // sea positivo
  int compute_pivot_row(int p) const noexcept
  {
    assert(p >= 0 and p < num_var + num_rest);

    int q = -1;
    T min_ratio = numeric_limits<T>::max();
    for (int i = q + 1, M = num_var + num_rest; i <= num_rest; i++)
      {
        const T val = m->read(i, M); 
        if (val < 0) 
          continue;

        const T den = m->read(i, p);
        if (den <= 0)
          continue;

        const T ratio = val / den;
        if (ratio < min_ratio)
          {
            q = i;
            min_ratio = ratio;
          }
      }
    return q;
  }

  State select_pivot(int & p, int & q) noexcept
  {
    assert(state == Not_Solved or state == Solving);

    const int col = compute_pivot_col();
    if (col == -1) 
      return state = Solved;

    const int row = compute_pivot_row(col);
    if (row == -1)
      return state = Unbounded;

    p = row;
    q = col;

    return state = Solving;
  }

  void to_pivot(size_t p, size_t q) noexcept
  {
    assert(p <= num_rest and q <= num_var + num_rest);

    const int M = num_var + num_rest; // cantidad de columnas 
    const T pivot = m->read(p, q);
    for (int j = 0; j <= M; j++) // fila del pivote
      if (j != q)
        m->write(p, j, m->read(p, j) / pivot);

    m->write(p, q, 1);

    for (int i = 0; i <= num_rest; i++) // resto de las filas
      for (int j = 0; j <= M; j++)
        if (i != p and j != q)
          m->write(i, j, m->read(i,j) - m->read(i,q)*m->read(p,j));

    for (int i = 0; i <= num_rest; i++) // col de pivote en 0 salvo q
      if (i != p)
        m->write(i, q, 0);
  }

  T find_value(const size_t j) const noexcept
  {
    assert(j < num_var);

    T ret_val = 0.0;
    for (int i = 1, count = 0; i < num_rest; i++)
      {
        const T & value = m->read(i, j);
        if (value == 0.0)
          continue;

        if (value == 1.0)
          if (count++ == 0)  
            ret_val = m->read(i, num_var + num_rest);
          else
            return 0.0;
        else
          return ret_val;
      }
    return ret_val;
  }

  DynMatrix<T> * m;

  T * objetive;

  DynDlist<T*> rest_list;

  int num_var;
  int num_rest;

  T * solution;

  public:

  private:

    State state;

    void verify_var_index(int i)
    {
      if (i >= num_var)
        throw std::out_of_range("index " + to_string(i) + " out of range (" +
				to_string(num_var - 1) + "");
    }

    T * create_restriction() 
    {
      T * ptr = new T [num_var + 1];
      rest_list.append(ptr);
      num_rest++;
      for (int i = 0; i <= num_var; i++)
        ptr[i] = 0;
      return ptr;
    }

    void create_matrix()
    {
      m = new DynMatrix<T> (num_rest + 1, num_var + num_rest + 1);

      int i = 0; // llena los num_var coeficientes de la función objetivo
      for (; i < num_var; i++)
        m->write(0, i, -objetive[i]);
      
          // llena los num_var coeficientes de las restricciones
      i = 1;
      for (typename DynDlist<T*>::Iterator it(rest_list); 
           it.has_current(); it.next(), i++)
        {
          T * rest = it.get_current();

          for (int j = 0; j < num_var; j++)
            m->write(i, j, rest[j]);

          m->write(i, num_var + i - 1, 1); // coef 1 i-esima variable slack

          m->write(i, num_var + num_rest, rest[num_var]); 
        }
    }

public:

  Simplex(int n) 
    : m(nullptr), objetive(nullptr), num_var(n), num_rest(0), state(Not_Solved)
  {
    objetive = new T [num_var];
    try
      {
        solution = new T [num_var];
      }
    catch (...)
      {
        delete objetive;
        throw;
      }
  }

  void put_objetive_function_coef(int i, const T & coef) noexcept
  {
    verify_var_index(i);
    objetive[i] = coef;
  }

  void put_objetive_function(DynArray<T> & coefs) noexcept
  {
    for (int i = 0; i < num_var; i++)
      objetive[i] = coefs;
  }

  void put_objetive_function(T coefs[]) noexcept
  {
    for (int i = 0; i < num_var; i++)
      objetive[i] = coefs;
  }
  T * put_restriction(T * coefs = nullptr)
  {
    T * rest = create_restriction();
      
    if (coefs == nullptr)
      return rest;

    for (int i = 0; i <= num_var; ++i)
      rest[i] = coefs[i];

    return rest;
  }

  T * get_restriction(int rest_num) 
  {
    if (rest_num >= num_rest)
      throw std::out_of_range("index " + to_string(rest_num) +
			      "out of range (" + to_string(num_rest - 1) + ")");

    int i = 1;
    for (typename DynDlist<T*>::Iterator it(rest_list); 
         it.has_current(); it.next())
      if (i == rest_num)
        return it.get_current();
  }

  T * put_restriction(DynArray<T> & coefs)
  {
    T * rest = create_restriction();
      
    for (int i = 0; i <= num_var; ++i)
      rest[i] = coefs[i];

    return rest;
  }

  State latex_solve(char * name = nullptr)
  {
    latex_matrix(string(name) + "-0.tex");
    for (int i, j, k = 1; true; k++)
      {
        const Simplex<T>::State state = select_pivot(i, j);

        string str = string(name) + "-" + to_string(k) + ".tex";
        if (state == Simplex<T>::Unbounded or state == Simplex<T>::Solved)
          {
            latex_matrix(str.c_str());
            return state;
          }

        latex_matrix(str.c_str(), 2, i, j);
        to_pivot(i, j);

        latex_matrix(name, i, j);
      } 
  }

  State solve()
  {
    if (state != Not_Solved)
      throw std::logic_error("solve() has already been called");
    if (num_rest == 0)
      throw std::logic_error("linear program without restrictions");

    for (int i, j; true;)
      {
        const Simplex<T>::State state = select_pivot(i, j);
        if (state == Simplex<T>::Unbounded or state == Simplex<T>::Solved)
          return state;

        to_pivot(i, j);
      } 
  }

  void load_solution() noexcept
  {
    for (int j = 0; j < num_var; j++)
      solution[j] = find_value(j);
  }

  const T & get_solution(size_t i) const noexcept
  {
    assert(i < num_var);
    return solution[i];
  }

  T objetive_value() const noexcept
  {
    T sum = 0;
    for (int i = 0; i < num_var; i++)
      sum += solution[i]*objetive[i];
    return sum;
  }

  bool verify_solution() const noexcept
  {
    for (typename DynDlist<T*>::Iterator it(rest_list); 
         it.has_current(); it.next())
      {
        T * rest = it.get_current();
        T sum = 0;
        for (int j = 0; j < num_var; j++)
          sum += rest[j]*solution[j];
        
        if (sum > rest[num_var])
          return false;
      }
    return true;
  }

  void print_matrix()
  {
    for (int i = 0; i <= num_rest; ++i)
      {
        for (int j = 0; j <= num_var + num_rest; j++)
          cout << float_f(m->read(i, j), 2) << " ";

        cout << endl;
      }
  }

  void latex_matrix(const string & name, int d = 2, int p = -1, int q = -1)
  {
    ofstream out(name, ios::out);

    const int cols = num_var + num_rest;

    out << "$\\left(\\begin{array}{c";
    for (int i = 0; i < cols; i++)
      out << "c";
    out << "}" << endl;

    for (int i = 0; i <= num_rest; ++i)
      {
        for (int j = 0; j <= cols; j++)
          {
            if (i == p and j == q)
              out << "\\circled{" << float_f(m->read(i, j), d) << "}" << " ";
            else
              out << float_f(m->read(i, j), d) << " ";
            if (j != cols)
              out << "& ";
          }

        if (i != num_rest)
          out << "\\\\";

        out << endl;
      }
    out << "\\end{array}\\right)$" << endl;
  }

  void latex_linear_program(const string & name)
  {
    ofstream out(name, ios::out);
    out << "\\begin{equation*}" << endl
        << "Z = " ;
    for (int i = 0; i < num_var; i++)
      {
        if (objetive[i] == 0.0)
          continue;

        if (i > 0)
          out << " + ";

        if (objetive[i] != 1.0)
          out << objetive[i];
        out << "x_" << i; 
      }
    out << endl
        << "\\end{equation*}" << endl
        << "Sujeto a:" << endl
        << "\\begin{eqnarray*}" << endl;

    for (typename DynDlist<T*>::Iterator it(rest_list); 
         it.has_current(); it.next())
      {
        T * rest = it.get_current();

        for (int i = 0; i < num_rest; i++)
          {
            if (rest[i] == 0.0)
              continue;

            if (i > 0)
              out << " + ";

            if (rest[i] != 1.0)
              out << rest[i];
            out << " x_" << i;
          }

        out << " & \\leq & " << rest[num_rest];

        if (not it.is_in_last())
          out << " \\\\";
              
        out << endl;
      }
    out << "\\end{eqnarray*}" << endl;
  }

  size_t get_num_restrictions() const noexcept { return num_rest; }

  size_t get_num_vars() const noexcept { return num_var; }

  ~Simplex()
  {
    delete [] objetive;

    if (m != nullptr)
      delete m;
  }

  T * get_objetive_function() noexcept
  {
    return objetive;
  }

  T & get_restriction_coef(int rest_num, int idx) 
  {
    verify_var_index(idx);

    return get_restriction(rest_num)[idx];
  } 

  void put_restriction_coef(int rest_num, int idx, const T & coef)
  {
    get_restriction_coef(rest_num, idx) = coef;
  }

  void prepare_linear_program()
  {
    create_matrix();
  }
};


} // end namespace Aleph

# endif // SIMPLEX_H

