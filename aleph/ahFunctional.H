
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

# ifndef AH_FUNCTIONAL_H
# define AH_FUNCTIONAL_H

# include <stdexcept>
# include <tuple>
# include <functional>
# include <algorithm>

/* Definitions of Aleph functional routines */

namespace Aleph
{
  template <typename T> struct Found_Item 
  {
    virtual T & get_item() = 0;
  };

  template <typename T> struct None : public Found_Item<T> 
  {
    virtual T & get_item()
    {
      throw std::invalid_argument("Access from None type");
    }
  };

  template <typename T> struct Some : public Found_Item<T>
  {
    T & item;

    Some(T & i) : item(i) {}

    virtual T & get_item() { return item; }
  };

template <typename tgtT, typename srcT>
  struct Dft_Map_Op
  {
    const tgtT & operator () (const srcT & item) noexcept { return item; }
  };

template <typename TR, typename TD>
  struct Dft_Fold_Op
  {
    const TR & operator () (const TR & /*acc */, 
			    const TD & /* val */) const noexcept 
    { return TR(); }
  };


template <typename T> class DynList;

  template <class Container, class Operation> inline
Container & for_each(Container & container, Operation & operation)
    noexcept(noexcept(operation))
{
  container.traverse([&operation] (const auto & item)
     { 
       operation(item);
       return true;
     });
  return container;
}

  template <class Container, class Operation> inline
const Container & for_each(const Container & container, Operation & operation)
    noexcept(noexcept(operation))
{
  container.traverse ([&operation] (const auto & item)
     { 
       operation(item);
       return true;
     });
  return container;
}

  template <class Container, class Operation> inline
Container& for_each(Container & container, Operation && operation = Operation())
    noexcept(noexcept(operation))
{
  return for_each<Container, Operation>(container, operation);
}

  template <class Container, class Operation> inline const Container &
for_each(const Container & container, Operation && operation = Operation())
    noexcept(noexcept(operation))
{
  return for_each<Container, Operation>(container, operation);
}


template <class Container, class Operation> inline
bool all(Container & container, Operation & operation)
  noexcept(noexcept(operation))
{
  return container.template traverse(operation);
}

template <class Container, class Operation> inline
bool all(const Container & container, Operation & operation)
  noexcept(noexcept(operation))
{
  return container.template traverse<Operation>(operation);
}

template <class Container, class Operation> inline
bool all(Container & container, Operation && operation = Operation())
  noexcept(noexcept(operation))
{
  return all<Container, Operation>(container, operation);
}

template <class Container, class Operation> inline
bool all(const Container & container, Operation && operation = Operation())
  noexcept(noexcept(operation))
{
  return all<Container, Operation>(container, operation);
}


template <class Container, class Operation> inline
bool exists(Container & container, Operation & operation)
  noexcept(noexcept(operation))
{
  return not container.traverse([&operation] (const auto & item)
				{
				  return not operation(item);
				});
}

template <class Container, class Operation> inline
bool exists(const Container & container, Operation & operation)
  noexcept(noexcept(operation))
{
  return not container.traverse([&operation] (const auto & item)
				{
				  return not operation(item);
				});
}

template <class Container, class Operation> inline
bool exists(Container & container, Operation && operation = Operation())
  noexcept(noexcept(operation))
{
  return exists<Container, Operation>(container, operation);
}

template <class Container, class Operation> inline
bool exists(const Container & container, Operation && operation = Operation())
  noexcept(noexcept(operation))
{
  return exists<Container, Operation>(container, operation);
}

template <typename T>
  struct Dft_Filter_Op
  {
    bool operator () (const T &) const noexcept { return true; }
  };


template <class Container1,
	  template <typename> class Container2 = Aleph::DynList,
	  class Operation = Dft_Filter_Op<typename Container1::Item_Type>>
inline Container2<typename Container1::Item_Type> 
filter(Container1 & container, Operation & operation)
{
  Container2<typename Container1::Item_Type> ret_val;
  container.
  for_each([&ret_val, &operation] (const auto & item)
	    {
	      if (operation(item))
		ret_val.append(item);
	   });
  return ret_val;
}

template <class Container1,
	  template <typename> class Container2 = Aleph::DynList,
	  class Operation = Dft_Filter_Op<typename Container1::Item_Type>>
inline Container2<typename Container1::Item_Type> 
filter(const Container1 & container, Operation & operation)
{
  Container2<typename Container1::Item_Type> ret_val;
  container.for_each
  ([&ret_val, &operation] (const auto & item)
	    {
	      if (operation(item))
		ret_val.append(item);
	   });
  return ret_val;
}

template <class Container1,
	  template <typename> class Container2 = Aleph::DynList,
	  class Operation = Dft_Filter_Op<typename Container1::Item_Type>>
inline Container2<typename Container1::Item_Type> 
filter(Container1 & container, Operation && operation)
{
  return filter<Container1, Container2, Operation>(container, operation);
}


template <class Container1,
	  template <typename> class Container2 = Aleph::DynList,
	  class Operation = Dft_Filter_Op<typename Container1::Item_Type>>
Container2<typename Container1::Item_Type> 
inline filter(const Container1 & container, Operation && operation)
{
  return filter<Container1, Container2, Operation>(container, operation);
}

template <typename                  Ta,
	  template <typename> class Ca, 
	  typename                  Tb = Ta,
	  template <typename> class Cb = Ca> inline
Cb<Tb> map_items(const Ca<Ta> & ca, std::function<Tb(const Ta &)> operation)
{
  Cb<Tb> ret_val;
  for (typename Ca<Ta>::Iterator it(ca); it.has_curr(); it.next())
    ret_val.append(operation(it.get_curr()));

  return ret_val;
}

template <template <typename> class Container, 
	  typename                  T, 
	  class                     Operation> inline
T foldl(const Container<T> & container, const T & init, Operation & operation)
  noexcept(noexcept(operation) and std::is_nothrow_copy_assignable<T>::value)
{
  T ret_val = init;
  for (typename Container<T>::Iterator it(container); it.has_curr(); it.next())
    ret_val = operation(ret_val, it.get_curr());

  return ret_val;
}


template <template <typename> class Container, 
	  typename                  T, 
	  class                     Operation> inline
T foldl(const Container<T> & container, 
	const T &            init      = T(),
	Operation &&         operation = Operation())
  noexcept(noexcept(operation) and std::is_nothrow_copy_assignable<T>::value)
{
  return foldl(container, operation, init) ;
}


  template <class Container1, class Container2> inline
DynList<std::pair<typename Container1::Item_Type, 
		  typename Container2::Item_Type>> 
zip(const Container1 & a, const Container2 & b)
{
  typedef typename Container1::Item_Type T1;
  typedef typename Container2::Item_Type T2;
  DynList<std::pair<T1, T2>> ret_val;
  
  typename Container1::Iterator it1(a);
  typename Container2::Iterator it2(b);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    ret_val.append(std::pair<T1, T2>(it1.get_curr(), it2.get_curr()));

  return ret_val;
}

template <class Container1, class Container2> inline
DynList<std::tuple<typename Container1::Item_Type, 
		   typename Container2::Item_Type>> 
tzip(const Container1 & a, const Container2 & b)
{
  typedef typename Container1::Item_Type T1;
  typedef typename Container2::Item_Type T2;
  DynList<std::tuple<T1, T2>> ret_val;
  
  typename Container1::Iterator it1(a);
  typename Container2::Iterator it2(b);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    ret_val.append(std::make_tuple(it1.get_curr(), it2.get_curr()));

  return ret_val;
}

template <class Container1, class Container2> inline
DynList<std::pair<typename Container1::Item_Type, 
		  typename Container2::Item_Type>> 
zipEq(const Container1 & a, const Container2 & b)
{
  typedef typename Container1::Item_Type T1;
  typedef typename Container2::Item_Type T2;
  DynList<std::pair<T1, T2>> ret_val;
  
  typename Container1::Iterator it1(a);
  typename Container2::Iterator it2(b);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    ret_val.append(std::pair<T1, T2>(it1.get_curr(), it2.get_curr()));

  if (it1.has_curr() or it2.has_curr())
    throw std::length_error("Container sizes mismatch");

  return ret_val;
}

template <class Container1, class Container2> inline
DynList<std::tuple<typename Container1::Item_Type, 
		   typename Container2::Item_Type>> 
tzipEq(const Container1 & a, const Container2 & b)
{
  typedef typename Container1::Item_Type T1;
  typedef typename Container2::Item_Type T2;
  DynList<std::tuple<T1, T2>> ret_val;
  
  typename Container1::Iterator it1(a);
  typename Container2::Iterator it2(b);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    ret_val.append(std::make_tuple(it1.get_curr(), it2.get_curr()));

  if (it1.has_curr() or it2.has_curr())
    throw std::length_error("Container sizes mismatch");

  return ret_val;
}

template <class Container>
DynList<std::pair<typename Container::Item_Type, size_t>> inline
enumerate(const Container & c)
{
  using Item = typename Container::Item_Type;
  using Pair = std::pair<Item, size_t>;
  DynList<Pair> ret;
  size_t i = 0;
  c.for_each([&i, &ret] (const Item & item) { ret.append(Pair(item, i++)); });
  return ret;
}


  template <typename T, template <typename> class Container = DynList,
	    class Eq = std::equal_to<T>> inline 
bool eq(const Container<T> & c1, const Container<T> & c2, Eq & e)
{
  typename Container<T>::Iterator it1(c1), it2(c2);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    if (not (e(it1.get_curr(), it2.get_curr())))
      return false;

  return not (it1.has_curr() or it2.has_curr());
}

  template <typename T, template <typename> class Container = DynList,
	    class Eq = std::equal_to<T>> inline 
bool eq(const Container<T> & c1, const Container<T> & c2, Eq && e = Eq())
{
  return eq<T, Container, Eq>(c1, c2, e);
}

  template <typename T, template <typename> class Container = DynList,
	    class Eq = std::equal_to<T>> inline 
    std::tuple<bool, size_t, T, T>
are_eq(const Container<T> & c1, const Container<T> & c2, Eq && e = Eq())
{
  typename Container<T>::Iterator it1(c1), it2(c2);
  size_t n = 0;
  for (; it1.has_curr() and it2.has_curr(); it1.next(), it2.next(), n++)
    {
      auto i1 = it1.get_curr();
      auto i2 = it2.get_curr();
      if (not (e(i1, i2)))
	return std::make_tuple(false, n, i1, i2);
    }

  return std::make_tuple(c1.is_empty() == c2.is_empty(), n, T(), T());
}


  template <typename T, template <typename> class Container = Aleph::DynList, 
	    class Cmp = std::less<T>> inline
bool lesser(const Container<T> & c1, const Container<T> & c2, Cmp & cmp)
{
  typename Container<T>::Iterator it1(c1), it2(c2);
  for (/* nothing */; it1.has_curr() and it2.has_curr(); it1.next(), it2.next())
    if (cmp(it1.get_curr(), it2.get_curr()))
      return true;
    else if (cmp(it2.get_curr(), it1.get_curr()))
      return false;

  if (not it1.has_curr() and not it2.has_curr())
    return false;

  return it2.has_curr();
}

template <typename T, template <typename> class Container = DynList, 
	  class Cmp = std::less<T>> inline
bool lesser(const Container<T> & c1, const Container<T> & c2, 
	    Cmp && cmp = Cmp())
{
  return lesser<T, Container, Cmp>(c1, c2, cmp);
}

template <typename T, template <typename> class Container = DynList,
	  class Eq = std::equal_to<T>> inline
bool diff(const Container<T> & c1, const Container<T> & c2, Eq & e)
{
  return not eq<T, Container, Eq>(c1, c2, e);
}

template <typename T, template <typename> class Container = DynList,
	  class Eq = std::equal_to<T>> inline
bool diff(const Container<T> & c1, const Container<T> & c2, Eq && e = Eq())
{
  return diff<T, Container, Eq>(c1, c2, e);
}

template <template <typename> class Container, typename T1, typename T2>
inline std::pair<Container<T1>, Container<T2>>
unzip(const Container<std::pair<T1, T2>> & l)
{
  Container<T1> l1;
  Container<T2> l2;
  for (typename Container<std::pair<T1, T2>>::Iterator it(l); it.has_curr();
       it.next())
    {
      std::pair<T1, T2> curr = it.get_curr();
      l1.append(curr.first);
      l2.append(curr.second);
    }

  return std::pair<Container<T1>, Container<T2>>(std::move(l1), std::move(l2));
}

template <template <typename> class Container, typename T1, typename T2>
inline std::tuple<Container<T1>, Container<T2>> 
tunzip(const Container<std::tuple<T1, T2>> & l)
{
  Container<T1> l1;
  Container<T2> l2;
  for (typename Container<std::pair<T1, T2>>::Iterator it(l); it.has_curr();
       it.next())
    {
      auto curr = it.get_curr();
      l1.append(curr.first);
      l2.append(curr.second);
    }

  return std::make_tuple(std::move(l1), std::move(l2));
}


template <class SrcContainer, 
	  template <typename> class TgtContainer = Aleph::DynList> inline
std::pair<TgtContainer<typename SrcContainer::Item_Type>,
	  TgtContainer<typename SrcContainer::Item_Type>> partition
  (const SrcContainer & c, 
   std::function<bool(const typename SrcContainer::Item_Type &)> operation)
{
  typedef typename SrcContainer::Item_Type Type;
  typedef std::pair<TgtContainer<Type>, TgtContainer<Type>> Pair;
      
  Pair ret_val;
  for_each(c, [&ret_val, &operation] (const Type & item)
	   {
	     if (operation(item))
	       ret_val.first.append(item);
	     else
	       ret_val.second.append(item);
	   });
  return ret_val;
}


  template <typename T = int, template <typename> class Container = DynList>
inline Container<T> range(const T start, const T end, const T step = 1)
{
  Container<T> ret_val;
  for (T i = start; i <= end; i += step)
    ret_val.append(i);
  return ret_val;
}

template <typename T = int, template <typename> class Container = DynList>
inline Container<T> range(const T n)
{
  Container<T> ret_val;
  for (T i = 0; i < n; ++i)
    ret_val.append(i);
  return ret_val;
}


  template <template <typename> class Container = DynList, typename T = int>
inline Container<T> rep(size_t n, const T & item)
{
  Container<T> ret_val;
  for (T i = 0; i < n; ++i)
    ret_val.append(item);
  return ret_val;
}

  template <template <typename> class Container = DynList, typename T = int>
inline Container<T> rep(size_t n, T && item = T())
{
  return rep<Container, T>(n, item);
}


template <class Container> inline
DynList<std::pair<size_t, typename Container::Key_Type>>
indexes(const Container & c)
{
  using T = typename Container::Key_Type;
  using Pair = std::pair<size_t, T>;
  size_t i = 0;
  
  return c.Container:: template map<Pair>([&i] (const T & d) { return Pair(i++, d); });
}


template <class Container> inline
DynList<std::tuple<size_t, typename Container::Key_Type>> 
tindexes(const Container & c)
{
  using T = typename Container::Key_Type;
  size_t i = 0;
  return c.Container::template map<std::tuple<size_t, T>>([&i] (const T & d) 
    {
      return std::make_tuple(i++, d); 
    });
}


template <typename T, template <typename> class Container>
inline Container<T> reverse(const Container<T> & l)
{
  Container<T> ret_val;
  l.for_each([&ret_val] (const T & item)
	     {
	       ret_val.insert(item);
	     });
  return ret_val;
}

template <typename T, template <typename> class Container>
DynList<DynList<T>> gen_seq_list_tuples(const Container<T> & c, size_t n)
{
  typename Container<T>::Iterator it(c);
  DynList<T> l;
  for (auto i = 0; i < n; ++i, it.next())
    l.append(it.get_curr());
	 
  DynList<DynList<T>> ret;
  ret.append(l);
  for (; it.has_curr(); it.next())
    {
      l.remove_first();
      l.append(it.get_curr());
      ret.append(l);
    }

  return ret;
}

  template <class S, class Itor = typename S::Iterator>
class Pair_Iterator
{
  Itor it1;
  Itor it2;

public:
  
  Pair_Iterator(const S & s1, const S & s2)
  noexcept(std::is_nothrow_constructible<typename S::Iterator, S>::value)
    : it1(s1), it2(s2)
  {
    // empty
  }

  Pair_Iterator(S && s1, S && s2)
  noexcept(noexcept(Pair_Iterator(move(s1), move(s2))))
  : Pair_Iterator(move(s1), move(s2))
  {
    // empty
  }

  Pair_Iterator(const S & s1, S && s2)
  noexcept(noexcept(Pair_Iterator(s1, move(s2))))
  : Pair_Iterator(s1, move(s2))
  {
    // empty
  }

  Pair_Iterator(S && s1, const S & s2)
  noexcept(noexcept(Pair_Iterator(move(s1), s2)))
  : Pair_Iterator(move(s1), s2)
  {
    // empty
  }

  bool has_curr() const { return it1.has_curr() and it2.has_curr(); }

  auto get_curr() const 
  {
    return std::make_pair(it1.get_curr(), it2.get_curr());
  }

  void next()
  {
    it1.next();
    it2.next();
  }
};



// These function are defined in tpl_dynSetHash.H

// union 
template <typename T, template <typename> class Container> inline
DynList<T> join(const Container<T> & c1, const Container<T> & c2);

template <typename T, template <typename> class Container> inline
DynList<T> intercept(const Container<T> & c1, const Container<T> & c2);

template <typename T, template <typename> class Container> inline
DynList<T> unique(const Container<T> & c);

template <typename T, template <typename> class Container> inline
DynList<T> repeated(const Container<T> & c);

template <typename T, template <typename> class Container> inline
DynList<std::pair<T, size_t>> repeated_with_index(const Container<T> & c);
			      
} // end namespace Aleph

# endif // AH_FUNCTIONAL_H
