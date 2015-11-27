
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


# ifndef TPL_DNODE_H
# define TPL_DNODE_H

# include <type_traits>
# include <ahFunction.H>
# include <dlink.H>

using namespace Aleph;

namespace Aleph {

template <typename T> class Dnode : public Dlink 
{
private:

  mutable T data;

public:

  Dnode<T> *& get_next() const noexcept { return (Dnode<T>*&) next; }

  Dnode<T> *& get_prev() const noexcept { return (Dnode<T>*&) prev; }

  Dnode<T>* remove_prev() noexcept { return (Dnode<T>*) Dlink::remove_prev(); }

  Dnode<T>* remove_next() noexcept { return (Dnode<T>*) Dlink::remove_next(); }

  Dnode<T> *& get_first() const noexcept { return get_next(); } 

  Dnode<T> *& get_last() noexcept { return get_prev(); }

  Dnode<T>* remove_last() noexcept 
  { 
    return static_cast<Dnode<T>*>(Dlink::remove_prev()); 
  }

  Dnode<T>* remove_first() noexcept
  {
    return static_cast<Dnode<T>*>(Dlink::remove_next()); 
  }
  
  Dnode() noexcept 
  {
    static_assert(std::is_default_constructible<T>::value, 
		  "No default constructor for T");
  }

  Dnode(const T & item) noexcept(noexcept(T(item))) : data(item) 
  {
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
  }

  Dnode(T && item) noexcept(noexcept(std::swap(data, item)))
  {
    static_assert(std::is_move_constructible<T>::value,
		  "No move constructor for T");
    std::swap(data, item);
  }

  Dnode & swap(Dnode & p) noexcept(noexcept(std::swap(data, p.data)))
  {
    
    Dlink::swap(p);
    std::swap(data, p.data);
    return *this;
  }

  Dnode(const Dnode & node) noexcept(std::is_nothrow_copy_assignable<T>::value)
  : Dlink(node), data(node.data) 
  {
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
  }

  Dnode(Dnode && node) noexcept(noexcept(std::forward<T>(node.data)))
  : Dlink(node), data(std::forward<T>(node.data))
  {
    /* empty */ 
  }

  Dnode & operator = (const Dnode & p) 
  noexcept(std::is_nothrow_copy_assignable<T>::value)
  {
     static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    data = p.data;
    return *this;
  }

  Dnode & operator = (Dnode && p) 
  noexcept(std::is_nothrow_move_assignable<T>::value)
  {
    std::swap(data, p.data);
    return *this;
  }

  T & get_data() noexcept { return data; }

  const T & get_data() const noexcept { return data; }

  using key_type = T;

  T & get_key() noexcept { return data; }

  const T & get_key() const noexcept { return data; }

  static Dnode * data_to_node(T & data) noexcept
  {
    Dnode * zero = 0;
    long offset = (long) &(zero->data);
    unsigned long  addr = (unsigned long) (&data);
    return (Dnode*) (addr - offset);
  }

  class Iterator : public Dlink::Iterator
  {
  public:
    
    using Set_Type = Dnode<T>;
    
    using Item_Type = Dnode<T>*;

    using Base = Dlink::Iterator;

    using Base::Base;

    Dnode<T> * get_current() const 
    {
      return static_cast<Dnode<T>*>(this->Dlink::Iterator::get_current()); 
    }

    Dnode<T> * get_curr() const  
    {
      return Dnode<T>::Iterator::get_current(); 
    }

    Dnode * del() 
    { return static_cast<Dnode*>(Dlink::Iterator::del()); }
  };
};

}

# endif /* TPL_DNODE_H */

