
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

# ifndef TPL_GRAPH_H
# define TPL_GRAPH_H

# include <memory>
# include <bitArray.H>
# include <tpl_dynArray.H>
# include <tpl_sort_utils.H>
# include <tpl_dynMapTree.H>
# include <tpl_dynDlist.H>
# include <tpl_treapRk.H>
# include <filter_iterator.H>
# include <aleph-graph.H> 
# include <graph-dry.H>

    
using namespace Aleph;

namespace Aleph {

template <typename Node_Info> struct Graph_Node; 

template <typename Arc_Info> struct Graph_Arc; 

class Arc_Node;

    template <class GT> 
class Path;

    template <typename __Graph_Node, typename __Graph_Arc> 
class List_Graph;

    template <typename __Graph_Node, typename __Graph_Arc> 
class List_Digraph;

    template <class GT> 
class Mat_Graph;

    template <typename MT, typename Entry_Info, typename Copy> 
class Ady_MaT;


    template <typename __Node_Info = Empty_Class> 
struct Graph_Node 
      : public Dlink,
	public GTNodeCommon<__Node_Info>
{
  friend class GTNodeCommon<__Node_Info>;
  friend class Arc_Node;

  using Base = GTNodeCommon<__Node_Info>;
  using Node_Info = __Node_Info; 
   
  Graph_Node(const Node_Info & info) noexcept : Base(info) { /* empty */ }

  Graph_Node(Node_Info && info = Node_Info()) noexcept : Base(move(info))
  { 
    /* empty */ 
  }

  Graph_Node(const Graph_Node & node) noexcept : Graph_Node(node.node_info) 
  {
    // empty
  }

  Graph_Node & operator = (const Graph_Node & node) 
  noexcept(std::is_nothrow_copy_assignable<Node_Info>::value)
  {
    if (&node == this)
      return *this;
    this->node_info = node.node_info;
    return *this;
  }

  Graph_Node(Graph_Node * node) 
  noexcept(std::is_nothrow_copy_assignable<Node_Info>::value)
  : Base(node->get_info())
  { 
    /* empty */ 
  }

  Dlink arc_list; 
};

    template <typename __Arc_Info = Empty_Class> 
struct Graph_Arc 
      : public Dlink,
	public GTArcCommon<__Arc_Info>
{
  friend class GTArcCommon<__Arc_Info>;
  
  using Base = GTArcCommon<__Arc_Info>;

  using Arc_Info = __Arc_Info; 

  Arc_Node * src_arc_node = nullptr; // pointer to source node
  Arc_Node * tgt_arc_node = nullptr; // pointer to target node

  Graph_Arc(const Arc_Info & info) 
  noexcept(noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value))
  : Base(info)
  {
    /* empty */ 
  }

  Graph_Arc(Arc_Info && info = Arc_Info()) 
  noexcept(noexcept(std::is_nothrow_move_constructible<Arc_Info>::value))
  : Base(move(info))
  {
    /* empty */ 
  }

  Graph_Arc(const Graph_Arc & arc) 
  noexcept(noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value))
  : Graph_Arc(arc.arc_info) 
  {
    /* empty */ 
  }

  Graph_Arc & operator = (const Graph_Arc & arc) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
  {
    if (&arc == this)
      return *this;
    this->arc_info = arc.arc_info;
    return *this;
  }
};

class Arc_Node : public Dlink
{
public:
  void * arc = nullptr;
  Arc_Node() noexcept : arc(nullptr) {}
  Arc_Node(void * __arc) noexcept : arc(__arc) {}
}; 

  template <typename __Graph_Node = Graph_Node<unsigned long>,
	    typename __Graph_Arc  = Graph_Arc<unsigned long>>
class List_Graph
    : public GraphCommon<List_Graph<__Graph_Node, __Graph_Arc>,
			 __Graph_Node, __Graph_Arc>
{
public:

  using GT = List_Graph; 

  using Node = __Graph_Node; 
  using Arc = __Graph_Arc; 
  
  using Node_Type = typename Node::Node_Type; 

  using Arc_Type = typename Arc::Arc_Type;

  friend class GraphCommon<List_Graph<__Graph_Node, __Graph_Arc>,
			   __Graph_Node, __Graph_Arc>;

  using CommonBase = GraphCommon<List_Graph<__Graph_Node, __Graph_Arc>,
				 __Graph_Node, __Graph_Arc>;

  using CommonBase::insert_node;
  using CommonBase::insert_arc;

private:

  Dlink  node_list; // lista de nodos
  Dlink  arc_list;  // lista de arcos

  static Node * dlink_to_node(Dlink * p) noexcept { return (Node*) p; }

  static Arc * dlink_to_arc(Dlink * p) noexcept { return (Arc*) p; }

  static Arc_Node * dlink_to_arc_node(Dlink * p) noexcept 
  {
    return (Arc_Node*) p; 
  }

  static Arc * void_to_arc(Arc_Node * arc_node) noexcept
  {
    return (Arc*) arc_node->arc; 
  }

public:
  
  virtual Node * insert_node(Node * node) noexcept
  {
    this->num_nodes++;
    node_list.append(node);
    return node;
  }

  virtual void remove_node(Node * node) noexcept
  {
    if (not this->digraph)
      while (not node->arc_list.is_empty()) // remove each arc related to node
	{     
	  Arc_Node * arc_node = dlink_to_arc_node(node->arc_list.get_next());
	  Arc * arc = void_to_arc(arc_node); 
	  remove_arc(arc);
	}
    else    // Scan all the arcs and remove those related to node
      for (Arc_Iterator it(*this); it.has_current();)
	{
	  Arc * a = it.get_current();
	  if (this->get_src_node(a) == node or this->get_tgt_node(a) == node)
	    {
	      it.next();
	      remove_arc(a);
	    }
	  else
	    it.next();
	}

    // At this point the node has not more arcs
    node->del(); // unlink it from arc_list
    this->num_nodes--; 
    delete node; 
  }

  Node * get_first_node() const
  {
    if (this->num_nodes == 0)
      throw std::range_error("Graph has not nodes");

    return dlink_to_node(const_cast<Dlink&>(node_list).get_next());
  }

  Arc * get_first_arc(Node * node) const
  {
    if (get_num_arcs(node) == 0)
      throw std::range_error("node has not arcs");

    void * arc = dlink_to_arc_node(node->arc_list.get_next())->arc;
    return reinterpret_cast <Arc *> (arc);
  }

private:
  
  Arc * insert_arc(Node * src_node, Node * tgt_node, void * a)
  {
    Arc * arc     =  (Arc *) a;
    arc->src_node = src_node;
    arc->tgt_node = tgt_node;

      // paso 3: (parcial): apartar Arc_Node de src_node  
    unique_ptr<Arc_Node> src_arc_node (new Arc_Node (arc));

      // paso 2: si es grafo ==> apartar Arc_Node de tgt_node
    if (not this->digraph) // si es digrafo ==> no insertar en otro nodo
      {     // inserción en nodo destino 
	if (src_node == tgt_node) // ¿es un lazo?
	  arc->tgt_arc_node = src_arc_node.get(); 
	else
	  {     // apartar arco nodo para tgt_node 
	    unique_ptr<Arc_Node> tgt_arc_node(new Arc_Node(arc)); 

              // inserción en lista de adyacencia de tgt_node
	    arc->tgt_arc_node = tgt_arc_node.get();    
	    tgt_node->arc_list.append(tgt_arc_node.get());
	    tgt_node->num_arcs++;
	    tgt_arc_node.release(); 
	  }
      }

      // paso 3 (resto): inserción en lista adyacencia src_node 
    arc->src_arc_node = src_arc_node.get();
    src_node->arc_list.append(src_arc_node.get());
    src_node->num_arcs++;

    arc_list.append(arc); //paso 4:insertar en lista arcos grafo 
    this->num_arcs++;
    src_arc_node.release();

    return arc;  
  }

public:

  virtual void remove_arc(Arc * arc) noexcept
  {      // paso 1: eliminar Arc_node de src_node
    Node * src_node         = this->get_src_node(arc); 
    Arc_Node * src_arc_node = arc->src_arc_node;

    src_arc_node->del();  // desenlaza src_node de la lista de nodos
    src_node->num_arcs--; // decrementa contador de arcos de src_node
    delete src_arc_node;  // entrega memoria

    if (not this->digraph) 
      {     // eliminación arco en nodo destino 
	Node * tgt_node = this->get_tgt_node(arc); 
	if (src_node != tgt_node) // verificar eliminación de ciclo 
	  {  // paso 2: eliminar Arc_node de tgt_node
	    Arc_Node * tgt_arc_node = arc->tgt_arc_node;
	    tgt_arc_node->del(); 
	    tgt_node->num_arcs--;
	    delete tgt_arc_node;
	  }
      }

    // eliminación de arco del grafo 
    arc->del(); // desenlazar arc de lista de arcos de grafo
    this->num_arcs--;
    delete arc;
  }

  virtual void disconnect_arc(Arc * arc) noexcept
  {
    Node * src_node         = this->get_src_node(arc); 
    Arc_Node * src_arc_node = arc->src_arc_node;
    src_arc_node->del();   // desenlaza src_node de la lista de nodos
    src_node->num_arcs--;  // decrementa contador de arcos de src_node

    if (not this->digraph) 
      {     // eliminación arco en nodo destino 
	Node * tgt_node = this->get_tgt_node(arc); 
	if (src_node != tgt_node) // verificar eliminación de ciclo 
	  {  // paso (2): eliminación del Arc_node del nodo destino tgt_node
	    Arc_Node * tgt_arc_node = arc->tgt_arc_node;
	    tgt_arc_node->del(); 
	    tgt_node->num_arcs--;
	  }
      }

    // eliminación de arco del grafo 
    arc->del(); // desenlazar arc de la lista de arcos del grafo
    this->num_arcs--;
  }

  virtual Arc * connect_arc(Arc * arc) noexcept
  {
    Node * src_node         = this->get_src_node(arc); 
    Node * tgt_node         = this->get_tgt_node(arc); 
    Arc_Node * src_arc_node = arc->src_arc_node;
    Arc_Node * tgt_arc_node = arc->tgt_arc_node;

    if (not this->digraph) // si es digrafo ==> no hay que insertar en otro nodo
      {     // inserción en nodo destino 
	if (src_node != tgt_node) // verificar si se trata de un ciclo
	  {     // inserción en lista de adyacencia de tgt_node
	    tgt_node->arc_list.append(tgt_arc_node);
	    tgt_node->num_arcs++;
	  }
      }

    src_node->arc_list.append(src_arc_node);
    src_node->num_arcs++;
    arc_list.append(arc);
    this->num_arcs++;

    return arc;
  }  

  Arc * get_first_arc() const 
  { 
    if (this->get_num_arcs() == 0)
      throw std::range_error("Graph has not arcs");

    return dlink_to_arc(const_cast<Dlink&>(arc_list).get_next());
  }

      template <class Compare> inline
  void sort_nodes(Compare & cmp) noexcept
  {
    Cmp_Dlink_Node<List_Graph, Compare> c = cmp;
    mergesort(node_list, c);
  }

      template <class Compare> inline 
  void sort_nodes(Compare && cmp = Compare()) noexcept
  {
    sort_nodes(cmp);
  }

      template <class Compare> inline
  void sort_arcs(Compare & cmp) noexcept
  {
    Cmp_Dlink_Arc<List_Graph, Compare> c = cmp;
    mergesort(arc_list, c);
  }

      template <class Compare> inline 
  void sort_arcs(Compare && cmp = Compare()) noexcept
  {
    sort_arcs(cmp);
  }

  List_Graph(const List_Graph & g)
  {
    copy_graph(*this, g);
  }

  List_Graph(List_Graph && g) noexcept
  {
    swap(g);
  }

  List_Graph & operator = (const List_Graph & g)
  {
    if (this == &g)
      return *this;

    copy_graph(*this, g);
    return *this;
  }
  
  List_Graph & operator = (List_Graph && g) noexcept
  {
    swap(g);
    return *this;
  }

  virtual ~List_Graph()
  {
    clear_graph(*this); 
  }

  struct Node_Iterator : public GTNodeIterator<List_Graph>
  {
    using Base = GTNodeIterator<List_Graph>;
    
    Node_Iterator(const List_Graph & g)
      : Base(const_cast<Dlink&>(g.node_list))
    {
      // empty
    }
  };

  class Node_Arc_Iterator : public Dlink::Iterator
  {
    Node * src_node; 
    
  public:

    using Item_Type = Arc *; 

    using Set_Type = Node *; 

    Node_Arc_Iterator() noexcept { /* empty */ }

    Node_Arc_Iterator(Node * src) noexcept
      : Dlink::Iterator(&(src->arc_list)), src_node(src)
    {
      // empty 
    }

    Arc * get_current_arc() const
    {
      return static_cast<Arc*>(get_current_arc_node()->arc);
    }

    Arc * get_current() const { return get_current_arc(); }

    Arc * get_curr() const { return get_current_arc(); }

    Node * get_tgt_node() const
    {
      return (Node*) get_current_arc()->get_connected_node(src_node);
    }

    Arc_Node * get_current_arc_node() const
    {
      return dlink_to_arc_node(Dlink::Iterator::get_current()); 
    }
  };

  struct Arc_Iterator : public Dlink::Iterator
  {
    using Item_Type = Arc *; 

    using Set_Type = List_Graph; 

    Arc_Iterator() noexcept { /* empty */ }

    Arc_Iterator(const List_Graph & g) noexcept
    : Dlink::Iterator(&const_cast<Dlink&>(g.arc_list))
    {
      // empty
    }

    Arc * get_current_arc() const
    {
      return dlink_to_arc(const_cast<Dlink*>(Dlink::Iterator::get_current())); 
    }

    Arc * get_current() const { return get_current_arc(); }  

    Arc * get_curr() const { return get_current_arc(); }  

    Node * get_src_node() const { return (Node*) get_current_arc()->src_node; }

    Node * get_tgt_node() const { return (Node*) get_current_arc()->tgt_node; }
  };

  List_Graph() noexcept { /* empty */ }

  void swap(List_Graph & g) noexcept
  {
    this->common_swap(g);
    node_list.swap(&g.node_list);
    arc_list.swap(&g.arc_list);
  }
};


    template <class GT>
struct Dft_Show_Arc
{
  bool operator () (typename GT::Arc * /* arc */) const noexcept
  { 
    return true; 
  }

  void set_cookie(void*) noexcept { /* empty */ }
};


    template <class GT, class Show_Arc = Dft_Show_Arc<GT>>
struct Node_Arc_Iterator : 
      public Filter_Iterator<typename GT::Node*, 
			     typename GT::Node_Arc_Iterator,
			     Show_Arc>
{
  using Filter_Itor = Filter_Iterator<typename GT::Node*, 
				      typename GT::Node_Arc_Iterator,
				      Show_Arc>;

  using Itor = Filter_Iterator<typename GT::Node*, 
			       typename GT::Node_Arc_Iterator, 
			       Show_Arc>;

  using Item_Type = typename Itor::Item_Type; 
  using Set_Type = typename Itor::Set_Type; 

  Node_Arc_Iterator() noexcept { /* empty */ }

  Node_Arc_Iterator(typename GT::Node * p, Show_Arc & sa) 
  noexcept(noexcept(Itor(p, sa)))
    : Itor(p, sa) 
  {
    // empty
  }

  Node_Arc_Iterator(typename GT::Node * p, Show_Arc && sa = Show_Arc()) 
  noexcept(noexcept(Itor(p, sa)))
    : Itor(p, sa) 
  {
    // empty
  }
};

    template <class GT, class Show_Arc = Dft_Show_Arc<GT>>
struct Arc_Iterator : 
      public Filter_Iterator<GT, typename GT::Arc_Iterator, Show_Arc>
{
  using Itor = Filter_Iterator<GT, typename GT::Arc_Iterator, Show_Arc>;

  using Item_Type = typename Itor::Item_Type; 
  using Set_Type = typename Itor::Set_Type; 

  Arc_Iterator() noexcept { /* empty */ }

  Arc_Iterator(const GT & g, Show_Arc & sa) noexcept(noexcept(Itor(g, sa)))
    : Itor(g, sa)
  {
    // empty
  }

  Arc_Iterator(const GT & g, Show_Arc && sa = Show_Arc()) 
  noexcept(noexcept(Arc_Iterator(g, sa)))
    : Arc_Iterator(g, sa)
  {
    // empty
  }
};



     template <class GT>
struct Dft_Show_Node
{
  bool operator () (typename GT::Node *) const noexcept
  { 
    return true; 
  }
};

    template <class GT, class Show_Node = Dft_Show_Node<GT>>
class Node_Iterator : 
  public Filter_Iterator<GT, typename GT::Node_Iterator, Show_Node>
{
public:

  using Itor = Filter_Iterator<GT, typename GT::Node_Iterator, Show_Node>;

  using Item_Type = typename Itor::Item_Type; 

  using Set_Type =  typename Itor::Set_Type; 

  Node_Iterator() noexcept { /* empty */ }

  Node_Iterator(const GT & g, Show_Node & sn) noexcept(noexcept(Itor(g, sn)))
    : Itor (g, sn) 
  {
    /* empty */ 
  }

  Node_Iterator(const GT & g, Show_Node && sn = Show_Node()) 
  noexcept(noexcept(Itor (g, sn)))
    : Itor (g, sn)
  {
    /* empty */ 
  }
};

template <class GT, class SN>
void for_each_node(const GT & g, 
		   std::function<void(typename GT::Node*)> operation,
		   SN & sn) noexcept(noexcept(operation) and noexcept(sn))
{
  for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
    operation(it.get_curr());
}

template <class GT, class SN = Dft_Show_Node<GT>>
void for_each_node(const GT &                              g, 
		   std::function<void(typename GT::Node*)> operation,
		   SN &&                                   sn = SN())
  noexcept(noexcept(operation) and noexcept(sn))
{
  for_each_node<GT, SN>(g, operation, sn);
}

template <class GT, class SA>
void for_each_arc(const GT &                             g, 
		  std::function<void(typename GT::Arc*)> operation,
		  SA &                                   sa)
  noexcept(noexcept(operation) and noexcept(sa))
{
  for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
    operation(it.get_curr());
}

template <class GT, class SA = Dft_Show_Arc<GT>>
void for_each_arc(const GT &                             g, 
		  std::function<void(typename GT::Arc*)> operation,
		  SA &&                                  sa = SA())
  noexcept(noexcept(for_each_arc<GT, SA>(g, operation, sa)))
{
  for_each_arc<GT, SA>(g, operation, sa);
}

template <class GT, class SA>
void for_each_arc(const GT&, 
		  typename GT::Node *                    p, 
		  std::function<void(typename GT::Arc*)> operation,
		  SA &                                   sa)
  noexcept(noexcept(operation) and noexcept(sa))
{
  for (Node_Arc_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    operation(it.get_curr());
}

template <class GT, class SA = Dft_Show_Arc<GT>>
void for_each_arc(const GT &                             g,
		  typename GT::Node *                    p, 
		  std::function<void(typename GT::Arc*)> operation,
		  SA &&                                  sa = SA())
  noexcept(noexcept(for_each_arc<GT, SA>(g, p, operation, sa)))
{
  for_each_arc<GT, SA>(g, p, operation, sa);
}

template <class GT, class SN>
bool forall_node(const GT &                              g, 
		 std::function<bool(typename GT::Node*)> cond,
		 SN &                                    sn)
  noexcept(noexcept(cond) and noexcept(sn))
{
  for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
    if (not cond(it.get_curr()))
      return false;
  return true;
}

template <class GT, class SN = Dft_Show_Node<GT>>
bool forall_node(const GT &                              g, 
		 std::function<bool(typename GT::Node*)> cond,
		 SN &&                                   sn = SN())
  noexcept(noexcept(forall_node<GT, SN>(g, cond, sn)))
{
  return forall_node<GT, SN>(g, cond, sn);
}

template <class GT, class SA = Dft_Show_Arc<GT>>
bool forall_arc(const GT &                             g, 
		std::function<bool(typename GT::Arc*)> cond,
		SA &                                   sa)
  noexcept(noexcept(cond) and noexcept(sa))
{
  for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
    if (not cond(it.get_curr()))
      return false;
  return true;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
bool forall_arc(const GT &                             g, 
		std::function<bool(typename GT::Arc*)> cond,
		SA &&                                  sa = SA())
  noexcept(noexcept(cond) and noexcept(sa))
{
  return forall_arc<GT, SA>(g, cond, sa);
}

template <class GT, class SA>
bool forall_arc(typename GT::Node *                    p, 
		std::function<bool(typename GT::Arc*)> cond,
		SA &                                   sa)
  noexcept(noexcept(cond) and noexcept(sa))
{
  for (Node_Arc_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    if (not cond(it.get_curr()))
      return false;
  return true;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
bool forall_arc(typename GT::Node * p, 
		std::function<bool(typename GT::Arc*)> cond,
		SA &&                                  sa = SA())
  noexcept(noexcept(sa) and noexcept(cond))
{
  return forall_arc<GT, SA>(p, cond, sa);
}


template <class GT, typename T, template <typename> class Container, class SN>
Container<T> nodes_map(GT &                                  g, 
		       std::function<T(typename GT::Node *)> transformation,
		       SN &                                  sn)
{
  Container<T> ret_val;
  for_each_node<GT, SN>(g, [&ret_val, &transformation] (typename GT::Node * p)
			{
			  ret_val.append(transformation(p));
			}, sn);
  return ret_val;
}

template <class GT, 
	  typename T                          = typename GT::Node_Type,
	  template <typename> class Container = DynDlist,
	  class SN                            = Dft_Show_Node<GT>>
Container<T> nodes_map(GT &                                  g, 
		       std::function<T(typename GT::Node *)> transformation,
		       SN &&                                 sn = SN())
{
  return nodes_map<GT, T, Container, SN>(g, transformation, sn);
}

template <class GT, typename T, template <typename> class Container, class SA>
Container<T> arcs_map(GT &                                 g, 
		      std::function<T(typename GT::Arc *)> transformation,
		      SA &                                 sa)
{
  Container<T> ret_val;
  for_each_arc<GT, SA>(g, [&ret_val, &transformation] (typename GT::Arc * p)
		       {
			 ret_val.append(transformation(p));
		       }, sa);
  return ret_val;
}

template <class GT, 
	  typename T                          = typename GT::Arc_Type,
	  template <typename> class Container = DynDlist,
	  class SA                            = Dft_Show_Arc<GT>>
Container<T> arcs_map(GT &                                 g, 
		      std::function<T(typename GT::Arc *)> transformation,
		      SA &&                                sa = SA())
{
  return arcs_map<GT, T, Container, SA>(g, transformation, sa);
}

template <class GT, typename T, template <typename> class Container, class SA>
Container<T> arcs_map(GT &                                 g, 
		      typename GT::Node *                  p,
		      std::function<T(typename GT::Arc *)> transformation,
		      SA &                                 sa)
{
  Container<T> ret_val;
  for_each_arc<GT, SA>(g, p, [&ret_val, &transformation] (typename GT::Arc * p)
		       {
			 ret_val.append(transformation(p));
		       }, sa);
  return ret_val;
}

template <class GT, 
	  typename T                          = typename GT::Arc_Type,
	  template <typename> class Container = DynDlist,
	  class SA                            = Dft_Show_Arc<GT>>
Container<T> arcs_map(GT &                                 g, 
		      typename GT::Node *                  p,
		      std::function<T(typename GT::Arc *)> transformation,
		      SA &&                                sa = SA())
{
  return arcs_map<GT, T, Container, SA>(g, p, transformation, sa);
}

template <class GT, typename T, class SN>
T foldl_nodes(GT & g, const T & init, 
	      std::function<T(const T&, typename GT::Node*)> operation,
	      SN & sn)
  noexcept(noexcept(operation) and noexcept(sn) and
	   std::is_nothrow_copy_assignable<T>::value)
{
  T ret_val = init;
  for_each_node<GT, SN>(g, [&ret_val, &operation] (typename GT::Node * p)
    {
      ret_val = operation(ret_val, p);
    }, sn);
  return ret_val;
}

template <class GT, typename T, class SN = Dft_Show_Node<GT>>
T foldl_nodes(GT & g, const T & init, 
	      std::function<T(const T&, typename GT::Node*)> operation,
	      SN && sn = SN())
  noexcept(noexcept(foldl_nodes<GT, T, SN>(g, init, operation, sn)))
{
  return foldl_nodes<GT, T, SN>(g, init, operation, sn);
}

template <class GT, typename T, class SA = Dft_Show_Arc<GT>>
T foldl_arcs(GT & g, const T & init,
	     std::function<T(const T&, typename GT::Arc*)> operation,
	     SA & sa)
  noexcept(noexcept(operation) and noexcept(sa) and
	   std::is_nothrow_copy_assignable<T>::value)
{
  T ret_val = init;
  for_each_arc<GT, SA>(g, [&ret_val, &operation] (typename GT::Arc* a)
    {
      ret_val = operation(ret_val, a);
    }, sa);
  return ret_val;
}

template <class GT, typename T, class SA = Dft_Show_Arc<GT>>
T foldl_arcs(GT & g, const T & init,
	     std::function<T(const T&, typename GT::Arc*)> operation,
	     SA && sa = SA())
  noexcept(noexcept(foldl_arcs<GT, T, SA>(g, init, operation, sa)))
{
  return foldl_arcs<GT, T, SA>(g, init, operation, sa);
}

template <class GT, typename T, class SA>
T foldl_arcs(GT & g, typename GT::Node * p,
	     const T & init,
	     std::function<T(const T&, typename GT::Arc*)> operation,
	     SA & sa)
  noexcept(noexcept(operation) and noexcept(sa) and
	   std::is_nothrow_copy_assignable<T>::value)
{
  T ret_val = init;
  for_each_arc<GT, SA>(g, p, [&ret_val, &operation] (typename GT::Arc* a)
    {
      ret_val = operation(ret_val, a);
    }, sa);
  return ret_val;
}

template <class GT, typename T, class SA = Dft_Show_Arc<GT>>
T foldl_arcs(GT & g, typename GT::Node * p,
	     const T & init,
	     std::function<T(const T&, typename GT::Arc*)> operation,
	     SA && sa = SA())
  noexcept(noexcept(foldl_arcs<GT, T, SA>(g, p, init, operation, sa)))
{
  return foldl_arcs<GT, T, SA>(g, p, init, operation, sa);
}			 

template <typename __Graph_Node = Graph_Node<int>,
	  typename __Graph_Arc  = Graph_Arc<int>>
class List_Digraph : public List_Graph<__Graph_Node, __Graph_Arc>
{
public:

  using GT = List_Graph<__Graph_Node, __Graph_Arc>;

  List_Digraph() noexcept
  {
    this->digraph = true; 
  }
  
  List_Digraph(const List_Digraph & dg) noexcept(noexcept(GT())) : GT()
  {
    this->digraph = true; 
    *this = dg;
  }

  List_Digraph(List_Digraph && dg) noexcept(noexcept(GT())) : GT()
  {
    this->digraph = true; 
    this->swap(dg);
  }

  List_Digraph & operator = (const List_Digraph & g)
  {
    if (this == &g) 
      return *this;

    copy_graph(*this, (const List_Digraph &) g);
    return *this;
  }

  List_Digraph & operator = (List_Digraph && g)
  {
    this->swap(g);
    return *this;
  }  
};


template <class GT>
using ArcPair = tuple<typename GT::Arc*, typename GT::Node*>;

template <class GT> class __Out_Filt
{
  typename GT::Node * src = nullptr;

public:

  __Out_Filt(typename GT::Node * __src) noexcept : src(__src) { /* empty */ }

  bool operator () (typename GT::Arc * a) const noexcept
  {
    assert(src);
    return a->src_node == src;
  }

  typename GT::Node * get_node(typename GT::Arc * a) const noexcept
  {
    assert(src);
    return (typename GT::Node *) a->tgt_node;
  }
};


template <class GT> class __In_Filt
{
  typename GT::Node * tgt = nullptr;

public:

  __In_Filt(typename GT::Node * __tgt) noexcept : tgt(__tgt) { /* empty */ }

  bool operator () (typename GT::Arc * a) const noexcept
  {
    assert(tgt);
    return a->tgt_node == tgt;
  }

  typename GT::Node * get_node(typename GT::Arc * a) const noexcept
  {
    assert(tgt);
    return (typename GT::Node *) a->src_node;
  }
};

template <class GT, class Filter>
class Digraph_Iterator
{
  using Itor = Filter_Iterator<typename GT::Node*, 
			       typename GT::Node_Arc_Iterator, Filter>;

  Filter filt;
  Itor it;

public:

  using Item_Type = typename Itor::Item_Type; 

  using Iterator_Type = Itor; 

  Digraph_Iterator(typename GT::Node * p) 
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

  void end() { reset_last(); next(); }
};


template <class GT>
using __Out_Iterator = typename GT::Out_Iterator;


template <class GT>
using __In_Iterator = typename GT::In_Iterator;


template <class GT, class Show_Arc = Dft_Show_Arc<GT>>
using Out_Iterator = 
  Filter_Iterator<typename GT::Node*, typename GT::Out_Iterator, Show_Arc>;


template <class GT, class SA = Dft_Show_Arc<GT>>
using In_Iterator = 
  Filter_Iterator<typename GT::Node*, typename GT::In_Iterator, SA>;


template <class GT, class SA>
DynList<typename GT::Node*> out_nodes(typename GT::Node * p, SA & sa)
{
  DynList<typename GT::Node *> ret;
  for (Out_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ret.append(it.get_node());
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<typename GT::Node*> out_nodes(typename GT::Node * p, SA && sa = SA())
{
  return out_nodes<GT, SA>(p, sa);
}

template <class GT, class SA>
DynList<typename GT::Node*> in_nodes(typename GT::Node * p, SA & sa)
{
  DynList<typename GT::Node *> ret;
  for (In_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ret.append(it.get_node());
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<typename GT::Node*> in_nodes(typename GT::Node * p, SA && sa = SA())
{
  return in_nodes<GT, SA>(p, sa);
}

template <class GT, class SA>
DynList<typename GT::Arc*> out_arcs(typename GT::Node * p, SA & sa)
{
  DynList<typename GT::Arc*> ret;
  for (Out_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ret.append(it.get_curr());
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<typename GT::Arc*> out_arcs(typename GT::Node * p, SA && sa = SA())
{
  return out_arcs<GT, SA>(p, sa);
}

template <class GT, class SA>
DynList<typename GT::Arc*> in_arcs(typename GT::Node * p, SA & sa)
{
  DynList<typename GT::Arc*> ret;
  for (In_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ret.append(it.get_curr());
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<typename GT::Arc*> in_arcs(typename GT::Node * p, SA && sa = SA())
{
  return in_arcs<GT, SA>(p, sa);
}


template <class GT, class SA>
DynList<typename GT::Arc*> arcs(typename GT::Node * p, SA & sa)
{
  DynList<typename GT::Arc *> ret;
  for (Node_Arc_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ret.append(it.get_curr());
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<typename GT::Arc*> arcs(typename GT::Node * p, SA && sa = SA())
{
  return arcs<GT, SA>(p, sa);
}


template <class GT, class SA>
DynList<ArcPair<GT>> in_pairs(typename GT::Node * p, SA & sa)
{
  DynList<ArcPair<GT>> ret;
  for (In_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    {
      typename GT::Arc * a = it.get_curr();
      ret.append(make_tuple(a, (typename GT::Node*) a->get_connected_node(p)));
    }
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<ArcPair<GT>> in_pairs(typename GT::Node * p, SA && sa = SA())
{
  return in_pairs<GT, SA>(p, sa);
}

template <class GT, class SA>
DynList<ArcPair<GT>> out_pairs(typename GT::Node * p, SA & sa)
{
  DynList<ArcPair<GT>> ret;
  for (Out_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    {
      typename GT::Arc * a = it.get_curr();
      ret.append(make_tuple(a, (typename GT::Node*) a->get_connected_node(p)));
    }
  return ret;
}

template <class GT, class SA = Dft_Show_Arc<GT>>
DynList<ArcPair<GT>> out_pairs(typename GT::Node * p, SA && sa = SA())
{
  return out_pairs<GT, SA>(p, sa);
}

template <class GT, class SA>
size_t in_degree(typename GT::Node * p, SA & sa)
{
  size_t count = 0;
  for (In_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ++count;
  return count;
}

template <class GT, class SA>
size_t in_degree(typename GT::Node * p, SA && sa = SA())
{
  return in_degree<GT, SA>(p, sa);
}

template <class GT>
size_t in_degree(typename GT::Node * p)
{
  size_t count = 0;
  for (__In_Iterator<GT> it(p); it.has_curr(); it.next())
    ++count;
  return count;
}

template <class GT, class SA>
size_t out_degree(typename GT::Node * p, SA & sa)
{
  size_t count = 0;
  for (Out_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
    ++count;
  return count;
}

template <class GT, class SA>
size_t out_degree(typename GT::Node * p, SA && sa = SA())
{
  return out_degree<GT, SA>(p, sa);
}


template <class GT>
size_t out_degree(typename GT::Node * p)
{
  size_t count = 0;
  for (__Out_Iterator<GT> it(p); it.has_curr(); it.next())
    ++count;
  return count;
}


template <class GT, class Itor, class Operation> inline
bool traverse_arcs(typename GT::Node * p, Operation & operation)
{
  for (Itor it(p); it.has_curr(); it.next())
    if (not operation(it.get_curr()))
      return false;
  return true;
}

template <class GT, class Itor, class Operation> inline
void for_each_arc(typename GT::Node * p, Operation & operation)
{
  for (Itor it(p); it.has_curr(); it.next())
    operation(it.get_curr());
}


		 // Functional operations on input arcs

template <class GT, class Op> inline
bool traverse_in_arcs(typename GT::Node * p, Op & op)
{
  return traverse_arcs<GT, __In_Iterator<GT>, Op>(p, op);
}

template <class GT, class Op> inline
bool traverse_in_arcs(typename GT::Node * p, Op && op = Op())
{
  return traverse_in_arcs<GT, Op>(p, op);
}

template <class GT, class Op> inline
void for_each_in_arc(typename GT::Node * p, Op & op)
{
  for_each_arc<GT, __In_Iterator<GT>, Op>(p, op);
}

template <class GT, class Op> inline
void for_each_in_arc(typename GT::Node * p, Op && op = Op())
{
  for_each_in_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
bool all_in_arc(typename GT::Node * p, Op & op)
{
  return traverse_in_arcs(p, [&op] (auto a) { return op(a); });
}

template <class GT, class Op> inline
bool all_in_arc(typename GT::Node * p, Op && op = Op())
{
  return all_in_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
bool exists_in_arc(typename GT::Node * p, Op & op)
{
  return not traverse_in_arcs<GT, Op>(p, [&op] (auto a) { return not op(a); });
}

template <class GT, class Op> inline
bool exists_in_arc(typename GT::Node * p, Op && op = Op())
{
  return exists_in_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
auto search_in_arc(typename GT::Node * p, Op & op)
{
  typename GT::Arc * ret = nullptr;
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

template <class GT, class Op> inline
auto search_in_arc(typename GT::Node * p, Op && op = Op())
{
  return search_in_arc<GT, Op>(p, op);
}

template <class GT, typename T> inline
auto map_in_arcs(typename GT::Node * p, std::function<T(typename GT::Arc*)> op)
{
  DynList<T> ret;
  for_each_in_arc(p, [&ret, &op] (auto a) { ret.append(op(a)); });
  return ret;
}

template <class GT, typename T> inline
T foldl_in_arcs(typename GT::Node * p, const T & init,
		std::function<T(const T&, typename GT::Arc*)> op)
{
  T ret = init;
  for_each_in_arc(p, [&ret, &op] (auto a) { ret = op(ret, a); });
  return ret;
}

template <class GT, class Op> inline
DynList<typename GT::Arc*> filter_in_arcs(typename GT::Node * p, Op & cond)
{
  DynList<typename GT::Arc*> ret;
  for_each_in_arc(p, [&ret, &cond] (auto a) 
		  {
		    if (cond(a))
		      ret.append(a);
		  });
  return ret;
}

template <class GT, class Op> inline
auto filter_in_arcs(typename GT::Node * p, Op && op = Op())
{
  return filter_in_arcs<GT, Op>(p, op);
}


		 // Functional operation on output arcs

template <class GT, class Op> inline
bool traverse_out_arcs(typename GT::Node * p, Op & op)
{
  return traverse_arcs<GT, __Out_Iterator<GT>, Op>(p, op);
}

template <class GT, class Op> inline
bool traverse_out_arcs(typename GT::Node * p, Op && op = Op())
{
  return traverse_out_arcs<GT, Op>(p, op);
}

template <class GT, class Op> inline
void for_each_out_arc(typename GT::Node * p, Op & op)
{
  for_each_arc<GT, __Out_Iterator<GT>, Op>(p, op);
}

template <class GT, class Op> inline
void for_each_out_arc(typename GT::Node * p, Op && op = Op())
{
  for_each_out_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
bool all_out_arc(typename GT::Node * p, Op & op)
{
  return traverse_out_arcs(p, [&op] (auto a) { return op(a); });
}

template <class GT, class Op> inline
bool all_out_arc(typename GT::Node * p, Op && op = Op())
{
  return all_out_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
bool exists_out_arc(typename GT::Node * p, Op & op)
{
  return not traverse_out_arcs<GT, Op>(p, [&op] (auto a) { return not op(a); });
}

template <class GT, class Op> inline
bool exists_out_arc(typename GT::Node * p, Op && op = Op())
{
  return exists_out_arc<GT, Op>(p, op);
}

template <class GT, class Op> inline
auto search_out_arc(typename GT::Node * p, Op & op)
{
  typename GT::Arc * ret = nullptr;
  __traverse_out_arcs(p, [&op, &ret] (auto a)
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

template <class GT, class Op> inline
auto search_out_arc(typename GT::Node * p, Op && op = Op())
{
  return search_out_arc<GT, Op>(p, op);
}

template <class GT, typename T> inline
auto map_out_arcs(typename GT::Node * p, std::function<T(typename GT::Arc*)> op)
{
  DynList<T> ret;
  for_each_out_arc(p, [&ret, &op] (auto a) { ret.append(op(a)); });
  return ret;
}


template <class GT, typename T> inline
T foldl_out_arcs(typename GT::Node * p, const T & init,
		std::function<T(const T&, typename GT::Arc*)> op)
{
  T ret = init;
  for_each_out_arc(p, [&ret, &op] (auto a) { ret = op(ret, a); });
  return ret;
}

template <class GT, class Op> inline
DynList<typename GT::Arc*> filter_out_arcs(typename GT::Node * p, Op & cond)
{
  DynList<typename GT::Arc*> ret;
  for_each_out_arc(p, [&ret, &cond] (auto a) 
		  {
		    if (cond(a))
		      ret.append(a);
		  });
  return ret;
}

template <class GT, class Op> inline
auto filter_out_arcs(typename GT::Node * p, Op && op = Op())
{
  return filter_out_arcs<GT, Op>(p, op);
}

    template <class GT, class SA> typename GT::Arc * 
search_arc(const GT & g, 
	   typename GT::Node * src, typename GT::Node * tgt,
	   SA & sa) noexcept
{
  assert(src != nullptr and tgt != nullptr);

  if (not g.is_digraph() and tgt->num_arcs < src->num_arcs)
    std::swap(tgt, src); // select the node with less arcs

  for (Node_Arc_Iterator<GT, SA> itor(src, sa); itor.has_curr(); itor.next())  
    if (itor.get_tgt_node() == tgt)
      return itor.get_current_arc();

  return nullptr; 
}

template <class GT, class SA = Dft_Show_Arc<GT>> typename GT::Arc * 
search_arc(const GT & g, 
	   typename GT::Node * src, typename GT::Node * tgt,
	   SA && sa = SA()) 
      noexcept
{
  return search_arc<GT, SA>(g, src, tgt, sa);
}

template <class GT, class SA> typename GT::Arc * 
search_directed_arc(const GT &, 
		    typename GT::Node * src, typename GT::Node * tgt, SA & sa) 
  noexcept
{
  assert(src != nullptr and tgt != nullptr);

  for (Node_Arc_Iterator<GT, SA> it(src, sa); it.has_curr(); it.next())
    {
      typename GT::Arc * a = it.get_curr();
      if (a->src_node == src and a->tgt_node == tgt)
	return a;
    }

  return nullptr; 
}

template <class GT, class SA = Dft_Show_Arc<GT>> typename GT::Arc * 
search_directed_arc(const GT & g, 
		    typename GT::Node * src, typename GT::Node * tgt, 
		    SA && sa = SA()) 
  noexcept
{
  return search_directed_arc<GT, SA>(g, src, tgt, sa);
}

    template <class GT> 
typename GT::Node * mapped_node(typename GT::Node * p) noexcept
{
 return (typename GT::Node *) NODE_COOKIE(p);
}

    template <class GT> 
typename GT::Arc * mapped_arc(typename GT::Arc * a) noexcept
{
  return (typename GT::Arc *) ARC_COOKIE(a);
}

    template <class GTSRC, class GTTGT>
typename GTTGT::Node * mapped_node(typename GTSRC::Node * p) noexcept
{
 return (typename GTTGT::Node *) NODE_COOKIE(p);
}

    template <class GTSRC, class GTTGT>
typename GTTGT::Arc * mapped_arc(typename GTSRC::Arc * a) noexcept
{
  return (typename GTTGT::Arc *) ARC_COOKIE(a);
}

    template <class GT> inline
void copy_graph(GT & gtgt, const GT & gsrc, const bool cookie_map = false);

template <class GT> inline void clear_graph(GT & g) noexcept;

    template <class GT, class Operation, class SN = Dft_Show_Node<GT>> 
class Operate_On_Nodes
{
  SN & sn;

public:

  Operate_On_Nodes(SN & __sn)
  noexcept(std::is_nothrow_constructible<SN>::value)
  : sn(__sn) { /* empty */ }

  Operate_On_Nodes(SN && __sn = SN()) 
  noexcept(std::is_nothrow_constructible<SN>::value)
  : sn(__sn) { /* empty */ }

  void operator () (const GT & g, Operation op = Operation()) 
    noexcept(noexcept(op))
  {
    for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
      op (g, it.get_curr());
  }

  void operator () (GT & g, Operation op = Operation()) 
    noexcept(noexcept(op))
  {
    for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
      op (g, it.get_curr());
  }
};

   
    template <class GT, class Operation, class SA = Dft_Show_Arc<GT>> 
class Operate_On_Arcs
{
  SA & sa;

public:

  Operate_On_Arcs(SA & __sa) 
  noexcept(std::is_nothrow_constructible<SA>::value)
 : sa(__sa) { /* empty */ }

  Operate_On_Arcs(SA && __sa = SA()) 
  noexcept(std::is_nothrow_constructible<SA>::value)
  : sa(__sa) { /* empty */ }

  void operator () (const GT & g, Operation op = Operation()) const 
    noexcept(noexcept(op))
  {
    for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
      op (g, it.get_curr());
  }

  void operator () (GT & g, Operation op = Operation()) const
    noexcept(noexcept(op))
  {
    for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
      op (g, it.get_curr());
  } 

  void operator () (const GT & g, typename GT::Node * p, 
                    Operation op = Operation()) const noexcept(noexcept(op))
  {
    for (Node_Arc_Iterator<GT, SA> it(p); it.has_curr(); it.next())
      op (g, it.get_current_arc());
  }

  void operator () (GT & g, typename GT::Node * p, 
                    Operation && op = Operation()) const
    noexcept(noexcept(op))
  {
    for (Node_Arc_Iterator<GT, SA> it(p); it.has_curr(); it.next())
      op (g, it.get_current_arc());
  }
};

    template <class GT> 
class Path
{
public:

  using Node_Type = typename GT::Node_Type;

  using Arc_Type = typename GT::Arc_Type;

private:

  const GT * g = nullptr;

  using Node = typename GT::Node;
  using Arc = typename GT::Arc;

  struct Path_Desc
  {
    Node * node; // nodo origen
    Arc *  arc;  // arco adyacente

    Path_Desc(Node * _node = nullptr, Arc * _arc = nullptr) noexcept
      : node(_node), arc(_arc) {}

    bool operator == (const Path_Desc & r) const noexcept
    { 
      if (not (node->get_info() == r.node->get_info()))
	return false;

      if (arc == nullptr)
	return r.arc == nullptr;

      if (r.arc == nullptr)
	return false;

      return arc->get_info() == r.arc->get_info(); 
    }
  };

  DynDlist<Path_Desc> list;

  void check_graph()
  {
    if (g == nullptr)
      throw std::domain_error("Path: Graph has not been specified");
  }

public:

  bool check() const
  {
    auto l = list;
    while (not l.is_unitarian_or_empty())
      {
	auto d = l.remove_first();
	auto nxt = l.get_first().node;
	if (nxt == g->get_connected_node(d.arc, d.node))
	  return false;
      }

    return true;
  }

  bool check_directed() const
  {
    return list.all([this] (auto d) { return g->get_src_node(d.arc) == d.node; }) 
      and list.get_last().arc == nullptr;
  }

  const GT & get_graph() const noexcept { return *g; }

  bool inside_graph(const GT & gr) const noexcept { return g == &gr; }

  Path(const GT & __g) noexcept : g(&__g) {}

  Path() noexcept : g(nullptr) { /* empty */ }

  void init(Node * start_node) { list.append(Path_Desc(start_node)); }

  Path(const GT & _g, Node * start_node) : g(&_g) { init(start_node); }

  void set_graph(const GT & __g, Node * start_node = nullptr) 
  {
    empty();
    g = &__g;
    if (start_node == nullptr)
      return;
    init(start_node);
  }

  size_t size() const noexcept { return list.size(); }

  bool is_empty() const noexcept { return list.is_empty(); }

  void empty() noexcept
  {
    check_graph();
    while (not list.is_empty())
      list.remove_first();
  }

  Path(const Path & path) : g(path.g), list(path.list) {}

  Path(Path && path) : g(path.g), list(move(path.list)) {}

  Path & operator = (const Path & path)
  {
    if (this == &path)
      return *this;

    empty();
    g    = path.g;
    list = path.list;
    return *this;
  }

  Path & operator = (Path && path)
  {
    empty();
    std::swap(g, path.g);
    std::swap(list, path.list);
    return *this;
  }

  void append(Arc * arc)
  {
    check_graph();

    if (list.is_empty())
      throw std::domain_error("path is empty");

    auto & last_path_desc = list.get_last();
    auto last_node = last_path_desc.node;
    if (arc->src_node != last_node and arc->tgt_node != last_node)
      throw std::invalid_argument("arc no link last node of path");

    last_path_desc.arc = arc;
    list.append(Path_Desc(g->get_connected_node(arc, last_node)));
  }

  void append(Node * node)
  {
    check_graph();

    if (list.is_empty())
      {
        init(node);
        return;
      }

    Node * last_node = get_last_node();
    Arc * arc        = search_arc(*g, last_node, node); 

    if (arc == nullptr)
      throw std::invalid_argument("There is no an arc connecting to the node");

    append(arc);
  }

  void append_directed(Node * p)
  {
    check_graph();
    if (list.is_empty())
      {
        init(p);
        return;
      }

    auto & last_path_desc = list.get_last();
    Node * last_node = last_path_desc.node;
    Arc * arc        = search_directed_arc(*g, last_node, p);

    if (arc == nullptr)
      throw std::invalid_argument("There is no an arc connecting to the node");

    assert(arc->src_node == last_path_desc.node);

    last_path_desc.arc = arc;
    list.append(Path_Desc(static_cast<Node*>(arc->tgt_node)));
  }

  void append_directed(Arc * arc)
  {
    check_graph();

    if (list.is_empty())
      throw std::domain_error("path is empty");

    auto & last_path_desc = list.get_last();
    Node * last_node = last_path_desc.node;
    if (arc->src_node != last_node)
      throw std::invalid_argument("The arc does not connect the last node");

    last_path_desc.arc = arc;
    list.append(Path_Desc(static_cast<Node*>(arc->tgt_node)));
  }

  void insert(Arc * arc)
  {
    check_graph();

    if (list.is_empty())
      throw std::domain_error("path is empty");

    auto & first_path_desc = list.get_first();
    auto first_node = first_path_desc.node;
    if (arc->src_node != first_node and arc->tgt_node != first_node)
      throw std::invalid_argument("arc no link first node of path");

    Path_Desc item(g->get_connected_node(arc, first_node), arc);
    list.insert(item);
  }

  void insert(Node * node)
  {
    check_graph();

    if (list.is_empty())
      {
        init(node);
        return;
      }

    Node * first_node = get_first_node();
    Arc * arc = search_arc(*g, node, first_node); // busque arco first_node-node
    if (arc == nullptr)
      throw std::domain_error("There is no arc connecting node");

    Path_Desc item(node, arc);
    list.insert(item);
  }

  void insert_directed(Node * p)
  {
    check_graph();

    if (list.is_empty())
      {
        init(p);
        return;
      }

    Node * first_node = get_first_node();
    Arc * arc         = search_directed_arc(*g, p, first_node);
    if (arc == nullptr)
      throw std::domain_error("There is no an arc connecting to the node");

    list.insert(Path_Desc(p, arc));
  }

  void insert_directed(Arc * arc)
  {
    check_graph();

    if (list.is_empty())
      throw std::domain_error("path is empty");

    auto & first_path_desc = list.get_first();
    Node * first_node = first_path_desc.node;
    if (arc->tgt_node != first_node)
      throw std::invalid_argument("The arc does not connect the first node");

    list.insert(Path_Desc(static_cast<Node*>(arc->src_node), arc));
  }

  Node * get_first_node() const noexcept { return list.get_first().node; }

  Node * get_last_node() const noexcept 
  {
    auto & last_path_desc = list.get_last();
    assert(last_path_desc.arc == nullptr);
    return last_path_desc.node; 
  }

  Arc * get_first_arc() const noexcept { return list.get_first().arc; }

  Arc * get_last_arc() const 
  {
    if (list.is_unitarian())
      throw std::domain_error("Path with only a node (without any arc");

    typename DynDlist<Path_Desc>::Iterator it(list);
    it.reset_last();
    it.prev();
    return it.get_current().arc; 
  }

  bool is_cycle() const noexcept { return get_first_node() == get_last_node(); }

  Node * remove_last_node() noexcept(noexcept(list.remove_last()))
  {
    auto d = list.remove_last();
    list.get_last().arc = nullptr;
    return d.node;
  }

  Node * remove_first_node() noexcept(noexcept(list.remove_first()))
  {
    auto d = list.remove_first();
    return d.node;
  }

  void swap(Path & path) noexcept
  {
    std::swap(g, path.g);
    list.swap(path.list);
  }

  class Iterator : public DynDlist<Path_Desc>::Iterator
  {
  public:

    Iterator(const Path & path) noexcept
      : DynDlist<Path_Desc>::Iterator(path.list) { }

  private:

    Path_Desc & get_curr_path_desc() const
    {
      return this->DynDlist<Path_Desc>::Iterator::get_current();
    }

  public:

    Node * get_current_node() const { return this->get_curr_path_desc().node; }

    Arc * get_current_arc() const
    { 
      if (this->is_in_last())
        throw std::overflow_error("Path iterator is in last node of path");

      return this->get_curr_path_desc().arc; 
    }

    Node * get_current() const { return get_current_node(); }

    Node * get_curr() const { return get_current_node(); }

    tuple<Node*, Arc*> get_pair() const
    {
      return make_tuple(get_current_node(), get_current_arc());
    }

    bool has_current_arc() const noexcept
    {
      return this->has_current() and not this->is_in_last();
    }

    bool has_current_node() const noexcept { return this->has_current(); }
  };

  template <class Operation> 
  void for_each_node(Operation & op) const noexcept(noexcept(op))
  {
    for (Iterator it(*this); it.has_current_node(); it.next())
      op (it.get_current_node());
  }

  template <class Operation> 
  void for_each_node(Operation && op = Operation()) const 
    noexcept(noexcept(op))
  {
    for_each_node<Operation>(op);
  }

  template <class Operation> 
  void for_each_arc(Operation & op) const noexcept(noexcept(op))
  {
    for (Iterator it(*this); it.has_current_arc(); it.next())
      op (it.get_current_arc());
  }

      template <class Operation> 
  void for_each_arc(Operation && op = Operation()) const
	noexcept(noexcept(op))
  {
    for_each_arc<Operation>(op);
  }

  bool contains_node(Node * node) const noexcept
  {
    for (Iterator it(*this); it.has_current_node(); it.next())
      if (it.get_current_node() == node)
        return true;
    return false;
  }

  bool contains_arc(Arc * arc) const noexcept
  {
    for (Iterator it(*this); it.has_current_arc(); it.next())
      if (it.get_current_arc() == arc) 
        return true;
    return false;
  }

  DynList<Node*> nodes() const
  {
    DynList<Node*> ret_val;
    for_each_node([&ret_val] (Node * p) { ret_val.append(p); });
    return ret_val;
  }

  DynList<Arc*> arcs() const
  {
    DynList<Arc*> ret_val;
    for_each_arc([&ret_val] (Arc * a) { ret_val.append(a); });
    return ret_val;
  }

  bool operator == (const Path & p) const noexcept
  {
    return eq(this->list, p.list);
  }

  bool operator != (const Path & p) const noexcept
  {
    return not eq(this->list, p.list);
  }
}; 

    template <class GT> inline 
Path<GT> find_path_depth_first(const GT & g, typename GT::Node * start_node,
			       typename GT::Node * end_node);

    template <class GT> static inline 
bool __find_path_depth_first(const GT & g, typename GT::Node * curr_node,
                             typename GT::Arc *  curr_arc, 
                             typename GT::Node * end_node, Path<GT> & path)
{
  if (curr_node == end_node) // this test must be first in order to find cycles
    {
      path.append(curr_arc);
      return true;
    }

  if (IS_NODE_VISITED(curr_node, Find_Path)) 
    return false;

  path.append(curr_arc);
  NODE_BITS(curr_node).set_bit(Find_Path, true); 

  for (auto it = g.get_arc_it(curr_node); it.has_current(); it.next())
    {
      auto next_arc = it.get_curr();
      if (IS_ARC_VISITED(next_arc, Find_Path)) 
        continue;

      ARC_BITS(next_arc).set_bit(Find_Path, true); 
      auto next_node = it.get_tgt_node(); 
      if (__find_path_depth_first<GT>(g, next_node, next_arc, end_node, path))
        {
          assert(path.get_last_node() == end_node);
	  return true;
        }
    }

  path.remove_last_node(); 

  return false;
}

    template <class GT> inline 
Path<GT> find_path_depth_first(const GT & g, typename GT::Node * start_node, 
			       typename GT::Node * end_node)
{
  Path<GT> path(g, start_node);

  g.reset_bit_nodes(Find_Path);
  g.reset_bit_arcs(Find_Path); 
  NODE_BITS(start_node).set_bit(Find_Path, true);

  for (auto it = g.get_arc_it(start_node); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      ARC_BITS(arc).set_bit(Find_Path, true); 
      auto next_node = it.get_tgt_node();
      if (IS_NODE_VISITED(next_node, Find_Path)) 
        continue; 

      if (__find_path_depth_first<GT>(g, next_node, arc, end_node, path)) 
        return path;
    }

  path.empty();

  return path;
}

    template <class GTS, class GTT>
void map_nodes(typename GTS::Node * p, typename GTT::Node * q) noexcept
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
	
    template <class GTS, class GTT>
void map_arcs(typename GTS::Arc * p, typename GTT::Arc * q) noexcept
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


    template <class GT> 
void clear_graph(GT & g) noexcept
{     
  for (typename GT::Arc_Iterator it(g); it.has_curr(); ) // eliminar arcos
    {
      typename GT::Arc * arc = it.get_curr();
      it.next();
      g.remove_arc(arc);
    }

  for (typename GT::Node_Iterator it(g); it.has_curr(); ) // eliminar nodos
    {     
      typename GT::Node * p = it.get_curr(); 
      it.next(); // avanzar antes de borrar (consistencia iterador)
      g.remove_node(p); // eliminarlo del grafo
    }
}


    template <class GT> 
void copy_graph(GT & gtgt, const GT & gsrc, const bool cookie_map)
{
  try
    {
      clear_graph(gtgt); // limpiar this antes de copiar
      DynMapAvlTree<typename GT::Node*, typename GT::Node*> map;  

          // fase 1: recorrer nodos de src_graph e insertar copia en this
      for (typename GT::Node_Iterator it(gsrc); it.has_curr(); it.next())
	{
	  typename GT::Node * src_node = it.get_current_node(); 
	  unique_ptr<typename GT::Node> 
	    tgt_node(new typename GT::Node(src_node->get_info()));
	  map.insert(src_node, tgt_node.get()); 

	  typename GT::Node * tgt = tgt_node.release();
	  assert(tgt->get_info() == src_node->get_info());
	  gtgt.insert_node(tgt); // insertar en grafo destino

	  if (cookie_map)
	    GT::map_nodes(src_node, tgt);
	}

      assert(gtgt.get_num_nodes() == gsrc.get_num_nodes());

          // fase 2: por cada arco de src_graph, crear en this un
          // arco que conecte a los nodos mapeados de map
      for (typename GT::Arc_Iterator it(gsrc); it.has_curr(); it.next())
	{
	  typename GT::Arc * src_arc = it.get_current_arc();

	      // obtener imágenes de nodos en el grafo destino y crear arco
	  typename GT::Node * src_node = map[gsrc.get_src_node(src_arc)];
	  typename GT::Node * tgt_node = map[gsrc.get_tgt_node(src_arc)];
	  typename GT::Arc * tgt_arc   = gtgt.insert_arc(src_node, tgt_node); 
	  // tgt_arc->get_info() = src_arc->get_info();
	  *tgt_arc = *src_arc;
	  if (cookie_map)
	    GT::map_arcs(src_arc, tgt_arc);
	}

      assert(gtgt.get_num_arcs() == gsrc.get_num_arcs());
    }
  catch (...)
    {     // Si ocurre excepción se limpia this 
      clear_graph(gtgt); 
      throw;
    }
}

    template <class GTT, class GTS>
struct Dft_Copy_Node
{
  void operator () (typename GTT::Node * tgt, typename GTS::Node * src) noexcept
  {
    tgt->get_info() = src->get_info();
  }
};


    template <class GTT, class GTS>
struct Dft_Copy_Arc
{
  void operator () (typename GTT::Arc * tgt, typename GTS::Arc * src) 
  {
    tgt->get_info() = src->get_info();
  }
};

    template <class GTT, class GTS, 
	      class Copy_Node = Dft_Copy_Node<GTT, GTS>, 
	      class Copy_Arc = Dft_Copy_Arc<GTT, GTS>> 
void inter_copy_graph(GTT & gtgt, const GTS & gsrc, 
		      const bool cookie_map = false)
{
  try
    {
      clear_graph(gtgt); // limpiar this antes de copiar
      DynMapAvlTree<typename GTS::Node*, typename GTT::Node*> map;  

          // fase 1: recorrer nodos de src_graph e insertar copia en this
      for (typename GTS::Node_Iterator it(gsrc); it.has_curr(); it.next())
	{
	  typename GTS::Node * src_node = it.get_current_node(); 
	  unique_ptr<typename GTT::Node> tgt_node(new typename GTT::Node);
	  Copy_Node () (tgt_node.get(), src_node);
	  map.insert(src_node, tgt_node.get()); 

	  typename GTT::Node * tgt = tgt_node.release();
	  gtgt.insert_node(tgt); // insertar en grafo destino

	  if (cookie_map)
	    map_nodes<GTS, GTT>(src_node, tgt);
	}

      assert(gtgt.get_num_nodes() == gsrc.get_num_nodes());

          // fase 2: por cada arco de src_graph, crear en this un
          // arco que conecte a los nodos mapeados de map
      for (typename GTS::Arc_Iterator it(gsrc); it.has_curr(); it.next())
	{
	  typename GTS::Arc * src_arc = it.get_current_arc();

	      // obtener imágenes de nodos en el grafo destino y crear arco
	  typename GTT::Node * src_node = map[gsrc.get_src_node(src_arc)]; 
	  typename GTT::Node * tgt_node = map[gsrc.get_tgt_node(src_arc)];
	  typename GTT::Arc * tgt_arc   = gtgt.insert_arc(src_node, tgt_node);
	  Copy_Arc () (tgt_arc, src_arc);
	  if (cookie_map)
	    map_arcs<GTS, GTT>(src_arc, tgt_arc);
	}

      assert(gtgt.get_num_arcs() == gsrc.get_num_arcs());
    }
  catch (...)
    {     // Si ocurre excepción se limpia this 
      clear_graph(gtgt); 
      throw;
    }
}


template <class GT, class SN = Dft_Show_Node<GT>, class SA = Dft_Show_Arc<GT>>
class Copy_Graph
{
  SN & sn;
  SA & sa;

public:

  Copy_Graph(SA & __sa, SN & __sn) : sn(__sn), sa(__sa)
  {
    // empty
  }

  Copy_Graph(SA && sa = SA(), SN && sn = SN()) : Copy_Graph(sa, sn)
  {
    // empty
  }

  Copy_Graph(SA & sa, SN && sn = SN()) : Copy_Graph(sa, sn)
  {
    // empty
  }

private:

  void copy(GT & gtgt, const GT & gsrc, const bool cookie_map)
  {
    try
      {
	clear_graph(gtgt); // limpiar this antes de copiar
	DynMapAvlTree<typename GT::Node*, typename GT::Node*> map;  

          // fase 1: recorrer nodos de src_graph e insertar copia en this
	for (Node_Iterator<GT, SN> it(gsrc, sn); it.has_curr(); it.next())
	  {
	    typename GT::Node * src_node = it.get_curr(); 
	    unique_ptr<typename GT::Node> 
	      tgt_node(new typename GT::Node(src_node));
	    map.insert(src_node, tgt_node.get()); 
	    
	    typename GT::Node * tgt = tgt_node.release();
	    gtgt.insert_node(tgt); // insertar en grafo destino

	    if (cookie_map)
	      GT::map_nodes(src_node, tgt);
	  }

          // fase 2: por cada arco de src_graph, crear en this un
          // arco que conecte a los nodos mapeados de map
	for (Arc_Iterator<GT, SA> it(gsrc, sa); it.has_curr(); it.next())
	  {
	    typename GT::Arc * src_arc = it.get_curr();

	      // obtener imágenes de nodos en el grafo destino y crear arco
	    typename GT::Node * src_node = map[gsrc.get_src_node(src_arc)]; 
	    typename GT::Node * tgt_node = map[gsrc.get_tgt_node(src_arc)];
	    typename GT::Arc * tgt_arc   = 
	      gtgt.insert_arc(src_node, tgt_node, src_arc->get_info()); 

	    if (cookie_map)
	      GT::map_arcs(src_arc, tgt_arc);
	  }
      }
    catch (...)
      {     // Si ocurre excepción se limpia this 
	clear_graph(gtgt); 
	throw;
      }
  }

public:

  void operator () (GT & gtgt, GT & gsrc, const bool cookie_map = true)
  {
    copy(gtgt, gsrc, cookie_map);
  }
};

template <class GT, class Distance>
struct Painted_Min_Spanning_Tree 
{
  typename Distance::Distance_Type dist;

  Painted_Min_Spanning_Tree() noexcept : dist(0) { /* empty */ }

  bool operator () (typename GT::Arc * a) noexcept
  {
    if (not IS_ARC_VISITED(a, Aleph::Spanning_Tree))
      return false;

    dist = dist + ARC_DIST(a);

    return true;
  }
};


template <class GT> inline
bool are_equal(const GT & g1, const GT & g2);



} // end namespace Aleph

# endif /* TPL_GRAPH_H */
