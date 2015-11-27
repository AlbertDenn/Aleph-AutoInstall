
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

# ifndef TPL_KGRAPH_H
# define TPL_KGRAPH_H

# include <Set.H>
# include <tpl_net.H>

namespace Aleph {

template <class GT, 
	  template <class> class Max_Flow = Random_Preflow_Maximum_Flow,
	  class SA = Dft_Show_Arc<GT> > 
long edge_connectivity(GT & g)
{
  typedef Net_Graph<Net_Node<Empty_Class>, Net_Arc<Empty_Class>> Net;
  Net net;
  typename Net::Node * source = nullptr; 
  long min_degree = numeric_limits<long>::max(); 
  for (Node_Iterator<GT> it(g); it.has_current(); it.next())
    {
      auto p = it.get_current();
      NODE_COOKIE(p) = net.insert_node();
      if (g.get_num_arcs(p) < min_degree)
	{
	  source = mapped_node<GT, Net> (p);
	  min_degree = g.get_num_arcs(p);
	}
    }

  if (min_degree <= 1)
    return min_degree; 

  for (Arc_Iterator<GT, SA> it(g); it.has_current(); it.next()) 
    {
      auto a   = it.get_current();
      auto src = mapped_node<GT, Net> (g.get_src_node(a));
      auto tgt = mapped_node<GT, Net> (g.get_tgt_node(a));
      if (src != source)
	net.insert_arc(tgt, src, 1);
      if (tgt != source)
	net.insert_arc(src, tgt, 1);
    }

  long min_k = min_degree;
  for (Node_Iterator<Net> it(net); it.has_curr(); it.next())
    {
      auto sink = it.get_curr();
      if (sink == source) 
	continue;

      DynDlist<typename Net::Arc*> from_sink_arcs;

      for (Node_Arc_Iterator<Net> it(sink); it.has_current(); it.next())
	from_sink_arcs.append(it.get_curr());

      for (typename DynDlist<typename Net::Arc*>::Iterator 
	     it(from_sink_arcs); it.has_curr(); it.next())
	net.disconnect_arc(it.get_curr());

      const typename Net::Flow_Type flow = Max_Flow <Net> () (net); 
      if (flow < min_k)
	min_k = flow;

      while (not from_sink_arcs.is_empty())
	net.connect_arc(from_sink_arcs.get());

      net.reset(); // colocar flujo en cero
    }

  return min_k;
}

    template <class GT, 
	      template <class> class Max_Flow = Heap_Preflow_Maximum_Flow> 
class Edge_Connectivity
{
public:

  long operator () (GT & g) { return edge_connectivity <GT, Max_Flow> (g); }
};


  template <class GT, 
	    template <class> class Max_Flow = Heap_Preflow_Maximum_Flow,
	    class SA = Dft_Show_Arc<GT> > 
long compute_min_cut(GT &                             g,
		     Aleph::set<typename GT::Node*> & l, 
		     Aleph::set<typename GT::Node*> & r,
		     DynDlist<typename GT::Arc *> &   cut)
{
  typedef Net_Graph<Net_Node<Empty_Class>, Net_Arc<Empty_Class> > Net;
  Net net;
  typename Net::Node * source = nullptr; 
  long min_degree = numeric_limits<long>::max(); 
  for (Node_Iterator<GT> it(g); it.has_current(); it.next())
    {
      auto p  = it.get_current();
      auto q = net.insert_node();
      GT::map_nodes (p, q);

      if (g.get_num_arcs(p) < min_degree)
	{
	  source = mapped_node<GT, Net> (p);
	  min_degree = g.get_num_arcs(p);
	}
    }

  if (min_degree <= 1)
    return min_degree; 

  for (Arc_Iterator<GT, SA> it(g); it.has_current(); it.next()) 
    {
      auto a   = it.get_current();
      auto src = mapped_node<GT, Net>(g.get_src_node(a));
      auto tgt = mapped_node<GT, Net>(g.get_tgt_node(a));
      if (src != source)
	{
	  auto arc = net.insert_arc(tgt, src, 1);
	  ARC_COOKIE(arc) = a;
	}

      if (tgt != source)
	{
	  auto arc = net.insert_arc(src, tgt, 1);
	  ARC_COOKIE(arc) = a;
	}
    }

  Aleph::set<typename Net::Node*> tmp_vs, tmp_vt;
  DynDlist<typename Net::Arc*>    tmp_cuts, tmp_cutt;
  long min_k = numeric_limits<long>::max();  
  for (Node_Iterator<Net> it(net); it.has_curr(); it.next())
    {
      auto sink = it.get_curr();
      if (sink == source) 
	continue;

      DynDlist<typename Net::Arc*> from_sink_arcs;

      for (Node_Arc_Iterator<Net> it(sink); it.has_curr(); it.next())
	from_sink_arcs.append(it.get_curr());

      for (typename DynDlist<typename Net::Arc*>::Iterator 
	     it(from_sink_arcs); it.has_curr(); it.next())
	net.disconnect_arc(it.get_curr());

      Aleph::set<typename Net::Node*> vs, vt;
      DynDlist<typename Net::Arc *> cuts, cutt;
      const auto flow = Min_Cut <Net, Max_Flow> () (net, vs, vt, cuts, cutt); 

      if (flow < min_k)
	{
	  min_k = flow;
	  tmp_vs.swap(vs);
	  tmp_vt.swap(vt);
	  tmp_cuts.swap(cuts);
	  tmp_cutt.swap(cutt);
	}

      net.reset(); // colocar flujo en cero

      while (not from_sink_arcs.is_empty())
	net.connect_arc(from_sink_arcs.get());
    }
  
  for (typename Aleph::set<typename Net::Node*>::iterator
	 it = tmp_vs.begin(); it != tmp_vs.end(); it++)
    l.insert(mapped_node <Net, GT> (*it));

  for (typename Aleph::set<typename Net::Node*>::iterator 
	 it = tmp_vt.begin(); it != tmp_vt.end(); it++)
    r.insert(mapped_node <Net, GT> (*it));

  for (typename DynDlist<typename Net::Arc*>::Iterator it(tmp_cuts); 
       it.has_current(); it.next())
    {
      typename Net::Arc* arc = it.get_current();
      cut.append(mapped_arc<Net, GT> (arc));
    }

  return min_k;
}

    template <class GT, 
	      template <class> class Max_Flow = Heap_Preflow_Maximum_Flow,
	      class SA = Dft_Show_Arc<GT> > 
class Compute_Min_Cut
{
public:
  long operator () (GT &                             g,
		    Aleph::set<typename GT::Node*> & l, 
		    Aleph::set<typename GT::Node*> & r,
		    DynDlist<typename GT::Arc *> &   cut) 
  { 
    return compute_min_cut <GT, Max_Flow, SA> (g, l, r, cut); 
  }
};


  template <class GT, 
	    template <class> class Max_Flow = Random_Preflow_Maximum_Flow,
	    class SA                        = Dft_Show_Arc<GT> > 
long vertex_connectivity(GT & g)
{
  typedef Net_Graph<Net_Node<Empty_Class>, Net_Arc<Empty_Class> > Net;
  Net net;
  for (Node_Iterator<GT> it(g); it.has_current(); it.next())
    {
      auto p = it.get_current();
      NODE_COOKIE(p) = net.insert_node();
    }

  for (Arc_Iterator<GT, SA> it(g); it.has_current(); it.next()) 
    {
      auto a   = it.get_current();
      auto src = mapped_node<GT,Net>(g.get_src_node(a));
      auto tgt = mapped_node<GT,Net>(g.get_tgt_node(a));
      net.insert_arc(tgt, src, 1);
      net.insert_arc(src, tgt, 1);
    }

  long min_k = g.get_num_nodes();
  int i = 1;

  for (Node_Iterator<Net> k(net); k.has_curr() and i < min_k; k.next(), i++)
    {
      auto source = k.get_curr();
      DynDlist<typename Net::Arc*> to_source_list;
      for (Node_Arc_Iterator<Net> it(source); it.has_curr(); it.next())
	{
	  auto from_arc = it.get_curr();
	  auto to_arc = 
	    search_arc<Net>(net, net.get_tgt_node(from_arc), source);
	  assert(to_arc != nullptr);
	  assert(net.get_tgt_node(to_arc) == source);
	  to_source_list.append(to_arc);
	}

      for (typename DynDlist<typename Net::Arc *>::Iterator it(to_source_list); 
	   it.has_curr(); it.next())
	net.disconnect_arc(it.get_curr());

      {
	Node_Iterator<Net> j(k);
	for (j.next(); j.has_curr(); j.next())
	  {
	    auto sink = j.get_curr();
	    if (search_arc <Net> (net, source, sink) != nullptr)
	      continue; // existe arco ==> ignórelo y avance a siguiente

	    DynDlist<typename Net::Arc*> from_sink_arcs;

	    for (Node_Arc_Iterator<Net> it(sink); it.has_curr(); it.next())
	      from_sink_arcs.append(it.get_curr());

	    for (typename DynDlist<typename Net::Arc*>::Iterator 
		   it(from_sink_arcs); it.has_curr(); it.next())
	      net.disconnect_arc(it.get_curr());
	    {
	      Net aux_net;
	      {
		DynMapTreap<typename Net::Node*,typename Net::Arc*> map;
		for (Node_Iterator<Net> it(net); it.has_curr(); it.next())
		  {
		    auto p = it.get_curr();
		    if (p == source or p == sink)
		      {
			NODE_COOKIE(p) = aux_net.insert_node();
			continue;
		      }

		    auto ps = aux_net.insert_node(p->get_info());
		    auto pt = aux_net.insert_node(p->get_info());
		    map.insert(p, aux_net.insert_arc(ps, pt, 1));
		  }

		for (Arc_Iterator<Net> it(net); it.has_curr(); it.next())
		  {
		    auto a    = it.get_curr();
		    auto nsrc = net.get_src_node(a);
		    auto ntgt = net.get_tgt_node(a);
		    typename Net::Node * asrc = nullptr;
		    typename Net::Node * atgt = nullptr;

		    if (nsrc == source)
		      asrc = (typename Net::Node *) NODE_COOKIE(nsrc);
		    else
		      {
			auto arc = map.find(nsrc);
			asrc = aux_net.get_tgt_node(arc);
		      }

		    if (ntgt == sink)
		      atgt = (typename Net::Node *) NODE_COOKIE(ntgt);
		    else
		      {
			auto arc = map.find(ntgt);
			atgt = aux_net.get_src_node(arc);
		      }
		    aux_net.insert_arc(asrc, atgt, 1);
		  }
	      } // fin de bloque que crea el mapeo

	      const auto flow = Max_Flow <Net> () (aux_net); 
	      if (flow < min_k)
		min_k = flow;
	    }

	    while (not from_sink_arcs.is_empty())
	      net.connect_arc(from_sink_arcs.get());

	    net.reset(); // colocar flujo en cero
	  }

	while (not to_source_list.is_empty())
	  net.connect_arc(to_source_list.get());
      }
    }

  return min_k;
}


} // end namespace Aleph

# endif // TPL_KGRAPH_H
