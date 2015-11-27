
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

# ifndef SLINK_NC_H
# define SLINK_NC_H

# include <aleph.H>

namespace Aleph
{
  class Slink_Nc
  {
  protected:
    Slink_Nc * next;
  
  public:
    Slink_Nc() : next(nullptr) { /* empty */ }

    Slink_Nc(const Slink_Nc &) : next(nullptr) { /* empty */ }

    ~Slink_Nc() { /* empty */ }

    Slink_Nc & operator = (const Slink_Nc & link)
    {
      if (&link == this)
        return *this;

      if (not is_empty())
        throw std::invalid_argument("link is not empty");

      next = nullptr; 
    }

    void reset()
    {
      assert(this not_eq nullptr);
      next = nullptr;
    }

    bool is_empty() const
    {
      assert(this not_eq nullptr);
      return next == nullptr;
    }

    Slink_Nc *& get_next()
    {
      assert(this not_eq nullptr);
      return next;
    }

    void insert(Slink_Nc * p)
    {
      assert(this not_eq nullptr);
      assert(p not_eq nullptr);
      assert(p->is_empty());
      p->next = next;
      next = p;
    }

    Slink_Nc * remove_next()
    {
      assert(this not_eq nullptr);
      Slink_Nc * ret_val = next;
      next = ret_val->next;
      ret_val->reset();
      return ret_val;
    }

    class Iterator
    {
      Slink_Nc * head;
      Slink_Nc * curr;
    
    public:
      Iterator() : head(nullptr), curr(nullptr) { /* Empty */ }

      Iterator(Slink_Nc * head_ptr) : head(head_ptr), curr(head->get_next())
      { 
        // empty
      }

      Iterator(Slink_Nc & _head) : head(&_head), curr(head->get_next())
      { 
        // empty
      }

      Iterator(Slink_Nc * head_ptr, Slink_Nc * curr_ptr)
        : head(head_ptr), curr(curr_ptr)
      {
        // empty
      }

      void reset_first() 
      {
        assert(curr != nullptr and head != nullptr);
        curr = head->get_next();
      }

      void set(Slink_Nc * new_curr) 
      {
        assert(curr != nullptr and head != nullptr);
        curr = new_curr;
      }

      void reset(Slink_Nc * new_head)
      {
        head = new_head;
        curr = head->get_next();;
      }

      bool has_current() const 
      {
        assert(head != nullptr);
        return curr != nullptr;
      }

      // sinÃ³nimo de has_current()
      bool has_curr() const { return has_current(); }

      Slink_Nc * get_current()  
      {
        assert(curr != nullptr and head != nullptr);

        if (not has_current())
          throw std::overflow_error("Not element in list");

        return curr;
      }

      // sinÃ³nimo de get_current()
      Slink_Nc * get_curr() { return get_current(); }

      bool is_in_first() const { return curr == head->next; }

      void next() throw(std::exception, std::overflow_error)
      {
        if (not has_current())
          throw std::overflow_error("Not next element in list");

        curr = curr->get_next();
      }

      bool operator == (const Iterator & it) const { return curr == it.curr; }

      bool operator != (const Iterator & it) const { return curr != it.curr; }
    
      bool verify(Slink_Nc * l) const { return head == l; }

      bool verify(const Iterator & it) const { return head == it.head; }
    };
    
  };
}

# endif

