
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

# ifndef TPL_DYN_SLIST_NC_H
# define TPL_DYN_SLIST_NC_H

# include <tpl_snode_nc.H>

namespace Aleph
{
  template <typename T> class Dyn_Slist_Nc : public Snode_Nc <T>
  {
  public:
    typedef Snode_Nc <T> Node;

  private:
    Node * head;
    size_t num_items;

  public:

    void empty()
    {
      while (not this->is_empty())
        delete Node::remove_next();
      this->reset();
      head = this;
      num_items = 0;
    }

    T & insert(const T & data) throw(std::exception, std::bad_alloc)
    {
      Node * p = new Node(data);
      Node::insert(p);

      if (num_items++ == 0)
        head = p;

      return p->get_data();
    }

    T & append(const T & data) throw(std::exception, std::bad_alloc)
    {
      assert(head not_eq nullptr);
      Node * p = new Node(data);
      head->insert(p);

      head = p;
      ++num_items;
      return p->get_data();
    }

    T & get_first()
    {
      if (this->is_empty())
        throw std::underflow_error("List is empty");

      return this->get_next()->get_data();
    }

    T & get_last()
    {
      if (this->is_empty())
        throw std::underflow_error("List is empty");

      return head->get_data();
    }

    T remove_first()
    {
      if (this->is_empty())
        throw std::underflow_error("List is empty");

      Node * p = Node::remove_next();
      T ret_val = p->get_data();
      delete p;
      if (--num_items == 0)
        head = this;
      return ret_val;
    }

    T & put(const T & item) { return append(item); }

    T get() { return remove_first(); }

    T & rear() { return get_last(); }

    T & front() { return get_first(); }

    T & push(const T & item) { return insert(item); }

    T pop() { return remove_first(); }

    T & top() const { return get_first(); }

    const size_t & size() const { return num_items; }

    class Iterator : public Node::Iterator
    {
    /* TODO: Agregar atributos y mÃ©todos que hagan falta
             la implementaciÃ³n de este iterador la hice minimalista
             para pruebas de Dyn_Slist_Nc
    */
    public:
      Iterator(Dyn_Slist_Nc<T> & list) : Node::Iterator(list)
      {
        // empty
      }

      T & get_current()
      {
        return Node::Iterator::get_current()->get_data();
      }

      T & get_curr() { return Dyn_Slist_Nc::Iterator::get_current(); }

    };

    Dyn_Slist_Nc() : Node(), head(this), num_items(0)
    {
      // empty
    }

    Dyn_Slist_Nc(const Dyn_Slist_Nc & l) : Node(l), head(this), num_items(0)
    {
      for (Iterator it(const_cast <Dyn_Slist_Nc &>(l));
           it.has_current(); it.next())
        append(it.get_current());
    } 

    ~Dyn_Slist_Nc()
    {
      empty();
    }

    Dyn_Slist_Nc & operator = (const Dyn_Slist_Nc & list)
    {
      if (this == &list) 
        return *this;
 
      empty();
    
      assert(this->is_empty());

      for (Iterator it(const_cast<Dyn_Slist_Nc &>(list)); 
           it.has_current(); it.next())
        append(it.get_current()); 

      return *this;
    }

    T & operator [] (const size_t & n)
    {
      Iterator it(*this);
      for (int i = 0; i < n and it.has_current(); ++i, it.next());

      return it.get_curr();
    }
  };
}

# endif

