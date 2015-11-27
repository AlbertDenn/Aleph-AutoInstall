
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

# ifndef GENERATE_TREE_H
# define GENERATE_TREE_H

# include <fstream>
# include <tpl_tree_node.H>
# include <tpl_binNodeUtils.H>

namespace Aleph
{

  template <class Node>
  struct Dft_Write
  {
    const string operator () (Node * p) 
    {
      return to_str(p->get_key());
    }
  };

/*
  Write debe exportar const string Write::operator () (Node *)

  Que se encarga de convertir el valor de clave contenido en el nodo a
  un string imprimible 
*/

    template <typename Node, class Write> static
void __generate_tree(Node *          node, 
		     int             deway [], 
		     const size_t &  current_level, 
		     const size_t &  size,
		     std::ostream & output)
{ 
  if (current_level >= size)
    throw std::overflow_error("Allocated size for deway array has been "
			      "exceeded");

      // imprimir número de Deway del padre
  output << "Node ";
  for (int i = 0; i < current_level; ++i)
    {
      output << deway[i];

      if (i < current_level - 1)
	output << ".";
    }
  output << " \"" << Write () (node) << "\" " << std::endl;

  Node * child = (Node*) node->get_left_child(); 
  for (int i = 0; child != nullptr; 
       i++, child = (Node*)child->get_right_sibling())
    {
      deway[current_level + 1] = i;
      __generate_tree<Node, Write>
	(child, deway, current_level + 1, size, output);
    }
}


const size_t Max_Tree_Node_Depth = 1024;

  template <typename Node, class Write = Dft_Write<Node>>
void generate_tree(Node *         root, 
		   std::ostream & out, 
		   const int &    tree_number = 0)
{                               
  out << "Root \"" << Write () (root) << "\" " << std::endl;

  int * deway = new int [Max_Tree_Node_Depth];

  const int level = 0; // Este es el nivel de partida

  deway[level] = tree_number; // nivel 0 en el número del árbol

  Node * child = (Node*) root->get_left_child(); 
  for (int i = 0; child != nullptr; 
       i++, child = (Node*)child->get_right_sibling())
    {
      deway[1] = i;
      __generate_tree<Node, Write>(child, deway, level + 1, 
				   Max_Tree_Node_Depth, out);
    }

  delete [] deway;
}

    template <typename Node, class Write = Dft_Write<Node>>
void generate_forest(Node * root, std::ostream & out)
{                               
  Node * tree = root; 

  for (int i = 0; tree != nullptr; i++, tree = tree->get_right_sibling())
    generate_tree<Node, Write>(tree, out, i);
}


    template <typename Node, class Write>
void generate_btree(Node * root, std::ostream & out)
{ 
  out << "start-prefix ";
  For_Each_Preorder<Node> () (root, Write());
  out << endl
      << "start_infix ";
  For_Each_In_Order<Node> () (root, Write());
  out << endl;
}

} // end namespace Aleph

# endif // GENERATE_TREE_H

