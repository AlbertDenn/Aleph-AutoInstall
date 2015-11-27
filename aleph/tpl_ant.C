
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


# include <tpl_ant.H>
# include <io_graph.H>

namespace Aleph {

double initial_life = 100000; 

double feromone_importance = 2.5;

double distance_importance = 1;
 
double food_importance = 4;

double & alpha = feromone_importance;
double & beta  = distance_importance;
double & chi   = food_importance;

double Q  = 10000; // constante de feromona

double K  = 1; // constante  de consumo de energía en transito

double L  = 1; // constante de pérdida de vida

double F  = 1; // constante de consumo de alimento


double min_dist  = 10;
double max_dist  = 1000;


long Working_Ant::born_count = 0;
long Working_Ant::died_count = 0;

int Working_Ant::bit_idx = 0;


bool Working_Ant::select_path(Ant_Graph::Node *  src_node, 
			      Ant_Graph::Node *& tgt_node, 
			      Ant_Graph::Arc *&  ant_arc)
{
  if (life < 0)
    return false;

  ant_arc  = NULL;
  tgt_node = NULL;

  double max_p = numeric_limits<double>::min();

  Ant_Graph::Node::Critical_Section node_critical_section(src_node);

  for (Node_Arc_Iterator<Ant_Graph> it(src_node); it.has_current(); it.next())
    {
      Ant_Node * tgt = it.get_tgt_node();

      // seleccionar entre los nodos que no hayan sido visitados el que
      // tenga mayor peso parámetros

      const double & food_w = tgt->food;

      if (food_w <= 0)
	continue;

      Ant_Arc * arc = it.get_current_arc();
 
      const double & tau = arc->feromone_level;

      const double & eta = arc->distance;

      const double total = pow(tau, alpha) + pow(eta, beta) + pow(food_w, chi);

      if (total > max_p)
	{
	  max_p    = total;
	  ant_arc  = it.get_current_arc();
	  tgt_node = it.get_tgt_node();
	}
    }

  return ant_arc != NULL ? true : false;
}

bool Working_Ant::walk(Ant_Graph *       g,
		       Ant_Graph::Node * tgt_node, 
		       Ant_Graph::Arc *  ant_arc)
{
  g->leave_agent_from_location(this);

  double distance;

  {
    CRITICAL_SECTION(ant_arc->mutex);

    distance = ant_arc->distance;

        // deposita feromona (mayor distancia ==> menor feromona)
    ant_arc->feromone_level += Q/distance;
  }

  {
    CRITICAL_SECTION(tgt_node->mutex);

        // Comida que consume hormiga (mayor distancia ==> mayor consumo)
    tgt_node->food -= F*distance/max_dist;
  }

      // vida que consume hormiga (mayor distancia ==> menos vida)
  life -= L*distance/max_dist;

  g->enter_agent_in_node(this, tgt_node);

  return true;
}


void ant_transit(void * __graph, void * __ant, void *)
{
  Ant_Graph * graph = static_cast<Ant_Graph*>(__graph);

  Working_Ant * working_ant = static_cast<Working_Ant*>(__ant); 

  {
    CRITICAL_SECTION(graph->bit_mutex);

    working_ant->my_bit = Working_Ant::bit_idx;

    Working_Ant::bit_idx = 
      (Working_Ant::bit_idx + 1) % graph->get_num_threads();
  }

  Ant_Graph::Node * current_node = graph->get_agent_node_location(working_ant);

  Ant_Graph::Node * next_node = NULL;

  Ant_Graph::Arc * next_arc = NULL;

  while (true)
    {
      const bool ant_is_alive = 
	working_ant->select_path(current_node, next_node, next_arc);

      if (not ant_is_alive) break;

      working_ant->walk(graph, next_node, next_arc);

      current_node = next_node;
    }

  graph->remove_agent(working_ant);
}


void save(Ant_Graph & g, ofstream & output)
{
  typedef Ant_Graph::Save_Node SN;
  typedef Ant_Graph::Load_Node LN;
  typedef Ant_Graph::Save_Arc  SA;
  typedef Ant_Graph::Load_Arc  LA;

  IO_Graph <Ant_Graph, LN, SN, LA, SA> (g).save_in_text_mode(output);
}

void load(Ant_Graph & g, ifstream & input)
{
  typedef Ant_Graph::Save_Node SN;
  typedef Ant_Graph::Load_Node LN;
  typedef Ant_Graph::Save_Arc  SA;
  typedef Ant_Graph::Load_Arc  LA;

  IO_Graph <Ant_Graph, LN, SN, LA, SA> (g).load_in_text_mode(input);
}

} // end namespace Aleph
