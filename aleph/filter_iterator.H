
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


# ifndef FILTER_ITERATOR_H
# define FILTER_ITERATOR_H

# include <stdexcept>

namespace Aleph {


    template <class Container, class It, class Show_Item>
class Filter_Iterator : public It
{
  Show_Item & show_item; 

  Container * container_ptr = nullptr;

  void * cookie = nullptr;

  void goto_first_valid_item() 
  {
    try
      {     // colocarse en el primer elemento que acepte show_item
        for (this->It::reset_first(); true; this->It::next())
          if (not this->It::has_curr() or show_item(this->It::get_curr()))
            return;
      }
    catch (std::overflow_error) { /* seguir en overflow; no propagar */ }
  }

  void forward()
  {
    this->It::next(); 
    try 
      {     // avanzar hasta el siguiente item que acepte show_item
        for (;true; this->It::next()) 
	  {
            if (not this->It::has_curr() or show_item(this->It::get_curr()))
	      return ;
	  }
      }
    catch (std::overflow_error) { /* seguir en overflow; no propagar */ }
  }

  void goto_last_valid_item()
  {
    for (this->It::reset_last(); true; this->It::prev())
      if (not this->It::has_curr() or show_item(this->It::get_curr()))
        return;
  }

  void backward()
  {
    this->It::prev();
    try 
      {     
        for (;true; this->It::prev()) 
          if (not this->It::has_curr() or show_item(this->It::get_curr()))
            return;
      }
    catch (std::underflow_error) { /* seguir en underflow sin propagar */ }
  }
 
public:

  typedef typename It::Item_Type Item_Type;

  typedef It Iterator_Type;

  const Container & get_container() const
  {
    return *const_cast<Container*>(container_ptr);
  }

  It & get_iterator() { return *this; }

  Show_Item & get_filter() { return show_item; }

  void set_filter(Show_Item && si)
  {
    show_item = si;
  }

  void set_filter(Show_Item & si)
  {
    show_item = si;
  }

  Filter_Iterator(Show_Item & si) 
    : show_item(si)
  {
    // empty
  }

  Filter_Iterator(Show_Item && si = Show_Item()) : Filter_Iterator(si)
  {
    // empty
  }

  Filter_Iterator(const Container & c, Show_Item & si) 
    : It(c), show_item(si), container_ptr(&const_cast<Container&>(c))
  {
    goto_first_valid_item();
  }

  Filter_Iterator(const Container & c, Show_Item && si = Show_Item()) 
    : Filter_Iterator(c, si)
  {
    // empty
  }

  Filter_Iterator(const Container & c, void * __cookie, Show_Item & si) 
    : Filter_Iterator(c, si)
  {
    cookie = __cookie;
  }

  void set_cookie(void * __cookie) { cookie = __cookie; }

  void next() { forward(); }

  void prev() { backward(); }

  void reset_first() { goto_first_valid_item(); }

  void reset_last() { goto_last_valid_item(); }
};


} // end namespace Aleph
# endif // FILTER_ITERATOR_H

