
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


# include <limits>
# include <iostream>
# include <tpl_matgraph.H>
# include <mat_latex.H>
# include <tpl_graph_utils.H>
# include <Floyd.H>

using namespace std;

using namespace Aleph;

# define INDENT "    "

    template <typename M>
struct C_i
{
  string operator () (M &, const long & i)
  {
    char buf[256];

    snprintf(buf, 256, "%ld", i);

    return string(buf);
  }
};



    template <typename M>
struct C_AM
{
  string operator () (M & mat, const long & i, const long & j)
  {
    typename M::Arc_Type::Distance_Type value = mat(i, j);

    if (value == numeric_limits<double>::infinity())
      return string("$\\infty$");

    if (value == 0)
      return string("$0$");

    char buf[256]; 

    snprintf(buf, 256, "$%.2f$", value);

    return string(buf);
  }
};

    template <typename M, class C_ij>
void mat_latex(M & m)
{
  const long & n = m.get_num_nodes();

  mat_to_latex<M, C_i<M>, C_i<M>, C_ij >(m, n, n, cout);
}

struct Ciudad
{
  string nombre;

  Ciudad() {}

  Ciudad(const string & str) : nombre(str) { /* empty */ }

  Ciudad(const char * str) : nombre(str) { /* empty */ }

  bool operator == (const Ciudad & der) const 
  {
    return nombre == der.nombre; 
  }
};


struct Via
{
  typedef double Distance_Type;

  static const double Max_Distance;

  static const double Zero_Distance;

  string nombre;

  double distancia;

  Via() : distancia(Max_Distance) { /* empty */ }

  Via(const double & dist) 
    : nombre("NO-DEFINIDO"), distancia(dist) 
  {
    /* empty */ 
  }

  Via(const string & n, const double & dist) 
    : nombre(n), distancia(dist) 
  {
    /* empty */ 
  }

  const double & get_distance() const { return distancia; }

  operator double & () { return distancia; }
}; 

const Via Via_Zero("Zero", 0);

const double Via::Max_Distance = numeric_limits<double>::infinity();

const double Via::Zero_Distance = 0;


typedef Graph_Node<Ciudad> Node_Ciudad;

typedef Graph_Arc<Via> Arco_Via;

typedef List_Digraph<Node_Ciudad, Arco_Via> Grafo;


void imprimir_grafo(Grafo & g)
{
  cout << endl
       << "Listado de nodos (" << g.get_num_nodes() << ")" << endl;

  for (Grafo::Node_Iterator node_itor(g); node_itor.has_current(); 
       node_itor.next())
    cout << INDENT << node_itor.get_current_node()->get_info().nombre << endl;

  cout << endl
       << endl
       << "Listado de arcos (" << g.get_num_arcs() << ")"
       << endl;

  for (Grafo::Arc_Iterator arc_itor(g); arc_itor.has_current();
       arc_itor.next())
    {
      Grafo::Arc * arc = arc_itor.get_current_arc();
      cout << arc->get_info().nombre << " " 
	   << arc->get_info().distancia << " de " 
	   << g.get_src_node(arc)->get_info().nombre << " a " 
	   << g.get_tgt_node(arc)->get_info().nombre << endl;
    }

  cout << endl
       << endl
       << "Listado del grafo por nodos y en cada nodo por arcos" 
       << endl;
  for (Grafo::Node_Iterator node_itor(g); node_itor.has_current(); 
       node_itor.next())
    {
      Grafo::Node * src_node = node_itor.get_current_node();
      cout << src_node->get_info().nombre << endl;
      for (Grafo::Node_Arc_Iterator itor(node_itor.get_current_node());
	   itor.has_current(); itor.next())
	{
	  Grafo::Arc * arc = itor.get_current_arc();
	  cout << INDENT << arc->get_info().distancia << " " 
	       << g.get_connected_node(arc, src_node)->get_info().nombre 
	       << endl;
	}
    }
  cout << endl;
}



void insertar_arco(Grafo &       grafo, 
		  const string & src_name, 
		  const string & tgt_name,
		  const double & distancia)
{
  Grafo::Node * n1 = grafo.search_node([&src_name] (Grafo::Node * p)
    {
      return p->get_info().nombre == src_name;
    });

  if (n1 == NULL)
    n1 = grafo.insert_node(src_name);

  Grafo::Node * n2 = grafo.search_node([&tgt_name] (Grafo::Node * p)
    {
      return p->get_info().nombre == tgt_name;
    });

  if (n2 == NULL)
    n2 = grafo.insert_node(tgt_name);

  string nombre_arco = n1->get_info().nombre + "-->" + n2->get_info().nombre;

  grafo.insert_arc(n1, n2, Via(nombre_arco, distancia));
}



void build_test_graph(Grafo & g)
{
  g.insert_node("0");
  g.insert_node("1");
  g.insert_node("2");
  g.insert_node("3");
  g.insert_node("4");
  g.insert_node("5");

  insertar_arco(g, "0", "1", 0.41);
  insertar_arco(g, "0", "5", 0.29);
  insertar_arco(g, "1", "2", 0.5);
  insertar_arco(g, "1", "4", 0.32);
  insertar_arco(g, "2", "3", 0.5);
  insertar_arco(g, "3", "0", 0.45);
  insertar_arco(g, "3", "1", 0.86);
  insertar_arco(g, "3", "5", 0.38);
  insertar_arco(g, "4", "2", 0.32);
  insertar_arco(g, "4", "3", 0.35);
  insertar_arco(g, "5", "1", 0.29);
  insertar_arco(g, "5", "4", 0.21);
}


    template <typename MT>
void imprimir_map(MT & mat)
{
  for (int i = 0; i < mat.get_num_nodes(); ++i)
    cout << "Nodo " << i << "(" << mat(i) << ") contiene: " 
	 << mat(i)->get_info().nombre << endl;

  for (int i = 0; i < mat.get_num_nodes(); ++i)
    {
      cout << mat(i)->get_info().nombre << " [ ";
      for (int j = 0; j < mat.get_num_nodes(); ++j)
	{
	  typename MT::Arc * arc = mat(i, j);

	  if (arc == NULL)
	    cout << "Null";
	  else
	    cout << arc->get_info().get_distance() ;
	  
	  if (j != mat.get_num_nodes() - 1)
	    cout << " , ";
	}
      cout << " ]" << endl;
    }
}

    template <typename MAT>
void imprimir_matriz(const MAT & mat)
{
  for (int i = 0; i < mat.get_num_nodes(); ++i)
    cout << "Nodo " << i << "(" << mat(i).nombre << ") contiene: " 
	 << mat(i).nombre << endl;

  for (int i = 0; i < mat.get_num_nodes(); ++i)
    {
      cout << mat(i).nombre << " [ ";
      for (int j = 0; j < mat.get_num_nodes(); ++j)
	{
	  cout << mat(i, j).get_distance();

	  if (j != mat.get_num_nodes() - 1)
	    cout << " , ";
	}
      cout << " ]" << endl;
    }
}



    template <typename AD>
void imprimir_ady_mat(const AD & mat)
{
  for (int i = 0; i < mat.get_num_nodes(); ++i)
    cout << "Nodo " << i << "(" << mat(i) << ") contiene: " 
	 << mat(i)->get_info().nombre << endl;

  for (int i = 0; i < mat.get_num_nodes(); ++i)
    {
      cout << mat(i)->get_info().nombre << " [ ";
      for (int j = 0; j < mat.get_num_nodes(); ++j)
	{
	  cout << mat(i, j);

	  if (j != mat.get_num_nodes() - 1)
	    cout << " , ";
	}
      cout << " ]" << endl;
    }
}


    template <typename GT>
void imprimir_bit_mat(Bit_Mat_Graph<GT> & mat)
{
  if (mat.get_list_graph() != NULL)
    for (int i = 0; i < mat.get_num_nodes(); ++i)
      cout << "Nodo " << i << "(" << mat(i) << ") contiene: " 
	   << mat(i)->get_info().nombre << endl;

  for (int i = 0; i < mat.get_num_nodes(); ++i)
    {
      if (mat.get_list_graph() != NULL)
	cout << mat(i)->get_info().nombre << " [ ";
      else
	cout  << " [ ";

      for (int j = 0; j < mat.get_num_nodes(); ++j)
	{
	  cout << mat(i, j);

	  if (j != mat.get_num_nodes() - 1)
	    cout << " , ";
	}
      cout << " ]" << endl;
    }
}

void imprimir_camino(Path<Grafo> & path)
{
  cout << endl
       << "Camino: ";
  for (Path<Grafo>::Iterator itor(path); itor.has_current(); itor.next())
    cout << itor.get_current_node()->get_info().nombre << "-";

  cout << endl;
}

void copia_Arco_Via(Arco_Via * arc, const long&, const long&, double& value)
{
  value = arc->get_info().get_distance();
}

int sum(const int & b1, const int & b2)
{
  return b1 == 1 or b2 == 1 ? 1 : 0;
}

int product(const int & b1, const int & b2)
{
  return b1 == 0 or b2 == 0 ? 0 : 1;
}

    template <class GT> 
Bit_Mat_Graph<GT> operator * (Bit_Mat_Graph<GT> & m1, Bit_Mat_Graph<GT> & m2)
{
  Bit_Mat_Graph<GT> ret(m1.get_num_nodes());

  const size_t & n = m1.get_num_nodes();

  for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
	{
	  int acum = 0;

	  for (int k = 0; k < n; k++)
	    {
	      acum = sum(acum, product(m1(i,k), m2(k, j)));

	      if (acum == 1)
		break; // el resultado será 1 ==> no vale la pena proseguir
	    }
	  
	  ret(i, j) = acum;
	}
    }

  return ret;
}



int main() 
{
  Grafo list_graph;

  build_test_graph(list_graph);

  //  imprimir_grafo(list_graph);

  Map_Matrix_Graph<Grafo> mat(list_graph);

  cout << "Matriz de adyacencia" << endl;

  imprimir_map(mat);

  Matrix_Graph<Grafo> matriz(list_graph, Via_Zero);

  imprimir_matriz(matriz);

# ifdef nada
  Ady_Mat<Grafo, double> ady_mat(list_graph, 
				 numeric_limits<double>::infinity());

  imprimir_ady_mat(ady_mat);

  Ady_Mat<Grafo, double> cost_mat(list_graph);

  Ady_Mat<Grafo, long> path_mat(list_graph);

  floyd_all_shortest_paths(list_graph, cost_mat, path_mat);

  Grafo::Path path;

  find_min_path(path_mat, 0, 2, path);

  imprimir_camino(path);

  find_min_path(path_mat, path.get_first_node(), path.get_last_node(), path);

  imprimir_camino(path);

  cout << "Matriz de costes" << endl;
  imprimir_ady_mat(cost_mat);

  cout << "Matriz de caminos" << endl;
  imprimir_ady_mat(path_mat);

# endif

  Bit_Mat_Graph<Grafo> bitmat(list_graph);
  imprimir_bit_mat(bitmat);

  Bit_Mat_Graph<Grafo> b1(bitmat);
  imprimir_bit_mat(b1);

  Bit_Mat_Graph<Grafo> b2(bitmat);
  imprimir_bit_mat(b2);

  Bit_Mat_Graph<Grafo> t = b1 * b2;
  imprimir_bit_mat(t);
} 
