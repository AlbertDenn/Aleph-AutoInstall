
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


# ifndef TPL_ARRAYSTACK_H
# define TPL_ARRAYSTACK_H

# include <iostream>
# include <aleph.H>
# include <htlist.H>
# include <tpl_dynDlist.H>
# include <ah-dry.H>
# include <tpl_memArray.H>

using namespace std;

using namespace Aleph;

namespace Aleph {

    template <typename T> 
class ArrayStack
      : public LocateFunctions<ArrayStack<T>, T>,
	public FunctionalMethods<ArrayStack<T>, T>,
	public GenericKeys<ArrayStack<T>, T>,
	public EqualToMethod<ArrayStack<T>>,
	public StlAlephIterator<ArrayStack<T>>
{
  MemArray<T> array;

public:

  using Item_Type = T; 

  ArrayStack(size_t dim = 4) : array(dim) { /* empty */ }

  ArrayStack(const ArrayStack & s) : array(s.array) { /* empty */ }

  ArrayStack(ArrayStack && s) 
    : array(std::forward<MemArray<T>>(s.array)) { /* empty */ }

  Special_Ctors(ArrayStack, T);

  ArrayStack & operator = (const ArrayStack & s)
  {
    if (this == &s)
      return *this;

    array = s.array;

    return *this;
  }

  void swap(ArrayStack & s) noexcept
  {
    std::swap(*this, s);
  }

  ArrayStack & operator = (ArrayStack && s) noexcept
  {
    array.swap(s.array);
    return *this;
  }

  T & push(const T & data) throw(std::exception, std::overflow_error)
  {
    return array.put(data);
  }

  T & push(T && data) throw(std::exception, std::overflow_error)
  {
    return array.put(std::forward<T>(data));
  }

  T & append(const T & data)
  {
    return push(data);
  }

  T & append(T && data)
  {
    return push(std::forward<T>(data));
  }

  T & pushn(const size_t & n = 1) throw(std::exception, std::overflow_error)
  {
    array.putn(n);
    return array.last();
  }

  T pop() throw(std::exception, std::underflow_error)
  {
    return array.get(1);
  }

  T popn(size_t n) throw(std::exception, std::underflow_error)
  {
    return array.get(n);
  }

  T & top() const throw(std::exception, std::underflow_error)
  {
    return array.last();
  }

  T & base() noexcept
  {
    return array.first();
  }

  T & top(size_t i) const throw(std::exception, std::underflow_error)
  {
    const size_t sz = array.size();
    if (i >= sz)
      std::out_of_range("top index out of range");
    return array.access(sz - i - 1);
  }

  void empty() noexcept { array.empty(); }

  bool is_empty() const noexcept { return array.size() == 0; }

  size_t size() const noexcept { return array.size(); }

  size_t capacity() const noexcept { return array.capacity(); }

  template <class Operation>
  bool traverse(Operation & operation) noexcept(noexcept(operation))
  {
    return array.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return array.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
    noexcept(noexcept(operation))
  {
    return array.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation())
    noexcept(noexcept(operation))
  {
    return array.traverse(operation);
  }

  struct Iterator : public MemArray<T>::Iterator
  {
    using Base = typename MemArray<T>::Iterator;

    using Base::Base;

    Iterator(const ArrayStack<T> & s) noexcept : Base(s.array) {}
  };
};

    template <typename T> 
class FixedStack : public LocateFunctions<FixedStack<T>, T>,
		   public FunctionalMethods<FixedStack<T>, T>,
		   public GenericKeys<FixedStack<T>, T>,
		   public EqualToMethod<FixedStack<T>>,
		   public StlAlephIterator<FixedStack<T>>
{
  T *    array = nullptr; 
  size_t head = 0;
  size_t dim = 0;

public:

  using Item_Type = T; 

  FixedStack(size_t d = 512) noexcept
    : array(new T[d]), head(0), dim(d) 
  {
    /* empty */ 
  }

  ~FixedStack()
  {
    if (array)
      delete [] array;
  }

  FixedStack(const FixedStack & s) 
    : array(new T [s.dim]), head(s.head), dim(s.dim)
  {
    for (int i = 0; i < head; ++i)
      array[i] = s.array[i];
  }

  Special_Ctors(FixedStack, T);

  void swap(FixedStack & s) noexcept
  {
    std::swap(array, s.array);
    std::swap(head, s.head);
    std::swap(dim, s.dim);
  }

  FixedStack(FixedStack && s) noexcept
    : array(nullptr), head(0), dim(0)
  {
    swap(s);
  }

  FixedStack & operator = (const FixedStack & s) 
  {
    if (this == &s)
      return *this;

    T * ptr = new T [s.dim];
    if (array)
      delete [] array;
    array = ptr;
    for (size_t i = 0; i < s.head; ++i)
      array[i] = s.array[i];
    head  = s.head;
    dim   = s.dim;

    return *this;
  }

  FixedStack & operator = (FixedStack && s) noexcept
  {
    swap(s);
    return *this;
  }

  T & push(const T & data) 
  {
    assert(head < dim );

    array[head++] = data;
    return array[head - 1];
  }

  T & push(T && data) 
  {
    assert(head < dim );

    std::swap(array[head++], data);
    return array[head - 1];
  }

  T & append(const T & data) 
  {
    return push(data);
  }

  T & append(T && data) 
  {
    return push(std::forward<T>(data));
  }

  T & pushn(const size_t & n = 1) noexcept
  {
    assert(head + n <= dim);

    head += n;
    return array[head - 1];
  }

  T pop() noexcept
  {
    assert(head > 0);

    return std::move(array[--head]);
  }

  T popn(const int & n) noexcept
  {
    assert((int (head - n)) >= 0);

    head -= n;
    return std::move(array[head]);
  }

  T & top() const noexcept
  {
    assert(head > 0);

    return array[head - 1]; 
  }

  T & base() const noexcept { return array[0]; }

  T & top(size_t i) const noexcept
  {
    assert(i < head);
    return array[head - i - 1];   
  }

  bool is_empty() const noexcept { return head == 0; }

  void empty() noexcept { head = 0; }

  size_t size() const noexcept { return head; }

  template <class Operation>
  bool traverse(Operation & operation)
  {
    for (int i = 0; i < head; i++)
      if (not operation(array[i]))
	return false;

    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const
  {
    return const_cast<FixedStack*>(this)->traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
  {
    return traverse<Operation>(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation())
  {
    return traverse<Operation>(operation);
  }

  struct Iterator : public Array_Iterator<T>
  {
    using Base = Array_Iterator<T>;
    using Base::Base;

    Iterator(const FixedStack<T> & s) : Base(s.array, s.dim, s.head) {}
  };
};

} // end namespace Aleph

# endif /* TPL_ARRAYSTACK_H */

