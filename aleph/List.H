
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


/*
	Aleph implementation of list C++ standard container
*/

# ifndef ALEPH_LIST_H
# define ALEPH_LIST_H

# include <ah_stdc++_utils.H>
# include <tpl_sort_utils.H>

namespace Aleph
{

    template <class T>
class list
{
  mutable Dnode<T> dlist;

  typedef Dnode<T> Node;

public:

  typedef T value_type;

  typedef typename list::value_type & reference;

  typedef const typename list::value_type & const_reference;

  typedef size_t size_type;

private:

  mutable size_type num_elem;

  mutable bool num_elem_is_updated;

  void reset_num_elem(const size_type & num = 0)
  {
    num_elem = num;
    num_elem_is_updated = true;
  }

  void update_num_elem()
  {
    assert(not num_elem_is_updated);

    size_type counter = 0;

    for (typename Dnode<T>::Iterator it(dlist); it.has_current(); it.next())
      ++counter;

    num_elem = counter;
    num_elem_is_updated = true;
  }

public:

  class iterator
  {
    friend class list<T>;

    typedef typename Dnode<T>::Iterator Iterator;
    
    Iterator itor;

    bool underflow;
    bool overflow;

    void init_flags()
    {
      if (itor.has_current())
	underflow = overflow = false;
      else
	underflow = overflow = true;
    }

    iterator(Dnode<T> & __list) : itor(__list)
    {
      init_flags();
    }

    void goto_begin()
    {
      itor.reset_first();
      init_flags();
    }

    void goto_last()
    {
      itor.reset_last();
      init_flags();
    }

    void goto_end()
    {
      itor.reset_last();
      init_flags();
      if (not overflow)
	itor.next();
      overflow = true;
    }

    void forward()
    {
      if (underflow)
	{
	  goto_begin();
	  return;
	}
      
      itor.next();

      if (not itor.has_current())
	overflow = true;
    }

    void backward()
    {
      if (overflow)
	{
	  goto_last();
	  return;
	}
      
      itor.prev();

      if (not itor.has_current())
	underflow = true;
    }

  public:

    typedef typename list::value_type   value_type;

    typedef int                         difference_type;

    typedef typename list::value_type * pointer;

    typedef typename list::reference    reference;

    iterator() : underflow(false), overflow(false) { /* empty */ }

    T & operator *()
    {
      return itor.get_current()->get_data();
    }

    T * operator ->()
    {
      return & itor.get_current()->get_data();
    }

    iterator operator ++()
    {
      forward();
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator return_value = *this;
      forward();
      return return_value;
    }

    iterator operator --()
    {
      backward();
      return *this;
    }

    iterator operator -- (int)
    {
      iterator return_value = *this;
      backward();
      return return_value;
    }
    
    iterator operator += (const size_type & n)
    {
      for (int i = 0; i < n; ++i)
	forward();

      return *this;
    } 

    iterator operator -= (const size_type & n)
    {
      for (int i = 0; i < n; ++i)
	backward();

      return *this;
    } 

    bool operator == (const iterator & __itor)
    {
      return itor == __itor.itor;
    }

    bool operator != (const iterator& __itor)
    {
      return itor != __itor.itor;
    }

    bool verify (const list<T> & list) const
    {
      return itor.verify((Dlink*)(&list.dlist));
    }

    bool verify(const iterator & it) const
    {
      return itor.verify(it.itor);
    }
  }; // end class iterator

private:

  void copy(const list& _list)
  {
    assert(dlist.is_empty());
    assert(num_elem == 0);

    for (typename Dnode<T>::Iterator it(_list.dlist); 
	 it.has_current(); it.next())
      {
	Node * node = new Node(it.get_current()->get_data());

	dlist.append(node);
	++num_elem;
      }
  }

public:

  list() : num_elem(0), num_elem_is_updated(true) { /* empty */ }

  list(const list & c) : num_elem(0), num_elem_is_updated(true)
  {
    copy(c);
  }

  list(const size_type & num) : num_elem(0), num_elem_is_updated(true)
  {
    for (/* nothing */; num_elem < num; ++num_elem)
      dlist.append(new Node);
   }

  list(const size_type & num, const T & value) 
    : num_elem(0), num_elem_is_updated(true)
  {
    for (/* nothing */; num_elem < num; ++num_elem)
      dlist.append(new Node(value));
  }

      template <class Itor>
  list(Itor beg, const Itor & end) 
    : num_elem(0), num_elem_is_updated(true)
  {
    Aleph::verify_iterators(beg, end);

    while (beg != end)
      {
	dlist.append(new Node(*beg++));
	++num_elem;
      }
  }

  ~list() 
  {
    clear();
  }

  size_type size() 
  {
    if (not num_elem_is_updated)
      update_num_elem();

    return num_elem;
  }

  bool empty() const
  {
    return dlist.is_empty();
  }

  bool operator == (const list & c) const
  {
    if (this == &c)
      return true;

    if (num_elem_is_updated and c.num_elem_is_updated)
      if (num_elem != c.num_elem)
	return false;

    typename Dnode<T>::Iterator it_l(*this), it_r(c.dlist);

    while (it_l.has_current() and it_r.has_current())
      {
	if (it_l.get_current()->get_data() != it_r.get_current()->get_data())
	  return false;

	it_l.next();
	it_r.next();
      }

    if (it_l.is_empty() and it_r.is_empty())
      return true;
    
    return false;
  }

  bool operator != (const list & c) const
  {
    return not (*this == c);
  }

  bool operator < (const list & c) const
  {
    if (this == &c)
      return false;

    typename Dnode<T>::Iterator it_l(*this);
    typename Dnode<T>::Iterator it_r(c);

    while (it_l.has_current() and it_r.has_current())
      {
	if (it_l.get_current()->get_data() < it_r.get_current()->get_data())
	  return true;
	else if (it_r.get_current()->get_data() < 
		 it_l.get_current()->get_data())
	  return false; // no son iguales

	it_l.next();
	it_r.next();
      }

    return it_l.is_empty();
  }

  bool operator > (const list & c) const
  {
    return c < *this;
  }

  bool operator <= (const list & c) const
  {
    return not (c > *this );
  }

  bool operator >= (const list & c) const
  {
    return not (*this < c);
  }

  list & operator = (const list & c)
  {
    if (this == &c)
      return *this;

    clear();
    copy(c);
  }

  void assign(const size_type & num, const T & value)
  {
    clear();
    
    for (int n = 0; n < num; ++n)
      push_back(value);
  }

      template <typename Itor>
  void assign (Itor beg, const Itor & end)
  {
    Aleph::verify_iterators(beg, end);

    clear();
    
    while (beg != end)
      push_back(*beg++);
  }
    
  void swap(list & c)
  {
    dlist.swap(c.dlist);
    std::swap(num_elem, c.num_elem);
    std::swap(num_elem_is_updated, c.num_elem_is_updated);
  }

  reference front() const
  {
    return dlist.get_next()->get_data();
  }

  reference back() const
  {
    return dlist.get_prev()->get_data();
  }

  iterator begin() const
  {
    return iterator(dlist);
  }
  
  iterator end() const
  {
    iterator _itor(dlist);
    _itor.goto_end();

    return _itor;
  }
 
  iterator insert(const iterator & pos, const T & value)
  {
    Aleph::verify_container_and_iterator(*this, pos);

    Node * new_node = new Node(value);

    Node * current_node = pos.itor.get_current();

    current_node->append(new_node);

    pos.itor.set(new_node);

    ++num_elem;

    return pos;
  }

  void insert(iterator pos, const size_type & num, const T & value)
  {
    Aleph::verify_container_and_iterator(*this, pos);

    Node new_list;

    try
      {
	for (int i = 0; i < num; ++i)
	  new_list.append (new Node(value));

	Node * current_node = pos.itor.get_current();
	current_node->insert_list(&new_list);
	num_elem += num;
      }
    catch (...)
      {
	new_list.remove_all_and_delete();
	throw;
      }
  }

      template <class Itor>
  void insert(iterator pos, Itor beg, const Itor & end)
  {
    Aleph::verify_container_and_iterator(*this, pos);
    Aleph::verify_iterators(beg, end);

    Node new_list;
    try
      {
	while (beg != end)
	  {
	    new_list.append(new Node(*beg++));
	    ++num_elem;
	  }

	Node * current_node = pos.itor.get_current();
	current_node->insert_list(&new_list);
      }
    catch (...)
      {
	new_list.remove_all_and_delete();
	throw;
      }
  }

  void push_front(const T & value)
  {
    dlist.insert(new Node(value));
    ++num_elem;
  }

  void push_back(const T & value)
  {
    dlist.append(new Node(value));
    ++num_elem;
  }

  void remove(const T & value)
  {
    for (typename Dnode<T>::Iterator it(dlist); it.has_current(); /* nothing */)
      if (it.get_current()->get_data() == value)
	{
	  delete it.del();
	  --num_elem;
	}
      else
	it.next();
  }

  iterator erase(iterator pos)
  {
    Aleph::verify_container_and_iterator(*this, pos);

    delete pos.itor.del();
    --num_elem;

    return pos;
  }

  iterator erase(iterator beg, const iterator & end)
  {
    Aleph::verify_container_and_iterator(*this, beg);
    Aleph::verify_iterators(beg, end);

    while (beg != end)
      {
	delete  beg.itor.del();
	--num_elem;
      }

    return beg;
  }

  void pop_front()
  {
    delete dlist.remove_next();
    num_elem--;
  }

  void pop_back()
  {
    delete dlist.remove_prev();
    --num_elem;
  }

  void clear()
  {
    dlist.remove_all_and_delete();
    reset_num_elem();
  }

  void resize(size_type num, const T & t = T())
  {
    if (num == size())
      return;
    
    if (num < num_elem)
      {
	while (num_elem > num)
	  {
	    delete dlist.remove_prev();
	    --num_elem;
	  }

	return;
      }

    iterator itor(end());
    --itor;
    insert(itor, num - num_elem, t);
  }

      template <class Op>
  void unique(const Op & op) 
  {
    reset_num_elem(); // recordar que coloca num_elem = 0
    
    for (typename Dnode<T>::Iterator it1(dlist); it1.has_current(); 
	 it1.next(), ++num_elem)
      {
	typename Dnode<T>::Iterator it2(it1); it2.next(); 

	while (it2.has_current())
	  if (op(it1.get_current()->get_data(), it2.get_current()->get_data()))
	    delete it2.del();
	  else
	    break;
      }
  }

  void unique()  
  {
    unique(Aleph::equal_to<T>());
  }

  void splice(iterator pos, list & l)
  {
    Aleph::verify_container_and_iterator(*this, pos);

    pos.itor.get_current()->insert_list(&l.dlist);

    num_elem_is_updated = l.num_elem_is_updated;
    num_elem += l.num_elem;

    l.reset_num_elem();

    assert(l.dlist.is_empty());
  }

  void splice(iterator pos, list & src_list, iterator src_pos)
  {
    Aleph::verify_container_and_iterator(*this, pos);
    Aleph::verify_container_and_iterator(src_list, src_pos);

    pos.itor.get_current()->insert(src_pos.itor.del());
    --src_list.num_elem;
    ++num_elem;
  }

  void splice(iterator pos, list & src_list, 
	      iterator src_beg, const iterator & src_end)
  {
    Aleph::verify_container_and_iterator(*this, pos);
    Aleph::verify_container_and_iterator(src_list, src_beg);
    Aleph::verify_iterators(src_beg, src_end);    

    Dlink list_to_insert;
    src_list.dlist.cut_list(src_beg.itor.get_current(), &list_to_insert);

    Dlink remaining_list;
    list_to_insert.cut_list(src_end.itor.get_current(), &remaining_list);
    
    pos.itor.get_current()->insert_list(&list_to_insert);
    num_elem_is_updated = false;

    src_list.dlist.concat_list(&remaining_list);
    src_list.num_elem_is_updated = false;
  }

      template <class Cmp>
  void sort(const Cmp &)
  {
    quicksort<T, Cmp>(dlist);	
  }

  void sort()
  {
    sort(Aleph::less<T>()); 
  }

      template <class Cmp>
  void merge(list & l, const Cmp &)
  {
    Dnode<T> result;

    Aleph::merge_lists<T, Cmp>(dlist, l.dlist, result);

    dlist.swap(result);

    num_elem += l.num_elem;

    l.reset_num_elem();

    assert(l.dlist.is_empty());
  }

  void merge(list & l)
  {
    merge(l, Aleph::less<T>());
  }

  void reverse()
  {
    reset_num_elem(dlist.reverse_list());
  }
};


    template <typename T>
typename list<T>::size_type distance(typename list<T>::iterator it1, 
				     typename list<T>::iterator it2)
{
  typename list<T>::size_type counter = 0;

  while (it1 != it2)
    {
      counter++;
      it1++;
    }

  return counter;
}

} // end namespace Aleph

# endif // ALEPH_LIST_H
