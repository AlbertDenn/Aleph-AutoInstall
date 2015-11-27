
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

# include <tpl_find_path.H>
# include <random_graph.H>
# include <tpl_sgraph.H>

//typedef List_SGraph<Graph_Snode<int>, Graph_Sarc<long>> Grafo;
typedef List_Graph<Graph_Node<int>, Graph_Arc<long>> Grafo;


int counter = 0;

unsigned int t = 0;

struct Inicia_Nodo
{
  void operator () (Grafo &, Grafo::Node * p)
  {
    p->get_info() = counter++;
  }
};

struct Inicia_Arco
{
  gsl_rng * r;

  Inicia_Arco()
  {
    r = gsl_rng_alloc (gsl_rng_mt19937);
    gsl_rng_set(r, t % gsl_rng_max(r));
  }

  Inicia_Arco(const Inicia_Arco & a) 
    : r(a.r)
  {
    const_cast<Inicia_Arco&>(a).r = NULL;
  }

  ~Inicia_Arco()
  {
    if (r != NULL)
      gsl_rng_free(r);
    r = NULL;
  }

  void operator () (Grafo &, Grafo::Arc * a) const
  {
    unsigned long rand_offset = 1 + gsl_rng_uniform_int(r, 99);

    a->get_info() = rand_offset;
  }
};

Grafo generar_grafo(size_t num_nodes, size_t num_arcs, unsigned int s)
{
  Grafo ret =
    Random_Graph <Grafo, Inicia_Nodo, Inicia_Arco> (s) (num_nodes, num_arcs);
  
  return ret;
}

Grafo::Node * buscar(Grafo & g, int num)
{
  return g.find_node(num);
}

// usage: test_find_path n m s t seed

int main(int argn, char * argc[])
{
  if (argn < 5)
    {
      printf("usage: test_find_path n m s t [seed]\n");
      exit(0);
    }

  size_t n = atoi(argc[1]);
  size_t m = atoi(argc[2]);
  
  int s = atoi(argc[3]);
  int t = atoi(argc[4]);

  unsigned int seed = 0;
  if (argn >= 6)
    seed = atoi(argc[5]);

  Grafo g = generar_grafo(n, m, seed);

  Grafo::Node * src = buscar(g, s);
  if (src == NULL)
    {
      printf("Node number %d not found\n", s);
      exit(1);
    }

  Grafo::Node * tgt = buscar(g, t);
  if (tgt == NULL)
    {
      printf("Node number %d not found\n", t);
      exit(1);
    }
  
  Path<Grafo> p1 = find_path_breadth_first(g, src, tgt);
  Path<Grafo> p2(g);

  Find_Path_Breadth_First<Grafo> () (g, src, tgt, p2);

  Path<Grafo>::Iterator it1(p1);
  Path<Grafo>::Iterator it2(p2);

  while (it1.has_curr() and it2.has_curr())
    {
      if (it1.get_current_node() != it2.get_current_node())
	{
	  printf("Distints paths!\n");
	  exit(1);
	}

      it1.next();
      it2.next();
    }
}
