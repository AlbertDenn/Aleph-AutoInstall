
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

# ifndef TPL_SGRAPH_H
# define TPL_SGRAPH_H

# include <memory>
# include <htlist.H>
# include <tpl_graph.H>
# include <tpl_dynSetTree.H>

using namespace Aleph;

namespace Aleph {


    template <typename Node_Info = Empty_Class> 
struct Graph_Snode : public GTNodeCommon<Node_Info>
{
  using Base = GTNodeCommon<Node_Info>;
  friend class GTNodeCommon<Node_Info>;

  DynList<void*> arc_list; // lista de adyacencia

  Graph_Snode(const Node_Info & info) 
  noexcept(std::is_nothrow_copy_constructible<Node_Info>::value) : Base(info)
  { 
    /* empty */ 
  }

  Graph_Snode(Node_Info && info = Node_Info()) 
  noexcept(std::is_nothrow_move_constructible<Node_Info>::value) 
  : Base(move(info))
  { 
    /* empty */ 
  }

  Graph_Snode(const Graph_Snode & node) 
  noexcept(std::is_nothrow_copy_constructible<Node_Info>::value)
    : Graph_Snode(node.node_info) 
  {
    /* empty */ 
  }

  Graph_Snode & operator = (const Graph_Snode & node)
  noexcept(std::is_nothrow_copy_assignable<Node_Info>::value)
  {
    if (&node == this)
      return *this;
    this->node_info = node.node_info;
    return *this;
  }

  Graph_Snode(Graph_Snode * node) 
  noexcept(std::is_nothrow_copy_assignable<Node_Info>::value)
    : Base(node->get_info())
  { 
    /* empty */ 
  }
};

    template <typename Arc_Info = Empty_Class> 
class Graph_Sarc : public GTArcCommon<Arc_Info>
{
  using Base = GTArcCommon<Arc_Info>;

public:

  Graph_Sarc(const Arc_Info & info) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
    : Base(info)
  { 
    /* empty */ 
  }

  Graph_Sarc(Arc_Info && info = Arc_Info()) 
  noexcept(std::is_nothrow_move_constructible<Arc_Info>::value)
  : Base(move(info))
  { 
    /* empty */ 
  }

  Graph_Sarc(const Graph_Sarc & arc) 
  noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
    : Graph_Sarc(arc.arc_info) 
  {
    /* empty */ 
  }

  Graph_Sarc & operator = (const Graph_Sarc & arc)
  noexcept(std::is_nothrow_copy_assignable<Arc_Info>::value)
  {
    if (&arc == this)
      return *this;
    this->arc_info = arc.arc_info;
    return *this;
  }

  Graph_Sarc(void * src, void * tgt, const Arc_Info & data) 
    noexcept(std::is_nothrow_copy_constructible<Arc_Info>::value)
  : Base(src, tgt, data)
  { 
    // empty
  }

  Graph_Sarc(void * src, void * tgt, Arc_Info && data) 
    noexcept(std::is_nothrow_move_constructible<Arc_Info>::value)
  : Base(src, tgt, move(data))
  { 
    // empty
  }
};


  template <typename __Graph_Node = Graph_Snode<unsigned long>, 
	    typename __Graph_Arc  = Graph_Sarc<unsigned long>>
class List_SGraph 
    : public GraphCommon<List_SGraph<__Graph_Node, __Graph_Arc>,
			 __Graph_Node, __Graph_Arc>
{
public:

  using Node = __Graph_Node;
  using Arc = __Graph_Arc;
  using Node_Type = typename Node::Node_Type;
  using Arc_Type = typename Arc::Arc_Type;

  friend class GraphCommon<List_SGraph<__Graph_Node, __Graph_Arc>,
			   __Graph_Node, __Graph_Arc>;

  using CommonBase = GraphCommon<List_SGraph<__Graph_Node, __Graph_Arc>,
				 __Graph_Node, __Graph_Arc>;

  // using FuncBase::search_arc; // TODO: delete after full dry
  using CommonBase::insert_node;
  using CommonBase::insert_arc;

private:

  using DynSetNode = DynSetTree<Node*, Rand_Tree>;
  using DynSetArc = DynSetTree<Arc*, Rand_Tree>;

  DynSetNode node_list; // lista de nodos
  DynSetArc  arc_list;  // lista de arcos

public:

  List_SGraph() noexcept { /* empty */ }

  virtual ~List_SGraph()
  {
    clear();
  }

  List_SGraph(const List_SGraph & g)
  {
    copy_graph(*this, g);
  }

  void swap(List_SGraph & g) noexcept
  {
    this->common_swap(g);
    node_list.swap(g.node_list);
    arc_list.swap(g.arc_list);
  }

  List_SGraph(List_SGraph && g) noexcept
  {
    swap(g);
  }

  List_SGraph & operator = (const List_SGraph & g)
  {
    if (&g == this)
      return *this;

    clear();

    copy_graph(*this, const_cast<List_SGraph&>(g));

    return *this;
  }

  List_SGraph & operator = (List_SGraph && g) noexcept
  {
    swap(g);

    return *this;
  }

  class Node_Iterator : public DynSetNode::Iterator
  {
  public:

    using Item_Type = Node *;

    using Set_Type = List_SGraph;

    Node_Iterator() noexcept { /* empty */ }

    Node_Iterator(const List_SGraph & g) noexcept
      : DynSetNode::Iterator(g.node_list)
    {
      // empty
    }

    Node * get_current_node() const
    {
      return this->get_curr();
    }
  };

  class Node_Arc_Iterator : public DynList<void*>::Iterator
  {
    Node * src_node; 
    
  public:

    using Item_Type =  Arc *;

    using Set_Type = Node *;

    Node_Arc_Iterator() noexcept { /* empty */ }

    Node_Arc_Iterator(Node * src) noexcept 
      : DynList<void*>::Iterator(src->arc_list), src_node(src)
    {
      // empty 
    }

    Arc * get_current_arc() const
    {
      return (Arc*) this->DynList<void*>::Iterator::get_curr();
    }

    Arc * get_current() const { return get_current_arc(); }

    Arc * get_curr() const { return get_current_arc(); }

    Node * get_tgt_node() const
    {
      Arc * a = get_curr();
      return (Node*) a->get_connected_node(src_node);
    }
  };

  class Arc_Iterator : public DynSetArc::Iterator
  {
  public:

    using Item_Type = Arc *;

    using Set_Type = List_SGraph;

    Arc_Iterator() noexcept { /* empty */ }

    Arc_Iterator(const List_SGraph & _g) noexcept
      : DynSetArc::Iterator(_g.arc_list)
    {
      // empty
    }

    Arc * get_current_arc() const 
    {
      return this->get_curr();
    }

    Node * get_src_node() const { return (Node*) get_current_arc()->src_node; }

    Node * get_tgt_node() const { return (Node*) get_current_arc()->tgt_node; }
  };


  virtual Node * insert_node(Node * p)
  {
    assert(p->arc_list.is_empty());

    this->num_nodes++;
    node_list.append(p);
    return p;
  }

private:

  Arc * insert_arc(Node * src, Node * tgt, void * a)
  {
    Arc * arc = (Arc*) a;

    arc->src_node = src;
    arc->tgt_node = tgt;

    src->arc_list.append(a);
    src->num_arcs++;
    if (not this->digraph and src != tgt)
      {
	tgt->arc_list.append(a);
	tgt->num_arcs++;
      }

    arc_list.append(arc);
    this->num_arcs++;

    return arc;
  }

  void disconnect_arc(Arc * arc) noexcept
  {
    Node * src = (Node*) arc->src_node;
    Node * tgt = (Node*) arc->tgt_node;

    src->arc_list.remove(arc);
    src->num_arcs--;

    if (not this->digraph and src != tgt)
      {
	tgt->arc_list.remove(arc);
	tgt->num_arcs--;
      }
  }

public:

  virtual void remove_arc(Arc * arc) noexcept
  {
    disconnect_arc(arc);
    arc_list.remove(arc);
    this->num_arcs--;
    delete arc;
  }

  virtual void remove_node(Node * p) noexcept
  {
    DynList<Arc*> arcs_to_remove;
    arc_list.for_each([this, &arcs_to_remove, p] (auto arc)
      {
        if (this->get_src_node(arc) == p or this->get_tgt_node(arc) == p)
          {
            this->disconnect_arc(arc);
            this->num_arcs--;
    	    arcs_to_remove.append(arc);
    	  }
      });

    arcs_to_remove.for_each([this] (auto a) 
    	      {
    		arc_list.remove(a);
		delete a; 
    	      });
    node_list.remove(p);
    this->num_nodes--;
    
    delete p;
  }

  Node * get_first_node() const
  {
    return (Node*) node_list.get_first();
  }

  Arc * get_first_arc() const
  {
    return (Arc*) arc_list.get_first();
  }

  Arc * get_first_arc(Node * p) const
  {
    return (Arc*) p->arc_list.get_first();
  }

private:

  void clear() noexcept
  {
    arc_list.for_each( [] (Arc * p) { delete p; } );
    node_list.for_each( [] (Node * p) { delete p; } );
  }

      template <class Cmp>
  struct Cmp_Arc
  {
    Cmp & cmp;

    Cmp_Arc(Cmp && __cmp = Cmp()) 
    noexcept(std::is_nothrow_constructible<Cmp>::value and
	     std::is_nothrow_move_assignable<Cmp>::value)
      : cmp(__cmp) { /* empty */ }

    Cmp_Arc(Cmp & __cmp) 
    noexcept(std::is_nothrow_constructible<Cmp>::value)
      : cmp(__cmp) { /* empty */ }

    bool operator () (Slinknc * s1, Slinknc * s2) const noexcept
    {
      Arc * arc1 = (Arc*) s1;
      Arc * arc2 = (Arc*) s2;
      return cmp(arc1, arc2);
    }
  };

public:

      template <class Compare> 
  void sort_arcs(Compare & cmp) noexcept
  {
    Cmp_Arc<Compare> comp(cmp);
    mergesort<HTList, Cmp_Arc<Compare>>(arc_list, comp);
  }

        template <class Compare> 
  void sort_arcs(Compare && cmp = Compare()) noexcept
  {
    mergesort<Cmp_Arc<Compare>>(arc_list, Cmp_Arc<Compare>(cmp)); 
  }
};

  template <typename __Graph_Node = Graph_Snode<int>,
	    typename __Graph_Arc  = Graph_Sarc<int>>
class List_SDigraph : public List_SGraph<__Graph_Node, __Graph_Arc>
{
  using GT = List_SGraph<__Graph_Node, __Graph_Arc>;

public:

  using Node = __Graph_Node;

  using Arc = __Graph_Arc;

  List_SDigraph() noexcept
  {
    this->digraph = true; 
  }
  
  List_SDigraph(const List_SDigraph & dg) : GT()
  {
    this->digraph = true; 
    copy_graph(*this, dg, false);
  }

  List_SDigraph & operator = (const List_SDigraph<Node, Arc> & g)
  {
    if (this == &g) 
      return *this;

    this->digraph = true;
    copy_graph(*this, g, false);

    return *this;
  }

  List_SDigraph(List_SDigraph && dg) noexcept
    : GT()
  {
    this->digraph = true;
    this->swap(dg);
  }

  List_SDigraph & operator = (List_SDigraph<Node, Arc> && g) noexcept
  {
    this->digraph = true; 
    this->swap(g);

    return *this;
  }
};


} // end namespace Aleph

# endif /* TPL_SGRAPH_H */
