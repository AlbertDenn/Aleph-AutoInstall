
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


# ifndef AH_DRY_H
# define AH_DRY_H


template <class Container>
struct GenericTraverse
{
  template <class Operation>
  bool traverse(Operation & operation) noexcept(noexcept(operation))
  {
    for (typename Container::Iterator it(*static_cast<Container*>(this)); 
	 it.has_curr(); it.next())
      if (not operation(it.get_curr()))
	return false;
    return true;
  }

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return const_cast<GenericTraverse*>(this)->traverse<Operation>(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation) const noexcept(noexcept(operation))
  {
    return traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation && operation) noexcept(noexcept(operation))
  {
    return traverse(operation);
  }
};

template <class Container, typename Type>
class LocateFunctions
{
  Container * me() noexcept { return static_cast<Container*>(this); }

  auto const_me() const noexcept 
  {
    return static_cast<const Container*>(this);
  }

  LocateFunctions<Container, Type> * base() const
  {
    return const_cast<LocateFunctions*>(this);
  }

public:

  auto get_it() const 
  { 
    auto ret = typename Container::Iterator(*const_me());
    return ret;
  }

  auto get_itor() const { return get_it(); }

  Type & nth(const size_t n) 
  {
    Type * ptr = nullptr;
    size_t i = 0;
    me()->traverse([&ptr, &i, &n] (Type & item)
		   {
		     if (i++ < n)
		       return true;
		     ptr = &item;
		     return false;
		   });

    if (i != n + 1)
      throw std::out_of_range("index out of range");

    return *ptr;
  }

  const Type & nth(const size_t n) const
  {
    return base()->nth(n);
  }

  template <class Operation>
  Type * find_ptr(Operation & operation) noexcept(noexcept(operation))
  {
    Type * ptr = nullptr;
    me()->traverse([&ptr,&operation] (Type & item)
		   {
		     if (operation(item))
		       {
			 ptr = &item;
			 return false;
		       }
		     return true;
		   });
    return ptr;
  }

  template <class Operation>
  const Type * find_ptr(Operation & operation) const 
    noexcept(noexcept(operation))
  {
    return base()->find_ptr(operation);
  }

  template <class Operation>
  const Type * find_ptr(Operation && operation) const 
    noexcept(noexcept(operation))
  {
    return find_ptr<Operation>(operation);
  }

  template <class Operation>
  Type * find_ptr(Operation && operation) noexcept(noexcept(operation))
  {
    return find_ptr(operation);
  }

  template <class Operation>
  size_t find_index(Operation & operation) const noexcept(noexcept(operation))
  {
    size_t i = 0;
    me()->traverse([&i,&operation] (Type & item)
		   {
		     if (operation(item))
		       return false;
		     ++i;
		     return true;
		   });
    return i;
  }

  template <class Operation>
  size_t find_index(Operation && operation) const noexcept(noexcept(operation))
  {
    return find_index<Operation>(operation);
  }

  template <class Operation>
  std::tuple<bool, Type> find_item(Operation & operation)
    noexcept(noexcept(operation))
  {
    auto ptr = find_ptr(operation);
    return ptr ? std::make_tuple(true, *ptr) : std::make_tuple(false, Type());
  }

  template <class Operation>
  std::tuple<bool, Type> find_item(Operation & operation) const
    noexcept(noexcept(operation))
  {
    auto ptr = find_ptr(operation);
    return ptr ? std::make_tuple(true, *ptr) : std::make_tuple(false, Type());
  }

  template <class Operation>
  std::tuple<bool, Type> find_item(Operation && operation)
    noexcept(noexcept(operation))
  {
    return find_item(operation);
  }

  template <class Operation>
  std::tuple<bool, Type> find_item(Operation && operation) const
    noexcept(noexcept(operation))
  {
    return find_item(operation);
  }
};

template <class Container, typename T>
struct SpecialCtors
{
  SpecialCtors() {}

  SpecialCtors(const SpecialCtors&) {}

  SpecialCtors(SpecialCtors&&) {}

  SpecialCtors & operator = (const SpecialCtors&) { return *this; }

  SpecialCtors & operator = (SpecialCtors&&) { return *this; }
  
  SpecialCtors(const DynList<T> & l)
  {
    l.for_each([this] (const T & item) 
	       {
		 static_cast<Container*>(this)->append(item); 
	       });
  }

  template <class It>
  SpecialCtors(It b, It e)
  {
    for (It it = b; it != e; ++it)
      static_cast<Container*>(this)->append(*it);
  }			

  SpecialCtors(std::initializer_list<T> l)
  {
    for (const auto & item : l)
      static_cast<Container*>(this)->append(item);
  }
};


template <class Container, typename T>
class FunctionalMethods
{
  Container * me() { return static_cast<Container*>(this); }

  FunctionalMethods<Container, T> * base() const noexcept
  {
    return const_cast<FunctionalMethods<Container, T>*>(this);
  }

  auto const_me() const noexcept 
  {
    return static_cast<const Container*>(this);
  }

public:

  template <typename ...Args>
  void emplace(Args && ... args)
  {
    me()->append(T(args...));
  }

  template <typename ...Args>
  void emplace_end(Args && ... args)
  {
    me()->append(T(args...));
  }

  template <typename ...Args>
  void emplace_ins(Args && ... args)
  {
    me()->insert(T(args...));
  }

  template <class Operation>
  void for_each(Operation & operation) noexcept(noexcept(operation))
  {
    me()->traverse([&operation] (const auto & item)
		   {
		     operation(item);
		     return true;
		   });
  }

  template <class Operation>
  void for_each(Operation & operation) const noexcept(noexcept(operation))
  {
    base()->for_each(operation);
  }

  template <class Operation>
  void for_each(Operation && operation) const noexcept(noexcept(operation))
  {
    for_each(operation);
  }

  template <class Operation>
  void for_each(Operation && operation) noexcept(noexcept(operation))
  {
    for_each(operation);
  }

  template <class Operation>
  void mutable_for_each(Operation & operation) noexcept(noexcept(operation))
  {
    me()->traverse([&operation] (auto & item)
		   {
		     operation(item);
		     return true;
		   });
  }
  
  template <class Operation>
  void mutable_for_each(Operation && operation) noexcept(noexcept(operation))
  {
    mutable_for_each(operation);
  }						

  template <class Operation>
  bool all(Operation & operation) const noexcept(noexcept(operation))
  {
    return const_me()->traverse(operation);
  }

  template <class Operation>
  bool all(Operation && operation) const noexcept(noexcept(operation))
  {
    return all(operation);
  }

  template <class Operation>
  bool exists(Operation & op) const noexcept(noexcept(op))
  {
    return not const_me()->
      traverse([&op] (const auto & i) { return not op(i); });
  }

  template <class Operation>
  bool exists(Operation && op) const noexcept(noexcept(op))
  {
    return exists(op);
  }

  template <typename __T    = T, class Operation = Dft_Map_Op<T, __T>>
  DynList<__T> map(Operation & op) const
  {
    DynList<__T> ret_val;
    const_me()->for_each([&ret_val, &op] (const auto & item)
	      {
		ret_val.append(op(item));
	      });
    return ret_val;
  }

  template <typename __T    = T, class Operation = Dft_Map_Op<__T, __T>>
  DynList<__T> map(Operation && op) const { return map<__T, Operation>(op); }

  template <typename __T = T, class Op = Dft_Fold_Op<__T, T>>
  __T foldl(const __T & init, Op & op) const noexcept(noexcept(op))
  {
    auto ret_val = init;
    const_me()->for_each([&ret_val, &op] (const T & item)
			 {
			   ret_val = op(ret_val, item);
			 });
    return ret_val;
  }

  template <typename __T = T, class Op = Dft_Fold_Op<__T, T>>
    __T foldl(const __T & init, Op && op = Op()) const noexcept(noexcept(op))
  {
    return foldl<__T, Op>(init, op);
  }

  template <class Operation>
  T fold(const T & init, Operation & operation) const
    noexcept(noexcept(operation))
  {
    auto ret_val = init;
    const_me()->for_each([&ret_val, &operation] (const T & item) 
			 {
			   ret_val = operation(ret_val, item);
			 });
    return ret_val;
  }

  template <class Operation>
  T fold(const T & init, Operation && operation) const
    noexcept(noexcept(operation))
  {
    return fold(init, operation);
  }

  template <class Operation>
  DynList<T> filter(Operation & operation) const
  {
    DynList<T> ret_val;
    const_me()->for_each([&ret_val, &operation] (const T & item)
			 {
			   if (operation(item))
			     ret_val.append(item);
			 });
    return ret_val;
  }
  
  template <class Operation>
  DynList<T> filter(Operation && operation) const
  {
    return filter(operation);
  }

  template <class Operation>
  DynList<std::tuple<T, size_t>> pfilter(Operation & operation) const
  {
    DynList<std::tuple<T, size_t>> ret_val;
    size_t i = 0;
    const_me()->for_each([&ret_val, &operation, &i] (const T & item)
			 {
			   if (operation(item))
			     ret_val.append(std::make_tuple(item, i));
			   ++i;
			 });
    return ret_val;
  }

  template <class Operation>
  auto pfilter(Operation && operation) const
  {
    return pfilter(operation);
  }

  template <class Operation>
  std::pair<DynList<T>, DynList<T>> partition(Operation & op) const
  {
    std::pair<DynList<T>, DynList<T>> ret_val;
    const_me()->for_each([&ret_val, &op] (const T & item)
			 {
			   if (op(item))
			     ret_val.first.append(item);
			   else
			     ret_val.second.append(item);
			 });
    return ret_val;
  }

  template <class Operation>
  auto partition(Operation && op) const
  {
    return partition(op);
  }

  template <class Operation>
  std::tuple<DynList<T>, DynList<T>> tpartition(Operation & op) const
  {
    DynList<T> r1, r2;
    const_me()->for_each([&r1, &r2, &op] (const T & item)
			 {
			   if (op(item))
			     r1.append(item);
			   else
			     r2.append(item);
			 });
    return std::make_tuple(r1, r2);
  }

  template <class Operation>
  auto tpartition(Operation && op) const
  {
    return partition(op);
  }

  size_t length() const noexcept
  {
    size_t count = 0;
    const_me()->for_each([&count] (const T &) { ++count; });
    return count;
  }

  DynList<T> rev() const
  {
    DynList<T> ret;
    const_me()->for_each([&ret] (const T & i) { ret.insert(i); });
    return ret;
  }

  DynList<T> take(const size_t n) const
  {
    size_t i = 0;
    DynList<T> ret;
    const_me()->traverse([&i, &ret, n] (const T & item)
			 {
			   if (i++ >= n)
			     return false;
			   ret.append(item);
			   return true;
			 });
    return ret;
  }

  DynList<T> drop(const size_t n) const
  {
    size_t i = 0;
    DynList<T> ret;
    const_me()->traverse([&i, &ret, n] (const T & item)
			 {
			   if (i++ >= n)
			     ret.append(item);
			   return true;
			 });
    return ret;
  }
};

template <class Container, typename T>
struct GenericItems 
{
  DynList<T> items() const
  {
    return static_cast<const Container*>(this)->
      Container::template map<T> ([] (const auto & key) { return key; });
  }

  auto keys() const { return items(); }
};


template <class Container, typename T>
using GenericKeys = GenericItems<Container, T>;


template <class Container>
class EqualToMethod
{
  const Container * const_me() const 
  {
    return static_cast<const Container*>(this);
  }

public:

  bool equal_to(const Container & r) const noexcept
  {
    if (this == &r)
      return true;

    if (const_me()->size() != r.size())
      return false;

    return const_me()->all([&r] (const auto  & k) 
			   { return r.search(k) != nullptr; }); 
  }

  bool operator == (const Container & r) const	noexcept
  {
    return equal_to(r);
  }

  bool operator != (const Container & r) const	noexcept
  {
    return not equal_to(r);
  }
};

template <class Container, typename Key, typename Data>
class MapSequencesMethods
{
  const Container * const_me() const 
  {
    return static_cast<const Container*>(this);
  }

public:

  template <template <typename> class C = DynList>
  C<Key> keys()	const
  {
    C<Key> ret_val; 
    const_me()->for_each([&ret_val] (const auto & p)
			 {
			   ret_val.append(p.first);
			 });
    return ret_val;
  }

  template <template <typename> class C = DynList>
  C<Data> values() const
  {
    C<Data> ret_val;
    const_me()->for_each([&ret_val] (const auto & p)
			 {
			   ret_val.append(p.second);
			 }); 
    return ret_val;
  }

  template <template <typename> class C = DynList>
  C<Data*> values_ptr() const
  {
    C<Data*> ret_val;
    const_me()->for_each([&ret_val] (auto & p) { ret_val.append(&p.second); });
    return ret_val;
  }

  template <template <typename> class C = DynList>
  C<std::pair<Key, Data>> items() const
  {
    C<std::pair<Key, Data>> ret;
    const_me()->for_each([&ret] (auto & p) { ret.append(p); });
    return ret;
  }

  template <template <typename> class C = DynList>
  C<std::pair<Key, Data*>> items_ptr() const
  {
    C<Data> ret_val;
    const_me()->for_each([&ret_val] (auto & p)
      {
	ret_val.append(std::make_pair(p.first, p.second));
      });
    return ret_val;
  }

  Data & operator () (const Key & key)
  {
    return this->find(key);
  }

  const Data & operator () (const Key & key) const
  {
    return this->find(key);
  }
};



# endif // AH_DRY_H
