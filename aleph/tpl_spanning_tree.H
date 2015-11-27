
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

# ifndef TPL_SPANNING_TREE_H
# define TPL_SPANNING_TREE_H

namespace Aleph {


    template <class GT, class SA = Dft_Show_Arc<GT> > 
class Find_Depth_First_Spanning_Tree
{
  SA & sa;
  GT * gptr;
  GT * tptr;

  bool build_tree(typename GT::Node * gnode, typename GT::Arc * garc, 
		  typename GT::Node * tnode)
  {
    NODE_BITS(gnode).set_bit(Spanning_Tree, true); // marcar nodo 
    ARC_BITS(garc).set_bit(Spanning_Tree, true);   // marcar arco

    auto tree_tgt_node = tptr->insert_node(gnode->get_info());
    GT::map_nodes(gnode, tree_tgt_node);

    auto tarc = tptr->insert_arc(tnode, tree_tgt_node, garc->get_info()); 
    GT::map_arcs(garc, tarc);

    tnode = tree_tgt_node; 
    if (tptr->get_num_nodes() == gptr->get_num_nodes()) // ¿grafo abarcado?
      return true; // tree ya contiene el árbol abarcador

    assert(tptr->get_num_nodes() > tptr->get_num_arcs()); // debe ser acíclico

    for (Node_Arc_Iterator<GT, SA> i(gnode, sa); 
	 i.has_curr() and tptr->get_num_nodes() < gptr->get_num_nodes();
	 i.next())
      {
        auto arc = i.get_current_arc();
        if (IS_ARC_VISITED(arc, Spanning_Tree)) 
          continue;

        auto arc_tgt_node = i.get_tgt_node();
        if (IS_NODE_VISITED(arc_tgt_node, Spanning_Tree))
          continue; // destino ya visitado desde otro arco

        if (build_tree(arc_tgt_node, arc, tnode))
	  return false;
      }

    return false;
  }

  bool build_tree(GT & g, typename GT::Node * gnode, GT & tree)
  {
    gptr = &g;
    tptr = &tree;

    gptr->reset_nodes();
    gptr->reset_arcs(); 

    clear_graph(*tptr); // asegurar que árbol destino esté vacío
  
    NODE_BITS(gnode).set_bit(Spanning_Tree, true); // marcar gnode
  
    auto tnode = tree.insert_node(gnode->get_info()); 
    GT::map_nodes(gnode, tnode);
  
    for (Node_Arc_Iterator<GT, SA> i(gnode, sa); 
	 i.has_curr() and tptr->get_num_nodes() < gptr->get_num_nodes(); 
	 i.next())
      {
	auto arc = i.get_current_arc();
	if (IS_ARC_VISITED(arc, Spanning_Tree)) 
	  continue;

	auto arc_tgt_node = i.get_tgt_node();
	if (IS_NODE_VISITED(arc_tgt_node, Spanning_Tree))
	  continue; // destino ya visitado desde otro arco

	if (build_tree(arc_tgt_node, arc, tnode))
	  return false;
      }

    return true;
  }

public:

  Find_Depth_First_Spanning_Tree(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Find_Depth_First_Spanning_Tree(SA & __sa) : sa(__sa) { /* empty */ }

  typename GT::Node * operator () (GT & g, GT & tree)
  {
    auto start = g.get_first_node();
    if (not build_tree(g, start, tree)) 
      return nullptr;

    return start;
  }

  typename GT::Node * operator () (GT & g, typename GT::Node * gnode, GT & tree)
  {
    this->build_tree(g, gnode, tree);
    return (typename GT::Node *) NODE_COOKIE(gnode);
  }
};



      template <class GT, class SA = Dft_Show_Arc<GT> > 
class Find_Breadth_First_Spanning_Tree
{
  SA & sa;

  void build_tree(GT & g, typename GT::Node * gp, GT & tree)
  {
    g.reset_bit_nodes(Spanning_Tree);
    g.reset_bit_arcs(Spanning_Tree);

    clear_graph(tree); 

    unique_ptr<typename GT::Node> tp_auto(new typename GT::Node(gp));
    tree.insert_node(tp_auto.get());
    GT::map_nodes(gp, tp_auto.release());

    DynListQueue<typename GT::Arc*> q; // insertar en cola arcos gp
    for (Node_Arc_Iterator<GT, SA> i(gp, sa); i.has_curr(); i.next())
      q.put(i.get_current_arc());

    NODE_BITS(gp).set_bit(Spanning_Tree, true); 

    while (not q.is_empty()) 
      {
	auto garc = q.get(); 
	ARC_BITS(garc).set_bit(Spanning_Tree, true); 
	auto gsrc = g.get_src_node(garc);  
	auto gtgt = g.get_tgt_node(garc);

	if (IS_NODE_VISITED(gsrc, Spanning_Tree) and 
	    IS_NODE_VISITED(gtgt, Spanning_Tree))
	  continue; // los dos nodos de garc ya fueron visitados

	if (IS_NODE_VISITED(gtgt, Spanning_Tree)) // ¿gtgt visitado?
	  std::swap(gsrc, gtgt); // sí, intercámbielo con gsrc

	auto tsrc = mapped_node<GT>(gsrc);
	NODE_BITS(gtgt).set_bit(Spanning_Tree, true); // gtgt visitado

            // crear copia de gtgt, insertarlo en tree y mapearlo
	unique_ptr<typename GT::Node> ttgt_auto(new typename GT::Node(gtgt));
	tree.insert_node(ttgt_auto.get());
	auto ttgt = ttgt_auto.release();
	GT::map_nodes(gtgt, ttgt);

            // insertar nuevo arco en tree y mapearlo
	auto tarc = tree.insert_arc(tsrc, ttgt, garc->get_info());
	GT::map_arcs(garc, tarc);
	if (tree.get_num_nodes() == g.get_num_nodes()) // ¿abarca a g?
	  break; 

            // insertar en cola arcos de gtgt
	for (Node_Arc_Iterator<GT, SA> i(gtgt, sa); i.has_curr(); i.next())
	  {
	    auto current_arc = i.get_current_arc();
	    if (IS_ARC_VISITED(current_arc, Spanning_Tree)) 
	      continue;

                // revise nodos de arcos para ver si han sido visitados
	    if (IS_NODE_VISITED(g.get_src_node(current_arc), Spanning_Tree) and 
		IS_NODE_VISITED(g.get_tgt_node(current_arc), Spanning_Tree))
	      continue; // nodos ya visitados ==> no meter arco
	    q.put(current_arc);
	  }
      }
  }

public:

  Find_Breadth_First_Spanning_Tree(SA && __sa = SA()) : sa(__sa) { /* empty */ }

  Find_Breadth_First_Spanning_Tree(SA & __sa) : sa(__sa) { /* empty */ }

    void operator () (GT & g, typename GT::Node * gnode, GT & tree) 
    {
      find_breadth_first_spanning_tree <GT, SA> (g, gnode, tree);
    }
  };


      template <class GT>
class Build_Spanning_Tree
{
public:

  GT operator () (const DynArray<typename GT::Arc> *  arcs) const
  {
    return build_spanning_tree(arcs);
  }
};


} // end namespace Aleph

# endif //  TPL_SPANNING_TREE_H
