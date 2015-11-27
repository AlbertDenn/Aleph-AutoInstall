
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

# ifndef IO_GRAPH_H
# define IO_GRAPH_H

# include <fstream>
# include <iostream>
# include <tpl_graph.H>

extern bool verbose; // comunmente definida en main

namespace Aleph { 


  template <class GT>
struct Dft_Store_Node
{
  void operator () (ofstream & output, GT &, typename GT::Node * p)
  {
    output.write((char*) &p->get_info(), sizeof(typename GT::Node_Type));
  }

  void operator () (ostream & output, GT &, typename GT::Node * p)
  {
    output << p->get_info() << endl;
  }
};

  template <class GT>
struct Dft_Store_Arc
{
  void operator () (ofstream & output, GT &, typename GT::Arc * a)
  {
    output.write((char*) &a->get_info(), sizeof(typename GT::Arc_Type));
  }

  void operator () (ostream & output, GT &, typename GT::Arc * a)
  {
    output << a->get_info() << endl;
  }
};

  template <class GT>
struct Dft_Load_Node
{
  void operator () (ifstream & input, GT &, typename GT::Node * p)
  {
    input.read((char*) &p->get_info(), sizeof(typename GT::Node_Type));
  }

  void operator () (istream & input, GT &, typename GT::Node * p)
  {
    input >> p->get_info();
  }
};

  template <class GT>
struct Dft_Load_Arc
{
  void operator () (ifstream & input, GT &, typename GT::Arc * a)
  {
    input.read((char*) &a->get_info(), sizeof(typename GT::Arc_Type));
  }

  void operator () (istream & input, GT &, typename GT::Arc * a)
  {
    input >> a->get_info();
  }
};

template <class GT, 
	  class Load_Node  = Dft_Load_Node<GT>,
	  class Store_Node = Dft_Store_Node<GT>,
	  class Load_Arc   = Dft_Load_Arc<GT>,
	  class Store_Arc  = Dft_Store_Arc<GT>,
	  class NF         = Aleph::Dft_Show_Node<GT>, // Filtro de nodos
	  class AF         = Aleph::Dft_Show_Arc<GT> > // Filtro de arcos
class IO_Graph
{

  GT & g;

  Load_Node load_node   = Load_Node();
  Store_Node store_node = Store_Node();
  Load_Arc load_arc     = Load_Arc();
  Store_Arc store_arc   = Store_Arc();

  NF node_filter = NF();
  AF arc_filter  = AF();

public:

  void set_load_node(const Load_Node & ln) { load_node = ln; }

  void set_store_node(const Store_Node & sn) { store_node = sn; }

  void set_load_arc(const Load_Arc & la) { load_arc = la; }

  void set_store_arc(const Store_Arc & sa) { store_arc = sa; }

  void set_node_filter(const NF & nf) { node_filter = nf; }

  void set_arc_filter(const AF & af) { arc_filter = af; }

  IO_Graph(GT & __g) noexcept : g(__g) { /* empty */ }

  IO_Graph(GT * gptr) noexcept : g(*gptr) { /* empty */ }

  void save(ofstream & output)
  {
        // guarda cantidad de nodos
    const size_t num_nodes = g.get_num_nodes();

    if (verbose)
      cout << "Storing " << num_nodes << " nodes ... ";

    output.write((const char*) &num_nodes, sizeof(num_nodes));

    int i = 0; // contador de nodos. Lleva los índices en la tabla

        // tabla de pares <ptr nodo, orden de isnerción>
    DynMapTreap<typename GT::Node *, int> nodes_table;

    for (Node_Iterator<GT,NF> it(g, node_filter); it.has_curr(); it.next(), ++i)
      {
	auto p = it.get_current();

	if (verbose)
	  cout << i << " ";

	store_node(output, g, p);

	nodes_table.insert(p, i);
      }

    const size_t num_arcs = g.get_num_arcs();

    if (verbose)
      cout << " done " << endl 
	   << "Storing " << num_arcs << " arcs ... " << endl;

    output.write((const char*) &num_arcs, sizeof(num_arcs));

    for (Arc_Iterator<GT, AF> it(g, arc_filter); it.has_curr(); it.next())
      {
	auto a = it.get_current();

	auto src = g.get_src_node(a);
	auto tgt = g.get_tgt_node(a);

	const auto & src_idx = nodes_table.find(src);
	const auto & tgt_idx = nodes_table.find(tgt);

	    // guarda índices de nodos origen y destino respectivamente
	output.write((const char*) &src_idx, sizeof(int));
	output.write((const char*) &tgt_idx, sizeof(int));

	if (verbose)
	  cout << " " << src_idx << "--" << tgt_idx << " ";

	store_arc(output, g, a);

	if (verbose)
	  cout << endl;
      }
    
    if (verbose)
      cout << " done " << endl << endl;
  }

  void load(ifstream & input)
  {
    // lee cantidad de nodos
    size_t num_nodes;
    input.read((char *) &num_nodes, sizeof(num_nodes));

    if (verbose)
      cout << "Loading " << num_nodes << " nodes ...";

    DynArray<typename GT::Node*> nodes_table(num_nodes);
    nodes_table.reserve(0, num_nodes - 1);

    for (size_t i = 0; i < num_nodes; ++i)
      {
	typename GT::Node * p = new typename GT::Node;

	if (verbose)
	  cout << " " << i;

	load_node(input, g, p);

	p = g.insert_node(p);
	nodes_table.access(i) = p;
      }

    size_t num_arcs;
    input.read((char *) &num_arcs, sizeof(num_arcs));

    if (verbose)
      cout << " done " << endl 
	   << "Loading " << num_arcs << " arcs ... " << endl;

    for (int i = 0; i < num_arcs; ++i)
      {
	int src_idx;
	input.read((char*) &src_idx, sizeof(int));
	auto src = nodes_table.access(src_idx);

	int tgt_idx;
	input.read((char*) &tgt_idx, sizeof(int));
	auto tgt = nodes_table.access(tgt_idx);

	auto a = g.insert_arc(src, tgt);

	if (verbose)
	  cout << " " << src_idx << "--" << tgt_idx << " ";

	load_arc(input, g, a);

	if (verbose)
	  cout << endl;
      }

    if (verbose)
      cout << " done " << endl << endl;
  }

  void save_in_text_mode(ostream & output)
  {        // guarda cantidad de nodos
    const size_t num_nodes = g.get_num_nodes();
    const size_t num_arcs  = g.get_num_arcs();
    output << num_nodes << endl
	   << num_arcs << endl;

    if (verbose)
      cout << "Storing " << num_nodes << " nodes ... ";

    int i = 0; // contador de nodos. Lleva los índices en la tabla

        // tabla de pares <ptr nodo, orden de inserción>
    DynMapTreap<typename GT::Node *, int> nodes_table;

    for (Node_Iterator<GT,NF> it(g, node_filter); it.has_curr(); it.next(), ++i)
      {
	typename GT::Node * p = it.get_curr();

	if (verbose)
	  cout << i << " ";

	store_node(output, g, p);

	nodes_table.insert(p, i);
      }

    if (verbose)
      cout << " done " << endl 
	   << "Storing " << num_arcs << " arcs ... " << endl;

    for (Arc_Iterator<GT, AF> it(g, arc_filter); it.has_curr(); it.next())
      {
	auto a = it.get_curr();

	auto src = g.get_src_node(a);
	auto tgt = g.get_tgt_node(a);

	const auto & src_idx = nodes_table.find(src);
	const auto & tgt_idx = nodes_table.find(tgt);

	    // guarda índices de nodos origen y destino respectivamente
	output << src_idx << " " << tgt_idx << " ";

	if (verbose)
	  cout << " " << src_idx << "--" << tgt_idx << " ";

	store_arc(output, g, a);

	if (verbose)
	  cout << endl;
      }

    if (verbose)
      cout << " done " << endl << endl;
  }

  void load_in_text_mode(istream & input)
  {
    // lee cantidad de nodos
    size_t num_nodes;
    size_t num_arcs;
    
    input >> num_nodes >> num_arcs;
    input.ignore();

    if (verbose)
      cout << "Loading " << num_nodes << " nodes ...";

    DynArray<typename GT::Node*> nodes_table(num_nodes);
    nodes_table.reserve(0, num_nodes - 1);

    for (size_t i = 0; i < num_nodes; ++i)
      {
	auto p = new typename GT::Node;

	if (verbose)
	  cout << " " << i;

	load_node(input, g, p);

	p = g.insert_node(p);
	nodes_table.access(i) = p;
      }

    if (verbose)
      cout << " done " << endl 
	   << "Loading " << num_arcs << " arcs ... " << endl;

    for (int i = 0; i < num_arcs; ++i)
      {
	int src_idx;
	int tgt_idx;

	input >> src_idx >> tgt_idx;

	auto src = nodes_table.access(src_idx);
	auto tgt = nodes_table.access(tgt_idx);
	auto a = g.insert_arc(src, tgt);

	if (verbose)
	  cout << " " << src_idx << "--" << tgt_idx << " ";

	load_arc(input, g, a);

	if (verbose)
	  cout << " " << endl;
      }

    if (verbose)
      cout << " done " << endl << endl;
  }
};

} // end namespace Aleph

# endif // IO_GRAPH_H
