
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


# include <tpl_agraph.H>
# include <random_graph.H>
# include <generate_graph.H>

unsigned long node_count = 0;
unsigned long arc_count = 0;

using Net = Array_Graph<>;


struct Init_Node
{
  void operator () (const Net &, Net::Node * p) const
  {
    p->get_info() = node_count++;
  }
};

struct Init_Arc
{
  void operator () (const Net &, Net::Arc * a) const
  {
    a->get_info() = arc_count++;
  }
};

Net create_graph(size_t n, double prob, unsigned long seed)
{
  return Random_Graph<Net, Init_Node, Init_Arc>(seed)(n, prob);
}

void usage()
{
  cout << "test n prob seed" << endl;
  exit(0);
}

inline ostream & operator << (ostream & s, const Path<Net> & path)
{
  if (path.is_empty())
    return s << "Path is Empty";

  const Net & net = path.get_graph();
  Path<Net>::Iterator it(path);
  s << it.get_current_node()->get_info();
  for (; it.has_current_arc(); it.next())
    {
      Net::Arc * a = it.get_current_arc();
      s << "(" << a->get_info() << ")" 
	<< net.get_connected_node(a, it.get_current_node())-> get_info();
    }
  return s;
}



int main(int argc, char *argv[])
{
  if (argc != 4)
    usage();

  size_t n = atoi(argv[1]);
  double prob = atof(argv[2]);
  unsigned long seed = atoi(argv[3]);

  Net g = create_graph(n, prob, seed);

  To_Graphviz<Net> ().digraph(g, cout);

  for (Net::Node_Iterator it(g); it.has_curr(); it.next())
    {
      cout << "ARCS(" << it.get_curr()->get_info() << ") =";
      arcs<Net>(it.get_curr()).for_each([&g] (Net::Arc * a)
        {
	  cout << " " << g.get_src_node(a)->get_info() << "(" 
	       << a->get_info() << ")" << g.get_tgt_node(a)->get_info();
	});
      cout << endl
	   << endl
	   << "IN(" << it.get_curr()->get_info() << ") =";
      in_arcs<Net>(it.get_curr()).for_each([&g] (Net::Arc * a)
        {
	  cout << " " << g.get_src_node(a)->get_info() << "(" 
	       << a->get_info() << ")" << g.get_tgt_node(a)->get_info();
	});
      cout << endl
	   << endl
	   << "in_nodes(" << it.get_curr()->get_info() << ") =";
      in_nodes<Net>(it.get_curr()).for_each([] (Net::Node * p)
        {
	  cout << " " << p->get_info();
	});
      cout << endl
	   << endl
	   << "Out(" << it.get_curr()->get_info() << ") =";
      out_arcs<Net>(it.get_curr()).for_each([&g] (Net::Arc * a)
        {
	  cout << " " << g.get_src_node(a)->get_info() << "(" 
	       << a->get_info() << ")" << g.get_tgt_node(a)->get_info();
	});
      cout << endl
	   << endl
	   << "out_nodes(" << it.get_curr()->get_info() << ") =";
      out_nodes<Net>(it.get_curr()).for_each([] (Net::Node * p)
        {
	  cout << " " << p->get_info();
	});
      cout << endl
	   << endl;
    }

  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));
  DynArray<Net::Node*> nodes = g.nodes();
  long src_idx, tgt_idx;
  while (cin >> src_idx >> tgt_idx)
    {
      cout << "src = " << src_idx << endl
	   << "tgt = " << tgt_idx << endl;

      cout << "DFS" << endl
	   << Directed_Find_Path<Net>(g).dfs(nodes(src_idx), nodes(tgt_idx)) 
	   << endl
	   << "BFS" << endl
	   << Directed_Find_Path<Net>(g).bfs(nodes(src_idx), nodes(tgt_idx)) 
	   << endl
	   << endl;
    }

  gsl_rng_free(r);
}
