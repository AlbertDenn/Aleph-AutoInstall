
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

# ifndef TPL_MAT_GRAPH_H
# define TPL_MAT_GRAPH_H

# include <tpl_graph.H>
# include <tpl_sort_utils.H>

using namespace Aleph;

namespace Aleph 
{

      template <typename GT> inline static 
typename GT::Node * get_node(DynArray<typename GT::Node *> & nodes, 
			     const long &                    i)
{
  if (i >= nodes.size())
    throw std::out_of_range("Index of node out of range");

  return nodes.access(i);
} 

      template <typename GT> inline static long 
node_index(DynArray<typename GT::Node *> & nodes, 
	   const long & n, typename GT::Node * p) 
{
  return Aleph::binary_search<typename GT::Node*>(nodes, p, 0, n - 1);
}

      inline static long 
index_array(const long & i, const long & j, const long & n) 
{
  if (i >= n or j >= n)
    throw std::out_of_range("Matrix index out of range");

  return i + j*n;
}

      template <typename Entry>
inline static Entry * read_matrix(const DynArray<Entry> & mat,
				  const long & i, const long & j, 
				  const long & n)
{
  const long index = index_array(i, j, n);
  if (not mat.exist(index)) 
    return nullptr;

  return &const_cast<Entry&>(mat.access(index));
}

      template <typename Entry>
inline static void write_matrix(DynArray<Entry> & mat,
				const long &      i, 
				const long &      j, 
				const long &      n, 
				const Entry &     entry)
{
  mat[index_array(i, j, n)] = entry;
}

        template <class GT, class SA = Dft_Show_Arc<GT> >
class Map_Matrix_Graph
{
public:
  typedef GT Graph_Type;

  typedef typename GT::Node_Type Node_Type;

  typedef typename GT::Arc_Type Arc_Type;

  typedef typename GT::Node Node;

  typedef typename GT::Arc Arc;

private:

  DynArray<Node*> nodes;
  GT * lgraph;
  mutable size_t num_nodes;
  SA & sa;

  struct Mat_Entry
  {
    Arc * arc;
    Mat_Entry(Arc * __arc = nullptr) : arc(__arc) { /* empty */ }
  };

  DynArray<Mat_Entry> mat;

public:

  inline void copy_list_graph(GT & g);

  Map_Matrix_Graph(GT & g, SA && __sa = SA())
    : lgraph(&g), num_nodes(g.get_num_nodes()), sa(__sa)
  {
    copy_list_graph(g); 
  }

  Map_Matrix_Graph(GT & g, SA & __sa)
    : lgraph(&g), num_nodes(g.get_num_nodes()), sa(__sa)
  {
    copy_list_graph(g); 
  }

  Map_Matrix_Graph(Map_Matrix_Graph & mat)
  {
    copy_list_graph(*(mat.lgraph));
  }

  inline Map_Matrix_Graph & operator = (Map_Matrix_Graph & mat);

  inline Map_Matrix_Graph & operator = (GT & g);

  inline Node * operator () (const long & i);

  
  inline long operator () (Node * node) const;

  inline Arc * operator () (Node * src_node, Node * tgt_node) const;
  
  inline Arc * operator () (const long & i, const long & j) const;  

  GT & get_list_graph() { return *lgraph; }

  const size_t & get_num_nodes() const { return num_nodes; }
};


    template <class GT, class SA> typename GT::Node * 
Map_Matrix_Graph<GT, SA>::operator () (const long & i) 
{
  return get_node <GT> (nodes, i); 
}

      template <class GT, class SA> long 
Map_Matrix_Graph<GT, SA>::operator () (typename GT::Node * node) const
{
  return node_index <GT> (nodes, num_nodes, node);
}

      template <class GT, class SA> typename GT::Arc * 
Map_Matrix_Graph<GT, SA>::operator () (const long & i, const long & j) const
{ 
  Mat_Entry * mat_entry = read_matrix<Mat_Entry>(mat, i, j, num_nodes); 
  if (mat_entry == nullptr) 
    return nullptr;

  return mat_entry->arc;
} 

      template <class GT, class SA> typename GT::Arc * 
Map_Matrix_Graph<GT, SA>::operator () (Node * src_node, Node * tgt_node) const
{ 
  Mat_Entry * mat_entry = 
    read_matrix<Mat_Entry>(mat, node_index<GT>(nodes, num_nodes, src_node), 
			   node_index<GT>(nodes, num_nodes, tgt_node), 
			   num_nodes); 
  if (mat_entry == nullptr)
    return nullptr;

  return mat_entry->arc;
} 

    template <class GT, class SA> Map_Matrix_Graph<GT, SA> & 
Map_Matrix_Graph<GT, SA>::operator = (Map_Matrix_Graph & mat)
{
  if (this == &mat)
    return *this;

  copy_list_graph(*(mat.lgraph));
  return *this;
}

      template <class GT, class SA>
Map_Matrix_Graph<GT, SA> & Map_Matrix_Graph<GT, SA>::operator = (GT & g)
{
  copy_list_graph(g);
}

      template <class GT, class SA>
void Map_Matrix_Graph<GT, SA>::copy_list_graph(GT & g)
{
      // copiar atributos
  lgraph    = &g;
  num_nodes = g.get_num_nodes();
  nodes.cut(); // limpiar arreglos dinámicos de contenidos anteriores
  mat.cut();
  int i = 0; 
  for (typename GT::Node_Iterator it(g); it.has_curr(); it.next(), ++i)
    nodes[i] = it.get_current_node();

  quicksort(nodes); // ordenar nodes para soportar búsqueda binaria
  for (typename GT::Node_Iterator nit(g); nit.has_current(); nit.next())
    {
      Node * src = nit.get_current_node();
      const long src_idx = node_index<GT>(nodes, num_nodes, src);

          // para cada arco de src escribirlo en la matriz mat
      for (Node_Arc_Iterator<GT, SA> ait(src, sa); ait.has_curr(); ait.next())
        {
          Arc * arc = ait.get_current_arc();
          Node * tgt = g.get_connected_node(arc, src);
          const long tgt_idx = node_index<GT>(nodes, num_nodes, tgt);
          write_matrix<Mat_Entry>(mat, src_idx, tgt_idx, num_nodes, arc);
        }
    }
}
 
      template <typename GT, class SA = Dft_Show_Arc<GT> >
class Matrix_Graph
{
public:

  typedef GT Graph_Type;

  typedef typename GT::Node_Type Node_Type;

  typedef typename GT::Arc_Type Arc_Type;

  typedef typename GT::Node Node;

  typedef typename GT::Arc Arc;

private:

  DynArray<Node_Type> nodes;
  DynArray<Arc_Type>  arcs;
  mutable size_t      n;
  mutable Arc_Type    Null_Value;
  SA &                sa;
  
  void copy(Matrix_Graph & mat)
  {
    if (this == &mat)
      return;

    n          = mat.n; // copiar atributos
    Null_Value = mat.Null_value;
    nodes.cut(); // limpiar memoria de arreglos dinámicos
    arcs.cut();
    arcs.set_default_initial_value(Null_Value);

    // recorrer mat[i,j] y copiarla a nodes[], arcs[] 
    for (int i = 0; i < n; ++i) 
      {
	nodes[i] = mat.nodes[i];
	for (int j = 0; j < n; ++j)
	  {
	    const long mat_index = index_array(i, j, n);
	    if (not arcs.exist(mat_index))
	      continue;
	    arcs.touch(mat_index) = mat.arcs.access(mat_index);
	  }
      }
  }

  void copy(GT & g)
  {
    n = g.get_num_nodes();
    DynArray<typename GT::Node *> ptr; // arreglo temporal 

    int i = 0;
    for (typename GT::Node_Iterator it(g); it.has_current(); it.next(), ++i)
      ptr[i] = it.get_current_node();

    quicksort(ptr); // ordenar por si se relaciona con Map_Matrix_Graph

    arcs.set_default_initial_value(Null_Value); 

    for (i = 0; i < n; ++i) // coloca contenidos de ptr[] en nodes[]
      {
	typename GT::Node * src = ptr[i];
	nodes[i] = src->get_info();
	for (Node_Arc_Iterator<GT, SA> it(src); it.has_current(); it.next())
	  {
	    typename GT::Arc * arc = it.get_current_arc();
	    typename GT::Node * tgt = it.get_tgt_node();
	    const long j = node_index<GT>(ptr, n, tgt);
	    const long mat_index = index_array(i, j, n);
	    arcs[mat_index] = arc->get_info();
	  }
      }
  }

public:

  const size_t & get_num_nodes() const { return n; }

  const Arc_Type & null_value() const { return Null_Value; }

  Matrix_Graph(GT & g, const Arc_Type & null, SA && __sa = SA()) 
    : Null_Value(null), sa(__sa)
  {
    copy(g);
  }
  
  Matrix_Graph(GT & g, const Arc_Type & null, SA & __sa) 
    : Null_Value(null), sa(__sa)
  {
    copy(g);
  }
  
  Matrix_Graph(Matrix_Graph & mat) 
  {
    copy(mat);
  }
  
  Matrix_Graph & operator = (Matrix_Graph & mat) 
  {
    copy(mat);
    return *this;
  }

  Matrix_Graph & operator = (GT & g) 
  {
    copy(g);
    return *this;
  }

  const Arc_Type & operator () (const long & i, const long & j) const
  {
    const long mat_index = index_array(i, j, n);
    if (not arcs.exist(mat_index)) // ¿Existe entrada?
      return Null_Value; // No ==> no hay arco

    return arcs.access(mat_index);      
  }

  Node_Type & operator () (const long & i) const
  {
    if (i >= n)
      throw std::out_of_range("node index out of range");

    return const_cast<Node_Type &>(nodes.access(i));
  }

  Arc_Type & operator () (const long & i, const long & j) 
  {
    const long mat_index = index_array(i, j, n);

    return arcs.touch(mat_index);
  }
};

  template <class GT, typename __Entry_Type, class SA = Dft_Show_Arc<GT> >
class Ady_Mat
{
public:

  typedef GT Graph_Type;

  typedef __Entry_Type Entry_Type;

  typedef typename GT::Node_Type Node_Type;

  typedef typename GT::Arc_Type Arc_Type;

  typedef typename GT::Node Node;

  typedef typename GT::Arc Arc;

private:

  GT *                 lgraph;
  DynArray<Node*>      nodes;
  DynArray<Entry_Type> mat;
  mutable size_t       num_nodes;
  mutable Entry_Type   Null_Value;

  void test_same_graph(Ady_Mat & mat)
  {
    if (lgraph == mat.lgraph) 
      return;

    throw 
      std::domain_error("mat does not refers the same GT than this");
  }

  void copy_nodes(GT & g)
  {
    lgraph     = &g;
    num_nodes  = g.get_num_nodes();
    nodes.cut(); // limpiar memoria por los nodos

          // coloca punteros a nodos en el arreglo nodes[]
    int i = 0;
    for (typename GT::Node_Iterator it(g); it.has_current(); it.next(), ++i)
      nodes[i] = it.get_current_node();

    quicksort(nodes); // ordena para luego hacer búsqueda binaria
  }

  Ady_Mat & copy(Ady_Mat & mat)
  {
    if (this == &mat) 
      return *this;

    test_same_graph(mat);
    Null_Value = mat.Null_Value;
    copy(mat.lgraph);
    
    return *this;
  }

public:

  Node * operator () (const long & i) 
  {
    return Aleph::get_node<GT>(nodes, i);
  }

  long operator () (Node * node) const
  {
    return node_index<GT>(nodes, num_nodes, node);
  }

  Entry_Type & operator () (const long & i, const long & j)
  {
    return mat.touch(index_array(i, j, num_nodes));
  }

  const Entry_Type & operator () (const long & i, const long & j) const
  {
    const long index = index_array(i, j, num_nodes);

    if (mat.exist(index)) 
      return mat.access(index);
        
    return Null_Value;
  }

  Entry_Type & operator () (Node * src, Node * tgt) 
  {
    return (*this)(node_index <GT> (nodes, num_nodes, src),
		   node_index <GT> (nodes, num_nodes, tgt));
  }

  const Entry_Type & operator () (Node * src, Node * tgt) const
  {
    return (*this)(node_index <GT> (nodes, num_nodes, src),
		   node_index <GT> (nodes, num_nodes, tgt));
  }

  GT & get_list_graph() { return *lgraph; }

  const Entry_Type & null_value() const { return Null_Value; }

  const size_t & get_num_nodes() const { return num_nodes; }

  Ady_Mat(GT & g) : lgraph(&g), num_nodes(lgraph->get_num_nodes()) 
  {
    copy_nodes(g); 
  }

  Ady_Mat(GT & g, const Entry_Type & null) 
    : lgraph(&g), num_nodes(lgraph->get_num_nodes()), Null_Value(null) 
  {
    copy_nodes(*lgraph); 
  }
  
  void set_null_value(const Entry_Type & null) { Null_Value = null; }
  
  Ady_Mat(Ady_Mat & mat) 
  {
    copy(mat);
  }

  template <class Operation> void operate_all_arcs_list_graph();

  template <class Operation> void operate_all_arcs_list_graph(void * ptr);
  
  template <class Operation> void operate_all_arcs_matrix();

      template <class Operation> 
  void operate_all_arcs_matrix(void * ptr); 
}; 


      template <class GT, typename __Entry_Type, class SA>
      template <class Operation>
void Ady_Mat<GT, __Entry_Type, SA>::operate_all_arcs_list_graph()
{      // recorrer todos los nodos del grafo
  for (typename GT::Node_Iterator nit(*lgraph); nit.has_current(); nit.next())
    {
      Node * src = nit.get_current_node();
      const long src_idx = node_index<Graph_Type>(nodes, num_nodes, src);

            // recorrer todos los arcos del nodo origen
      for (Node_Arc_Iterator<GT, SA> at(src); at.has_current(); at.next())
	{
	  Arc * arc = at.get_current_arc();
	  const long tgt_idx = 
	    node_index<Graph_Type>(nodes, num_nodes, arc->get_tgt_node());
	  Entry_Type & entry = 
	    mat.touch(index_array(src_idx, tgt_idx, num_nodes));

	  Operation () (*this, arc, src_idx, tgt_idx, entry);
	}
    }
}

      template <class GT, typename __Entry_Type, class SA>
      template <class Operation>
void Ady_Mat<GT, __Entry_Type, SA>::operate_all_arcs_matrix()
{
  const long & n = num_nodes;
  for (int s = 0; s < n; ++s)
    {
      Node * src_node = get_node<GT>(nodes, s);
      for (int t = 0; t < n; ++t)
	{
	  Node * tgt_node = get_node<GT>(nodes, t);
	  Entry_Type & entry = mat.touch(index_array(s, t, num_nodes));

	  Operation () (*this, src_node, tgt_node, s, t, entry);
	}
    }
}

      template <class GT, typename __Entry_Type, class SA>
      template <class Operation>
void Ady_Mat<GT, __Entry_Type, SA>::operate_all_arcs_list_graph(void * ptr)
{      // recorrer todos los nodos del grafo
  for (typename GT::Node_Iterator nit(*lgraph); nit.has_current(); nit.next())
    {
      Node * src = nit.get_current_node();
      const long src_idx = node_index<Graph_Type>(nodes, num_nodes, src);

            // recorrer todos los arcos del nodo origen
      for (Node_Arc_Iterator<GT, SA> at(src); at.has_current(); at.next())
	{
	  Arc * arc = at.get_current_arc();
	  Node * tgt = lgraph->get_tgt_node(arc);
	  const long tgt_idx = node_index<Graph_Type>(nodes, num_nodes, tgt);
	  Entry_Type & entry = // asegurar acceso a mat(src_idx, tgt_idx)
	    mat.touch(index_array(src_idx, tgt_idx, num_nodes));

	  Operation () (*this, arc, src_idx, tgt_idx, entry, ptr);
	}
    }
}

      template <class GT, typename __Entry_Type, class SA>
      template <class Operation>
void Ady_Mat<GT, __Entry_Type, SA>::operate_all_arcs_matrix(void * ptr)
{
  const long & n = num_nodes;

  for (int s = 0; s < n; ++s)
    {
      Node * src_node = get_node<GT>(nodes, s);

      for (int t = 0; t < n; ++t)
	{
	  Node * tgt_node = get_node<GT>(nodes, t);
	  Entry_Type & entry = mat.touch(index_array(s, t, num_nodes));
	  Operation () (*this, src_node, tgt_node, s, t, entry, ptr);
	}
    }
}

      template <class GT, class SA = Dft_Show_Arc<GT> >
class Bit_Mat_Graph
{
public:

  typedef GT Graph_Type;

  typedef typename GT::Node Node;

  typedef typename GT::Arc Arc;

private:

  BitArray bit_array;
  GT *                         lgraph;
  DynArray<typename GT::Node*> nodes;
  mutable size_t               n;
  
  void copy_list_graph(GT & g)
  {
    n = g.get_num_nodes();
    nodes.cut(); // liberar memoria 

    // copiar todos los nodos de g al arreglo nodes[]
    int i = 0;
    for (typename GT::Node_Iterator it(g); it.has_current(); it.next())
      nodes[i++] = static_cast<typename GT::Node*>(it.get_current_node());

    quicksort(nodes); // ordenar para luego hacer búsqueda binaria

    // recorrer todos los nodos de g para asignar los arcos en bit_array
    for (typename GT::Node_Iterator it(g); it.has_current(); it.next())
      {
	typename GT::Node * src = it.get_current_node();
	const size_t src_idx = node_index<Graph_Type>(nodes, n, src);
          
	// recorrer los arcos de src para asignar entrada en matriz
	for (Node_Arc_Iterator<GT, SA> jt(src); jt.has_current(); jt.next())
	  { 
	    typename GT::Node * tgt = jt.get_tgt_node();
	    const size_t tgt_idx = node_index<Graph_Type>(nodes, n, tgt);
	    bit_array[index_array(src_idx, tgt_idx, n)] = 1;
	  }
      }
  }

  struct Proxy
  {
    BitArray & bit_array;

    const size_t bit_index;

    Proxy(Bit_Mat_Graph & __bitmat, const long & i, const long & j)
      : bit_array(__bitmat.bit_array), bit_index(index_array(i, j, __bitmat.n))
    {
      // empty
    }

    Proxy& operator = (const Proxy & proxy)
    {
      bit_array[bit_index] = proxy.bit_array[proxy.bit_index];
    }
      
    Proxy& operator = (const int & i)
    {
      bit_array[bit_index] = i;

      return *this;
    }

    operator int () const
    {
      return bit_array[bit_index];
    }
  };

public: 

  const size_t & get_num_nodes() const { return n; }

  Bit_Mat_Graph() : lgraph(nullptr) {}

  Bit_Mat_Graph(GT & g) 
    : bit_array(g.get_num_nodes()*g.get_num_nodes()), lgraph(&g)
  { 
    copy_list_graph(g); 
  }
  
  Bit_Mat_Graph(const Bit_Mat_Graph & bitmat)
    : bit_array(bitmat.bit_array), lgraph(bitmat.lgraph), 
      nodes(bitmat.nodes), n(bitmat.n) 
  {
    // empty
  }

  Bit_Mat_Graph(const size_t & dim) 
    : bit_array(dim*dim), lgraph(nullptr), nodes(dim), n(dim) 
  {
    // empty
  }

  void set_list_graph(GT & g)
  {
    const size_t & n = g.get_num_nodes();
    bit_array.set_size(n*n);
    lgraph = &g;
    copy_list_graph(g);
  }

  GT * get_list_graph() { return lgraph; }

  Bit_Mat_Graph & operator = (const Bit_Mat_Graph & bitmat)
  {
    if (this == &bitmat) 
      return *this;

    lgraph    = bitmat.lgraph;
    bit_array = bitmat.bit_array;

    return *this;
  }

  Bit_Mat_Graph & operator = (GT & g)
  {
    if (&g == lgraph) 
      return *this;

    copy_list_graph(g);

    return *this;
  }
  
  Node * operator () (const long & i) 
  {
    if (lgraph == nullptr)
      throw std::domain_error("There is no a GT object");

    return Aleph::get_node<GT>(nodes, i);
  }
  long operator () (Node * node) const
  {
    if (lgraph == nullptr)
      throw std::domain_error("There is no a GT object");

    return node_index<GT>(nodes, n, node);
  }

  Proxy operator () (const long & i, const long & j)
  {
    return Proxy(*this, i, j);
  }

  Proxy operator () (const long & i, const long & j) const
  {
    return Proxy(const_cast<Bit_Mat_Graph&>(*this), i, j);
  }

  Proxy operator () (Node * src_node, Node * tgt_node) 
  {
    if (lgraph == nullptr)
      throw std::domain_error("There is no a GT object");

    return Proxy(*this, node_index<GT>(nodes, n, src_node), 
		 node_index<GT>(nodes, n, tgt_node)); 
  }

  Proxy operator () (Node * src_node, Node * tgt_node) const
  {
    if (lgraph == nullptr)
      throw std::domain_error("There is no a GT object");

    return Proxy(*this, node_index<GT>(nodes, n, src_node), 
		 node_index<GT>(nodes, n, tgt_node)); 
  }
};

} // end namespace Aleph

# endif // TPL_MAT_GRAPH_H
