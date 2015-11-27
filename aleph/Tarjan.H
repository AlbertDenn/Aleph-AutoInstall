
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

# ifndef TARJAN_H
# define TARJAN_H

# include <tpl_dynListStack.H>
# include <tpl_dynSetTree.H>
# include <htlist.H>
# include <tpl_graph_utils.H>
# include <tpl_find_path.H>

namespace Aleph {


     template <class GT, 
	       template <typename, class> class Itor = Out_Iterator,
	       class SA = Dft_Show_Arc<GT>> 
class Tarjan_Connected_Components
{
  SA & sa;

  GT * g_ptr = nullptr;

  DynListStack<typename GT::Node*> stack;

  long df_count;
  mutable size_t n; // número de nodos del grafo

      // lista de listas; cada lista almacena los nodos de los bloques
  DynList<DynList<typename GT::Node*>> * list_list_ptr; 

  DynList <GT> * block_list_ptr; // lista de bloques fuertemente
				 // conexos

  DynList<size_t> * list_len_ptr; // lista de tamaños de componentes

  Path<GT> * path_ptr;

public:

  Tarjan_Connected_Components(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Tarjan_Connected_Components(SA & __sa) 
    noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

private:

  struct Init_Tarjan_Node
  {
    void operator () (const GT & g, typename GT::Node * p) const noexcept
    {
      g.reset_bits(p);
      g.reset_counter(p); // inicializa df
      low <GT> (p) = -1;  // inicializa low
    }
  };
 
  bool is_node_in_stack(typename GT::Node * p) const noexcept
  {
    return IS_NODE_VISITED(p, Aleph::Min);
  }

  void init_node_and_push_in_stack(typename GT::Node * p)
  {
    assert(not is_node_in_stack(p));

    stack.push(p);
    NODE_BITS(p).set_bit(Aleph::Min, true);
    NODE_BITS(p).set_bit(Aleph::Depth_First, true);
    df<GT>(p) = low<GT>(p) = df_count++;
  }

  typename GT::Node * pop_from_stack() 
  {
    auto ret = stack.pop();
    NODE_BITS(ret).set_bit(Aleph::Min, false);

    return ret;
  }

  void scc_by_blocks(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

        // recorrer en profundidad todos los nodos conectados a v
    for (Itor<GT, SA> it(v, sa); it.has_curr(); it.next())
    {
      auto w = g_ptr->get_tgt_node(it.get_curr());
      if (not IS_NODE_VISITED(w, Aleph::Depth_First))
        {
          scc_by_blocks(w);
          low <GT> (v) = std::min(low <GT> (v), low <GT> (w));
        }
      else if (is_node_in_stack(w))
        // si está en pila ==> v fue visitado antes que p
        low <GT> (v) = std::min(low <GT> (v), df <GT> (w));
    }

    if (low <GT> (v) == df <GT> (v)) // ¿primer nodo visitado del bloque?
      {	    // sí ==> saque los nodos del bloque que están en pila
	const auto & blk_idx = block_list_ptr->size();
	GT & blk = block_list_ptr->append(GT());

	while (true) // sacar el bloque de la pila hasta sacar a v
	  {
	    auto p = pop_from_stack();
	    auto q = blk.insert_node(p->get_info());
	    *q = *q; // copia contenido del nodo
	    NODE_COOKIE(p) = NODE_COOKIE(q) = nullptr;
	    GT::map_nodes(p, q);
	    NODE_COUNTER(p) = NODE_COUNTER(q) = blk_idx;
	    if (p == v)
	      break;
	  }
      }
  } 

  void scc_by_lists(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

        // recorrer en profundidad todos los nodos conectados a v
    for (Itor<GT, SA> it(v, sa); it.has_curr(); it.next())
    { 
      auto w = g_ptr->get_tgt_node(it.get_curr());
      if (not IS_NODE_VISITED(w, Aleph::Depth_First))
        {
          scc_by_lists(w);
          low <GT> (v) = std::min(low <GT> (v), low <GT> (w));
        }
      else if (is_node_in_stack(w))
        // si está en pila ==> v fue visitado antes que p
        low <GT> (v) = std::min(low <GT> (v), df <GT> (w));
    }

    if (low<GT>(v) == df<GT>(v)) // ¿primer nodo visitado del bloque?
      {     // sí ==> saque los nodos del bloque que están en pila
	auto & l = list_list_ptr->append(DynList<typename GT::Node*>());
	while (true) // sacar bloque de pila hasta llegar a v
	  {
	    auto p = pop_from_stack();
	    l.append(p);
	    if (p == v)
	      break;
	  }
      }
  }

  void scc_by_len(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

        // recorrer en profundidad todos los nodos conectados a v
    for (Itor<GT, SA> it(v, sa); it.has_curr(); it.next())
    { 
      auto w = g_ptr->get_tgt_node(it.get_curr());
      if (not IS_NODE_VISITED(w, Aleph::Depth_First))
        {
          scc_by_len(w);
          low <GT> (v) = std::min(low <GT> (v), low <GT> (w));
        }
      else if (is_node_in_stack(w))
        // si está en pila ==> v fue visitado antes que p
        low <GT> (v) = std::min(low <GT> (v), df <GT> (w));
    }

    if (low<GT>(v) == df<GT>(v)) // ¿primer nodo visitado del bloque?
      {     // sí ==> saque los nodos del bloque que están en pila
	size_t count = 0;
	while (true) // sacar bloque de pila hasta llegar a v
	  {
	    auto p = pop_from_stack();
	    ++count;

	    if (p == v)
	      break;
	  }
	list_len_ptr->append(count);
      }
  }

  void init_tarjan(const GT & g)
  {
    Operate_On_Nodes <GT, Init_Tarjan_Node> () (g); // inicia bits, df y low
    df_count = 0; // contador de visitas
    stack.empty();
    n = g.get_num_nodes();
    g_ptr = &const_cast<GT&>(g);
  }

      // retorna true si se encuentra un ciclo desde v
  bool has_cycle(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

      // recorrer en profundidad todos los nodos conectados a v
    for (Itor <GT, SA> it(v, sa); it.has_curr(); it.next())
    {
      auto w = g_ptr->get_tgt_node(it.get_curr());
      if (not IS_NODE_VISITED(w, Aleph::Depth_First))
        {
          if (has_cycle(w))
            return true;

          low<GT>(v) = std::min(low<GT>(v), low<GT>(w));
        }
      else if (is_node_in_stack(w))
            // si está en pila ==> v fue visitado antes que p
        low<GT>(v) = std::min(low<GT>(v), df<GT>(w));
    }

    if (low<GT>(v) == df<GT>(v)) // ¿primer nodo visitado de bloque?
      {     // sí ==> verifique si tiene dos o más nodos
	int i = 0;
	for (; i < 2; ++i) // sacar bloque de la pila hasta sacar a v
	  if (pop_from_stack() == v)
	    break;
      
	return i >= 2; // si i>= 2 ==> hay un ciclo
      }  

    return false; // se recorrió todo v sin encontrar ciclo
  }

  // Toma el bloque block, que está mapeado del grafo original, que es
  // fuertemente conexo, y construye en path_ptr el ciclo
      void 
  build_path(const GT &                                              block, 
	     DynMapAvlTree<typename GT::Node*, typename GT::Node*> & table)
  {
          // buscar ciclo en blk.
    auto a     = block.get_first_arc();
    auto start = block.get_tgt_node(a);
    auto end   = block.get_src_node(a);
    assert(start != end);

    auto aux_path = Directed_Find_Path<GT, Itor, SA>(block, sa).dfs(start, end);
    assert(not aux_path.is_empty()); // como es conexo debe encontrarse

    // aux_path es sobre el bloque mapeado. Pero nosotros necesitamos
    // que el camino sea sobre el grafo original. Los nodos del grafo
    // original se recuperan del mapeo table
    path_ptr->empty();
    for (typename Path<GT>::Iterator i(aux_path); i.has_curr(); i.next())
      path_ptr->append_directed(table.find(i.get_current_node()));
    
    path_ptr->append_directed(path_ptr->get_first_node());
  }

      // retorna true si encuentra un ciclo, en cuyo caso lo pone en
      // path. De lo contrario, retorna false y path queda intacto
  bool build_cycle(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

    // recorrer en profundidad todos los nodos conectados a v
    for (Itor <GT, SA> it(v, sa); it.has_curr(); it.next())
      {
	auto w = g_ptr->get_tgt_node(it.get_curr());
	if (not IS_NODE_VISITED(w, Aleph::Depth_First))
	  {
	    if (build_cycle(w))
	      return true;

	    low<GT>(v) = std::min(low<GT>(v), low<GT>(w));
	  }
	else if (is_node_in_stack(w))
	  // si está en pila ==> v fue visitado antes que p
	  low<GT>(v) = std::min(low<GT>(v), df<GT>(w));
      }

    if (low<GT>(v) == df<GT>(v)) // ¿primer nodo visitado del bloque?
      {
	GT blk; // grafo auxiliar

	// mapeo de nodos de g con blk (los cookies están ocupados)
	DynMapAvlTree<typename GT::Node*, typename GT::Node*> table; 
	
	    // saque nodos de pila e insertelos en bloque auxiliar
	while (true) // saca el componente y lo inserta en blk
	  {
	    auto p = pop_from_stack();
	    auto q = blk.insert_node();
	    *q = *p; // copia contenido del nodo
	    table.insert(q, p);
	    table.insert(p, q);
	    if (p == v)
	      break;
	  }

	if (blk.get_num_nodes() == 1) 
	  return false; // si blk sólo tiene un nodo no hay ciclo

    	    // terminanos de construir el bloque con los arcos
	for (typename GT::Node_Iterator j(blk); j.has_curr(); j.next())
	  {
	    auto bsrc = j.get_curr(); 
	    auto gsrc = table.find(bsrc); 

    	        // recorrer los arcos de gsrc
	    for (Itor <GT, SA> k(gsrc, sa); k.has_curr(); k.next())
	      {
		auto ga   = k.get_curr();
		auto gtgt = g_ptr->get_tgt_node(ga);
		auto ptr  = table.search(gtgt);
		if (ptr == nullptr) // ¿arco del bloque?
		  continue;

		auto ta = blk.insert_arc(bsrc, ptr->second);
		*ta = *ga; // copia contenido del arco
	      }
	  }

	build_path(blk, table);

	return true;
      }  

    return false;
  }

  bool is_connected(typename GT::Node * v)
  {
    init_node_and_push_in_stack(v);

        // recorrer en profundidad todos los nodos conectados a v
    for (Itor <GT, SA> it(v, sa); it.has_curr(); it.next())
      {
	auto w = g_ptr->get_tgt_node(it.get_curr());
	if (not IS_NODE_VISITED(w, Aleph::Depth_First))
	  {
	    if (not is_connected(w))
	      return false;

	    low <GT> (v) = std::min(low <GT> (v), low <GT> (w));
	  }
	else if (is_node_in_stack(w))
	  low <GT> (v) = std::min(low <GT> (v), df <GT> (w));
      }

    if (low<GT>(v) == df<GT>(v)) // ¿primer nodo visitado de bloque?
      {     // saque nodos de pila hasta encontrar a v
	while (pop_from_stack() != v); 

	return stack.is_empty();
      }  

    return true;
  }

public:

  void connected_components(const GT & g, DynList<GT> & blk_list,
			    DynList<typename GT::Arc*> & arc_list)
  {
    init_tarjan(g);

    block_list_ptr = &blk_list;

    for (typename GT::Node_Iterator it(g); df_count < n; it.next())
      {
	auto v = it.get_curr();
	if (not IS_NODE_VISITED(v, Aleph::Depth_First)) 
	  scc_by_blocks(v); 
      }

    assert(stack.is_empty());

        // recorrer cada uno de subgrafos parciales y añadir sus arcos
    for (typename DynList<GT>::Iterator i(blk_list); i.has_curr(); i.next())
      {     // recorrer todos los nodos del bloque
	GT & blk = i.get_curr();
	for (typename GT::Node_Iterator j(blk); j.has_curr(); j.next())
	  {
	    auto bsrc = j.get_curr();
	    auto gsrc = mapped_node<GT>(bsrc);

              // recorrer arcos de gsrc
	    for (Itor<GT, SA> k(gsrc, sa); k.has_curr(); k.next())
	      {
		auto ga   = k.get_curr();
		auto gtgt = g_ptr->get_tgt_node(ga);
		if (NODE_COUNTER(gsrc) != NODE_COUNTER(gtgt)) 
		  {   // arco inter-bloque ==> añádalo a arc_list
		    arc_list.append(ga);
		    continue;
		  }

    		    // insertar y mapear el arco en el sub-bloque
		auto btgt = mapped_node<GT>(gtgt);
		auto ba = blk.insert_arc(bsrc, btgt);
		*ba = *ga; // copia contenido del arco
		GT::map_arcs(ga, ba);
	      }
	  }
      }
  }

  void connected_components(const GT & g, 
			    DynList<DynList<typename GT::Node*>> & blks)
  {
    init_tarjan(g);
    list_list_ptr = &blks;
    for (typename GT::Node_Iterator it(g); df_count < n; it.next())
      {
	auto v = it.get_curr();
	if (not IS_NODE_VISITED(v, Aleph::Depth_First))
	  scc_by_lists(v); 
      }
  }

  DynList<DynList<typename GT::Node*>> connected_components(const GT & g)
  {
    DynList<DynList<typename GT::Node*>> blks;
    connected_components(g, blks);
    return blks;
  }

  void connected_components(const GT & g, DynList<size_t> & blks)
  {
    init_tarjan(g);
    list_len_ptr = &blks;
    for (typename GT::Node_Iterator it(g); df_count < n; it.next())
      {
	auto v = it.get_curr();
	if (not IS_NODE_VISITED(v, Aleph::Depth_First))
	  scc_by_len(v); 
      }
  }

  void operator () (const GT &                   g, 
                    DynList <GT> &               blk_list,
                    DynList<typename GT::Arc*> & arc_list)
  {
    connected_components(g, blk_list, arc_list);
  }

  void operator () (const GT &                             g, 
                    DynList<DynList<typename GT::Node*>> & blks)
  {
    connected_components(g, blks);
  }

  DynDlist<DynDlist<typename GT::Node*>> operator () (const GT & g) 
  {
    return connected_components(g);
  }

  void operator () (const GT &                    g, 
                    DynDlist <GT> &               blk_list,
                    DynDlist<typename GT::Arc*> & arc_list)
  {
    DynList<GT> blist;
    DynList<typename GT::Arc*> alist;
    connected_components(g, blist, alist);

    for (typename DynList<GT>::Iterator it(blist); it.has_curr(); it.next())
      {
	GT & curr = it.get_curr();
	GT & block = blk_list.append(GT());
	curr.swap(block);
      }

    for (typename DynList<typename GT::Arc*>::Iterator it(alist); 
	 it.has_curr(); it.next())
      arc_list.append(it.get_curr());
  }

  void operator () (const GT &                               g, 
                    DynDlist<DynDlist<typename GT::Node*>> & blks)
  {
    DynList<DynList<typename GT::Node*>> b;
    connected_components(g, b);
      
    for (typename DynList<DynList<typename GT::Node*>>::Iterator it(b);
         it.has_curr(); it.next())
      {
	auto & tgt_list = blks.append(DynDlist<typename GT::Node*>());

	auto & blk = it.get_curr();
	while (not blk.is_empty())
	  tgt_list.append(blk.remove_first());
      }   
  }

  bool has_cycle(const GT & g)
  {
    init_tarjan(g);
    for (typename GT::Node_Iterator it(g); df_count < n; it.next())
    {
      auto v = it.get_curr();
      if (not IS_NODE_VISITED(v, Aleph::Depth_First)) 
        if (has_cycle(v)) 
          return true;
    }
    
    return false; 
  }

  bool is_dag(const GT & g)
  {
    return not has_cycle(g);
  }

  bool compute_cycle(const GT & g, Path<GT> & path)
  {
    init_tarjan(g);
    path_ptr = &path;
    path_ptr->set_graph(g);

    for (Node_Iterator<GT> it(g); df_count < n; it.next())
      {
	auto v = it.get_curr();
	if (not IS_NODE_VISITED(v, Aleph::Depth_First)) // ¿p visitado?
	  if (build_cycle(v)) 
	    return true;
      }

    return false; 
  }

  bool test_connectivity(const GT & g)
  {
    init_tarjan(g);
    for (typename GT::Node_Iterator it(g); df_count < n; it.next())
      {
	auto v = it.get_curr();
	if (not IS_NODE_VISITED(v, Aleph::Depth_First)) 
	  if (not is_connected(v))
	    return false;
      }

    assert(stack.is_empty());

    return true;
  }
};



     template <class GT, 
	       template <typename, class> class Itor = Node_Arc_Iterator,
	       class SA = Dft_Show_Arc<GT>> 
class Compute_Cycle_In_Digraph
{
  SA & sa;

public:

  Compute_Cycle_In_Digraph(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Compute_Cycle_In_Digraph(SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  bool operator () (GT & g, Path<GT> & path) const
  {
    Tarjan_Connected_Components<GT, Itor, SA> tarjan(sa);

    return tarjan.compute_cycle(g, path);
  }
};

} // end namespace Aleph

# endif // TARJAN_H
