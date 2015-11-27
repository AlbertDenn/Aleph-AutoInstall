
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

# include <Dijkstra.H>
# include <euclidian-graph-common.H>
# include <io_graph.H>

using namespace Aleph;

bool verbose = false;

typedef List_SGraph<Graph_Snode<My_P>, Graph_Sarc<int>> Graph;
//typedef Array_Graph<Graph_Anode<My_P>, Graph_Aarc<int>> Graph;
//typedef Array_Graph<Graph_Anode<My_P>, Graph_Aarc<int>> Graph;

void usage(int argn, char * argc[])
{
  if (argn > 3)
    return;

  printf("usage: %s filename [pair list]\n", argc[0]);
  exit(1);
}
 
void print(Path<Graph> & path)
{
  for (Path<Graph>::Iterator it(path); it.has_curr(); it.next())
    {
      Graph::Node * p = it.get_current_node();
      cout << "(" << p->get_info().x << "," << p->get_info().y << ")";
      if (it.has_current_arc())
	cout << "-" << it.get_current_arc()->get_info() << "-";
    }
}

Graph::Node * locate_node(Graph & g, size_t n)
{
  Graph::Node_Iterator it(g);
  
  for (int i = 0; it.has_curr() and i < n; it.next(), ++i) ;
    
  return it.has_curr() ? it.get_curr() : NULL;
}

struct Aplus
{
  Graph & g;
  int x, y; // coordenadas del destino
  float alpha;

  Aplus(Graph & __g, Graph::Node * end, const float a) : g(__g), alpha(a)
  {
    My_P & info = end->get_info();

    x = info.x;
    y = info.y;
  }

  int operator () (Graph::Arc * arc, const int & op1, const int & op2)
  {
    Graph::Node * tgt = g.get_tgt_node(arc);
    if (IS_NODE_VISITED(tgt, Aleph::Spanning_Tree))
      tgt = g.get_src_node(arc);

    My_P & info = tgt->get_info();

    float H = hypot(info.x - x, info.y - y);

    return op1 + op2 + alpha*H;
  }

  int operator () (const int & op1, const int & op2)
  {
    return op1 + op2;
  } 
};

int main(int argn, char ** argc)
{
  usage(argn, argc);

  ifstream in(argc[1]);

  if (not in)
    {
      cerr << "cannot open " << argc[1] << " file" << endl;
      exit(1);
    }

  Graph g;

  cout << "Loading graph ..." << endl;

  {
    IO_Graph
      <Graph, Rnode<Graph>, Wnode<Graph>, Rarc<Graph>, Warc<Graph>> (g) 
	. load_in_text_mode(in);
  }
      
  cout << "Done!" << endl;

  for (int i = 2; i < argn; i += 2)
    {
      cout << "Preparing to compute min path from " << argc[i] 
	   << " to " << argc[i+1] << endl
	   << "Searching " << argc[i] << "th node" << endl;
      
      Graph::Node * s = locate_node(g, atoi(argc[i]));
      if (s == NULL)
	{
	  cout << "Source node " << argc[i] << " has not been found " << endl;
	  exit(1);
	}
      cout << "Source = (" << s->get_info().x << ","
	   << s->get_info().y << ")" << endl;
      
      Graph::Node * t = locate_node(g, atoi(argc[i+1]));
      if (t == NULL)
	{
	  cout << "target node " << argc[i+1] << " has not been found " << endl;
	  exit(1);
	}
      cout << "Target = (" << t->get_info().x << ","
	   << t->get_info().y << ")" << endl;
      
      Path<Graph> p(g);

      cout << "Computing min path from " << argc[i] 
	   << " to " << argc[i+1] << endl;

      int d = Dijkstra_Min_Paths<Graph>()
// int d = Dijkstra_Min_Paths<Graph, Dft_Dist<Graph>, Aplus>(Aplus(g, t, 1))
	.find_min_path(g, s, t, p);

      cout << "Done!" << endl;

      print(p);
      cout << " = " << d << endl;
    }
}
