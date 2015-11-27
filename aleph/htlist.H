
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

# ifndef HTLIST_H
# define HTLIST_H

# include <type_traits>
# include <cassert>
# include <stdexcept>
# include <utility>
# include <ahFunction.H>
# include <ahFunctional.H>
# include <ah-iterator.H>

# define NEXT(p) (p->get_next())

using namespace Aleph;

namespace Aleph {

class Slinknc
{
  Slinknc * next = nullptr;

public:

  Slinknc() noexcept : next(nullptr) { /* empty */ }

  Slinknc(const Slinknc &) noexcept : next(nullptr) { /* empty */ }

  void reset() noexcept
  {
    next = nullptr;
  }
  
  Slinknc & operator = (const Slinknc & ) noexcept
  {
    next = nullptr; 
    return *this;
  }

  Slinknc *& get_next() noexcept
  {
    return next;
  }

  void insert(Slinknc * p) noexcept
  {
    assert(p != nullptr);
    p->next = next;
    next = p;
  }

  Slinknc * remove_next() noexcept
  {
    Slinknc * ret_val = next;
    next = ret_val->next;
    ret_val->reset();
    return ret_val;
  }


  class Iterator
  {
    Slinknc * head;
    Slinknc * curr;

  public:

    Iterator() noexcept : head(nullptr), curr(nullptr)
    {
      // Empty
    }

    Iterator(Slinknc & list) noexcept : head(&list), curr(head)
    {
      // Empty
    }

    Iterator(Slinknc * _head, Slinknc * _curr) noexcept
      : head(_head), curr(_curr)
    {
      // Empty
    }

    bool has_current() const noexcept
    {
      return curr != nullptr;
    }

    bool has_curr() const noexcept
    {
      return has_current();
    }

    Slinknc * get_current() const
    {
      if (not has_current())
        throw std::overflow_error("Iterator is at the end of the list");
      return curr;
    }

    Slinknc * get_curr() const 
    {
      return get_current();
    }

    void next()
    {
      if (not has_current())
        throw std::overflow_error("Iterator is at the end of the list");
      curr = curr->next;
    }

    void reset_first() noexcept
    {
      curr = head;
    }
  };	
};


    template <typename T>
class Snodenc : public Slinknc
{
  T data;

public:

  T & get_data() noexcept { return data; }

  const T & get_data() const noexcept { return data; }

  Snodenc() noexcept(std::is_nothrow_constructible<T>::value) 
  { 
    static_assert(std::is_default_constructible<T>::value, 
		  "No default constructor for T");
  }

  Snodenc(const T & item) noexcept(noexcept(T(item))) : data(item) 
  {
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
  }

  Snodenc(T && item) noexcept(noexcept(std::swap(data, item)))
  : data(std::forward<T>(item))
  {
    static_assert(std::is_move_constructible<T>::value,
		  "No move constructor for T");
  }

  Snodenc * remove_next() noexcept { return (Snodenc*) Slinknc::remove_next(); }

  Snodenc *& get_next() noexcept { return (Snodenc*&) Slinknc::get_next(); }

  Snodenc * remove_first() noexcept{ return Snodenc::remove_next(); }

  Snodenc *& get_first() const noexcept { return Snodenc::get_next(); }
};


class HTList
{
  Slinknc * head;
  Slinknc * tail;

public:

  HTList() noexcept : head(nullptr), tail(nullptr) { /* empty */ }

  bool is_empty() const noexcept { return head == nullptr; }

  bool is_unitarian() const noexcept
  {
    return head != nullptr and head == tail; 
  }

  bool is_unitarian_or_empty() const noexcept { return head == tail; }

  Slinknc * get_head() const noexcept { return head; }

  Slinknc * get_tail() const noexcept { return tail; }

  Slinknc * get_first() const noexcept { return get_head(); }

  Slinknc * get_last() const noexcept { return get_tail(); }

  HTList & swap(HTList & l) noexcept
  {
    std::swap(head, l.head);
    std::swap(tail, l.tail);
    return *this;
  }

  void insert(Slinknc * link) noexcept
  {
    assert(NEXT(link) == nullptr);

    if (head == nullptr)
      {
	assert(tail == nullptr);
	head = tail = link;
	return;
      }
    
    NEXT(link) = head;
    head = link;
  }

  void push(Slinknc * link) noexcept { insert(link); }

  void append(Slinknc * link) noexcept
  {
    assert(link != nullptr and NEXT(link) == nullptr);

    if (head == nullptr)
      {
	assert(tail == nullptr);
	head = tail = link;
	return;
      }

    NEXT(tail) = link;
    tail = link;
  }

  void append(HTList & l) noexcept
  {
    if (l.is_empty())
      return;

    if (this->is_empty())
      {
	this->swap(l);
	return;
      }
    
    NEXT(tail) = l.head;
    tail = l.tail;
    l.head = l.tail = nullptr;
  }

  void put(Slinknc * link) noexcept { append(link); }

  void concat(HTList & l) noexcept { append(l); }

  void concat_list(HTList & l) noexcept { append(l); }

  void insert(HTList & l) noexcept
  {
    l.append(*this);
    this->swap(l);
  }

  void insert(Slinknc * link, HTList & list) noexcept
  {
    NEXT(link) = list.head;
    tail = list.tail;
    list.head = list.tail = nullptr;
  }

  Slinknc * remove_head() 
  {
    if (is_empty())
      throw std::underflow_error("HTList is empty");

    Slinknc * ret_val = head;
    if (head == tail)
      head = tail = nullptr;
    else
      {
	head = NEXT(head);
	if (head == nullptr)
	  tail = nullptr;
      }

    ret_val->reset();

    return ret_val;
  }
  
  Slinknc * remove_first() noexcept { return remove_head(); }

  bool remove(Slinknc * link) noexcept
  {
    if (is_empty())
      throw std::underflow_error("Removing from a empty list");

    if (link == head)
      {
	head = NEXT(head);
	link->reset();
	return true;
      }

    for (Slinknc * prev = head, * p = NEXT(head); p != nullptr; 
	 prev = p, p = NEXT(p))
      if (p == link)
	{
	  NEXT(prev) = NEXT(p);
	  if (link == tail)
	    tail = prev;
	  link->reset();
	  return true;	
	}

    return false;
  }

  Slinknc * pop() noexcept { return remove_head(); }

  size_t split_list(HTList & l, HTList & r) noexcept
  {
    assert(l.is_empty() and r.is_empty()); // l y r deben estar vacías

    if (is_empty())
      return 0;

    if (is_unitarian())
      {
	swap(l);
	return 1;
      }

    size_t count = 0;
    Slinknc * p = head;
    Slinknc * q = head;
    while (q != tail and q != nullptr)
      {
	q = NEXT(q); ++count;
	if (q == tail or q == nullptr)
	  break;

	p = NEXT(p); 
	q = NEXT(q); ++count;
      }

    l.head = head;
    l.tail = p;

    r.head = NEXT(p);
    r.tail = tail;

    head = tail = nullptr;

    return count;
  }

  size_t split(HTList & l, HTList & r) noexcept
  {
    return split_list(l, r);
  }

  size_t reverse() noexcept
  {
    HTList tmp;

    size_t count = 0;
    while (not is_empty())
      {
	tmp.insert(remove_first());
	++count;
      }

    swap(tmp);

    return count;
  }

  size_t reverse_list() noexcept
  {
    return reverse();
  }
  
  void cut(Slinknc * link, HTList & list) noexcept
  {
    assert(list.is_empty());

    list.head = NEXT(link);
    list.tail = tail;

    tail = link;
    NEXT(link) = nullptr;
  }

  void cut_list(Slinknc * link, HTList & list) noexcept { cut(link, list); }

  void remove_all_and_delete() noexcept
  {
    while (not is_empty())
      delete remove_head();
  }

  class Iterator
  {
    HTList * lptr = nullptr;
    Slinknc * curr = nullptr;
    Slinknc * prev = nullptr;
    
    long pos = 0;
 
  public:

    Iterator(const HTList & list) noexcept
      : lptr(& (HTList&) list), curr(lptr->head), prev(curr)
    { /* empty */ }

    void reset() noexcept
    {
      prev = curr = lptr->head;
      pos = 0;
    }

    long get_pos() const noexcept { return pos; }

    void reset_first() noexcept { reset(); }

    void reset_last() 
    {
      throw std::domain_error("reset_last() not implemented for DynList");
    }

    void end() noexcept
    {
      curr = nullptr;
    }

    Iterator & operator = (const Iterator & it) noexcept
    {
      lptr = it.lptr;
      curr = it.curr;
      prev = it.prev;
      return *this;
    }

    bool has_curr() const noexcept { return curr != nullptr; }

    bool has_current() const noexcept { return has_curr(); }
  
    Slinknc * get_curr() const
    {
      if (curr == nullptr)
	throw std::overflow_error("Iterator overflow");

      return curr;
    }

    Slinknc * get_current() const { return get_curr(); }

    void next() 
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      if (curr == lptr->head)
	{
	  assert(prev == lptr->head);
	  curr = NEXT(curr);
	}
      else if (curr == lptr->tail)
	{
	  assert(NEXT(prev) == curr);
	  curr = nullptr;
	}
      else
	{
	  assert(NEXT(prev) == curr);
	  prev = curr;
	  curr = NEXT(curr);
	  assert(NEXT(prev) == curr);
	}
      pos++;
    }

    Slinknc * del()
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      Slinknc * ret_val = nullptr;
      if (curr == lptr->head) // first item removal
	{
	  ret_val = lptr->remove_first();
	  prev = curr = lptr->head;
	}
      else if (curr == lptr->tail) // last item removal
	{
	  assert(NEXT(prev) == curr);
	  ret_val = curr;
	  NEXT(prev) = NEXT(curr);
	  lptr->tail = prev;
	  curr = nullptr; // put in overflow
	}
      else
	{
	  assert(NEXT(prev) == curr);
	  ret_val = curr;
	  NEXT(prev) = NEXT(curr);
	  curr = NEXT(curr);
	}
      
      ret_val->reset();
      return ret_val;
    }
  };

  size_t size() const noexcept
  {
    size_t count = 0;
    for (Iterator it(*this); it.has_curr(); it.next())
      ++count;

    return count;
  }

  void rotate_left(size_t n)
  {
    if (is_empty())
      throw std::domain_error("List is empty");

    for (auto i = 0; i < n; ++i)
      append(remove_first());
  }
};

# include <ah-dry.H>

    template <typename T = int>
class DynList : public HTList, 
		public GenericTraverse<DynList<T>>,
		public LocateFunctions<DynList<T>, T>,
		public SpecialCtors<DynList<T>, T>,
		public FunctionalMethods<DynList<T>, T>,
		public GenericItems<DynList<T>, T>,
		public StlAlephIterator<DynList<T>>
{
 private:

  using CtorBase = SpecialCtors<DynList<T>, T>;
  
  using CtorBase::CtorBase;

 public:

  using Item_Type = T;

  using Key_Type = T;

  DynList & swap(DynList & l) noexcept
  {
    return (DynList&) HTList::swap(l); 
  }

  DynList() noexcept(std::is_nothrow_constructible<T>::value) { /* empty */ }

  DynList(const DynList & l) : HTList(), CtorBase(l) {}

private:

  T & __insert(Snodenc<T> * p) noexcept
  {
    static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    HTList::insert(p);
    return p->get_data();
  }

  T & __append(Snodenc<T> * p) noexcept
  {
    static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    HTList::append(p);
    return p->get_data();
  }

public:

  T & insert(const T & item) 
  {
    return __insert(new Snodenc<T> (item));
  }

  T & insert(T && item) 
  {
    return __insert(new Snodenc<T> (std::forward<T>(item)));
  }

  T & push(const T & item) 
  {
    return insert(item);
  }

  T & push(T && item) { return insert(std::forward<T>(item)); }

  T & put(const T & item) noexcept(noexcept(T(item)))
  {
    return push(item); 
  }

  T & put(T && item) noexcept(noexcept(std::forward<T>(item)))
  {
    return push(std::forward<T>(item)); 
  }

  T & append(const T & item)
  {
    return __append(new Snodenc<T> (item));
  }

  T & append(T && item) 
  {
    return __append(new Snodenc<T> (std::forward<T>(item)));
  } 

  DynList(const T & item)
  {
    insert(item);
  }

  DynList(T && item) 
  {
    insert(std::forward<T>(item));
  }

  T remove()
  {
    Slinknc * l = this->HTList::remove_head();
    Snodenc<T> * p = static_cast<Snodenc<T>*> (l);
    T ret_val = std::move(p->get_data());
    delete p;

    return ret_val;
  }
  
  T remove_first() 
  {
    return remove(); 
  }
  
  T pop() { return remove_first(); }

  T & get() 
  {
    Snodenc<T> * p = static_cast<Snodenc<T>*> (this->HTList::remove_first());
    return p->get_data();
  }

  T & get_last() const
  {
    if (is_empty())
      throw std::underflow_error("List is empty");

    Snodenc<T> * p = static_cast<Snodenc<T>*> (this->HTList::get_last());
    return p->get_data();
  }

  T & get_first() const
  {
    if (is_empty())
      throw std::underflow_error("List is empty");
    return static_cast<Snodenc<T>*>(this->HTList::get_first())->get_data();
  }

  T & top() const 
  {
    return get_first();
  }

  void empty() noexcept
  {
    while (not is_empty())
      remove();

    assert(this->is_empty());
  }

  ~DynList() { empty(); }
  
  class Iterator : public HTList::Iterator
  {
  public:

    using Item_Type = T; 

    using Iterator_Type = Iterator;

    using Set_Type = DynList; 

    Iterator() noexcept { /* empty */ }

    Iterator(const DynList & list) noexcept
    : HTList::Iterator(list) 
    { /* empty */ }

    T & get_curr() const 
    {
      return ((Snodenc<T>*) (HTList::Iterator::get_curr()))->get_data();
    }

    T & get_current() const 
    {
      return get_curr();
    }

    T del() 
    {
      Snodenc<T> * p = (Snodenc<T> *) this->HTList::Iterator::del();
      T ret_val = std::move(p->get_data());
      delete p;
      return ret_val;
    }
  };

  template <class Equal = std::equal_to<T>>
    bool remove(const T & item, Equal & eq) noexcept
    {
      for (Iterator it(*this); it.has_curr(); it.next())
	if (eq(it.get_curr(), item))
	  {
	    it.del();
	    return true;
	  }

      return false;
    }

  template <class Equal = std::equal_to<T>>
    bool remove(const T & item, Equal && eq = Equal()) noexcept
    {
      return remove(item, eq);
    }

  DynList(DynList && l) noexcept
  {
    swap(l);
  }

  DynList & operator = (const DynList & l) 
  {
    if (&l == this)
      return *this;

    empty();

    for (typename DynList::Iterator it(l); it.has_curr(); it.next())
      append(it.get_curr());
    
    return *this;
  }

  DynList & operator = (DynList && l) noexcept
  {
    return (DynList&) this->swap(l);
  }

  void append(DynList && list) noexcept
  {
    HTList::append(list);
  }

  void insert(DynList && list) noexcept
  {
    HTList::insert(list);
  }

  void append(const DynList & list) noexcept(noexcept(DynList(list)))
  {
    if (&list == this)
      return;

    DynList copy = list;
    HTList::append(copy);
  }

  void insert(const DynList & list) noexcept(noexcept(DynList(list)))
  {
    if (&list == this)
      return;

    DynList tmp = list;
    HTList::insert(tmp);
  }

  T & get(const size_t & i)
  {
    Iterator it(*this);

    for (size_t __i = 0 ; it.has_current() and __i < i; it.next(), ++__i);

    return it.get_current();
  }

  T & operator [] (const size_t & i)
  {
    return get(i);
  }
};


# undef NEXT

} // end namespace Aleph


# endif // HTLIST_H
