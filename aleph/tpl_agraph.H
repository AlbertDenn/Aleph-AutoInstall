
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/

# ifndef TPL_AGRAPH_H
# define TPL_AGRAPH_H

# include <tpl_dynSetTree.H>
# include <array_it.H>
# include <tpl_sgraph.H>

namespace Aleph {

using namespace Aleph;

    template <typename Node_Info = Empty_Class> 
class Graph_Anode 
      : public Dlink,
	public GTNodeCommon<Node_Info>
{
  using Base = GTNodeCommon<Node_Info>;
  friend class GTNodeCommon<Node_Info>;

  static const size_t Contract_Factor = 4;
  static const size_t Default_Cap = 4;

  void init(size_t dim)
  {
    arcs_dim = dim;
    this->num_arcs = 0;
    contract_threshold = arcs_dim/Contract_Factor;
    arc_array = nullptr;
    if (arcs_dim == 0)
      return;

    arc_array = (void**) malloc(arcs_dim*sizeof(void*));
    if (arc_array == nullptr)
      throw std::bad_alloc();
  }
  
public:

  void ** arc_array;
  size_t arcs_dim;
  size_t contract_threshold;
    
  Graph_Anode()
  {
    init(0);    
  }

  Graph_Anode(const Node_Info & info) : Base(info)
  {
    init(Default_Cap);
  }

  Graph_Anode(Node_Info && info) : Base(move(info))
  {
    init(Default_Cap);
  }

  Graph_Anode(const Graph_Anode & node) : Base(node.node_info)
  {
    init(0);
  }

  Graph_Anode & operator = (const Graph_Anode & node)
  noexcept(std::is_nothrow_copy_assignable<Node_Info>::value)
  {
    if (&node == this)
      return *this;
    this->node_info = node.node_info;
    return *this;
  }

  Graph_Anode(Graph_Anode * p) : Base(p->get_info())
  { 
    init(0);
  }
 
  ~Graph_Anode()
  {
    if (arc_array != nullptr)
      free(arc_array);
  }

  void allocate_more(size_t new_size)
  {
    if (new_size == 0)
      new_size = 1;

    void ** new_array = (void**) realloc(arc_array, new_size*sizeof(void*));
    if (new_array == nullptr)
      throw std::bad_alloc();
    
    arc_array = new_array;
    arcs_dim = new_size;
    contract_threshold = arcs_dim/Contract_Factor;
  }

  void * insert_arc(void * arc) 
  {
    if (this->num_arcs == arcs_dim)
      allocate_more(arcs_dim << 1); // 2*arcs_dim
    
    arc_array[this->num_arcs++] = arc;

    return arc;
  }

  void remove_arc(void * arc) 
  {
    bool removed = false;
    for (size_t i = 0; i < this->num_arcs; ++i)
      if (arc_array[i] == arc)
	{
	  arc_array[i] = arc_array[--(this->num_arcs)];
	  removed = true;
	  break;
	}

    if (not removed)
      throw std::domain_error("arc for deleting not found");

    if (this->num_arcs > contract_threshold)
      return;
	
        // contraction 
    size_t new_sz = arcs_dim >> 1; // num_arcs/2
    arc_array = (void**) realloc(arc_array, new_sz*sizeof(void*));
    arcs_dim = new_sz;
    contract_threshold = arcs_dim/Contract_Factor;
  }

  bool compress() noexcept
  {
    void ** new_array = (void**) realloc(arc_array, 
					 this->num_arcs*sizeof(void*));
    if (new_array == nullptr)
      return false;
    
    arc_array = new_array;
    arcs_dim = this->num_arcs;
    contract_threshold = this->num_arcs/Contract_Factor;

    return true;
  }
};


    template <typename Arc_Info = Empty_Class> 
class Graph_Aarc 
      : public Dlink,
	public GTArcCommon<Arc_Info>
{
  friend class GTArcCommon<Arc_Info>;
  using Base = GTArcCommon<Arc_Info>;

public:

  Graph_Aarc(const Arc_Info & info) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value) : Base(info)
  { 
    /* empty */ 
  }

  Graph_Aarc(Arc_Info && info = Arc_Info()) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
  : Base(move(info))
  { 
    /* empty */ 
  }

  Graph_Aarc(const Graph_Aarc & arc) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
    : Graph_Aarc(arc.arc_info) { /* empty */ }

  Graph_Aarc & operator = (const Graph_Aarc & arc)
  noexcept(std::is_nothrow_copy_assignable<Arc_Info>::value)
  {
    if (&arc == this)
      return *this;
    this->arc_info = arc.arc_info;
    return *this;
  }

  Graph_Aarc(void * src, void * tgt, const Arc_Info & data) 
    noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value) 
  : Base(src, tgt, data)
  { 
    // empty
  }

  Graph_Aarc(void * src, void * tgt, Arc_Info && data = Arc_Info()) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
  : Base(src, tgt, move(data))
  { 
    // empty
  }
};


  template <class __Graph_Node = Graph_Anode<unsigned long>, 
	    class __Graph_Arc  = Graph_Aarc<unsigned long>>
class Array_Graph
	      :  public GraphCommon<Array_Graph<__Graph_Node, __Graph_Arc>,
				    __Graph_Node, __Graph_Arc>
{
public:

  using Node = __Graph_Node;
  using Arc = __Graph_Arc;
  using Node_Type = typename Node::Node_Type;
  using Arc_Type = typename Arc::Arc_Type;

  friend class GraphCommon<Array_Graph<__Graph_Node, __Graph_Arc>,
			   __Graph_Node, __Graph_Arc>;

  using CommonBase = GraphCommon<Array_Graph<__Graph_Node, __Graph_Arc>,
				 __Graph_Node, __Graph_Arc>;

  using CommonBase::insert_node;
  using CommonBase::insert_arc;

private:

  // using DynSetNode = DynSetTree<Node*, Avl_Tree>;
  using DynSetArc = DynSetTree<Arc*, Rand_Tree>;

  Dlink node_set;
  Dlink arc_set;

public:

  struct Node_Iterator : public GTNodeIterator<Array_Graph>
  {
    using Base = GTNodeIterator<Array_Graph>;
    using Base::Base;
    
    Node_Iterator(const Array_Graph & g) noexcept
      : Base(const_cast<Dlink&>(g.node_set))
    {
      // empty
    }
  };

  struct Arc_Iterator : public GTArcIterator<Array_Graph>
  {
    using Base = GTArcIterator<Array_Graph>;
    using Base::Base;

    Arc_Iterator(const Array_Graph & g) 
      : Base(const_cast<Dlink&>(g.arc_set))
    {
      // empty
    }
  };
  // class Arc_Iterator : public DynSetArc::Iterator
  // {
  // public:

  //       
  //   using Item_Type = Arc *;

  //      
  //   using Set_Type = Array_Graph;

  //   Arc_Iterator() noexcept { /* empty */ }

  //   Arc_Iterator(const Array_Graph & _g) noexcept
  //     : DynSetArc::Iterator(_g.arc_set)
  //   {
  //     // empty
  //   }

  //       
  //   Arc * get_current_arc() const
  //   {
  //     return this->get_curr();
  //   }

  //       
  //   Node * get_src_node() noexcept 
  //   {
  //     return (Node*) get_current_arc()->src_node; 
  //   }

  //       
  //   Node * get_tgt_node() noexcept
  //   {
  //     return (Node*) get_current_arc()->tgt_node; 
  //   }
  // };

  
  class Node_Arc_Iterator : public Array_Iterator<void*>
  {
    Node * src_node = nullptr;

  public:

    using Item_Type = Arc *;

    using Set_Type = Node *;

    Node_Arc_Iterator() noexcept { /* empty */ }

    Node_Arc_Iterator(Node * src) noexcept
      : Array_Iterator<void*>(src->arc_array, src->arcs_dim, src->num_arcs), 
	src_node(src)
    {
      // empty 
    }

    Arc * get_current_arc() const
    {
      return (Arc*) const_cast<Node_Arc_Iterator*>(this)->
	Array_Iterator<void*>::get_curr();
    }

    Arc * get_current() const { return get_current_arc(); }

    Arc * get_curr() const { return get_current_arc(); }

    Node * get_tgt_node() const
    {
      Arc * a = get_curr();
      return (Node*) a->get_connected_node(src_node);
    }
  };

  virtual Node * insert_node(Node * p)
  {
    assert(p != nullptr);
    assert(p->num_arcs == 0);

    node_set.append(p);
    this->num_nodes++;

    return p;
  }

  void compress()
  {
    for (Node_Iterator it(*this); it.has_curr(); it.next())
      it.get_curr()->compress();
  }

private:

  Arc * try_insert_arc(Node * src, Node * tgt, void * a)
  {
    Arc * aptr = (Arc*) a;

    aptr->src_node = src;
    aptr->tgt_node = tgt;
    src->insert_arc(aptr);

    if (not this->digraph and src != tgt)
      {
	try
	  {
	    tgt->insert_arc(aptr);
	  }
	catch (std::bad_alloc)
	  {
	    src->remove_arc(aptr);
	    throw;
	  }
      }
    
    try
      {
	arc_set.append(aptr);
	this->num_arcs++;
      }
    catch (std::bad_alloc)
      {
	src->remove_arc(aptr);
	if (not this->digraph and src != tgt)
	  tgt->remove_arc(aptr);
	throw;
      }
    
    return aptr;
  }

public:

  Arc * connect_arc(Arc * arc)
  {
    return 
    try_insert_arc(this->get_src_node(arc), this->get_tgt_node(arc), arc);
  }

private:

  Arc * insert_arc(Node * src, Node * tgt, void * a)
  {
    bool compress_done = false;
    
  retry:
    try
      {
	return try_insert_arc(src, tgt, a);
      }
    catch (bad_alloc)
      {
	if (compress_done)
	  throw;
	
	compress();
	compress_done = true;
	goto retry;
      }
  }

public:

  Arc * disconnect_arc(Arc * arc) noexcept
  {
    Node * src = (Node*) arc->src_node;
    Node * tgt = (Node*) arc->tgt_node;

    src->remove_arc(arc);
    if (not this->digraph and src != tgt)
      tgt->remove_arc(arc);

    arc->del(); // delete it from arc_set
    this->num_arcs--;

    return arc;
  }

  virtual void remove_arc(Arc * a)
  {
    delete disconnect_arc(a);
  }  

  virtual void remove_node(Node * p) 
  {
    DynList<Arc*> arcs; // store arcs to delete
    if (this->digraph)
      // traverse all arcs of graph and store those whose source or target is p
      for (Arc_Iterator it(*this); it.has_curr(); it.next())
        {
          Arc * arc = it.get_curr();
          if (this->get_src_node(arc) == p or this->get_tgt_node(arc) == p)
            arcs.append(arc);
        }
    else
      // traverse arc of node
      for (size_t i = 0, n = p->num_arcs; i < n; ++i)
        {
	  Arc * arc = (Arc*) p->arc_array[i];
	  arcs.append(arc);
        }
    
    arcs.for_each([this] (auto arc) { this->remove_arc(arc); });

    p->del(); // remove from node_set
    this->num_nodes--;

    delete p;
  }

  Node * get_first_node() const
  {
    return (Node*) const_cast<Dlink&>(node_set).get_first(); 
  }

  Arc * get_first_arc() const 
  {
    return (Arc*) const_cast<Dlink&>(arc_set).get_first(); 
  }

  Arc * get_first_arc(Node * p) const
  {
    if (get_num_arcs(p) == 0)
      throw std::range_error("Node has not arcs");
    return (Arc*) p->arc_array[0];
  }

  Array_Graph() 
  { 
    assert(this->num_nodes == 0 and this->num_arcs == 0 and 
	   node_set.is_empty() and arc_set.is_empty()); 
  }

private:

  void clear() noexcept
  {
    while (not arc_set.is_empty())
      delete static_cast<Arc*>(arc_set.remove_first());

    while (not node_set.is_empty())
      delete static_cast<Node*>(node_set.remove_first());

    this->num_nodes = this->num_arcs = 0;
  }

public:

  virtual ~Array_Graph() noexcept
  {
    clear();
  }

  Array_Graph(const Array_Graph & g) 
  {
    assert(this->num_nodes == 0 and this->num_arcs == 0);
    copy_graph(*this, g, false);
  }

  void swap(Array_Graph & g) noexcept
  {
    this->common_swap(g);
    node_set.swap(g.node_set);
    arc_set.swap(g.arc_set);
  }

  Array_Graph(Array_Graph && g) noexcept
  {
    swap(g);
  }

  Array_Graph & operator = (const Array_Graph & g)
  {
    if (&g == this)
      return *this;

    copy_graph(*this, g, false);
    
    return *this;
  }

  Array_Graph & operator = (Array_Graph && g) noexcept
  {
    swap(g);
    return *this;
  }

private:

      template <class Cmp>
  struct Cmp_Arc
  {
    Cmp & cmp;

    Cmp_Arc(Cmp && __cmp = Cmp()) 
    noexcept(std::is_nothrow_constructible<Cmp>::value and
	     std::is_nothrow_move_assignable<Cmp>::value)
      : cmp(__cmp) { /* empty */ }

    Cmp_Arc(Cmp & __cmp) noexcept(std::is_nothrow_copy_assignable<Cmp>::value)
      : cmp(__cmp) { /* empty */ }

    bool operator () (Arc * a1, Arc * a2) const noexcept
    {
      return cmp(a1, a2);
    }
  };

public:

  template <class Compare> inline
  void sort_nodes(Compare & cmp) noexcept
  {
    Cmp_Dlink_Node<Array_Graph, Compare> c = cmp;
    mergesort(node_set, c);
  }

  template <class Compare> inline 
  void sort_nodes(Compare && cmp = Compare()) noexcept
  {
    sort_nodes(cmp);
  }

      template <class Compare> 
  void sort_arcs(Compare & cmp)
  {
    Cmp_Dlink_Arc<Array_Graph, Compare> c = cmp;
    mergesort(arc_set, c);
  }

      template <class Compare> 
  void sort_arcs(Compare && cmp) { sort_arcs(cmp); }
};

template <typename __Graph_Node = Graph_Anode<int>, 
	  typename __Graph_Arc  = Graph_Aarc<int>>
class Array_Digraph : public Array_Graph<__Graph_Node, __Graph_Arc>
{
  using GT = Array_Graph<__Graph_Node, __Graph_Arc>;

public:

  using Node = __Graph_Node;

  using Arc = __Graph_Arc;

  Array_Digraph() noexcept
  {
    this->digraph = true; 
  }
  
  Array_Digraph(const Array_Digraph & dg) noexcept
    : Array_Graph<Node, Arc>((Array_Digraph&) dg)
  {
    this->digraph = true; 
  }

  Array_Digraph & operator = (const Array_Digraph<Node, Arc> & g)
  {
    if (this == &g) 
      return *this;

    copy_graph(*this, g, false);

    return *this;
  }

  Array_Digraph(Array_Digraph && dg) noexcept : GT()
  {
    this->digraph = true; 
    this->swap(dg);
  }

  Array_Digraph & operator = (Array_Digraph<Node, Arc> && g) noexcept
  {
    this->swap(g);

    return *this;
  }
};


} // namespace Aleph {

# endif //  TPL_AGRAPH_H
