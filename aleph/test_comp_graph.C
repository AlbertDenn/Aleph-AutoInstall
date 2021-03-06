
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

# include <tpl_graph.H>
# include <tpl_components.H>
# include <tpl_cut_nodes.H>

# define INDENT "    "

using namespace Aleph;

struct Ciudad 
{
  enum Tipo_Ciudad { CAPITAL, CIUDAD, PUEBLO, CASERIO, CRUZ, DESCONOCIDO };

  string nombre;

  Tipo_Ciudad tipo;

  Ciudad() : tipo(DESCONOCIDO) { /* empty */ }

  Ciudad(const char * nom) : nombre(nom), tipo(DESCONOCIDO) { /* empty */ }

  Ciudad(char * nom) : nombre(nom), tipo(DESCONOCIDO) { /* empty */ }

  Ciudad(const string & str) : nombre(str), tipo(DESCONOCIDO) { /* empty */ }

  bool operator == (const Ciudad & c) const
  {
    return nombre == c.nombre;
  }
};

struct Via
{ 
  enum Tipo_Via { AUTOPISTA, CARRETERA1, CARRETERA2, CARRETERA3, GRANZON,
		  CHALANA, DESCONOCIDO };

  string nombre;
  int distancia; 
  Tipo_Via tipo;

  Via() : tipo(DESCONOCIDO) {}

  Via(int d) 
    : nombre("Desconocido"), distancia(d), tipo(DESCONOCIDO) { /* empty */ }

  Via(const string& nom, int d) 
    : nombre(nom), distancia(d), tipo(DESCONOCIDO) { /* empty */ }
};


typedef Graph_Node<Ciudad> Nodo_Ciudad;

typedef Graph_Arc<Via> Arco_Via;


typedef List_Graph<Nodo_Ciudad, Arco_Via> Mapa;

typedef List_Digraph<Nodo_Ciudad, Arco_Via> Dimapa;


struct Distancia_Via
{
  typedef int Distance_Type;

  static const Distance_Type Zero_Distance = 0;

  static const Distance_Type Max_Distance;

  Distance_Type operator () (Mapa::Arc * a) const
  {
    return a->get_info().distancia;
  }
};


const int Distancia_Via::Max_Distance = INT_MAX;


struct Ciudad_Igual
{
  bool operator () (const Ciudad & c1, const Ciudad & c2) const
  {
    return c1.nombre == c2.nombre;
  }

  bool operator () (const Ciudad & c1, const string & nom) const
  {
    return c1.nombre == nom;
  }
};


struct Arco_Igual
{
  bool operator () (const Via &, const Via &) const
  {
    return true;
  }
};


Mapa::Node * buscar_ciudad(Mapa & mapa, const string & nombre)
{
  return mapa.search_node <string, Ciudad_Igual> (nombre);
}

bool visitar(Mapa & g, Mapa::Node * node, Mapa::Arc * arc)
{
  cout << "Estoy en  " << node->get_info().nombre << " viniendo desde " 
       << (arc != NULL ? g.get_connected_node(arc, node)->get_info().nombre : 
	   "NULO") << endl ;

  return false;
}

struct Visitar
{
  Mapa::Node * dest;

  Visitar(Mapa::Node * tgt = NULL) : dest(tgt) { /* empty */ }

  bool operator () (Mapa & g, Mapa::Node * p, Mapa::Arc * a)
  {
    cout << "Estoy en  " << p->get_info().nombre << " viniendo desde " 
	 << (a != NULL ? g.get_connected_node(a, p)->get_info().nombre : 
	     "NULO") << endl ;

    if (dest != NULL)
      if (p == dest)
	{
	  cout << "Se alcanz� el nodo destino " << dest->get_info().nombre 
	       << endl;
	  return true;
	}

  return false;
  }
};

struct Contar_Arcos
{
  int count;

  Contar_Arcos() : count (0) { /* empty */ }

  bool operator () (Mapa::Arc *)
  {
    count++;
    return true;
  }
};


struct Total_Dist
{
  int dist;

  Total_Dist () : dist(0) { /* empty */ }

  bool operator () (Mapa::Arc * a)
  {
    if (a != NULL)
      dist += a->get_info().distancia;

    return true;
  }
};

void insert_via(Mapa& mapa, 
		const string & c1, const string & c2,
		int distancia)
{
  Mapa::Node * n1 = buscar_ciudad(mapa, c1);

  if (n1 == NULL)
    n1 = mapa.insert_node(c1);

  Mapa::Node * n2 = buscar_ciudad(mapa, c2);

  if (n2 == NULL)
    n2 = mapa.insert_node(c2);

  string nombre_arco = n1->get_info().nombre + "--" + n2->get_info().nombre;

  mapa.insert_arc(n1, n2, Via(nombre_arco, distancia));
}


typedef Path<Mapa> Camino;

void imprimir_camino(Camino & path)
{
  cout << endl
       << "Camino: ";

  for (Camino::Iterator itor(path); itor.has_current(); itor.next())
    cout << itor.get_current_node()->get_info().nombre << "-";

  cout << endl;
}


void imprimir_mapa(Mapa & g)
{
  cout << endl
       << "Listado de nodos (" << g.get_num_nodes() << ")" << endl;

  for (Mapa::Node_Iterator node_itor(g); node_itor.has_current(); 
       node_itor.next())
    cout << INDENT << node_itor.get_current_node()->get_info().nombre << endl;

  cout << endl
       << endl
       << "Listado de arcos (" << g.get_num_arcs() << ")"
       << endl;

  for (Mapa::Arc_Iterator arc_itor(g); arc_itor.has_current();
       arc_itor.next())
    {
      Mapa::Arc * arc = arc_itor.get_current_arc();
      cout << arc->get_info().nombre << " " << arc->get_info().distancia 
	   << " de " << g.get_src_node(arc)->get_info().nombre 
	   << " a " << g.get_tgt_node(arc)->get_info().nombre << endl;
    }

  cout << endl
       << endl
       << "Listado del grafo por nodos y en cada nodo por arcos" 
       << endl;
  for (Mapa::Node_Iterator node_itor(g); node_itor.has_current(); 
       node_itor.next())
    {
      Mapa::Node * src_node = node_itor.get_current_node();
      cout << src_node->get_info().nombre << endl;
      for (Mapa::Node_Arc_Iterator itor(node_itor.get_current_node());
	   itor.has_current(); itor.next())
	{
	  Mapa::Arc * arc = itor.get_current_arc();
	  cout << INDENT << arc->get_info().distancia << " " 
	       << g.get_connected_node(arc, src_node)->get_info().nombre 
	       << endl;
	}
    }
  cout << endl;
}

void construir_mapa(Mapa& g)
{
  insert_via(g, "San Cristobal", "La Fria", 69);
  insert_via(g, "San Cristobal", "Sacramento", 113);
  insert_via(g, "San Cristobal", "San Antonio", 36);
  insert_via(g, "San Cristobal", "Rubio", 22);
  insert_via(g, "Rubio", "San Antonio", 48);
  insert_via(g, "Rubio", "Caparo", 150);
  insert_via(g, "Sacramento", "El Canton", 38);
  insert_via(g, "La Fria", "El Vigia", 86);
  insert_via(g, "El Vigia", "Santa Barbara", 59);
  insert_via(g, "El Vigia", "Merida", 79);
  insert_via(g, "La Fria", "Machiques", 252);
  insert_via(g, "Machiques", "Maracaibo", 130);
  insert_via(g, "Machiques", "Santa Barbara", 295);
  insert_via(g, "Maracaibo", "Paraguaipos", 55);
  insert_via(g, "Maracaibo", "Coro", 254);
  insert_via(g, "Maracaibo", "Valera", 201);
  insert_via(g, "Valera", "Merida", 167);
  insert_via(g, "Valera", "Carora", 120);
  insert_via(g, "Carora", "Barquisimeto", 102);
  insert_via(g, "Merida", "Barinas", 180);
  insert_via(g, "Barinas", "Caparo", 200);
  insert_via(g, "Barinas", "Guanare", 94);
  insert_via(g, "Caracas", "Barcelona", 310);
  insert_via(g, "Caracas", "San Juan", 139);
  insert_via(g, "Guanare", "Barquisimeto", 170);
  insert_via(g, "Barquisimeto", "San Fernando", 526);
  insert_via(g, "Barinas", "San Fernando", 547);
  insert_via(g, "Caparo", "San Cristobal", 201);
  insert_via(g, "Coro", "Valencia", 252);
  insert_via(g, "Valencia", "Barquisimeto", 220);
  insert_via(g, "Valencia", "Maracay", 49);
  insert_via(g, "Valencia", "San Carlos", 100);
  insert_via(g, "Maracay", "Caracas", 109);
  insert_via(g, "San Felipe", "Maracay", 315);
  insert_via(g, "San Felipe", "San Carlos", 241);
  insert_via(g, "San Felipe", "Barquisimeto", 86);
  insert_via(g, "San Felipe", "San Juan", 222);
  insert_via(g, "Guanare", "San Carlos", 173);
  insert_via(g, "San Juan", "San Fernando", 261);
  insert_via(g, "Barcelona", "Pto La Cruz", 10);
  insert_via(g, "Pto La Cruz", "Cumana", 82);
  insert_via(g, "Cumana", "Maturin", 199);
  insert_via(g, "Pto Ordaz", "Maturin", 171);
  insert_via(g, "Pto Ordaz", "Ciudad Bolivar", 107);
  insert_via(g, "El Tigre", "Ciudad Bolivar", 130);
  insert_via(g, "El Tigre", "Barcelona", 166);
  insert_via(g, "El Tigre", "San Juan", 435);
}


int main()
{
  Mapa g;

  construir_mapa(g);

  DynDlist<Mapa::Node*> cut_nodes;
  Compute_Cut_Nodes<Mapa> cut(g);
  cut(cut_nodes);
  DynDlist<Mapa::Arc*> cross_arcs;
  DynDlist<Mapa> bloques;
  Mapa cut_g;
  cut.compute_blocks(bloques, cut_g, cross_arcs);

 
  int i = 0;
  cout << "Subgrafos en torno a los puntos de corte" << endl;
  for (DynDlist<Mapa>::Iterator it(bloques); it.has_curr(); it.next(), i++)
    {
      Mapa & m = it.get_curr();
      cout << "Bloque " << i << endl;
      imprimir_mapa(m);
    }
  cout << "Un total de " << bloques.size() << " bloques" << endl;

  cout << "Eliminando los nodos de corte ..." << endl;
  for (DynDlist<Mapa::Node*>::Iterator it(cut_nodes); it.has_curr(); it.next())
    {
      Mapa::Node * p = it.get_curr();
      cout << p->get_info().nombre << " es nodo de corte" << endl;
      g.remove_node(p);
    }
  cout << endl;

  DynDlist<DynDlist<Mapa::Node*> > list;

  Inconnected_Components<Mapa> () (g, list);

  cout << "Imprimiendo bloques inconexos" << endl;
  i = 0;
  for (DynDlist<DynDlist<Mapa::Node*> >::Iterator it(list); it.has_curr();
       it.next(), ++i)
    {
      DynDlist<Mapa::Node*> & l = it.get_curr();
      cout << "Lista nodos bloque " << i << " (" << l.size() << ")" << endl;
      for (DynDlist<Mapa::Node*>::Iterator i(l); i.has_curr(); i.next())
	cout << i.get_curr()->get_info().nombre << " ";
      cout << endl;
    }
}
