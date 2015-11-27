
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

# ifndef TPL_INDEXARC_H
# define TPL_INDEXARC_H

# include <tpl_dynSetTree.H>
# include <tpl_graph.H>

using namespace Aleph;

namespace Aleph
{


  template <
    class GT,
    template <class /* Key */, class /* Compare */> class Tree = Rand_Tree,
    class SA = Dft_Show_Arc<GT>
    >
class IndexArc
{
private:
			
  typedef typename GT::Arc GT_Arc;
  typedef typename GT::Node GT_Node;
  typedef typename GT::Arc_Type GT_Arc_Type;
  typedef typename GT::Node_Type GT_Node_Type;

  typedef std::pair<void*, void*> Node_Pair;

  struct Cmp_Arc
  {
    bool operator () (GT_Arc * a1, GT_Arc * a2) const
    {
      if (a1->src_node < a2->src_node)
	return true;

      return not (a2->src_node < a1->src_node) and a1->tgt_node < a2->tgt_node;
    }
  };

  GT & g;
  DynSetTree <GT_Arc *, Tree, Cmp_Arc> index;
  SA & sa;

public:

  GT_Arc * insert(GT_Arc * e)
  {
    return *index.put(e);
  }

  GT_Arc * search(void * src, void * tgt)
  {
    GT_Arc arc;
    arc.src_node = src; 
    arc.tgt_node = tgt;
    
    GT_Arc ** ret_val = index.search(&arc);

    if (ret_val != nullptr)
      return *ret_val;
 
    if (g.is_digraph())
      return nullptr;

    std::swap(arc.src_node, arc.tgt_node);
    ret_val = index.search(&arc);

    if (ret_val == nullptr)
      return nullptr;

    assert(((src == (*ret_val)->src_node) and (tgt == (*ret_val)->tgt_node)) or 
      ((tgt == (*ret_val)->src_node) and (src == (*ret_val)->tgt_node)));

    return *ret_val; 
  }

  GT_Arc * search_directed(void * src, void * tgt)
  {
    GT_Arc arc;
    arc.src_node = src; 
    arc.tgt_node = tgt;
    
    GT_Arc ** ret_val = index.search(&arc);
    
    return ret_val != nullptr ? *ret_val : nullptr;
  }

  GT_Arc * search(GT_Arc * a)
  {
    return search(a->src_node, a->tgt_node);
  }

  GT_Arc * insert_in_graph(GT_Node * src, GT_Node * tgt, 
			   const GT_Arc_Type & info)
    throw (std::exception, std::domain_error)
  {
    GT_Arc * a = search(src, tgt);

    if (a != nullptr)
      throw std::domain_error("There is already in arc between these nodes");

    a = g.insert_arc(src, tgt, info);
    insert(a);
    
    return a;
  }

  GT_Arc * insert_in_graph(GT_Node * src, GT_Node * tgt, 
			   GT_Arc_Type && info = GT_Arc_Type())
    throw (std::exception, std::domain_error)
  {
    GT_Arc * a = search(src, tgt);

    if (a != nullptr)
      throw std::domain_error("There is already in arc between these nodes");

    a = g.insert_arc(src, tgt, std::forward<GT_Arc_Type>(info));
    insert(a);
    
    return a;
  }

  void remove(GT_Arc * e)
  {
    index.remove(e);
  }

  void remove_from_graph(GT_Arc * a)
  {
    remove(a);
    g.remove_arc(a);
  }

  void clear_index()
  {
    index.empty();
  }

  void build_index()
  {
    for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
      {
	GT_Arc * a = it.get_curr();

	if (search(a) != a)
	  insert(a);
      }
  }

private:

  void init()
  {
    for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
      insert(it.get_curr());
  }

public:

  IndexArc(GT & __g, bool with_init = true, SA && __sa = SA()) 
    : g(__g), sa(__sa)
  {
    if (with_init)
      init();
  }

  IndexArc(GT & __g, bool with_init, SA & __sa) : g(__g), sa(__sa)
  {
    if (with_init)
      init();
  }

  size_t size() const { return index.size(); }
};

}

# endif

