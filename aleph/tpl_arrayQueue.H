
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


# ifndef TPL_ARRAYQUEUE_H
# define TPL_ARRAYQUEUE_H

# include <ahAssert.H>
# include <ahDefs.H>
# include <htlist.H>
# include <ah-dry.H>
# include <tpl_dynDlist.H>
# include <tpl_memArray.H>

using namespace Aleph;

namespace Aleph {

    template <typename T> 
class ArrayQueue 
      : public MemArray<T>,
	public LocateFunctions<ArrayQueue<T>, T>,
	public FunctionalMethods<ArrayQueue<T>, T>,
	public GenericKeys<ArrayQueue<T>, T>,
	public EqualToMethod<ArrayQueue<T>>,
	public StlAlephIterator<ArrayQueue<T>>
{
private:

  size_t front_index; // items are extracted from this point
  size_t rear_index;  // new item are inserted by this point

  void increase_index(size_t & i, const size_t inc = 1) const noexcept
  {
    i += inc;
    i %= this->dim;
  }

  T & rear_item(const size_t i = 0) const noexcept
  {
    return this->access((size_t) (rear_index - i - 1) % this->dim);
  }

public:

  void swap(ArrayQueue & q) noexcept
  {
    this->MemArray<T>::swap(q);
    std::swap(front_index, q.front_index);
    std::swap(rear_index, q.rear_index);
  }

  ArrayQueue(const size_t sz = 8) 
    : MemArray<T>(sz), front_index(0), rear_index(0)
  {
    // empty
  }

  ArrayQueue(const ArrayQueue & q)
    : MemArray<T>(q), front_index(q.front_index), rear_index(q.rear_index)
  {

  }

  ArrayQueue(ArrayQueue && q) 
    : MemArray<T>(std::forward<ArrayQueue>(q)), 
      front_index(q.front_index), rear_index(q.rear_index)
  {
    // empty
  }

  Special_Ctors(ArrayQueue, T);

  ArrayQueue & operator = (const ArrayQueue & q)
  {
    if (this == &q)
      return *this;

    static_cast<MemArray<T>&>(*this) = q;
    
    front_index = q.front_index;
    rear_index = q.rear_index;

    return *this;
  }

  ArrayQueue & operator = (ArrayQueue && q)
  {
    std::swap(front_index, q.front_index);
    std::swap(rear_index, q.rear_index);
    static_cast<MemArray<T>*>(this)->swap(q);
    return *this;
  }

private:

  T & complete_put() noexcept
  {
    T & ret_val = this->access(rear_index);
    increase_index(rear_index);
    ++this->n;
    return ret_val;
  }

public:

  T & put(const T & item) throw(std::exception, std::bad_alloc)
  {
    if (this->expand(front_index))
      {
	front_index = 0;
	rear_index = this->n; 
      }
    this->access(rear_index) = item;
    return complete_put();
  }

  T & put(T && item) throw(std::exception, std::bad_alloc)
  {
    if (this->expand(front_index))
      {
	front_index = 0;
	rear_index = this->n; 
      }
    this->access(rear_index) = std::forward<T>(item);
    return complete_put();
  }

  T & append(const T & item) { return put(item); }

  T & append(T && item) { return put(std::forward<T>(item)); }

  T & putn(size_t sz) throw(std::exception, std::bad_alloc)
  {
    const size_t max_sz = 2*this->dim - this->n;
    if (sz > max_sz)
      throw std::overflow_error("Maximum putn size reached");

    const int avail_n = this->dim - this->n;
    if (avail_n <= sz)
      {
	sz -= this->dim - this->n;
	this->n = this->dim;
	this->expand(front_index);
      }

    increase_index(rear_index, sz);
    this->n += sz;
    return rear_item();
  }

  T get() throw(std::exception, std::underflow_error, std::bad_alloc)
  {
    if (this->n == 0)
      throw std::underflow_error ("queue is empty");

    T ret_val = std::forward<T>(this->access(front_index)); 
    this->n--;
    increase_index(front_index);

    if (this->contract(front_index))
      {
	front_index = 0;
	rear_index = this->n;
      }

    return ret_val;
  }

  T & getn(const size_t i) throw(std::exception, std::underflow_error)
  {
    if (i >= this->n)
      throw std::underflow_error ("queue is empty");

    this->n -= i;
    increase_index(front_index, i);

    if (this->contract(front_index))
      {
	front_index = 0;
	rear_index = this->n;
      }

    return this->access(front_index);
  } 

  T & front(const size_t i = 0) const throw(std::exception, std::range_error)
  {
    if (i >= this->n) 
      throw std::range_error ("index of front out of range");

    return this->access((front_index + i) % this->dim);
  }

  T & rear(const size_t i = 0) const throw(std::exception, std::range_error)
  {
    if (i >= this->n) 
      throw std::range_error ("index of rear out of range");

    return rear_item(i);
  }

  template <class Operation>
  bool traverse(Operation & operation)
  {
    if (this->n == 0)
      return true;

    for (size_t i = front_index; i != rear_index; increase_index(i))
      if (not operation(this->access(i)))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const 
  {
    return const_cast<ArrayQueue*>(this)->traverse(operation);
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

  struct Iterator : public MemArray<T>::Iterator
  {
    using Base = typename MemArray<T>::Iterator;
    using Base::Base;

    Iterator(const ArrayQueue & q) noexcept
      : Base(q.ptr, q.dim, q.n, q.front_index, q.rear_index - 1) { }
  };
};


    template <typename T> 
class FixedQueue 
      : public LocateFunctions<FixedQueue<T>, T>,
	public FunctionalMethods<FixedQueue<T>, T>,
	public GenericKeys<FixedQueue<T>, T>,
	public EqualToMethod<FixedQueue<T>>,
	public StlAlephIterator<FixedQueue<T>>
{
private:

  size_t dim;
  T *    array;
  size_t front_index; /* index of oldest inserted item */
  size_t rear_index;
  size_t mask;
  size_t num_items; 

  void increase_index(size_t & i, const size_t inc = 1) const noexcept
  {
    assert( ((i + inc)%dim) == ((i+ inc)&mask) );

    i += inc;
    i &= mask;  
  }

  T & rear_item(const size_t i = 0) const noexcept
  {
    assert(static_cast<size_t>((rear_index - i - 1) & mask) == 
      (rear_index - i - 1)%dim);

    return array[static_cast<size_t> ((rear_index - i - 1) & mask)];
  }

public:

  using Item_Type = T;

  void swap(FixedQueue & q) noexcept
  {
    std::swap(dim, q.dim);
    std::swap(array, q.array);
    std::swap(front_index, q.front_index);
    std::swap(rear_index, q.rear_index);
    std::swap(mask, q.mask);
    std::swap(num_items, q.num_items);
  }

  FixedQueue(size_t d = 128)  
    : array(nullptr), front_index(0), rear_index(0), num_items(0)
  { 
    size_t k = log2(d);
    dim = is_power_of_2(d) ? d : 1 << ++k;
    mask = dim - 1;
    array = new T [dim];
  }
      
  ~FixedQueue()
  {
    if (array != nullptr)
      delete [] array;
  }

  FixedQueue(const FixedQueue & q) 
    : dim(q.dim), array(new T [dim]), front_index(q.front_index), 
    rear_index(q.rear_index), mask(q.mask), num_items(q.num_items)
  {
    for (size_t i = front_index; i != rear_index; ++i)
      array[i] = q.array[i];
  }

  FixedQueue(FixedQueue && q) noexcept : FixedQueue() 
  {
    swap(q);
  }

  Special_Ctors(FixedQueue, T);

  FixedQueue & operator = (const FixedQueue & q)
  {
    if (this == &q)
      return *this;

    if (array != nullptr)
      delete [] array;

    new (this) FixedQueue(q);
    return *this;
  }

  FixedQueue & operator = (FixedQueue && q) noexcept
  {
    swap(q);
    return *this;
  }

  T & put(const T& item) throw(std::exception, std::overflow_error)
  {
    assert(num_items < dim);
    array[rear_index] = item;
    T & ret_val = array[rear_index];
    increase_index(rear_index);
    num_items++;
    return ret_val;
  }

  T & put(T && item) throw(std::exception, std::overflow_error)
  {
    assert(num_items < dim);
    array[rear_index] = std::forward<T>(item);
    T & ret_val = array[rear_index];
    increase_index(rear_index);
    num_items++;
    return ret_val;
  }

  T & append(const T& item) { return put(item); }

  T & append(T && item) { return put(std::forward<T>(item)); }

  T & putn(const size_t n) throw(std::exception, std::overflow_error)
  {
    assert(num_items + n < dim);
    increase_index(rear_index, n);
    num_items += n;
    return rear_item();
  }

  T get() throw(std::exception, std::underflow_error)
  {
    assert(num_items > 0);
    num_items--;
    T ret_val = std::move(array[front_index]);
    increase_index(front_index);
    return ret_val;
  }

  T & getn(const size_t n) throw(std::exception, std::underflow_error)
  {
    assert(num_items >= n);
    num_items -= n;
    increase_index(front_index, n);
    return array[front_index];
  } 

  T & front(const size_t i = 0) const throw(std::exception, std::range_error)
  {
    assert(i < num_items);
    return array[front_index + i];
  }

  T & rear(const size_t i = 0) const throw(std::exception, std::range_error)
  {
    assert(i < num_items);
    return rear_item(i);
  }

  size_t size() const noexcept { return num_items; }

  bool is_empty() const noexcept { return num_items == 0; }

  size_t capacity() const noexcept { return dim; }

  template <class Operation>
  bool traverse(Operation & operation)
  {
    if (num_items == 0)
      return true;
    for (size_t i = front_index; i != rear_index; increase_index(i))
      if (not operation(array[i]))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const 
  {
    return const_cast<FixedQueue*>(this)->traverse(operation);
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

  struct Iterator : public MemArray<T>::Iterator
  {
    using Base = typename MemArray<T>::Iterator;
    using Base::Base;

    Iterator(const FixedQueue & q) noexcept
      : Base(q.array, q.dim, q.num_items, q.front_index, q.rear_index - 1) { }
  };
};

} // end namespace Aleph

# endif /* TPL_ARRAYQUEUE_H */

