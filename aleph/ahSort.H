
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

# ifndef AHSORT_H
# define AHSORT_H

# include <ahFunctional.H>
# include <tpl_sort_utils.H>


# define List_Sort(List)						\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> sort(const List<T> & c, Cmp & cmp)				\
  {									\
    List<T> ret_val = c;						\
    mergesort<List, T, Cmp>(ret_val, cmp);				\
    return ret_val;							\
  }									\
									\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> sort(const List<T> & c, Cmp && cmp = Cmp())			\
  {									\
    return sort<T, Cmp>(c, cmp);					\
  }									\
									\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> sort(List<T> && c, Cmp & cmp)				\
  {									\
    mergesort<List, T, Cmp>(c, cmp);					\
    return move(c);							\
  }									\
									\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> sort(List<T> && c, Cmp && cmp = Cmp())			\
  {									\
    return sort<T, Cmp>(move(c), cmp);					\
  }									\
									\
									\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> & in_place_sort(List<T> & c, Cmp & cmp) noexcept		\
  {									\
    mergeinsertsort(c, cmp);						\
    return c;								\
  }									\
									\
  template <typename T, class Cmp = Aleph::less<T>> inline		\
    List<T> & in_place_sort(List<T> & c, Cmp && cmp = Cmp()) noexcept	\
  {									\
    return in_place_sort<T, Cmp>(c, cmp);				\
  }

namespace Aleph
{

  List_Sort(DynList);

  List_Sort(DynDlist);

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> sort(const DynArray<T> & a, Cmp & cmp) 
  {
    DynArray<T> ret_val = a;
    quicksort_op(ret_val, cmp);
    return ret_val;
  }

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> sort(const DynArray<T> & a, Cmp && cmp = Cmp()) 
  {
    return sort<T, Cmp>(a, cmp);
  }

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> sort(DynArray<T> && a, Cmp & cmp) 
  {
    quicksort_op(a, cmp);
    return move(a);
  }

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> sort(DynArray<T> && a, Cmp && cmp = Cmp()) 
  {
    return sort<T, Cmp>(move(a), cmp);
  }

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> & in_place_sort(DynArray<T> & c, Cmp & cmp) noexcept
  {
    quicksort_op(c, cmp);
    return c;
  }

  template <typename T, class Cmp = Aleph::less<T>> inline
    DynArray<T> & in_place_sort(DynArray<T> & c, Cmp && cmp = Cmp()) 
    noexcept
  {
    return in_place_sort<T, Cmp>(c, cmp);
  }


  template <typename T>
class Compute_Ranks
{
  using P = std::pair<T, size_t>;

  DynArray<size_t> indexes;

public:

  DynArray<size_t> compute_ranks(const DynArray<T> & c)
  {
    const size_t n = c.size();
    indexes.reserve(n);
    for (size_t i = 0; i < n; ++i)
      indexes(i) = i;
    quicksort_op(indexes, [&c] (auto i1, auto i2) { return c(i1) < c(i2); });
    DynArray<size_t> ret; ret.reserve(n);
    for (size_t i = 0; i < n; ++i)
      ret(indexes(i)) = i;
    return ret;
  }

  template <class List>
  DynArray<size_t> compute_ranks(const List & c)
  {
    DynArray<T> items;
    size_t n = 0;
    c.for_each([this, &items, &n] (auto k)
	       {
		 items.append(k);
		 indexes.append(n++);
	       });
    quicksort_op(indexes, [&items] (auto i1, auto i2)
		 {
		   return items(i1) < items(i2);
		 });
    DynArray<size_t> ret; ret.reserve(n);
    for (size_t i = 0; i < n; ++i)
      ret(indexes(i)) = i;
    return ret;
  }

  DynArray<P> compute_pair_ranks(const DynArray<T> & c)
  {
    const size_t n = c.size();
    indexes.reserve(n);
    for (size_t i = 0; i < n; ++i)
      indexes(i) = i;
    quicksort_op(indexes, [&c] (auto i1, auto i2) { return c(i1) < c(i2); });
    DynArray<P> ret; ret.reserve(n);
    for (size_t i = 0; i < n; ++i)
      {
	auto idx = indexes(i);
	ret(idx) = make_pair(c(idx), i);
      }
    return ret;
  }

  template <template <typename> class List>
  DynArray<P> compute_pair_ranks(const List<T> & c)
  {
    DynArray<T> items; // array copy of c for direct access
    size_t n = 0;
    c.for_each([this, &items, &n] (auto k) // copy elements of c to item
	       {
		 items.append(k);
		 indexes.append(n++);
	       });
    quicksort_op(indexes, [&items] (auto i1, auto i2) 
		 {
		   return items(i1) < items(i2);
		 });
    DynArray<P> ret; ret.reserve(n);
    for (size_t i = 0; i < n; ++i)
      {
	auto idx = indexes(i);
	ret(idx) = make_pair(items(idx), i);
      }
    return ret;
  }
};

  template <typename T>
DynArray<size_t> ranks(const DynArray<T> & array)
{
  return Compute_Ranks<T>().compute_ranks(array);
}

template <typename T>
DynArray<size_t> ranks(const DynList<T> & l)
{
  return Compute_Ranks<T>().compute_ranks(l);
}

template <typename T>
DynArray<size_t> ranks(const DynDlist<T> & l)
{
  return Compute_Ranks<T>().compute_ranks(l);
}

template <typename T>
auto pair_ranks(const DynArray<T> & array)
{
  return Compute_Ranks<T>().compute_pair_ranks(array);
}

template <typename T, template <typename> class List>
auto pair_ranks(const List<T> & l)
{
  Compute_Ranks<T> func;
  return func.template compute_pair_ranks<List>(l);
}

} // end namespace Aleph

# endif // AHSORT_H
