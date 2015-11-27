
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

# ifndef PRIM_H
# define PRIM_H

# include <tpl_agraph.H>
# include <tpl_graph_utils.H>
# include <archeap.H>

namespace Aleph {

using namespace Aleph;

    template <class GT> 
struct Prim_Info
{
  typename GT::Node * tree_node; // imagen en el árbol abarcador
  void *              heap_node; // puntero en el heap exclusivo

  Prim_Info() : tree_node(nullptr), heap_node(nullptr) { /* empty */ } 
};

# define PRIMINFO(p) static_cast<Prim_Info<GT>*>(NODE_COOKIE(p))
# define TREENODE(p) (PRIMINFO(p)->tree_node)
# define HEAPNODE(p) (PRIMINFO(p)->heap_node)

    template <class GT, class Distance> 
struct Prim_Heap_Info
{
  typedef typename ArcHeap<GT, Distance, Prim_Heap_Info>::Node Node;

  Node *& operator () (typename GT::Node * p) 
  { 
    return (Node*&) HEAPNODE(p); 
  }
};

  template <class GT, class Distance> 
struct Simple_Prim_Heap
{
  typedef typename ArcHeap<GT, Distance, Simple_Prim_Heap>::Node Node;

  Node *& operator () (typename GT::Node * p) 
  { 
    return (Node*&) NODE_COOKIE(p); 
  }
};

    template <class GT> 
struct Init_Prim_Info
{
  GT & tree;

  Init_Prim_Info(GT & __tree) : tree(__tree) { /* empty */ }

  void operator () (const GT & g, typename GT::Node * p)
  {
    g.reset_bit(p, Aleph::Spanning_Tree); 
    NODE_COOKIE(p) = new Prim_Info<GT>;
    TREENODE(p) = tree.insert_node(p->get_info());
  }
};

    template <class GT> 
struct Uninit_Prim_Info
{
  void operator () (const GT &, typename GT::Node * p)
  {
    Prim_Info<GT> * aux = PRIMINFO(p);
    GT::map_nodes(p, TREENODE(p));
    delete aux;
  }
};


 template <class GT, 
	   class Distance = Dft_Dist<GT>, 
	   class SA       = Dft_Show_Arc<GT>> 
class Prim_Min_Spanning_Tree
{
  typedef Prim_Heap_Info<GT, Distance> Acc_Heap;

  typedef Simple_Prim_Heap<GT, Distance> Acc_Simple_Heap;

  typedef ArcHeap<GT, Distance, Acc_Heap> Heap;

  typedef ArcHeap<GT, Distance, Acc_Simple_Heap> Simple_Heap;

  Distance & dist;
  SA &       sa; 

public:

  Prim_Min_Spanning_Tree(Distance && __dist = Distance(),  
			 SA &&       __sa = SA())
    : dist(__dist), sa(__sa)
  {
    // empty
  }

  Prim_Min_Spanning_Tree(Distance & __dist, SA & __sa)
    : dist(__dist), sa(__sa)
  {
    // empty
  }

private:

  void paint_min_spanning_tree(const GT & g, typename GT::Node * first)
  {
    if (g.is_digraph())
      throw std::domain_error("g is a digraph");

    g.reset_nodes();
    g.reset_arcs();

    NODE_BITS(first).set_bit(Aleph::Spanning_Tree, true); // visitado

    Simple_Heap heap(dist, Acc_Simple_Heap());
    for (Node_Arc_Iterator<GT, SA> it(first, sa); it.has_curr(); it.next())
      {
	typename GT::Arc * arc = it.get_current_arc();
	heap.put_arc(arc, it.get_tgt_node());
      }

    const size_t V1 = g.get_num_nodes() - 1;
    size_t count = 0;

    while (count < V1 and not heap.is_empty()) 
      {     // obtenga siguiente menor arco 
	typename GT::Arc * min_arc = heap.get_min_arc();
	if (IS_ARC_VISITED(min_arc, Aleph::Spanning_Tree))
	  continue;

	ARC_BITS(min_arc).set_bit(Aleph::Spanning_Tree, true);

	typename GT::Node * src = g.get_src_node(min_arc);
	typename GT::Node * tgt = g.get_tgt_node(min_arc);
	if (IS_NODE_VISITED(src, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  continue; // este arco cerraría un ciclo en el árbol 

	typename GT::Node * tgt_node = 
	  IS_NODE_VISITED(src, Aleph::Spanning_Tree) ? tgt : src;

	NODE_BITS(tgt_node).set_bit(Aleph::Spanning_Tree, true);

	    // insertar en heap arcos no visitados de tgt_node
	for (Node_Arc_Iterator<GT, SA> it(tgt_node, sa); it.has_curr();
	     it.next())
	  {
	    typename GT::Arc * arc = it.get_current_arc();
	    if (IS_ARC_VISITED(arc, Aleph::Spanning_Tree))
	      continue;

	    typename GT::Node * tgt = it.get_tgt_node();
	    if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree)) 
	      continue; // nodo visitado ==> causará ciclo 

	    heap.put_arc(arc, tgt);
	  }

	++count;
	ARC_BITS(min_arc).set_bit(Aleph::Spanning_Tree, true);
      }
  }

  void min_spanning_tree(const GT & g, typename GT::Node * first, GT & tree)
  {
    if (g.is_digraph())
      throw std::domain_error("g is a digraph");

    clear_graph(tree); 

    Init_Prim_Info <GT> init(tree);
    Operate_On_Nodes <GT, Init_Prim_Info<GT>> () (g, init);
    g.reset_arcs();

    NODE_BITS(first).set_bit(Aleph::Spanning_Tree, true); // visitado

    Heap heap(dist, Acc_Heap()) ;
        // meter en heap arcos iniciales del primer nodo
    for (Node_Arc_Iterator<GT, SA> it(first, sa); it.has_curr(); it.next())
      {
	typename GT::Arc * arc = it.get_current_arc();
	heap.put_arc(arc, it.get_tgt_node());
      }

    const size_t V1 = g.get_num_nodes() - 1;

    while (tree.get_num_arcs() < V1 and not heap.is_empty()) 
      {     // obtenga siguiente menor arco 
	typename GT::Arc * min_arc = heap.get_min_arc();
	if (IS_ARC_VISITED(min_arc, Aleph::Spanning_Tree))
	  continue;

	ARC_BITS(min_arc).set_bit(Aleph::Spanning_Tree, true);

	typename GT::Node * src    = g.get_src_node(min_arc);
	typename GT::Node * tgt    = g.get_tgt_node(min_arc);
	if (IS_NODE_VISITED(src, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  continue; // este arco cerraría un ciclo en el árbol 

	typename GT::Node * tgt_node = 
	  IS_NODE_VISITED(src, Aleph::Spanning_Tree) ? tgt : src;

	NODE_BITS(tgt_node).set_bit(Aleph::Spanning_Tree, true);

	    // insertar en heap arcos no visitados de tgt_node
	for (Node_Arc_Iterator<GT, SA> it(tgt_node, sa); it.has_curr(); 
	     it.next())
	  {
	    typename GT::Arc * arc = it.get_current_arc();
	    if (IS_ARC_VISITED(arc, Aleph::Spanning_Tree))
	      continue;

	    typename GT::Node * tgt = it.get_tgt_node();
	    if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree)) 
	      continue; // nodo visitado ==> causará ciclo 

	    heap.put_arc(arc, tgt);
	  }

	    // insertar nuevo arco en tree y mapearlo
	typename GT::Arc * tree_arc = 
	  tree.insert_arc(TREENODE(src), TREENODE(tgt), min_arc->get_info());
	GT::map_arcs(min_arc, tree_arc); 
      }

    Operate_On_Nodes <GT, Uninit_Prim_Info<GT> > () (g);   
  }

public:

  void operator () (const GT & g, GT & tree) 
  {
    min_spanning_tree(g, g.get_first_node(), tree); 
  }

  void operator () (const GT & g, typename GT::Node * start, GT & tree) 
  {
    min_spanning_tree(g, start, tree);
  }

  void operator () (const GT & g) 
  {
    paint_min_spanning_tree(g, g.get_first_node()); 
  }

  void operator () (const GT & g, typename GT::Node * start) 
  {
    paint_min_spanning_tree(g, start); 
  }
};

   

# undef HEAPNODE
# undef TREENODE
# undef PRIMINFO
} // end namespace Aleph

# endif // PRIM_H
