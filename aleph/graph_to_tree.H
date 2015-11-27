
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

# ifndef GRAPH_TO_TREE_H
# define GRAPH_TO_TREE_H

# include <tpl_tree_node.H>
# include <tpl_graph.H>
# include <tpl_graph_utils.H>

namespace Aleph {

    template <class GT, typename Key, class Convert> static
Tree_Node<Key> * graph_to_tree_node(GT & g, typename GT::Node * groot);
    template <class GT, typename Key, class Convert> static void 
__graph_to_tree_node(GT & g, typename GT::Node * groot, 
                     Tree_Node<Key> * troot);

  template <class GT, typename Key, typename Convert, class SA> static inline
void __graph_to_tree_node(GT & g, typename GT::Node * groot,
                          Tree_Node<Key> * troot);

    template <class GT, typename Key, 
              class Convert, class SA = Dft_Show_Arc<GT> > inline
Tree_Node<Key> * graph_to_tree_node(GT & g, typename GT::Node * groot)
{ 
  if (not is_graph_acyclique<GT>(g))
    throw std::domain_error("Graph is not a tree (not acyclique)");

  Tree_Node<Key> * troot = new Tree_Node<Key>; // apartar memoria raíz

  Convert () (groot, troot); //convertir de groot y copiar a troot

  __graph_to_tree_node <GT, Key, Convert, SA> (g, groot, troot);

  return troot;
}

    template <class GT, typename Key, 
	      class Convert, 
	      class SA = Dft_Show_Arc<GT> > 
class Graph_To_Tree_Node
{
  SA &      sa;
  Convert * convert;

  void graph_to_tree(typename GT::Node * groot,
			  Tree_Node<Key> * troot)
  {           
    typedef typename GT::Node Node;
    typedef typename GT::Arc Arc;

        // recorrer arcos de groot y construir recursivamente
    for (Node_Arc_Iterator<GT, SA> it(groot, sa); it.has_curr(); it.next())
      {
	Arc * arc = it.get_current_arc();
	if (IS_ARC_VISITED(arc, Convert_Tree)) 
	  continue;

	ARC_BITS(arc).set_bit(Convert_Tree, true); // arc visitado 
	Node * gtgt = it.get_tgt_node();
	Tree_Node<Key> * ttgt = new Tree_Node<Key>; 

	(*convert) (gtgt, ttgt); // asignarle la clave

	troot->insert_rightmost_child(ttgt); // insertarlo como hijo 

	graph_to_tree(gtgt, ttgt);
      }
  }

  Tree_Node<Key> * graph_to_tree(GT & g,
				 typename GT::Node * groot,
				 Convert & conv)
  { 
    if (not is_graph_acyclique<GT>(g))
      throw std::domain_error("Graph is not a tree (not acyclique)");

    Tree_Node<Key> * troot = new Tree_Node<Key>; // apartar memoria raíz

    convert = &conv;

    (*convert) (groot, troot); //convertir de groot y copiar a troot

    graph_to_tree(groot, troot);

    return troot;
  }

public:

  Graph_To_Tree_Node(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Graph_To_Tree_Node(SA & __sa) : sa(__sa) { /* empty */ }

      Tree_Node<Key> * 
  operator () (GT & g, typename GT::Node * groot, Convert && conv = Convert())
  { 
    return graph_to_tree(g, groot, conv);
  }

      Tree_Node<Key> * 
  operator () (GT & g, typename GT::Node * groot, Convert & conv)
  { 
    return graph_to_tree(g, groot, conv);
  }
};


  template <class GT, typename Key, typename Convert, class SA> static inline
void __graph_to_tree_node(GT & g, typename GT::Node * groot,
                          Tree_Node<Key> * troot)
{           
  typedef typename GT::Node Node;
  typedef typename GT::Arc Arc;

      // recorrer arcos de groot y construir recursivamente
  for (Node_Arc_Iterator<GT, SA> it(groot); it.has_current(); it.next())
    {
      Arc * arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Convert_Tree)) 
        continue;

      ARC_BITS(arc).set_bit(Convert_Tree, true); // arc visitado 
      Node * gtgt = it.get_tgt_node();
      Tree_Node<Key> * ttgt = new Tree_Node<Key>; 

      Convert () (gtgt, ttgt); // asignarle la clave

      troot->insert_rightmost_child(ttgt); // insertarlo como hijo 

      __graph_to_tree_node <GT, Key, Convert, SA> (g, gtgt, ttgt);
    }
}

} // end namespace Aleph

# endif // GRAPH_TO_TREE_H
