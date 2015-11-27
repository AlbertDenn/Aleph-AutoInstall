
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

# ifndef DIJKSTRA_H
# define DIJKSTRA_H

# include <ahFunction.H>
# include <archeap.H>
# include <tpl_find_path.H>
# include <tpl_agraph.H>

namespace Aleph {

    // conversión de cookie a Node_Info 
# define DNassert(p) ((Node_Info*) NODE_COOKIE((p)))

    // Acceso al nodo del árbol en el grafo
# define TREENODE(p) (((Tree_Node_Info*)DNassert(p))->tree_node)

# define ACC(p) (DNassert(p)->dist) // Acceso a la distancia acumulada
# define HEAPNODE(p) (DNassert(p)->heap_node)
# define PARENT(p) (DNassert(p)->ret_node)

# define DAassert(p) ((Arc_Info*) ARC_COOKIE(p))
# define ARC_DIST(p) (Distance () (p))
# define TREEARC(p) (((Tree_Arc_Info*)DAassert(p))->tree_arc)
# define POT(p) (DAassert(p)->pot)
# define GRAPHNODE(p) (static_cast<typename GT::Node*>(NODE_COOKIE(p)))



  template <class GT, 
	    class Distance = Dft_Dist<GT>,
	    template <typename, class> class Itor = Node_Arc_Iterator,
	    class SA       = Dft_Show_Arc<GT>>
class Dijkstra_Min_Paths
{
  // Aunque fundamentalmente el algoritmo es el mismo, hay dos enfoques
  // para calcular el camino mínimo. El primero es pintar el árbol
  // abarcador de todos los caminos mínimos a partir de un nodo
  // start. Por pintar se entiende que la solución se encuentra dentro
  // del mismo grafo y que se distingue mediante marcas.
  //
  // El otro enfoque consiste en construir un árbol abarcador separado.
  //
  // Según un enfoque u otro, el grafo se inicializa con información
  // distinta. Así, las clases que comiencen con el prefijo "Tree" son
  // clases que atañen a la solucipon que construye un árbol abarcador
  // separado. 

  // Información a colocar en el arco para la versión que pinta
  struct Arc_Info
  {
    typename Distance::Distance_Type pot; // potencial del arco
  };

  // Información a colocar en el arco para la versión que construye árbol
  struct Tree_Arc_Info : public Arc_Info
  {
    typename GT::Arc * tree_arc; // imagen en árbol 
    typename Distance::Distance_Type pot; // potencial del arco
  };

  // Wrapper de acceso a la distancia (mediante la clase parámetro Distance)
  struct Get_Potential_Arc : public Distance
  {
    Get_Potential_Arc() noexcept { /* empty */ }

    Get_Potential_Arc(Distance & d) noexcept : Distance(d) { /* empty */ }

    typename Distance::Distance_Type 
    operator () (typename GT::Arc *a) const noexcept
    {
      auto arc_info = (Arc_Info*) ARC_COOKIE(a);
      return arc_info->pot;
    }
  };

  // Información a colocar en el nodo para la versión que pinta
  struct Node_Info
  {
    typename Distance::Distance_Type dist;      // distancia acumulada
    void *                           heap_node;
    void *                           ret_node; // padre en árbol

    Node_Info() noexcept : dist(0), heap_node(nullptr), ret_node(nullptr)
    { 
      // empty
    }
  };

  // Información a colocar en el nodo para la versión que construye árbol
  struct Tree_Node_Info : public Node_Info
  {
    typename GT::Node *              tree_node; // nodo árbol abarcador
    typename Distance::Distance_Type dist;      // distancia acumulada
    void *                           heap_node;

    Tree_Node_Info() noexcept : tree_node(nullptr)
    { 
      // empty
    }
  };

  // Acceso al heap de arcos
  struct Dijkstra_Heap_Info
  {
        typedef typename 
    ArcHeap<GT, Get_Potential_Arc, Dijkstra_Heap_Info>::Node Node;

    Node *& operator () (typename GT::Node * p) const noexcept
    {
      return (Node*&) HEAPNODE(p); 
    }
  }; 

  // Inicialización de nodo para la versión que pinta
  struct Initialize_Node
  {
    void operator () (const GT & g, typename GT::Node * p) const noexcept
    {
      g.reset_bit(p, Aleph::Spanning_Tree);
      NODE_COOKIE(p) = new Node_Info; 
    }
  };

  // Liberación de memoria de nodo para la versión que pinta
  struct Destroy_Node
  {
    void operator () (const GT &, typename GT::Node * p) const noexcept
    {
      void * tmp = PARENT(p);
      delete DNassert(p); //bloque a liberar
      NODE_COOKIE(p) = tmp;
    }
  };

  // Inicialización de arco para la versión que pinta
  struct Initialize_Arc
  {
    void operator () (const GT & g, typename GT::Arc * a) const noexcept
    {
      g.reset_bit(a, Aleph::Spanning_Tree);
      ARC_COOKIE(a) = new Arc_Info;
      POT(a) = 0;
    }
  };

  // Liberación de memoria de arco para la versión que pinta
  struct Destroy_Arc
  {
    void operator () (const GT &, typename GT::Arc * ga) const noexcept
    {
      delete DAassert(ga); 
    }
  };

  // Inicialización de memoria de nodo para la versión que construye árbol
  struct Initialize_Tree_Node
  {
    void operator () (const GT & g, typename GT::Node * p) const noexcept
    {
      g.reset_bit(p, Aleph::Spanning_Tree);
      NODE_COOKIE(p) = new Tree_Node_Info; 
    }
  };

  // Liberación de memoria de nodo y mapeo para versión que construye árbol
  struct Destroy_Tree_Node
  {
    void operator () (const GT &, typename GT::Node * p) const noexcept
    {
      auto aux = (Tree_Node_Info *) DNassert(p); //bloque a liberar
      auto tp = TREENODE(p); // imagen en árbol abarcador
      if (tp != nullptr) // ¿está este nodo incluído en el árbol abarcador? 
	{
	  NODE_COOKIE(p) = NODE_COOKIE(tp) = nullptr;
	  GT::map_nodes (p, tp);
	}
      else
	NODE_COOKIE(p) = nullptr;

      delete aux; 
    }
  };

  // Inicialización de arco para la versión que construye árbol
  struct Initialize_Tree_Arc
  {
    void operator () (const GT & g, typename GT::Arc * a) const noexcept
    {
      g.reset_bit(a, Aleph::Spanning_Tree);
      ARC_COOKIE(a) = new Tree_Arc_Info;
      POT(a) = 0;
      TREEARC(a) = nullptr;
    }
  };

  // Liberación de memoria y mapeo para la versión que construye árbol
  struct Destroy_Tree_Arc
  {
    void operator () (const GT &, typename GT::Arc * ga) const noexcept
    {
      auto aux = (Tree_Arc_Info *) DAassert(ga); 
      typename GT::Arc * ta = TREEARC(ga);
      if (ta != nullptr) // ¿pertenece este arco al árbol abarcador?
	{    
	  assert(IS_ARC_VISITED(ga, Aleph::Spanning_Tree));
	  GT::map_arcs (ga, ta); // sí ==> mapearlo
	}

      delete aux;
    }
  };

  typedef Dijkstra_Heap_Info Heap_Info;

  typedef ArcHeap<GT, Get_Potential_Arc, Heap_Info> Heap;

  SA &                sa;
  Get_Potential_Arc   get_pot;
  Heap                heap;
  bool                painted;
  GT *                ptr_g;
  typename GT::Node * s;

public:

  // Constructores
  
  Dijkstra_Min_Paths(Distance && dist = Distance(),  
		     SA &&       __sa = SA())
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa), get_pot(dist), heap(get_pot, Heap_Info()), 
      painted(false), ptr_g(nullptr), s(nullptr)
  {
    // empty
  }

  Dijkstra_Min_Paths(Distance & dist, SA & __sa)
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa), get_pot(dist), heap(get_pot, Heap_Info()), 
      painted(false), ptr_g(nullptr), s(nullptr)
  {
    // empty
  }

private:

      template <class IN, class IA>
  void init(const GT & g, typename GT::Node * start)
  {
    heap.empty();

    ptr_g = &const_cast<GT&>(g);
    s     = start;

    Operate_On_Nodes<GT, IN> () (g);

    (Operate_On_Arcs <GT, IA> (sa)) (g);
  }

     template <class DN, class DA>
  void uninit()
  {
    Operate_On_Nodes <GT, DN> () (*ptr_g);

    (Operate_On_Arcs <GT, DA, SA> (sa)) (*ptr_g);
  }

public:

  void 
  compute_min_paths_tree(const GT & g, typename GT::Node * start, GT & tree)
  {
    init<Initialize_Tree_Node, Initialize_Tree_Arc>(g, start);

    clear_graph(tree); // limpiar árbol abarcador destino 

    NODE_BITS(start).set_bit(Aleph::Spanning_Tree, true); 
    ACC(start)                   = 0; 
    TREENODE(start)              = tree.insert_node(start->get_info());
    NODE_COOKIE(TREENODE(start)) = start;

    for (Itor<GT, SA> it(start, sa); it.has_curr(); it.next()) 
      {
	auto arc = it.get_current_arc();
	POT(arc) = ARC_DIST(arc);
	heap.put_arc(arc, it.get_tgt_node());
      }

    const auto & n = g.get_num_nodes();

    while (tree.get_num_nodes() < n)  // mientras tree no abarque a g
      {
	auto garc  = heap.get_min_arc(); 
	if (IS_ARC_VISITED(garc, Aleph::Spanning_Tree))
	  continue;

	auto gsrc = g.get_src_node(garc);
	auto gtgt = g.get_tgt_node(garc);

	      // ¿Están los dos nodos visitados?
	if (IS_NODE_VISITED(gsrc, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(gtgt, Aleph::Spanning_Tree))
	  continue; // insertar arco causaría ciclo

	ARC_BITS(garc).set_bit(Aleph::Spanning_Tree, true);	  

	if (IS_NODE_VISITED(gtgt, Aleph::Spanning_Tree))
	  std::swap(gsrc, gtgt);

	NODE_BITS(gtgt).set_bit(Aleph::Spanning_Tree, true);

	auto ttgt = tree.insert_node(gtgt->get_info());
	TREENODE(gtgt)           = ttgt;

	auto tarc = // insertar nuevo arco en tree 
	  tree.insert_arc(TREENODE(gsrc), TREENODE(gtgt), garc->get_info());
	TREEARC(garc) = tarc;

	ACC(gtgt) = ACC(gsrc) + ARC_DIST(garc); // total dist nodo inicial
	const auto & acc = ACC(gtgt);

	// por cada arco calcular potencial e insertarlo en heap
	for (Itor<GT, SA> it(gtgt, sa); it.has_curr(); it.next()) 
	  {
	    auto arc = it.get_current_arc();
	    if (IS_ARC_VISITED(arc, Aleph::Spanning_Tree))
	      continue;

	    auto tgt = it.get_tgt_node();
	    if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree)) 
	      continue; // causaría ciclo ==> no se mete en heap  

	    POT(arc) = acc + ARC_DIST(arc); // calcula potencial
	    heap.put_arc(arc, tgt);
	  }
      } 

    uninit<Destroy_Tree_Node, Destroy_Tree_Arc> ();
  }

  void compute_partial_min_paths_tree(const GT & g, 
				      typename GT::Node * start,
				      typename GT::Node * end, 
				      GT & tree)
  {
    init <Initialize_Tree_Node, Initialize_Tree_Arc> (g, start);
    clear_graph(tree);

    NODE_BITS(start).set_bit(Aleph::Spanning_Tree, true); 
    ACC(start)                   = 0; 
    TREENODE(start)              = tree.insert_node(start->get_info());
    NODE_COOKIE(TREENODE(start)) = start;

    for (Itor<GT, SA> it(start, sa); it.has_curr(); it.next()) 
      {
	auto arc = it.get_current_arc();
	POT(arc) = ARC_DIST(arc);
	heap.put_arc(arc, it.get_tgt_node());
      }

    const auto & n = g.get_num_nodes();

    while (tree.get_num_nodes() < n and not heap.is_empty()) // tree no abarque
      {
	auto garc  = heap.get_min_arc(); 
	if (IS_ARC_VISITED(garc, Aleph::Spanning_Tree))
	  continue;

	auto gsrc = g.get_src_node(garc);
	auto gtgt = g.get_tgt_node(garc);

	// ¿Están los dos nodos visitados?
	if (IS_NODE_VISITED(gsrc, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(gtgt, Aleph::Spanning_Tree))
	  continue; // insertar arco causaría ciclo

	ARC_BITS(garc).set_bit(Aleph::Spanning_Tree, true);

	if (IS_NODE_VISITED(gtgt, Aleph::Spanning_Tree))
	  std::swap(gsrc, gtgt);

	auto ttgt      = tree.insert_node(gtgt->get_info());
	TREENODE(gtgt) = ttgt;
	NODE_BITS(gtgt).set_bit(Aleph::Spanning_Tree, true);

	auto tarc = // insertar nuevo arco en tree 
	  tree.insert_arc(TREENODE(gsrc), TREENODE(gtgt), garc->get_info());
	TREEARC(garc) = tarc;

	if (gtgt == end) // ¿está end_node en árbol abarcador? 
	  break; // sí ==> camino mínimo ya está en árbol abarcador

	ACC(gtgt) = ACC(gsrc) + ARC_DIST(garc); // total dist nodo inicial
	const auto & acc = ACC(gtgt);

	      // por cada arco calcular potencial e insertarlo en heap
	for (Itor<GT, SA> it(gtgt, sa); it.has_curr(); it.next()) 
	  {
	    auto arc = it.get_current_arc();
	    if (IS_ARC_VISITED(arc, Aleph::Spanning_Tree))
	      continue;

	    auto tgt = it.get_tgt_node();
	    if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree)) 
	      continue; // causaría ciclo ==> no se mete en heap  

	    POT(arc) = acc + ARC_DIST(arc); // calcula potencial
	    heap.put_arc(arc, tgt);
	  }
      }

    uninit <Destroy_Tree_Node, Destroy_Tree_Arc> ();
  }

  bool paint_partial_min_paths_tree(const GT & g, 
				    typename GT::Node * start, 
				    typename GT::Node * end)
  {
    bool ret_val = false;
    init<Initialize_Node, Initialize_Arc> (g, start);

    NODE_BITS(start).set_bit(Aleph::Spanning_Tree, true); 
    ACC(start) = 0; 

    for (Itor<GT, SA> it(start, sa); it.has_curr(); it.next()) 
      {
	auto arc = it.get_current_arc();
	POT(arc) = ARC_DIST(arc);
	heap.put_arc(arc, it.get_tgt_node());
      }

    const auto & n = g.get_num_nodes();
    size_t tn = 1; // número de nodos pintados

    while (tn < n and not heap.is_empty()) // tree no abarque g
      {
	auto garc  = heap.get_min_arc(); 
	if (IS_ARC_VISITED(garc, Aleph::Spanning_Tree))
	  continue;

	auto src = g.get_src_node(garc);
	auto tgt = g.get_tgt_node(garc);

	// ¿Están los dos nodos visitados?
	if (IS_NODE_VISITED(src, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  continue; // este arco causaría un ciclo

	ARC_BITS(garc).set_bit(Aleph::Spanning_Tree, true);

	if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  std::swap(src, tgt);

	NODE_BITS(tgt).set_bit(Aleph::Spanning_Tree, true);
	PARENT(tgt) = src; 

	++tn; // simula que p se metió en el árbol abarcador

	if (tgt == end) // ¿está end_node en árbol abarcador? 
	  {
	    ret_val = true;
	    break; // sí ==> camino mínimo ya está pintado
	  }

	ACC(tgt) = ACC(src) + ARC_DIST(garc); // total dist nodo inicial

	const auto & acc = ACC(tgt);

	// por cada arco calcular potencial e insertarlo en heap
	for (Itor<GT, SA> it(tgt, sa); it.has_curr(); it.next()) 
	  {
	    auto a = it.get_current_arc();
	    if (IS_ARC_VISITED(a, Aleph::Spanning_Tree))
	      continue;

	    auto t = it.get_tgt_node();
	    if (IS_NODE_VISITED(t, Aleph::Spanning_Tree)) 
	      continue; // causaría ciclo ==> no se mete en heap  

	    POT(a) = acc + ARC_DIST(a); // calcula potencial
	    heap.put_arc(a, t);
	  }
      }

    uninit<Destroy_Node, Destroy_Arc> ();
    painted = true;
    
    return ret_val;
  }

  void paint_min_paths_tree(const GT & g, typename GT::Node * start)
  {
    init<Initialize_Node, Initialize_Arc> (g, start);

    NODE_BITS(start).set_bit(Aleph::Spanning_Tree, true); 
    ACC(start) = 0; 

    for (Itor<GT, SA> it(start, sa); it.has_curr(); it.next()) 
      {
	auto arc = it.get_current_arc();
	POT(arc) = ARC_DIST(arc);
	heap.put_arc(arc, it.get_tgt_node());
      }

    const auto & n = g.get_num_nodes();
    size_t tn = 1; // número de nodos pintados

    while (tn < n and not heap.is_empty()) // tree no abarque g
      {
	auto garc  = heap.get_min_arc(); 
	if (IS_ARC_VISITED(garc, Aleph::Spanning_Tree))
	  continue;

	auto src = g.get_src_node(garc);
	auto tgt = g.get_tgt_node(garc);

	// ¿Están los dos nodos visitados?
	if (IS_NODE_VISITED(src, Aleph::Spanning_Tree) and 
	    IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  continue; // este arco causaría un ciclo

	ARC_BITS(garc).set_bit(Aleph::Spanning_Tree, true);

	if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	  std::swap(src, tgt);

	NODE_BITS(tgt).set_bit(Aleph::Spanning_Tree, true);
	PARENT(tgt) = src; 

	++tn; // simula que p se metió en el árbol abarcador

	ACC(tgt) = ACC(src) + ARC_DIST(garc); // total dist nodo inicial
	const auto & acc = ACC(tgt);

	// por cada arco calcular potencial e insertarlo en heap
	for (Itor<GT, SA> it(tgt, sa); it.has_curr(); it.next()) 
	  {
	    auto a = it.get_current_arc();
	    if (IS_ARC_VISITED(a, Aleph::Spanning_Tree))
	      continue;

	    auto t = it.get_tgt_node();
	    if (IS_NODE_VISITED(t, Aleph::Spanning_Tree)) 
	      continue; // causaría ciclo ==> no se mete en heap  

	    POT(a) = acc + ARC_DIST(a); // calcula potencial
	    heap.put_arc(a, t);
	  }
      }

    uninit<Destroy_Node, Destroy_Arc> ();
    painted = true;
  }

      typename Distance::Distance_Type 
  get_min_path(typename GT::Node * end, Path<GT> & path)
  {
    if (ptr_g == nullptr)
      throw std::domain_error("Min path has not been computed");

    if (not painted)
      throw std::domain_error("Graph has not previously painted");
      
    return Aleph::get_min_path<GT, Distance>(s, end, path);
  }

      typename Distance::Distance_Type 
  find_min_path(const GT & g, 
		typename GT::Node * start, typename GT::Node * end, 
		Path<GT> & min_path)
  {
    min_path.empty();
    if (paint_partial_min_paths_tree(g, start, end))
      return get_min_path(end, min_path);

    return numeric_limits<typename Distance::Distance_Type>::max(); 
  }
  
  void operator () (const GT & g, typename GT::Node * s, GT & tree)
  {
    compute_min_paths_tree(g, s, tree);
  }

      typename Distance::Distance_Type 
  copy_painted_min_paths_tree(const GT & g, GT & tree)
  {
    if (not painted)
      throw std::domain_error("Graph has not previously painted");

    using Paint_Filt = Painted_Min_Spanning_Tree<GT, Distance>;
    Paint_Filt painted;
    (Copy_Graph<GT, Dft_Show_Node<GT>, Paint_Filt> (painted)) (tree, g);

    return painted.dist;
  }

  typename Distance::Distance_Type operator () (const GT & g, 
						typename GT::Node * s, 
						typename GT::Node * e,
						Path<GT> & path)
  {
    return find_min_path (g, s, e, path);
  }

  struct Total
  {
    typename  Distance::Distance_Type dist;
    
    Total() noexcept : dist(0) { /* empty */ }

    bool operator () (typename GT::Arc * a) noexcept
    {
      dist += ARC_DIST(a);
      return true;
    }
  };

      typename Distance::Distance_Type 
  get_min_path(const GT & tree, typename GT::Node * end, Path<GT> & path)
  {
    if (ptr_g == nullptr)
      throw std::domain_error("Min path has not been computed");

    auto ts = mapped_node<GT>(s);
    auto te = mapped_node<GT>(end);

    Path<GT> tree_path(tree);
    Total total;
    (Find_Path_Depth_First<GT, Itor, Total> (total)) (tree, ts, te, tree_path); 

    path.empty();
    path.init(s);
    typename Path<GT>::Iterator it(tree_path);
    for (it.next(); it.has_curr(); it.next())
      path.append(mapped_node<GT>(it.get_current_node()));

    return total.dist;
  }
};



# undef DNI
# undef PARENT
# undef TREENODE
# undef ACC
# undef HEAPNODE
# undef DAI
# undef ARC_DIST
# undef TREEARC
# undef POT
# undef GRAPHNODE
} // end namespace Aleph
# endif // DIJKSTRA_H
