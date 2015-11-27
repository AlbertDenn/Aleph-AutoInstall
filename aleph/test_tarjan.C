
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
# include <Tarjan.H>


typedef Graph_Node<string> Node_Nodo;

typedef Graph_Arc<Empty_Class> Arco_Arco;

typedef List_Graph<Node_Nodo, Arco_Arco> Digrafo;


void insertar_arco(Digrafo &        grafo, 
		   const string & src_name, 
		   const string & tgt_name)
{
  Digrafo::Node * n1 = grafo.find_node(src_name);

  if (n1 == NULL)
    n1 = grafo.insert_node(src_name);

  Digrafo::Node * n2 = grafo.find_node(tgt_name);

  if (n2 == NULL)
    n2 = grafo.insert_node(tgt_name);

  grafo.insert_arc(n1, n2);
}

void build_test_graph_1(Digrafo & g)
{
  g.insert_node("E");

  insertar_arco(g, "A", "B");
  insertar_arco(g, "C", "D");
  insertar_arco(g, "D", "E");
  insertar_arco(g, "B", "E");

  insertar_arco(g, "E", "G");

  insertar_arco(g, "G", "F");
  insertar_arco(g, "F", "J");

  insertar_arco(g, "E", "H");

  insertar_arco(g, "H", "I");

  insertar_arco(g, "I", "J");
  insertar_arco(g, "I", "L");
  insertar_arco(g, "L", "K");

  insertar_arco(g, "K", "O");
  insertar_arco(g, "K", "M");
  insertar_arco(g, "O", "N");
}

void build_test_graph_3(Digrafo & g)
{
  g.insert_node("E");

  insertar_arco(g, "A", "B");
  insertar_arco(g, "A", "D");
  insertar_arco(g, "B", "C");
  insertar_arco(g, "C", "A");
  insertar_arco(g, "D", "E");
  insertar_arco(g, "E", "B");
  insertar_arco(g, "D", "C");

  //  insertar_arco(g, "E", "G");

  insertar_arco(g, "G", "F");
  insertar_arco(g, "F", "G");

  insertar_arco(g, "E", "H");

  insertar_arco(g, "H", "I");

  insertar_arco(g, "I", "J");
  insertar_arco(g, "J", "K");
  insertar_arco(g, "K", "I");
  insertar_arco(g, "K", "L");
  insertar_arco(g, "L", "I");

  insertar_arco(g, "K", "O");
  insertar_arco(g, "O", "N");
  insertar_arco(g, "N", "M");
  insertar_arco(g, "O", "N");
  insertar_arco(g, "M", "O");
}

void build_test_graph_2(Digrafo & g)
{
  g.insert_node("E");

  insertar_arco(g, "A", "B");
  insertar_arco(g, "A", "D");
  insertar_arco(g, "B", "C");
  insertar_arco(g, "C", "A");
  insertar_arco(g, "D", "E");
  insertar_arco(g, "E", "B");
  insertar_arco(g, "D", "C");

  insertar_arco(g, "E", "G");

  insertar_arco(g, "G", "F");
  insertar_arco(g, "F", "G");

  insertar_arco(g, "E", "H");

  insertar_arco(g, "H", "I");

  insertar_arco(g, "I", "J");
  insertar_arco(g, "J", "K");
  insertar_arco(g, "K", "I");
  insertar_arco(g, "K", "L");
  insertar_arco(g, "L", "I");

  insertar_arco(g, "K", "O");
  insertar_arco(g, "O", "N");
  insertar_arco(g, "N", "M");
  insertar_arco(g, "O", "N");
  insertar_arco(g, "M", "O");
}

# define INDENT "    "

void print_graph(Digrafo & g)
{
  cout << "Grafo (" << g.vsize() << " nodos)" << endl;
  for (Digrafo::Node_Iterator node_it(g); node_it.has_curr(); node_it.next())
    {
      Digrafo::Node * src = node_it.get_current_node();
      cout << INDENT << "Nodo " << src->get_info() << endl
	   << INDENT << "Arcos:" << endl;
      for (Digrafo::Node_Arc_Iterator it(src); it.has_curr(); it.next())
	{
	  Digrafo::Node * tgt = it.get_tgt_node();
	  cout << INDENT << INDENT << " --> " << tgt->get_info() << endl;
	}
    }
  cout << endl;
}


int main()
{
  {
    Digrafo g;

    build_test_graph_2(g);

    DynDlist<Digrafo> list;
    DynDlist<Digrafo::Arc*> arc_list;

    Tarjan_Connected_Components<Digrafo> () (g, list, arc_list);

    DynDlist<DynDlist<Digrafo::Node*> > nodes;

    Tarjan_Connected_Components<Digrafo> () (g, nodes);

    for (DynDlist<DynDlist<Digrafo::Node*> >::Iterator it(nodes); it.has_curr();
	 it.next())
      {
	DynDlist<Digrafo::Node*> & list = it.get_current();
	cout << "Bloque: ";
	for (DynDlist<Digrafo::Node*>::Iterator i(list); i.has_curr(); i.next())
	  cout << i.get_current()->get_info() << " ";
	cout << endl;
      }

    if ((Tarjan_Connected_Components<Digrafo> ()) . has_cycle(g))
      cout << "El digrafo tiene ciclos" << endl;
    else
      cout << "El digrafo no tiene ciclos" << endl;
  }

  {
    Digrafo g;
    build_test_graph_2(g);

    DynDlist<Digrafo> bloques;
    DynDlist<Digrafo::Arc*> arcos;
    
    Tarjan_Connected_Components<Digrafo> () (g, bloques, arcos);

    for (DynDlist<Digrafo>::Iterator it(bloques); it.has_curr(); it.next())
      print_graph(it.get_curr());

    if ((Tarjan_Connected_Components<Digrafo> ()). test_connectivity (g))
      cout << "El digrafo es conexo" << endl;
    else
      cout << "El digrafo no es conexo" << endl;
  }

  cout << "****************" << endl;

  {
    Digrafo g;
    build_test_graph_1(g);

    DynDlist<Digrafo> bloques;
    DynDlist<Digrafo::Arc*> arcos;
    
    Tarjan_Connected_Components<Digrafo> () (g, bloques, arcos);

    for (DynDlist<Digrafo>::Iterator it(bloques); it.has_curr(); it.next())
      print_graph(it.get_curr());

    if ((Tarjan_Connected_Components<Digrafo> ()) . has_cycle(g))
      cout << "El digrafo tiene ciclos" << endl;
    else
      cout << "El digrafo no tiene ciclos" << endl;

    if ((Tarjan_Connected_Components<Digrafo> ()) . is_dag(g))
      cout << "El digrafo es aciclico" << endl;
    else
      cout << "El digrafo no tiene ciclos" << endl;
  }

  {
    Digrafo g;
    build_test_graph_2(g);

    Path<Digrafo> p;

    (Tarjan_Connected_Components<Digrafo> ()).compute_cycle (g, p);

    for (Path<Digrafo>::Iterator it(p); it.has_curr(); it.next())
      cout << it.get_current_node()->get_info() << " --> ";
    cout << endl;

    if ((Tarjan_Connected_Components<Digrafo> ()). test_connectivity (g))
      cout << "El digrafo es conexo" << endl;
    else
      cout << "El digrafo no es conexo" << endl;
  }

  {
    Digrafo g;
    build_test_graph_3(g);

    Path<Digrafo> p;

    if ((Tarjan_Connected_Components<Digrafo> ()). test_connectivity (g))
      cout << "El digrafo es conexo" << endl;
    else
      cout << "El digrafo no es conexo" << endl;
  }

}
