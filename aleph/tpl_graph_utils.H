
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

# ifndef TPL_GRAPH_UTILS_H
# define TPL_GRAPH_UTILS_H


# include <limits>
# include <tpl_agraph.H> 
# include <tpl_dynListQueue.H>

using namespace Aleph;

namespace Aleph {

    template <class GT> inline static bool 
__depth_first_traversal(const GT & g, typename GT::Node * node,
			typename GT::Arc *  arc,
			bool (*visit)(const GT & g, typename GT::Node *, 
				      typename GT::Arc *),
			size_t & count);

    template <class GT> inline size_t 
depth_first_traversal(const GT & g, typename GT::Node * start_node,
		      bool (*visit)(const GT & g, typename GT::Node *, 
				    typename GT::Arc *))
{
  g.reset_bit_nodes(Depth_First); // reiniciar Depth_First de nodos
  g.reset_bit_arcs(Depth_First);  // reiniciar Depth_First de arcos
  size_t counter = 0; // inicialmente no se ha visitado ningún nodo

  __depth_first_traversal(g, start_node, nullptr, visit, counter);

  return counter;
}

  template <class GT> inline 
size_t depth_first_traversal(const GT & g, 
			     bool (*visit)(const GT &, typename GT::Node *, 
					   typename GT::Arc *))
{
  return depth_first_traversal(g, g.get_first_node(), visit);
}

  template <class GT>
struct Default_Visit_Op
{
  bool operator () (const GT &, typename GT::Node *, typename GT::Arc *)
  {
    return false;
  }
};

  template <class GT, 
	    class Operation = Default_Visit_Op<GT>, 
	    class SA        = Dft_Show_Arc<GT>> 
class Depth_First_Traversal
{
  Operation * op;
  SA        & sa;
  size_t      count;
  const GT *  g_ptr;

private:

  bool __dft(typename GT::Node * node, typename GT::Arc * arc = nullptr)
  {
    if (IS_NODE_VISITED(node, Depth_First)) 
      return false; 

    NODE_BITS(node).set_bit(Depth_First, true); // marca nodo visitado
    count++;

    if ((*op)(*g_ptr, node, arc)) 
      return true; // sí, invóquela

    if (count == g_ptr->get_num_nodes()) // ¿se visitaron todos lo nodos?
      return true;

        // recorrer recursivamente arcos de node 
    for (Node_Arc_Iterator<GT, SA> it(node, sa); it.has_current(); it.next())
      {
	auto arc = it.get_current_arc();
	if (IS_ARC_VISITED(arc, Depth_First)) 
	  continue;

	ARC_BITS(arc).set_bit(Depth_First, true); // visitado
	if (__dft (it.get_tgt_node(), arc))
	  return true; // ya se exploró cabalmente it.get_tgt_node()
      }

    return false; // retorne y siga explorando
  }

  size_t dft(const GT & g, typename GT::Node * start_node, Operation & __op)
  {
    op = &__op;
    g_ptr = &g;
    
    g_ptr->reset_bit_nodes(Depth_First); // reiniciar Depth_First de nodos
    g_ptr->reset_bit_arcs(Depth_First);  // reiniciar Depth_First de arcos

    count = 0;

    __dft(start_node);

    return count;
  }

public:

  Depth_First_Traversal(SA & __sa) : sa(__sa) { /* empty */ }

  Depth_First_Traversal(SA && __sa = SA()) : sa(__sa) { /* empty */ }

   size_t operator () (const GT & g, Operation & op) 
  {
    return dft(g, g.get_first_node(), op);
  }

  size_t operator () (const GT & g, Operation && op = Operation()) 
  {
    return dft(g, g.get_first_node(), op);
  }
 
  size_t operator () (const GT & g, typename GT::Node * sn, Operation & op)
  {
    return dft(g, sn, op);
  }

  size_t operator () (const GT & g, typename GT::Node * sn, 
		      Operation && op = Operation())
  {
    return dft(g, sn, op);
  }
};


  template <class GT> inline static bool
__depth_first_traversal(const GT & g, typename GT::Node * node, 
			typename GT::Arc *  arc,
			bool (*visit)(const GT & g, typename GT::Node *, 
				      typename GT::Arc *),
			size_t & count)
{
  if (IS_NODE_VISITED(node, Depth_First)) 
    return false; 

  NODE_BITS(node).set_bit(Depth_First, true); // marca nodo visitado
  count++;

  if (visit != nullptr) // verifique si hay función de visita
    if ((*visit)(g, node, arc)) 
      return true;

  if (count == g.get_num_nodes()) // ¿se visitaron todos lo nodos?
    return true;

  for (auto it = g.get_arc_it(node); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Depth_First)) 
	continue;

      ARC_BITS(arc).set_bit(Depth_First, true); // visitado
      if (__depth_first_traversal(g, it.get_tgt_node(), arc, visit, count))
	return true; // ya se exploró cabalmente it.get_tgt_node()
    }

  return false; // retorne y siga explorando
} 


  template <class GT> inline size_t
breadth_first_traversal(const GT & g, typename GT::Node * start,
			bool (*visit)(const GT &, typename GT::Node *, 
				      typename GT::Arc *) )
{
  g.reset_bit_nodes(Breadth_First); 
  g.reset_bit_arcs(Breadth_First);  
  DynListQueue<typename GT::Arc*> q;

  for (auto it = g.get_arc_it(start); it.has_current(); it.next())
    q.put(it.get_current_arc());

  NODE_BITS(start).set_bit(Breadth_First, true); 
  size_t node_counter = 1; // count the visited nodes

  if (visit != nullptr)
    if ((*visit)(g, start, nullptr))
      return 1;

  while (not q.is_empty() and node_counter < g.get_num_nodes()) 
    {
      auto arc = q.get();// saque de cola más cercano
      ARC_BITS(arc).set_bit(Breadth_First, true); 

      auto src = g.get_src_node(arc); 
      auto tgt = g.get_tgt_node(arc);
      if (IS_NODE_VISITED(src, Breadth_First) and 
	  IS_NODE_VISITED(tgt, Breadth_First)) 
	continue; 

      auto visit_node = IS_NODE_VISITED(src, Breadth_First) ? tgt : src;
      if (visit != nullptr)
	if ((*visit)(g, visit_node, arc))
	  break;

      NODE_BITS(visit_node).set_bit(Breadth_First, true); 
      node_counter++;

      for (auto it = g.get_arc_it(visit_node); it.has_curr(); it.next())
	{
	  auto curr_arc = it.get_current_arc();
	  if (IS_ARC_VISITED(curr_arc, Breadth_First)) 
	    continue; 

	  if (IS_NODE_VISITED(g.get_src_node(curr_arc), Breadth_First) and 
	      IS_NODE_VISITED(g.get_tgt_node(curr_arc), Breadth_First))
	    continue; // nodos ya visitados 
	  
	  q.put(curr_arc);
	}
    }

    return node_counter;
  }

  template <class GT> inline size_t
breadth_first_traversal(GT & g, 
			bool (*visit)(const GT &, typename GT::Node *, 
				      typename GT::Arc *))
{
  return breadth_first_traversal(g, g.get_first_node(), visit);
}

      
      template <class GT, 
                class Operation = Default_Visit_Op<GT>, 
                class SA        = Dft_Show_Arc<GT>> 
class Breadth_First_Traversal
{
  SA     & sa;
  size_t   count;

  size_t bft(const GT & g, typename GT::Node * start, Operation & op)
  {
    g.reset_bit_nodes(Breadth_First); 
    g.reset_bit_arcs(Breadth_First);  
    DynListQueue<typename GT::Arc*> q; // cola de arcos pendientes

    for (Node_Arc_Iterator<GT, SA> it(start); it.has_current(); it.next())
      q.put(it.get_current_arc());

    NODE_BITS(start).set_bit(Breadth_First, true); 
    count = 1; // contador de nodos visitados

    if (op (g, start, nullptr))
      return 1;

    while (not q.is_empty() and count < g.get_num_nodes()) 
      {
	auto arc = q.get(); // saque de cola más cercano
	ARC_BITS(arc).set_bit(Breadth_First, true); 

	auto src = g.get_src_node(arc); 
	auto tgt = g.get_tgt_node(arc);

	if (IS_NODE_VISITED(src, Breadth_First) and
	    IS_NODE_VISITED(tgt, Breadth_First))
	  continue;

	auto curr = IS_NODE_VISITED(src, Breadth_First) ? tgt : src;
	if (op (g, curr, arc))
	  break;

	NODE_BITS(curr).set_bit(Breadth_First, true); 
	count++;

	for (Node_Arc_Iterator<GT, SA> it(curr); it.has_current(); it.next())
	  {
	    auto curr_arc = it.get_current_arc();
	    if (IS_ARC_VISITED(curr_arc, Breadth_First)) 
	      continue; 

	    if (IS_NODE_VISITED(g.get_src_node(curr_arc), Breadth_First) and 
		IS_NODE_VISITED(g.get_tgt_node(curr_arc), Breadth_First))
	      continue; // nodos ya visitados 

	    q.put(curr_arc);
	  }
      }

    return count;
  }  

public:

  Breadth_First_Traversal(SA & __sa) : sa(__sa) { /* empty */ }

  Breadth_First_Traversal(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  size_t operator () (const GT & g, Operation & op)
  {
    return bft (g, g.get_first_node(), op);
  }

  size_t operator () (const GT & g, Operation && op = Operation())
  {
    return bft (g, g.get_first_node(), op);
  }

  size_t operator () (const GT & g, typename GT::Node * p, 
		      Operation && op = Operation())
  {
    return bft(g, p, op);
  }

  size_t operator () (const GT & g, typename GT::Node * p, Operation & op)
  {
    return bft(g, p, op);
  }
};

    template <class GT> inline 
Path<GT> find_path_breadth_first(const GT & g, typename GT::Node * start, 
				 typename GT::Node * end)
{
  g.reset_nodes();
  g.reset_arcs(); 
  
  DynListQueue<typename GT::Arc*> q; 

  for (auto it = g.get_arc_it(start); it.has_curr(); it.next())
    q.put(it.get_current_arc());
        
  NODE_BITS(start).set_bit(Find_Path, true);

  bool path_found = false;

  while (not q.is_empty())
    {
      auto arc = q.get(); 
      auto src = g.get_src_node(arc); 
      auto tgt = g.get_tgt_node(arc);

      if (IS_NODE_VISITED(src, Find_Path) and IS_NODE_VISITED(tgt, Find_Path))
	continue;
      
      if (IS_NODE_VISITED(tgt, Find_Path))
	std::swap(src, tgt);

      ARC_BITS(arc).set_bit(Find_Path, true); 
      NODE_BITS(tgt).set_bit(Find_Path, true);
      NODE_COOKIE(tgt) = src;
        
      if (tgt == end)
	{
	  path_found = true;
	  break;
	}

      for (auto it = g.get_arc_it(tgt); it.has_curr(); it.next())
	{
	  auto a = it.get_current_arc();
	  if (IS_ARC_VISITED(a, Find_Path))
	    continue;

	  if (IS_NODE_VISITED(g.get_src_node(a), Find_Path) and 
	      IS_NODE_VISITED(g.get_tgt_node(a), Find_Path))
	    continue;

	  q.put(a);
	}
    }
  
  if (not path_found)
    return Path<GT>();

  q.empty(); // free queue memory for eventually saving the required for path
  
  Path<GT> path(g, end);
  auto p = end;
  while (p != start)
    {
      p = (typename GT::Node *) NODE_COOKIE(p);
      path.insert(p);
    }

  return path;
}


    template <class GT> inline 
bool test_connectivity(const GT & g)
{
  if (g.is_digraph()) // only valid for non directed
    throw std::domain_error("test_connectivity() does not work on digraphs");

  if (g.get_num_arcs() < g.get_num_nodes() - 1) 
    return false; 

  return depth_first_traversal<GT>(g, nullptr) == g.get_num_nodes();
}


template <class GT, class SA> inline static
bool __test_cycle(const GT & g, typename GT::Node *, typename GT::Node *);


  template <class GT> inline 
bool test_for_cycle(const GT & g, typename GT::Node * src)
{
  g.reset_bit_nodes(Test_Cycle);
  g.reset_bit_arcs(Test_Cycle);
  for (auto it = g.get_arc_it(src); it.has_curr(); it.next())
    {
      auto arc = it.get_curr();
      if (IS_ARC_VISITED(arc, Test_Cycle)) 
	continue;

      ARC_BITS(arc).set_bit(Test_Cycle, true);
      if (__test_cycle(g, src, it.get_tgt_node()))
	return true;
    }

  return false;
}


  template <class GT> inline static bool
__test_cycle(const GT & g, typename GT::Node * src, typename GT::Node * curr)
{
  if (src == curr) 
    return true; // detected cycle

  if (IS_NODE_VISITED(curr, Test_Cycle)) 
    return false;

  NODE_BITS(curr).set_bit(Test_Cycle, true);

  for (auto it = g.get_arc_it(curr); it.has_curr(); it.next())
    {
      auto arc = it.get_curr();
      if (IS_ARC_VISITED(arc, Test_Cycle)) 
	continue;

      ARC_BITS(arc).set_bit(Test_Cycle, true);
      if (__test_cycle(g, src, it.get_tgt_node()))
	return true;
    }

  return false; 
}  

      template <class GT> inline static
bool __is_graph_acyclique(const GT & g, typename GT::Node * curr_node)
  {
    if (IS_NODE_VISITED(curr_node, Test_Cycle)) 
      return false;

    NODE_BITS(curr_node).set_bit(Test_Cycle, true); // marcar nodo

    for (auto it = g.get_arc_it(curr_node); it.has_current(); it.next())
      {
        auto arc = it.get_current_arc();
        if (IS_ARC_VISITED(arc, Test_Cycle)) 
          continue; 

        ARC_BITS(arc).set_bit(Test_Cycle, true); 

        if (not __is_graph_acyclique(g, it.get_tgt_node())) 
          return false;
      }
        // todos los arcos recorridos sin encontrar ciclo ==>
        // el grafo es acíclico pasando por curr_node
    return true; 
  }


    template <class GT> inline 
bool is_graph_acyclique(const GT & g, typename GT::Node * start_node)
{
  if (g.is_digraph())
    throw std::domain_error("is_graph_acyclique() does not work for digraps");
  
  if (g.get_num_arcs() >= g.get_num_nodes()) 
    return false; 

  g.reset_bit_arcs(Test_Cycle);
  g.reset_bit_nodes(Test_Cycle);

  return __is_graph_acyclique(g, start_node);
}

    template <class GT> inline 
bool is_graph_acyclique(const GT & g)
{
  if (g.is_digraph())
    throw std::domain_error("is_graph_acyclique() does not work for digraps");

  if (g.get_num_arcs() >= g.get_num_nodes()) 
    return false; 

  g.reset_bit_arcs(Test_Cycle);
  g.reset_bit_nodes(Test_Cycle);

  for (auto it = g.get_node_it(); it.has_current(); it.next()) 
    {
      auto current_node = it.get_current_node();
      if (IS_NODE_VISITED(current_node, Test_Cycle)) 
	continue; 

      if (not __is_graph_acyclique(g, current_node)) 
	return false;
    }

  return true;
}

      
      template <class GT> 
inline bool has_cycle(const GT & g)
{
  return not is_graph_acyclique(g);
}

 
      template <class GT> inline 
bool test_for_path(const GT & g, typename GT::Node * start_node, 
		   typename GT::Node * end_node)
{ 
  if (not g.is_digraph() and g.get_num_arcs() >= g.get_num_nodes()) 
    return true;

  g.reset_bit_nodes(Find_Path); 
  g.reset_bit_arcs(Find_Path);

  for (auto it = g.get_arc_it(start_node); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      ARC_BITS(arc).set_bit(Find_Path, true); // marcar arco
      if (__test_for_path(g, it.get_tgt_node(), end_node)) 
	return true; 
    }

  return false;
}


      template <class GT> inline static
bool __test_for_path(const GT & g, typename GT::Node * curr_node, 
		     typename GT::Node * end_node)  
{
  if (curr_node == end_node) 
    return true;

  if (IS_NODE_VISITED(curr_node, Find_Path)) // ¿se visitó curr_node?
    return false; // sí, no explore

  NODE_BITS(curr_node).set_bit(Find_Path, true);

  for (auto it = g.get_arc_it(curr_node); it.has_current(); it.next())
    { 
      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Find_Path)) 
	continue; 

      ARC_BITS(arc).set_bit(Find_Path, true);
      if (__test_for_path(g, it.get_tgt_node(), end_node)) 
	return true;
    }

  return false;
}

template <class GT> inline 
DynList<GT> inconnected_components(const GT & g);

template <class GT> inline 
void build_subgraph(const GT & g, GT & sg, 
		    typename GT::Node * g_src, size_t & node_count); 
    

      template <class GT> inline
DynList<GT> inconnected_components(const GT & g) 
{
  g.reset_nodes(); 
  g.reset_arcs(); 

  DynList<GT> list;
  size_t count = 0; // counter of visited nodes
  for (auto it = g.get_node_it(); count < g.get_num_nodes() and it.has_curr(); 
       it.next())
    {
      auto curr = it.get_current_node();
      if (IS_NODE_VISITED(curr, Build_Subtree)) 
	continue;

      list.append(GT()); // crea subgrafo y lo inserta en lista
      GT & subgraph = list.get_last(); // grafo insertado en list
      build_subgraph(g, subgraph, curr, count); 
    }

  return list;
}


    template <class GT> inline 
void build_subgraph(const GT & g, GT & sg, 
		    typename GT::Node * g_src, size_t & node_count)
{
  if (IS_NODE_VISITED(g_src, Build_Subtree)) 
    return;

  NODE_BITS(g_src).set_bit(Build_Subtree, true); // mark g_src as visited
  ++node_count; 

  auto sg_src = mapped_node<GT>(g_src);
  if (sg_src == nullptr) // sg_src already mapped
    {
      sg_src = sg.insert_node(g_src->get_info());
      GT::map_nodes(g_src, sg_src);
    }

  for (auto it = g.get_arc_it(g_src); 
       node_count < g.get_num_nodes() and it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Build_Subtree)) 
	continue; 

      ARC_BITS(arc).set_bit(Build_Subtree, true);
      auto g_tgt  = it.get_tgt_node();
      auto sg_tgt = mapped_node<GT>(g_tgt);
      if (sg_tgt == nullptr) // sg_tgt mapped in sg?
	{
	  sg_tgt = sg.insert_node(g_tgt->get_info()); 
	  GT::map_nodes(g_tgt, sg_tgt); 
	}

      auto sg_arc = sg.insert_arc(sg_src, sg_tgt, arc->get_info());
      GT::map_arcs(arc, sg_arc); 

      build_subgraph(g, sg, g_tgt, node_count);
    }
}

  
    template <class GT> inline static
bool __find_depth_first_spanning_tree(const GT &          g, 
				      typename GT::Node * gnode, 
				      typename GT::Arc *  garc, 
				      GT &                tree,
				      typename GT::Node * tnode);

template <class GT> inline
GT find_depth_first_spanning_tree(const GT & g, typename GT::Node * gnode)
{
  g.reset_nodes();
  g.reset_arcs(); 

  GT tree;

  NODE_BITS(gnode).set_bit(Spanning_Tree, true); // marcar gnode
  
  auto tnode = tree.insert_node(gnode->get_info()); 
  GT::map_nodes(gnode, tnode);
  
  for (auto it = g.get_arc_it(gnode); it.has_curr(); it.next())
    {
      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Spanning_Tree)) 
	continue;

      auto arc_tgt_node = it.get_tgt_node();
      if (IS_NODE_VISITED(arc_tgt_node, Spanning_Tree))
	continue; // destino ya visitado desde otro arco

      if (__find_depth_first_spanning_tree(g, arc_tgt_node, arc, tree, tnode))
	return tree;
    }

  return tree;
}

      template <class GT> inline
GT find_depth_first_spanning_tree(const GT & g)
{
  return find_depth_first_spanning_tree(g, g.get_node());
}

  
      template <class GT> inline static
bool __find_depth_first_spanning_tree(const GT & g, typename GT::Node * gnode, 
				      typename GT::Arc *  garc, 
				      GT & tree, typename GT::Node * tnode)
{
  NODE_BITS(gnode).set_bit(Spanning_Tree, true); // marcar nodo 
  ARC_BITS(garc).set_bit(Spanning_Tree, true);   // marcar arco

  auto tree_tgt_node = tree.insert_node(gnode->get_info());
  GT::map_nodes(gnode, tree_tgt_node);

  auto tarc = tree.insert_arc(tnode, tree_tgt_node, garc->get_info()); 
  GT::map_arcs(garc, tarc);

  tnode = tree_tgt_node; 
  if (tree.get_num_nodes() == g.get_num_nodes()) // ¿grafo abarcado?
    return true; // tree ya contiene el árbol abarcador

  assert(tree.get_num_nodes() > tree.get_num_arcs()); // debe ser acíclico

  for (auto it = g.get_arc_it(gnode); it.has_curr(); it.next())
    {
      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Spanning_Tree)) 
	continue;

      auto arc_tgt_node = it.get_tgt_node();
      if (IS_NODE_VISITED(arc_tgt_node, Spanning_Tree))
	continue; // destino ya visitado desde otro arco

      if (__find_depth_first_spanning_tree(g, arc_tgt_node, arc, tree, tnode))
	return false; // ya el árbol está calculado 
    }

  return false;
}

      template <class GT> inline
GT find_breadth_first_spanning_tree(GT & g, typename GT::Node * gp)
{
  g.reset_bit_nodes(Spanning_Tree);
  g.reset_bit_arcs(Spanning_Tree);

  GT tree;

  unique_ptr<typename GT::Node> tp_auto(new typename GT::Node(gp));
  tree.insert_node(tp_auto.get());
  GT::map_nodes(gp, tp_auto.release());
  NODE_BITS(gp).set_bit(Spanning_Tree, true); // mark gp as visited

  DynListQueue<typename GT::Arc*> q; // queue of arcs
  for (auto it = g.get_arc_it(gp); it.has_curr(); it.next())
    q.put(it.get_curr());

  while (not q.is_empty()) 
    {
      auto garc = q.get(); 
      ARC_BITS(garc).set_bit(Spanning_Tree, true); // mark the arc as visited
      auto gsrc = g.get_src_node(garc);  
      auto gtgt = g.get_tgt_node(garc);

      if (IS_NODE_VISITED(gsrc, Spanning_Tree) and 
	  IS_NODE_VISITED(gtgt, Spanning_Tree))
	continue; // los dos nodos de garc ya fueron visitados

      if (IS_NODE_VISITED(gtgt, Spanning_Tree)) // gtgt visited?
	std::swap(gsrc, gtgt); // the non visited is gsrc

      auto tsrc = mapped_node<GT>(gsrc);
      NODE_BITS(gtgt).set_bit(Spanning_Tree, true); // mark gtgt visited

            // crear copia de gtgt, insertarlo en tree y mapearlo
      unique_ptr<typename GT::Node> ttgt_auto(new typename GT::Node(gtgt));
      tree.insert_node(ttgt_auto.get());
      auto ttgt = ttgt_auto.release();
      GT::map_nodes(gtgt, ttgt);

            // insertar nuevo arco en tree y mapearlo
      auto tarc = tree.insert_arc(tsrc, ttgt, garc->get_info());
      GT::map_arcs(garc, tarc);
      if (tree.get_num_nodes() == g.get_num_nodes()) // ¿abarca a g?
	break; 

            // insertar en cola arcos de gtgt
      for (auto it = g.get_arc_it(gtgt); it.has_curr(); it.next())
	{
	  auto current_arc = it.get_current_arc();
	  if (IS_ARC_VISITED(current_arc, Spanning_Tree)) 
	    continue;

                // revise nodos de arcos para ver si han sido visitados
	  if (IS_NODE_VISITED(g.get_src_node(current_arc),Spanning_Tree) and 
	      IS_NODE_VISITED(g.get_tgt_node(current_arc),Spanning_Tree))
	    continue; // nodos ya visitados ==> no meter arco
	  q.put(current_arc);
	}
    }

  return tree;
}

template <class GT>
GT build_spanning_tree(const DynArray<typename GT::Arc*> & arcs)
{
  using Node = typename GT::Node;
  using Arc  = typename GT::Arc;

  GT ret;
  DynMapTree<Node*, Node*> table;
  arcs.for_each([&table, &ret] (Arc * ga)
    {
      if (ga == nullptr)
	return;

      Node * gsrc = (Node*) ga->src_node;
      Node * gtgt = (Node*) ga->tgt_node;

      Node * tsrc;
      auto * pair_ptr = table.search(gsrc);
      if (pair_ptr)
	tsrc = pair_ptr->second;
      else
	{
	  tsrc = ret.insert_node(gsrc->get_info()); // borrar get_info()
	  table.insert(gsrc, tsrc);
	  NODE_COOKIE(tsrc) = gsrc;
	}
	
      Node * ttgt;
      pair_ptr = table.search(gtgt);
      if (pair_ptr)
	ttgt = pair_ptr->second;
      else
	{
	  ttgt = ret.insert_node(gtgt->get_info());
	  table.insert(gtgt, ttgt);
	  NODE_COOKIE(ttgt) = gtgt;
	}

      Arc * ta = ret.insert_arc(tsrc, ttgt);
      *ta = *ga;
      ARC_COOKIE(ta) = ga;
    });

  return ret;
}

    template <class GT> inline static 
long & df(typename GT::Node * p)
{
  return NODE_COUNTER(p);
}

    template <class GT> inline static 
long & low(typename GT::Node * p)
{
  return reinterpret_cast<long&>(NODE_COOKIE(p));
}

      template <class GT> inline static
void __compute_cut_nodes(const GT & g, DynList<typename GT::Node *> & list, 
			 typename GT::Node * p, typename GT::Arc * a,
			 long & curr_df);

      template <class GT>
DynList<typename GT::Node*> 
compute_cut_nodes(const GT & g, typename GT::Node * start)
{
  DynList<typename GT::Node*> list;

  g.for_each_node([] (auto p) // init the nodes
	  {
	    NODE_COUNTER(p) = 0;
	    NODE_BITS(p).reset();
	    low<GT>(p) = -1;
	  });
  g.reset_arcs();
  long current_df = 0; // contador global de visitas 
  NODE_BITS(start).set_bit(Depth_First, true); // marcar start
  df<GT>(start) = current_df++;
  int call_counter = 0; // contador llamadas recursivas
  
      // Recorra los arcos de start mientras g no haya sido abarcado
  for (auto it = g.get_arc_it(start); 
       it.has_curr() and current_df < g.get_num_nodes(); it.next())
    {
      auto tgt = it.get_tgt_node();
      if (IS_NODE_VISITED(tgt, Depth_First)) 
        continue; 

      auto arc = it.get_current_arc();
      if (IS_ARC_VISITED(arc, Depth_First)) 
        continue;

      ARC_BITS(arc).set_bit(Depth_First, true);
      __compute_cut_nodes(g, list, tgt, arc, current_df); 
      ++call_counter;
    }

  if (call_counter > 1) // ¿es la raíz un punto de articulación?
    {
      NODE_BITS(start).set_bit(Cut, true);
      list.append(start);
    }

  return list;
}

      template <class GT>
DynList<typename GT::Node*> compute_cut_nodes(const GT & g)
{
  return compute_cut_nodes(g, g.get_node());
}
      
      template <class GT> inline static
void __compute_cut_nodes(const GT & g, DynList<typename GT::Node *> & list, 
			 typename GT::Node * p, typename GT::Arc * a, 
			 long & curr_df)
{
  NODE_BITS(p).set_bit(Depth_First, true); // pinte p visitado
  low <GT> (p) = df <GT> (p) = curr_df++;  // asígnele df

      // recorrer arcos de p mientras no se abarque a g
  bool p_is_cut_node = false;
  for (auto it = g.get_arc_it(p); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      if (arc == a) 
	continue; // a es el padre de arc ==> ignorarlo

      auto tgt = it.get_tgt_node();
      if (IS_NODE_VISITED(tgt, Depth_First)) 
	{ 
	  if (not IS_ARC_VISITED(arc, Depth_First)) // no abarcador?
	    if (df<GT>(tgt) < low<GT>(p)) // sí, verificar valor low
	      low<GT>(p) = df<GT>(tgt); // actualizar low(p)

	  continue;
	}

      if (IS_ARC_VISITED(arc, Depth_First)) 
	continue;

      ARC_BITS(arc).set_bit(Depth_First, true); // marque arco

      __compute_cut_nodes(g, list, tgt, arc, curr_df);

      if (low<GT>(tgt) < low<GT>(p)) 
	low<GT>(p) = low<GT>(tgt); // actualizar low(p)

      if (low<GT>(tgt) >= df<GT>(p) and df<GT>(tgt) != 0) // ¿de corte?
	p_is_cut_node = true;
    }

        // aquí, p ya fue explorado recursivamente
  if (p_is_cut_node)
    {
      NODE_BITS(p).set_bit(Cut, true);
      list.append(p);
    }
}

const long Cross_Arc = -1;

    template <class GT> inline static 
bool is_a_cross_arc(typename GT::Arc * a) 
{
  return ARC_COUNTER(a) == Cross_Arc; 
}

     template <class GT> inline static 
bool is_a_cut_node(typename GT::Node * p)
{
  return NODE_BITS(p).get_bit(Cut);
}

      template <class GT> inline static 
bool is_an_cut_arc(typename GT::Arc * a)
{
  return ARC_BITS(a).get_bit(Cut);
}
      template <class GT> inline static 
bool is_node_painted(typename GT::Node * p)
{
  return NODE_COUNTER(p) > 0;
}

      template <class GT> inline static 
bool is_arc_painted(typename GT::Arc * arc)
{
  return ARC_COUNTER(arc) > 0;
}

      template <class GT> inline static 
void paint_node(typename GT::Node * p, const long & color)
{
  NODE_COUNTER(p) = color;
}

      template <class GT> inline static 
void paint_arc(typename GT::Arc * a, const long & color)
{
  ARC_COUNTER(a) = color;
}

      template <class GT> inline static 
const long & get_color(typename GT::Node * p)
{
  return NODE_COUNTER(p);
}

      template <class GT> inline static 
const long & get_color(typename GT::Arc * a)
{
  return ARC_COUNTER(a);
}


      template <class GT> inline static
void __paint_subgraph(const GT & g, typename GT::Node * p, long current_color)
{
  assert(not is_a_cut_node <GT> (p));

  if (is_node_painted <GT> (p)) 
    return; 

  paint_node <GT> (p, current_color);

  for (auto it = g.get_arc_it(p); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();
      if (is_arc_painted <GT> (arc))
          continue;

      auto tgt = it.get_tgt_node();
      if (is_a_cut_node <GT> (tgt))
	continue;

      paint_arc <GT> (arc, current_color); 

      __paint_subgraph(g, tgt, current_color); 
    }
}

template <class GT> inline static
void 
__paint_from_cut_node(const GT & g, typename GT::Node * p, long & current_color)
{
  assert(is_a_cut_node <GT> (p));

      // pintar recursivamente con dif colores bloques conectados a p
  for (auto it = g.get_arc_it(p); it.has_current(); it.next())
    {
      auto arc = it.get_current_arc();

      assert(not is_arc_painted <GT> (arc));

      auto tgt_node = it.get_tgt_node();
      if (is_a_cut_node <GT> (tgt_node)) // ¿ es un arco de corte?
	{
	  ARC_BITS(arc).set_bit(Cut, true); // marque como de corte
	  continue; // avance a próximo arco
	}
      else 
	{
	  paint_arc <GT> (arc, Cross_Arc); // marque como de cruce
	  if (is_node_painted <GT> (tgt_node)) 
	    continue; 
	}

          // pintar recursivamente nodo conectado a arc
      __paint_subgraph(g, tgt_node, current_color);

      current_color++; // cambiar color (sig arco en otro bloque)

      assert(not is_arc_painted <GT> (arc));
    }
}

    template <class GT> inline long 
paint_subgraphs(const GT & g, const DynList<typename GT::Node*> & cut_node_list)
{
  g.reset_counter_nodes();
  g.reset_counter_arcs();
  long current_color = 1;

        // Recorrer cada nodo de corte y pintar sus bloques
  for (auto it = cut_node_list.get_it(); it.has_current(); it.next())
    __paint_from_cut_node(g, it.get_current(), current_color);

  return current_color;
}

         

       template <class GT> inline static
void __map_subgraph(const GT & g, GT & sg, typename GT::Node * gsrc, 
		    const long color)
{
  assert(get_color <GT> (gsrc) == color);

  auto tsrc = mapped_node<GT>(gsrc); // gsrc en sg

      // recorrer arcos de gsrc y añadir a sg los del color de interés
  for (auto it = g.get_arc_it(gsrc); it.has_current(); it.next())
    {
      auto garc = it.get_current_arc();
      if (get_color<GT>(garc) != color or IS_ARC_VISITED(garc, Build_Subtree))
	continue; // arco es de otro color o ya está visitado 

      ARC_BITS(garc).set_bit(Build_Subtree, true); 

      auto gtgt = it.get_tgt_node(); 

      assert(get_color <GT> (gtgt) == color);

      auto ttgt = nullptr; // imagen gtgt en sg
      if (IS_NODE_VISITED(gtgt, Build_Subtree)) // ¿gtgt en sg?
	ttgt = mapped_node<GT> (gtgt);
      else
	{     // gtgt no está en sg ==> copiarlo y mapearlo
	  unique_ptr<typename GT::Node> ttgt_auto(new typename GT::Node(gtgt));
	  sg.insert_node(ttgt_auto.get());
	  GT::map_nodes(gtgt, ttgt_auto.get());
	  NODE_BITS(gtgt).set_bit(Build_Subtree, true); 
	  ttgt = ttgt_auto.release(); 
	}

      auto tarc = sg.insert_arc(tsrc, ttgt, garc->get_info());
      GT::map_arcs(garc, tarc);

      __map_subgraph(g, sg, gtgt, color);
    }
}

      template <class GT>
GT map_subgraph(const GT & g, const long color)
{
  typename GT::Node * first = nullptr; // busque primer nodo con color
  for (auto it = g.get_node_it(); it.has_current(); it.next())
    if (get_color <GT> (it.get_current_node()) == color)
      first = it.get_current_node();

  if (first == nullptr) // Encontró el color?
    throw std::domain_error("Color does not exist in the graph");

  GT sg;
  unique_ptr<typename GT::Node> auto_tsrc(new typename GT::Node(first));
  sg.insert_node(auto_tsrc.get());
  GT::map_nodes(first, auto_tsrc.release());
  NODE_BITS(first).set_bit(Build_Subtree, true);

  __map_subgraph(g, sg, first, color); // mapee first
  
  return sg;
}

      template <class GT>
      std::tuple<GT, DynList<typename GT::Arc*>>
map_cut_graph(const GT & g, const DynList<typename GT::Node*> & cut_node_list)
{
  GT cut_graph;
  DynList<typename GT::Arc*> cross_arc_list;

  for (auto it = cut_node_list.get_it(); it.has_curr(); it.next())
    {
      auto gp = it.get_current();

      assert(is_a_cut_node <GT> (gp));

      unique_ptr<typename GT::Node> tp_auto(new typename GT::Node(gp));
      cut_graph.insert_node(tp_auto.get());
      GT::map_nodes(gp, tp_auto.release());
    }

  // cut_graph = {not cut arcs} U cross_arc_list = {cross arcs}
  for (auto it = g.get_arc_it(); it.has_current(); it.next())
    {
      auto garc = it.get_current_arc();
      if (is_a_cross_arc <GT> (garc))
	{
	  cross_arc_list.append(garc); 
	  continue;
	}

      if (not is_an_cut_arc <GT> (garc)) 
	continue;

      auto src = mapped_node<GT>(g.get_src_node(garc));
      auto tgt = mapped_node<GT>(g.get_tgt_node(garc));

      assert(src != nullptr and tgt != nullptr);

      auto arc = cut_graph.insert_arc(src, tgt, garc->get_info());
      GT::map_arcs(garc, arc);
    }
  
  return { cut_graph, cross_arc_list };
}


    template <class GT, class Distance> 
struct Distance_Compare
{
  Distance & dist;

  Distance_Compare(Distance & __dist) : dist(__dist) { /* empty */ }

  Distance_Compare(Distance && dist = Distance()) 
    : Distance_Compare(dist)
  {
    // empty
  }

  bool operator () (typename GT::Arc * a1, typename GT::Arc * a2)
  {
    return dist(a1) < dist(a2);
  }
};


      template <class GT> 
GT invert_digraph(const GT & g)
{
  g.reset_nodes();
  GT gi;

  for (auto it = g.get_arc_it(); it.has_current(); it.next())
    {
      auto arc   = it.get_current();

      auto ssrc = g.get_src_node(arc); 
      auto rsrc = mapped_node<GT> (ssrc);
      if (rsrc == nullptr) // ¿ya está creado ssrc en gi?
	{     // no == crearlo, insertarlo y mapearlo
	  unique_ptr<typename GT::Node> rsrc_auto(new typename GT::Node(ssrc));
	  gi.insert_node(rsrc_auto.get());
	  GT::map_nodes(ssrc, rsrc_auto.get());
	  rsrc = rsrc_auto.release(); 
	}

      auto stgt = g.get_tgt_node(arc);
      auto rtgt = mapped_node<GT> (stgt);
      if (rtgt == nullptr) // ¿ya está creado ssrc en gi?
	{     // no == crearlo, insertarlo y mapearlo
	  unique_ptr<typename GT::Node> rtgt_auto(new typename GT::Node(stgt));
	  gi.insert_node(rtgt_auto.get());
	  GT::map_nodes(stgt, rtgt_auto.get());
	  rtgt = rtgt_auto.release(); 
	}

      typename GT::Arc * ai = gi.insert_arc(rtgt, rsrc, arc->get_info());
      GT::map_arcs(arc, ai);
    }

  assert(g.get_num_arcs() == gi.get_num_arcs() and 
    g.get_num_nodes() == gi.get_num_nodes());

  return gi;
}


      template <class GT, class SA = Dft_Show_Arc<GT>>
class Invert_Digraph
{
  SA & sa;

public:

  Invert_Digraph(SA & __sa) : sa(__sa) { /* empty */ }
  
  Invert_Digraph(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  GT operator () (const GT & g) const
  {
    g.reset_nodes();
    GT gi;

    for (Arc_Iterator<GT, SA> it(g, sa); it.has_curr(); it.next())
      {
	auto arc   = it.get_current();

	auto ssrc = g.get_src_node(arc); 
	auto rsrc = mapped_node<GT> (ssrc);
	if (rsrc == nullptr) // ¿ya está creado ssrc en gi?
	  {     // no == crearlo, insertarlo y mapearlo
	    unique_ptr<typename GT::Node> rsrc_auto(new typename GT::Node(ssrc));
	    gi.insert_node(rsrc_auto.get());
	    GT::map_nodes(ssrc, rsrc_auto.get());
	    rsrc = rsrc_auto.release(); 
	  }

	auto stgt = g.get_tgt_node(arc);
	auto rtgt = mapped_node<GT> (stgt);
	if (rtgt == nullptr) // ¿ya está creado ssrc en gi?
	  {     // no == crearlo, insertarlo y mapearlo
	    unique_ptr<typename GT::Node> rtgt_auto(new typename GT::Node(stgt));
	    gi.insert_node(rtgt_auto.get());
	    GT::map_nodes(stgt, rtgt_auto.get());
	    rtgt = rtgt_auto.release(); 
	  }

	typename GT::Arc * ai = gi.insert_arc(rtgt, rsrc, arc->get_info());
	GT::map_arcs(arc, ai);
      }

    assert(g.get_num_arcs() == gi.get_num_arcs() and 
	   g.get_num_nodes() == gi.get_num_nodes());

    return gi;
    invert_digraph <GT, SA> (g, gi, sa);
  }
};

    template <class GT>
class Dft_Dist
{
public:

  typedef typename GT::Arc_Type Distance_Type;

  static const Distance_Type Zero_Distance;

  static const Distance_Type Max_Distance;

  Distance_Type & operator () (typename GT::Arc * a) const
  {
    return a->get_info();
  }

  Distance_Type & operator () (typename GT::Arc * a, typename GT::Node*) const
  {
    return a->get_info();
  }

  static void set_zero(typename GT::Arc * a) { a->get_info() = 0; }
};

template <class GT>
const typename Dft_Dist<GT>::Distance_Type Dft_Dist<GT>::Max_Distance =
  std::numeric_limits<typename Dft_Dist<GT>::Distance_Type>::max();

template <class GT>
const typename Dft_Dist<GT>::Distance_Type Dft_Dist<GT>::Zero_Distance = 0.0;

template <class GT, class Distance = Dft_Dist<GT>>
  typename Distance::Distance_Type
get_min_path(typename GT::Node * s, typename GT::Node * end, Path<GT> & path)
{
  typename Distance::Distance_Type dist = 0.0;
  path.empty();
  path.insert(end);

  auto p = end;
  do
    {
      p = (typename GT::Node *) NODE_COOKIE(p);
      path.insert(p);
      dist = dist + Distance() (path.get_first_arc());
    }
  while (p != s);

  return dist;
}

template <class GT, 
	  class Distance = Dft_Dist<GT>, 
	  class SA       = Dft_Show_Arc<GT>>
class Total_Cost
{
  Distance & dist;
  SA &       sa;

public:

  Total_Cost(Distance && __dist = Distance(), SA && __sa = SA()) 
    : dist(__dist), sa(__sa)
  {
    // empty
  }

  Total_Cost(Distance & __dist, SA & __sa) : dist(__dist), sa(__sa)
  {
    // empty
  }

  typename Distance::Distance_Type total_cost(GT & g)
  {
    typename Distance::Distance_Type sum = 0;
  
        // recorrer todos los arcos y sumar su peso
    for (Arc_Iterator <GT, SA> it(g, sa); it.has_curr(); it.next())
      sum += dist(it.get_current_arc());

    return sum;
  }

  typename Distance::Distance_Type operator () (GT & g)
  {
    return total_cost (g);
  }

  bool operator () (typename GT::Arc * a)
  {
    if (not sa(a))
      return false;

    dist += dist(a);
    return true;
  }
};



} // end namespace Aleph

# endif // TPL_GRAPH_UTILS_H
