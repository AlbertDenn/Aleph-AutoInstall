
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


# include <random_graph.H>
# include <io_graph.H>


struct Data_Node
{
  int i, j;
};

typedef List_Graph<Graph_Node<int>, Graph_Arc<string> > Grafo;

bool verbose = true; 

struct Write_Arc
{
  void operator () (ofstream & output, Grafo &, Grafo::Arc * a)
  {
    size_t sz = a->get_info().size() + 1;
    output.write((char*) &sz, sizeof(sz));
    output.write(a->get_info().c_str(), a->get_info().size() + 1);
  }
};

struct Read_Arc
{
  static const size_t max_sz = 255;

  static char buffer[max_sz];

  void operator () (ifstream & input, Grafo &, Grafo::Arc * a)
  {
    size_t sz;
    input.read((char*) &sz, sizeof(sz));

    if (sz > max_sz)
      std::range_error("string too long; possibly bad");

    input.read(buffer, sz);

    a->get_info() = string(buffer);
  }
};

  char Read_Arc::buffer[max_sz];

Grafo build_graph()
{
  Grafo g = Random_Graph <Grafo> () (10000, 0.2);

  int i = 0;

  for (Grafo::Node_Iterator it(g); it.has_current(); it.next())
    it.get_current()->get_info() = i++;

  for (Grafo::Arc_Iterator it(g); it.has_current(); it.next())
    {
      Grafo::Arc * a = it.get_current();
      Grafo::Node * src = g.get_src_node(a);
      Grafo::Node * tgt = g.get_tgt_node(a);

      a->get_info() = 
	to_string(src->get_info()) + "--" + to_string(tgt->get_info());
    }

  return g;
}


void print_graph(Grafo & g)
{
  cout << "Nodes' content ";
  for (Grafo::Node_Iterator it(g); it.has_current(); it.next())
    cout << it.get_current()->get_info() << " ";

  cout << " done" << endl << endl
       << "Arcs' content ";
  for (Grafo::Arc_Iterator it(g); it.has_current(); it.next())
    {
      Grafo::Arc * a = it.get_current();

      cout << a->get_info() << " ";
    }
  cout << " done" << endl << endl;
}


int main()
{
  {
    Grafo g = build_graph();

    ofstream out("test-random-graph.gra", ios::trunc);

    cout << "saving graph ...";

    IO_Graph<Grafo,Dft_Load_Node<Grafo>,Dft_Store_Node<Grafo>,Read_Arc,Write_Arc> 
      (g).save (out); 

    cout << " done" << endl << endl;

    print_graph(g);
  }
  
  {
    ifstream in("test-random-graph.gra");

    Grafo  g;

    cout << "Enter a key in order to star loading ...";
    cin.get();

    cout << "loading graph ...";

    IO_Graph<Grafo,Dft_Load_Node<Grafo>,Dft_Store_Node<Grafo>,Read_Arc,Write_Arc> 
      (g).load (in); 

    cout << " done" << endl << endl;

    print_graph(g);
  }
}
