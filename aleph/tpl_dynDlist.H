
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

# ifndef TPL_DYNDLIST_H
# define TPL_DYNDLIST_H

# include <ahFunctional.H>
# include <ahDry.H>
# include <ahIterator.H>
# include <ah-iterator.H>
# include <tpl_dnode.H>

using namespace Aleph;

namespace Aleph {

# include <ah-dry.H>

    template <typename T = int> 
class DynDlist 
      : public Dnode<T>,
	public GenericTraverse<DynDlist<T>>,
	public SpecialCtors<DynDlist<T>, T>,
	public LocateFunctions<DynDlist<T>, T>,
	public FunctionalMethods<DynDlist<T>, T>,
	public GenericItems<DynDlist<T>, T>,
	public StlAlephIterator<DynDlist<T>>
{
  size_t num_elem;

public:

  using Set_Type = DynDlist;

  using Item_Type = T;

  using Key_Type = T;

  const size_t & size() const noexcept { return num_elem; }

  DynDlist() noexcept(std::is_nothrow_constructible<T>::value) 
    : Dnode<T>(), num_elem(0) { /* Empty */ }

  DynDlist(const DynList<T> & l) : Dnode<T>(), CtorBase(l) {}

  using CtorBase = SpecialCtors<DynDlist<T>, T>;
  using CtorBase::CtorBase;
  
  void empty() noexcept 
  {
    while (not this->is_empty())
      delete this->remove_next();

    num_elem = 0;
  }

  ~DynDlist() 
  { 
    empty();
  }

private:
  
  T & __insert(Dnode<T> * p) noexcept
  {
    Dnode<T>::insert(p);
    ++num_elem;
    return p->get_data();
  }

  T & __append(Dnode<T> * p) noexcept
  {
    Dnode<T>::append(p);
    ++num_elem;
    return p->get_data();
  }

public:

  T & insert(const T & item) throw(std::exception, std::bad_alloc)
  {
    return __insert(new Dnode<T> (item));
  }

  T & insert(T && item) 
  {
    return __insert(new Dnode<T> (std::forward<T>(item)));
  }

  T & append(const T & item) throw(std::exception, std::bad_alloc)
  {
    return __append(new Dnode<T> (item));
  }

  T & append(T && item) throw(std::exception, std::bad_alloc)
  {
    return __append(new Dnode<T> (std::forward<T>(item)));
  }

  void insert_list(DynDlist & list) noexcept
  {
    Dlink::insert_list(&list);
    num_elem += list.num_elem;
    list.num_elem = 0;

    assert(list.is_empty());
  }

  void insert_list(DynDlist && list) noexcept 
  {
    insert_list(list);
  }

  void append_list(DynDlist & list) noexcept
  {
    Dlink::append_list(&list);
    num_elem += list.num_elem;
    list.num_elem = 0;

    assert(list.is_empty());
  }

  void append_list(DynDlist && list) noexcept
  {
    append_list(list);
  }

  T & get_first() const throw(std::exception, std::underflow_error)
  {
    return this->get_next()->get_data();
  }

  T & get_last() const throw(std::exception, std::underflow_error)
  {
    return this->get_prev()->get_data();
  }

  T remove_first() throw(std::exception, std::underflow_error)
  {
    Dnode<T> * ptr = this->remove_next();
    T retVal = ptr->get_data();
    delete ptr;
    --num_elem;

    return retVal;
  }

  T remove_last() throw(std::exception, std::underflow_error)
  {
    Dnode<T> * ptr = this->remove_prev();
    T retVal = ptr->get_data();
    delete ptr;
    --num_elem;

    return retVal;
  }

  T & put(const T & item) { return append(item); }

  T & put(T && item) { return append(std::forward<T>(item)); }

  T get() { return remove_first(); }

  T & rear() { return get_last(); }

  T & front() { return get_first(); }

  T & push(const T & item) { return insert(item); }

  T & push(T && item) { return insert(std::forward<T>(item)); }

  T pop() { return remove_first(); }

  T & top() const { return get_first(); }
  
  void remove(T & data) noexcept
  {
    Dnode<T> * p = Dnode<T>::data_to_node(data);
    p->del();
    delete p;
    --num_elem;
  }

  void erase(T & data) noexcept
  {
    remove(data);
  }

  void swap(DynDlist & l) noexcept
  {
    std::swap(num_elem, l.num_elem);
    this->Dlink::swap(&l);
  }

  void split_list(DynDlist & l, DynDlist & r) 
    throw(std::exception, std::domain_error)
  {
    if ((not l.is_empty()) or (not r.is_empty()))
      throw std::domain_error("lists are not empty");

    Dlink::split_list(l, r);
    l.num_elem = r.num_elem = num_elem/2;

    if (num_elem % 2 == 1) // ¿es num_elem impar?
      l.num_elem++;

    num_elem = 0;
  }

  class Iterator : public Dnode<T>::Iterator
  {
    DynDlist * list_ptr; // puntero a la lista
    long       pos;      // posición del elemento actual en la secuencia

    using Base = typename Dnode<T>::Iterator;

  public:

    using Set_Type = DynDlist;

    using Item_Type = T;

    long get_pos() const noexcept { return pos; }

    void next()
    {
      Dnode<T>::Iterator::next();
      pos++;
    }

    void prev()
    {
      Dnode<T>::Iterator::prev();
      pos--;
    }

    void reset_first() noexcept
    {
      Dnode<T>::Iterator::reset_first();
      pos = 0;
    }

    void reset_last() noexcept
    {
      Dnode<T>::Iterator::reset_last();
      pos = list_ptr->num_elem - 1;
    }

    void end() noexcept
    {
      reset_last();
      next();
    }

    Iterator(const DynDlist<T> & list) noexcept
    : Base(list), list_ptr(&const_cast<DynDlist&>(list)), pos(0) 
    {
      // empty
    }

    Iterator() noexcept : list_ptr(nullptr) { /* empty */ }

    Iterator & operator = (const Iterator & it) noexcept
    { 
      Dnode<T>::Iterator::operator = (it);
      list_ptr = it.list_ptr;
      pos      = it.pos;

      return *this;
    }

    T & get_current() const
    {
      return Dnode<T>::Iterator::get_current()->get_data(); 
    }

    T & get_curr() const
    {
      return get_current(); 
    }

    void insert(const T & item) 
      throw(std::exception, std::overflow_error, std::bad_alloc)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->insert(new Dnode<T>(item));
      ++list_ptr->num_elem;
    }

    void insert(T && item) 
      throw(std::exception, std::overflow_error, std::bad_alloc)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->
	insert(new Dnode<T>(std::forward<T>(item)));
      ++list_ptr->num_elem;
    }

    void append(const T & item) 
      throw(std::exception, std::overflow_error, std::bad_alloc)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->append(new Dnode<T>(item));
      ++list_ptr->num_elem;
    }

    void append(T && item) 
      throw(std::exception, std::overflow_error, std::bad_alloc)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->
	append(new Dnode<T>(std::forward<T>(item)));
      ++list_ptr->num_elem;
    }

    void insert_list(DynDlist & list) 
      throw(std::exception, std::overflow_error)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->insert_list(&list);
      list_ptr->num_elem += list.num_elem;
      list.num_elem = 0;

      assert(list.is_empty());
    }

    void append_list(DynDlist & list) 
      throw(std::exception, std::overflow_error)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T>::Iterator::get_current()->append_list(&list);
      list_ptr->num_elem += list.num_elem;
      list.num_elem = 0;

      assert(list.is_empty());
    }

    T del() throw(std::exception, std::overflow_error)
    {
      if (not this->has_current())
        throw std::overflow_error ("DynDlist Iterator has not current");

      Dnode<T> * ptr = Dnode<T>::Iterator::get_current(); 
      T ret_val      = ptr->get_data(); 
      Dnode<T>::Iterator::next();
      ptr->del();
      delete ptr;
      --list_ptr->num_elem;
      return ret_val;
    }
  };

  DynDlist<T> & operator = (const DynDlist & list)
  {
    if (this == &list) 
      return *this;

    empty();

    for (typename DynDlist<T>::Iterator itor(const_cast<DynDlist&>(list)); 
	 itor.has_current();itor.next())
      this->append(itor.get_current()); 

    return *this;
  }

  DynDlist(const DynDlist & list) : DynDlist()
  {
    assert(this->is_empty());

    for (typename DynDlist<T>::Iterator itor(const_cast<DynDlist&>(list)); 
	 itor.has_current();itor.next())
      this->append(itor.get_current()); 
  }

  DynDlist(DynDlist<T> && list) noexcept
    : DynDlist()
  {
    swap(list);
  }

  DynDlist<T> & operator = (DynDlist && list) noexcept
  {
    swap(list);
    return *this;
  }
 
  T & operator [] (const size_t & n) const
  {
    typename DynDlist<T>::Iterator it(*this);

    for (int i = 0; i < n and it.has_current(); i++, it.next()) ;

    return it.get_current();
  }
};

} // end namespace Aleph

# endif /* TPL_DYNDLIST_H */

