
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

# ifndef KOSARAJU_H
# define KOSARAJU_H

# include <tpl_graph_utils.H>

namespace Aleph {

     template <class GT> inline static
void __dfp(const GT & g, typename GT::Node * p, 
	   DynArray<typename GT::Node*> & df)
{
  if (IS_NODE_VISITED(p, Depth_First))
    return;

  NODE_BITS(p).set_bit(Depth_First, true); 

      // recorre en profundidad los arcos de p 
  for (auto it = g.get_out_it(p); it.has_current(); it.next())
    {
      auto a = it.get_current_arc();
      if (IS_ARC_VISITED(a, Depth_First))
        continue;

      ARC_BITS(a).set_bit(Depth_First, true); 

      __dfp(g, it.get_tgt_node(), df);
    }
  df.append(p); // asignación de df(p) en sufijo
  NODE_COUNTER(p) = df.size();
}

    /* Recorre el grafo invertido e inserta los nodos alcanzables en el
       sub-grafo blk. A cada bloque le coloca el color color

     */
     template <class GT> inline static
void __dfp(const GT & g, typename GT::Node * p, GT & blk, const int & color)
{
  if (IS_NODE_VISITED(p, Depth_First))
    return;

  NODE_BITS(p).set_bit(Depth_First, true); 

  auto q = blk.insert_node(p->get_info());
  NODE_COUNTER(q) = color;
  GT::map_nodes(p, q);

  for (auto it = g.get_out_it(p); it.has_current(); it.next())
    {
      auto a = it.get_current_arc();
      if (IS_ARC_VISITED(a, Depth_First))
        continue;
      ARC_BITS(a).set_bit(Depth_First, true); 

      __dfp(g, it.get_tgt_node(), blk, color);
    }
}

     template <class GT> inline 
void kosaraju_connected_components(const GT &                     g,
                                   DynDlist <GT> &                blk_list,
                                   DynDlist<typename GT::Arc *> & arc_list)
{
  g.reset_nodes();
  g.reset_arcs();

  DynArray<typename GT::Node*> df; // arreglo de df por sufijo

      // recorre en profundidad y marca en sufijo los nodos
  for (auto it = g.get_node_it(); it.has_current() and df.size() < g.vsize();
       it.next())
    __dfp(g, it.get_current(), df);

  GT gi = invert_digraph(g); // gi es el grafo invertido de g

  DynArray<GT*> array; // areglo de sub-grafos

  for (int i = df.size() - 1, color = 0; i >= 0; i--)
    {
      auto gp = df.access(i);
      auto bp = mapped_node<GT>(gp);
      if (IS_NODE_VISITED(bp, Depth_First))
        continue;
      
      GT & blk = blk_list.append(GT());
      array[color] = &blk;
      
      __dfp(gi, bp, blk, color++); // recorre el grafo inverso 
                                            // inserta los nodos en blk 
      assert(NODE_COLOR(mapped_node<GT>(bp)) == color - 1);
    }

  for (auto it = g.get_arc_it(); it.has_current(); it.next())
    {
      auto a  = it.get_current();
      auto gs = g.get_src_node(a);
      auto gt = g.get_tgt_node(a);

      auto bs = mapped_node<GT>(mapped_node<GT>(gs));
      auto bt = mapped_node<GT>(mapped_node<GT>(gt));

      const long & color = NODE_COLOR(bs);

      if (color == NODE_COLOR(bt))
        {
          typename GT::Arc * ba = array.access(color)->insert_arc(bs, bt);
          GT::map_arcs(a, ba);
        }
      else 
        arc_list.append(a); 
    }
} 


    /* Recorre el grafo invertido e inserta los nodos alcanzables en la
       lista list. A cada nodo le coloca el color color

     */
     template <class GT> inline static
void __dfp(const GT & g, 
	   typename GT::Node * p, 
	   DynDlist<typename GT::Node *> & list)
{
  if (IS_NODE_VISITED(p, Depth_First))
    return;

  NODE_BITS(p).set_bit(Depth_First, true); 

  list.append(mapped_node<GT>(p));

  for (auto it = g.get_out_it(p); it.has_current(); it.next())
    {
      auto a = it.get_current_arc();
      if (IS_ARC_VISITED(a, Depth_First))
        continue;
      ARC_BITS(a).set_bit(Depth_First, true); 

      __dfp(g, it.get_tgt_node(), list);
    }
}

template <class GT> inline 
DynDlist<DynDlist<typename GT::Node*>> 
kosaraju_connected_components(const GT & g)
{
  DynDlist<DynDlist<typename GT::Node*>> list;

  g.reset_nodes();
  g.reset_arcs();
  DynArray<typename GT::Node*> df; // arreglo de df por sufijo

      // recorre en profundidad y marca en sufijo los nodos
  for (auto it = g.get_node_it(); it.has_curr() and df.size() < g.vsize(); 
       it.next())
    __dfp(g, it.get_current(), df);

  GT gi = invert_digraph(g); // gi es el grafo invertido de g

  for (int i = df.size() - 1; i >= 0; i--)
    {
      auto gp = df.access(i);
      auto bp = mapped_node<GT>(gp);
      if (IS_NODE_VISITED(bp, Depth_First))
        continue;

      auto & blk = list.append(DynDlist<typename GT::Node*>());
      
      __dfp(gi, bp, blk);
    }

  return list;
} 



} // end namespace Aleph

# endif // KOSARAJU_H
