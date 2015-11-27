
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

# ifndef TPL_CUT_NODES_H
# define TPL_CUT_NODES_H

# include <tpl_graph_utils.H>

namespace Aleph {


      template <class GT, class SA = Dft_Show_Arc<GT> >
class Compute_Cut_Nodes
{
  SA &                            sa;
  GT *                            gptr;
  DynDlist<typename GT::Node *> * list_ptr;
  long                            curr_df;
  long                            curr_color;

  enum State { Init, Cut_Nodes_Computed, Painted } state;

  void cut_nodes(typename GT::Node * p, typename GT::Arc * a)
  {
    NODE_BITS(p).set_bit(Depth_First, true); // pinte p visitado
    low <GT> (p) = df <GT> (p) = curr_df++;  // asígnele df

        // recorrer arcos de p mientras no se abarque a g
    bool p_is_cut_node = false;
    for (Node_Arc_Iterator <GT, SA> i(p, sa); i.has_curr(); i.next())
      {
        auto arc = i.get_current_arc();
        if (arc == a) 
          continue; // a es el padre de arc ==> ignorarlo

        auto tgt = i.get_tgt_node();
        if (IS_NODE_VISITED(tgt, Depth_First)) 
          { 
            if (not IS_ARC_VISITED(arc, Depth_First)) // no abarcador?
	      low<GT>(p) = std::min(df<GT>(tgt), low<GT>(p));
            continue;
          }

        if (IS_ARC_VISITED(arc, Depth_First)) 
          continue;

        ARC_BITS(arc).set_bit(Depth_First, true); // marque arco

        cut_nodes(tgt, arc);
	low<GT>(p) = std::min(low<GT>(tgt), low<GT>(p));
        if (low<GT>(tgt) >= df<GT>(p) and df<GT>(tgt) != 0) // ¿de corte?
          p_is_cut_node = true;
      }

        // aquí, p ya fue explorado recursivamente
    if (p_is_cut_node)
      {
        NODE_BITS(p).set_bit(Cut, true);
        list_ptr->append(p);
      }
  }

public:

  void cut_nodes(typename GT::Node *             start, 
		 DynDlist<typename GT::Node *> & list)
  {
    curr_df  = 0; // contador global de visitas 
    list_ptr = &list;

    list_ptr->empty();

    gptr->for_each_node([] (auto p) // init the nodes
			{
			  NODE_COUNTER(p) = 0;
			  NODE_BITS(p).reset();
			  low<GT>(p) = -1;
			});
    gptr->reset_arcs();

    NODE_BITS(start).set_bit(Depth_First, true); // marcar start
    df <GT> (start) = curr_df++;

    int call_counter = 0; // contador de llamadas recursivas

        // Recorra los arcos de start mientras g no haya sido abarcado
    for (Node_Arc_Iterator<GT, SA> it(start, sa); 
         it.has_curr() and curr_df < gptr->get_num_nodes(); it.next())
      {
	auto tgt = it.get_tgt_node();
	if (IS_NODE_VISITED(tgt, Depth_First)) 
	  continue; 

	auto arc = it.get_current_arc();
	if (IS_ARC_VISITED(arc, Depth_First)) 
	  continue;

	ARC_BITS(arc).set_bit(Depth_First, true);
	cut_nodes(tgt, arc); 
	++call_counter;
      }  

    if (call_counter > 1) // ¿es la raíz un punto de articulación?
      {    // sí == métala en la lista
        NODE_BITS(start).set_bit(Cut, true);
        list_ptr->append(start);
      }

    state = Cut_Nodes_Computed;
  }

private:

  void paint_subgraph(typename GT::Node * p)
  {
    assert(not is_a_cut_node <GT> (p));

    if (is_node_painted <GT> (p)) 
      return; 

    paint_node <GT> (p, curr_color);

    for (Node_Arc_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
      {
	auto arc = it.get_current_arc();
	if (is_arc_painted <GT> (arc))
          continue;

	auto tgt = it.get_tgt_node();
	if (is_a_cut_node <GT> (tgt))
	  continue;

	paint_arc<GT>(arc, curr_color); 
	paint_subgraph(tgt); 
      }
  }

  void paint_from_cut_node(typename GT::Node * p)
  {
    assert(is_a_cut_node <GT> (p));

        // pintar recursivamente con dif colores bloques conectados a p
    for (Node_Arc_Iterator<GT, SA> it(p, sa); it.has_curr(); it.next())
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
	paint_subgraph(tgt_node);

	curr_color++; // cambiar color (sig arco en otro bloque)

	assert(not is_arc_painted <GT> (arc));
      }
  }

  typename GT::Node * create_and_map_node(typename GT::Node * gp, 
					  const long &        color,
					  GT &                sg)
  {
    assert(get_color<GT>(gp) == color);
    assert(not IS_NODE_VISITED(gp, Build_Subtree));

    unique_ptr<typename GT::Node> tp_auto(new typename GT::Node(gp));
    sg.insert_node(tp_auto.get());
    GT::map_nodes(gp, tp_auto.get());
    NODE_BITS(gp).set_bit(Build_Subtree, true); 

    return tp_auto.release();
  }

  void map_subgraph(GT & sg, typename GT::Node * gsrc, const long & color)
  {
    assert(get_color <GT> (gsrc) == color);

    auto tsrc = mapped_node<GT>(gsrc); // gsrc en sg

        // recorrer arcos de gsrc y añadir a sg los del color de interés
    for (Node_Arc_Iterator <GT, SA> i(gsrc, sa); i.has_curr(); i.next())
      {
        auto garc = i.get_current_arc();
        if (get_color<GT>(garc) != color or IS_ARC_VISITED(garc, Build_Subtree))
          continue; // arco es de otro color o ya está visitado 

        ARC_BITS(garc).set_bit(Build_Subtree, true); 

        auto gtgt = i.get_tgt_node(); 

        assert(get_color <GT> (gtgt) == color);

        typename GT::Node * ttgt = nullptr; // imagen gtgt en sg
        if (IS_NODE_VISITED(gtgt, Build_Subtree)) // ¿gtgt en sg?
          ttgt = mapped_node<GT> (gtgt);
        else
	  ttgt = create_and_map_node(gtgt, color, sg);

        auto tarc = sg.insert_arc(tsrc, ttgt, garc->get_info());
        GT::map_arcs(garc, tarc);

        map_subgraph(sg, gtgt, color);
      }
  }
  
public:

  Compute_Cut_Nodes(const GT & g, SA && __sa = SA()) 
    : sa(__sa), gptr(&const_cast<GT&>(g)), state(Init)
  {
    /* empty */ 
  }

  Compute_Cut_Nodes(const GT & g, SA & __sa) 
    : sa(__sa), gptr(&const_cast<GT&>(g))
  {
    /* empty */ 
  }

  void operator () (DynDlist<typename GT::Node *> & list)
  {
    cut_nodes(gptr->get_first_node(), list);
  }

  void operator () (typename GT::Node *             start,
		    DynDlist<typename GT::Node *> & list)
  {
    cut_nodes(start, list);
  }
  
  long paint_subgraphs()
  {
    if (state != Cut_Nodes_Computed)
      throw std::logic_error("Cut nodos has not been computed or the class is"
			     " another phase");

    gptr->reset_counter_nodes();
    gptr->reset_counter_arcs();
    curr_color = 1;

        // Recorrer cada nodo de corte y pintar sus bloques
    for (typename DynDlist<typename GT::Node*>::Iterator i(*list_ptr); 
	 i.has_curr(); i.next())
      paint_from_cut_node(i.get_curr());

    state = Painted;

    return curr_color;
  }

  void map_subgraph(GT & sg, const long & color)
  {
    if (state != Painted)
      throw std::logic_error("Graph is not painted");

    clear_graph(sg);

    typename GT::Node * first = nullptr; // busque primer nodo con color

    for (typename GT::Node_Iterator it(*gptr); it.has_current(); it.next())
      if (get_color <GT> (it.get_current_node()) == color)
        first = it.get_current_node();

    if (first == nullptr) // Encontró el color?
      throw std::domain_error("Color does not exist in the graph");

        // cree first, insértelo en sg y mapéelo
    create_and_map_node(first, color, sg);
    try
      {    
	map_subgraph (sg, first, color); // mapee first
      }
    catch (...)
      {
        clear_graph(sg);
      }
  }

  void map_cut_graph(GT &                          cut_graph, 
		     DynDlist<typename GT::Arc*> & cross_arc_list)
  {
    if (state != Painted)
      throw std::logic_error("Graph is not painted");

    clear_graph(cut_graph);

        // recorra lista de nodos de corte e insértelos en cut_graph
    for (typename DynDlist<typename GT::Node*>::Iterator it(*list_ptr);
	 it.has_curr(); it.next())
      {
	auto gp = it.get_current();

	assert(is_a_cut_node <GT> (gp));

	unique_ptr<typename GT::Node> tp_auto(new typename GT::Node(gp));
	cut_graph.insert_node(tp_auto.get());
	GT::map_nodes(gp, tp_auto.release());
      }

        // recorra arcos de g ==> cut_graph = {arcos no corte} U 
        // cross_arc_list = {arcos de cruce}
    for (Arc_Iterator <GT, SA> it(*gptr, sa); it.has_curr(); it.next())
      {
	auto garc = it.get_current_arc();
	if (is_a_cross_arc <GT> (garc))
	  {
	    cross_arc_list.append(garc); 
	    continue;
	  }

	if (not is_an_cut_arc <GT> (garc)) 
	  continue;

	typename GT::Node * src = mapped_node<GT>(gptr->get_src_node(garc));
	typename GT::Node * tgt = mapped_node<GT>(gptr->get_tgt_node(garc));

	assert(src != nullptr and tgt != nullptr);

	typename GT::Arc * arc = 
	  cut_graph.insert_arc(src, tgt, garc->get_info());
	GT::map_arcs(garc, arc);
      }
  }

  void compute_blocks(DynDlist<GT> &                block_list, 
		      GT &                          cut_graph, 
		      DynDlist<typename GT::Arc*> & cross_arc_list)
  {
    if (state < Cut_Nodes_Computed)
      throw std::logic_error("Cut nodes have not been computed");

    if (state == Cut_Nodes_Computed)
      paint_subgraphs();

    const long & num_colors = curr_color;
    
    DynArray<GT*> blocks; // bloques en un arreglo para rápido
			  // acceso. Que esté o no vacío indica si ha
			  // sido o no procesado.
    blocks.reserve(num_colors);

        // crear lista de componentes vacíos ordenador por color i
    for (int i = 0; i < num_colors; ++i)
      blocks.access(i) = &block_list.append(GT());

        // Recorrer los nodos y copiar y mapear según color
    for (typename GT::Node_Iterator it(*gptr); it.has_curr(); it.next())
      {
	auto p = it.get_curr();
	if (IS_NODE_VISITED(p, Build_Subtree))
	  continue;

	if (is_a_cut_node <GT> (p))
	  continue;

	const long color = get_color<GT>(p);

	GT & sg = *blocks.access(color - 1);

	create_and_map_node(p, color, sg);

	map_subgraph(sg, p, color);	
      }

    map_cut_graph(cut_graph, cross_arc_list);
  }
};

} // end namespace Aleph

# endif // TPL_CUT_NODES_H
