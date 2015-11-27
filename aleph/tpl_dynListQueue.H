
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


# ifndef TPL_DYNLISTQUEUE_H
# define TPL_DYNLISTQUEUE_H

# include <iostream>
# include <ahDry.H>
# include <ahIterator.H>
# include <htlist.H>
# include <tpl_dynDlist.H>

using namespace std;


namespace Aleph {

    template <typename T> 
class DynListQueue 
      : public LocateFunctions<DynListQueue<T>, T>,
	public FunctionalMethods<DynListQueue<T>, T>,
	public GenericKeys<DynListQueue<T>, T>,
	public EqualToMethod<DynListQueue<T>>,
	public StlAlephIterator<DynListQueue<T>>
{
  DynList<T> q;
  size_t num_items = 0;

  using Base = DynList<T>;

public:

  void swap(DynListQueue & __q) noexcept
  {
    std::swap(num_items, __q.num_items);
    q.swap(__q.q);
  }

  DynListQueue() noexcept : num_items(0) { /* empty */ }

  DynListQueue(const DynListQueue & __q) : q(__q.q), num_items(__q.num_items)
  {
    // empty
  }

  DynListQueue(DynListQueue && __q) noexcept
  {
    swap(__q);
  }

  Special_Ctors(DynListQueue, T);

  DynListQueue & operator = (const DynListQueue & rhs)
  {
    if (this == &rhs)
      return *this;

    q = rhs.q;
    num_items = rhs.num_items;

    return *this;
  }

  DynListQueue & operator = (DynListQueue && rhs) noexcept
  {
    std::swap(num_items, rhs.num_items);
    q.swap(rhs.q);
    return *this;
  }

  size_t size() const noexcept { return num_items; }

  bool is_empty() const noexcept { return q.is_empty(); }

  using Set_Type = DynListQueue; 

  using Item_Type = T; 

  T & put(const T & data) 
  {
    T & ret_val = q.append(data);
    ++num_items;
    return ret_val;
  }

  T & put(T && data) noexcept(noexcept(std::forward<T>(data)))
  {
    T & ret_val = q.append(std::forward<T>(data));
    ++num_items;
    return ret_val;
  }

  T & append(const T & data) { return put(data); }

  T & append(T && data) noexcept(noexcept(std::forward<T>(data)))
  {
    return put(std::forward<T>(data)); 
  }

  T get() 
  {
    T ret_val = q.remove_first();
    --num_items;
    return ret_val;
  }
 
  T & front() const
  {
    return q.get_first();
  }

  T & rear() const 
  {
    return q.get_last();
  } 

  void empty() noexcept { q.empty(); }

  template <class Operation>
  bool traverse(Operation & operation) noexcept(noexcept(operation))
  {
    return q.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return q.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
    noexcept(noexcept(operation))
  {
    return q.traverse(std::forward<Operation>(operation));
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) 
    noexcept(noexcept(operation))
  {
    return q.traverse(std::forward<Operation>(operation));
  }

  struct Iterator : public DynList<T>::Iterator
  {
    using Base = typename DynList<T>::Iterator;
    using Base::Base;

    Iterator(const DynListQueue<T> & q) noexcept : Base(q.q) {}
  };
};

} // end namespace Aleph
# endif // TPL_DYNLISTQUEUE_H

