
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

# ifndef TPL_SORT_UTILS_H
# define TPL_SORT_UTILS_H

# include <ahUtils.H>
# include <ahFunctional.H>
# include <tpl_arrayStack.H>
# include <tpl_dynArray.H>
# include <tpl_dynDlist.H>
# include <htlist.H>

namespace Aleph 
{
  extern size_t Insertion_Threshold;

  extern size_t Quicksort_Threshold;

  extern const int Not_Found;

  template <template <typename> class Container, typename T, class Compare>
bool is_sorted(const Container<T> & cont, Compare & cmp) noexcept
{
  if (cont.is_empty())
    return true;

  T item = cont.get_first();
  typename Container<T>::Iterator it(cont); it.next();
  for (long i = 1; it.has_curr(); it.next(), ++i)
    {
      const T & curr = it.get_curr();
      if (cmp(curr, item))
	{
	  cout << "cont[" << i << "] = " << curr 
	       << " < cont[" << i - 1 << "] = " << item << endl;
	  return false;
	}

      item = curr;
    }
  return true;
}

  template <template <typename> class Container, typename T, 
	    class Compare = Aleph::less<T>>
bool is_sorted(const Container<T> & cont, Compare && cmp = Compare()) noexcept
{
  return is_sorted(cont, cmp);
}

    template <typename T, class Compare> inline
void selection_sort(T * a, const size_t n, Compare & cmp) noexcept
{
  for (long i = 0, min, j; i < n - 1; ++i)
    {
      for (min = i, j = i + 1; j < n; ++j)
	if (cmp(a[j], a[min]))
	  min = j;

      if (cmp(a[min], a[i]))
        std::swap(a[min], a[i]);
    }
}


    template <typename T, class Compare = Aleph::less<T>> inline
void selection_sort(T * a, const size_t n, Compare && cmp = Compare()) noexcept
{
  selection_sort<T, Compare>(a, n, cmp);
}


  template <class Link, class Compare> inline 
Link * search_extreme(const Link & list, Compare & cmp) noexcept
{
  typename Link::Iterator it(const_cast<Link&>(list));
  Link * extreme = it.get_current();

  for (it.next(); it.has_curr(); it.next())
    {
      Link * curr = it.get_curr();
      if (cmp(curr, extreme))
	extreme = curr;
    } 

  return extreme;
}

    template <class Compare> inline 
Dlink * search_extreme(const Dlink & list, Compare & cmp) noexcept
{
  return search_extreme<Dlink, Compare>(list, cmp);
}

    // Wrapper para llamar a search_extreme con un r-value
    template <class Compare> inline 
Dlink * search_extreme(const Dlink & list, Compare && cmp = Compare()) noexcept
{
  return search_extreme<Dlink, Compare>(list, cmp);
}


    template <class Compare> inline 
Slinknc * search_extreme(const Slinknc & list, Compare & cmp) noexcept
{
  return search_extreme<Slinknc, Compare>(list, cmp);
}

    // Wrapper para llamar a search_extreme con un r-value
    template <class Compare> inline 
Slinknc * search_extreme(const Slinknc & list, Compare && cmp = Compare())
      noexcept
{
  return search_extreme<Compare>(list, cmp);
}

    template <class Compare> inline 
void selection_sort(Dlink & list, Compare & cmp) noexcept
{
  Dlink aux;
  while (not list.is_empty())
    {
      Dlink * extreme = search_extreme <Dlink, Compare> (list, cmp); 
      extreme->del(); // saque extremo de list
      aux.append(extreme); // insértelo ordenado en aux;
    }

  list.swap(&aux);
}

    template <class Compare> inline 
void selection_sort(Dlink & list, Compare && cmp = Compare()) noexcept
{
  selection_sort<Compare>(list, cmp);
}


template <class Tlink, 
	  template <class> class Tnode, 
	  typename T, class Compare> 
class Compare_Tnode
{
  Compare & cmp;

public:

  Compare_Tnode(Compare && __cmp = Compare()) 
  noexcept(std::is_nothrow_move_assignable<Compare>::value)
    : cmp(__cmp) { /* empty */ }

  Compare_Tnode(Compare & __cmp) 
  noexcept(std::is_nothrow_copy_assignable<Compare>::value)
    : cmp(__cmp) { /* empty */ }

  bool operator () (Tlink * l1, Tlink * l2) const noexcept
  {
    Tnode<T> * n1 = static_cast<Tnode<T>*>(l1);
    Tnode<T> * n2 = static_cast<Tnode<T>*>(l2);

    assert(n1 == l1 and n2 == l2);

    return cmp(n1->get_data(), n2->get_data());
  }

  // Para generalizar en el sequential_search
  bool operator () (Tlink * l, const T & x) const noexcept
  {
    Tnode<T> * n = static_cast<Tnode<T>*>(l);

    assert(n == l);

    return cmp(n->get_data(), x);
  }
};

    template <typename T, class Compare> 
struct Compare_Dnode : public Compare_Tnode<Dlink, Dnode, T, Compare>
{
  Compare_Dnode(Compare && cmp = Compare()) 
  noexcept(std::is_nothrow_move_assignable<Compare>::value)
    : Compare_Tnode<Dlink, Dnode, T, Compare> (std::forward<Compare>(cmp)) 
  { /* empty */ }

  Compare_Dnode(Compare & cmp) 
  noexcept(std::is_nothrow_copy_assignable<Compare>::value)
    : Compare_Tnode<Dlink, Dnode, T, Compare> (cmp) 
  { /* empty */ }
};

template <typename T, class Compare>
using Compare_Snodenc = Compare_Tnode<Slinknc, Snodenc, T, Compare>;


    template <typename T, class Compare> 
inline void selection_sort(Dnode<T> & list, Compare & cmp) noexcept
{
  selection_sort <Compare_Dnode<T, Compare>>
    (list, Compare_Dnode<T, Compare>(cmp));
}

    template <typename T, class Compare = Aleph::less<T>> 
inline void selection_sort(Dnode<T> & list, Compare && cmp = Compare()) noexcept
{
  selection_sort<T, Compare>(list, cmp);
}

      template <typename T, class Equal> inline 
int sequential_search(T * a, const T & x, const int l, const int r, Equal & eq)
	noexcept
{
  for (int i = l; i <= r; i++)
    if (eq(a[i], x)) return i;

  return Not_Found;
}

      template <typename T, class Equal = Aleph::equal_to<T>> inline 
int sequential_search(T * a, const T & x, const int l, const int r,
		      Equal && eq = Equal()) noexcept
{
  return sequential_search<T, Equal>(a, x, l, r, eq);
}


    template <typename T, class Equal = Aleph::equal_to<T>> inline
int sequential_search(const DynArray<T> & a, const T & x, 
		      const int l, const int r, Equal & eq) noexcept
{
  for (int i = l; i <= r; i++)
    if (a.exist(i))
      if (eq(a.access(i), x))
	return i;

  return -1;
}

    template <typename T, class Equal = Aleph::equal_to<T>> inline
int sequential_search(const DynArray<T> & a, 
		      const T & x, const int l, const int r,
		      Equal && eq = Equal()) noexcept
{
  return sequential_search<T, Equal>(a, x, l, r, eq);
}


    template <class Link, typename T, class Equal>
Link * sequential_search(const Link & list, const T & x, Equal & eq) noexcept
{
  for (typename Link::Iterator it(const_cast<Link&>(list)); 
       it.has_curr(); it.next())
    {
      Link * curr = it.get_curr();
      if (eq(curr, x)) 
	return curr;
    }

  return nullptr;
}

    template <typename T, class Equal>
Dlink * sequential_search(const Dlink & list, const T & x, Equal & eq) noexcept
{
  return sequential_search <Dlink, T, Equal>(list, x, eq);
}

template <typename T, class Equal>
Dlink * sequential_search(const Dlink & list, const T & x, 
			  Equal && eq = Equal()) noexcept
{
  return sequential_search <T, Equal>(list, x, eq);
}

    template <typename T, class Equal>
Slinknc * sequential_search(const Slinknc & list, const T & x, Equal & eq)
      noexcept
{
  return sequential_search <Slinknc, T, Equal>(list, x, eq);
}

template <typename T, class Equal>
Slinknc * sequential_search(const Slinknc & list, const T & x, 
			    Equal && eq = Equal()) noexcept
{
  return sequential_search <T, Equal>(list, x, eq);
}

    template <typename T, class Equal = Aleph::equal_to<T>> inline
Dnode<T> * sequential_search(const Dnode<T> & list, const T & x, 
			     Equal & eq) noexcept
{
  Dlink * ret =
   sequential_search<T, Compare_Dnode <T, Equal>>
      ((Dlink &) list, x, Compare_Dnode<T, Equal>(eq));

  return ret == nullptr ? nullptr : static_cast <Dnode<T> *>(ret);
}

    template <typename T, class Equal = Aleph::equal_to<T>> inline
Dnode<T> * sequential_search(const Dnode<T> & list, const T & x, 
			     Equal && eq = Equal()) noexcept
{
  return sequential_search<T, Equal>(list, x, eq);
}

    template <typename T, class Equal> inline
T * sequential_search(const DynDlist<T> & list, const T & x, Equal & eq) 
      noexcept
{
  Dnode<T> * ret = sequential_search<T, Equal>((Dnode<T>&) list, x, eq);
  return ret != nullptr ? &ret->get_data() : nullptr;
}

    template <typename T, class Equal = Aleph::equal_to<T>> inline
T * sequential_search(const DynDlist<T> & list, const T & x, 
		      Equal && eq = Equal()) noexcept
{
  return sequential_search<T, Equal>(list, x, eq);
}

    template <typename T, class Equal> inline
T * sequential_search(const DynList<T> & list, const T & x, Equal & eq)
      noexcept
{
  Slinknc * ret =
    sequential_search<T, Compare_Snodenc<T, Equal>>
     (*list.get_head(), x, Compare_Snodenc<T, Equal>(eq));

  return 
    ret != nullptr ? &((static_cast<Snodenc<T>*>(ret))->get_data()) : nullptr;
}

    template <typename T, class Equal = Aleph::equal_to<T>> inline
T * sequential_search(const DynList<T> & list, const T & x, 
		      Equal && eq = Equal()) noexcept
{
  return sequential_search<T, Equal>(list, x, eq);
}

    template <typename T, class Compare = Aleph::less<T>> inline
int search_extreme(T * a, const int l, const int r, Compare & cmp) noexcept
{
  T extreme_index = l;
  for (int i = l + 1; i <= r; i++)
    if (cmp(a[i], a[extreme_index])) // ¿se ve un nuevo menor?
      extreme_index = i; // sí

  return extreme_index;
}

    template <typename T, class Compare = Aleph::less<T>> inline
int search_extreme(T * a, const int l, const int r, 
		   Compare && cmp = Compare()) noexcept
{
  return search_extreme<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
int search_min(T * a, const int l, const int r, Compare & cmp) noexcept
{
  return search_extreme<T, Compare> (a, l, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
int search_min(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept
{
  return search_min<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare = Aleph::greater<T>> inline
int search_max(T * a, const int l, const int r, Compare & cmp) noexcept
{
  return search_extreme<T, Compare> (a, l, r, cmp);
}

    template <typename T, class Compare = Aleph::greater<T>> inline
int search_max(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept
{
  return search_max<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare> inline 
Dnode<T> * search_extreme(const Dnode<T> & list, Compare & cmp) noexcept
{
  Compare_Dnode<T, Compare> cmp_dnode(cmp);
  Dlink * ret = 
    search_extreme<Dlink, Compare_Dnode<T, Compare>>((Dlink&) list, cmp_dnode);

  return static_cast <Dnode<T>*> (ret);
}

    template <typename T, class Compare> inline
T * search_extreme(const DynDlist<T> & list, Compare & cmp) noexcept
{
  Dnode<T> * ret = search_extreme <T, Compare> ((Dnode<T>&) list, cmp); 

  return ret != nullptr ? &(ret->get_data()) : nullptr;
}

    template <typename T, class Compare = Aleph::less<T>> inline 
Dnode<T> * search_extreme(const Dnode<T> & list, Compare && cmp = Compare())
      noexcept
{
  return search_extreme<T, Compare>(list, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
T * search_extreme(const DynDlist<T> & list, Compare && cmp = Compare()) 
      noexcept
{
  return search_extreme<T, Compare>(list, cmp);
}

    template <typename T, class Compare> inline 
T * search_extreme(const DynList<T> & list, Compare & cmp) noexcept
{
  Slinknc * ret =
    search_extreme <Compare_Snodenc<T, Compare>>
      (*list.get_head(), Compare_Snodenc<T, Compare>(cmp)); 

  return ret != nullptr ? &(((Snodenc<T>*)ret)->get_data()) : nullptr;
}

    template <typename T, class Compare> inline 
T * search_extreme(const DynList<T> & list, Compare && cmp = Compare()) noexcept
{
  return search_extreme<T, Compare>(list, cmp);
}

    template <typename T, class Compare> inline
T * search_min(const DynDlist<T> & list, Compare & cmp) noexcept
{
  return search_extreme<T, Compare> (list, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
T * search_min(const DynDlist<T> & list, Compare && cmp = Compare()) noexcept
{
  return search_min<T, Compare>(list, cmp);
}

    template <typename T, class Compare> inline
T * search_max(const DynDlist<T> & list, Compare & cmp) noexcept
{
  return search_extreme<T, Compare> (list, cmp);
}


    template <typename T, class Compare = Aleph::greater<T>> inline
T * search_max(const DynDlist<T> & list, Compare && cmp = Compare()) noexcept
{
  return search_max<T, Compare>(list, cmp);
}

    template <typename T, class Compare> inline
void insertion_sort(T * a, const size_t l, const size_t r, Compare & cmp)
      noexcept
{
  for (long i = l, j; i <= r; ++i)
    {
      T tmp = a[i]; // memorice a[i], pues será sobre escrito
      
      for (j = i; j > l and cmp(tmp, a[j - 1]); --j)
	a[j] = a[j - 1]; // desplazar hacia la derecha

      a[j] = tmp; // inserte tmp en la brecha
    }
}

    template <typename T, class Compare = Aleph::less<T>> inline
void insertion_sort(T * a, const size_t l, const size_t r,
		    Compare && cmp = Compare()) noexcept
{
  insertion_sort<T, Compare>(a, l, r, cmp);
}

    template <class Compare> inline 
void insert_sorted(Dlink & list, Dlink * p, Compare & cmp) noexcept
{
  typename Dlink::Iterator it(list); 
  while (it.has_curr() and cmp(it.get_curr(), p)) 
    it.next();

  if (it.has_curr())
    it.get_curr()->append(p); // insertar antes de current
  else
    list.append(p); 
}


    template <class Compare> inline 
void insert_sorted(Dlink & list, Dlink * p, Compare && cmp = Compare()) noexcept
{
  insert_sorted<Compare>(list, p, cmp);
}

template <class Compare> inline
void insert_sorted(HTList & list, Slinknc * p, Compare & cmp) noexcept
{
  Slinknc * first = list.get_first();
  if (cmp(p, first) or not cmp(first, p)) // p <= first?
    {
      list.insert(p);
      return;
    }

  Slinknc * last = list.get_last();
  if (cmp(last, p) or not cmp(p, last)) // p >= last?
    {
      list.append(p);
      return;
    }

  Slinknc * prev = list.get_first();
  typename HTList::Iterator it(list); 
  for (it.next(); it.has_curr(); it.next())
    {
      Slinknc * curr = it.get_curr();
      if (cmp(p, curr)) // p < curr
	{
	  prev->insert(p);
	  return;
	}
      prev = curr;
    }
  assert(false); // it should be unreachable
}

template <class Compare> inline
void insert_sorted(HTList & list, Slinknc * p, Compare && cmp = Compare())
  noexcept
{
  insert_sorted<Compare>(list, p, cmp);
}

template <class ListType, class Compare> inline 
void list_insertion_sort(ListType & list, Compare & cmp) noexcept
{
  if (list.is_empty())
    return;

  ListType aux; aux.append(list.remove_first());
  while (not list.is_empty())
    insert_sorted<Compare>(aux, list.remove_first(), cmp);

  list.swap(aux);
}


template <typename T, class Compare> inline 
void insertion_sort(DynList<T> & l, Compare & cmp) noexcept
{
  using Cmp = Compare_Snodenc<T, Compare>;
  Cmp c(cmp);  
  list_insertion_sort<HTList, Cmp>(l, c);
}

template <typename T, class Compare = Aleph::less<T>> inline 
void insertion_sort(DynList<T> & l, Compare && cmp = Compare()) noexcept
{
  insertion_sort<T, Compare>(l, cmp);
}

template <typename T, class Compare> inline 
DynList<T> insertion_sort(DynList<T> && l, Compare & cmp) noexcept
{
  using Cmp = Compare_Snodenc<T, Compare>;
  Cmp c(cmp);  
  list_insertion_sort<HTList, Cmp>(l, c);
  return move(l);
}

template <typename T, class Compare = Aleph::less<T>> inline 
DynList<T> insertion_sort(DynList<T> && l, Compare && cmp = Compare()) noexcept
{
  return insertion_sort<T, Compare>(move(l), cmp);
}



    template <typename T, class Compare> 
inline void insertion_sort(Dnode<T> & list, Compare & cmp) noexcept
{
  using Cmp = Compare_Dnode<T, Compare>;
  Cmp c(cmp);
  list_insertion_sort <Dlink, Cmp> (list, c);
}


    template <typename T, class Compare = Aleph::less<T>> 
inline void insertion_sort(Dnode<T> & list, Compare && cmp = Compare())
{
  insertion_sort<T, Compare>(list, cmp);
}


    template <typename T, class Compare> inline
void merge(T * a, const int l, const int m, const int r, Compare & cmp)
      noexcept
{
  int i, j, k, s;
  s = r - l + 1;
  T b[s];
 
  for (i = l, k = 0; i <= m; ++i, ++k)
    b[k] = a[i];

  for (j = r; j > m; ++k, --j)
    b[k] = a[j];

  for (k = l, i = 0, j = s - 1; k <= r; ++k)
    if (cmp(b[i], b[j]))
      a[k] = b[i++];
    else
      a[k] = b[j--];
}

    template <typename T, class Compare = Aleph::less<T>> inline
void merge(T * a, const int l, const int m, const int r, 
	   Compare && cmp = Compare()) noexcept
{
  merge<T, Compare>(a, l, m, r, cmp);
}

    template <typename T, class Compare> inline
void mergesort(T * a, const int l, const int r, Compare & cmp) noexcept
{
  if (l >= r) 
    return;

  const int m = (l + r)/2;

  mergesort<T, Compare>(a, l, m, cmp);
  mergesort<T, Compare>(a, m + 1, r, cmp);

  if (cmp(a[m], a[m + 1]))
    return;

  merge<T, Compare>(a, l, m, r, cmp);
}


    template <typename T, class Compare = Aleph::less<T>> inline
void mergesort(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept
{
  mergesort<T, Compare>(a, l, r, cmp);
}

    template <class Tlist, class Compare> inline
void merge_lists(Tlist & l1, Tlist & l2, Tlist & result, Compare & cmp) noexcept
{
  assert(result.is_empty());

  while (not l1.is_empty() and not l2.is_empty())
    if (cmp(l1.get_first(), l2.get_first()))
      result.append(l1.remove_first());
    else
      result.append(l2.remove_first());

  if (l1.is_empty())
    result.concat_list(l2);
  else
    result.concat_list(l1);

  assert(l1.is_empty() and l2.is_empty());
}

    template <class Tlist, class Compare> inline
void merge_lists(Tlist & l1, Tlist & l2, Tlist & result,
		 Compare && cmp = Compare()) noexcept
{
  merge_lists<Tlist, Compare>(l1, l2, result, cmp);
}

     template <class T, class Compare> inline
void merge_lists(Dnode<T> & l1, Dnode<T> & l2, Dnode<T> & result,
		 Compare & cmp) noexcept
{
  merge_lists<Dnode<T>, Compare_Dnode<T, Compare>>(l1, l2, result, cmp);
}

     template <class T, class Compare = Aleph::less<T>> inline
void merge_lists(Dnode<T> & l1, Dnode<T> & l2, Dnode<T> & result,
		 Compare && cmp = Compare()) noexcept
{
  merge_lists<Dnode<T>, Compare_Dnode<T, Compare>>(l1, l2, result, cmp);
}


    template <class Tlist, class Compare> inline 
void mergesort(Tlist & list, Compare & cmp) noexcept
{
  if (list.is_unitarian_or_empty()) 
    return;

  Tlist l, r;
  list.split_list(l, r);         // dividir en dos listas

  mergesort <Tlist, Compare> (l, cmp);
  mergesort <Tlist, Compare> (r, cmp);  
  
  merge_lists <Tlist, Compare> (l, r, list, cmp); // mezclarlas 
}

    template <class Tlist, class Compare> inline 
void mergesort(Tlist & list, Compare && cmp = Compare()) noexcept
{
  mergesort<Tlist, Compare>(list, cmp);
}


template <template <typename> class Tlist, typename T, class Compare> inline 
void mergeinsertsort(Tlist<T> & list, Compare & cmp, 
		     const size_t lsz = Aleph::Insertion_Threshold)
  noexcept
{
  if (lsz < Aleph::Insertion_Threshold)
    {
      insertion_sort<T, Compare>(list, cmp);
      return;
    }

  if (list.is_unitarian_or_empty()) 
    return;

  Tlist<T> l, r;
  list.split_list(l, r);         // dividir en dos listas

  mergeinsertsort <Tlist, T, Compare> (l, cmp);
  mergeinsertsort <Tlist, T, Compare> (r, cmp);  
  
  merge_lists <Tlist<T>, Compare> (l, r, list, cmp); // mezclarlas 
}

template <template <typename> class Tlist, typename T, class Compare> inline 
void mergeinsertsort(Tlist<T> & list, Compare && cmp = Compare()) noexcept
{
  mergeinsertsort<Tlist, T, Compare>(list, cmp);
}

template <template <typename> class Tlist, typename T, class Compare> inline 
void mergesort(Tlist<T> & list, Compare & cmp) noexcept
{
  if (list.is_unitarian_or_empty()) 
    return;

  Tlist<T> l, r;
  list.split_list(l, r);         // dividir en dos listas

  mergesort <Tlist, T, Compare> (l, cmp);
  mergesort <Tlist, T, Compare> (r, cmp);  
  
  merge_lists <Tlist<T>, Compare> (l, r, list, cmp); // mezclarlas 
}

template <template <typename> class Tlist, typename T, class Compare> inline 
void mergeort(Tlist<T> & list, Compare && cmp = Compare()) noexcept
{
  mergesort<Tlist, T, Compare>(list, cmp);
}



    template <typename T, class Compare> inline
void mergesort(Dnode<T> & list, Compare & cmp) noexcept
{
  mergesort <Dnode<T>, Compare_Dnode<T, Compare>> (list, cmp);
}


    template <typename T, class Compare = Aleph::less<T>> inline
void mergesort(Dnode<T> & list, Compare && cmp = Compare()) noexcept
{
  mergesort<Dnode<T>, Compare_Dnode<T, Compare>>(list, cmp);
}

    template <typename T, class Compare> inline
void mergesort(DynDlist<T> & list, Compare & cmp) noexcept
{
  mergesort <Dnode<T>, Compare_Dnode<T, Compare>> (list, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void mergesort(DynDlist<T> & list, Compare && cmp = Compare()) noexcept
{
  mergesort<T, Compare>(list, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void mergesort(DynList<T> & list, Compare & cmp) noexcept
{
  mergesort<DynList<T>, Compare>(list, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void mergesort(DynList<T> & list, Compare && cmp = Compare()) noexcept
{
  mergesort<DynList<T>, Compare>(list, cmp);
}

    template <typename T, class Compare> inline
int select_pivot(T * a, const int l, const int r, Compare & cmp) noexcept; 

    template <typename T, class Compare = Aleph::less<T>> inline
int select_pivot(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept;


    template <typename T, class Compare> inline
int partition(T * a, const int l, const int r, Compare & cmp) noexcept
{
  const int p = select_pivot <T, Compare> (a, l, r, cmp);
  std::swap(a[p], a[r]);

  T pivot = a[r]; // elemento pivot
  int i = l - 1,  // índice primer elemento a la izquierda > que pivot
      j = r;      // índice primer elemento a la derecha < que pivot
  while (true)
    {
          // avance mientras a[i] < a[pivot]
      while (cmp(a[++i], pivot)) { /* no hay cuerpo */ }

      while (cmp(pivot, a[--j])) // avance mientras a[pivot]< a[j] 
	if (j == l) // ¿se alcanzó el borde izquierdo?
	  break; // sí ==> hay que terminar la iteración

      if (i >= j) 
        break;

        // En este punto hay una inversión a[i] > a[pivot] > a[j] 
      std::swap(a[i], a[j]); // Eliminar la inversión
    }

  std::swap(a[i], a[r]);

  return i;
}

    template <typename T, class Compare> inline
int partition(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept
{
  return partition<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare> inline
void quicksort_rec(T * a, const int l, const int r, Compare & cmp) noexcept
{
  if (l >= r) 
    return;

  const int pivot = partition <T, Compare> (a, l, r, cmp);

  quicksort_rec <T, Compare> (a, l, pivot - 1, cmp);
  quicksort_rec <T, Compare> (a, pivot + 1, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline
void quicksort_rec(T * a, const int l, const int r, 
		   Compare && cmp = Compare()) noexcept
{
  quicksort_rec<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare> inline
void quicksort_no_tail(T * a, int l, int r, Compare & cmp) noexcept
{
 start:
  if (l >= r) 
    return;

  const int pivot = partition <T, Compare> (a, l, r, cmp);

  quicksort_no_tail <T, Compare> (a, l, pivot - 1, cmp);

  l = pivot + 1;
  goto start;
}

    template <typename T, class Compare = Aleph::less<T>> inline
void quicksort_no_tail(T * a, int l, int r, 
		       Compare && cmp = Compare()) noexcept
{
  quicksort_no_tail<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare> inline
void quicksort_rec_min(T * a, const int l, const int r, Compare & cmp) noexcept
{
  if (r <= l) 
    return;  

  const int pivot = partition<T, Compare>(a, l, r, cmp);
  if (pivot - l < r - pivot) // ¿cual es la partición más pequeña?
    {     // partición izquierda más pequeña
      quicksort_rec_min<T, Compare>(a, l, pivot - 1, cmp);
      quicksort_rec_min<T, Compare>(a, pivot + 1, r, cmp);
    }
  else
    {     // partición derecha más pequeña
      quicksort_rec_min<T, Compare>(a, pivot + 1, r, cmp);
      quicksort_rec_min<T, Compare>(a, l, pivot - 1, cmp);
    }
} 


    template <typename T, class Compare = Aleph::less<T>> inline
void quicksort_rec_min(T * a, const int l, const int r, 
		       Compare && cmp = Compare()) noexcept
{
  quicksort_rec_min<T, Compare>(a, l, r, cmp);  
}


    template <typename T, class Compare> inline
int select_pivot(T * a, const int l, const int r, Compare & cmp) noexcept
{
  assert(l <= r);

  if (r - l <= 2) 
    return r;

  const int m = (r + l) / 2; // índice del centro
  const int p = cmp(a[l], a[m]) ? m : l; // p=max(a[l],a[m])

  return cmp (a[r], a[m]) ? r : p; // retornar min(a[r], a[m])
}


    template <typename T, class Compare> inline
int select_pivot(T * a, const int l, const int r, Compare && cmp) noexcept
{
  return select_pivot<T, Compare>(a, l, r, cmp);
}

    template <typename T, class Compare> inline
void quicksort(T * a, const int l, const int r, Compare & cmp) noexcept
{
  if (r - l < Quicksort_Threshold) 
    {
      insertion_sort(a, l, r, cmp);
      return;
    }

  typedef typename std::pair<int, int> Partition; 
  FixedStack<Partition> stack(64);
  stack.push(Partition(l, r)); // todo el arreglo como partición inicial

  while (stack.size() > 0)
    {
      const Partition p = stack.pop();
      const long diff = p.second - p.first;
      
      if (diff <= 1)
	continue;

      if (diff < Quicksort_Threshold)
	{
	  insertion_sort(a, p.first, p.second, cmp);
	  continue;
	}

      const int pivot = partition <T, Compare>(a, p.first, p.second, cmp);

      if (pivot - p.first < p.second - pivot) // ¿cuál más pequeña?
	{     // partición izquierda más pequeña
	  stack.push(Partition(pivot + 1, p.second));
	  stack.push(Partition(p.first, pivot - 1));
	}
      else
	{     // partición derecha más pequeña
	  stack.push(Partition(p.first, pivot - 1));
	  stack.push(Partition(pivot + 1, p.second));
	}
    }
}

    template <typename T, class Compare = Aleph::less<T>> inline
void quicksort(T * a, const int l, const int r, Compare && cmp = Compare())
      noexcept
{
  quicksort<T, Compare>(a, l, r, cmp);
}

    template <class Compare> 
void quicksort(Dlink & list, Compare & cmp) noexcept
{
  if (list.is_unitarian_or_empty()) 
    return;

  Dlink * pivot = list.remove_next();
  Dlink smaller, bigger; // listas de menores y mayores que pivot

  while (not list.is_empty()) 
    {
      Dlink * p = list.remove_next();
      if (cmp(p, pivot))
	smaller.append(p);
      else
	bigger.append(p);
    }

  quicksort <Compare> (bigger, cmp);  
  quicksort <Compare> (smaller, cmp);

  list.concat_list(&smaller); // restaurar listas ordenadas en list
  list.append(pivot);
  list.concat_list(&bigger);
} 

    template <class Compare> 
void quicksort(Dlink & list, Compare && cmp = Compare()) noexcept
{
  quicksort<Compare>(list, cmp);
}

    template <typename T, class Compare>
void quicksort(Dnode<T> & list, Compare & cmp) noexcept
{
  quicksort <Compare_Dnode<T, Compare>> (list, cmp);
}


    template <typename T, class Compare = Aleph::less<T>>
void quicksort(Dnode<T> & list, Compare && cmp = Compare()) noexcept
{
  quicksort<T, Compare>(list, cmp);
}


template <typename T, class Compare> 
void quicksort(HTList & list, Compare & cmp) noexcept
{
  if (list.is_unitarian_or_empty()) 
    return;

  Snodenc<T> * pivot = (Snodenc<T> *) list.remove_first();
  HTList smaller, bigger; // listas de menores y mayores que pivot

  while (not list.is_empty()) 
    {
      Snodenc<T> * p = (Snodenc<T> *) list.remove_first();
      if (cmp(p->get_data(), pivot->get_data()))
	smaller.append(p);
      else
	bigger.append(p);
    }

  quicksort <T, Compare> (bigger, cmp);  
  quicksort <T, Compare> (smaller, cmp);

  list.concat_list(smaller); // restaurar listas ordenadas en list
  list.append(pivot);
  list.concat_list(bigger);
} 


    template <typename T, class Compare = Aleph::less<T>>
void quicksort(DynList<T> & list, Compare && cmp = Compare()) noexcept
{
  quicksort<T, Compare>((HTList&) list, cmp);
}

     template <typename T, class Compare> inline
void quicksort_insertion(T * a, const int l, const int r, Compare & cmp) 
       noexcept
{
  if (r <= l) 
    return;

  const int pivot = partition<T, Compare>(a, l, r, cmp);

  const int l_size = pivot - l; // tamaño partición izquierda
  const int r_size = r - pivot; // tamaño partición derecha
  bool left_done  = false; // true si partición izq está ordenada
  bool right_done = false; // true si partición der está ordenada

  if (l_size <= Aleph::Insertion_Threshold) 
    {     // ordene partición izq por inserción
      insertion_sort<T, Compare>(a, l, pivot - 1, cmp);  
      left_done = true;
    }

  if (r_size <= Aleph::Insertion_Threshold) 
    {     // ordene partición der  por inserción
      insertion_sort<T, Compare>(a, pivot + 1, r, cmp);
      right_done = true;
    }

  if (left_done and right_done) 
    return; // ambas particiones ordenadas por inserción

  if (left_done) // ¿partición izq ordenada por inserción?
    { // sí; sólo resta ordenar recursivamente partición der
      quicksort_insertion<T, Compare>(a, pivot + 1, r, cmp);
      return;
    }

  if (right_done) // ¿partición der ordenada por inserción?
    { // sí; sólo resta ordenar recursivamente partición izq
      quicksort_insertion<T, Compare>(a, l, pivot - 1, cmp);
      return;
    }

      // aquí, ambas particiones no fueron ordenadas por inserción
  if (l_size < r_size) // ordenar primero partición más pequeña 
    {     // partición izquierda más pequeña
      quicksort_insertion <T, Compare> (a, l, pivot - 1, cmp);
      quicksort_insertion <T, Compare> (a, pivot + 1, r, cmp);
    }
  else
    {     // partición derecha más pequeña
      quicksort_insertion <T, Compare> (a, pivot + 1, r, cmp);
      quicksort_insertion <T, Compare> (a, l, pivot - 1, cmp);
    }
}    

     template <typename T, class Compare = Aleph::less<T>> inline
void quicksort_insertion(T * a, const int l, const int r,
			 Compare && cmp = Compare()) noexcept
{
  quicksort_insertion<T, Compare>(a, l, r, cmp);
}

     template <typename T, class Compare> inline
int random_search(T * a, const T & x, const long l, const long r, 
		  Compare & cmp) noexcept
{
  if (l > r)
    return Not_Found;

  const long pivot = partition<T, Compare>(a, l, r, cmp);

  if (cmp(x, a[pivot]))
    return random_search<T, Compare>(a, x, l, pivot - 1, cmp);
  else if (cmp(a[pivot], x))
    return random_search<T, Compare>(a, x, pivot + 1, r, cmp);

  return pivot; // elemento encontrado en el índice x
}

     template <typename T, class Compare = Aleph::less<T>> inline
int random_search(T * a, const T & x, const long l, const long r,
		  Compare && cmp = Compare()) noexcept
{
  return random_search<T, Compare>(a, x, l, r, cmp);
}


    template <typename T, class Compare> inline
int random_search(DynArray<T> & a, const T & x, const long l, const long r,
		  Compare & cmp) noexcept
{
  if (l > r)
    return Not_Found;

  const long pivot = partition<T, Compare>(a, l, r, cmp);

  if (cmp(x, a(pivot)))
    return random_search<T, Compare>(a, x, l, pivot - 1, cmp);
  else if (cmp(a(pivot), x))
    return random_search<T, Compare>(a, x, pivot + 1, r, cmp);

  return pivot; // elemento encontrado en el índice x
}

template <typename T, class Compare = Aleph::less<T>> inline
int random_search(DynArray<T> & a, const T & x, const long l, const long r,
		  Compare && cmp = Compare()) noexcept
{
  return random_search<T, Compare>(a, x, l, r, cmp);
}

    template <typename T, class Compare> inline
Dnode<T> * dlink_random_search(Dlink & list, const T & x, Compare & cmp)
      noexcept
{
  if (list.is_empty()) 
    return nullptr;

  Dnode<T>   item(x);
  Dnode<T> * item_ptr = &item; // puntero a celda que contiene a x

  Dlink smaller; // lista de los menores que pivot
  Dlink bigger;  // lista de los mayores que pivot

  Dnode<T> * pivot = static_cast<Dnode<T>*>(list.remove_next());
  
  while (not list.is_empty()) 
    {
      Dlink * p = list.remove_next();
      if (cmp(p, pivot))
	smaller.append(p);
      else
	bigger.append(p);
    }

  Dnode<T> * ret_val = nullptr;
  if (cmp(item_ptr, pivot))
    ret_val = dlink_random_search <T, Compare> (smaller, x, cmp);
  else if (cmp(pivot, item_ptr))
    ret_val = dlink_random_search <T, Compare> (bigger, x, cmp);
  else
    ret_val = pivot;

  assert(list.is_empty());

  list.swap(&smaller); 
  list.append(pivot);  
  list.concat_list(&bigger);

  return ret_val;
}

    template <typename T, class Compare> inline
Dnode<T> * dlink_random_search(Dlink & list, const T & x,
			       Compare && cmp = Compare()) noexcept
{
  return dlink_random_search<T, Compare>(list, x, cmp);
}

    template <typename T, class Compare>
Dnode<T> * random_search(Dlink & list, const T & x, Compare & cmp) noexcept
{
  return dlink_random_search <T, Compare_Dnode<T, Compare>> (list, x, cmp);
}

    template <typename T, class Compare = Aleph::less<T>>
Dnode<T> * random_search(Dlink & list, const T & x, Compare && cmp = Compare())
      noexcept
{
  return random_search<T, Compare>(list, x, cmp);
}


    template <typename T, class Compare> inline
T * random_search(DynDlist<T> & list, const T & x, Compare & cmp) noexcept
{
  Dnode<T> * p = 
    dlink_random_search <T, Compare_Dnode<T, Compare>> (list, x, cmp);

  return p == nullptr ? nullptr : &(p->get_data());
}

    template <typename T, class Compare = Aleph::less<T>> inline
T * random_search(DynDlist<T> & list, const T & x, Compare && cmp = Compare())
      noexcept
{
  return random_search<T, Compare>(list, x, cmp);
}

    template <typename T, class Compare> static inline
const T & __random_select(T * a, const long i, const long l, const long r,
			  Compare & cmp) noexcept
{
  const long pivot = partition<T, Compare>(a, l, r, cmp);
  if (i == pivot) 
    return a[i];

  if (i < pivot) // ¿está en partición izquierda?
    return __random_select<T, Compare>(a, i, l, pivot - 1, cmp);
  else
    return __random_select<T, Compare>(a, i, pivot + 1, r, cmp);
}

    template <typename T, class Compare> static inline
const T & __random_select(T * a, const long i, const long l, const long r,
			  Compare && cmp = Compare()) noexcept
{
  return __random_select<T, Compare>(a, i, l, r, cmp);
}


    template <typename T, class Compare> inline
long select_pivot_op(const DynArray<T> & a, const long l, const long r,
		     Compare & cmp) noexcept
{
  assert(l <= r);

  if (r - l <= 5)
    return r;

  const long m = (l + r)/2; // elemento central

      // Accede las entradas del arreglo una sola vez
  T & la = a.access(l); 
  T & ra = a.access(r);
  T & ma = a.access(m);
  
  const T * med_ptr = Aleph::median<T, Compare>(la, ma, ra, cmp);
  
  if (med_ptr == &la)
    return l;

  if (med_ptr == &ma)
    return m;

  assert(med_ptr == &ra);

  return r;
}

    template <typename T, class Compare> inline
int select_pivot_op(const DynArray<T> & a, const long l, const long r,
		    Compare && cmp = Compare()) noexcept
{
  return select_pivot_op<T, Compare>(a, l, r, cmp);
}


    template <typename T, class Compare> inline
int partition_op(const DynArray<T> & a, long l, long r, Compare & cmp) noexcept
{
  if (l == r)
    return l;

  long i = l - 1, 
       j = r;

  T & pivot       = a.access(r);
  long pivot_idx  = select_pivot_op<T, Compare>(a, l, r, cmp);
  T & pivot_value = a.access(pivot_idx);
  std::swap(pivot, pivot_value);

  while (true)
    {
      while (cmp(a.access(++i), pivot)) ; 

      while (cmp(pivot, a.access(--j)))
	if (j == l)
	  break;

      if (i >= j)
	break;

      std::swap(a.access(i), a.access(j));
    }

  std::swap(a.access(i), a.access(r));

  return i;
}

    template <typename T, class Compare> inline
int partition_op(const DynArray<T> & a, long l, long r, 
		 Compare && cmp = Compare()) noexcept
{
  return partition_op<T, Compare>(a, l, r, cmp);
}


    template <typename T, class Compare> static inline
const T & __random_select(const DynArray<T> & a, const long i, 
			  const long l, const long r, Compare & cmp) noexcept
{
  assert(i >= l and i <= r);

  const long pivot = partition_op<T, Compare>(a, l, r, cmp);
  if (i == pivot) 
    return a(i);

  if (i < pivot) // ¿está en partición izquierda?
    return __random_select<T, Compare>(a, i, l, pivot - 1, cmp);
  else
    return __random_select<T, Compare>(a, i, pivot + 1, r, cmp);
}

    template <typename T, class Compare> static inline
const T & __random_select(const DynArray<T> & a, const long i, 
			  const long l, const long r,
			  Compare && cmp = Compare()) noexcept
{
  return __random_select<T, Compare>(a, i, l, r, cmp);
}


template <typename T, class Compare> 
const T & random_select(DynArray<T> & a, const long i, Compare & cmp) noexcept
{
  const int n = a.size() - 1;
  if (i > n)
    throw std::out_of_range("index out of range");

  return __random_select<T, Compare>(a, i, 0, n, cmp);
}

  template <typename T, class Compare = Aleph::less<T>> 
const T & random_select(DynArray<T> & a, const long i, 
			Compare && cmp = Compare()) noexcept
{
  return random_select<T, Compare>(a, i, cmp);
}


  template <typename T, class Compare> 
const T & random_select(T * a, const long i, const long n, Compare & cmp)
    noexcept
{
  if (i >= n)
    throw std::out_of_range("index out of range");

  return __random_select<T, Compare>(a, i, 0, n - 1, cmp);
}

  template <typename T, class Compare = Aleph::less<T>> 
const T & random_select(T * a, const long i, const long n, 
			Compare && cmp = Compare()) noexcept
{
  return random_select<T, Compare>(a, i, n, cmp);
}

    template <class Compare>
Dlink * dlink_random_select(Dlink & list, const size_t i, Compare & cmp)
      noexcept
{
  if (list.is_empty()) 
    return nullptr;

  Dlink smaller; // lista de los menores que pivot
  Dlink bigger;  // lista de los mayores que pivot

  size_t smaller_count = 0, // cantidad de elementos de smaller
         bigger_count  = 0; // cantidad de elementos de bigger

  Dlink * pivot = list.remove_next();
  
  while (not list.is_empty()) 
    {
      Dlink * p = list.remove_next();
      if (cmp(p, pivot)) // ¿p < pivot?
	{  smaller.append(p); ++smaller_count;  }
      else
	{  bigger.append(p); ++bigger_count;  }
    }

  if (i >= smaller_count + bigger_count + 1) 
    throw std::out_of_range("index of selection greater than list's size");

  Dlink * ret_val = nullptr;
  if (i == smaller_count)
    ret_val = pivot;
  else if (i < smaller_count)
    ret_val = dlink_random_select<Compare>(smaller, i, cmp);
  else
    ret_val = dlink_random_select<Compare>(bigger, i - (smaller_count+1), cmp);

  list.concat_list(&smaller);
  list.append(pivot);
  list.concat_list(&bigger);

  return ret_val;
}


    template <class Compare>
Dlink * dlink_random_select(Dlink & list, const size_t i, 
			    Compare && cmp = Compare()) noexcept
{
  return dlink_random_select<Compare>(list, i, cmp);
}

    template <typename T, class Compare>
Dnode<T> * random_select(Dlink & list, const size_t i, Compare & cmp) noexcept
{
  return (Dnode<T>*) 
    dlink_random_select<Compare_Dnode<T, Compare>> (list, i, cmp);
}


    template <typename T, class Compare = Aleph::less<T>>
Dnode<T> * random_select(Dlink & list, const size_t i, 
			 Compare && cmp = Compare()) noexcept
{
  return random_select<T, Compare> (list, i, cmp);
}

    template <typename T, class Compare>
T * random_select(DynDlist<T> & list, const size_t i, Compare & cmp)
      noexcept
{
  Dlink * link = dlink_random_select <Compare_Dnode<T, Compare>> (list, i, cmp);

  Dnode<T> * p = static_cast<Dnode<T>*>(link);

  return p != nullptr ? &(p->get_data()) : nullptr;
}

    template <typename T, class Compare = Aleph::less<T>>
T * random_select(DynDlist<T> & list, const size_t i, 
		  Compare && cmp = Compare()) noexcept
{
  return random_select<T, Compare>(list, i, cmp);
}
    template <class T, class Compare> inline
void selection_sort(DynArray<T> & a, Compare & cmp) noexcept
{
  const int n = a.size();

  for (int i = 0; i < n - 1; i++)
    {
      int min = i;

      for (int j = i + 1; j < n; j++)
	if (cmp(a.access(j), a.access(min)))
	  min = j;

      if (cmp(a.access(min), a.access(i)))
	std::swap(a.access(min), a.access(i));
    }
}

    template <class T, class Compare = Aleph::less<T>> inline
void selection_sort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
  selection_sort<T, Compare>(a, cmp);
}

    template <class T, class Compare> inline
void bubble_sort(DynArray<T> & a, Compare & cmp) noexcept
{
  const int n = a.size();

  for (int i = 0; i < n - 1; i++)
    for (int j = n - 1; j > i; j--)
      if (cmp(a.access(j), a.access(j - 1)))
	std::swap(a.access(j - 1), a.access(j));
}

    template <class T, class Compare = Aleph::less<T>> inline
void bubble_sort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
  bubble_sort<T, Compare>(a, cmp);
}

    template <class T, class Compare> inline
void insertion_sort(DynArray<T> & a, long l, long r, Compare & cmp) noexcept
{
  for (long i = l + 1; i <= r; i++)
    {
      T tmp = a.access(i);
      long j = i;
      for (/* nothing */; j > 0 and cmp(tmp, a.access(j - 1)); --j)
	a.access(j) = a.access(j - 1);

      a.access(j) = tmp;
    }
}

    template <class T, class Compare = Aleph::less<T>> inline
void insertion_sort(DynArray<T> & a, long l, long r, Compare && cmp = Compare())
      noexcept
{
  insertion_sort<T, Compare>(a, l, r, cmp);
}

    template <class T, class Compare> inline
void insertion_sort(DynArray<T> & a, Compare & cmp) noexcept
{
  insertion_sort<T, Compare> (a, 0, a.size() - 1, cmp);
}

    template <class T, class Compare = Aleph::less<T>> inline
void insertion_sort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
  insertion_sort<T, Compare> (a, cmp);
}

    template <class T, class Compare> inline
void shellsort(DynArray<T> & a, Compare & cmp) noexcept
{
  const long n = a.size();
  int incs[16] = { 1391376, 463792, 198768, 86961, 33936, 13776, 
		   4592, 1968, 861, 336, 112, 48, 21, 7, 3, 1 }; 

  for (long k = 0; k < 16; k++) 
    { 
      const long h = incs[k]; 
      for (long i = h; i < n; i++) 
      { 
	T tmp = a.access(i); 
	long j = i;

	while (j >= h and cmp(tmp, a.access(j - h)))
	  {
	    a.access(j) = a.access(j - h);
	    j -= h;
	  } 

	a.access(j) = tmp;
      } 
    } 
}

    template <class T, class Compare = Aleph::less<T>> inline
void shellsort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
 shellsort<T, Compare>(a, cmp);
}
    inline static 
long back_index(const long i) noexcept { return i - 1; }


    template <class T, class Compare> inline
void sift_up(DynArray<T> & table, const size_t n, Compare & cmp) noexcept
{ 
  long p;
  for (long i = n; i > 1; i = p) 
    {
      p = i >> 1;     // c = i/2 

      if (cmp(table.access(back_index(p)), table.access(back_index(i))))
	return;

      std::swap(table.access(back_index(p)), table.access(back_index(i)));
    }
}


    template <class T, class Compare = Aleph::less<T>> inline
void sift_up(DynArray<T> & table, const size_t n, Compare && cmp = Compare()) 
      noexcept
{
  sift_up<T, Compare>(table, n, cmp);
}

    template <class T, class Compare> inline
void sift_down(DynArray<T> & table, const size_t n, Compare & cmp) noexcept
{
  long i = 1;

  while (true)
    {
      long c = i << 1; // c = 2*i

      if (c > n) 
	return;

      if (c + 1 <= n)
	if (cmp(table.access(back_index(c + 1)), table.access(back_index(c))))
	  c++;

      if (cmp(table.access(back_index(i)), table.access(back_index(c))))
	return; 

      std::swap(table.access(back_index(c)), table.access(back_index(i)));
      i = c;
    }
}

    template <class T, class Compare = Aleph::less<T>> inline
void sift_down(DynArray<T> & table, const size_t n, Compare && cmp = Compare()) 
      noexcept
{
  sift_down<T, Compare>(table, n, cmp);
}

    template <class T, class Compare>
class Negate_Compare
{
  Compare & cmp;

public:

  Negate_Compare(Compare & _cmp) 
  noexcept(std::is_nothrow_copy_assignable<Compare>::value)
    : cmp(_cmp) { /* Empty */ }

  Negate_Compare(Compare && _cmp = Compare()) 
  noexcept(std::is_nothrow_move_assignable<Compare>::value)
    : cmp(_cmp) { /* Empty */ }

  bool operator () (const T & e1, const T & e2) noexcept
  {
    return not cmp(e1, e2);
  }
};

    template <class T, class Compare> inline
void heapsort(DynArray<T> & a, Compare & cmp) noexcept
{
  const long n = a.size();

  long i;
  for (i = 2; i <= n; i++)
    sift_up<T, Negate_Compare<T, Compare>>(a, i, 
                                Negate_Compare<T, Compare>(cmp));

  for (i = n; i >= 2; i--)
    {
      std::swap(a.access(0), a.access(i - 1));

      sift_down<T, Negate_Compare<T, Compare>>(a, i - 1,
                                 Negate_Compare<T, Compare>(cmp));
    }
}

    template <class T, class Compare> inline
void heapsort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
  heapsort<T, Compare>(a, cmp);
}

    template <class T, class Compare> inline
long partition(DynArray<T> & a, long l, long r, Compare & cmp) noexcept
{
  if (l == r)
    return l;

  long i = l - 1, 
       j = r;
  const T & pivot = a.access(r);

  while (true)
    {
      while (cmp(a.access(++i), pivot)) { /* Más nada */ }

      while (cmp(pivot, a.access(--j)))
	if (j == l)
	  break;

      if (i >= j)
	break;
      
      std::swap(a.access(i), a.access(j));
    }

    std::swap(a.access(i), a.access(r));

  return i;
}


    template <class T, class Compare> inline
int partition(DynArray<T> & a, const long l, const long r,
	      Compare && cmp = Compare()) noexcept
{
  return partition<T, Compare>(a, l, r, cmp);
}


    template <class T, class Compare> inline 
void quicksort_rec(DynArray<T> & a, const long l, const long r, Compare & cmp)
      noexcept
{
  if (r <= l) 
    return;

  long i = partition<T, Compare>(a, l, r, cmp);
  if (i - l < r - i)
    {
      quicksort_rec<T, Compare>(a, l, i - 1, cmp);
      quicksort_rec<T, Compare>(a, i + 1, r, cmp);
    }
  else
    {
      quicksort_rec<T, Compare>(a, i + 1, r, cmp);
      quicksort_rec<T, Compare>(a, l, i - 1, cmp);
    }
}


    template <class T, class Compare = Aleph::less<T>> inline 
void quicksort_rec(DynArray<T> & a, const long l, const long r,
		   Compare && cmp = Compare()) noexcept
{
  quicksort_rec<T, Compare>(a, l, r, cmp);
}

# define push2(stack, a, b)  stack.push(b); stack.push(a);

    template <class T, class Compare> inline
void quicksort(DynArray<T> & a, Compare & cmp) noexcept
{
  long i, l = 0, r = a.size() -1;

  FixedStack<long> stack(40);

  push2(stack, l, r);

  while (not stack.is_empty())
    {
      l = stack.pop(); r = stack.pop();

      if (r <= l)
	continue;

      i = partition<T, Compare>(a, l, r, cmp);

      if (i - l > r - i)
	{
	  push2(stack, l, i - 1); push2(stack, i + 1, r);
	}
      else 
	{
	  push2(stack, i + 1, r); push2(stack, l, i - 1); 
	} 
    }
}

    template <class T, class Compare = Aleph::less<T>> inline
void quicksort(DynArray<T> & a, Compare && cmp = Compare()) noexcept
{
  quicksort<T, Compare>(a, cmp);
}

    template <class T, class Compare> inline
long search_extreme(const DynArray<T>& a, const long l, const long r, 
		    Compare & cmp) noexcept
{
  long extreme_index = l;

  for (long i = l + 1; i <= r; i++)
    if (cmp(a.access(i), a.access(extreme_index)))
      extreme_index = i;

  return extreme_index;
}

    template <class T, class Compare = Aleph::less<T>> inline
long search_extreme(const DynArray<T>& a, const long l, const long r,
		    Compare && cmp = Compare()) noexcept
{
  return search_extreme<T, Compare>(a, l, r, cmp);
}

    template <class T, class Compare> inline
long search_max(const DynArray<T>& a, const long l, const long r,
		Compare & cmp) noexcept
{
  return search_extreme<T, Compare>(a, l, r, cmp);
}  

    template <class T, class Compare = Aleph::greater<T>> inline
long search_max(const DynArray<T> & a, const long l, const long r,
		Compare && cmp = Compare()) noexcept
{
  return search_max<T, Compare>(a, l, r, cmp);
}

    template <class T, class Compare> inline
long binary_search(const DynArray<T> & a, const T & x, long l, long r,
		   Compare & cmp) noexcept
{
  if (l > r) 
    return l;

  long m;
  while (l <= r)
    {
      m = (l + r)/2;
      if (cmp(x, a(m)))
	r = m -1;
      else if (cmp(a(m), x))
	l = m + 1;
      else
	return m; // key found
    }

  return m;
} 

    template <class T, class Compare = Aleph::less<T>> inline
long binary_search(const DynArray<T> & a, const T & x, long l, long r,
		   Compare && cmp = Compare()) noexcept
{
  return binary_search(a, x, l, r, cmp);
}


    template <class T, class Compare> inline
long binary_search(const DynArray<T*> & a, const T & x, long l, long r,
		   Compare & cmp) noexcept
{
  if (l > r) 
    return l;

  long m;
  while (l <= r)
    {
      m = (l + r)/2;
      if (cmp(x, *a(m)))
	r = m - 1;
      else if (cmp(*a(m), x))
	l = m + 1;
      else
	return m; // clave encontrada
    }

  return m;
} 

    template <class T, class Compare = Aleph::less<T>> inline
long binary_search(const DynArray<T*> & a, const T & x, long l, long r,
		     Compare && cmp = Compare()) noexcept
{
  return binary_search(a, x, l, r, cmp);
}

    template <class T, class Compare = Aleph::less<T>> inline
long binary_search(const DynArray<T*> & a, const T & x,
		   Compare && cmp = Compare()) noexcept
{
  return binary_search(a, x, 0, a.size() - 1, cmp);
}

    template <class T, class Compare> inline
long binary_search(const DynArray<T> & a, const T & x, Compare & cmp) noexcept
{
  return binary_search<T, Compare>(a, x, 0, a.size() - 1, cmp);
}

    template <class T, class Compare = Aleph::less<T>> inline
long binary_search(const DynArray<T> & a, const T & x, 
		   Compare && cmp = Compare()) noexcept
{
  return binary_search(a, x, cmp);
}

    template <class T, class Compare> inline
DynList<size_t> binary_search_dup(const DynArray<T> & a, const T & x,
				  Compare & cmp) noexcept
{
  DynList<size_t> ret;
  long idx = binary_search(a, x, 0, a.size() - 1, cmp);
  if (idx < 0)
    return ret;

  if (not are_equals(a(idx), x, cmp))
    return ret;;

  ret.append(idx);
  for (long i = idx - 1; i >= 0; --i)
    {
      if (not are_equals(a(i), x, cmp))
	break;
      ret.insert(i);
    }

  for (long i = idx + 1, n = a.size(); i < n; ++i)
    {
      if (not are_equals(a(i), x, cmp))
	break;
      ret.append(i);
    }

  return ret;
}

    template <class T, class Compare = Aleph::less<T>> inline
DynList<size_t> binary_search_dup(const DynArray<T> & a, const T & x, 
				  Compare && cmp = Compare()) noexcept
{
  return binary_search_dup(a, x, cmp);
}

    template <class T, class Compare> inline
T * bsearch(const DynArray<T> & a, const T & x, Compare & cmp) noexcept
{
  long i = binary_search(a, x, cmp);
  if (i < 0)
    return nullptr;
  T * ptr = &a(i);
  return are_equals(*ptr, x, cmp) ? ptr : nullptr;
}

    template <class T, class Compare = Aleph::less<T>> inline
T* bsearch(const DynArray<T> & a, const T & x, 
	   Compare && cmp = Compare()) noexcept
{
  return bsearch(a, x, cmp);
}

    template <class T, class Compare> inline
T * bsearch(const DynArray<T*> & a, const T & x, Compare & cmp) noexcept
{
  long i = binary_search(a, x, cmp);
  if (i < 0)
    return nullptr;
  T * ptr = a(i);
  return are_equals(*ptr, x, cmp) ? ptr : nullptr;
}

    template <class T, class Compare = Aleph::less<T>> inline
T * bsearch(const DynArray<T*> & a, const T & x, Compare && cmp = Compare()) 
      noexcept
{
  return bsearch(a, x, cmp);
}

template <typename T, class Compare> inline
DynList<T*> bsearch_dup(const DynArray<T> & a, const T & x, Compare & cmp)
  noexcept
{
  DynList<T*> ret;
  long idx = binary_search(a, x, cmp);
  if (idx < 0)
    return ret;

  T * found_ptr = const_cast<T*>(&a(idx));
  if (not are_equals(*found_ptr, x, cmp))
    return ret;;

  for (long i = idx - 1; i >= 0; --i)
    {
      T * ptr = const_cast<T*>(&a(i));
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.insert(ptr);
    }

  ret.append(found_ptr);

  for (long i = idx + 1, n = a.size(); i < n; ++i)
    {
      T * ptr = const_cast<T*>(&a(i));
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.append(ptr);
    }

  return ret;
}

template <class T, class Compare = Aleph::less<T>> inline
DynList<T*> bsearch_dup(const DynArray<T> & a, const T & x, 
			Compare && cmp = Compare()) noexcept
{
  return bsearch_dup(a, x, cmp);
}

template <typename T, class Compare> inline
DynList<T*> bsearch_dup(const DynArray<T*> & a, const T & x, Compare & cmp)
  noexcept
{
  DynList<T*> ret;
  long idx = binary_search(a, x, cmp);
  if (idx < 0)
    return ret;

  T * found_ptr = a(idx);
  if (not are_equals(*found_ptr, x, cmp))
    return ret;;

  for (long i = idx - 1; i >= 0; --i)
    {
      T * ptr = a(i);
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.insert(ptr);
    }

  ret.append(found_ptr);

  for (long i = idx + 1, n = a.size(); i < n; ++i)
    {
      T * ptr = a(i);
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.append(ptr);
    }

  return ret;
}


    template <class T, class Compare> inline
long binindex(const DynArray<T> & a, const T & x, Compare & cmp) noexcept
{
  return binary_search(a, x, cmp);
}

    template <class T, class Compare = Aleph::less<T>> inline
long  binindex(const DynArray<T> & a, const T & x, Compare && cmp = Compare()) 
      noexcept
{
  return binindex(a, x, cmp);
}

    template <class T, class Compare> inline
long binindex(const DynArray<T*> & a, const T & x, Compare & cmp) noexcept
{
  return binary_search(a, x, cmp);
}

    template <class T, class Compare = Aleph::less<T>> inline
long binindex(const DynArray<T*> & a, const T & x, Compare && cmp = Compare()) 
      noexcept
{
  return binindex(a, x, cmp);
}

template <typename T, class Compare> inline
DynList<long> binindex_dup(const DynArray<T> & a, const T & x, Compare & cmp)
  noexcept
{
  DynList<long> ret;
  long idx = binary_search(a, x, cmp);
  if (idx < 0)
    return ret;

  T * ptr = const_cast<T*>(&a(idx));
  if (not are_equals(*ptr, x, cmp))
    return ret;;

  long mid = idx;

  for (long i = idx - 1; i >= 0; --i)
    {
      ptr = const_cast<T*>(&a(i));
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.insert(i);
    }

  ret.append(mid);

  for (long i = idx + 1, n = a.size(); i < n; ++i)
    {
      ptr = const_cast<T*>(&a(i));
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.append(i);
    }

  return ret;
}

template <class T, class Compare = Aleph::less<T>> inline
DynList<long> binindex_dup(const DynArray<T> & a, const T & x, 
			   Compare && cmp = Compare()) noexcept
{
  return binindex_dup(a, x, cmp);
}

template <typename T, class Compare> inline
DynList<long> binindex_dup(const DynArray<T*> & a, const T & x, Compare & cmp)
  noexcept
{
  DynList<long> ret;
  long idx = binary_search(a, x, cmp);
  if (idx < 0)
    return ret;

  T * ptr = a(idx);
  if (not are_equals(*ptr, x, cmp))
    return ret;;

  for (long i = idx - 1; i >= 0; --i)
    {
      ptr = a(i);
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.insert(i);
    }

  ret.append(ptr);

  for (long i = idx + 1, n = a.size(); i < n; ++i)
    {
      T * ptr = a(i);
      if (not are_equals(*ptr, x, cmp))
	break;
      ret.append(i);
    }

  return ret;
}

template <class T, class Compare = Aleph::less<T>> inline
DynList<T*> binindex_dup(const DynArray<T*> & a, const T & x, 
			Compare && cmp = Compare()) noexcept
{
  return binindex_dup(a, x, cmp);
}

    template <class T, class Compare> inline
DynArray<size_t> build_index(const DynArray<T> & a, Compare & cmp)
{
  const size_t & n = a.size();
  DynArray<size_t> ret; ret.reserve(a.size());
  for (size_t i = 0; i < n; ++i)
    ret(i) = i;
  
  quicksort_op(ret, 
	       [&a, &cmp] (size_t i, size_t j) { return cmp(a(i), a(j)); });

  return ret;
}

    template <class T, class Compare = Aleph::less<T>> inline
DynArray<size_t> build_index(const DynArray<T> & a, Compare && cmp = Compare())
{
  return build_index(a, cmp);
}


    template <class T, class Compare> inline
DynArray<T*> build_index_ptr(const DynArray<T> & a, Compare & cmp)
{
  const size_t & n = a.size();
  DynArray<T*> ret; ret.reserve(a.size());
  for (size_t i = 0; i < n; ++i)
    ret(i) = &a(i);
  
  quicksort_op(ret, [&cmp] (const T * ptr1, const T * ptr2) 
	       { return cmp(*ptr1, *ptr2); });

  return ret;
}

    template <class T, class Compare = Aleph::less<T>> inline
DynArray<T*> build_index_ptr(const DynArray<T> & a, Compare && cmp = Compare())
{
  return build_index_ptr(a, cmp);
}


    template <class T, class Compare> inline
void quicksort_op(DynArray<T> & a, Compare & cmp,
		  size_t threshold = Quicksort_Threshold) noexcept
{
  long i, l = 0, r = a.size() -1;

  FixedStack<int> stack(40);

  push2(stack, l, r);

  while (not stack.is_empty())
    {
      l = stack.pop(); r = stack.pop();

      long partition_size = r - l + 1;
      if (partition_size <= 1)  
	continue;

      if (partition_size <= threshold)
	{
	  insertion_sort<T, Compare>(a, l, r, cmp);
	  continue;
	}

      i = partition_op<T, Compare>(a, l, r, cmp);

      if (i - l > r - i)
	{
	  push2(stack, l, i - 1); push2(stack, i + 1, r);
	}
      else 
	{
	  push2(stack, i + 1, r); push2(stack, l, i - 1); 
	} 
    }
}



    template <class T, class Compare = Aleph::less<T>> inline
void quicksort_op(DynArray<T> & a, Compare && cmp = Compare(),
		  size_t threshold = Quicksort_Threshold) noexcept
{
  quicksort_op<T, Compare>(a, cmp, threshold);
}

# undef push2


    template <typename T, class Compare> inline 
int binary_search_rec(T * a, const T & x, const long l, const long r,
		      Compare & cmp) noexcept
{
  const long m = (l + r) / 2;
  if (l > r) 
    return m;

  if (cmp(x, a[m]))
    return binary_search_rec<T, Compare>(a, l, m - 1, cmp);
  else if (cmp(a[m], x))
    return binary_search_rec<T, Compare>(a, m + 1, r, cmp);

  return m; // encontrado
}

    template <typename T, class Compare = Aleph::less<T>> inline 
int binary_search_rec(T * a, const T & x, 
		      const long l, const long r,
		      Compare && cmp = Compare()) noexcept
{
  return binary_search_rec<T, Compare>(a, x, l, r, cmp);
}


    template <typename T, class Compare> inline 
long binary_search(T * a, const T & x, long l, long r, Compare & cmp) noexcept
{
  long m;
  while (l <= r)
    {
      m = (l + r) / 2;
      if (cmp(x, a[m]))
	r = m - 1;
      else if (cmp(a[m], x))
	l = m + 1;
      else 
	break;
    }
  return m; 
}

template <typename T, class Compare> inline 
long binary_search(T * a, T && x, long l, long r, Compare & cmp) noexcept
{
  return binary_search<T, Compare>(a, x, l, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline 
long binary_search(T * a, const T & x, long l, long r, 
		   Compare && cmp = Compare()) noexcept
{
  return binary_search<T, Compare>(a, x, l, r, cmp);
}

    template <typename T, class Compare = Aleph::less<T>> inline 
long binary_search(T * a, T && x, long l, long r, Compare && cmp = Compare())
      noexcept
{
  return binary_search<T, Compare>(a, x, l, r, cmp);
}


}

# endif // TPL_SORT_UTILS_H
