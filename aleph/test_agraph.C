
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
# include <random_graph.H>
# include <tpl_agraph.H>

# define INDENT "    "

using namespace Aleph;

typedef Array_Digraph<Graph_Anode<int>, Graph_Aarc<int>> Agraph;

struct Init_Node
{
  static int count;

  void operator () (Agraph&, Agraph::Node * p)
  {
    p->get_info() = count++;
  }
};

int Init_Node::count = 0;


struct Init_Arc
{
  static int count;

  void operator () (Agraph&, Agraph::Arc * a)
  {
    a->get_info() = count++;
  }
};

int Init_Arc::count = 0;


Agraph generate_graph(size_t num_nodes, size_t num_arcs, unsigned int seed)
{
  Random_Digraph<Agraph, Init_Node, Init_Arc> gen(seed);

  return gen(num_nodes, num_arcs);
}


int main(int argn, char * argc[])
{
  if (argn < 4)
    {
      printf("usage %s n m seed [rand-arcs-to-delete] [nodes to delete]\n", 
	     argc[0]);
      exit(1);
    }

  size_t n = 1000;
  size_t m = 4000;
  unsigned int seed = time(0);

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    m = atoi(argc[2]);

  if (argn > 3)
    seed = atoi(argc[3]);

  cout << "Generando grafo .." << endl;

  Agraph g = generate_graph(n, m, seed);

  cout << "Generado con " << g.get_num_nodes() << " y " 
       << g.get_num_arcs() << endl;

  if (argn < 5)
    return 0;

  int num_arcs = atoi(argc[4]);
  gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
  cout << "Borrando " << num_arcs << " seleccionados al azar ... " << endl;
  for (int i = 0; i < num_arcs; ++i)
    {
      int num = gsl_rng_uniform_int(r, g.get_num_arcs());
      cout << i << ": buscando arco " << num << " a eliminar ..." << endl;
      Agraph::Arc * a = g.find_arc(num);
      if (a == NULL)
	{
	  cout << "RARO: arco " << num << " no encontrado" << endl;
	  continue;
	}

      cout << "Elinando arco " << a->get_info() << endl;
      g.remove_arc(a);
      cout << "eliminado!" << endl
	   << endl;
    }
  gsl_rng_free(r);

  for (int i = 5; i < argn; ++i)
    {
      int num = atoi(argc[i]);
      cout << "Buscando nodo " << num << endl;
      Agraph::Node * p = g.find_node(num);
      if (p != NULL)
	{
	  cout << "Encontrado!" << endl
	       << endl
	       << "eliminando nodo " << num << " ... " << endl;
      
	  g.remove_node(p);

	  cout << "eliminado!" << endl;
	}
      else
	cout << "No encontrado!" << endl;
      
      cout << endl;
    }
}


