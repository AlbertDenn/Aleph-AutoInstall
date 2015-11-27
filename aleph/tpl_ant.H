
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

# ifndef TPL_ANT_H
# define TPL_ANT_H

# include <gsl/gsl_rng.h>
# include <gsl/gsl_randist.h>

# include <limits>

# include <fstream>

# include <tpl_dynArray.H>
# include <tpl_graph_utils.H>
# include <tpl_agent_graph.H>


extern bool verbose;

using namespace Aleph;

namespace Aleph {


extern double  initial_life; // = 100000

extern double  feromone_importance; // = 2.5

extern double  distance_importance; // = 1;
 
extern double  food_importance; // = 4;

extern double  & alpha; // = feromone_importance;
extern double  & beta;  // = distance_importance;
extern double  & chi;   // = food_importance;

extern double  Q; // = 10000; // constante de feromona

extern double  K; // = 1; // constante  de consumo de energía en transito

extern double  L; // = 1; // constante de pérdida de vida

extern double  F; // = 1; // constante de consumo de alimento

extern double  min_dist; //  = 10;
extern double  max_dist; //  = 1000;


struct Ant_Node : public Agent_Node<Empty_Class>
{
  double food;

  int num;

  Ant_Node() : food(0), num(-1)
  {
    // empty
  }

  Ant_Node(const double & __food)
    : food(__food), num(-1)
  {
    // empty
  }

  Ant_Node(const Empty_Class&) : num(-1) { /* Empty */ }

  Ant_Node(Ant_Node * p) 
    : food(p->food), num(p->num)
  {
    // empty
  }
};


struct Ant_Arc : public Agent_Arc<Empty_Class>
{
  double feromone_level;

  double distance;

  Ant_Arc() : feromone_level(0), distance(0) { /* empty */ }

  Ant_Arc(Ant_Node * src, Ant_Node * tgt) 
    : Agent_Arc <Empty_Class> (src, tgt), feromone_level(0), distance(0) 
  {
    // empty 
  }
};


struct Ant_Graph;


struct Ant : public Walking_Agent<Empty_Class>
{
  Ant() { /* empty */ }

  // Ant(Transit transit) 
  //   : Walking_Agent<Empty_Class>(Empty_Class(), transit)
  // {
  //   // empty
  // }

      // return true si hormiga debe continuar; false de lo contrario
  virtual bool select_path(void *  /* src_node */, 
			   void  *& /* tgt_node */, 
			   void *&  /* ant_arc */)
  {
    EXIT("ERROR: must not be called");

    return true;
  }

  virtual bool walk(Ant_Graph * /* g */, 
		    void *  /* tgt_node */, 
		    void *   /* ant_arc */)
  {
    EXIT("ERROR: must not be called");
    
    return false;
  }
};


struct Ant_Graph : public Agent_Graph<List_Graph, Ant_Node, Ant_Arc, Ant> 
{
  typedef Ant_Graph::Node Node;

  typedef Ant_Graph::Arc Arc;

  pthread_mutex_t bit_mutex;

  BitArray nodes;

  Ant_Graph(const size_t & num_threads = 1) 
    : Agent_Graph<List_Graph, Ant_Node, Ant_Arc, Ant> (num_threads), 
      nodes(nullptr)
  {
    init_mutex(bit_mutex);
  }

  ~Ant_Graph()
  {
    destroy_mutex(bit_mutex);
  }

  Ant * insert_ant(Ant * p)
  {
    
    return p;
  }

      // subclases para cargar y guardar el grafo en disco
  struct Save_Node
  {
    void operator () (ofstream & output, Ant_Graph &, Ant_Graph::Node * p)
    {
      output << p->food << " " << p->num << endl;

      if (verbose)
	cout << endl;
    }
  };

  struct Load_Node
  {
    void operator () (ifstream & input, Ant_Graph &, Ant_Graph::Node * p)
    {
      input >> p->food >> p->num;

      if (verbose)
	cout <<  p->food << " " << p->num;
    }
  };

  struct Save_Arc
  {
    void operator () (ofstream & output, Ant_Graph &, Ant_Graph::Arc * a)
    {
      output << a->feromone_level << " " << a->distance << endl;

      if (verbose)
	cout << a->feromone_level << " " << a->distance;
    }
  };

  struct Load_Arc
  {
    void operator () (ifstream & input, Ant_Graph &, Ant_Graph::Arc * a)
    {
      input >> a->feromone_level >> a->distance;

      if (verbose)
	cout << a->feromone_level << " " << a->distance;
    }
  };
};


struct Working_Ant : public Ant 
{
  double life;

  static long born_count;
  static long died_count;

  static int bit_idx;

  int my_bit; // bit asignado para marcar nodos visitados

  Working_Ant() : life(initial_life)
  {
    // empty
  }

  // Working_Ant(Transit transit, const double & __life) 
  //   : Ant(transit), life(__life)
  // {
  //   // Empty
  // }

  ~Working_Ant()
  {
    // empty
  }

  virtual bool select_path(Ant_Graph::Node *  src_node, 
			   Ant_Graph::Node *& tgt_node, 
			   Ant_Graph::Arc *&  ant_arc);

  virtual bool walk(Ant_Graph * g, 
		    Ant_Graph::Node *  tgt_node, 
		    Ant_Graph::Arc *   ant_arc);
};


void save(Ant_Graph & g, ofstream & output);

void load(Ant_Graph & g, ifstream & input);


} // end namespace Aleph

# endif 
