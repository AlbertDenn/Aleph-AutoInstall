
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


# ifndef TPL_RANDOM_QUEUE_H
# define TPL_RANDOM_QUEUE_H

# include <gsl/gsl_rng.h>
# include <htlist.H>
# include <ah-dry.H>
# include <tpl_dynArray.H>

namespace Aleph {

    template <class T> 
class Random_Set
      : public LocateFunctions<Random_Set<T>, T>,
	public FunctionalMethods<Random_Set<T>, T>,
	public GenericKeys<Random_Set<T>, T>,
	public EqualToMethod<Random_Set<T>>,
	public StlAlephIterator<Random_Set<T>>
{
private:

  DynArray<T> array;
  gsl_rng * r = nullptr;

public:

  using Set_Type = Random_Set; 

  using Item_Type = T; 

  gsl_rng * get_rng() const { return r; }

  void set_seed(unsigned long seed) noexcept { gsl_rng_set(r, seed); }

  size_t size() const { return array.size(); }
  
  Random_Set() : array(0), r(nullptr)
  {
    r = gsl_rng_alloc (gsl_rng_mt19937);
    if (r == nullptr)
      throw std::bad_alloc();

    gsl_rng_set(r, time(nullptr) % gsl_rng_max(r));
  }

  void swap(Random_Set & s)
  {
    array.swap(s.array);
    std::swap(r, s.r);
  }

  Random_Set(const Random_Set & s) : Random_Set()
  {
    s.for_each([this] (auto item) { this->append(item); });
  }

  Random_Set(Random_Set && s) { swap(s); }

  Random_Set & operator = (const Random_Set & s)
  {
    if (this == &s)
      return *this;

    if (s.size() > size())
      array.reserve(s.size());
    else
      array.cut(s.size);
    s.for_each([this] (auto item) { this->append(item); });

    return *this;
  }

  Random_Set & operator = (Random_Set && s) 
  {
    swap(s);
    return *this;
  }

  Random_Set(const DynList<T> & l) : Random_Set()
  {
    l.for_each([this] (const T & item) { append(item); });
  }

  template <class It> Random_Set(It b, It e) : Random_Set()
  {
    for (It it = b; it != e; ++it)
      append(*it);
  }
  
  Random_Set(std::initializer_list<T> l) : Random_Set()
  {
    for (const auto & item : l)
      append(item);
  }

  ~Random_Set()
  {
    if (r != nullptr)
      gsl_rng_free(r);
  }

  void put(const T & item) { array.append(item); }

  
  void put(T && item) { array.append(std::forward<T>(item)); }

  void append(const T & item)
  {
    put(item);
    auto pos = gsl_rng_uniform_int(r, array.size()); // al azar
    std::swap(array(pos), array(array.size() - 1));
  }

  void append(T && item)
  {
    put(move(item));
    auto pos = gsl_rng_uniform_int(r, array.size()); // al azar
    std::swap(array(pos), array(array.size() - 1));
  }

  T get()
  {
    if (array.size() == 0)
      throw std::underflow_error("Random set is empty");

    const size_t pos = gsl_rng_uniform_int(r, array.size()); // al azar
    T ret_val = array.access(pos);
    array.access(pos) = array.access(array.size() - 1);
    array.cut(array.size() - 1);
    return ret_val;
  }

  bool is_empty() const { return array.size() == 0; }

  struct Iterator : public DynArray<T>
  {
    using Base = DynArray<T>;
    using Base::Base;
  };

  template <class Operation>
    bool traverse(Operation & operation) 
    noexcept(noexcept(array.traverse(operation)))
  {
    return array.traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return const_cast<Random_Set*>(this)->traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation()) const
    noexcept(noexcept(operation))
  {
    return traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation = Operation())
    noexcept(noexcept(operation))
  {
    return traverse(operation);
  }
};


  template <typename T, template <typename> class C>
auto shuffle(const C<T> & c)
{
  Random_Set<T*> q;
  c.for_each([&q] (const T & item) { q.put(&const_cast<T&>(item)); });
  
  C<T> ret;
  while (not q.is_empty())
    ret.append(*q.get());

  return ret;
}


  template <typename T, template <typename> class C>
C<T*> shuffle_ptr(const C<T> & c)
{
  Random_Set<T*> q;
  c.for_each([&q] (const T & item) { q.put(&const_cast<T&>(item)); });
  
  C<T*> ret;
  while (not q.is_empty())
    ret.append(q.get());

  return ret;
}

} // end namespace Aleph

# endif // TPL_RANDOM_QUEUE_H

