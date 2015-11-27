
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


# ifndef DLINK_H
# define DLINK_H

# include <aleph.H>

using namespace Aleph;

namespace Aleph {

class Dlink 
{
protected: 

  mutable Dlink * prev; 
  mutable Dlink * next;

public:

  Dlink() noexcept : prev(this), next(this) { assert(is_empty()); }

  Dlink(const Dlink & l) noexcept : prev(l.prev), next(l.next) { /* empty */ }

  void swap(Dlink * link) noexcept
  {
    if (is_empty() and link->is_empty()) 
      return;

    if (is_empty())
      {
        link->next->prev = this;
        link->prev->next = this;
        next = link->next;
        prev = link->prev;
        link->reset();
        return;
      }

    if (link->is_empty())
      {
        next->prev = link;
        prev->next = link;
        link->next = next;
        link->prev = prev;
        reset();
        return;
      }

    std::swap(prev->next, link->prev->next);
    std::swap(next->prev, link->next->prev);
    std::swap(prev, link->prev);
    std::swap(next, link->next);
  }

  void swap(Dlink & l) noexcept
  {
    swap(&l);
  }   

  Dlink(Dlink && l) noexcept : Dlink()
  {
    swap(l);
  }

  Dlink & operator = (const Dlink & l) noexcept
  {
    assert(is_empty());
    prev = l.prev;
    next = l.next;
    return *this;
  }   

  Dlink & operator = (Dlink && l) noexcept
  {
    swap(l);
    return *this;
  }

  void reset() noexcept 
  {
    next = prev = this; 
  }

  void init() noexcept
  {
    reset();
  }

  bool is_empty() const noexcept { return this == next and this == prev; }

  bool is_unitarian() const noexcept { return this != next and next == prev; }

  bool is_unitarian_or_empty() const noexcept { return next == prev; }

  void insert(Dlink * node) noexcept
  {
    assert(next != nullptr and prev != nullptr);
    assert(node != nullptr); 
    assert(node->is_empty());

    node->prev = this;
    node->next = next;
    next->prev = node;
    next       = node;
  }

  void push(Dlink * node) noexcept
  {
    insert(node);
  }

  void append(Dlink * node) noexcept
  {
    assert(next != nullptr and prev != nullptr);
    assert(node != nullptr);
    assert(node->is_empty());

    node->next = this;
    node->prev = prev;
    prev->next = node;
    prev       = node;
  }

  Dlink *& get_next() const noexcept
  {
    assert(next != nullptr and prev != nullptr);
    return next; 
  }

  Dlink * top() noexcept { return get_next(); }

  Dlink *& get_prev() const noexcept
  {
    assert(next != nullptr and prev != nullptr);
    return prev; 
  }

  Dlink *& get_first() const noexcept { return next; }

  Dlink *& get_last() const noexcept { return prev; } 

  void wrap_header(Dlink * l) noexcept
  {
    assert(is_empty());
    l->append(this);
  }

  void insert_list(Dlink * head) noexcept
  {
    if (head->is_empty()) 
      return;

    head->prev->next = next;
    head->next->prev = this;
    next->prev       = head->prev;
    next             = head->next;
    head->reset();
  }

  void append_list(Dlink * head) noexcept
  {
    if (head->is_empty()) 
      return;

    head->next->prev = prev;
    head->prev->next = this;
    prev->next       = head->next;
    prev             = head->prev;
    head->reset();
  }

  void splice(Dlink * l) noexcept
  {
    Dlink head;
    head.wrap_header(l);
    insert_list(&head);
    assert(head.is_empty());
  }

  void concat_list(Dlink * head) noexcept
  { 
    assert(head != nullptr);

    if (head->is_empty()) 
      return;

    if (this->is_empty())
      {
        swap(head);
        return;
      }

    prev->next       = head->next;
    head->next->prev = prev;
    prev             = head->prev;
    head->prev->next = this;
    head->reset();
  }

  void concat_list(Dlink & head) noexcept
  { 
    concat_list(&head);
  }

  void del() noexcept 
  {
    assert(next != nullptr and prev != nullptr);

    prev->next = next;
    next->prev = prev;
    reset();
  }

  void erase() noexcept { del(); }

  Dlink * remove_prev() noexcept
  {
    assert(not is_empty());

    Dlink* retValue = prev;
    retValue->del();

    return retValue;
  }

  Dlink * remove_next() noexcept 
  {
    assert(not is_empty());

    Dlink* retValue = next;
    retValue->del();

    return retValue;
  }

  Dlink * remove_last() noexcept 
  {
    return remove_prev();
  }

  Dlink * remove_first() noexcept 
  {
    return remove_next();
  }

  Dlink * pop() noexcept 
  {
    return remove_next();
  }

  size_t reverse_list() noexcept
  {
    if (is_empty()) 
      return 0;

    Dlink tmp; 
    size_t counter = 0; 
    for (; not is_empty(); counter++)
      tmp.insert(remove_next()); 

    swap(&tmp);

    return counter;
  }

  size_t split_list(Dlink & l, Dlink & r) noexcept
  {
    assert(l.is_empty() and r.is_empty()); // l y r deben estar vacías

    size_t count = 0;
    while (not is_empty())
      {
        l.append(remove_next()); ++count;

        if (is_empty())
          break;

        r.insert(remove_prev()); ++count;
      }

    return count;
  }

  Dlink cut_list(Dlink * link) noexcept 
  {
    assert(not is_empty() and not link->is_empty() and link != this);

    Dlink list;
    if (link == this->prev) // is link the last item?
      {
	link->del();
	list.append(link);
	return list;
      }

    if (link == this->next) // is link the first item?
      {
	list.swap(this);
	assert(this->is_empty());
	return list;
      }

    list.prev = this->prev;       // enlazar list a link (punto de corte)
    list.next = link; 
    this->prev = link->prev;       // quitar de this todo a partir de link
    link->prev->next = this;
    link->prev = &list;       // colocar el corte en list
    list.prev->next = &list; 
    
    return list;
  }

  class Iterator 
  {
  private:

    mutable Dlink * head = nullptr;
    mutable Dlink * curr = nullptr;

  public:

    using Set_Type = Dlink;

    using Item_Type = Dlink *;

    Iterator(Dlink * head_ptr) noexcept 
    : head(head_ptr), curr(head->get_next()) { /* */ }

    Iterator(const Dlink & list) noexcept
      : head(&const_cast<Dlink&>(list)), curr(head->get_next())
    {
      // Empty
    }

    void set(Dlink * new_curr) noexcept
    {
      assert(curr != nullptr and head != nullptr);
      curr = new_curr;
    }
       
    Iterator() noexcept : head(nullptr), curr(nullptr) { /* Empty */ }

    void reset_first() noexcept 
    {
      assert(curr != nullptr and head != nullptr);
      curr = head->get_next();
    }

    void reset_last() noexcept
    {
      assert(curr != nullptr and head != nullptr);
      curr = head->get_prev();
    }
    
    void end() noexcept
    {
      reset_last();
      next(); 
    }

    bool has_current() const noexcept
    {
      assert(curr != nullptr and head != nullptr);
      return curr != head;
    }

    bool has_curr() const noexcept { return has_current(); }

    Dlink * get_current() const
    {
      assert(curr != nullptr and head != nullptr);

      if (not has_current())
        throw std::overflow_error("Not element in list");

      return curr;
    }

    Dlink * get_curr() const { return get_current(); }

    bool is_in_first() const noexcept { return curr == head->next; }

    bool is_in_last() const noexcept { return curr == head->prev; }

    void prev() throw(std::exception, std::underflow_error)
    {
      if (not has_current())
        throw std::underflow_error("Not previous element in list");

      curr = curr->get_prev();
    }

    void next() throw(std::exception, std::overflow_error)
    {
      if (not has_current())
        throw std::overflow_error("Not next element in list");

      curr = curr->get_next();
    }

    bool operator == (const Iterator & it) const noexcept 
    { return curr == it.curr; }

    bool operator != (const Iterator & it) const noexcept 
    { return curr != it.curr; }
    
    Dlink * del() 
    {
      assert(curr != nullptr and head != nullptr);
      assert(has_current());

      Dlink * current = get_current(); // obtener nodo actual
      next(); // avanzar al siguiente nodo
      current->del(); // eliminar de la lista antiguo nodo actual 
      return current;
    } 

    bool verify(Dlink * l) const { return head == l; }

    bool verify(const Iterator & it) const { return head == it.head; }
  };

  void remove_all_and_delete() noexcept
  {
    while (not is_empty())
      delete remove_next();
  }
 
  void rotate_left(size_t n)
  {
    if (is_empty())
      throw std::domain_error("List is empty");

    for (auto i = 0; i < n; ++i)
      append(remove_first());
  }

  void rotate_right(size_t n)
  {
    if (is_empty())
      throw std::domain_error("List is empty");

    for (auto i = 0; i < n; ++i)
      insert(remove_last());
  }

  bool check()
  {
    Iterator itor(this);
    Dlink* node;

    for (/* nothing */; itor.has_current(); itor.next())
      {
        node = itor.get_current();

        if (not (node->get_next()->get_prev() == node))
          return false;

        if (not (node->get_prev()->get_next() == node))
          return false;
      }

    for (itor.reset_last(); itor.has_current(); itor.prev())
      {
        node = itor.get_current();

        if (not (node->get_next()->get_prev() == node))
          return false;

        if (not (node->get_prev()->get_next() == node))
          return false;
      }

    return true;
  }
};

# define DLINK_TO_TYPE(type_name, link_name)			      \
  inline static type_name * dlink_to_##type_name(Dlink * link) noexcept \
  {									\
    type_name * ptr_zero = 0;						\
    size_t offset_link = reinterpret_cast<size_t>(&(ptr_zero->link_name)); \
    char * address_type = reinterpret_cast<char*>(link) - offset_link;	\
    return reinterpret_cast<type_name *>(address_type);			\
  } 

# define LINKNAME_TO_TYPE(type_name, link_name)				\
  inline static type_name * link_name##_to_##type_name(Dlink * link) noexcept \
  {									\
    type_name * ptr_zero = 0;						\
    size_t offset_link = reinterpret_cast<size_t>(&(ptr_zero->link_name)); \
    char * address_type = reinterpret_cast<char*>(link) - offset_link;	\
    return reinterpret_cast<type_name *>(address_type);			\
  } 


# define DLINK_TO_BASE(type_name, link_name)				\
  inline static type_name * dlink_to_base(Dlink * link)	noexcept	\
  {									\
    type_name * ptr_zero = 0;						\
    size_t offset_link = reinterpret_cast<size_t>(&(ptr_zero->link_name)); \
    char * address_type = reinterpret_cast<char*>(link) - offset_link;	\
    return reinterpret_cast<type_name *>(address_type);			\
  } 

}

# endif /* DLINK_H */ 

