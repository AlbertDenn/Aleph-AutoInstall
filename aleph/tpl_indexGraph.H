
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

# ifndef TPL_INDEXGRAPH
# define TPL_INDEXGRAPH

# include <tpl_indexNode.H>
# include <tpl_indexArc.H>

namespace Aleph
{

template 
<class GT, 
 class Compare = Dft_Node_Cmp<GT>,
 template <class /* Key */, class /* Compare */> class Tree = Treap>
class Index_Graph
{
private:

  typedef typename GT::Arc GT_Arc;
  typedef typename GT::Node GT_Node;
  typedef typename GT::Arc_Type GT_Arc_Type;
  typedef typename GT::Node_Type GT_Node_Type;

  IndexNode<GT, Compare, Tree> idx_node;
  IndexArc<GT, Tree>           idx_arc;

public:

  Index_Graph(GT & g) : idx_node(g), idx_arc(g)
  {
    // empty
  }

  GT_Node * insert_node(const GT_Node_Type & info)
  {
    return idx_node.insert_in_graph(info);
  }

  GT_Arc * insert_arc(GT_Node * src, GT_Node * tgt, 
		      const GT_Arc_Type & info = GT_Arc_Type())
    throw (std::exception, std::domain_error)
  {
    if (idx_node.search(src) == nullptr)
      throw std::domain_error("src node not in index");

    if (idx_node.search(tgt) == nullptr)
      throw std::domain_error("tgt node not in index");

    return idx_arc.insert_in_graph(src, tgt, info);
  }

  GT_Node * search_node(GT_Node * p)
  {
    return idx_node.search(p);
  } 

  GT_Node * search_node(const GT_Node_Type & info)
  {
    return idx_node.search(info);
  }

  GT_Arc * search_arc(GT_Node * src, GT_Node * tgt)
  {
    return idx_arc.search(src, tgt);
  }

  void remove_node(GT_Node * p)
  {
        // eliminar del índice los arcos emanantes del nodo (ellos serán
        // eliminados por la eliminación de nodo del grafo)
    for (typename GT::Node_Arc_Iterator it(p); it.has_current(); it.next())
      idx_arc.remove(it.get_current());

    return idx_node.remove_from_graph(p); 
  }

  void remove_arc(GT_Arc * a)
  {
    return idx_arc.remove_from_graph(a);
  }

  size_t get_num_arcs() const { return idx_arc.size(); }

  size_t get_num_nodes() const { return idx_node.size(); }
};

template <class GT> inline
bool are_equal(const GT & g1, const GT & g2)
{
  if (g1.vsize() != g2.vsize() or g1.esize() != g2.esize())
    return false;

  {
    IndexNode<GT> t2(const_cast<GT&>(g2));
    if (not g1.all_nodes([&t2] (auto p) 
			 {
			   auto q = t2.search(p);
			   if (q == nullptr)
			     return false;
			   GT::map_nodes(p, q);
			   return true;
			 }))
      return false;
  }
  
  IndexArc<GT> t2(const_cast<GT&>(g2));
  return g1.all_arcs([idx = &t2, &g1] (auto a) 
		     {
		       auto s1 = g1.get_src_node(a);
		       auto t1 = g1.get_tgt_node(a);
		       auto s2 = mapped_node<GT>(s1);
		       auto t2 = mapped_node<GT>(t1);
		       auto a2 = idx->search(s2, t2);
		       if (a2 == nullptr)
			 return false;
		       return a2->get_info() == a->get_info();
		     });
}



}

# endif // TPL_INDEXGRAPH


