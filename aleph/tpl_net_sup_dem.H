
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

#line 620 "reducciones.nw"
# ifndef TPL_NET_SUP_DEM_H
# define TPL_NET_SUP_DEM_H

# include <tpl_netgraph.H>

namespace Aleph {

#line 586 "reducciones.nw"
    template <typename Node_Info, typename F_Type = long>
class Net_Sup_Dem_Node : public Net_Node<Node_Info, F_Type>
{
public:
  typedef F_Type Flow_Type;

  Flow_Type supply_flow; 
  Flow_Type & get_supply_flow() { return supply_flow; }

  Net_Sup_Dem_Node() : supply_flow(0) { /* empty */ }

  Net_Sup_Dem_Node(const Node_Info & node_info) 
    : Net_Node<Node_Info, F_Type>(node_info), supply_flow(0) { /* empty */ }

  Net_Sup_Dem_Node(Net_Sup_Dem_Node * node) 
    : Net_Node<Node_Info, F_Type>(*node), supply_flow(node->supply_flow) { /* empty */ }
};

#line 629 "reducciones.nw"
    template <class NodeT = Net_Sup_Dem_Node<Empty_Class, double>, 
	      class ArcT  = Net_Arc<Empty_Class, double> >
class Net_Sup_Dem_Graph : public Net_Graph<NodeT, ArcT>
{
public:
  typedef Net_Graph<NodeT, ArcT> Net_Class;
  typedef ArcT Arc;
  typedef NodeT Node;
  typedef typename Arc::Flow_Type Flow_Type;
  typedef typename Node::Node_Type Node_Type;
  typedef typename Arc::Arc_Type Arc_Type;

  
#line 675 "reducciones.nw"
Node * insert_node(const Node_Type & node_info, const Flow_Type & supply = 0)
{
  Node * p = Net_Class::insert_node(node_info);
  p->supply_flow = supply;
  return p;
}
Node * insert_node(const Flow_Type & supply = 0)
{
  return insert_node(Node_Type(), supply);
}
#line 719 "reducciones.nw"
private:

  Node * super_source;
  Node * super_sink;

public:
  Net_Sup_Dem_Graph() : super_source(nullptr), super_sink(nullptr) { /* empty */ }
  bool exist_aux_net() const 
  { 
    return super_source != nullptr or super_sink != nullptr; 
  }

#line 739 "reducciones.nw"
typedef Net_Sup_Dem_Graph Aux_Net;

Net_Sup_Dem_Graph * compute_aux_net()
{
  if (exist_aux_net())
    throw std::domain_error("Auxiliar net is already computed");
  if (this->residual_net)
    throw std::domain_error("Residual net has been already computed");

  super_source = insert_node();   // fuente auxiliar
  super_sink   = insert_node();   // sumidero auxiliar
      // recorrer nodos y atar arcos a fuente o sumidero según signo provisión
  for (Node_Iterator<Net_Sup_Dem_Graph> it(*this); it.has_current(); it.next())
    {
      Node * p = it.get_current();
      if (p->supply_flow > 0) // ¿nodo proveedor?
	{
 	  if (p->out_cap < p->supply_flow)
 	    throw std::range_error(gnu::autosprintf("Supply flow in node at %p is "
						    "greater than out capacity", p));
        insert_arc(super_source, p, p->supply_flow);
	}
      else if (p->supply_flow < 0) // ¿nodo demandante?
	{
 	  if (p->in_cap < -p->supply_flow)
 	    throw std::range_error(gnu::autosprintf("Supply flow in node at %p is "
						    "smaller than in capacity", p));
        insert_arc(p, super_sink, -p->supply_flow);
	}
    }
  if (this->get_out_degree(super_source) == 0)
    {
      remove_node(super_source);
      super_source = nullptr;
    }
   if (this->get_in_degree(super_sink) == 0)
    {
      remove_node(super_sink);
      super_sink = nullptr;
    }
  return this;
}
#line 825 "reducciones.nw"
  Net_Sup_Dem_Graph * get_aux_net() { return exist_aux_net() ? this : nullptr; }
#line 833 "reducciones.nw"
bool is_feasible() const
{
  if (not exist_aux_net())
    throw std::domain_error("Auxiliar net has not been computed");
  for (Node_Iterator<Net_Sup_Dem_Graph> it(*this); it.has_current(); it.next())
    {
      Node * p = it.get_current();
      Flow_Type & supply_flow = p->supply_flow;
      if (supply_flow == 0)
	continue;
      if (supply_flow > 0 and p->out_flow < supply_flow)
	return false;
      else if (supply_flow < 0 and p->in_flow < -supply_flow)
	return true;
    }
  return true;
}

void non_feasible_nodes(DynDlist<Node *> & supply_list, DynDlist<Node *> & demand_list)
{
  for (Node_Iterator<Net_Sup_Dem_Graph> it(*this); it.has_current(); it.next())
    {
      Node * p = it.get_current();
      Flow_Type & supply_flow = p->supply_flow;
      if (supply_flow == 0)
	continue;
      if (supply_flow > 0 and p->out_flow < supply_flow)
	supply_list.append(p);
      else if (supply_flow < 0 and p->in_flow < -supply_flow)
	demand_list.append(p);
    }
}
#line 895 "reducciones.nw"
void set_supply_flow(Node * p, const Flow_Type & supply)
{
  if (supply > 0 and p->out_cap < supply)
    throw std::range_error(gnu::autosprintf("Supply flow in node at %p is "
					    "greater than out capacity", p));
  else if (supply < 0 and p->in_cap < -supply)
    throw std::range_error(gnu::autosprintf("Supply flow in node at %p is "
					    "smaller than in capacity", p));
  p->supply_flow = supply;
}
#line 922 "reducciones.nw"
void free_aux_net()
{
  if (not exist_aux_net())
    throw std::domain_error("Auxiliar net has not been computed");

  if (this->residual_net)
    {
      this->unmake_residual_net();
      this->unmake_super_nodes();
    } 
  if (super_source != nullptr)
    {
      remove_node(super_source);
      super_source = nullptr;
    }
  if (super_sink != nullptr)
    {
      remove_node(super_sink);
      super_sink = nullptr;
    }
}
#line 664 "reducciones.nw"
};

} // end namespace Aleph

# endif // TPL_NET_SUP_DEM_H
