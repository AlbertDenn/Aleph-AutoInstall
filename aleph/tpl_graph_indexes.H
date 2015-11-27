
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

# ifndef TPL_GRAPH_INDEXES_H
# define TPL_GRAPH_INDEXES_H

# include <tpl_dynSetTree.H>
# include <tpl_graph.H>

using namespace Aleph;

namespace Aleph
{

template <class GT>
struct Dft_Node_Cmp
{
  bool operator () (typename GT::Node * p1, typename GT::Node * p2) const
  {
    return p1->get_info() < p2->get_info();
  }
};

template <class GT>
struct Dft_Arc_Cmp
{
  bool operator () (typename GT::Arc * a1, typename GT::Arc * a2) const
  {
    if (a1->src_node < a2->src_node)
      return true;

    return not (a2->src_node < a1->src_node) and a1->tgt_node < a2->tgt_node;
  }
};

template <
          class GT,
          class Compare = Dft_Node_Cmp<GT>,
          template <typename /* Key */, class /* Compare */> class Tree = Treap,
          class SN = Dft_Show_Node<GT>
         >
class Nodes_Index : public DynSetTree <typename GT::Node *, Tree, Compare>
{
  typedef typename GT::Node GT_Node;
  typedef typename GT::Node_Type GT_Node_Info;
  typedef DynSetTree <GT_Node *, Tree, Compare> Tree_Type;

  GT & g;
  SN & sn;

  void init()
  {
    for (Node_Iterator<GT, SN> it(g, sn); it.has_current(); it.next())
      this->insert(it.get_current());
  }

public:

  Nodes_Index(GT & _g, Compare & cmp, SN & _sn)
    : Tree_Type(cmp), g(_g), sn(_sn)
  {
    init();
  }

  Nodes_Index(GT & _g, Compare && cmp = Compare(), SN && _sn = SN())
    : Tree_Type(cmp), g(_g), sn(_sn)
  {
    init();
  }


  GT_Node * insert_in_graph(GT_Node * p)
  {
    g.insert_node(p);

    if (insert(p) == nullptr)
      {
        g.remove_node(p);
        return nullptr;
      }

    return p;
  }

  GT_Node * search_or_insert_in_graph(GT_Node * p)
  {
    g.insert_node(p);
    GT_Node ** ptr_node = search_or_insert(p);
    GT_Node * q = *ptr_node;

    if (p != q)
      g.remove_node(p);

    return q;
  }

  GT_Node * insert_in_graph(const GT_Node_Info & info)
  {
    GT_Node * p = g.insert_node(info);
    
    if (this->insert(p) == nullptr)
      {
        g.remove_node(p);
        return nullptr;
      }

    return p;
  }

  GT_Node * search_or_insert_in_graph(const GT_Node_Info & info)
  {
    GT_Node * p = g.insert_node(info);
    GT_Node ** ptr_node = search_or_insert(p);
    GT_Node * q = *ptr_node;

    if (p != q)
      g.remove_node(p);

    return q;
  }
  
  GT_Node * insert_in_graph(const GT_Node_Info && info = GT_Node_Info())
  {
    return insert_in_graph(info);
  }

  GT_Node * search(GT_Node * p)
  {
    GT_Node ** ret_val = Tree_Type::search(p);

    if (ret_val == nullptr)
      return nullptr;

    return *ret_val;
  }

  GT_Node * search(const GT_Node_Info & info)
  {
    GT_Node dummy_node(info);
    GT_Node * dummy_node_ptr = &dummy_node;

    return search(dummy_node_ptr);
  }

  void remove_from_graph(GT_Node * p) throw(std::exception, std::domain_error)
  {
    Tree_Type::find(p); // dispara excepciÃ³n si p no estÃ¡ en el Ã­ndice
    Tree_Type::remove(p);
    g.remove_node(p);
  }
};


template <
          class GT,
          class Compare = Dft_Arc_Cmp<GT>,
          template <typename /* Key */, class /* Compare */> class Tree = Treap,
          class SA = Dft_Show_Arc<GT>
         >
class Arcs_Index : public DynSetTree <typename GT::Arc *, Tree, Compare>
{
  typedef typename GT::Node GT_Node;
  typedef typename GT::Node_Type GT_Node_Info;
  typedef typename GT::Arc GT_Arc;
  typedef typename GT::Arc_Type GT_Arc_Info;
  typedef DynSetTree <typename GT::Arc *, Tree, Compare> Tree_Type;

  GT & g;
  SA & sa;

  void init()
  {
    for (Arc_Iterator<GT, SA> it(g, sa); it.has_current(); it.next())
      this->insert(it.get_current());
  }

public:
  Arcs_Index(GT & _g, Compare & cmp, SA & _sa)
    : Tree_Type(cmp), g(_g), sa(_sa)
  {
    init();
  }

  Arcs_Index(GT & _g, Compare && cmp = Compare(), SA && _sa = SA())
    : Tree_Type(cmp), g(_g), sa(_sa)
  {
    init();
  }

  GT_Arc * insert_in_graph(GT_Node * src, GT_Node * tgt,
                           const GT_Arc_Info & info)
  {
    GT_Arc * a = g.insert_arc(src, tgt, info);
    
    if (this->insert(a) == nullptr)
      {
        g.remove_arc(a);
        return nullptr;
      }

    return a;
  }

  GT_Arc * insert_in_graph(GT_Node * src, GT_Node * tgt,
                           const GT_Arc_Info && info = GT_Arc_Info())
  {
    return insert_in_graph(src, tgt, info);
  }

  GT_Arc * search(GT_Node * src, GT_Node * tgt, const GT_Arc_Info & info)
  {
    GT_Arc arc(info);
    arc.src_node = src; arc.tgt_node = tgt;
    GT_Arc * ptr_arc = &arc;
    
    GT_Arc ** ret_val = Tree_Type::search(ptr_arc);

    if (ret_val != nullptr)
      return *ret_val;
 
    if (g.is_digraph())
      return nullptr;

    std::swap(arc.src_node, arc.tgt_node);
    ret_val = Tree_Type::search(ptr_arc);
    if (ret_val == nullptr)
      return nullptr;

    assert(((src == (*ret_val)->src_node) and (tgt == (*ret_val)->tgt_node)) or 
      ((tgt == (*ret_val)->src_node) and (src == (*ret_val)->tgt_node)));

    return *ret_val;
  }

  GT_Arc * search(GT_Node * src, GT_Node * tgt,
                  const GT_Arc_Info && info = GT_Arc_Info())
  {
    return search(src, tgt, info);
  }

  void remove_from_graph(GT_Arc * a)  throw(std::exception, std::domain_error)
  {
    Tree_Type::find(a);
    Tree_Type::remove(a);
    g.remove_arc(a);
  }

};

} // End namespace Aleph

# endif // GRAPH_INDEXES_H

