
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

# ifndef TPL_NETCAPGRAPH_H
# define TPL_NETCAPGRAPH_H
# include <tpl_netgraph.H>

namespace Aleph {

    template <typename Node_Info, typename F_Type = double>
class Net_Cap_Node : public Net_Node<Node_Info, F_Type>
{
public:
  typedef F_Type Flow_Type;

  Flow_Type max_cap; 

  Net_Cap_Node() : max_cap(numeric_limits<Flow_Type>::max()) {}

  Net_Cap_Node(const Node_Info & node_info) 
    : Net_Node<Node_Info, F_Type>(node_info), 
      max_cap(numeric_limits<Flow_Type>::max())
  { /* empty */ }

  Net_Cap_Node(Net_Cap_Node * node) 
    : Net_Node<Node_Info, F_Type>(*node), max_cap(node->max_cap) { /* empty */ }
}; 

    template <class NodeT, class ArcT>
class Net_Cap_Graph : public Net_Graph<NodeT, ArcT>
{
public:
  typedef Net_Graph<NodeT, ArcT> Net_Class;
  typedef ArcT Arc;
  typedef NodeT Node;
  typedef typename Arc::Flow_Type Flow_Type;
  typedef typename Node::Node_Type Node_Type;
  typedef typename Arc::Arc_Type Arc_Type;

  Node * insert_node(const Node_Type & node_info, 
                     const Flow_Type & cap = numeric_limits<Flow_Type>::max())
  {
    Node * p = Net_Class::insert_node(node_info);
    p->max_cap = cap;
    return p;
  }
    typedef Net_Graph<Net_Node<Empty_Class, Flow_Type>, 
                      Net_Arc<bool, Flow_Type> > Aux_Net;
  private:
    Aux_Net * aux_net;
  public:
    Net_Cap_Graph() : aux_net(nullptr) { /* empty */ }
  Aux_Net * get_aux_net() { return aux_net; }
  Aux_Net * compute_aux_net()
  {
    if (aux_net != nullptr)
      throw std::domain_error("aux_net has already computed");
    aux_net = new Aux_Net;
    try
      {
        for (Node_Iterator<Net_Cap_Graph> it(*this); it.has_current(); it.next())
          {
            Node * p = it.get_current();
            typename Aux_Net::Node * src = aux_net->insert_node();
            typename Aux_Net::Node * tgt = aux_net->insert_node();
            typename Aux_Net::Arc  * arc = 
              aux_net->insert_arc(src, tgt, true, p->max_cap, 0);
            NODE_COOKIE(p)  = arc;
            ARC_COOKIE(arc) = p;
          }
        for (Arc_Iterator<Net_Cap_Graph> it(*this); it.has_current(); it.next())
          {
            Arc * arc = it.get_current();
            typename Aux_Net::Arc * src_arc = 
              (typename Aux_Net::Arc *) NODE_COOKIE(get_src_node(arc));
            typename Aux_Net::Arc * tgt_arc = 
              (typename Aux_Net::Arc *) NODE_COOKIE(get_tgt_node(arc));
            typename Aux_Net::Arc * a = 
              aux_net->insert_arc(aux_net->get_tgt_node(src_arc), 
                                  aux_net->get_src_node(tgt_arc), false, 
                                  arc->cap, arc->flow);
            ARC_COOKIE(arc) = a;
            ARC_COOKIE(a)   = arc;
          }
        return aux_net;
      }
    catch (...)
      {
        free_aux_net();
      }
  }
  void update()
  {
    if (aux_net == nullptr)
      throw std::domain_error("Auxiliar net has not been generated");

    for (Arc_Iterator<Aux_Net, No_Res_Arc<Aux_Net> > it(*aux_net); 
         it.has_current(); it.next())
      {
        typename Aux_Net::Arc * arc = it.get_current();
        if (arc->get_info())
          {
            Node * p = (Node*) ARC_COOKIE(arc);
            p->in_flow = p->out_flow = arc->flow;
          }
        else
          {
            Arc * a = (Arc*) ARC_COOKIE(arc);
            a->flow = arc->flow;
          }
      }
  }
  void free_aux_net()
  {
    if (aux_net == nullptr)
      throw std::domain_error("Auxiliar net has not been generated");

    clear_graph(*aux_net);
    delete aux_net;
    aux_net = nullptr;
  }

  ~Net_Cap_Graph()
  {
    if (aux_net != nullptr)
      free_aux_net();
  }
};

} // end namespace Aleph

# endif // endif
