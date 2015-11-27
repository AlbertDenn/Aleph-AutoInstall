
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

# ifndef TOPOLOGICAL_SORT_H
# define TOPOLOGICAL_SORT_H

# include <tpl_dynListQueue.H>
# include <tpl_graph.H>

namespace Aleph {


    template <class GT, 
	      template <typename, class> class Itor = Out_Iterator,
	      class SA = Dft_Show_Arc<GT>> 
class Topological_Sort
{
  SA & sa;
  const GT * gptr;

public:

  Topological_Sort(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa), gptr(nullptr) { /* empty */ }

  Topological_Sort(SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa), gptr(nullptr)  { /* empty */ }

private:
 
      template <template <class> class List>
  void topological_sort(typename GT::Node * curr, 
			List<typename GT::Node*> & list)
  {
    assert(gptr != nullptr);

    if (IS_NODE_VISITED(curr, Depth_First))
      return;

    NODE_BITS(curr).set_bit(Depth_First, 1); // marcarlo como visitado

    const auto & n = gptr->get_num_nodes();

      // visitar recursivamente en sufijo los nodos adyacentes a curr
    for (Itor<GT,SA> it(curr,sa); it.has_curr() and list.size() < n; it.next()) 
      topological_sort(it.get_tgt_node(), list);

    list.insert(curr); // inserción sufija de nodo que devino sumidero
  }

public:

  template <template <class> class List>
  List<typename GT::Node*> perform (const GT & g)
  {
    g.reset_bit_nodes(Depth_First); // iniciar bit de marca de visita en nodos

    gptr = &g;
    List<typename GT::Node*> list;

        // recorrer todos los nodos
    const auto & n = gptr->get_num_nodes();
    for (auto it = g.get_node_it(); it.has_curr() and list.size() < n; 
	 it.next())
    {
      auto curr = it.get_current_node();
      if (not IS_NODE_VISITED(curr, Depth_First))
        topological_sort(curr, list);
    }
    
    return list;
  }

  void operator () (const GT & g, DynDlist<typename GT::Node*> & list) 
  {
    perform<DynDlist>(g).swap(list);
  }
};

   template <class GT, 
	     template <typename, class> class Itor = Out_Iterator,
	     class SA = Dft_Show_Arc<GT>> 
class Q_Topological_Sort
{
  SA & sa;

public:

  Q_Topological_Sort(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Q_Topological_Sort(SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

      template <template <class> class List>
  List<typename GT::Node*> perform (const GT & g)
  {
    g.reset_counter_nodes();

    List<typename GT::Node*> list;

        // recorra todos los arcos y contar grados de entrada
    for (Arc_Iterator<GT,SA> it(g, sa); it.has_curr(); it.next()) 
      NODE_COUNTER(it.get_tgt_node())++; 

      // buscar nodos con grado de entrada 0 y meterlos en cola
    DynListQueue<typename GT::Node*> q; // cola de fuentes
    for (auto it = g.get_node_it(); it.has_curr(); it.next())
      {
	auto p = it.get_current_node();
	if (NODE_COUNTER(p) == 0) // ¿es un nodo fuente?
	  q.put(p); // sí ==> colocarlo en la cola
      }

    while (not q.is_empty()) 
      {
	auto p = q.get(); // saque último fuente

	assert(NODE_COUNTER(p) == 0);

	list.append(p); // insértelo en el orden topológico

          // decrementar grado de entrada de cada nodo adyacente a p
	for (Itor<GT, SA> it(p, sa); it.has_curr(); it.next())
	  {
	    auto tgt = it.get_tgt_node();
	    if (--NODE_COUNTER(tgt) == 0) // ¿tgt deviene fuente?
	      q.put(tgt); // sí ==> colóquelo en la cola
	  }
      }

    return list;
  }

  template <template <class> class RankList = DynList, 
	    template <class> class List = DynList>
  RankList<List<typename GT::Node*>> ranks(const GT & g)
  {
    g.reset_counter_nodes();

        // recorra todos los nodos para contabilizar grado de entrada
    for (typename GT::Node_Iterator i(g); i.has_curr(); i.next()) 
      for (Itor<GT, SA> j(i.get_current_node(), sa); j.has_curr(); j.next()) 
	NODE_COUNTER(j.get_tgt_node())++;

        // revisar nodos con grado de entrada 0 y meterlos en cola
    DynListQueue<typename GT::Node*> q; // cola de fuentes
    for (typename GT::Node_Iterator it(g); it.has_curr(); it.next())
      {
	auto p = it.get_current_node();
	if (NODE_COUNTER(p) == 0) // ¿es un nodo fuente?
	  q.put(p); // sí ==> colocarlo en la cola
      }
    
    RankList<List<typename GT::Node*>> ranks; // valor de retorno
    while (not q.is_empty()) 
      {
	List<typename GT::Node*> rank;
	DynListQueue<typename GT::Node*> aq;

	while (not q.is_empty()) // saca todos los nodos del nivel i
	  {
	    auto p = q.get(); // saque último fuente
	    rank.append(p); // insértelo en el rango topológico

                // decrementar grado entrada de cada nodo adyacente a p
	    for (Itor<GT, SA> it(p, sa); it.has_curr(); it.next())
	      {
		auto tgt = it.get_tgt_node();
		if (--NODE_COUNTER(tgt) == 0) // ¿tgt deviene fuente?
		  aq.put(tgt); // sí ==> colóquelo en cola auxiliar
	      }
	  }

	ranks.append(std::move(rank));
	q.swap(aq);
      }

    return ranks; 
  }

  void operator () (const GT & g, DynDlist<DynList<typename GT::Node *>> & list)
  {
    this->ranks<>(g).swap(list);
  }

  void operator () (const GT & g, DynList<DynList<typename GT::Node *>> & list)
  {
    this->ranks<DynList>(g).swap(list);
  }

  void operator () (const GT & g, DynDlist<typename GT::Node*> & list) 
  {
    this->perform<DynDlist>(g).swap(list);
  }
};

} // end namespace Aleph

# endif // TOPOLOGICAL_SORT_H
