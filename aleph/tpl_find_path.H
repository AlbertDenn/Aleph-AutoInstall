
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


# ifndef TPL_FIND_PATH_H
# define TPL_FIND_PATH_H

# include <tpl_dynListStack.H>
# include <tpl_dynListQueue.H>
# include <tpl_graph_utils.H>

namespace Aleph {

    template <class GT, 
	      template <class, class> class Itor = Node_Arc_Iterator,
	      class SA = Dft_Show_Arc<GT>> 
class Find_Path_Depth_First
{
  SA & sa;
  typename GT::Node * end = nullptr;
  GT * g_ptr = nullptr;
  Path<GT> * path = nullptr;
  
  bool find_path(typename GT::Node * curr, typename GT::Arc * arc)
  {
    if (curr == end) // ¿se alcanzó nodo final?
      {     // sí, terminar añadir el arco y terminar
	path->append(arc);

	return true;
      }

    if (IS_NODE_VISITED(curr, Find_Path)) // ¿ha sido visitado?
      return false; // sí ==> desde él no hay camino

    path->append(arc); // añadir curr_arc al camino
    NODE_BITS(curr).set_bit(Find_Path, true); 

      // buscar recursivamente a través de arcos de curr_node
    for (Itor<GT, SA> i(curr, sa); i.has_curr(); i.next())
      {
	auto next_arc = i.get_curr();
	if (IS_ARC_VISITED(next_arc, Find_Path)) 
	  continue;

	ARC_BITS(next_arc).set_bit(Find_Path, true); 
	auto next_node = g_ptr->get_connected_node(next_arc, curr);
	if (find_path(next_node, next_arc))
	  {
	    assert(path->get_last_node() == end);	    
	    return true; // se encontró camino
	  }
      }

    path->remove_last_node(); 

    return false;
  }

  bool find_path(const GT & g, typename GT::Node * start, 
		 typename GT::Node * end_node, Path<GT> & p)
  {
    if (not p.inside_graph(g))
      throw std::invalid_argument("Path does not belong to graph");

    end  = end_node;
    path = &p;
    g_ptr = &const_cast<GT&>(g);

    path->empty();         
    path->init(start); 

    g.reset_bit_nodes(Find_Path);
    g.reset_bit_arcs(Find_Path); 

    NODE_BITS(start).set_bit(Find_Path, true);

      // explorar recursivamente cada arco de start_node
    for (Itor<GT, SA> i(start, sa); i.has_curr(); i.next())
    {
      auto arc = i.get_curr();
      ARC_BITS(arc).set_bit(Find_Path, true); 

      auto next_node = g.get_connected_node(arc, start);
      if (IS_NODE_VISITED(next_node, Find_Path)) 
        continue; 

      if (find_path(next_node, arc)) 
	{
	  assert(p.get_first_node() == start and p.get_last_node() == end_node);
	  return true;
	}
    }

    return false;
  }

public:

  Find_Path_Depth_First(SA && __sa = SA()) 
  noexcept(std::is_nothrow_constructible<SA>::value and
	   std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Find_Path_Depth_First(SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  bool operator () (const GT & g, typename GT::Node * start_node, 
                    typename GT::Node * end_node, Path<GT> & path) 
  {
    return find_path(g, start_node, end_node, path);
  }
};


      template <class GT, 
		template <typename, class> class Itor = Node_Arc_Iterator,
		class SA = Dft_Show_Arc<GT> > 
class Find_Path_Breadth_First
{
  SA & sa;

  bool find_path(const GT & g, typename GT::Node * start, 
		 typename GT::Node * end, Path<GT> & path)
  {
    if (not path.inside_graph(g))
      throw std::invalid_argument("Path does not belong to graph");

    path.empty(); // limpiamos cualquier cosa que esté en path
    g.reset_nodes();
    g.reset_arcs(); 
  
    DynListQueue<typename GT::Arc*> q; 

    for (Itor<GT, SA> i(start, sa); i.has_curr(); i.next())
      q.put(i.get_current_arc());
        
    NODE_BITS(start).set_bit(Find_Path, true); // márquelo visitado

    bool path_found = false;

    while (not q.is_empty()) // mientras queden arcos por visitar
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

	if (tgt == end) // ¿se encontró un camino?
	  { 
	    path_found = true;
	    break;
	  }

	for (Itor<GT,SA> i(tgt); i.has_curr(); i.next())
	  {
	    auto a = i.get_current_arc();
	    if (IS_ARC_VISITED(a, Find_Path)) // ¿arco visitado?
	      continue; // sí ==> avanzar al siguiente 

	    // revise nodos del arco para ver si han sido visitados
	    if (IS_NODE_VISITED(g.get_src_node(a), Find_Path) and 
		IS_NODE_VISITED(g.get_tgt_node(a), Find_Path))
	      continue; // nodos ya visitados ==> no  meter arco

	    q.put(a);
	  }
      }

    if (not path_found)
      return false;

    q.empty();
    path.insert(end);
    auto p = end;
    while (p != start)
      {
	p = (typename GT::Node *) NODE_COOKIE(p);
	path.insert(p);
      }

    return true;
  }

public:

  Find_Path_Breadth_First(SA & _sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(_sa) { /* Empty */ }

  Find_Path_Breadth_First(SA && _sa = SA()) 
  noexcept(std::is_nothrow_constructible<SA>::value and
	   std::is_nothrow_copy_assignable<SA>::value)
    : sa(_sa) { /* Empty */ }
 
  bool operator () (const GT & g, typename GT::Node * start, 
		    typename GT::Node * end, Path<GT> & path) 
  {
    return find_path(g, start, end, path);
  }
};

  template <class GT,
	    template <typename, class> class Itor = Out_Iterator,
	    class SA = Dft_Show_Arc<GT>>
class Directed_Find_Path
{
  const GT & g;
  SA & sa;

  template <template <typename T> class Q>
  Path<GT> find(typename GT::Node * start, typename GT::Node * end)
  {
    g.reset_nodes();
    g.reset_arcs();

    start->set_state(Processed);

    Q<typename GT::Arc*> q;
    for (Itor<GT, SA> it(start, sa); it.has_curr(); it.next())
      {
	auto a = it.get_curr();
	it.get_node(a)->set_state(Processing);
	a->set_state(Processing);
	q.put(a);
      }

    bool path_found = false;
    typename GT::Node * curr = nullptr;
    while (not q.is_empty())
      {
	auto arc = q.get();
	assert(arc->state() == Processing);
	arc->set_state(Processed);

	curr = g.get_tgt_node(arc);
	if (curr->state() == Processed)
	  continue;

	curr->set_state(Processed);
	NODE_COOKIE(curr) = g.get_src_node(arc);

	if (curr == end)
	  {
	    path_found = true;
	    break;
	  }

	for (Out_Iterator<GT, SA> it(curr, sa); it.has_curr(); it.next()) 
	  {
	    auto a  = it.get_curr();
	    a->set_state(Processing);

	    auto tgt = it.get_node(a);
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

    Path<GT> ret(g);
    if (not path_found)
      return ret;

    assert(curr == end);

    while (curr != start)
      {
	ret.insert(curr);
	curr = (typename GT::Node*) NODE_COOKIE(curr);
      }
    ret.insert(start);

    return ret;
  }

public:

  Directed_Find_Path(const GT & __g, SA & __sa) : g(__g), sa(__sa) {}

  Directed_Find_Path(const GT & __g, SA && __sa = SA()) : g(__g), sa(__sa) {}

  Path<GT> dfs(typename GT::Node * start, typename GT::Node * end) 
  {
    return find<DynListStack>(start, end);
  }

  Path<GT> bfs(typename GT::Node * start, typename GT::Node * end) 
  {
    return find<DynListQueue>(start, end);
  }
};




} // namespace Aleph

# endif // TPL_FIND_PATH_H
