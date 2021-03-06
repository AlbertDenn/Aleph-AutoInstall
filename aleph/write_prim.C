
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

# include <autosprintf.h>
# include <tpl_dynBinHeap.H>
# include <Prim.H>
# include <generate_spanning_tree_picture.H>

struct Nodo 
{
  string str;

  Nodo(const int & c)
  {
    str = c;
  }

  Nodo(const string & s) : str(s)
  {
    // empty
  }

  bool operator == (const Nodo & c) const
  {
    return c.str == str;
  }
};


struct Arco
{
  int w;

  Arco(const int & __w) : w(__w) { /* empty */ }

  Arco() { /* empty */ }
};


typedef List_Graph<Graph_Node<Nodo>, Graph_Arc<Arco> > Grafo;


struct Distancia
{
  typedef int Distance_Type;

  static const Distance_Type Zero_Distance = 0;

  int operator () (Grafo::Arc * a) const
  {
    return a->get_info().w;
  }
};


struct Nodo_String
{
  string operator () (Grafo::Node * p) const
  { 
    return p->get_info().str;
  }
};


struct Arco_String
{
  string operator () (Grafo::Arc * a) const
  { 
    return gnu::autosprintf("%d", a->get_info().w);
  }
};


Grafo::Node * bn(Grafo * g, const string & str)
{
  return g->search_node(Nodo(str));
}


void insertar_arco(Grafo *        g,
		   const string & s1, 
		   const string & s2, 
		   const int &    i)
{
  Grafo::Node * src = bn(g, s1);
  Grafo::Node * tgt = bn(g, s2);

  g->insert_arc(src, tgt, i);
}


Grafo * build_graph()
{
  Grafo * g = new Grafo;

  for (int i = 'A'; i <= 'P'; ++i)
    g->insert_node(new Grafo::Node(i));

  insertar_arco(g, "A", "B", 3);
  //  insertar_arco(g, "B", "A", 3);
  insertar_arco(g, "B", "C", 4);
  insertar_arco(g, "C", "D", 3);
  insertar_arco(g, "D", "E", 4);
  insertar_arco(g, "E", "K", 2);
  insertar_arco(g, "K", "P", 7);
  insertar_arco(g, "P", "J", 1);
  insertar_arco(g, "J", "K", 4);
  insertar_arco(g, "K", "D", 5);
  insertar_arco(g, "D", "J", 2);
  insertar_arco(g, "J", "I", 2);
  //  insertar_arco(g, "I", "J", 2);
  insertar_arco(g, "I", "D", 1);
  insertar_arco(g, "I", "C", 2);
  insertar_arco(g, "I", "H", 2);
  insertar_arco(g, "H", "C", 3);
  insertar_arco(g, "H", "B", 7);
  insertar_arco(g, "B", "G", 1);
  insertar_arco(g, "B", "M", 15);
  //  insertar_arco(g, "M", "B", 3);
  insertar_arco(g, "M", "G", 10);
  insertar_arco(g, "G", "A", 4);
  insertar_arco(g, "A", "F", 9);
  insertar_arco(g, "F", "G", 5);
  insertar_arco(g, "F", "L", 10);
  insertar_arco(g, "F", "M", 12);
  insertar_arco(g, "H", "M", 8);
  insertar_arco(g, "L", "M", 4);
  insertar_arco(g, "M", "N", 2);
  insertar_arco(g, "N", "H", 3);
  insertar_arco(g, "N", "I", 1);
  insertar_arco(g, "N", "O", 3);
  insertar_arco(g, "O", "I", 3);
  insertar_arco(g, "O", "J", 1);
  insertar_arco(g, "O", "P", 6);

  return g;    
}

# define TREENODE(p) (static_cast<typename GT::Node*>(NODE_COOKIE(p)))


    template <class GT, class Distance, class Compare> inline 
void write_prim_min_spanning_tree(GT & g, GT & tree)
{
  if (g.is_digraph())
    throw std::domain_error("g is a digraph");

  if (not test_connectivity(g))
    throw std::invalid_argument("Input graph is not conected");

  g.reset_bit_nodes(Aleph::Spanning_Tree);

  clear_graph(tree); /* limpia grafo destino */

  DynBinHeap<typename GT::Arc*, Distance_Compare<GT, Distance, Compare> > heap;


  /* inicializar la ra�z con el primer nodo del grafo */
  typename GT::Node * first = g.get_first_node();
  NODE_BITS(first).set_bit(Aleph::Spanning_Tree, true); // marcarlo visitado

  // mapear los nodos en ambos grafos con el cookie
  NODE_COOKIE(first) = tree.insert_node(first->get_info());
  NODE_COOKIE(TREENODE(first)) = first;

  // meter en heap arcos iniciales del primer nodo
  for (typename GT::Node_Arc_Iterator it(first); it.has_current(); it.next())
    {
      typename GT::Arc * arc = it.get_current_arc();
      ARC_BITS(arc).set_bit(Aleph::Spanning_Tree, true);
      heap.insert(arc);
    }

  int i = 0, j = 0;

  while (tree.get_num_nodes() < g.get_num_nodes()) // mientras tree no abarque a g
    {     // obtenga siguiente menor arco 
      typename GT::Arc * min_arc = heap.getMin();

      typename GT::Node * src = g.get_src_node(min_arc);
      typename GT::Node * tgt = g.get_tgt_node(min_arc);

      if (IS_NODE_VISITED(src, Aleph::Spanning_Tree) and IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
	continue; // Este arco cerrar�a un ciclo en el �rbol 

      typename GT::Node * tgt_node = // seleccione nodo no marcado
	IS_NODE_VISITED(src, Aleph::Spanning_Tree) ? tgt : src;

      // inserte en �rbol, actualice mapeo entre nodos y maque nodo
      // como visitado
      typename GT::Node * tree_node = tree.insert_node(tgt_node->get_info());
      GT::map_nodes(tgt_node, tree_node);
      NODE_BITS(tgt_node).set_bit(Aleph::Spanning_Tree, true);

      // insertar arcos de tgt_node en el heap
      for (typename GT::Node_Arc_Iterator it(tgt_node); it.has_current(); it.next())
	{
	  typename GT::Arc * arc = it.get_current_arc();

	  if (IS_ARC_VISITED(arc, Aleph::Spanning_Tree))
	    continue;

	  ARC_BITS(arc).set_bit(Aleph::Spanning_Tree, true); // marcar arco

	  heap.insert(arc);
	}

      // inserte nuevo arco en tree
      typename GT::Arc * tree_arc = 
	tree.insert_arc(TREENODE(src), TREENODE(tgt), min_arc->get_info());

      // actualice mapeo entre arcos
      GT::map_arcs(min_arc, tree_arc);

      if (i++ % 3 == 0)
	{
	  string name = gnu::autosprintf("prim-%d.gra", j++);
	  ofstream out(name, ios::out);

	  generate_cross_spanning_tree
	    <Grafo, Nodo_String, Arco_String> (g, 6, 20, 20, out);
	}
    }

  string name = gnu::autosprintf("prim-%d.gra", j++);
  ofstream out(name, ios::out);

  generate_cross_spanning_tree
    <Grafo, Nodo_String, Arco_String> (g, 6, 20, 20, out);
}

    template <class GT, class Distance> inline 
void write_prim_min_spanning_tree(GT & g, GT & tree)
{
  write_prim_min_spanning_tree
    <GT, Distance, Aleph::less<typename Distance::Distance_Type> > (g, tree);
}

# undef TREENODE


int main()
{
  Grafo * g = build_graph();

  Grafo tree1;

  write_prim_min_spanning_tree<Grafo, Distancia>(*g, tree1);

  ofstream out("test.gra", ios::out);

  generate_cross_spanning_tree<Grafo, Nodo_String, Arco_String>(*g, 6, 20, 20, out);
}

