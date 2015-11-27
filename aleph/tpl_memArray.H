
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

# ifndef TPL_MEMARRAY_H
# define TPL_MEMARRAY_H

# include <utility>
# include <stdlib.h>
# include <math.h>
# include <stdexcept>

# include <ahUtils.H>
# include <ahDry.H>
# include <ahIterator.H>
# include <array_it.H>

namespace Aleph
{

    template <typename T>
class MemArray
{
  static const size_t Min_Dim = 4; // some day with constexp

protected:

  T *    ptr = nullptr;
  size_t dim = Min_Dim;
  size_t n   = 0;
  size_t contract_threshold;

public:

  T * get_ptr() const noexcept { return ptr; }

  size_t get_dim() const noexcept { return dim; }

protected:

  void allocate()
  {
    assert(is_power_of_2(dim));
    ptr = new T [dim];
    contract_threshold = dim / 4; 
  }

  bool expand(const size_t first = 0)
  {
    assert(is_power_of_2(dim));
    if (n < dim)
      return false;

    const size_t newsz = dim << 1; // 2*dim
    const size_t mask = dim - 1;
    T * new_ptr = new T [newsz];
    for (int i = 0; i < dim; ++i)
      {
	assert(((i + first) & mask) == ((i + first) % dim));
	std::swap(ptr[(i + first) & mask], new_ptr[i]);
      }
    delete [] ptr;
    ptr = new_ptr;
    dim = newsz;
    contract_threshold = dim/4;

    return true;
  }

  bool contract(const size_t first = 0)
  {
    if (n > contract_threshold)
      return false;

    const size_t newsz = dim >> 1; // dim/2

    if (newsz <= Min_Dim)
      return false;

    T * new_ptr = new T [newsz];

    const size_t mask = dim - 1;
    for (int i = 0; i < newsz; ++i)
      {
	assert(((first + i) & mask) == ((first + i) % dim));
	std::swap(ptr[(first + i) & mask], new_ptr[i]);
      }

    for (int i = newsz; i < dim; ++i) // call destructors 
      ptr[i].~T();

    delete [] ptr;
    ptr = new_ptr;
    dim = newsz;
    contract_threshold = dim/4;

    return true;
  }

  // if dim == 2^k returns dim; else next two power 2^k > dim 
  void init_dim(const size_t d) noexcept
  {
    assert(d > 0);

    size_t k = log2(d);
    dim = is_power_of_2(d) ? d : 1 << ++k;

    assert(dim >= d);
    assert(dim == 1 << k);
  }

public:

  using Item_Type = T; 

  size_t capacity() const noexcept { return dim; }

  size_t size() const noexcept { return n; }

  bool is_empty() const noexcept { return n == 0; }

  MemArray(size_t __dim = Min_Dim) : ptr(nullptr), n(0)
  {
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
    static_assert(std::is_move_constructible<T>::value,
		  "No move constructor for T");
    static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    static_assert(std::is_move_assignable<T>::value,
		  "No move assign for T");
    init_dim(__dim);
    allocate();
  }

  ~MemArray()
  {
    if (ptr != nullptr)
      {
	delete [] ptr;
	ptr = nullptr;
      }
  }

  void swap(MemArray & a) noexcept
  {
    std::swap(ptr, a.ptr);
    std::swap(dim, a.dim);
    std::swap(n, a.n);
    std::swap(contract_threshold, a.contract_threshold);
  }

  MemArray(const MemArray & a) 
    : ptr(nullptr), dim(a.dim), n(a.n)
  {
    allocate();
    for (int i = 0; i < dim; ++i)
      ptr[i] = a.ptr[i];
  }
  
  MemArray(MemArray && a) noexcept
  : ptr(nullptr), dim(0), n(0), contract_threshold(0)
  {
    swap(a);
  }

  MemArray & operator = (const MemArray & a)
  {
    if (this == &a)
      return *this;

    T * newptr = new T [a.dim];      // allocate new array
    for (int i = 0; i < a.n; ++i)  // copy items to new array
      newptr[i] = a.ptr[i];

    delete [] ptr;
    ptr = newptr;
    dim = a.dim;
    n = a.n;

    return *this;
  }

  MemArray & operator = (MemArray && a) noexcept
  {
    swap(a);
    return *this;
  }

  void empty() { n = 0; }

  void empty_and_release()
  {
    n = 0;
    if (dim <= Min_Dim)
      return;

    delete [] ptr;
    ptr = nullptr;
    dim = Min_Dim;
    allocate();
  }

  T & put(const T & item)
  {
    expand();

    T & ret = ptr[n++] = item;
    return ret;
  }

  T & put(T && item)
  {
    expand();
    
    T & ret = ptr[n++] = std::forward<T>(item);
    return ret;
  }

  T & append(T & item)
  {
    return put(item);
  }

  T & append(T && item)
  {
    return put(std::forward<T>(item));
  }
  
  void putn(const size_t more)
  {
    size_t new_n = n + more;
    if (new_n <= dim)
      {
	n = new_n;
	return;
      }

    size_t new_dim = dim;
    while (new_dim < new_n)
      new_dim <<= 1; // dim = 2*dim

    T * new_ptr = new T[new_dim];
    for (size_t i = 0; i < n; ++i)
      std::swap(ptr[i], new_ptr[i]);
    
    delete [] ptr;
    ptr = new_ptr;
    dim = new_dim;
    n = new_n;
  }
  
  T get(int i = 1) 
  {
    int idx = n - i;
    if (idx < 0)
      throw std::underflow_error("Deleted more entries than capacity");

    n = idx;
    T ret = std::move(ptr[n]);

    contract();

    return ret;
  }

  T & last() const noexcept { return ptr[n - 1]; }

  T & first() const noexcept { return ptr[0]; }

  T & access(const size_t i) const noexcept
  {
    return ptr[i];
  }

  T & operator [] (const size_t i) const
  {
    if (i >= n)
      throw std::range_error("access out of range");

    return ptr[i];
  }
  
  T & operator () (const size_t i) const noexcept
  {
    assert(i < dim);
    return ptr[i];
  }

  template <class Operation>
  bool traverse(Operation & operation) 
  {
    for (int i = 0; i < n; i++)
      if (not operation(ptr[i]))
	return false;

    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const
  {
    return const_cast<MemArray*>(this)->traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation) const
  {
    return traverse<Operation>(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation)
  {
    return traverse<Operation>(operation);
  }

  struct Iterator : public Array_Iterator<T>
  {
    using Base = Array_Iterator<T>;
    using Base::Base;

    Iterator(const MemArray<T> & a) noexcept
      : Array_Iterator<T>(a.ptr, a.dim, a.n)
    {}
  };
};


} // end namespace Aleph

# endif // TPL_MEMARRAY_H
