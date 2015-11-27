
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


# ifndef TPL_SLIST_H
# define TPL_SLIST_H

# include <ahDefs.H>
# include <tpl_snode.H>

using namespace Aleph;

namespace Aleph {

template <typename T> class Slist : public Snode<T>
{

public:

  typedef Snode<T> Node;

  Slist() { /* empty */ }

  void insert_first(Node * node)
  {
    assert(node not_eq nullptr);
    assert(node->is_empty());

    this->insert_next(node);
  }

  Node * remove_first() 
    throw(std::exception, std::underflow_error)
  {
    if (this->is_empty())
      throw std::underflow_error ("list is empty");

    return this->remove_next();
  }

  Node * get_first() 
    Exception_Prototypes(std::underflow_error)
  {
    if (this->is_empty())
      throw std::underflow_error ("list is empty");

    return this->get_next(); 
  }

  class Iterator
  {

  private:

    Slist * list;
    Node  * current;

  public:
    typedef Snode<T> Set_Type;
    typedef Snode<T> * Item_Type;

    Iterator(Slist & _list) : list(&_list), current(list->get_first()) {}

    bool has_current() const { return current != list; }

    Node * get_current() 

      throw(std::exception, std::overflow_error)

    {

      if (not this->has_current())
        throw std::overflow_error ("");

      return current;
    }
    void next()

      throw(std::exception, std::overflow_error)

    {

      if (not this->has_current())
        throw std::overflow_error ("");

      current = current->get_next();
    }
    void reset_first() { current = list->get_next(); }
    Iterator & operator = (Node * node)
    {
      if (this == node) 
        return *this;

      current = node;
      return *this;
    }
  };
};

} // end namespace Aleph

# endif /* TPL_SLIST_H */

