
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


# include <tpl_indexNode.H>
# include <random_graph.H>
# include <eulerian.H>
# include <hamiltonian.H>
# include <single_graph.H>

typedef List_Graph<Graph_Node<int>, Graph_Arc<int> > Grafo;

typedef List_Digraph<Graph_Node<int>, Graph_Arc<int> > Digrafo;

    template <class GT>
GT crear_grafo(const size_t & num_nodes, const size_t & num_arcs)
{
  GT g = Random_Graph<GT> () (num_nodes, num_arcs);

  {
    typename GT::Node_Iterator it(g);
    for (int i = 0; it.has_current(); it.next(), ++i)
      it.get_current()->get_info() = i;
  }

  {
    typename GT::Arc_Iterator it(g);
    for (int i = 0; it.has_current(); it.next(), ++i)
      it.get_current()->get_info() = i;
  }

  return std::move(g);
}

void index_verify(Grafo & g)
{
  {
    IndexNode<Grafo> idx_node(g); 

    for (Grafo::Node_Iterator it(g); it.has_current(); it.next())
      assert(idx_node.search(it.get_current()) == it.get_current());
  }

{
    IndexArc<Grafo> idx_arc(g); 

    for (Grafo::Arc_Iterator it(g); it.has_current(); it.next())
      assert(idx_arc.search(it.get_current()) == it.get_current());
  }
}

int main(int argn, char ** argc)
{
  size_t num_nodes = argc[1] ? atoi(argc[1]) : 1000;
  size_t num_arcs = num_nodes;
  if (argn > 2)
    num_arcs = atoi(argc[2]);

  double p = 0.1;

  if (argn > 3)
    p = atof(argc[3]);

  unsigned int t = time(0);
  if (argn > 4)
    t = atoi(argc[4]);

  cout << argc[0] << " " << num_nodes << " " << num_arcs 
       << " " << p << " " << t << endl;

  {
    cout << "Building random graph by selecting random nodes ..." << endl;

    Grafo g = Random_Graph<Grafo> (t) (num_nodes, num_arcs);

    cout << "Created graph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not test_connectivity (g))
      ERROR("Grafo no es conexo");
    
    assert(Test_Single_Graph<Grafo>()(g));
  }

  {
    cout << "Building random graph by arc probability ..." << endl;

    Grafo g = Random_Graph<Grafo> (t) (num_nodes, p);

    cout << "Created graph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not test_connectivity (g))
      ERROR("Grafo no es conexo");

    assert(Test_Single_Graph<Grafo>()(g));
  }

  {
    cout << "Building random digraph by selecting random nodes ..." << endl;

    Digrafo g = Random_Digraph<Digrafo> (t) (num_nodes, num_arcs);

    cout << "Created digraph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    Digrafo g1 = Random_Digraph<Digrafo> (t) (num_nodes, num_arcs);

    cout << "Created digraph of " << g1.get_num_nodes() << " nodes and " 
     	 << g1.get_num_arcs() << " arcs " << endl << endl;

    {
      DynDlist<DynDlist<Digrafo::Node*> > blocks;
      Tarjan_Connected_Components <Digrafo> () (g, blocks);
      if (blocks.size() > 1)
	ERROR("Digrafo no es conexo");
    }
    
    assert(Test_Single_Graph<Digrafo>()(g));
  }
 
  {
    cout << "Building random digraph by arc probability ..." << endl;

    Digrafo g = Random_Digraph<Digrafo> (t) (num_nodes, p);

    cout << "Created digraph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    {
      DynDlist<DynDlist<Digrafo::Node*> > blocks;
      Tarjan_Connected_Components <Digrafo> () (g, blocks);
      if (blocks.size() > 1)
	ERROR("Digrafo no es conexo");
    
      assert(Test_Single_Graph<Digrafo>()(g));
    }
  }

  {
    cout << "Building random eulerian graph by selecting random nodes ..." << endl;

    Grafo g =  Random_Graph <Grafo> () . eulerian (num_nodes, num_arcs);

    cout << "Created eulerian graph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not test_connectivity (g))
      ERROR("Grafo no es conexo");

    if (Test_Eulerian <Grafo> () (g))
      cout << "el grafo es euleriano" << endl;
    else
      cout << "el grafo no es euleriano" << endl;
    
    cout << endl;
    
    assert(Test_Single_Graph<Grafo>()(g));
  }

  {
    cout << "Building random eulerian graph by arc probability ..." << endl;

    Grafo g =  Random_Graph <Grafo> (t) . eulerian (num_nodes, p);

    cout << "Created eulerian graph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not test_connectivity (g))
      ERROR("Grafo no es conexo");

    if (Test_Eulerian <Grafo> () (g))
      cout << "el grafo es euleriano" << endl;
    else
      cout << "el grafo no es euleriano" << endl;

    assert(Test_Single_Graph<Grafo>()(g));

    cout << endl;
  }

  {
    cout << "Building random eulerian digraph by selecting random nodes ..." << endl;

    Digrafo g =  Random_Digraph <Digrafo> () . eulerian (num_nodes, num_arcs);

    cout << "Created eulerian digraph of " << g.get_num_nodes() 
	 << " nodes and " << g.get_num_arcs() << " arcs " << endl << endl;

    {
      DynDlist<DynDlist<Digrafo::Node*>> blocks;
      Tarjan_Connected_Components <Digrafo> () (g, blocks);
      if (blocks.size() > 1)
	ERROR("Digrafo no es conexo");
    }

    if (Test_Eulerian <Digrafo> () (g))
      cout << "el grafo es euleriano" << endl;
    else
      cout << "el grafo no es euleriano" << endl;
    
    assert(Test_Single_Graph<Digrafo>()(g));

    cout << endl;
  }

  {
    cout << "Building random eulerian digraph by arc probability ..." << endl;

    Digrafo g = Random_Digraph <Digrafo> (t) . eulerian (num_nodes, p);

    cout << "Created eulerian graph of " << g.get_num_nodes() << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    {
      DynDlist<DynDlist<Digrafo::Node*> > blocks;
      Tarjan_Connected_Components <Digrafo> () (g, blocks);
      if (blocks.size() > 1)
	ERROR("Digrafo no es conexo");
    }

    if (Test_Eulerian <Digrafo> () (g))
      cout << "el grafo es euleriano" << endl;
    else
      cout << "el grafo no es euleriano" << endl;

    assert(Test_Single_Graph<Digrafo>()(g));

    cout << endl;
  }

  {
    cout << "Building random hamiltonian graph by arc probability ..." 
	 << endl;

    Grafo g = Random_Graph <Grafo> () . sufficient_hamiltonian (num_nodes);

    cout << "Created hamiltonian graph of " << g.get_num_nodes() 
	 << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not test_connectivity (g))
      ERROR("Grafo no es conexo");

    if (Test_Hamiltonian_Sufficiency <Grafo> () (g))
      cout << "el grafo es hamiltonian" << endl;
    else
      cout << "el grafo no es hamiltonian" << endl;
    
    assert(Test_Single_Graph<Grafo>()(g));

    cout << endl;
  }
 
  {
    cout << "Building random hamiltonian digraph by arc probability ..." 
	 << endl;

    Digrafo g = 
      Random_Digraph <Digrafo> () . sufficient_hamiltonian (num_nodes);

    cout << "Created hamiltonian digraph of " << g.get_num_nodes() 
	 << " nodes and " 
	 << g.get_num_arcs() << " arcs " << endl << endl;

    if (not (Tarjan_Connected_Components <Digrafo> ()) . test_connectivity(g))
      ERROR("Digrafo no es conexo");

    if (Test_Hamiltonian_Sufficiency <Digrafo> () (g))
      cout << "el digrafo es hamiltonian" << endl;
    else
      cout << "el digrafo no es hamiltonian" << endl;

    assert(Test_Single_Graph<Digrafo>()(g));
    
    cout << endl;
  }
}
