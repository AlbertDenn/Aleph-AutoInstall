
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


# ifndef TPL_DYNSLIST_H
# define TPL_DYNSLIST_H

# include <tpl_slist.H>

using namespace Aleph;

namespace Aleph {

    template <typename T>
class DynSlist : public Slist<T>
{

private:

  size_t num_items;
  int        current_pos;
  Snode<T> * current_node;
  typename Slist<T>::Node * get_previous_to_pos(const int & pos)
  {

    if (pos > num_items)
      throw std::out_of_range ("position out of range");

    if (pos < current_pos) // hay que retroceder?
      { // Si, reinicie posición actual
        current_pos  = 0;
        current_node = this;
      }
    while (current_pos < pos)  // avanzar hasta nodo predecesor a pos
      {
        current_node = current_node->get_next();
        ++current_pos;
      }
    return current_node;
  }

public:

  DynSlist() : num_items(0), current_pos(0), current_node(this)
  { 
    // Empty 
  } 
  T & operator [] (const size_t & i) 

    throw(std::exception, std::out_of_range)

  {
    return get_previous_to_pos(i)->get_next()->get_data();
  }    

  size_t size() const { return num_items; }

  void insert(const int & pos, const T & data) 

    throw(std::exception, std::bad_alloc, std::out_of_range)

  {      // apartar nodo para nuevo elemento
    typename Slist<T>::Node * node = new typename Slist<T>::Node (data);
    typename Slist<T>::Node * prev = get_previous_to_pos(pos); 
    prev->insert_next(node);
    ++num_items;
  }
  void remove(const int & pos) 

    throw(std::exception, std::range_error)

  {      // obtener nodo predecesor al nuevo elemento
    typename Slist<T>::Node * prev = get_previous_to_pos(pos);
    typename Slist<T>::Node * node_to_delete = prev->remove_next();
    delete node_to_delete;
    --num_items;
  }
  ~DynSlist() 
  {      // eliminar nodo por nodo hasta que la lista devenga vacía
    while (not this->is_empty())
      delete this->remove_first(); // remove_first de clase Slink
  }
  class Iterator : public Slist<T>::Iterator
  {
  public:
    typedef Slist<T> Set_Type;
    typedef T Item_Type;

    Iterator(DynSlist & list) : Slist<T>::Iterator(list) { /* Empty */ } 

    T & get_current() { return Slist<T>::Iterator::get_current()->get_data(); }
  };

};

} // end namespace Aleph

# endif // TPL_DYNSLIST_H

