
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/

# ifndef TPL_DYN_GRAPH_H
# define TPL_DYN_GRAPH_H

# include <tpl_agraph.H>

using namespace Aleph;

namespace Aleph
{

      template <class GT>
  class Dyn_Graph
  {
  public:

    typedef typename GT::Node Node;

    typedef typename GT::Arc Arc;

    typedef typename Node::Node_Info Node_Info;

    typedef typename Arc::Arc_Info Arc_Info;

  private:

    static Node * info_to_node(Node_Info & info)
    {
      Node * node_zero = 0;
      long off_set = (long) &node_zero->get_info();
      long ptr_info = (long) &info;
      return (Node *) (ptr_info - off_set); 
    }

    static Arc * info_to_arc(Arc_Info & info)
    {
      Arc * arc_zero = 0;
      long off_set = (long) &arc_zero->get_info();
      long ptr_info = (long) &info;
      return (Arc *) (ptr_info - off_set); 
    }

    GT graph;

  public:

    Dyn_Graph()
      : graph()
    {
      // Empty
    }

    Dyn_Graph(const Dyn_Graph & g)
      : graph(g.graph)
    {
      // Empty
    }

    Dyn_Graph(Dyn_Graph && g)
      : graph(std::move(g.graph))
    {
      // Empty
    }


    Node_Info & insert_node(const Node_Info & info)
    {
      Node * node = graph.insert_node(new Node(info));
      return node->get_info();
    }

    Node_Info & insert_node(Node_Info && info = Node_Info())
    {
      Node * node = graph.insert_node(new Node(std::move(info)));
      return node->get_info();
    }

    Arc_Info & insert_arc(Node_Info & src_info, Node_Info & tgt_info,
			  const Arc_Info & info)
    {
      Node * src = info_to_node(src_info);
      Node * tgt = info_to_node(tgt_info);
      Arc * arc = graph.insert_arc(src, tgt);
      arc->get_info() = info;
      return arc->get_info();
    }

    Arc_Info & insert_arc(Node_Info & src_info, Node_Info & tgt_info,
			  Arc_Info && info = Arc_Info())
    {
      Node * src = info_to_node(src_info);
      Node * tgt = info_to_node(tgt_info);
      Arc * arc = graph.insert_arc(src, tgt);
      arc->get_info() = std::move(info);
      return arc->get_info();
    }

    Node_Info & get_src_node(Arc_Info & info)
    {
      Arc * arc = info_to_arc(info);
      Node * src = graph.get_src_node(arc);
      return src->get_info();
    }

    Node_Info & get_tgt_node(Arc_Info & info)
    {
      Arc * arc = info_to_arc(info);
      Node * tgt = graph.get_tgt_node(arc);
      return tgt->get_info();
    }

    Node_Info & get_connected_node(Node_Info & node_info, Arc_Info & arc_info)
    {
      Node * node = info_to_node(node_info);
      Arc * arc = info_to_arc(arc_info);
      Node * connected_node(node, arc);
      return connected_node->get_info();
    }

    void remove_arc(Arc_Info & info)
    {
      Arc * arc = info_to_arc(info);
      graph.remove_arc(arc);
    }

    void remove_node(Node_Info & info)
    {
      Node * node = info_to_node(info);
      graph.remove_node(node);
    }

    const size_t & get_num_nodes() const
    {
      return graph.get_num_nodes();
    }

    const size_t & get_num_arcs() const
    {
      return graph.get_num_arcs();
    }

    const size_t & get_num_arcs(Node_Info & info) const
    {
      Node * node = info_to_node(info);
      return graph.get_num_arcs(node);
    }
  };

  # define GRAPH_SPECIALIZATION(G, N, A) \
    template <typename Node_Info, typename Arc_Info>                     \
    class Dyn_##G : public Dyn_Graph<G<N<Node_Info>, A<Arc_Info>>>       \
    {                                                                    \
      typedef G<N<Node_Info>, A<Arc_Info>> Graph_Type;                   \
                                                                         \
    public:                                                              \
      Dyn_##G()                                                          \
        : Dyn_Graph <Graph_Type>() { }                                   \
                                                                         \
      Dyn_##G(const Dyn_##G & g)                                         \
        : Dyn_Graph <Graph_Type>(g) { }                                  \
                                                                         \
      Dyn_##G(Dyn_##G && g)                                              \
        : Dyn_Graph <Graph_Type>(std::move(g)) { }                       \
    };

  GRAPH_SPECIALIZATION(List_Graph, Graph_Node, Graph_Arc)

  GRAPH_SPECIALIZATION(List_Digraph, Graph_Node, Graph_Arc)

} // End namespace Aleph

# endif // TPL_DYN_GRAPH_H

