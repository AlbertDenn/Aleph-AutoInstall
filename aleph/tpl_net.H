
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

# ifndef TPL_NET_H
# define TPL_NET_H

# include <limits>
# include <set>

# include <tpl_dynDlist.H>
# include <tpl_dynListStack.H>
# include <tpl_dynBinHeap.H>
# include <tpl_dynSetTree.H>
# include <tpl_dynSetHash.H>
# include <tpl_random_queue.H>
# include <tpl_graph_utils.H>
# include <tpl_find_path.H>
# include <graph-traverse.H>


  
using namespace Aleph;

namespace Aleph {

  template <typename Arc_Info, typename Flow_Type>
struct Net_Arc_Info : public Arc_Info
{
  Flow_Type cap; 

  Flow_Type flow; 

  Net_Arc_Info() noexcept(std::is_nothrow_constructible<Arc_Info>::value) {}

  Net_Arc_Info(const Arc_Info & info, Flow_Type __cap, Flow_Type __flow)
  noexcept(noexcept(Arc_Info(info)))
    : Arc_Info(info), cap(__cap), flow(__flow)
  {
    // empty
  }
};

  template <typename Arc_Info, typename F_Type = double>
struct Net_Arc : public Graph_Aarc<Arc_Info> // Tipo que representa el flujo 
{
  using Base = Graph_Aarc<Arc_Info>;

  using Flow_Type = F_Type;

  Flow_Type cap = 0; 

  Flow_Type flow = 0; 

  bool check_arc() const noexcept { return flow >= 0 and flow <= cap; }

  Net_Arc(const Arc_Info & info) 
  noexcept(noexcept(Base(info)))
    : Base(info) { /* empty */ } 

  Net_Arc(const Net_Arc & arc) 
  noexcept(noexcept(Base(arc.arc_info)))
    : Base(arc.arc_info), cap(arc.cap), flow(arc.flow)
  {
    // empty
  }

  Net_Arc() { /* empty */ }

  Net_Arc & operator = (const Net_Arc & arc)
  noexcept(std::is_nothrow_copy_assignable<Arc_Info>::value)
  {
    if (this == &arc) 
      return *this;

    *(Base*) this = arc;
    cap           = arc.cap;
    flow          = arc.flow;

    return *this;
  }
};


template <class Net>
bool is_residual(typename Net::Node * src, typename Net::Arc * a) noexcept
{
  assert(a->src_node == src or a->tgt_node == src);
  return a->tgt_node == src;
}

  template <class Net>
struct Net_Filt
{
  typename Net::Node * p = nullptr;

  void * cookie = nullptr;

  void set_cookie(void * __cookie) noexcept { cookie = __cookie; }

  Net_Filt(typename Net::Node * s = nullptr) noexcept
    : p(s) { /* empty */ }

  bool operator () (typename Net::Arc * a) const noexcept
  {
    assert(p);
    assert(a->src_node == p or a->tgt_node == p);
    auto src = (typename Net::Node*) a->src_node;
    if (src == p)
      return a->cap - a->flow > 0; // normal arc
    
    assert(is_residual<Net>(p, a));
    return a->flow > 0; // residual arc
  }

  bool operator () (const Net &, typename Net::Arc * arc) noexcept
  {
    return (*this)(arc);
  }

  typename Net::Node* get_node(typename Net::Arc * a) const noexcept
  {
    assert(p);
    assert(a->src_node == p or a->tgt_node == p);
    return (typename Net::Node*) (a->src_node == p ? a->tgt_node : a->src_node);
  }
};


template <class Net>
using __Net_Iterator = Digraph_Iterator<Net, Net_Filt<Net>>;

template <class Net, class Show_Arc = Dft_Show_Arc<Net>>
using Net_Iterator = 
  Filter_Iterator<typename Net::Node*, __Net_Iterator<Net>, Show_Arc>;


template <class Net>
typename Net::Flow_Type
remaining_flow(typename Net::Node * src, typename Net::Arc * a) noexcept
{
  return is_residual<Net>(src, a) ? a->flow : a->cap - a->flow;
}

template <typename Node_Info = Empty_Class>
using Net_Node = Graph_Anode<Node_Info>;


  template <class NodeT = Net_Node<Empty_Class>, 
            class ArcT  = Net_Arc<Empty_Class, double>>
struct Net_Graph : public Array_Graph<NodeT, ArcT>
{
  using Net = Net_Graph<NodeT, ArcT>;

  using Base = Array_Graph<NodeT, ArcT>;

  using Base::Base;

  using Graph = Base;
  
  using Arc = ArcT;

  using Node = NodeT;

  using Flow_Type = typename Arc::Flow_Type;

  using Node_Type = typename Node::Node_Type;

  using Arc_Type = typename Arc::Arc_Type;

  mutable Flow_Type Infinity;

  DynList<Arc*> out_arcs(Node * p) const noexcept
  {
    return Aleph::out_arcs<Net_Graph>(p);
  }

      // Nodos a los cuales se les accede desde p
  DynList<Node*> out_nodes(Node * p) const noexcept
  {
    return out_pairs<Net_Graph>(p).template map<Node*>
    ([] (const ArcPair<Net_Graph> & p) { return get<1>(p); });
  }

      // Arcos que llegan a p
  DynList<Arc*> in_arcs(Node * p) const noexcept
  {
    return Aleph::in_arcs<Net_Graph>(p);
  }

      // Nodos desde los cuales se llega a p
  DynList<Node*> in_nodes(Node * p) const noexcept
  {
    return in_pairs<Net_Graph>(p).template map<Node*>
    ([] (const ArcPair<Net_Graph> & p) { return get<1>(p); });
  }

  Flow_Type get_in_cap(Node * node) const noexcept
  {
    Flow_Type sum = 0.0;
    for (__In_Iterator<Net_Graph> it(node); it.has_curr(); it.next())
      sum += it.get_curr()->cap;
    return sum;
  }

  Flow_Type get_out_cap(Node * node) const noexcept 
  {
    Flow_Type sum = 0.0;
    for (__Out_Iterator<Net_Graph> it(node); it.has_curr(); it.next())
      sum += it.get_curr()->cap;
    return sum;
  }

  size_t get_in_degree(Node * p) const noexcept
  {
    return this->in_degree(p); 
  }

  size_t get_out_degree(Node * p) const noexcept
  {
    return this->out_degree(p);
  }

  Flow_Type get_out_flow(Node * node) const noexcept
  {
    Flow_Type sum = 0;
    for (__Out_Iterator<Net_Graph> it(node); it.has_curr(); it.next())
      sum += it.get_curr()->flow;
    return sum;
  }

  Flow_Type get_in_flow(Node * node) const noexcept
  {
    Flow_Type sum = 0;
    for (__In_Iterator<Net_Graph> it(node); it.has_curr(); it.next())
      sum += it.get_curr()->flow;
    return sum;
  }

  bool is_source(Node * node) noexcept { return src_nodes.contains(node); }

  bool is_sink(Node * node) noexcept { return sink_nodes.contains(node); }

  bool is_single_source() const noexcept { return src_nodes.size() == 1; }

  bool is_single_sink() const noexcept { return sink_nodes.size() == 1; }

  bool is_connected(Node * p) const noexcept
  {
    return get_in_degree(p) != 0 or get_out_degree(p) != 0;
  }

  bool check_node(Node * node) noexcept
  { 
    if (not is_connected(node)) 
      return false;
    
    auto out_flow = get_out_flow(node);
    auto in_flow = get_in_flow(node);

    if (is_sink(node)) 
      return out_flow == 0 and in_flow >= 0;

    if (is_source(node))
      return in_flow == 0 and out_flow >= 0;

    return out_flow == in_flow;
  }

private:

  DynSetTree<Node*> src_nodes;
  DynSetTree<Node*> sink_nodes;

public:

  const DynSetTree<Node*> & get_src_nodes() const noexcept { return src_nodes; }

  const DynSetTree<Node*> & get_sink_nodes() const noexcept 
  {
    return sink_nodes; 
  }

public:

  void make_super_source()
  {
    if (src_nodes.size() == 1) 
      return;

    if (src_nodes.size() == 0)
      throw std::domain_error("network has not source node (it has cicles)");

    Node * super_source = insert_node(); 
    src_nodes.for_each([super_source, this] (Node * p)
	      {
		insert_arc(super_source, p, get_out_cap(p));
	      });

    with_super_source = true;
  }

  void unmake_super_source() noexcept
  {
    if (not with_super_source) 
      return;

    assert(src_nodes.size() == 1);

    remove_node(src_nodes.get_item());
    with_super_source = false;
  }

  void make_super_sink()
  {
    if (sink_nodes.size() == 1) 
      return;

    if (sink_nodes.size() == 0)
      throw std::domain_error("network has not sink node (it has cicles)");

    Node * super_sink = insert_node(); 
    sink_nodes.for_each([super_sink, this] (Node * p)
	      {
		insert_arc(p, super_sink, get_in_cap(p));
	      });
    with_super_sink = true;
  }

  void unmake_super_sink() noexcept
  {
    if (not with_super_sink) 
      return;

    assert(sink_nodes.size() == 1);

    remove_node(sink_nodes.get_item());
    with_super_sink = false;
  }

  void make_super_nodes()
  {
    make_super_source();
    try
      {
	make_super_sink();
      }
    catch (std::bad_alloc)
      {
	unmake_super_source();
      }
  }

  void unmake_super_nodes()
  {
    unmake_super_source();
    unmake_super_sink();
  }

  Node * get_source() const noexcept { return src_nodes.get_item(); }

  Node * get_sink() const noexcept { return sink_nodes.get_item(); }

  Node * insert_node(const Node_Type & node_info)
  {
    auto p = Graph::insert_node(node_info); 
    try
      {
	src_nodes.insert(p);
	try
	  {
	    sink_nodes.insert(p);
	    return p;
	  }
	catch (bad_alloc)
	  {
	    src_nodes.remove(p);
	    Graph::remove_node(p);
	    throw;
	  }
      }
    catch (bad_alloc)
      {
	Graph::remove_node(p);
	throw; // propagar excepción 
      }
  }

  Node * insert_node(Node_Type && info = Node_Type()) 
  { 
    return insert_node(info); 
  }

  template <typename ...Args>
  Node * emplace_node(Args && ... args) 
  { 
    return insert_node(Node_Type(args...)); 
  }

  

  Node * insert_node(Node * p)
  {
    Graph::insert_node(p); 
    try
      {
	src_nodes.insert(p);
	try
	  {
	    sink_nodes.insert(p);
	    return p;
	  }
	catch (bad_alloc)
	  {
	    src_nodes.remove(p);
	    Graph::remove_node(p);
	    throw;
	  }
      }
    catch (bad_alloc)
      {
	Graph::remove_node(p);
	throw; // propagar excepción 
      }
  }

  Arc * insert_arc(Node * src_node, Node * tgt_node, 
		   const Flow_Type & cap, const Flow_Type & flow,
		   const typename Arc::Arc_Type & arc_info = Arc_Type())
  {     // inserción en clase base
    auto arc = Graph::insert_arc(src_node, tgt_node, arc_info); 

    src_nodes.remove(tgt_node);  // actualización de source/sink
    sink_nodes.remove(src_node); 

    arc->cap  = cap;            // ajuste capacidad y flujo de arco
    arc->flow = flow;

    if (not arc->check_arc())
      throw std::overflow_error("flow is greater than capacity");

    return arc;
  }

  template <typename ...Args>
  Arc * emplace_arc(Node * src_node, Node * tgt_node, 
		    const Flow_Type & cap, const Flow_Type & flow,
		    Args && ... args)
  {   
    return insert_arc(src_node, tgt_node, cap, flow, Arc_Type(args...));
  }

  Arc * connect_arc(Arc * arc) noexcept
  {     
    Graph::connect_arc(arc); 

    auto src = this->get_src_node(arc);
    auto tgt = this->get_tgt_node(arc);

    src_nodes.remove(tgt);        // elimina destino de src_nodes
    sink_nodes.remove(src);       // elimina fuente de sink_nodes

    return arc;
  }

  Arc * insert_arc(Node * src_node, Node * tgt_node, const Flow_Type & cap)
  {
    return insert_arc(src_node, tgt_node, cap, 0, Arc_Type());
  }

  Arc * insert_arc(Node * src_node, Node * tgt_node, 
		   const typename Arc::Arc_Type & arc_info = Arc_Type())
  {
    return insert_arc(src_node, tgt_node, 0, 0, arc_info);
  }

  virtual void remove_arc(Arc * arc)
  {
    auto src = this->get_src_node(arc);
    auto tgt = this->get_tgt_node(arc);
    if (get_in_degree(tgt) == 1)
      src_nodes.insert(tgt);  // tgt deviene un nodo fuente 

    Graph::remove_arc(arc); // eliminar en clase base

    if (get_out_degree(src) == 0)
      sink_nodes.insert(src); // src deviene un nodo sumidero
  }

  void disconnect_arc(Arc * arc) noexcept
  {
    auto src = this->get_src_node(arc);
    auto tgt = this->get_tgt_node(arc);
    if (get_in_degree(tgt) == 1)
      src_nodes.insert(tgt); // tgt deviene un nodo fuente 

    Graph::disconnect_arc(arc); // desconeción en clase base

    if (get_out_degree(src) == 0)
      sink_nodes.insert(src); // src deviene un nodo sumidero
  }

  virtual void remove_node(Node * p) noexcept
  {
    Graph::remove_node(p);  // eliminación en clase base
    src_nodes.remove(p);
    sink_nodes.remove(p);
  }

  Net_Graph(const Net_Graph & net) 
    : Array_Graph<NodeT, ArcT>::Array_Graph(),
      Infinity(numeric_limits<typename Arc::Flow_Type>::max()),
      with_super_source(net.with_super_source),
      with_super_sink(net.with_super_sink)
  {
    copy_graph(*this, net, false); // copia sin mapear
    
    using Pair = std::pair<typename Net_Graph::Arc*, typename Net_Graph::Arc*>;
    zip(this->arcs(), net.arcs()).for_each([] (const Pair & p)
      {
	auto atgt = p.first;
	auto asrc = p.second;
	atgt->cap = asrc->cap;
	atgt->flow = asrc->flow;
      });
  }

  void set_cap(Arc * arc, const Flow_Type & cap)
  {
    if (cap < arc->flow)
      throw std::out_of_range("capacity value is smaller than flow");

    arc->cap = cap;
  }

  void set_flow(Arc * arc, const Flow_Type & flow)
  {
    if (flow > arc->cap)
      throw std::out_of_range("flow value is greater than capacity");

    arc->flow = flow;
  }

  const Flow_Type & get_flow(Arc * arc) const noexcept { return arc->flow; }

  const Flow_Type & get_cap(Arc * arc) const noexcept { return arc->cap; }

  void reset() noexcept
  {
    for (Arc_Iterator<Net_Graph> it(*this); it.has_curr(); it.next())
      it.get_curr()->flow = 0;
  }

  bool check_network() noexcept
  {
    return this->nodes().all([this] (Node * p) { return check_node(p); }) and
      this->arcs().all([] (Arc * a) { return a->check_arc(); }) and
      get_out_flow(get_source()) == get_in_flow(get_sink());
  }

  Flow_Type flow_value() const noexcept
  {
    assert(get_in_flow(get_source()) == get_out_flow(get_sink()));
    return get_out_flow(get_source()); 
  }

  bool with_super_source; 

  bool with_super_sink;

  Net_Graph() 
    : Infinity(numeric_limits<typename Arc::Flow_Type>::max()),
      with_super_source(false), with_super_sink(false)
  { /* empty */ }

  friend ostream & operator << (ostream & s, const Path<Net_Graph> & path)
  {
    if (path.is_empty())
      return s << "Path is Empty";

    const Net_Graph & net = path.get_graph();
    typename Path<Net_Graph>::Iterator it(path);
    s << it.get_current_node()->get_info();
    for (; it.has_current_arc(); it.next())
      {
	typename Net_Graph::Arc * a = it.get_current_arc();
	s << "(" << a->cap << "," << a->flow << ")" 
	  << net.get_connected_node(a, it.get_current_node())-> get_info();
      }
    return s;
  }
};

template <class Net>
using Parc = tuple<typename Net::Arc*, bool>; // 2do campo dice si normal

template <class Net>
using SemiPath = tuple<bool, typename Net::Flow_Type, DynList<Parc<Net>>>;

template <class Net>
inline void print(const DynList<Parc<Net>> & sp)
{
  if (sp.is_empty())
    {
      cout << "Semi path is Empty";
      return;
    }

  for (typename DynList<Parc<Net>>::Iterator it(sp); it.has_curr(); it.next())
    {
      const Parc<Net> & pa = it.get_curr();
      auto a = get<0>(pa);
      auto s = (typename Net::Node *) a->src_node;
      auto t = (typename Net::Node *) a->tgt_node;
      cout << s->get_info() << "(" << a->flow << "," << a->cap << ")" 
	   << t->get_info() << " " << (get<1>(pa) ? "Normal" : "Reduced") 
	   << endl;
    }
}

    template <class Net>
typename Net::Flow_Type increase_flow(Net & net, const Path<Net> & path)
{
  typename Net::Flow_Type slack = net.Infinity; // eslabón mínimo
  using Tuple = tuple<typename Net::Node*, typename Net::Arc*>;

      // calcular el eslabón mínimo del camino de aumento
  for (typename Path<Net>::Iterator it(path); it.has_current_arc(); it.next())
    {
      Tuple t = it.get_pair();
      auto p = get<0>(t);
      auto arc = get<1>(t);
      const auto w = remaining_flow<Net>(p, arc);
      if (w < slack)
        slack = w;
    }

      // aumentar el flujo de la red por el camino de aumento
  for (typename Path<Net>::Iterator it(path); it.has_current_arc(); it.next())
    {
      auto t = it.get_pair();
      auto p = get<0>(t);
      auto arc = get<1>(t);

      if (is_residual<Net>(p, arc))
	arc->flow -= slack; 
      else
	arc->flow += slack; 

      assert(arc->check_arc());
    }

  return slack;
}


    template <class Net>
void increase_flow(Net & net, 
		   const DynList<Parc<Net>> & semi_path,
		   const typename Net::Flow_Type slack)
{
      // aumentar el flujo de la red por el camino de aumento
  for (typename DynList<Parc<Net>>::Iterator it(semi_path); it.has_curr(); 
	 it.next())
    {
      auto p = it.get_curr();
      auto arc = get<0>(p);
      if (get<1>(p)) // es arco de normal?
	arc->flow += slack; 
      else
	arc->flow -= slack; 

      assert(arc->check_arc());
    }
  assert(net.check_network());
}
  
    template <class Net>
void decrease_flow(Net & net, const DynList<Parc<Net>> & semi_path,
		   typename Net::Flow_Type slack)
{
  increase_flow(net, semi_path, slack);
}


  template <class Net, template <typename T> class Q>
class Find_Aumenting_Path
{
  const Net & net;

    // returna nodo end si se encuentra un camino
  typename Net::Node * search(typename Net::Node* start, 
			      typename Net::Node* end,
			      typename Net::Flow_Type min_slack)
  {
    using Itor = Net_Iterator<Net>;
    net.reset_nodes();
    net.reset_arcs();
    
    start->set_state(Processed);
    Q<typename Net::Arc*> q;
    for (Itor it(start); it.has_curr(); it.next())
      {
	auto a = it.get_curr();
	if (remaining_flow<Net>(start, a) < min_slack)
	  continue;
	auto tgt = net.get_connected_node(a, start);
	tgt->set_state(Processing);
	a->set_state(Processing);
	q.put(a);
      }

    typename Net::Node * curr = nullptr;
    while (not q.is_empty())
      {
	auto arc = q.get();
	assert(arc->state() == Processing);
	arc->set_state(Processed);
	
	auto s = net.get_src_node(arc);
	auto t = net.get_tgt_node(arc);
	
	if (s->state() == Processed and t->state() == Processed)
	  continue;
	
	curr = s->state() == Processed ? t : s;
	assert(curr->state() == Processing);
	curr->set_state(Processed);
	NODE_COOKIE(curr) = net.get_connected_node(arc, curr);

	if (curr == end)
	  return curr;

	for (Itor it(curr); it.has_curr(); it.next()) 
	  {
	    auto a = it.get_curr();
	    if (remaining_flow<Net>(curr, a) < min_slack)
	      continue;
	    a->set_state(Processing);

	    auto tgt = net.get_connected_node(a, curr);
	    if (tgt->state() == Processed)
	      continue;

	    if (tgt->state() != Processed)
	      {
		q.put(a);
		tgt->set_state(Processing);
	      }
	    else
	      a->set_state(Processed);
	  }
      } // end while

    return nullptr;
  }

  Path<Net> find(typename Net::Node * start, typename Net::Node * end,
		 const typename Net::Flow_Type & min_slack = 0.0)
  {
    auto curr = search(start, end, min_slack);

    Path<Net> ret(net);
    if (not curr)
      return ret;

    assert(curr == end);

    while (curr != start)
      {
	ret.insert(curr);
	curr = (typename Net::Node*) NODE_COOKIE(curr);
      }
    ret.insert(start);

    return ret;
  }

  SemiPath<Net> find_path(typename Net::Node * start,
			  typename Net::Node * end,
			  typename Net::Flow_Type min_slack = 0.0)
  {
    auto t = search(start, end, min_slack);
    if (not t)
      return make_tuple(false, 0, DynList<Parc<Net>>());

    assert(t == end);

    DynList<Parc<Net>> semi_path;
    auto m = std::numeric_limits<typename Net::Flow_Type>::max();
    while (t != start)
      {
	auto s = (typename Net::Node*) NODE_COOKIE(t);
	auto a_ptr = net.arcs(t).find_ptr([s, t] (typename Net::Arc * a)
          {
	    return ((a->src_node == s and a->tgt_node == t) or 
		    (a->src_node == t and a->tgt_node == s));
	  });
	assert(a_ptr);
	auto a = *a_ptr;
	bool normal = a->tgt_node == t;
	auto slack = normal ? a->cap - a->flow : a->flow;
	m = std::min(m, slack);
	semi_path.insert(make_tuple(a, normal));
	t = s;
      }

    return make_tuple(true, m, move(semi_path));
  }

public:

  SemiPath<Net> find_aum_path(typename Net::Flow_Type min_slack = 0.0)
  {
    return find_path(net.get_source(), net.get_sink(), min_slack);
  }

  SemiPath<Net> find_dec_path(typename Net::Flow_Type min_slack = 0.0)
  {
    return find_path(net.get_sink(), net.get_source(), min_slack);
  }

  Find_Aumenting_Path(const Net & __g) : net(__g)
      {
	// empty
      }

  Path<Net> operator () (typename Net::Node *    start, 
			 typename Net::Node *    end,
			 typename Net::Flow_Type min_slack = 0) 
  {
    return find(start, end, min_slack);
  }

  SemiPath<Net> operator () (typename Net::Flow_Type min_slack = 0) 
  {
    return find_aum_path(min_slack);
  }

  typename Net::Flow_Type 
  semi_path(typename Net::Node *            start, 
	    typename Net::Node *            end,
	    DynList<Parc<Net>> &            semi_path,
	    const typename Net::Flow_Type & min_slack = 0) 
  {
    return find_semi_path(start, end, semi_path, min_slack);
  }
};


template <class Net>
using Find_Aumenting_Path_DFS = Find_Aumenting_Path<Net, DynListStack>;


template <class Net>
using Find_Aumenting_Path_BFS = Find_Aumenting_Path<Net, DynListQueue>;


template <class Net, template <typename T> class Q>
Path<Net> find_aumenting_path(const Net &                     net,
			      const typename Net::Flow_Type & min_slack)
{
  auto s = net.get_source();
  auto t = net.get_sink();
  return Find_Aumenting_Path<Net, Q> (net) (s, t, min_slack);
}


template <class Net>
Path<Net> find_aumenting_path_dfs(const Net &                     net,
				  const typename Net::Flow_Type & min_slack)
{
  return find_aumenting_path<Net, DynListStack> (net, min_slack);
}


template <class Net>
Path<Net> find_aumenting_path_bfs(const Net &                     net,
				  const typename Net::Flow_Type & min_slack)
{
  return find_aumenting_path<Net, DynListQueue> (net, min_slack);
}


template <class Net, template <typename T> class Q>
SemiPath<Net> find_aumenting_semi_path(const Net &                     net,
				       const typename Net::Flow_Type & slack)
{
  return Find_Aumenting_Path<Net, Q> (net). find_aum_path(slack);
}


template <class Net> 
SemiPath<Net> 
find_aumenting_semi_path_dfs(const Net &                     net,
			     const typename Net::Flow_Type & slack)
{
  return find_aumenting_semi_path<Net, DynListStack> (net, slack);
}


template <class Net>
SemiPath<Net> 
find_aumenting_semi_path_bfs(const Net &                     net,
			     const typename Net::Flow_Type & slack)
{
  return find_aumenting_semi_path<Net, DynListQueue> (net, slack);
}


template <class Net, template <typename T> class Q>
SemiPath<Net>
find_decrementing_path(const Net &                     net,
		       const typename Net::Flow_Type & slack)
{
  return Find_Aumenting_Path<Net, Q>(net).find_dec_path(slack);
}


template <class Net>
SemiPath<Net>
find_decrementing_path_dfs(const Net &                     net,
			   const typename Net::Flow_Type & slack)
{
  return find_decrementing_path<Net, DynListStack> (net, slack);
}


template <class Net>
SemiPath<Net> 
find_decrementing_path_bfs(const Net &                     net,
			   const typename Net::Flow_Type & slack)
{
  return find_decrementing_path<Net, DynListQueue> (net, slack);
}


// Residual node for using is preflow-push algorithms
template <class Net>
struct PP_Res_Node : public Net_Node<Empty_Class>
{
  using Base = Net_Node<Empty_Class>;
  using Base::Base;

  typename Net::Flow_Type in_flow;
  typename Net::Flow_Type out_flow;
};

template <class Net>
struct __Res_Arc : public Net_Arc<Empty_Class, typename Net::Flow_Type>
{
  using Base = Net_Arc<Empty_Class, typename Net::Flow_Type>;
  using Base::Base;

  typename Net::Arc * img = nullptr; // nullptr indicates residual arc
  __Res_Arc * dup = nullptr;

  bool is_residual() const { return img == nullptr; }
};


template <class Net>
using AP_Res_Net = Array_Digraph<Net_Node<Empty_Class>, __Res_Arc<Net>>;

    // Residual net for preflow-push algorithms
template <class Net>
using PP_Res_Net = Array_Digraph<PP_Res_Node<Net>, __Res_Arc<Net>>;

// precondition: a->src and a->tgt are mapped to residual net
template <class Net> inline
void create_residual_arc(const Net & net, PP_Res_Net<Net> & rnet, 
			 typename Net::Arc * a)
{
  using Rnet = PP_Res_Net<Net>;
  auto s = net.get_src_node(a);
  auto t = net.get_tgt_node(a);

  assert(NODE_COOKIE(s) and NODE_COOKIE(t));

  auto src = (typename Rnet::Node*) NODE_COOKIE(s);
  auto tgt = (typename Rnet::Node*) NODE_COOKIE(t);

  auto arc = rnet.insert_arc(src, tgt);
  auto dup = rnet.insert_arc(tgt, src);

  arc->img = a;          // mark it as not residual
  arc->cap = a->cap;
  arc->flow = a->flow;
  arc->dup = dup;

  dup->cap = arc->cap;
  dup->flow = arc->cap - arc->flow;
  dup->dup = arc;
}

template <class Rnet>
struct Res_F
{
  Res_F(typename Rnet::Node *) noexcept {}
  Res_F() noexcept {}

  bool operator () (typename Rnet::Arc * a) const
  {
    return a->cap > a->flow;
  }
};

template <class Net> inline
tuple<PP_Res_Net<Net>, typename PP_Res_Net<Net>::Node*, 
      typename PP_Res_Net<Net>::Node*>
preflow_create_residual_net(Net & net)
{
  using Rnet = PP_Res_Net<Net>;
  net.reset_nodes();
  Rnet rnet;

  for (typename Net::Node_Iterator it(net); it.has_curr(); it.next())
    {
      auto p = it.get_curr();
      auto q = rnet.insert_node();
      q->in_flow = net.get_in_flow(p);
      q->out_flow = net.get_out_flow(p);
      map_nodes<typename Rnet::Node, typename Net::Node>(q, p);
    }

  for (typename Net::Arc_Iterator it(net); it.has_curr(); it.next())
    create_residual_arc(net, rnet, it.get_curr());
    
  return make_tuple(std::move(rnet), 
		    (typename Rnet::Node*) NODE_COOKIE(net.get_source()), 
		    (typename Rnet::Node*) NODE_COOKIE(net.get_sink()));
}

template <class Rnet> inline
void update_flow(const Rnet & rnet)
{
  for (typename Rnet::Arc_Iterator it(rnet); it.has_curr(); it.next())
    {
      auto arc = it.get_curr();
      auto img = arc->img;
      if (img == nullptr)
	continue;
      img->flow = arc->flow;
    }
}

template <class Net, 
	  template <class> class Find_Path>
typename Net::Flow_Type aumenting_path_maximum_flow(Net & net)
{
  if (not (net.is_single_source() and net.is_single_sink()))
    throw std::domain_error("Network is not single source and single sink");

  while (true) // mientras exista un camino de aumento
    {
      SemiPath<Net> semi_path = Find_Path<Net> (net) ();
      if (not get<0>(semi_path))
	break;
	  
      increase_flow <Net> (net, get<2>(semi_path), get<1>(semi_path));
    }

  return net.get_out_flow(net.get_source());
}


template <class Net>
typename Net::Flow_Type ford_fulkerson_maximum_flow(Net & net)
{
  return aumenting_path_maximum_flow <Net, Find_Aumenting_Path_DFS> (net);
} 


template <class Net> 
struct Ford_Fulkerson_Maximum_Flow 
{
  typename Net::Flow_Type operator () (Net & net) const
  {
    return ford_fulkerson_maximum_flow(net);
  }
};

template <class Net>
typename Net::Flow_Type edmonds_karp_maximum_flow(Net & net)
{
  return aumenting_path_maximum_flow <Net, Find_Aumenting_Path_BFS> (net);
}


template <class Net> 
struct Edmonds_Karp_Maximum_Flow
{
  typename Net::Flow_Type operator () (Net & net) const
  {
    return edmonds_karp_maximum_flow<Net>(net);
  }
};

    template <class Net> static inline
bool is_node_active(const Net & net, typename Net::Node * p) 
{
  return net.get_in_flow(p) != net.get_out_flow(p); 
}

template <class Rnet> static inline
bool is_node_active(typename Rnet::Node * p) 
{
  return p->in_flow != p->out_flow; 
}


    template <class Net> static inline
long & node_height(typename Net::Node * p) { return NODE_COUNTER(p); }


    template <class Net> static inline
void init_height_in_nodes(Net & net)
{
  Graph_Traverse_BFS<Net, Node_Arc_Iterator<Net>>(net).
    exec(net.get_sink(), [&net] (typename Net::Node * p, typename Net::Arc * a)
	 {
	   if (a)
	     node_height<Net>(p) = node_height<Net>(net.get_tgt_node(a)) + 1;
	   return true;
	 });
}


    template <class Q_Type> static inline
void put_in_active_queue(Q_Type & q, typename Q_Type::Item_Type & p)
{
  if (NODE_BITS(p).get_bit(Aleph::Maximum_Flow))
    return; // the node is already into the queue
  NODE_BITS(p).set_bit(Aleph::Maximum_Flow, true);
  q.put(p);
}


    template <class Q_Type> static inline
typename Q_Type::Item_Type get_from_active_queue(Q_Type & q)
{
  auto p = q.get();
  assert(NODE_BITS(p).get_bit(Aleph::Maximum_Flow));
  NODE_BITS(p).set_bit(Aleph::Maximum_Flow, false);
  return p;
}


   template <class Q_Type> static inline
void remove_from_active_queue(Q_Type & q, typename Q_Type::Item_Type & p)
{
  assert(NODE_BITS(p).get_bit(Aleph::Maximum_Flow));
  NODE_BITS(p).set_bit(Aleph::Maximum_Flow, false);
  q.remove(p);
}


template <class Net, class Q_Type> typename Net::Flow_Type
generic_preflow_vertex_push_maximum_flow(Net & net)
{
  if (not (net.is_single_source() and net.is_single_sink()))
    throw std::domain_error("Network is not single source and single sink");

  net.reset_nodes();         // especially assures that counters are in zero
  init_height_in_nodes(net); // set height to minimum distance in nodes to sink

  auto source = net.get_source();
  auto sink   = net.get_sink();
  node_height<Net>(source) = net.vsize(); // except the source
  const auto Max = numeric_limits<long>::max();

  using Itor = __Net_Iterator<Net>;
  Q_Type q;  
  for (Itor it(source); it.has_curr(); it.next()) // initial preflow
    {
      auto arc  = it.get_curr();  
      arc->flow = arc->cap; // saturate arc
      auto tgt = net.get_tgt_node(arc);
      put_in_active_queue(q, tgt);
    }

  while (not q.is_empty()) // while there are active nodes
    {
      auto src = get_from_active_queue(q);
      auto excess = net.get_in_flow(src) - net.get_out_flow(src);
      long m = Max;
      bool was_eligible_arc = false;
      for (Itor it(src); it.has_curr() and excess > 0; it.next())
	{
	  auto arc = it.get_curr();
	  auto tgt = net.get_connected_node(arc, src);
	  if (node_height<Net>(src) != node_height<Net>(tgt) + 1)
	    {
	      m = std::min(m, node_height<Net>(tgt));
	      continue;
	    }

	  was_eligible_arc = true;
	  typename Net::Flow_Type flow_to_push;
	  if (is_residual<Net>(src, arc))
	    {
	      flow_to_push = std::min(arc->flow, excess);
	      arc->flow -= flow_to_push;
	    }
	  else
	    {
	      flow_to_push = std::min(arc->cap - arc->flow, excess);
	      arc->flow += flow_to_push;
	    }
	  excess -= flow_to_push;
	  if (tgt != source and tgt != sink)
	    {
	      assert(is_node_active(net, tgt));
	      put_in_active_queue(q, tgt);
	    }
	}

      if (excess > 0) // is src still active
	{
	  if (not was_eligible_arc)
	    node_height<Net>(src) = m + 1;
	  put_in_active_queue(q, src);
	}
    }

  assert(net.check_network());

  return net.flow_value(); 
}


template <class Rnet> inline
void init_height_in_nodes(Rnet & rnet, typename Rnet::Node * sink)
{
  Graph_Traverse_BFS<Rnet, Node_Arc_Iterator<Rnet>>(rnet).
    exec(sink, [&rnet] (typename Rnet::Node * p, typename Rnet::Arc * a)
	 {
	   if (a)
	     node_height<Rnet>(p) = node_height<Rnet>(rnet.get_src_node(a)) + 1;
	   return true;
	 });
}


template <class Net> typename Net::Flow_Type
fifo_preflow_maximum_flow(Net & net)
{
  using Node = typename Net::Node;
  return generic_preflow_vertex_push_maximum_flow
    <Net, DynListQueue<Node*>>(net);
}

template <class Net> 
struct Fifo_Preflow_Maximum_Flow
{
  typename Net::Flow_Type 
  operator () (Net & net) const
  {
    return fifo_preflow_maximum_flow(net);
  }
};


    template <class Net> 
struct Compare_Height
{
  bool operator () (typename Net::Node * n1, typename Net::Node * n2) const
  {
    return node_height<Net>(n1) > node_height<Net>(n2);
  }
};

template <class Net> typename Net::Flow_Type
heap_preflow_maximum_flow(Net & net)
{
  using Node = typename Net::Node;
  return generic_preflow_vertex_push_maximum_flow
    <Net, DynBinHeap<Node*, Compare_Height<Net>>>(net);
}

  template <class Net> 
struct Heap_Preflow_Maximum_Flow
{
  typename Net::Flow_Type operator () (Net & net) const
  {
    return heap_preflow_maximum_flow(net);
  }
};

    template <class Net> typename Net::Flow_Type
random_preflow_maximum_flow(Net & net)
{
  using Node = typename Net::Node;
  return generic_preflow_vertex_push_maximum_flow<Net, Random_Set<Node*>>(net);
}


      template <class Net> 
struct Random_Preflow_Maximum_Flow
{
  typename Net::Flow_Type operator () (Net & net) const
  {
    return random_preflow_maximum_flow(net);
  }
};


template <class Net, template <class> class Maxflow>
typename Net::Flow_Type min_cut(Net &                             net, 
                                DynSetTree<typename Net::Node*> & vs, 
                                DynSetTree<typename Net::Node*> & vt,
                                DynList<typename Net::Arc *> &    cuts,
                                DynList<typename Net::Arc *> &    cutt)
{
  Maxflow <Net> () (net); // calcula flujo máximo

  typename Net::Node * source = net.get_source();

  // Recorre en profundidad la red residual desde source. Todo lo
  // alcanzable pertenece a vs y por tanto es insertado
  (Graph_Traverse_BFS<Net, Net_Iterator<Net>> (net))
   (source, [&vs] (typename Net::Node * p)
      {
	vs.insert(p); return true;
      });

  // Calcula vt por el complemento
  const size_t size_vt = net.get_num_nodes() - vs.size();
  for (Node_Iterator<Net> it(net); it.has_curr() and 
         vt.size() < size_vt; it.next())
    {
      auto p = it.get_curr();
      if (p->state() == Unprocessed)
        vt.insert(p);
    }     

  for (Arc_Iterator<Net> it(net); it.has_curr(); it.next())
    {
      auto arc = it.get_curr();
      if (arc->flow == 0) // ¿candidato a arco de retroceso?
        if (vt.contains(net.get_src_node(arc)) and // ¿de vt hacia vs?
            vs.contains(net.get_tgt_node(arc)) > 0)
          {
            cutt.insert(arc);
            continue;
          }

      if (arc->flow == arc->cap) // ¿candidato a arco de cruce?
        if (vs.contains(net.get_src_node(arc)) and // ¿de vs hacia vt?
            vt.contains(net.get_tgt_node(arc)) > 0)
          cuts.insert(arc); 
    }

  return net.flow_value();
}


  template <class Net, 
            template <class> class Maxflow = Heap_Preflow_Maximum_Flow>
struct Min_Cut
{
  typename Net::Flow_Type operator () (Net &                             net, 
                                       DynSetTree<typename Net::Node*> & vs, 
                                       DynSetTree<typename Net::Node*> & vt,
                                       DynList<typename Net::Arc *> &    cuts,
                                       DynList<typename Net::Arc *> &    cutt)
  {
    return min_cut <Net, Maxflow> (net, vs, vt, cuts, cutt);
  }
};


} // end namespace Aleph

# endif // TPL_NET_H
