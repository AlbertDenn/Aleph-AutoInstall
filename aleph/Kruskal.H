
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

# ifndef KRUSKAL_H
# define KRUSKAL_H

# include <ahFunction.H>
# include <tpl_agraph.H>
# include <tpl_graph_utils.H>
# include <tpl_test_acyclique.H>
# include <tpl_union.H>

using namespace Aleph;

namespace Aleph {

  
 template <class GT, 
	   class Distance = Dft_Dist<GT>, 
	   class SA       = Dft_Show_Arc<GT>> 
class Kruskal_Min_Spanning_Tree 
{
  Distance & dist;
  SA      &  sa;
  bool       painted;
  
public:

  Kruskal_Min_Spanning_Tree(Distance && __dist = Distance(),  
			    SA &&       __sa   = SA()) 
  noexcept(std::is_nothrow_move_assignable<Distance>::value and
	   std::is_nothrow_move_assignable<SA>::value)
    : dist(__dist), sa(__sa), painted(false)
  { 
    /* empty */ 
  }

  Kruskal_Min_Spanning_Tree(Distance & __dist, SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<Distance>::value and
	   std::is_nothrow_copy_assignable<SA>::value)
    : dist(__dist), sa(__sa), painted(false)
  { 
    /* empty */ 
  }

public:

      template <class G, class GT_SA>
  struct Paint_Filt
  {
    GT_SA & sa;

    Paint_Filt(GT_SA & __sa) 
    noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

    bool operator () (typename G::Arc * a) const noexcept
    {
      if (not sa(a))
	return false;

      return IS_ARC_VISITED(a, Aleph::Spanning_Tree);
    }
  };
 
private:

  struct Init_Node
  {
    long count;

    Init_Node() noexcept : count(0) { /* empty */ }

    void operator () (const GT &, typename GT::Node * p) noexcept
    {
      NODE_COUNTER(p) = count++;
      NODE_BITS(p).set_bit(Aleph::Spanning_Tree, false);
    }
  };

  static bool arc_is_in_tree(Fixed_Relation & tree, long i, long j) noexcept
  {
    return tree.are_connected(i, j);
  }

public:

  void paint_min_spanning_tree(const GT & g)
  {
    if (g.is_digraph())
      throw std::domain_error("g is a digraph");

    g.reset_bit_arcs(Aleph::Spanning_Tree); // limpiar bits de marcado de arcos
    Operate_On_Nodes<GT, Init_Node> () (g, Init_Node());
    
    typedef Distance_Compare<GT, Distance> DCMP;
    DCMP comp(dist);
    const_cast<GT&>(g).template sort_arcs<DCMP>(comp);
    const size_t V = g.get_num_nodes();

    Fixed_Relation tree(V);

    // Recorrer arcos ordenados de g hasta que numero de arcos de
    // tree sea igual al numero de nodos de g 
    for (Arc_Iterator <GT, SA> it(g, sa); tree.get_num_blocks() > 1 and
	   it.has_curr(); it.next()) 
      {     // siguiente menor arco
	auto arc = it.get_current_arc();
	long i = NODE_COUNTER(g.get_src_node(arc));
	long j = NODE_COUNTER(g.get_tgt_node(arc));
	if (arc_is_in_tree(tree, i, j))
	  continue;
	
	tree.join(i, j);
	ARC_BITS(arc).set_bit(Aleph::Spanning_Tree, true);
      }

    painted = true;
  }

  void paint_min_spanning_tree(const GT & g, GT & tree)
  {
    paint_min_spanning_tree(g);
    clear_graph(tree); // limpia grafo destino 

    for (typename GT::Node_Iterator it(g); it.has_curr(); it.next())
      {
	auto gp = it.get_curr();
	auto tp = tree.insert_node(gp->get_info());
	GT::map_nodes(gp, tp);
      }

    typedef Paint_Filt<GT, SA> F;
    for (Arc_Iterator<GT, F> it(g, F(sa)); it.has_curr(); it.next()) 
      {
	auto ga    = it.get_current_arc();
	auto tsrc = mapped_node<GT>(g.get_src_node(ga));
	auto ttgt = mapped_node<GT>(g.get_tgt_node(ga));
	auto ta   = tree.insert_arc(tsrc, ttgt, ga->get_info());
	GT::map_arcs(ga, ta);
      }
  }

public:

  void operator () (const GT & g, GT & tree) 
  {
    paint_min_spanning_tree(g, tree);
  }

  void operator () (const GT & g) 
  {
    paint_min_spanning_tree(g);
  }
};
      

} // end namespace Aleph

# endif // KRUSKAL_H
