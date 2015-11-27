
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


# include <now.h>
# include <ahPair.H>
# include <tpl_dnode.H>
# include <tpl_dynSetTree.H>


# include <fstream>
# include <iostream>



// Renombro el tipo lista por comodidad
typedef Dnode<int> Int_List;

/* Es recomendable, al querer consultar el nodo de una lista en
   particular, declararlo de la forma Tipo_Lista::Node de modo que si en
   algun momento cambia la plantilla de la lista, los nodos se mantienen
   fieles al tipo de la lista 
*/

// Por comodidad tambien renombro el tipo de nodo
typedef Dnode<int> Int_Node;

    // Cuenta las ocurrencias de number en el arreglo a
int count_num(DynArray<short> & a, short number)
{
  int count = 0;

  for (int i = 0, n = a.size(); i < n; ++i)
    if (a.access(i) == number)
      ++count;

  return count;
}

Aleph::pair<int, int> mode(DynArray<short> & a)
{
  int max_freq = 0;
  Aleph::pair<int, int> ret_val = -1;

  for (int i = 0, n = a.size(); i < n; ++i)
    {
      short num = a.access(i);
      int freq = count_num(a, num);

      if (freq > max_freq)
        {
          max_freq = freq;
	  ret_val = Aleph::pair<int, int>(num, freq);
        }
    }

  return ret_val;
}

// Funcion que lee los valores del archivo, los carga en la secuencia y calcula la moda
Aleph::pair<int, int> seq_mode(std::ifstream & file)
{
  DynArray<short> a(6, 7, 19);

  while (not file.eof())
    {
      short n;
      file >> n;

      if (not file.eof())
        a.append(n);
    }

  return mode(a);
}



// Clave para guardar en el arbol
struct Number_With_Frequency
{
  int n;
  int f;

  Number_With_Frequency() : f(0)
  {
    // Empty
  }

  Number_With_Frequency(int _n) : n(_n), f(0)
  {
    // Empty
  }
};

/*
  Criterio de comparacion para el arbol

  Permite definir hacia que lado de la raiz del arbol se van los menores
  y los mayores y tratar repitencia o no de los valores.
  
  Con este criterio las claves que tengan un valor de n menores se van
  por la izquierda y los mayores por la derecha no permite valores
  repetidos 
*/
struct Cmp
{
  bool operator () (const Number_With_Frequency & e1, 
		    const Number_With_Frequency & e2) const
  {
    return e1.n < e2.n;
  }
};

/*
  La operacion for_each de la clase DynSetTree itera sobre todos los
  nodos del arbol en cada nodo que pasa ejecuta una operacion, la cual
  debe ser definida por el usuario 
  
  El prototipo de la operacion se define con el operador () y debe
  recibir como parametros una referencia a la clave visitada y un
  puntero a void (lo que permite pasar un puntero a cualquier tipo de
  dato y luego hacerle la conversion al tipo deseado y efectuar la
  operacion, podria pasarse un NULL si no se desea pasar ese parametro. 
*/
struct Op
{
  Number_With_Frequency n;
  Op() : n(0) { /* empty */ }

  /*
    En esta operacion el parametro ptr va a contener un puntero a un
    objeto de tipo Number_With_Frequency que tiene almacenado el n y f
    del numero con maxima frecuencia leido en un instante de la
    iteracion. 
  */
  void operator () (Number_With_Frequency & nf)
  {
     // Si la frecuencia actual es mayor a la que se consideraba mayor,
     // la actual pasa a ser la mayor 
     if (nf.f > n.f)
       {
         n.f = nf.f;
         n.n = nf.n;
       } 
  }
};

// Funcion que lee los numeros del archivo, los carga en un arbol y
// calcula la moda 
    template <template <class, class> class Tree>
Aleph::pair<int, int> tree_mode(std::ifstream & file)
{
  DynSetTree<Number_With_Frequency, Tree, Cmp> tree;

  while (not file.eof())
    {
      int n; 
      file >> n;
      if (not file.eof())
        {
          Number_With_Frequency nf(n);
          Number_With_Frequency * __nf = tree.search_or_insert(nf);
	  ++__nf->f; // incremento frecuencia
        }
    }

  Op op;
  tree. template for_each_inorder <Op> (op);

  // Al salir del for_each _nf tiene el n y el f del numero con mayor frecuencia

  return Aleph::pair<int, int> (op.n.n, op.n.f);
}


int main(int argc, char * argv[])
{
  if (argc < 3)
    {
      std::cout << "Faltan argumentos" << std::endl;
      std::cout << "Debe ser, ./test_mode input_file algoritmo" << std::endl;
      std::cout << "El algoritmo es un numero" << std::endl;
      std::cout << "1 para el algoritmo basado en secuencias" << std::endl;
      std::cout << "2 para el algoritmo basado en arbol" << std::endl;
      return 1;
    }

  std::ifstream file(argv[1]);

  if (not file)
    {
      std::cout << "No se puede abrir el archivo" << argv[1] << std::endl;
      return 2;
    }

  /*
    Declaro un puntero a funcion de modo que pueda ejecutar cualquiera de las dos formas
    de calcular la moda
  */

  Aleph::pair<int, int> (*mode_algorithm)(std::ifstream &);

  int algorithm = atoi(argv[2]);

  switch (algorithm)
    {
    case 1:
      mode_algorithm = &seq_mode;
      break;
    case 2:
      mode_algorithm = &tree_mode<Avl_Tree>;
      break;
    default:
      std::cout << "Valor invalido para algoritmo de moda" << std::endl;
      std::cout << "El algoritmo es un numero" << std::endl;
      std::cout << "1 para el algoritmo basado en secuencias" << std::endl;
      std::cout << "2 para el algoritmo basado en arbol" << std::endl;
      file.close();
      return 3;
    }

  Aleph::pair<int, int> result = mode_algorithm(file);

  std::cout << "La moda es: " << result.first << " con " << result.second
	    << " ocurrencias" << endl;

  file.close();
}

