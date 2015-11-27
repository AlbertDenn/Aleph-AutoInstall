
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


# ifndef TPL_ARRAYHEAP_H
# define TPL_ARRAYHEAP_H

# include <ahFunction.H>
# include <ahUtils.H>
# include <ahDefs.H>
# include <ahAssert.H>
# include <array_it.H>
# include <htlist.H>
# include <tpl_dynDlist.H>
# include <ahDry.H>
# include <ah-dry.H>

using namespace Aleph;

namespace Aleph {


    template <typename T, class Compare> inline
T & sift_up(T * ptr, const size_t l, const size_t r, Compare & cmp)
{
  size_t i = r;
  for (size_t p; i > l; i = p) 
    {
      p = u_index(i); // índice del padre (c = i/2)
      if (cmp(ptr[p], ptr[i])) // ¿cumple propiedad orden?
        return ptr[i]; // si, todo el arreglo es un heap

      std::swap(ptr[p], ptr[i]); // intercambie y restaure nivel p
    }

  return ptr[i];
}

    template <typename T, class Compare> inline
void sift_down(T * ptr, const size_t l, const size_t r, Compare & cmp)
{
  size_t i = l, c;
  while (true)
    {
      c = l_index(i); // índice del hijo izquierdo (c = i/2)
      if (c > r) // ¿hay hijo izquierdo?
        return; // no ==> termine

      if (c + 1 <= r) // ¿hay hijo derecho?
        if (cmp (ptr[c + 1], ptr[c])) // sí ==> escoja menor
          c++;

      if (cmp (ptr[i], ptr[c])) // ¿cumple propiedad orden?
        return;  // sí ==> termine

      std::swap(ptr[c], ptr[i]); 
      i = c;
    }
}

    template <typename T, class Compare> inline
void sift_down_up(T * ptr, const size_t l, const size_t i, const size_t r,
		  Compare & cmp)
{
  sift_down <T, Compare> (ptr, i, r, cmp);
  sift_up <T, Compare> (ptr, l, i, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void heapsort(T * array, const size_t n, Compare & cmp) 
{
  Aleph::Inversed_Compare<T, Compare> inv_cmp(cmp);

  --array; //desplazar posición hacia atrás ==> array[1] == primero
  for (int i = 2; i <= n; ++i)
    sift_up <T, Aleph::Inversed_Compare<T, Compare>> (array, 1, i, inv_cmp);
  for (int i = n; i > 1; --i)
    {
      std::swap(array[1], array[i]); // poner en raíz i-ésimo item 
      sift_down<T,Aleph::Inversed_Compare<T,Compare>>(array, 1, i - 1, inv_cmp);
    }
}

    template <typename T, class Compare = Aleph::less<T>> inline
void heapsort(T * array, const size_t n, Compare && cmp = Compare())
{
  heapsort(array, n, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void faster_heapsort(T * array, const size_t n, Compare & cmp)
{
  Aleph::Inversed_Compare<T, Compare> inv_cmp(cmp);

  --array; // desplazar posición hacia atrás ==> array[1] == primero
  for (int i = n/2; i >= 1; --i)
    sift_down(array, i, n, inv_cmp);
  for (int i = n; i > 1; --i)
    {
      std::swap(array[1], array[i]); // poner en raíz i-ésimo item 
      sift_down(array, 1, i - 1, inv_cmp);
    }
}

    template <typename T, class Compare = Aleph::less<T>> inline
void faster_heapsort(T * array, const size_t n, Compare && cmp = Compare())
{
  faster_heapsort(array, n, cmp);
}


      template <typename T, class Compare>
bool valid_heap(T * array, const size_t l, const size_t r, Compare & cmp) 
{
  size_t i;
  for (i = l_index(l) /* i = 2*l */; i <= r; i++)
    if (cmp(array[i], array[u_index (i)]))
      break;
  return i > r;
}

      template <typename T, class Compare>
bool valid_heap(T * array, const size_t l, const size_t r, 
		Compare && cmp = Compare()) 
{
  return valid_heap(array, l, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> 
class ArrayHeap :
	public LocateFunctions<ArrayHeap<T, Compare>, T>,
	public FunctionalMethods<ArrayHeap<T, Compare>, T>,
	public GenericKeys<ArrayHeap<T, Compare>, T>,
	public EqualToMethod<ArrayHeap<T, Compare>>,
	public StlAlephIterator<ArrayHeap<T, Compare>>
{
  T *            array = nullptr;
  mutable size_t dim;
  size_t         num_items = 0;

  mutable bool array_allocated = true;

  Compare & cmp;

  static size_t r_index(const size_t & i)
  {
    return (i << 1) + 1; // multiplica i por 2 y suma 1
  }

public:

  void swap(ArrayHeap & h)
  {
    std::swap(array, h.array);
    std::swap(dim, h.dim);
    std::swap(num_items, h.num_items);
    std::swap(array_allocated, h.array_allocated);
    std::swap(cmp, h.cmp);
  }

  using Item_Type = T;

  using Key_Type = T;

  Special_Ctors(ArrayHeap, T);

  ArrayHeap(const size_t d, Compare & __cmp) 
    : array(nullptr), dim(d), num_items(0), array_allocated(false), cmp(__cmp)
  { 
    array = new T [d + 1];
    array_allocated = true;
  }

  ArrayHeap(const size_t & d = 1024, Compare && cmp = Compare()) 
    : ArrayHeap(d, cmp)
  {
    assert(array_allocated);
  }

  ArrayHeap(T * ptr, const size_t & d, Compare & __cmp) 
    : array(ptr), dim(d), num_items(0), array_allocated(false), cmp(__cmp)
  { 
    // empty
  }

  ArrayHeap(T * ptr, const size_t & d, Compare && cmp = Compare()) 
    : ArrayHeap(ptr, d, cmp)
  { 
    // empty
  }

  ArrayHeap(const ArrayHeap & h)
    : dim(h.dim), num_items(h.num_items), cmp(h.cmp)
  {
    array = new T [dim + 1];
    for (size_t i = 1; i <= num_items; ++i)
      array[i] = h.array[i];
  }

  ArrayHeap(ArrayHeap && h) noexcept
    : array_allocated(false), cmp(h.cmp)
    {
      swap(h);
    }

  ArrayHeap & operator = (const ArrayHeap & h)
  {
    if (this == &h)
      return *this;

    if (dim < h.dim)
      {
	T * ptr = new T [h.dim + 1];
	if (array_allocated)
	  delete [] array;
	array = ptr;
	array_allocated = true;
	dim = h.dim;
      }

    for (size_t i = 1; i <= h.num_items; ++i)
      array[i] = h.array[i];
    num_items = h.num_items;
    cmp = h.cmp;

    return *this;
  }

  ArrayHeap & operator = (ArrayHeap && h)
  {
    swap(h);
    return *this;
  }

  virtual ~ArrayHeap() 
  {
    if (array_allocated and array != nullptr)
      delete [] array;
  }

  T & top() throw(std::exception, std::underflow_error)
  {
    if (num_items == 0)
      throw std::underflow_error("Heap is empty");

    return array[1];
  }

  T & insert(const T & key) throw(std::exception, std::overflow_error)
  {
    if (num_items >= dim)
      throw std::overflow_error("Heap out of capacity"); 

    array[++num_items] = key;  // colocar nuevo elemento
    return sift_up(array, 1, num_items, cmp); 
  }

  T & insert(T && key) throw(std::exception, std::overflow_error)
  {
    if (num_items >= dim)
      throw std::overflow_error("Heap out of capacity"); 

    array[++num_items] = std::move(key);  // colocar nuevo elemento
    return sift_up(array, 1, num_items, cmp); 
  }

  T & put(const T & key) { return insert(key); }

  T & append(const T & key) { return insert(key); }

  T & put(T && key) { return insert(std::forward<T>(key)); }

  T & append(T && key) { return insert(std::forward<T>(key)); }

  T getMin() throw(std::exception, std::underflow_error)
  {
    if (num_items == 0)
      throw std::underflow_error("Heap is empty");

    T ret_val = array[1];
    array[1] = array[num_items--]; 
    sift_down(array, 1, num_items, cmp); 
    return ret_val;
  }

  T get() throw(std::exception, std::underflow_error)
  {
    return getMin();
  }

  T getMax() throw(std::exception, std::underflow_error)
  {
    return getMin();
  }

  const size_t & size() const { return num_items; }
  
  bool is_empty() const { return num_items == 0; }

  void update(T & data) 
  {
    assert(&data >= &array[0] and &data <= &array[dim]);

    const size_t i = &data - array;
    sift_down_up(array, 1, i, num_items, cmp);
    sift_up(array, i, num_items, cmp);
  }

  void remove(T & item)
  {
    item = array[num_items--];
    update(item);
  }

  T & operator [] (const size_t i)
  {
    return array[i];
  }

  struct Iterator : public Array_Iterator<T>
  {
    // Iterator() {}

    Iterator(const ArrayHeap & h) noexcept
    : Array_Iterator<T>(h.array + 1, h.dim, h.num_items) { /* empty */ }
  };

private:

    // super fast array traversal
  template <class Operation>
  bool __traverse(Operation & operation)
  {
    for (size_t i = 1; i <= num_items; ++i)
      if (not operation(array[i]))
	return false;

    return true;
  }

public:

  template <class Operation>
  bool traverse(Operation & operation) const
  {
    return const_cast<ArrayHeap&>(*this).__traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation & operation) 
  {
    return __traverse(operation); 
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
  {
    return const_cast<ArrayHeap&>(*this).__traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) 
  {
    return traverse(operation);
  }
};

} // end namespace Aleph
# endif /* TPL_ARRAYHEAP_H */

