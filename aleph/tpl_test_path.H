
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

# ifndef TEST_PATH_H
# define TEST_PATH_H

# include <tpl_graph.H>

namespace Aleph {


      template <class GT, class SA = Dft_Show_Arc<GT> > 
class Test_For_Path
{
  SA & sa;
  typename GT::Node * tgt;

  bool test_path(typename GT::Node * curr)  
  {
    if (curr == tgt) 
      return true; // se alcanzó a tgt

    if (IS_NODE_VISITED(curr, Find_Path)) // ¿se visitó curr_node?
      return false; // sí, no explore

    NODE_BITS(curr).set_bit(Find_Path, true); // pintar curr_node

    // buscar recursivamente a través de arcos de curr 
    for (Node_Arc_Iterator<GT, SA> i(curr, sa); i.has_current(); i.next())
      { 
        typename GT::Arc * arc = i.get_current_arc();
        if (IS_ARC_VISITED(arc, Find_Path)) 
          continue; 

        ARC_BITS(arc).set_bit(Find_Path, true); // pintar arco
        if (test_path(i.get_tgt_node())) 
          return true;
      }

    // todos los arcos adyacentes de curr_node explorados sin
    // encontrar a end_node ==> no existe camino por curr_node
    return false;
  }

  bool test_path(const GT & g, typename GT::Node * src, typename GT::Node * dest)
  {      // si el grafo es conexo ==> existe camino
    if (not g.is_digraph() and g.get_num_arcs() >= g.get_num_nodes()) 
      return true;

    g.reset_bit_nodes(Find_Path); 
    g.reset_bit_arcs(Find_Path);

    tgt = dest;

        // buscar recursivamente por arcos adyacentes a src
    for (Node_Arc_Iterator<GT, SA> i(src, sa); i.has_current(); i.next())
      {
        typename GT::Arc * arc = i.get_current_arc();
        ARC_BITS(arc).set_bit(Find_Path, true); // marcar arco
        if (test_path(i.get_tgt_node())) 
          return true; 
      }

    // todos los arcos de start_node han sido explorados sin 
    // encontrar camino hasta end_node ==> no existe camino
    return false;
  }

public:

  Test_For_Path(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Test_For_Path(SA & __sa) : sa(__sa) { /* empty */ }

  bool operator () (const GT& g, typename GT::Node * start_node, 
		    typename GT::Node * end_node) 
  {
    return test_path(g, start_node, end_node);
  }
};



} // end namespace Aleph

# endif // TEST_PAT_H
