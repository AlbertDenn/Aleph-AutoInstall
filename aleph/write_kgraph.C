
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

# include <iostream>
# include <autosprintf.h>
# include <tpl_graph_utils.H>
# include <generate_graph.H>
# include <tpl_kgraph.H>

using namespace Aleph;
using namespace std;

typedef List_Graph<Graph_Node<int>, Graph_Arc<Empty_Class> > Grafo;

void crear_arco(Grafo & g, int isrc, int itgt)
{
  Grafo::Node * src = g.search_node(isrc);
  if (src == NULL)
    src = g.insert_node(isrc);

  Grafo::Node * tgt = g.search_node(itgt);
  if (tgt == NULL)
    tgt = g.insert_node(itgt);

  if (search_arc<Grafo>(g, src, tgt) != NULL)
    throw std::invalid_argument("Duplicated arc");

  g.insert_arc(src, tgt);
}

void crear_grafo(Grafo & g)
{
  g.insert_node(1);
  g.insert_node(2);
  g.insert_node(3);
  g.insert_node(4);
  g.insert_node(5);
  g.insert_node(6);
  g.insert_node(7);
  g.insert_node(8);
  g.insert_node(9);
  g.insert_node(10);
  g.insert_node(11);
  g.insert_node(12);
  g.insert_node(13);
  g.insert_node(14);
  g.insert_node(15);
  g.insert_node(16);
  g.insert_node(17);
  g.insert_node(18);
  g.insert_node(19);
  g.insert_node(20);
  g.insert_node(21);
  g.insert_node(22);
  g.insert_node(23);
  g.insert_node(24);
  g.insert_node(25);
  g.insert_node(26);
  g.insert_node(27);

  crear_arco(g, 17, 6);
  crear_arco(g, 17, 12);
  crear_arco(g, 17, 18);
  crear_arco(g, 17, 23);
  crear_arco(g, 23, 12);
  crear_arco(g, 23, 18);
  crear_arco(g, 23, 24);
  crear_arco(g, 12, 6);
  crear_arco(g, 12, 7);
  crear_arco(g, 18, 12);
  crear_arco(g, 24, 18);
  crear_arco(g, 24, 19);
  crear_arco(g, 24, 25);
  crear_arco(g, 25, 18);
  crear_arco(g, 25, 12);
  crear_arco(g, 25, 19);
  crear_arco(g, 26, 15);
  crear_arco(g, 19, 12);
  crear_arco(g, 19, 13);
  crear_arco(g, 13, 12);
  crear_arco(g, 13, 6);
  crear_arco(g, 13, 7);
  crear_arco(g, 7, 6);
  crear_arco(g, 7, 1);
  crear_arco(g, 13, 8);
  crear_arco(g, 8, 7);
  crear_arco(g, 8, 1);
  crear_arco(g, 8,2 );
  crear_arco(g, 8, 3);
  crear_arco(g, 8, 9);
  crear_arco(g, 9, 4);
  crear_arco(g, 14, 13);
  crear_arco(g, 14, 8);
  crear_arco(g, 14, 3);
  crear_arco(g, 14, 19);
  crear_arco(g, 14, 25);
  crear_arco(g, 14, 20);
  crear_arco(g, 14, 15);
  crear_arco(g, 14, 9);
  crear_arco(g, 9, 3);
  crear_arco(g, 15, 9);
  crear_arco(g, 20, 26 );
  crear_arco(g, 20, 25);
  crear_arco(g, 20, 27);
  crear_arco(g, 20, 21);
  crear_arco(g, 20, 15);
  crear_arco(g, 15, 21);
  crear_arco(g, 21, 27);
  crear_arco(g, 21, 16);
  crear_arco(g, 21, 22);
  crear_arco(g, 15, 16);
  crear_arco(g, 15, 10);
  crear_arco(g, 10, 9);
  crear_arco(g, 10, 4);
  crear_arco(g, 10, 5);
  crear_arco(g, 10, 11);
  crear_arco(g, 10, 16);
  crear_arco(g, 16, 11);
  crear_arco(g, 16, 22);
  crear_arco(g, 16, 27);
  crear_arco(g, 27, 22);
  crear_arco(g, 22, 11);
  crear_arco(g, 11, 5);
  crear_arco(g, 5, 4);
  crear_arco(g, 5, 9);
  crear_arco(g, 4, 3);
  crear_arco(g, 3, 2);
  crear_arco(g, 2, 1);
  crear_arco(g, 2, 13);
  crear_arco(g, 2, 15);
  crear_arco(g, 2, 19);
  crear_arco(g, 2, 7);
  crear_arco(g, 1, 6);
  crear_arco(g, 25, 26);
  crear_arco(g, 26, 27);
  crear_arco(g, 26, 21);
}


void crear_otro_grafo(Grafo & g)
{
  g.insert_node(1);
  g.insert_node(2);
  g.insert_node(3);
  g.insert_node(4);
  g.insert_node(5);
  g.insert_node(6);
  g.insert_node(7);
  g.insert_node(8);
  g.insert_node(9);
  g.insert_node(10);
  g.insert_node(11);
  g.insert_node(12);
  g.insert_node(13);
  g.insert_node(14);
  g.insert_node(15);
  g.insert_node(16);
  g.insert_node(17);
  g.insert_node(18);
  g.insert_node(19);
  g.insert_node(20);
  g.insert_node(21);
  g.insert_node(22);
  g.insert_node(23);
  g.insert_node(24);
  g.insert_node(25);
  g.insert_node(26);
  g.insert_node(27);

  crear_arco(g, 17, 6);
  crear_arco(g, 17, 12);
  crear_arco(g, 17, 18);
  crear_arco(g, 17, 23);
  crear_arco(g, 23, 12);
  crear_arco(g, 23, 18);
  crear_arco(g, 23, 24);
  crear_arco(g, 12, 6);
  crear_arco(g, 12, 7);
  crear_arco(g, 18, 12);
  crear_arco(g, 24, 18);
  crear_arco(g, 24, 19);
  crear_arco(g, 24, 25);
  crear_arco(g, 25, 18);
  crear_arco(g, 25, 12);
  crear_arco(g, 25, 19);
  crear_arco(g, 26, 15);
  crear_arco(g, 19, 12);
  crear_arco(g, 19, 13);
  crear_arco(g, 13, 12);
  crear_arco(g, 13, 6);
  crear_arco(g, 13, 7);
  crear_arco(g, 7, 6);
  //  crear_arco(g, 7, 8); // ojo
  crear_arco(g, 7, 1);
  crear_arco(g, 7, 2);
  crear_arco(g, 13, 8);
  crear_arco(g, 8, 1);
  crear_arco(g, 8, 2);
  crear_arco(g, 8, 3);

  crear_arco(g, 9, 4);

  crear_arco(g, 14, 19);

  crear_arco(g, 14, 20);
  crear_arco(g, 14, 15);
  crear_arco(g, 14, 9);
  crear_arco(g, 9, 3);
  crear_arco(g, 15, 9);
  crear_arco(g, 20, 26 );

  crear_arco(g, 20, 27);
  crear_arco(g, 20, 21);
  crear_arco(g, 20, 15);
  crear_arco(g, 15, 21);
  crear_arco(g, 21, 27);
  crear_arco(g, 21, 16);
  crear_arco(g, 21, 22);
  crear_arco(g, 15, 16);
  crear_arco(g, 15, 10);
  crear_arco(g, 10, 9);
  crear_arco(g, 10, 4);
  crear_arco(g, 10, 5);
  crear_arco(g, 10, 11);
  crear_arco(g, 10, 16);
  crear_arco(g, 16, 11);
  crear_arco(g, 16, 22);
  crear_arco(g, 16, 27);
  crear_arco(g, 27, 22);
  crear_arco(g, 22, 11);
  crear_arco(g, 11, 5);
  crear_arco(g, 5, 4);
  crear_arco(g, 5, 9);
  crear_arco(g, 4, 3);
  crear_arco(g, 3, 2);
  crear_arco(g, 2, 1);
  crear_arco(g, 2, 13);
  crear_arco(g, 2, 19);
  crear_arco(g, 1, 6);

  crear_arco(g, 26, 27);
  crear_arco(g, 26, 21);
}

void escribir_grafo(Grafo & g)
{
  for (Node_Iterator<Grafo> it(g); it.has_current(); it.next())
    {
      Grafo::Node * p = it.get_current();
      cout << p->get_info() << endl;
      for (Node_Arc_Iterator<Grafo> i(p); i.has_current(); i.next())
	cout << "    " << i.get_tgt_node()->get_info() << endl;
    }
}


DynDlist<Grafo::Arc*> * ptr_cut = NULL;


struct Sombra_Nodo
{
  string operator () (Grafo::Node * p) const
  {
     if (ptr_cut == NULL) 
       return "";

     for (DynDlist<Grafo::Arc*>::Iterator it(*ptr_cut); 
	  it.has_current(); it.next())
       {
	 Grafo::Arc * arc = it.get_current();
	 if (arc->src_node == p or arc->tgt_node == p)
	   return "shadow-node";
       }
     return "";
  }
};



struct Sombra_Arco
{
  string operator () (Grafo::Arc * a) const
  {
    if (ptr_cut == NULL) 
      return "ARC";
    return sequential_search(*ptr_cut, a) == NULL ? "Arc" : "Shadow-arc";
  }
};

struct Nodo_String
{
  string operator () (Grafo::Node * p) const 
  {
    //    return gnu::autosprintf("%d", p->get_info()); 
    return "";  
  }
};


struct Arco_Normal
{
  string operator () (Grafo::Arc * a) const
  { 
    return "";
  }
};

int main()
{

  {
    Grafo g;
    crear_grafo(g);

    Aleph::set<Grafo::Node*> l, r;
    DynDlist<Grafo::Arc*> cut;

    int k = Compute_Min_Cut <Grafo, Edmonds_Karp_Maximum_Flow> () (g, l, r, cut);

    cout << "Ke(G) = " << k << endl;

    cout << "l=" ;
    for (Aleph::set<Grafo::Node*>::iterator it = l.begin(); it != l.end(); ++it)
      cout << (*it)->get_info() << " ";
    cout << endl;

    cout << "r=" ;
    for (Aleph::set<Grafo::Node*>::iterator it = r.begin(); it != r.end(); ++it)
      cout << (*it)->get_info() << " ";
    cout << endl;

    ptr_cut = &cut;

    ofstream out1("kmin-1.mf", ios::out);
    generate_cross_graph
	<Grafo, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco>
	(g, 6, 100, 100, out1); 
  }

  {
    Grafo g;
    crear_otro_grafo(g);

    Aleph::set<Grafo::Node*> l, r;
    DynDlist<Grafo::Arc*> cut;

    int k = Compute_Min_Cut <Grafo, Edmonds_Karp_Maximum_Flow> () (g, l, r, cut);
    cout << "Ke(G) = " << k << endl;

    cout << "l=" ;
    for (Aleph::set<Grafo::Node*>::iterator it = l.begin(); it != l.end(); ++it)
      cout << (*it)->get_info() << " ";
    cout << endl;

    cout << "r=" ;
    for (Aleph::set<Grafo::Node*>::iterator it = r.begin(); it != r.end(); ++it)
      cout << (*it)->get_info() << " ";
    cout << endl;

    ptr_cut = &cut;

    ofstream out1("kmin-2.mf", ios::out);
    generate_cross_graph
	<Grafo, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco>
	(g, 6, 100, 100, out1); 
  }

}
