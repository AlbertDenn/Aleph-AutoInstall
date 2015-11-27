
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


# ifndef TPL_ARRAY_H
# define TPL_ARRAY_H

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
class Array
      : public LocateFunctions<Array<T>, T>,
	public FunctionalMethods<Array<T>, T>,
	public GenericKeys<Array<T>, T>,
	public EqualToMethod<Array<T>>,
	public StlAlephIterator<Array<T>>
{
  MemArray<T> array;

public:

  using Item_Type = T; 

  Array(size_t dim = 32) : array(dim) { /* empty */ }

  Array(const Array & s) : array(s.array) { /* empty */ }

  Array(Array && s) : array(std::forward<MemArray<T>>(s.array)) { /* empty */ }

  Special_Ctors(Array, T);

  Array & operator = (const Array & s)
  {
    if (this == &s)
      return *this;

    array = s.array;

    return *this;
  }

  void swap(Array & s) noexcept
  {
    std::swap(*this, s);
  }

  Array & operator = (Array && s) noexcept
  {
    array.swap(s.array);
    return *this;
  }

  T & append(const T & data) throw(std::exception, std::overflow_error)
  {
    return array.put(data);
  }

  T & append(T && data) throw(std::exception, std::overflow_error)
  {
    return array.put(std::forward<T>(data));
  }

  void putn(const size_t n) { array.putn(n); }

  T & base() const noexcept { return array.first(); }

  void empty() noexcept { array.empty(); }

  bool is_empty() const noexcept { return array.size() == 0; }

  size_t size() const noexcept { return array.size(); }

  size_t capacity() const noexcept { return array.capacity(); }

  T & operator [] (size_t i) const
  {
    return array[i];
  }
  
  T & operator () (const size_t i) const noexcept
  {
    return array(i);
  }

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

    Iterator(const Array<T> & s) noexcept : Base(s.array) {}
  };
};


} // end namespace Aleph

# endif /* TPL_ARRAY_H */

