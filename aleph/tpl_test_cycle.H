
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

# ifndef TPL_TEST_CYCLE_H
# define TPL_TEST_CYCLE_H

namespace Aleph {


      template <class GT, class SA = Dft_Show_Arc<GT>> 
class Test_For_Cycle
{
  typename GT::Node * src;
  SA & sa;

  bool test_cycle(typename GT::Node * curr)
  {
    if (src == curr) 
      return true; // ciclo detectado!

    if (IS_NODE_VISITED(curr, Test_Cycle)) 
      return false;

    NODE_BITS(curr).set_bit(Test_Cycle, true); // marque nodo

      // buscar caminos desde current_node a ver si llega a src_node 
    for (Node_Arc_Iterator<GT, SA> it(curr, sa); it.has_current(); it.next())
      {
        typename GT::Arc * arc = it.get_current_arc();
        if (IS_ARC_VISITED(arc, Test_Cycle)) 
          continue;

        ARC_BITS(arc).set_bit(Test_Cycle, true); // marque arco

        if (test_cycle(it.get_tgt_node()))
          return true; // ciclo encontrado desde el arco actual 
      }

        // En este punto se han explorado caminos desde curr_node
        // sin encontrar src_node ==> no existe ciclo por curr_node  
    return false; 
  }  

  bool test_cycle(GT & g, typename GT::Node * s)
  {
    src = s;

    g.reset_bit_nodes(Test_Cycle); // reiniciar Test_Cycle para nodos
    g.reset_bit_arcs(Test_Cycle);  // reiniciar Test_Cycle para arcos

      // explorar recursivamente por arcos adyacentes a src_node
    for (Node_Arc_Iterator<GT, SA> it(src, sa); it.has_curr(); it.next())
      {
        typename GT::Arc * arc = it.get_current_arc();
        if (IS_ARC_VISITED(arc, Test_Cycle)) 
          continue;

        ARC_BITS(arc).set_bit(Test_Cycle, true); // pintar arco

        if (test_cycle(it.get_tgt_node()))
          return true; // ciclo detectado
      }

        // Se han explorado todos los caminos desde src_node 
        // sin encontrar de nuevo a src_node ==> no hay ciclo 
    return false;
  }

public:

  Test_For_Cycle(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Test_For_Cycle(SA & __sa) : sa(__sa) { /* empty */ }

  bool operator () (GT & g, typename GT::Node * src) 
  {
    return this->test_cycle(g, src);
  }
};

} // end namespace Aleph


# endif //  TPL_TEST_CYCLE_H
