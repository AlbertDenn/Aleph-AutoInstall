
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

# include <string.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include <tpl_dynBinHeap.H>
#include <tpl_agent.H>
#include <tpl_graph_utils.H>
#include <tpl_dynDlist.H>

using namespace std;

const string nodos[] = { 
  "Las Tapias", "Humboldt", "Pie del Llano", "Casa Blanca", "Alto Prado", 
  "Mercado", "Medicina", "Santa Juana", "Glorias Patrias", "Santo Ni�o", 
  "Viaducto", "Santa Barbara", "Mc Donalds" }; 

struct Agent_Info
{
  int id;
  string descripcion;

  Agent_Info(int id, string desc) : id(id), descripcion(desc) { /* Empty */ }

  bool operator == (const Agent_Info & otroAgente) const 
  {
    return id == otroAgente.id;
  }

  Agent_Info() { /* empty */ }
};

struct Nodo_Mapa
{
  string descripcion;

  size_t count;

  Nodo_Mapa (string desc) : descripcion(desc), count(0) { /* Empty */ }

  bool operator == (const Nodo_Mapa & n) const
  {
    return strcasecmp(n.descripcion.c_str(), descripcion.c_str()) == 0;
  }
};

struct Arco_Mapa
{
  string descripcion;
  int delay;

  Arco_Mapa () { }

  Arco_Mapa (string desc, int delay) 
  : descripcion(desc), delay(delay) 
  { /* Empty */ }
};

typedef Agent_Node<Nodo_Mapa> Nodo;
typedef Agent_Arc<Arco_Mapa> Arco;

typedef Agent_Graph<Nodo, Arco, Agent_Info> Grafo_Agentes;

Grafo_Agentes * mundo; 

  
Grafo_Agentes::Arc * dejar_nodo(Grafo_Agentes *                     graph,
				Grafo_Agentes::Node *               curr, 
				Grafo_Agentes::Node_to_Node_Agent * agent)
{
  cout << agent->get_info().descripcion << " esta partiendo de nodo " 
       << curr->get_info().descripcion << endl;

      // Decidir proximo paso
  
  const size_t & num_arcs = graph->get_num_arcs(curr);

//   const size_t & num_arcs = graph->List_Graph<Grafo_Agentes::Node,
//     Grafo_Agentes::Arc>::get_num_arcs(curr);

  int prox_arc_idx = rand() % num_arcs;
  int num_arc = 0;
  Grafo_Agentes::Arc * selected_arc;
  for (Grafo_Agentes::Node_Arc_Iterator it(curr); it.has_current(); it.next())
    if (num_arc == prox_arc_idx)
      {
	selected_arc = it.get_current_arc();
	break;
      } 
    else
      num_arc++;

  cout << agent->get_info().descripcion << " seleccion� arco " 
       << selected_arc->get_info().descripcion << endl;

  return selected_arc;
}


bool entrar_nodo(Grafo_Agentes *                     graph,
		 Grafo_Agentes::Node *               p, 
		 Grafo_Agentes::Node_to_Node_Agent * agent)
{
  return not ((p->get_info().count++) == 1000);
}


void create_street(string src_node_desc, 
		   string dest_node_desc, int delay, string street_name) 
{
  Nodo_Mapa info1(src_node_desc);

  Grafo_Agentes::Node * n1 = mundo->search_node(info1);
  if (n1 == NULL) 
    n1 = mundo->insert_node(info1);

  Nodo_Mapa info2(dest_node_desc);
  Grafo_Agentes::Node * n2 = mundo->search_node(info2);

  if (n2 == NULL) 
    n2 = mundo->insert_node(info2);

  Arco_Mapa info_arc(street_name, delay);

  cout << "Creada calle " << info_arc.descripcion << " desde " 
       << info1.descripcion  << " hasta " << info2.descripcion << endl;
  mundo->insert_arc(n1, n2, info_arc);
}


void configure_agent_graph()
{
  // Crear grafo
  create_street("Las Tapias", "Humboldt", 5, "Albarregas");
  create_street("Las Tapias", "Pie del Llano", 10, "Av. Andres Bello");
  create_street("Humboldt", "Santo Niño", 6, "Av. Humboldt");
  create_street("Humboldt", "Santa Barbara", 11, "Las Americas");
  create_street("Santo Ni�o", "Casa Blanca", 20, "Panamericana");
  create_street("Santa Barbara", "Mc Donalds", 8, "Las Americas");
  create_street("Pie del Llano", "Santa Barbara", 7, "Sucre");
  create_street("Pie del Llano", "Santa Juana", 5, "Av. Ppal Santa Juana");
  create_street("Santa Juana", "Medicina", 16, "16 de Septiembre");
  create_street("Medicina", "Glorias Patrias", 4, "Don Tulio");
  create_street("Pie del Llano", "Glorias Patrias", 15, "Av. Urdaneta");
  create_street("Glorias Patrias", "Mercado", 6, "Miranda");
  create_street("Mc Donalds", "Mercado", 7, "Las Americas");
  create_street("Mc Donalds", "Casa Blanca", 3, "El Rodeo");
  create_street("Casa Blanca", "Alto Prado", 14, "Los Proceres");
  create_street("Mercado", "Viaducto", 7, "Las Americas");
  create_street("Viaducto", "Alto Prado", 6, "Campo Elias");
}


void create_agents_in_graph()
{
  cout << "Creando los agentes en el Grafo" << endl;

  for (int i = 1; i <= 5; i++) 
    {
      stringstream agent_name_strm(stringstream::out) ;
      agent_name_strm << "Agente " << i;
      Agent_Info a(i, agent_name_strm.str());  	
      int index = rand() % 13;

      Grafo_Agentes::Node * n1 = mundo->search_node(Nodo_Mapa(nodos[index]));

      Grafo_Agentes::Node_to_Node_Agent * agent =
	new Grafo_Agentes::Node_to_Node_Agent(a, dejar_nodo, entrar_nodo);

      mundo->insert_agent_in_node(agent, n1);
    }
}

void print_agents_status()
{
  for (int i = 1; i <= 30; i++) 
    {
      Agent_Info a(i, "");

      Grafo_Agentes::Agent * agent = mundo->search_agent(a);

      if (agent != NULL) 
	if (mundo->is_agent_in_node(agent))
	  {
	    Grafo_Agentes::Node * n1 = mundo->get_agent_node_location(agent);
	      
	    // Imprimir donde se encuentra
	    cout << agent->get_info().descripcion << " se encuentra en nodo " 
		 << n1->get_info().descripcion << endl;
	  }
	else 
	  {
	    Grafo_Agentes::Arc * a1 = mundo->get_agent_arc_location(agent);

	    // Imprimir donde se encuentra
	    cout << agent->get_info().descripcion 
		 << " se encuentra en arco " << a1->get_info().descripcion 
		 << endl;
	  }
    }
}

int main()
{
  srand((unsigned)time(0));
  mundo = new Grafo_Agentes(5);
  configure_agent_graph();
  create_agents_in_graph();
 
  // Comenzar la ejecución
  mundo->start_graph();
  
  
  // Permitir que se ejecute por 10 segundos
  sleep(13);
  // Pausar la ejecucion
  mundo->suspend_graph();
  // Mostrar el estado actual de los agentes
  print_agents_status();
  cout << "Presione cualquier tecla para continuar...";
  //  cin.get();
  // Reanudar la ejecución
  mundo->resume_graph();
  // Dormir otros 10 seg
  sleep(13);
  mundo->stop_graph(); 
  print_agents_status();
  delete mundo;

  return 0;
}


