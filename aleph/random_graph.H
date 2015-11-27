
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

# ifndef RANDOM_GRAPH_H
# define RANDOM_GRAPH_H

# include <gsl/gsl_rng.h> 
# include <memory>
# include <tpl_indexArc.H>
# include <tpl_graph_utils.H>
# include <tpl_components.H>
# include <single_graph.H>
# include <Tarjan.H>

namespace Aleph 
{

    template <class GT>
struct Dft_Init_Rand_Node
{
  void operator () (GT &, typename GT::Node *) const noexcept
  {
    // empty
  }
};


    template <class GT>
struct Dft_Init_Rand_Arc
{
  void operator () (GT &, typename GT::Arc *) const noexcept
  {
    // empty
  }
};

  // TODO: eliminar clases Init_Node e Init_Arc y en su lugar usar lambdas

  template <class GT, class Init_Node, class Init_Arc>
class Random_Graph_Base
{
protected:

  typedef typename GT::Node GT_Node;
  typedef typename GT::Arc GT_Arc;

  gsl_rng * r;

  Init_Node & init_node;
  Init_Arc &  init_arc;

  unique_ptr<DynArray<GT_Node*>> nodes; // puntero para ahorrar
				        // directorio en caso de que no 
                                        // se utilice

  unique_ptr<IndexArc<GT>> idx_arc; // puntero porque el constructor
				    // requiere el grafo.
  mutable size_t num_nodes;
  mutable size_t num_arcs;
  mutable unsigned long rand_max; 

  GT g;

  bool save_parity; // indica si se debe o no guardar relaciones de
		    // paridad entre los nodos. Sólo para construir
		    // eulerianos y hamiltonianos

       virtual void 
  update_parity_after_arc_insertion(GT_Node * src, GT_Node * tgt) = 0;

  GT_Arc * insert_arc(GT_Node * src, GT_Node * tgt)
  {
    auto a = idx_arc->insert(g.insert_arc(src, tgt));
    init_arc (g, a);
    update_parity_after_arc_insertion(src, tgt);
    return a;
  }

      // selecciona un nodo azar distinto a excluded
  GT_Node * select_random_node(GT_Node * excluded = nullptr) noexcept
  {
    assert(nodes.get() != nullptr);

    unsigned long idx;
    GT_Node * ret_val = nullptr;
    while (true)
      {
	idx = gsl_rng_uniform_int(r, num_nodes);
	ret_val = nodes->access(idx);
	if (excluded == nullptr or ret_val != excluded)
	  break;
      }

    return ret_val;
  }

      // selecciona un nodo azar entre los contenidos en la lista list
  GT_Node * select_random_node(DynList<GT_Node*> & list) noexcept
  {
    const unsigned long k = gsl_rng_uniform_int(r, list.size());
    typename DynList<GT_Node*>::Iterator it(list);
    for (int i = 0; i < k; ++i, it.next()) ;

    return it.get_curr();
  }

  virtual void create_nodes_and_initialize_arc_index() = 0;

  virtual void connect() = 0;

  void initialize_and_create_nodes(const size_t & __num_nodes, 
				   const size_t & __num_arcs)
  {
    num_nodes = __num_nodes;

    const size_t num_nodes_2 = num_nodes*num_nodes;
    if (g.is_digraph())
      num_arcs = std::min(__num_arcs, num_nodes_2 - num_nodes);
    else
      num_arcs = std::min(__num_arcs, (num_nodes_2 - num_nodes)/2);
    
    create_nodes_and_initialize_arc_index();
  }

  Random_Graph_Base(unsigned long seed,
		    const Init_Node & __init_node,
		    const Init_Arc  & __init_arc) 
    : r(gsl_rng_alloc (gsl_rng_mt19937)), 
      init_node(const_cast<Init_Node&>(__init_node)), 
      init_arc(const_cast<Init_Arc&>(__init_arc)),
      num_nodes(0), num_arcs(0),
      rand_max(gsl_rng_max(r)), save_parity(false) 
  {
    if (r == nullptr)
      throw std::bad_alloc();

    gsl_rng_set(r, seed % rand_max);
  }

  ~Random_Graph_Base()
  {
    if (r != nullptr)
      gsl_rng_free(r);
  }

       // Crea un grafo aleatorio esparcido.
  GT create(const size_t & __num_nodes, const size_t & __num_arcs, 
	    bool connected)
  {
    initialize_and_create_nodes(__num_nodes, __num_arcs);

      // inserta al azar los arcos seleccionando al azar pares de nodos
    for (int i = 0; i < num_arcs; ++i)
      {
	auto src = select_random_node();
	auto tgt = select_random_node(src);
	if (idx_arc->search(src, tgt) == nullptr) // se repite el arco?
	  insert_arc(src, tgt);
      }

    if (connected)
      connect();
    
    return move(g);
  }

  virtual GT create_p(const size_t & __num_nodes, const double & p, 
		      bool connected) = 0;

  virtual void make_eulerian() = 0;

  virtual void make_hamiltonian () = 0;

public:

  GT eulerian(const size_t & __num_nodes, const size_t & __num_arcs) 
  {
    save_parity = true;
    g = this->create(__num_nodes, __num_arcs, true);
    make_eulerian();

    return std::move(g);
  }

  GT eulerian (const size_t & __num_nodes, const double & p) 
  {
    save_parity = true;
    g = this->create_p(__num_nodes, p, true);
    make_eulerian();

    return std::move(g);
  }

  GT sufficient_hamiltonian (const size_t & __num_nodes, 
			     const double & p = 0.5) 
  {
    g = this->create_p(__num_nodes, p, true);
    make_hamiltonian();

    return std::move(g);
  }
};

 
  template <class GT, 
	    class Init_Node = Dft_Init_Rand_Node<GT>,
	    class Init_Arc = Dft_Init_Rand_Arc<GT> >
class Random_Graph : public Random_Graph_Base<GT, Init_Node, Init_Arc>
{
  typedef typename GT::Node GT_Node;
  typedef typename GT::Arc GT_Arc;

  DynSetRandTree<GT_Node*> odd_nodes;  // nodos con grado impar
  DynSetRandTree<GT_Node*> even_nodes; // nodos con grado par
  
  virtual void update_parity_after_arc_insertion(GT_Node * src, GT_Node * tgt)
  {
    if (not this->save_parity)
      return;

    if (is_even(this->g.get_num_arcs(src)))
      { // era impar antes de la inserción
	this->odd_nodes.remove(src);
	this->even_nodes.insert(src);
      }
    else
      {
	this->even_nodes.remove(src);
	this->odd_nodes.insert(src);
      }

    if (is_even(this->g.get_num_arcs(tgt)))
      { // era impar antes de la inserción
	this->odd_nodes.remove(tgt);
	this->even_nodes.insert(tgt);
      }
    else
      {
	this->even_nodes.remove(tgt);
	this->odd_nodes.insert(tgt);
      }
  }

  virtual void create_nodes_and_initialize_arc_index()
  {
    this->nodes = unique_ptr<DynArray<GT_Node*>>
      (new DynArray<GT_Node*>(this->num_nodes));

    this->nodes->reserve(this->num_nodes);

    for (int i = 0; i < this->num_nodes; ++i)
      {
	auto p = this->g.insert_node(new GT_Node);
	this->nodes->access(i) = p;
	this->init_node(this->g, p);
	if (this->save_parity)
	  {
	    this->even_nodes.insert(p);
	    NODE_COUNTER(p) = 0;
	  }
      }

    this->idx_arc = unique_ptr<IndexArc<GT>> (new IndexArc<GT>(this->g) ); 
  }

  virtual void connect()
  {
    DynList<DynList<GT_Node*>> subgraphs; // lista sufgrafos

    Inconnected_Components <GT> () (this->g, subgraphs);

    const size_t & num_subs = subgraphs.size();

    if (num_subs == 1)
      return;

    DynArray<GT_Node*> block_nodes;

    for (typename DynList<DynList<GT_Node*>>::Iterator it(subgraphs); 
	 it.has_curr(); it.next())
      block_nodes.append(this->select_random_node(it.get_curr()));
    
    for (int i = 1; i < num_subs; ++i)
      {
	auto src = block_nodes.access(i - 1);
	auto tgt = block_nodes.access(i);
	this->insert_arc(src, tgt);
      }
  }

  // crea un grafo aleatorio en el cual la probabilida por arco entre un
  // par de nodos es p
  virtual GT create_p (const size_t & __num_nodes, const double & p, 
		       bool connected)
  {
    if (p > 1.0 or p <= 0.0)
      throw std::domain_error("Invalid value for p");

    this->initialize_and_create_nodes(__num_nodes, __num_nodes);

    for (int i = 0; i < this->num_nodes - 1; ++i)
      {
	auto src = this->nodes->access(i);
	for (int j = i + 1; j < this->num_nodes; ++j)
	  if (gsl_rng_uniform(this->r) <= p) // sorteo
	    {
	      auto tgt = this->nodes->access(j);
	      assert(src != tgt);
	      this->insert_arc(src, tgt);
	    }	
      }

    if (connected)
      connect();

    return move(this->g);
  }

public:

  Random_Graph(unsigned long seed,
	       const Init_Node & __init_node,
	       const Init_Arc  & __init_arc)	       
    : Random_Graph_Base<GT, Init_Node, Init_Arc>(seed, __init_node, __init_arc)
  {
    if (this->g.is_digraph())
      throw std::domain_error("Building of random digraph through a graph");
  }

  Random_Graph(unsigned long seed = time(nullptr),
	       const Init_Node && __init_node = Init_Node(),
	       const Init_Arc  && __init_arc  = Init_Arc())	       
    : Random_Graph_Base<GT, Init_Node, Init_Arc>(seed, __init_node, __init_arc)
  {
    if (this->g.is_digraph())
      throw std::domain_error("Building of random digraph through a graph");
  }


  GT operator () (const size_t & __num_nodes, const size_t & __num_arcs, 
		  bool connected = true) 
  {
    return this->create(__num_nodes, __num_arcs, connected);
  }
  
public:

  GT operator () (const size_t & __num_nodes, const double & p, 
		  bool connected = true) 
  {
    return create_p(__num_nodes, p, connected);
  }

private:

  virtual void make_eulerian()
  {
    while (this->odd_nodes.size() > 1)
      {
	GT_Node * src = nullptr;
	GT_Node * tgt = nullptr;

	while (true)
	  {
	    src = this->odd_nodes.select
	      (gsl_rng_uniform_int(this->r, this->odd_nodes.size()));
	    do
	      tgt = this->odd_nodes.select
		(gsl_rng_uniform_int(this->r, this->odd_nodes.size()));
	    while (tgt == src);

	    if (this->idx_arc->search(src, tgt) == nullptr) 
	      break;
	    else if (this->odd_nodes.size() == 2)
	      { // seleccionar nodo al azar que no tenga arco hacia src o tgt
		GT_Node * p = nullptr;
		do
		  p = this->even_nodes.select
		    (gsl_rng_uniform_int(this->r, this->even_nodes.size()));
		while (this->idx_arc->search(src, p) != nullptr or
		       this->idx_arc->search(tgt, p) != nullptr);
		this->insert_arc(src, p);
		this->insert_arc(p, tgt);

		return;
	      }
	  }
	   
	this->insert_arc(src, tgt);
      }
    
    assert(this->odd_nodes.size() == 0);
  }

  void balance_graph_nodes_degree(GT_Node * src, GT_Node * tgt)
  {
    if (this->idx_arc->search(src, tgt) == nullptr)
      this->insert_arc(src, tgt);

    const size_t & n = this->g.get_num_nodes();

    while (this->g.get_num_arcs(src) + this->g.get_num_arcs(tgt) < n)
      {
	auto p = this->nodes->access(gsl_rng_uniform_int(this->r, n));
	if (p == src or p == tgt)
	  continue;
	
	if (this->idx_arc->search(src, p) == nullptr)
	  this->insert_arc(src, p);

	if (this->g.get_num_arcs(src) + this->g.get_num_arcs(tgt) == n)
	  break;

	if (this->idx_arc->search(tgt, p) == nullptr)
	  this->insert_arc(tgt, p);
      }
  }

  virtual void make_hamiltonian ()
  {
    const size_t & n = this->g.get_num_nodes();
    for (int i = 0; i < n - 1; ++i)
      {
	auto src = this->nodes->access(i);
	for (int j = i + 1; j < n; ++j)
	  balance_graph_nodes_degree(src, this->nodes->access(j));
      }
  }

public:

  GT eulerian (const size_t & __num_nodes, const size_t & __num_arcs) 
  {
    this->save_parity = true;
    this->g = this->create(__num_nodes, __num_arcs, true);
    make_eulerian();

    return std::move(this->g);
  }

  GT eulerian (const size_t & __num_nodes, const double & p) 
  {
    this->save_parity = true;
    this->g = this->create_p(__num_nodes, p, true);
    make_eulerian();
 
    return std::move(this->g);
  }

  GT sufficient_hamiltonian (const size_t & __num_nodes, 
			     const double & p = 0.5) 
  {
    this->g = this->create_p(__num_nodes, p, true);
    make_hamiltonian();

    return std::move(this->g);
  }
};


  template <class GT, 
	    class Init_Node = Dft_Init_Rand_Node<GT>,
	    class Init_Arc = Dft_Init_Rand_Arc<GT>>
class Random_Digraph : public Random_Graph_Base<GT, Init_Node, Init_Arc>
{
  typedef typename GT::Node GT_Node;
  typedef typename GT::Arc GT_Arc;

  DynSetRandTree<GT_Node*> greater; // nodos con grado out mayor que in
  DynSetRandTree<GT_Node*> smaller; // nodos con grado out menor que in
  DynSetRandTree<GT_Node*> equal;   // nodos con grado out igual a in

  bool verify_tables()
  {
    const size_t & n = this->nodes->size();

    if (n != this->g.get_num_nodes())
      cout << "Warning num of nodes of graph does not match with array "
	   << this->g.get_num_nodes() << "!=" << n << endl;
    
    size_t total = greater.size() + smaller.size() + equal.size();
    if (total != this->g.get_num_nodes())
      cout << "Inconsistency with nodes parity" << endl
	   << "greater = " << greater.size() << endl
	   << "smaller = " << smaller.size() << endl
	   << "equal   = " << equal.size() << endl
	   << "total   = " << total << endl
	   << "|V|     = " << this->g.get_num_nodes();

    for (int i = 0; i < n; ++i)
      {
	auto p = this->nodes->access(i);
	
	const long & in_sz    = NODE_COUNTER(p);
	const size_t & out_sz = this->g.get_num_arcs(p);
	
	if (in_sz == out_sz)
	  {
	    if (smaller.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << " in smaller table" << endl;
	    
	    if (greater.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << " in greater table" << endl;
	    
	    if (equal.search(p) == nullptr)
	      {
		cout << "node of same in/out degree is not in equal table" 
		     << endl;

		return false;
	      }
	  }
	else if (in_sz > out_sz)
	  {
	    if (greater.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << " in greater table" << endl;
	    
	    if (equal.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << endl;

	    if (smaller.search(p) == nullptr)
	      {
		cout << "node with " << in_sz << "/" << out_sz << " not found "
		     << "smaller table" << endl;

		return false;
	      }	    	    
	  }
	else
	  {
	   if (smaller.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << " in smaller table" << endl;
	    
	    if (equal.search(p) != nullptr)
	      cout << "Inconsistency " << in_sz << "/" << out_sz << " found "
		   << endl;

	    if (greater.search(p) == nullptr)
	      {
		cout << "node with " << in_sz << "/" << out_sz << " not found "
		     << "greater table" << endl;

		return false;
	      }	    	     
	  }
      }

    return true;
  }  
 
  // Esta llamada se efectúa justo después de insertar un nuevo
  // arco src-->tgt. Esto implica que out(src) está actualizado, pero
  // in(tgt) no 
  virtual void update_parity_after_arc_insertion(GT_Node * src, GT_Node * tgt)
  {
    if (not this->save_parity)
      return;

    const size_t & src_out_degree = this->g.get_num_arcs(src);
    const long & src_in_degree    = NODE_COUNTER(src);

    if (src_out_degree == src_in_degree)
      { // src está en greater ==> sacarlo y meterlo en equal
	assert(this->smaller.search(src) != nullptr);
	this->smaller.remove(src);
	this->equal.insert(src);
      }
    else if (src_out_degree > src_in_degree)
      if (src_out_degree == src_in_degree + 1)
	{
	  assert(this->equal.search(src) != nullptr);
	  this->equal.remove(src);
	  this->greater.insert(src);
	}
      else
	assert(this->greater.search(src) != nullptr);
    else // src_out_degree < src_in_degree
      assert(this->smaller.search(src) != nullptr);
    
    const size_t & tgt_out_degree = this->g.get_num_arcs(tgt);
    const long tgt_in_degree      = ++NODE_COUNTER(tgt);

    if (tgt_out_degree == tgt_in_degree)
      {
	assert(this->greater.search(tgt));
	this->greater.remove(tgt);
	this->equal.insert(tgt);
      }
    else if (tgt_out_degree > tgt_in_degree)
      assert(this->greater.search(tgt));
    else // (tgt_out_degree < tgt_in_degree)
      {
	if (tgt_in_degree - 1 == tgt_out_degree)
	  { // tgt está en equal ==> sacarlo
	    assert(this->equal.search(tgt) != nullptr);
	    this->smaller.insert(tgt);	  
	    this->equal.remove(tgt);
	  }
	else
	  assert(this->smaller.search(tgt) != nullptr);
      } 
  }

  virtual void create_nodes_and_initialize_arc_index()
  {
    this->nodes = unique_ptr<DynArray<GT_Node*>>
      (new DynArray<GT_Node*>(this->num_nodes));

    this->nodes->reserve(this->num_nodes);

    for (int i = 0; i < this->num_nodes; ++i)
      {
	typename GT::Node * p = this->g.insert_node(new GT_Node);
	this->nodes->access(i) = p;
	this->init_node(this->g, p);

	if (this->save_parity)
	  {
	    NODE_COUNTER(p) = 0;
	    this->equal.insert(p);
	  }
      }

    this->idx_arc = unique_ptr<IndexArc<GT>> (new IndexArc<GT>(this->g) ); 
  }

  virtual void connect()
  {
    DynList<DynList<typename GT::Node*>> blk_list; // subgrafos inconexos

    {     // guarda los grados de entrada puesto que el algoritmo de Tarjan
          // los va a modificar 
      DynArray<int> in_degree; 
      in_degree.reserve(this->g.get_num_nodes());

      typename GT::Node_Iterator it(this->g);
      for (int i = 0; it.has_curr(); it.next(), ++i)
	in_degree.access(i) = NODE_COUNTER(it.get_curr());

      Tarjan_Connected_Components <GT> () (this->g, blk_list);

      it.reset_first(); // restaurar los grados de entrada
      for (int i = 0; it.has_curr(); it.next(), ++i)
	NODE_COUNTER(it.get_curr()) = in_degree.access(i);
    }

    const size_t & num_blocks = blk_list.size();

    if (num_blocks == 1) 
      return;

        // cada nodo de esta lista es un nodo del bloque i seleccionado
        // aleatoriamente  
    DynArray<typename GT::Node*> b1; b1.reserve(num_blocks);
    DynArray<typename GT::Node*> b2; b2.reserve(num_blocks);
    {
      typename DynList<DynList<GT_Node*>>::Iterator it(blk_list);
      for (int i = 0; it.has_curr(); it.next(), ++i)
      {     // seleccione dos nodos al azar del componente actual
	DynList<typename GT::Node*> & list = it.get_curr();
	b1.access(i) = this->select_random_node(list);
	b2.access(i) = this->select_random_node(list);
      }
    }

    for (int i = 0; i < num_blocks - 1; ++i)
      {
        auto src = b1.access(i);                    // nodo en bloque i
        auto tgt = b1.access((i + 1) % num_blocks); // nodo en bloque i + 1
	
	if (this->idx_arc->search_directed(src, tgt) == nullptr)
	  this->insert_arc(src, tgt);
	
	src = b2.access(i);                    // nodo en bloque i
	tgt = b2.access((i + 1) % num_blocks); // nodo en bloque i + 1

	if (this->idx_arc->search_directed(tgt, src) == nullptr)
	  this->insert_arc(tgt, src);
      }
  }

  // crea un grafo aleatorio en el cual la probabilidad por arco entre un
  // par de nodos es p
  virtual GT create_p(const size_t & __num_nodes, const double & p, 
		      bool connected)
  {
    if (p > 1.0 or p <= 0.0)
      throw std::domain_error("Invalid value for p");

    this->initialize_and_create_nodes(__num_nodes, __num_nodes);

    for (int i = 0; i < this->num_nodes; ++i)
      {
	auto src = this->nodes->access(i);
	for (int j = 0; j < this->num_nodes; ++j)
	  if (i != j and gsl_rng_uniform(this->r) <= p)
	    {
	      auto tgt = this->nodes->access(j);
	      assert(this->idx_arc->search_directed(src, tgt) == nullptr);
	      this->insert_arc(src, tgt);
	    }
      }

    if (connected)
      connect();

    return move(this->g);
  }

public:

  Random_Digraph(unsigned long seed, 
		 const Init_Node & __init_node,
		 const Init_Arc  & __init_arc)
    : Random_Graph_Base<GT, Init_Node, Init_Arc>(seed, __init_node, __init_arc)
  {
    this->g.set_digraph(true);
  } 

  Random_Digraph(unsigned long seed = time(nullptr), 
		 const Init_Node && __init_node = Init_Node(),
		 const Init_Arc  && __init_arc  = Init_Arc())
    : Random_Digraph(seed, __init_node, __init_arc)
  {
    // empty
  }

  Random_Digraph(const Init_Node & __init_node,
		 const Init_Arc  & __init_arc)
    : Random_Digraph(time(nullptr), __init_node, __init_arc)
  {
    // empty
  } 

  ~Random_Digraph() 
  {
    this->g.set_digraph(false); 
  }

  GT operator () (const size_t & __num_nodes, const size_t & __num_arcs, 
		  bool connected = true) 
  {
    return this->create(__num_nodes, __num_arcs, connected);
  }

public:

  GT operator () (const size_t & __num_nodes, const double & p, 
		  bool connected = true) 
  {
    return this->create_p(__num_nodes, p, connected);
  }

private:

  virtual void make_eulerian()
  {
    GT_Node * src = nullptr;
    GT_Node * tgt = nullptr;

    while (this->greater.size() > 0 and this->smaller.size() > 0)
      {
	do
	  {
	    tgt = this->greater.select
	      (gsl_rng_uniform_int(this->r, this->greater.size()));
	    src = this->smaller.select
	      (gsl_rng_uniform_int(this->r, this->smaller.size()));
	  }
	while (src == tgt);
      
	if (this->idx_arc->search_directed(src, tgt) == nullptr)
	  this->insert_arc(src, tgt);
	else
	  {
	    auto mid = 
	      this->equal.select(gsl_rng_uniform_int(this->r, 
						     this->equal.size()));

	    while (this->idx_arc->search_directed(src, mid) != nullptr or 
		   this->idx_arc->search_directed(mid, tgt) != nullptr)
	      mid = this->equal.select
		(gsl_rng_uniform_int(this->r, this->equal.size()));
	    
	    this->insert_arc(src, mid);
	    this->insert_arc(mid, tgt);
	  }
      }
  }

  void balance_digraph_node(GT_Node * p)
  {
    const size_t & n = this->g.get_num_nodes();
    const size_t n2 = n/2;

    while (not (this->g.get_num_arcs(p) >= n2 and NODE_COUNTER(p) >= n2))
      {
	auto q = this->nodes->access(gsl_rng_uniform_int(this->r, n));
	if (q == p)
	  continue;

	if (this->idx_arc->search_directed(p, q) == nullptr)
	  {
	    this->insert_arc(p, q);
	    NODE_COUNTER(q)++;
	  }

	if (this->idx_arc->search_directed(q, p) == nullptr)
	  {
	    this->insert_arc(q, p);
	    NODE_COUNTER(p)++;
	  }
      }
  }

  // balancea los dos nodos para que satisfagan la condición de
  // hamiltoniano. Si existe arco src-->tgt
  void balance_digraph_nodes_degree(GT_Node * src, GT_Node * tgt)
  {
    if (this->idx_arc->search_directed(src, tgt) != nullptr)
      {
	balance_digraph_node(src);
	balance_digraph_node(tgt);

	return;
      }

    const size_t & n = this->g.get_num_nodes();

    while (this->g.get_num_arcs(src) + NODE_COUNTER(tgt) < n)
      {
	auto p = this->nodes->access(gsl_rng_uniform_int(this->r, n));
	if (p == src or p == tgt)
	  continue;
	
	if (this->idx_arc->search_directed(src, p) == nullptr)
	  {
	    this->insert_arc(src, p);
	    NODE_COUNTER(p)++;
	  
	    if (this->g.get_num_arcs(src) + NODE_COUNTER(tgt) == n)
	      break;
	  }

	if (this->idx_arc->search_directed(p, tgt) == nullptr)
	  {
	    this->insert_arc(p, tgt);
	    NODE_COUNTER(tgt)++;
	  }
      }

    assert(this->g.get_num_arcs(src) + NODE_COUNTER(tgt) >= n);
  }

  virtual void make_hamiltonian ()
  {
    this->g.reset_counter_nodes();

        // contabiliza el grado de entrada por cada nodo
    for (typename GT::Arc_Iterator it(this->g); it.has_curr(); it.next())
      NODE_COUNTER(it.get_tgt_node())++;

    const size_t & n = this->g.get_num_nodes();

    for (int i = 0; i < n; ++i)
      {
	auto src = this->nodes->access(i);
	for (int j = 0; j < n; ++j)
	  {
	    if (i == j)
	      continue;

	    auto tgt = this->nodes->access(j);	    
	    balance_digraph_nodes_degree(src, tgt);
	  }
      }
  }
};

}

# endif // RANDOM_GRAPH_H
