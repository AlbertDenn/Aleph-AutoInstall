
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

# ifndef TPL_DYNARRAYHEAP_H
# define TPL_DYNARRAYHEAP_H

# include <tpl_dynArray.H>

namespace Aleph {


    template <typename T, class Compare> inline
void sift_up(DynArray<T> & a, size_t l, size_t r, Compare & cmp) noexcept
{
  for (size_t p, i = r; i > l; i = p) 
    {
      p = u_index(i);
      T & ap = a.access(p);
      T & ai = a.access(i);
      if (cmp (ap, ai)) // ¿cumple propiedad orden?
        return; // si, todo el arreglo es un heap

      std::swap(ap, ai); // intercambie y restaure nivel p
    }
}

    template <typename T, class Compare> inline
void sift_down(DynArray<T> & a, size_t l, size_t r, Compare & cmp) noexcept
{
  size_t i = l, c;
  while (true)
    {
      c = l_index(i); // índice del hijo izquierdo (c = i/2)
      if (c > r) // ¿hay hijo izquierdo?
        return; // no ==> termine

      T * ac = & a.access(c);
      if (c + 1 <= r) // ¿hay hijo derecho?
	{
	  T * ac1 =  & a.access(c + 1);
	  if (cmp(*ac1, *ac)) // sí ==> escoja menor
	    {
	      c++;
	      ac = ac1;
	    }
	}

      T & ai = a.access(i);
      if (cmp(ai, *ac)) // ¿cumple propiedad orden?
        return;  // sí ==> termine

      std::swap(*ac, ai); 
      i = c;
    }
}


    template <typename T, class Compare = Aleph::less<T>> 
class DynArrayHeap :
    public LocateFunctions<DynArrayHeap<T, Compare>, T>,
    public FunctionalMethods<DynArrayHeap<T, Compare>, T>,
    public GenericKeys<DynArrayHeap<T, Compare>, T>,
    public EqualToMethod<DynArrayHeap<T, Compare>>,
    public StlAlephIterator<DynArrayHeap<T, Compare>>
{
  DynArray<T> array;
  size_t      num_items;

  Compare & cmp;

  static size_t r_index(const size_t & i) noexcept
  {
    return (i << 1) + 1; // multiplica i por 2 y suma 1
  }

public:

  using Item_Type = T;

  DynArrayHeap(Compare & __cmp) : num_items(0), cmp(__cmp)
  { 
    // empty
  }

  DynArrayHeap(Compare && cmp = Compare()) : DynArrayHeap(cmp)
  { 
    // empty
  }

  Special_Ctors(DynArrayHeap, T);

  T & top() throw(std::exception, std::underflow_error)
  {
    if (num_items == 0)
      throw std::underflow_error("Heap is empty");

    return array.access(1);
  }

  T & insert(const T & key) throw(std::exception, std::overflow_error)
  {
    array.touch(++num_items) = key;  // colocar nuevo elemento
    sift_up(array, 1, num_items, cmp); // restaurar propiedad orden
    return array.access(num_items);
  }

  T & insert(T && key) throw(std::exception, std::overflow_error)
  {
    array.touch(++num_items) = move(key);  // colocar nuevo elemento
    sift_up(array, 1, num_items, cmp); // restaurar propiedad orden
    return array.access(num_items);
  }

  void reserve(size_t n) 
  {
    if (num_items > n)
      throw out_of_range("DynArrayHeap: n is greater than current heap size");
    array.reserve(n); 
  }

  T & insert_direct(const T & key) noexcept
  {
    array(++num_items) = key;  // colocar nuevo elemento
    sift_up(array, 1, num_items, cmp); // restaurar propiedad orden
    return array.access(num_items);
  }

  T & insert_direct(T && key) noexcept
  {
    array(++num_items) = move(key);  // colocar nuevo elemento
    sift_up(array, 1, num_items, cmp); // restaurar propiedad orden
    return array.access(num_items);
  }

  T & put(const T & key) { return insert(key); }

  T & put(T && key) { return insert(std::forward<T>(key)); }

  T & append(const T & key) { return insert(key); }

  T & append(T && key) { return insert(std::forward<T>(key)); }

  T getMin() throw(std::exception, std::underflow_error)
  {
    if (num_items == 0)
      throw std::underflow_error("Heap is empty");

    T & a1 = array(1);
    T ret_val = move(a1);
    a1 = move(array(num_items--)); 
    sift_down(array, 1, num_items, cmp); // propiedad orden

    array.cut(num_items + 1);

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

  const size_t & size() const noexcept { return num_items; }
  
  bool is_empty() const noexcept { return num_items == 0; }

  struct Iterator : DynArray<T>::Iterator
  {
    using Base = typename DynArray<T>::Iterator;

    Iterator(const DynArrayHeap & h) noexcept : Base(h.array) 
    {
      if (h.num_items != 0)
	this->next();
    }

    Iterator() noexcept { /* empty */ }

    bool has_curr() const noexcept
    {
      return this->curr_idx != 0 and this->curr_idx != this->array_ptr->size();
    }

    long get_pos() const noexcept { return this->Base::get_pos() - 1; }
  };

  template <class Operation>
  bool traverse(Operation & operation) noexcept(noexcept(operation))
  {
    for (Iterator it(*this); it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return const_cast<DynArrayHeap&>(*this).traverse<Operation>(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
    noexcept(noexcept(operation))
  {

    return traverse<Operation>(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) 
    noexcept(noexcept(operation))
  {
    return traverse<Operation>(operation);
  }
};



} // end namespace Aleph

# endif // TPL_DYNARRAYHEAP_H
