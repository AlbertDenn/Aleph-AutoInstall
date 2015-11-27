
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

<<tpl\_netgraph.H>>=
%% start-nobook
# ifndef TPL_NETGRAPH_H
# define TPL_NETGRAPH_H

# include <limits>
# include <tpl_graph.H>

using namespace Aleph;

namespace Aleph {

%% end-nobook
    template <typename Arc_Info, typename F_Type = unsigned long>
class Net_Arc : public Graph_Arc<Arc_Info>
{
public:

  typedef F_Type Flow_Type;
  
private:

  Flow_Type cap;
  Flow_Type flow;

public:

  const Flow_Type & get_cap() const { return cap; }

  const Flow_Type & get_flow() const { return flow; }

  void set_cap(const Flow_Type & __cap)
  {
    if (cap < flow)
      throw std::overflow_error("current flow is greater than capacity");

    cap = __cap;
  }

  void set_flow(const Flow_Type & __flow)
  {
    if (cap < flow)
      throw std::overflow_error("current cap is smaller than flow");

    flow = __flow;
  }
};

    template <typename Node_Info, typename F_Type = unsigned long>
class Net_Node : public Graph_Node<Node_Info>
{
public:

  typedef F_Type Flow_Type;
  
private:

  size_t in_degree;
  Flow_Type out_flow;
  Flow_Type in_flow;

  Net_Node(const Node_Info & node_info) 
    : Graph_Node<Node_Info>(node_info), in_degree(0),
      out_flow(numeric_limits<Flow_Type>::min()),
      out_flow(numeric_limits<Flow_Type>::min()) { /* empty */ }
};

    template <class NodeT, class ArcT>
class Net_Graph : public List_Digraph<NodeT, ArcT>
{
public:

  typedef typename List_Digraph<NodeT, ArcT>::Arc Arc;

  typedef typename List_Digraph<NodeT, ArcT>::Node Node;

  typedef typename ArcT::Flow_Type Flow_Type;

  Arc * insert_arc(Node * src_node, Node * tgt_node, 
		   const typename Arc::Arc_Type & arc_info,
		   const Flow_Type & cap, const Flow_Type & flow)
  {
    Arc * arc = insert_arc(src_node, tgt_node, arc_info);

    arc->set_cap(cap);            // error de compilación en estas líneas 
    arc->set_flow(flow);          // puede corresponder con que los tipos 
                                  // Flow_Type del nodo y del arco no son
    tgt_node->in_degree++;        // iguales
    src_node->out_flow += flow;   
    tgt_node->in_flow  += flow;
  }

  void set_cap(Arc * arc, const Flow_Type & cap)
  {
    arc->set_cap(cap);
  }

  void set_flow(Arc * arc, const Flow_Type & flow)
  {
    const Flow_Type old_flow = arc->flow;
    
    arc->set_flow(flow);
	
    Node * src_node = get_src_node(arc);
    src_node->out_flow -= old_flow;
    src_node->out_flow += flow;
    
    Node * tgt_node = get_tgt_node(arc);
    tgt_node->in_flow -= old_flow;
    tgt_node->in_flow += flow;
  }

  bool check_network() const
  {
    
  }
};

%% start-nobook
# endif // TPL_NETGRAPH_H
%% end-nobook
@ 
