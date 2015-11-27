
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

# ifndef TPL_COMPONENTS_H
# define TPL_COMPONENTS_H

# include <tpl_agraph.H>

namespace Aleph {


      template <class GT, class SA = Dft_Show_Arc<GT> > 
class Build_Subgraph
{
  SA & sa;
  const GT * gptr;
  size_t count;

public:

  Build_Subgraph(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Build_Subgraph(SA & __sa) 
      noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

private:

  void build_subgraph(GT & sg, typename GT::Node * g_src)
  {
    if (IS_NODE_VISITED(g_src, Build_Subtree)) 
      return;

    NODE_BITS(g_src).set_bit(Build_Subtree, true); // g_src visitado
    ++count; 

    auto sg_src = mapped_node <GT> (g_src);
    if (sg_src == nullptr) // ¿está mapeado g_src?
      {     // No, cree imagen de g_src en el subgrafo sg y mapee
	sg_src = sg.insert_node(g_src->get_info());
	GT::map_nodes(g_src, sg_src);
      }

    for (Node_Arc_Iterator<GT, SA> i(g_src, sa); // explore desde g_src 
	 count < gptr->get_num_nodes() and i.has_current(); i.next())
      {
        auto arc = i.get_current_arc();
        if (IS_ARC_VISITED(arc, Build_Subtree)) 
          continue; // avance próximo arco

        ARC_BITS(arc).set_bit(Build_Subtree, true); // arc visitado

        auto g_tgt  = i.get_tgt_node(); // destino de arc
        auto sg_tgt = mapped_node <GT> (g_tgt);
        if (sg_tgt == nullptr) // ¿está mapeado en sg?
          {    // no, hay que mapearlo e insertarlo en el subgrafo sg
            sg_tgt = sg.insert_node(g_tgt->get_info()); 
            GT::map_nodes(g_tgt, sg_tgt); 
          }

    	    // tenemos nodos en subgrafo, insertamos arco y mapeamos
        auto sg_arc = sg.insert_arc(sg_src, sg_tgt, arc->get_info());

        GT::map_arcs(arc, sg_arc); 

        build_subgraph(sg, g_tgt);
      }
  }

      template <template <class> class List>
  void build_subgraph(List<typename GT::Node*> & l, typename GT::Node * p)
  {
    if (IS_NODE_VISITED(p, Build_Subtree)) 
      return;

    NODE_BITS(p).set_bit(Build_Subtree, true); // g_src visitado
    ++count; 
    l.append(p);

    for (Node_Arc_Iterator<GT, SA> it(p, sa); // explore desde g_src 
	 count < gptr->get_num_nodes() and it.has_curr(); it.next())
      {
        auto arc = it.get_current_arc();
        if (IS_ARC_VISITED(arc, Build_Subtree)) 
          continue; // avance próximo arco

        ARC_BITS(arc).set_bit(Build_Subtree, true); // arc visitado
        build_subgraph(l, it.get_tgt_node());
      }
  }
    
  public:

  void operator () (const GT & g, GT & sg, typename GT::Node * g_src) 
  {
    gptr = &g;
    count = 0;
    build_subgraph (sg, g_src);
  }

  GT operator () (const GT & g, typename GT::Node * src) 
  {
    GT sg;
    gptr = &g;
    count = 0;
    build_subgraph(sg, src);
    return sg;
  }

  void operator () (const GT & g, DynList<typename GT::Node*> & list, 
		    typename GT::Node * src) 
  {
    gptr = &g;
    count = 0;
    build_subgraph<DynList>(list, src);
  }
}; 


      template <class GT, class SA = Dft_Show_Arc<GT> > 
class Inconnected_Components
{
  SA & sa;

public:

  Inconnected_Components(SA && __sa = SA()) 
  noexcept(std::is_nothrow_move_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

  Inconnected_Components(SA & __sa) 
  noexcept(std::is_nothrow_copy_assignable<SA>::value)
    : sa(__sa) { /* empty */ }

      template <template <class> class List>
  void compute_blocks(const GT & g, List <GT> & list)
  {
    g.reset_nodes(); 
    g.reset_arcs();  
    size_t count = 0; // contador de nodos visitados
    for (typename GT::Node_Iterator it(g); // recorrer nodos de g
         count < g.get_num_nodes() and it.has_curr(); it.next())
      {
        auto curr = it.get_current_node();
        if (IS_NODE_VISITED(curr, Build_Subtree)) 
          continue;

        GT & subgraph = list.append(GT()); // grafo insertado en list

        Build_Subgraph <GT, SA> build(sa);
	build(g, subgraph, curr); 
	
	count += subgraph.get_num_nodes();
      }
  }

      template <template <class> class List>
  void compute_lists(const GT & g, List<List<typename GT::Node*> > & list)
  {
    g.reset_nodes(); 
    g.reset_arcs();  
    size_t count = 0; // contador de nodos visitados
    for (typename GT::Node_Iterator i(g); // recorrer nodos de g
         count < g.get_num_nodes() and i.has_curr(); i.next())
      {
        auto curr = i.get_current_node();
        if (IS_NODE_VISITED(curr, Build_Subtree)) 
          continue;

	// crear subgrafo componente inconexo conectado por curr_node 
        auto & l = list.append(List<typename GT::Node*>()); 

        Build_Subgraph <GT, SA> build(sa);
	build(g, l, curr); 
	
	count += l.size();
      }
  }
    
  void operator () (const GT & g, DynList <GT> & list)
  {
    compute_blocks<DynList>(g, list);
  }

  void operator () (const GT & g, DynList<DynList<typename GT::Node*>> & list)
  {
     compute_lists<DynList>(g, list);
  }
};


} // end namespace Aleph

# endif //  TPL_COMPONENTS_H
