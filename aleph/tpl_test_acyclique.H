
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

# ifndef TPL_TEST_ACYCLIQUE_H
# define TPL_TEST_ACYCLIQUE_H

namespace Aleph {



      template <class GT, class SA = Dft_Show_Arc<GT>> 
class Is_Graph_Acyclique
{
  SA & sa;

  bool is_acyclique(typename GT::Node * curr)
  {
    if (IS_NODE_VISITED(curr, Test_Cycle)) 
      return false;

    NODE_BITS(curr).set_bit(Test_Cycle, true); // marcar nodo

    for (Node_Arc_Iterator<GT, SA> i(curr, sa); i.has_current(); i.next())
      {
        typename GT::Arc * arc = i.get_current_arc();
        if (IS_ARC_VISITED(arc, Test_Cycle)) 
          continue; 

        ARC_BITS(arc).set_bit(Test_Cycle, true); 

        if (not is_acyclique(i.get_tgt_node())) 
          return false;
      }

    // todos los arcos recorridos sin encontrar ciclo ==>
    // el grafo es acíclico pasando por curr_node
    return true; 
  }

  bool is_acyclique(GT & g, size_t num_arcs)
  {
    if (g.is_digraph())
      throw std::domain_error("is_graph_acyclique() does not work for digraps");

    if (num_arcs >= g.get_num_nodes()) 
      return false; 

    g.reset_bit_arcs(Test_Cycle);
    g.reset_bit_nodes(Test_Cycle);

    for (typename GT::Node_Iterator it(g); it.has_current(); it.next()) 
    {
      typename GT::Node * curr = it.get_current_node();
      if (IS_NODE_VISITED(curr, Test_Cycle)) 
	continue; 

      if (not is_acyclique(curr)) 
	return false;
    }

  return true;
}

public:
  
  Is_Graph_Acyclique(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Is_Graph_Acyclique(SA & __sa) : sa(__sa) { /* empty */ }

  bool operator () (GT & g, size_t num_arcs) 
  {
    return is_acyclique(g, num_arcs);
  }

  bool operator () (GT & g) 
  {
    return is_acyclique(g, g.get_num_arcs());
  }
};

      template <class GT, class SA = Dft_Show_Arc<GT>> 
class Has_Cycle
{
  SA & sa;

public:

  Has_Cycle(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Has_Cycle(SA & __sa) : sa(__sa) { /* empty */ }

  bool operator () (GT & g) const
  {
    return not Is_Graph_Acyclique <GT, SA> (sa) (g);
  }

  bool operator () (GT & g, size_t num_arcs) const
  {
    return not Is_Graph_Acyclique <GT, SA> (sa) (g, num_arcs);
  }
};




} // end namespace Aleph

# endif //  TPL_TEST_ACYCLIQUE_H
