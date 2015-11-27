
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
# include <limits>
# include <Bellman_Ford.H>


using namespace std;

using namespace Aleph;

    template <class GT>
void print_pred(GT & g, DynArray<typename GT::Node*> & pred)
{
  for (Node_Iterator<GT> it(g); it.has_current(); it.next())
    cout << it.get_current()->get_info() << " ";
  cout << endl;

  const unsigned int & n = pred.size();

  for (int i = 0; i < n; ++i)
    {
      typename GT::Node * p = pred[i];
      cout << (p != NULL ? p->get_info() : "-");
      //      if (arcs[i] == NULL)
      //	cout << " (" << i << ") ";
      cout << " ";
    }
  cout << endl;
} 

# define INDENT "    "

typedef List_Digraph<Graph_Node<string>, Graph_Arc<double>> Grafo;

struct Distancia
{
  typedef double Distance_Type;

  Distance_Type & operator () (Grafo::Arc * a) const
  {
    return a->get_info();
  }

  static void set_zero(Grafo::Arc * a) { a->get_info() = 0; }
};

void insertar_arco(Grafo &       grafo, 
		   const string & src_name, 
		   const string & tgt_name,
		   const double & distancia)
{
  auto n1 = grafo.search_node([&src_name] (auto p) 
			      { return p->get_info() == src_name; });
  if (n1 == NULL)
    n1 = grafo.insert_node(src_name);

  auto n2 = grafo.search_node([&tgt_name] (auto p)
			      { return p->get_info() == tgt_name; });
  if (n2 == NULL)
    n2 = grafo.insert_node(tgt_name);

  grafo.insert_arc(n1, n2, distancia);
}

Grafo build_test_graph_1()
{
  Grafo g;

  g.insert_node(string("A"));
  g.insert_node(string("B"));
  g.insert_node(string("C"));
  g.insert_node(string("D"));
  g.insert_node(string("E"));
  g.insert_node(string("F"));
  g.insert_node(string("G"));
  g.insert_node(string("H"));
  g.insert_node(string("I"));

  insertar_arco(g, "A", "B", 2);
  insertar_arco(g, "A", "F", 5);
  insertar_arco(g, "B", "F", 1);
  insertar_arco(g, "B", "D", 3);
  insertar_arco(g, "C", "A", 1);  // -11 para hacer ciclo negativo
  insertar_arco(g, "C", "E", 4);  
  insertar_arco(g, "F", "D", -1);
  insertar_arco(g, "F", "C", -1);
  insertar_arco(g, "F", "E", 1);
  insertar_arco(g, "D", "F", 3);
  insertar_arco(g, "D", "H", 4); 
  insertar_arco(g, "E", "G", 2);
  insertar_arco(g, "E", "I", -2);
  insertar_arco(g, "G", "D", 3);
  insertar_arco(g, "G", "F", -1);
  insertar_arco(g, "G", "H", 2);
  insertar_arco(g, "H", "D", -2);
  insertar_arco(g, "H", "G", -1);
  insertar_arco(g, "I", "G", 2);
  insertar_arco(g, "G", "I", 3);

  return g;
}

Grafo build_test_graph_2()
{
  Grafo g;
  g.insert_node(string("A"));
  g.insert_node(string("B"));
  g.insert_node(string("C"));
  g.insert_node(string("D"));
  g.insert_node(string("E"));
  g.insert_node(string("F"));
  g.insert_node(string("G"));
  g.insert_node(string("H"));
  g.insert_node(string("I"));

  insertar_arco(g, "A", "B", 2);
  insertar_arco(g, "A", "F", 5);
  insertar_arco(g, "B", "F", 1);
  insertar_arco(g, "B", "D", 3);
  insertar_arco(g, "C", "A", 1);  
  insertar_arco(g, "C", "E", 4);  
  insertar_arco(g, "F", "D", -2);
  insertar_arco(g, "F", "C", -18);
  insertar_arco(g, "F", "E", 5);
  insertar_arco(g, "D", "F", 3);
  insertar_arco(g, "D", "H", 4); 
  insertar_arco(g, "E", "G", 2);
  insertar_arco(g, "E", "I", -2);
  insertar_arco(g, "G", "D", 3);
  insertar_arco(g, "G", "F", -1);
  insertar_arco(g, "G", "H", 2);
  insertar_arco(g, "H", "D", -2);
  insertar_arco(g, "H", "G", -1);
  insertar_arco(g, "I", "G", 2);
  insertar_arco(g, "I", "H", 3);

  return g;
}

typedef Path<Grafo> Camino;

void imprimir_camino(Camino & path)
{
  cout << endl
       << "Camino: ";

  if (path.is_empty())
    {
      cout << "vacio" << endl;
      return;
    }

  cout << path.get_first_node()->get_info();
  for (Camino::Iterator itor(path); itor.has_current_arc(); itor.next())
    {
      Grafo::Arc * a = itor.get_current_arc();
      Grafo::Node * tgt = (Grafo::Node*) a->tgt_node;
      cout << " " << a->get_info() << " --> " 
	   << tgt->get_info();
    }
  cout << endl;
}

void imprimir_arbol(Grafo & g)
{
  for (Grafo::Node_Iterator i(g); i.has_current(); i.next())
    for (Grafo::Node_Arc_Iterator j(i.get_current()); j.has_current(); j.next())
      {
	Grafo::Arc * a = j.get_current();
	cout << g.get_src_node(a)->get_info() << " " 
	     << a->get_info() << " -->" 
	     << g.get_tgt_node(a)->get_info() << endl;
      }
  cout << endl;
}

void test(const Grafo & g)
{
  Bellman_Ford<Grafo, Distancia> bf(g);

  cout << "Normal painting " << endl;
  bool has_neg_cycle = bf.paint_spanning_tree(g.get_first_node());
  if (has_neg_cycle)
    {
      cout << "El grafo tiene un ciclo negativo " << endl;
      Path<Grafo> cycle =  bf.test_negative_cycle(g.get_first_node());
      imprimir_camino(cycle);


      cout << "Verificando ciclo por algoritmo general" << endl;
      cycle = bf.test_negative_cycle();
      if (cycle.is_empty())
	ERROR("No se encontro el ciclo");
      else
	imprimir_camino(cycle);

      cout << "Verificando ciclo negativo por algoritmo basado en cola" << endl;
      tuple<Path<Grafo>, size_t> c =
	bf.search_negative_cycle(g.get_first_node(), 0.7);
      if (get<0>(c).is_empty())
	ERROR("No se encontro el ciclo");
      else
	imprimir_camino(get<0>(c));

      cout << "Todo OK" << endl;
    }
  else
    {
      Grafo tree;
      bf.build_tree(tree);
      cout << endl
	   << "Arbol abarcador segun Bellman-Ford" << endl;
      imprimir_arbol(tree);
    }

  cout << endl
       << "Fast painting " << endl;
  has_neg_cycle = bf.faster_paint_spanning_tree(g.get_first_node());
  if (has_neg_cycle)
    {
      cout << "El grafo tiene un ciclo negativo " << endl
	   << "Buscando ciclo con test_negative_cycle()" << endl;
      Path<Grafo> cycle =  bf.test_negative_cycle(g.get_first_node());
      imprimir_camino(cycle);

      cout << "Buscando ciclo con search_negative_cycle()" << endl;
      tuple<Path<Grafo>, size_t> c = 
	bf.search_negative_cycle(g.get_first_node());
      imprimir_camino(get<0>(c));
    }
  else
    {
      Grafo tree;
      bf.build_tree(tree);
      cout << endl
	   << "Arbol abarcador segun Bellman-Ford" << endl;
      imprimir_arbol(tree);
    }
}

int main()
{
  {
    Grafo g = build_test_graph_2();
    test(g);
  }

  cout << "========================================================" << endl;

  {
    Grafo g = build_test_graph_1();
    test(g);
  }
}
