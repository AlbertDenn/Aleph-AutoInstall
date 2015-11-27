
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


# include <iostream>
# include <string>
# include <stdexcept>
# include <tpl_tree_node.H>
# include <tpl_dynArray.H>

using namespace std;

void print_node(Tree_Node<int> * node, int level, int index)
{
  cout << node->get_data() << " level = " << level << " index = " 
       << index << endl;
}


bool is_string_an_int(const string& str)
{
  string::size_type len = str.size();

  for (string::size_type i = 0; i < len; i++)
    if (not isdigit(str[i]))
      return false;

  return true;
}


int * string_to_deway(const string& str, size_t & size) 
  throw (invalid_argument, bad_alloc)
{
  DynArray<int> a;

  const string delimiters(".");
  string::size_type begin_index, end_index, number_str_len;

  begin_index = str.find_first_not_of(delimiters);
  while (begin_index not_eq string::npos)
    {
      end_index = str.find_first_of(delimiters, begin_index);

      number_str_len = end_index - begin_index;

      const string number_str = 
	str.substr(begin_index, number_str_len).c_str();

      if (not is_string_an_int(number_str))
	throw invalid_argument("character is not a digit");
      
      a[a.size()] = atoi(number_str.c_str());

      begin_index = str.find_first_not_of(delimiters, end_index);
    }

  int * ret_val = new int [a.size() + 1];

  for (int i = 0; i < a.size(); i++)
    ret_val[i] = a[i];
  ret_val[a.size()] = -1;

  size = a.size();

  return ret_val;
}


int main()
{
  int number;
  cout << "Ingrese el valor de la raiz: ";
  cin >> number;

  Tree_Node<int> * root = new Tree_Node<int> (number);

  cout << "Ingreso de nodos del arbol (deway luego numero) (letra termina)" 
       << endl;

  while (true)
    {
      try
	{
	  cout << "    Numero de Deway del padre = ";
	  string deway_string;
	  size_t deway_size;
	  cin >> deway_string;
	  int * deway = string_to_deway(deway_string, deway_size);

	  Tree_Node<int> * node = deway_search(root, deway, deway_size);

	  if (node == NULL)
	    {
	      cout << "No existe el nodo " << deway_string << endl;
	      continue;
	    }

	  cout << "    Insercion de hijo derecho en " << deway_string 
	       << " - " << node->get_data() << endl
	       << "    Clave del nuevo nodo = ";

	  int key;
	  cin >> key;
	  Tree_Node<int> * p = new Tree_Node<int> (key);
	  node->insert_rightmost_child(p);

	  //	  assert(check_tree(root));

	  delete deway;
	  
	  cout << endl;
	}
      catch (invalid_argument & e)
	{
	  cout << e.what() << endl
	       << "Terminando " << endl;
	  break;
	}      
      catch (bad_alloc)
	{
	  cout << "No hay memoria";
	}
    }

  cout << "    preorder" << endl;
  tree_preorder_traversal(root, &print_node);

  cout << "    postorder" << endl;
  tree_postorder_traversal(root, &print_node);

  auto aux = clone_tree(root);

  assert(are_tree_equal(root, aux));

  destroy_tree(root);
  destroy_tree(aux);
}
