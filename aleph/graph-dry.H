
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


# ifndef GRAPH_DRY_H
# define GRAPH_DRY_H


template <class GT>
struct GTNodeIterator : public Dlink::Iterator
{
  using Node = typename GT::Node;

  using Item_Type = Node*;

  using Set_Type = GT;

  GTNodeIterator() noexcept { /* empty */ }

  GTNodeIterator(Dlink & head) noexcept : Dlink::Iterator(head) { /* empty */ }

  Node * get_current_node() const
  {
    return (Node*) Dlink::Iterator::get_current(); 
  }

  Node * get_current() const { return get_current_node(); }
  
  Node * get_curr() const { return get_current_node(); }
};


template <class GT>
struct GTArcIterator : public Dlink::Iterator
{
  using Node = typename GT::Node;
  using Arc = typename GT::Arc;

  using Item_Type = Arc*;

  using Set_Type = GT;

  GTArcIterator() noexcept { /* empty */ }

  GTArcIterator(Dlink & head) noexcept
  : Dlink::Iterator(head) 
  { /* empty */ }

  Arc * get_current_arc() const
  {
    return (Arc*) Dlink::Iterator::get_current(); 
  }

  Arc * get_current() const { return get_current_arc(); }
  
  Arc * get_curr() const { return get_current_arc(); }

  Node * get_src_node() noexcept 
  {
    return (Node*) get_current_arc()->src_node; 
  }

  Node * get_tgt_node() noexcept
  {
    return (Node*) get_current_arc()->tgt_node; 
  }
};


template <class GT, class Cmp>
struct Cmp_Dlink_Node
{
  using Node = typename GT::Node;

  Cmp & cmp;

  Cmp_Dlink_Node(Cmp && __cmp = Cmp()) noexcept : cmp(__cmp) { /* empty */ }

  Cmp_Dlink_Node(Cmp & __cmp) noexcept : cmp(__cmp) { /* empty */ }

  bool operator () (Dlink * d1, Dlink * d2) const noexcept
  {
    Node * p1 = static_cast<Node*>(d1); 
    Node * p2 = static_cast<Node*>(d2); 
    return cmp(p1, p2);
  }
};

template <class GT, class Cmp>
struct Cmp_Dlink_Arc
{
  using Arc = typename GT::Arc;

  Cmp & cmp;

  Cmp_Dlink_Arc(Cmp && __cmp = Cmp()) noexcept : cmp(__cmp) { /* empty */ }

  Cmp_Dlink_Arc(Cmp & __cmp) noexcept : cmp(__cmp) { /* empty */ }

  bool operator () (Dlink * d1, Dlink * d2) const noexcept
  {
    Arc * arc1 = static_cast<Arc*>(d1); 
    Arc * arc2 = static_cast<Arc*>(d2); 
    return cmp(arc1, arc2);
  }
};

template <typename NodeInfo>
class GTNodeCommon
{
public:
  
  Graph_Attr attrs;
  
  NodeInfo node_info; 


  size_t num_arcs = 0; 

  using Item_Type = NodeInfo; 

  using Node = GTNodeCommon;  

  using Node_Type = NodeInfo; 

  GTNodeCommon() {}

  GTNodeCommon(const NodeInfo & info) : node_info(info) {}

  GTNodeCommon(NodeInfo && info) : node_info(move(info)) {}

  NodeInfo & get_info() noexcept { return node_info; }

  const NodeInfo & get_info() const noexcept { return node_info; }

  unsigned int state() const noexcept { return NODE_BITS(this).state; }	

  void set_state(unsigned int s) noexcept { NODE_BITS(this).state = s; }
};


template <typename ArcInfo>
class GTArcCommon
{
public:

  using Item_Type = ArcInfo;
  using Arc_Type = ArcInfo;

  void *     src_node = nullptr; 
  void *     tgt_node = nullptr; 

  Graph_Attr attrs; 

  ArcInfo    arc_info; 

  GTArcCommon(const ArcInfo & info) : arc_info(info) {}

  GTArcCommon(ArcInfo && info) : arc_info(move(info)) {}

  GTArcCommon(void * src, void * tgt, const ArcInfo & data)
    : src_node(src), tgt_node(tgt), arc_info(data) {}

  GTArcCommon(void * src, void * tgt, ArcInfo && data = ArcInfo())
    : src_node(src), tgt_node(tgt), arc_info(move(data)) {}

  unsigned int state() const noexcept { return ARC_BITS(this).state; }

  void set_state(unsigned int s) noexcept { ARC_BITS(this).state = s; }

  ArcInfo & get_info() noexcept { return arc_info; }

  const ArcInfo & get_info() const noexcept { return arc_info; }

  void * get_connected_node(void * node) noexcept
  {
    return src_node == node ? tgt_node : src_node;
  }

  void * get_img_node(void * node) noexcept
  {
    return src_node == node ? src_node : tgt_node;
  }
};


template <class GT, class Node, class Arc>
class GraphCommon
{
  GT * me() { return static_cast<GT*>(this); }

  const GT * const_me() const { return static_cast<const GT*>(this); }

protected:

  void * cookie = nullptr;
  size_t num_nodes = 0;
  size_t num_arcs = 0;
  bool digraph = false;

public:

  using Node_Type = typename Node::Node_Type;
  using Arc_Type = typename Arc::Arc_Type;
  
protected:

  void init() noexcept
  {
    num_nodes = num_arcs = 0;
    cookie = nullptr;
    digraph = false;
  }

  void common_swap(GT & g) noexcept
  { 
    std::swap(num_nodes, g.num_nodes);
    std::swap(num_arcs, g.num_arcs);
    std::swap(digraph, g.digraph);
    std::swap(cookie, g.cookie);	
  }

public:

  void *& get_cookie() noexcept { return cookie; }

  void * get_cookie() const noexcept { return cookie; }

  bool is_digraph() const noexcept { return digraph; }

  void set_digraph(bool val) { digraph = val; } // TODO: delete after

  size_t get_num_nodes() const noexcept { return num_nodes; }

  size_t vsize() const noexcept { return get_num_nodes(); }

  Node * get_node() const { return const_me()->get_first_node(); }
  
  Node * get_arc() const { return const_me()->get_first_arc(); }

  Node * get_arc(Node * p) { return const_me()->get_first_arc(p); }

  Node * get_src_node(Arc * arc) const noexcept
  {
    return (Node*) arc->src_node;
  }

  Node * get_tgt_node(Arc * arc) const noexcept
  {
    return (Node*) arc->tgt_node;
  } 

  Node * get_connected_node(Arc * arc, Node * node) const noexcept
  {
    return (Node*) arc->get_connected_node(node);		
  }
  
  size_t get_num_arcs() const noexcept { return num_arcs; }

  size_t get_num_arcs(Node * node) const noexcept
  {
    return node->num_arcs;
  }

  size_t degree(Node * p) const noexcept { return get_num_arcs(p); }

  size_t esize() const noexcept { return get_num_arcs(); }

  Bit_Fields & get_control_bits(Node * node) const noexcept
  {
    return NODE_BITS(node).reset();
  }

  void reset_bit(Node * node, int bit) const noexcept
  {
    NODE_BITS(node).reset(bit);
  }
  
  void reset_bits(Node * node) const noexcept { NODE_BITS(node).reset(); }

  int get_bit(Node * node, int bit) const noexcept
  {
    return NODE_BITS(node).get_bit(bit);
  }

  void set_bit(Node * node, int bit, int value) const noexcept
  {
    NODE_BITS(node).set_bit(bit, value);
  }

  Bit_Fields & get_control_bits(Arc * arc) const noexcept
  {
    return ARC_BITS(arc);
  }

  void reset_bit(Arc * arc, int bit) const noexcept
  {
    ARC_BITS(arc).reset(bit);
  }

  void reset_bits(Arc * arc) const noexcept { ARC_BITS(arc).reset(); } 

  int get_bit(Arc * arc, int bit) const noexcept
  {
    return ARC_BITS(arc).get_bit(bit);
  }

  void set_bit(Arc * arc, int bit, int value) const noexcept
  {
    ARC_BITS(arc).set_bit(bit, value);
  }

  void *& get_cookie(Node * node) const noexcept
  {
    return NODE_COOKIE(node);
  }

  void *& get_cookie(Arc * arc) const noexcept
  {
    return ARC_COOKIE(arc);
  }

  long & get_counter(Node * node) const noexcept
  {
    return NODE_COUNTER(node);
  }

  void reset_counter(Node * node) const noexcept 
  {
    NODE_COUNTER(node) = 0;
  }

  void reset_node_counters() const noexcept 
  {
    for_each_node([this] (auto p) { reset_counter(p); });
  }

  void reset_node(Node * p) const noexcept
  {
    p->attrs.reset();
  }

  long & get_counter(Arc * arc) const noexcept
  {
    return ARC_COUNTER(arc);
  }

  void reset_counter(Arc * arc) const noexcept
  {
    ARC_COUNTER(arc) = No_Visited;
  }

  void reset_arc_counters() const noexcept 
  {
    for_each_arc([this] (auto a) { reset_counter(a); });
  }

  void reset_arc(Arc * arc) const noexcept
  {
    arc->attrs.reset();
  }
 
  void reset_nodes() const
  {
    for_each_node([] (auto p) { p->attrs.reset(); });
  }

  void reset_arcs() const
  {			
    for_each_arc([] (auto a) { a->attrs.reset(); });
  }

  template <class N1, class N2 = N1> static
  void map_nodes(N1 * p, N2 * q) noexcept
  {
    assert(p != nullptr and q != nullptr);
    if (NODE_COOKIE(p) == nullptr)
      {
       NODE_COOKIE(p) = q;
       NODE_COOKIE(q) = p;
       return;
     }
    NODE_COOKIE(q) = NODE_COOKIE(p);
    NODE_COOKIE(p) = q;
  }

  template <class A1, class A2 = A1> static
  void map_arcs(A1 * p, A2 * q) noexcept
  {
    assert(p != nullptr and q != nullptr);
    if (ARC_COOKIE(p) == nullptr)
      {
	ARC_COOKIE(p) = q;
	ARC_COOKIE(q) = p;
	return;
      }
    ARC_COOKIE(q) = ARC_COOKIE(p);
    ARC_COOKIE(p) = q;
  }
  
  void reset_bit_nodes(int bit)	const noexcept
  {
    for_each_node([bit, this] (auto p) { reset_bit(p, bit); });
  }

  void reset_bit_arcs(int bit) const noexcept
  {
    for_each_arc([bit, this] (auto a) { reset_bit(a, bit); });
  }

  void reset_bit_nodes() const noexcept
  {
    for_each_node([this] (auto p) { reset_bits(p); });
  }

  void reset_bit_arcs()	const noexcept
  {
    for_each_arc([this] (auto a) { reset_bits(a); });
  }
  
  void reset_counter_nodes() const noexcept
  {
    for_each_node([this] (auto p) { reset_counter(p); });
  }

  void reset_counter_arcs() const noexcept
  {
    for_each_arc([this] (auto a) { reset_counter(a); });
  }

  void reset_cookie_nodes() const noexcept
  {
    for_each_node([] (auto p) { NODE_COOKIE(p) = nullptr; });
  }

  void reset_cookie_arcs() const noexcept
  {
    for_each_arc([] (auto a) { ARC_COOKIE(a) = nullptr; });
  }

  Node * insert_node(const Node_Type & node_info)
  {
    return me()->insert_node(new Node (node_info));
  }

  Node * insert_node(Node_Type && node_info = Node_Type())
  {
    return me()->insert_node(new Node(std::forward<Node_Type>(node_info)));
  }

  template <typename ...Args>
  Node * emplace_node(Args && ... args)
  {
    return me()->insert_node(Node_Type(args...));
  }

  Arc * insert_arc(Node * src, Node * tgt, const Arc_Type & arc_info)
  {
    std::unique_ptr<Arc> arc(new Arc(arc_info));
    me()->insert_arc(src, tgt, arc.get());
    return arc.release();
  }

  Arc * insert_arc(Node * src, Node * tgt, Arc_Type && arc_info = Arc_Type())
  {
    std::unique_ptr<Arc> arc(new Arc(std::forward<Arc_Type>(arc_info)));
    me()->insert_arc(src, tgt, arc.get());
    return arc.release();
  }

  template <typename ...Args>
  Arc * emplace_arc(Node * src, Node * tgt, Args && ... args)
  {
    return me()->insert_arc(src, tgt, Arc_Type(args...));
  }
  
  template <class Operation>
  bool traverse_nodes(Operation & operation) const
  {
    for (typename GT::Node_Iterator it(*const_me()); it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse_nodes(Operation && operation = Operation()) const
  {
    return traverse_nodes(operation);
  }

  template <class Operation>
  bool traverse_arcs(Operation & operation) const
  {
    for (typename GT::Arc_Iterator it(*const_me()); it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse_arcs(Operation && operation = Operation()) const
  {
    return traverse_arcs(operation);
  } 

  template <class Operation>
  bool traverse_arcs(Node * p, Operation & operation) const
  {
    for (typename GT::Node_Arc_Iterator it(p); it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true; 
  }

  template <class Operation>
  bool traverse_arcs(Node * p, Operation && operation = Operation()) const
  {
    return traverse_arcs(p, operation);
  }

  template <class Operation>
  void for_each_node(Operation & operation) const
  {
    for (typename GT::Node_Iterator it(*const_me()); it.has_curr(); it.next())
      operation(it.get_curr());
  }

  template <class Operation>
  void for_each_node(Operation && operation = Operation()) const
  {
    for_each_node(operation);
  }

  template <class Operation> 
  void for_each_arc(Operation & operation) const
  {
    for (typename GT::Arc_Iterator it(*const_me()); it.has_curr(); it.next())
      operation(it.get_curr());
  }

  template <class Operation>
  void for_each_arc(Operation && operation = Operation()) const
  {
    for_each_arc(operation);
  }

    template <class Operation>
  void for_each_arc(Node * p, Operation & operation) const
  {
    for (typename GT::Node_Arc_Iterator it(p); it.has_curr(); it.next())
      operation(it.get_curr());
  }

  template <class Operation>
  void for_each_arc(Node * p, Operation && operation = Operation()) const
  {
    for_each_arc(p, operation);
  }

  template <class Operation>
  bool all_nodes(Operation & operation) const
  {
    return traverse_nodes(operation);
  }

  template <class Operation>
  bool all_nodes(Operation && operation = Operation()) const
  {
    return all_nodes(operation);
  }

  template <class Operation>
  bool all_arcs(Operation & operation) const
  {
    return traverse_arcs(operation);
  }

  template <class Operation>
  bool all_arcs(Operation && operation = Operation()) const
  {
    return all_arcs(operation);
  }

  template <class Operation>
  bool all_arcs(Node * p, Operation & operation) const
  { 
    return traverse_arcs(p, operation);
  }

  template <class Operation>
  bool all_arcs(Node * p, Operation && operation = Operation()) const
  {
    return all_arcs(p, operation);
  }
  
  template <typename T = Node_Type>
  auto nodes_map(std::function<T(Node *)> operation) const
  {
    DynList<T> ret_val;
    for_each_node([&ret_val, &operation] (Node * p)
		  {
		    ret_val.append(operation(p));
		  });
    return ret_val;
  }

  template <typename T = Arc_Type>
  auto arcs_map(std::function<T(Arc *)> operation) const
  {
    DynList<T> ret_val;
    for_each_arc([&ret_val, &operation] (Arc * p)
		 {
		   ret_val.append(operation(p));
		 });
    return ret_val;
  }

  template <typename T = Arc_Type>
  auto arcs_map(Node * p, std::function<T(Arc *)> operation) const
  {
    DynList<T> ret_val;
    for_each_arc(p, [&ret_val, &operation] (Arc * a)
		 {
		   ret_val.append(operation(a));
		 });
    return ret_val;
  }

  template <typename T = Node_Type>
  T foldl_nodes(const T & init,
		std::function<T(const T&, Node*)> operation) const
  {
    T ret = init;
    for_each_node([&ret, &operation] (Node * p)
		  {
		    ret = operation(ret, p);
		  });				
    return ret;
  }

  template <typename T = Arc_Type>
  T foldl_arcs(const T & init,
	       std::function<T(const T&, Arc*)> operation) const
  {
    T ret = init;
    for_each_arc([&ret, &operation] (Arc * p)
		 {
		   ret = operation(ret, p);
		 });
    return ret;
  }

  template <typename T = Arc_Type>
  T foldl_arcs(Node * p, const T & init,
	       std::function<T(const T&, Arc*)> operation) const
  {
    T ret = init;
    for_each_arc(p, [&ret, &operation] (Arc * a)
		 {
		   ret = operation(ret, a);
		 });
    return ret;
  }
  
  template <class Op> auto filter_nodes(Op & op) const
  {
    DynList<Node*> ret;
    for_each_node([&ret, &op] (Node * p)
		  {
		    if (op(p))
		      ret.append(p); 
		  });
    return ret;
  }

  template <class Op> auto filter_nodes(Op && op) const
  {
    return filter_nodes(op);
  }

  template <class Op> auto filter_arcs(Op & op) const
  {
    DynList<Arc*> ret;
    for_each_arc([&ret, &op] (Arc * a)
		 {
		   if (op(a))
		     ret.append(a);
		 });
    return ret;
  }

  template <class Op> auto filter_arcs(Op && op) const
  {
    return filter_arcs(op);
  }

  template <class Op> auto filter_arcs(Node * p, Op & op) const
  {
    DynList<Arc*> ret;
    for_each_arc(p, [&ret, &op] (Arc * a)
		  {
		    if (op(a))
		      ret.append(a);
		  });
    return ret;
  } 

  template <class Op> auto filter_arcs(Node * p, Op && op) const
  {
    return filter_arcs(p, op);
  }

  template <class Operation>
  bool exists_node(Operation & operation) const
  {
    return not traverse_nodes([&operation] (Node * p)
			      {
				return not operation(p);
			      });
  }

  template <class Operation>
  bool exists_node(Operation && operation = Operation()) const
  {
    return exists_node(operation);
  }

  template <class Operation>
  bool exists_arc(Operation & operation) const
  {
    return not traverse_arcs([&operation] (Arc * a)
			     {
			       return not operation(a);
			     });
  }

  template <class Operation>
  bool exists_arc(Operation && operation = Operation()) const
  {
    return exists_arc(operation);
  }

  template <class Operation>
  bool exists_arc(Node * p, Operation & operation) const
  {
    return not traverse_arcs(p, [&operation] (Arc * a)
			     {
			       return not operation(a);
			     });
  }

  template <class Operation>
  bool exists_arc(Node * p, Operation && operation = Operation()) const
  {
    return exists_arc(p, operation);
  }

  template <class Op>
  Node * search_node(Op & op) const
  {
    for (typename GT::Node_Iterator it(*const_me()); it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	if (op(p))
	  return p;
      }
    return nullptr;
  }

  template <class Op>
  Node * search_node(Op && op) const { return search_node(op); }

  Node * find_node(const Node_Type & info) const
  {
    return search_node([&info] (auto p) { return p->get_info() == info; });
  }

  template <class Op>
  Arc * search_arc(Op & op) const
  {
    for (typename GT::Arc_Iterator it(*const_me()); it.has_curr(); it.next())
      {
	auto a = it.get_curr();
	if (op(a))
	  return a;
      }
    return nullptr;
  }

  template <class Op>
  Arc * search_arc(Op && op) const { return search_arc(op); }

  Arc * find_arc(const Arc_Type & info) const
  {
    return search_arc([&info] (auto a) { return a->get_info() == info; });
  }
  
  template <class Operation>
  Arc * search_arc(Node * p, Operation & op) const
  { 
    for (typename GT::Node_Arc_Iterator it(p); it.has_curr(); it.next())
      {
	Arc * arc = it.get_curr();
	if (not op(arc))
	  return arc;
      }
    return nullptr;
  }

  template <class Operation>
  Arc * search_arc(Node * p, Operation && op = Operation()) const
  {
    return search_arc(p, op);
  }

  Arc * search_arc(Node * src, Node * tgt) const
  {
    for (typename GT::Node_Arc_Iterator it(src); it.has_curr(); it.next())
      if (it.get_tgt_node() == tgt)
	return it.get_curr();
    return nullptr;
  }

  template <template <typename> class Container = Aleph::DynList>
  Container<Node*> nodes() const
  {
    Container<Node*> ret;
    for_each_node([&ret] (Node * p) { ret.append(p); });
    return ret;
  }

  template <template <typename> class Container = Aleph::DynList>
  Container<Arc*> arcs() const
  {
    Container<Arc*> ret;
    for_each_arc([&ret] (Arc * a) { ret.append(a); });
    return ret;
  }

  template <template <typename> class Container = Aleph::DynList>
  Container<Arc*> arcs(Node * p) const
  {
    Container<Arc*> ret;
    this->for_each_arc(p, [&ret] (Arc * a) { ret.append(a); });
    return ret;
  }

  auto get_node_it() const noexcept 
  {
    return typename GT::Node_Iterator(*const_me()); 
  }

  auto get_arc_it() const noexcept
  {
    return typename GT::Arc_Iterator(*const_me()); 
  }

  auto get_arc_it(Node * p) const noexcept
  {
    return typename GT::Node_Arc_Iterator(p); 
  }

  struct In_Filt
  {
    Node * tgt = nullptr; 

    In_Filt(Node * __tgt = nullptr) noexcept : tgt(__tgt) { /* empty */ }

    bool operator () (Arc * a) const noexcept
    {
      assert(tgt);
      return a->tgt_node == tgt;
    }

    Node * get_node(Arc * a) const noexcept
    {
      assert(tgt);
      return (typename GT::Node *) a->src_node;
    }
  };

  struct Out_Filt
  {
    Node * src = nullptr; 

    Out_Filt(Node * __src) noexcept : src(__src) { /* empty */ }

    bool operator () (Arc * a) const noexcept
    {
      assert(src);
      return a->src_node == src;
    }

    Node * get_node(Arc * a) const noexcept
    {
      assert(src);
      return (Node *) a->tgt_node;
    }
  };

  template <class Filter>
  class Digraph_Iterator
  {
    using Itor = Filter_Iterator<Node*, typename GT::Node_Arc_Iterator, Filter>;

    Filter filt;
    Itor it;

  public:

    using Item_Type = typename Itor::Item_Type; 

    using Iterator_Type = Itor;

    Digraph_Iterator(Node * p)
    noexcept(noexcept(Filter(p)) and noexcept(Itor(p, filt)))
    : filt(p), it(p, filt) 
    {
      // empty
    }

    void next() { it.next(); }

    void prev() { it.next(); }

    bool has_curr() const noexcept { return it.has_curr(); }

    auto has_current() const noexcept { return has_curr(); }

    typename GT::Arc * get_curr() const { return it.get_curr(); }

    auto get_current() const { return get_curr(); }
		
    auto get_current_arc() const { return get_curr(); }
	
    typename GT::Node * get_node(typename GT::Arc * a) const 
      noexcept(noexcept(filt.get_node(a)))
    {
      return filt.get_node(a);
    }

    typename GT::Node * get_node() 
    { 
      return this->get_node(this->get_curr()); 
    }

    auto get_tgt_node() { return get_node(); }

    void reset_first() noexcept { it.reset_first(); }

    void reset_last() noexcept { it.reset_last(); }
  };

  template <class Filt> using Filter_Iterator = Digraph_Iterator<Filt>;

  using In_Iterator = Digraph_Iterator<In_Filt>;

  using Out_Iterator = Digraph_Iterator<Out_Filt>;

  In_Iterator get_in_it(Node * p) const noexcept { return In_Iterator(p); }

  Out_Iterator get_out_it(Node * p) const noexcept { return Out_Iterator(p); }

  Arc * search_directed_arc(Node * src, Node * tgt) const
  {
    for (typename GT::Out_Iterator it(src); it.has_curr(); it.next())
      if (it.get_tgt_node() == tgt)
	return it.get_curr();
    return nullptr;
  }

  DynList<Node*> in_nodes(Node * p) const
  {
    DynList<Node*> ret;
    for (In_Iterator it(p); it.has_curr(); it.next())
      ret.append(it.get_node());
    return ret;
  }

  DynList<Node*> out_nodes(Node * p) const
  {
    DynList<Node*> ret;
    for (Out_Iterator it(p); it.has_curr(); it.next())
      ret.append(it.get_node());
    return ret;
  }

  DynList<Arc*> out_arcs(Node * p) const
  {
    DynList<Arc*> ret;
    for (Out_Iterator it(p); it.has_curr(); it.next())
      ret.append(it.get_curr());
    return ret;
  }

  DynList<Arc*> in_arcs(Node * p) const
  {
    DynList<Arc*> ret;
    for (In_Iterator it(p); it.has_curr(); it.next())
      ret.append(it.get_curr());
    return ret;
  }

  using ArcPair = tuple<Arc*, Node*>;

  auto in_pairs(Node * p) const
  {
    DynList<ArcPair> ret;
    for (In_Iterator it(p); it.has_curr(); it.next())
      {
	auto a = it.get_curr();
	ret.append(make_tuple(a, (Node*) a->get_connected_node(p)));
      }
    return ret;
  }

  auto out_pairs(Node * p) const
  {
    DynList<ArcPair> ret;
    for (Out_Iterator it(p); it.has_curr(); it.next())
      {
	auto a = it.get_curr();
	ret.append(make_tuple(a, (Node*) a->get_connected_node(p)));
      }
    return ret;
  }

  size_t in_degree(Node * p) const noexcept
  {
    size_t count = 0;
    for (In_Iterator it(p); it.has_curr(); it.next())
      ++count;
    return count;
  }
  
  size_t out_degree(Node * p) const noexcept
  {
    size_t count = 0;
    for (Out_Iterator it(p); it.has_curr(); it.next())
      ++count;
    return count;
  }

  template <class Itor, class Operation> 
  bool traverse_arcs(Node * p, Operation & operation) const
  {
    for (Itor it(p); it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true;
  }

  template <class Itor, class Operation>
  void for_each_arc(Node * p, Operation & operation) const
  {
    for (Itor it(p); it.has_curr(); it.next())
      operation(it.get_curr());
  }

  template <class Op>
  bool traverse_in_arcs(Node * p, Op & op) const
  {
    return traverse_arcs<In_Iterator, Op>(p, op);
  }

  template <class Op>
  bool traverse_in_arcs(Node * p, Op && op = Op()) const
  {
    return traverse_in_arcs(p, op);
  }

  template <class Op>
  void for_each_in_arc(Node * p, Op & op) const
  {
    for_each_arc(p, op); 
  }

  // \overload for_each_in_arc(Node * p, Op & op)
  template <class Op>
  void for_each_in_arc(Node * p, Op && op = Op()) const
  {
    for_each_in_arc(p, op);
  }

  template <class Op>
  bool all_in_arcs(Node * p, Op & op) const
  {
    return traverse_in_arcs(p, [&op] (auto a) { return op(a); });
  }

  template <class Op>
  bool all_in_arcs(Node * p, Op && op = Op()) const
  {
    return all_in_arc(p, op);
  }

  template <class Op>
  bool exists_in_arc(Node * p, Op & op) const
  {
    return not traverse_in_arcs(p, [&op] (auto a) { return not op(a); });
  }

  template <class Op>
  bool exists_in_arc(Node * p, Op && op = Op()) const
  {
    return exists_in_arc(p, op);
  }

  template <class Op>
  auto search_in_arc(Node * p, Op & op) const
  {
    Arc * ret = nullptr;
    traverse_in_arcs(p, [&op, &ret] (auto a)
		     {
		       if (op(a))
			 {
			   ret = a;
			   return false;
			 }
		       return true;
		     });
    return ret;
  }

  template <class Op>
  auto search_in_arc(Node * p, Op && op = Op()) const
  {
    return search_in_arc(p, op);
  }

  template <typename T>
  auto in_arcs_map(Node * p, std::function<T(Arc*)> op) const
  {
    DynList<T> ret;
    for_each_in_arc(p, [&ret, &op] (auto a) { ret.append(op(a)); });
    return ret;
  }

  template <typename T = Arc_Type>
  T foldl_in_arcs(Node * p, const T & init, 
		  std::function<T(const T&, Arc*)> op) const
  {
    T ret = init;
    for_each_in_arc(p, [&ret, &op] (auto a) { ret = op(ret, a); });
    return ret;
  }

  template <class Op>
  DynList<Arc*> filter_in_arcs(Node * p, Op & op) const
  {
    DynList<Arc*> ret;
    for_each_in_arc(p, [&ret, &op] (auto a) 
		    {
		      if (op(a))
			ret.append(a);
		    });
    return ret;
  }

  template <class Op>
  auto filter_in_arcs(Node * p, Op && op = Op()) const
  {
    return filter_in_arcs(p, op);
  }

  template <class Op>
  bool traverse_out_arcs(Node * p, Op & op) const
  {
    return traverse_arcs(p, op);
  }

  template <class Op>
  bool traverse_out_arcs(Node * p, Op && op = Op()) const
  {
    return traverse_out_arcs(p, op);
  }

  template <class Op>
  void for_each_out_arc(Node * p, Op & op) const
  {
    for_each_arc(p, op);
  }

  template <class Op>
  void for_each_out_arc(Node * p, Op && op = Op()) const
  {
    for_each_out_arc(p, op);
  }

  template <class Op>
  bool all_out_arcs(Node * p, Op & op) const
  {
    return traverse_out_arcs(p, [&op] (auto a) { return op(a); });
  }

  template <class Op>
  bool all_out_arcs(Node * p, Op && op = Op()) const
  {
    return all_out_arc(p, op);
  }

  template <class Op>
  bool exists_out_arc(Node * p, Op & op) const
  {
    return not traverse_out_arcs(p, [&op] (auto a) { return not op(a); });
  }

  template <class Op>
  bool exists_out_arc(Node * p, Op && op = Op()) const
  {
    return exists_out_arc(p, op);
  }

  template <class Op>
  auto search_out_arc(Node * p, Op & op) const
  {
    typename GT::Arc * ret = nullptr;
    traverse_out_arcs(p, [&op, &ret] (auto a)
		      {
			if (op(a))
			  {
			    ret = a;
			    return false;
			  }
			return true;
		      });
    return ret;
  }

  template <class Op>
  auto search_out_arc(Node * p, Op && op = Op()) const
  {
    return search_out_arc(p, op);
  }

  template <typename T = Arc_Type>
  auto out_arcs_map(Node * p, std::function<T(Arc*)> op) const
  {
    DynList<T> ret;
    for_each_out_arc(p, [&ret, &op] (auto a) { ret.append(op(a)); });
    return ret;
  }

  template <typename T = Arc_Type>
  T foldl_out_arcs(Node * p, const T & init,
		   std::function<T(const T&, Arc*)> op) const
  {
    T ret = init;
    for_each_out_arc(p, [&ret, &op] (auto a) { ret = op(ret, a); });
    return ret;
  }

  template <class Op>
  DynList<Arc*> filter_out_arcs(Node * p, Op & op) const
  {
    DynList<Arc*> ret;
    for_each_out_arc(p, [&ret, &op] (auto a) 
		     {
		       if (op(a))
			 ret.append(a);
		     });
    return ret;
  }

  template <class Op>
  auto filter_out_arcs(Node * p, Op && op = Op()) const
  {
    return filter_out_arcs(p, op);
  }
};



# endif // GRAPH_DRY_H
