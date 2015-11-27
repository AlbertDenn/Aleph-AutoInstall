
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


# ifndef AH_ITERATOR_H
# define AH_ITERATOR_H



template <class SetType>
struct StlIterator : public SetType::Iterator
{
  using T = typename SetType::Item_Type;
  using Itor = typename SetType::Iterator;

  using Itor::Itor;

  Itor operator ++ ()
  {
    this->next();
    return *this;
  }

  Itor operator ++ (int)
  {
    Itor ret_val = *this;
    this->next();
    return ret_val;
  }

  bool operator == (const Itor & it) const noexcept
  {
    if (this->has_curr() and it.has_curr())
      return this->get_pos() == it.get_pos();
      // return this->get_curr() == it.get_curr();

    if (not this->has_curr() and not it.has_curr())
      return true;

    return false;
  }

  bool operator != (const Itor & it) const noexcept
  {
    return not (*this == it);
  }

  T & operator * () noexcept { return const_cast<T&>(this->get_curr()); }

  T * operator -> () noexcept { return &const_cast<T&>(this->get_curr()); }

  static StlIterator begin(const SetType & s) noexcept
  {
    return StlIterator(s);
  }

  static StlIterator end(const SetType & s) noexcept
  {
    StlIterator it(s);
    it.Itor::end();
    return it;
  }
};



template <class SetType>
struct StlConstIterator : public SetType::Iterator
{
  using T = typename SetType::Item_Type;
  using Itor = typename SetType::Iterator;

  using Itor::Itor;

  Itor operator ++ ()
  {
    this->next();
    return *this;
  }

  Itor operator ++ (int)
  {
    Itor ret_val = *this;
    this->next();
    return ret_val;
  }

  bool operator == (const Itor & it) const noexcept
  {
    if (this->has_curr() and it.has_curr())
      return this->get_pos() == it.get_pos();

    if (not this->has_curr() and not it.has_curr())
      return true;

    return false;
  }

  bool operator != (const Itor & it) const noexcept
  {
    return not (*this == it);
  }

  const T & operator * () const noexcept { return this->get_curr(); }

  T const * operator -> () const noexcept
  { 
    return const_cast<T*>(&this->get_curr()); 
  }

  static StlConstIterator cbegin(const SetType & s) noexcept
  {
    return StlConstIterator(s);
  }

  static StlConstIterator cend(const SetType & s) noexcept
  {
    StlConstIterator it(s);
    it.Itor::end();
    return it;
  }
};


template <class SetName>
class StlAlephIterator
{
  SetName * me() { return static_cast<SetName*>(this); }

  const SetName * const_me() const { return static_cast<const SetName*>(this); }

public:

  using iterator = StlIterator<SetName>;

  using const_iterator = StlConstIterator<SetName>;

  iterator begin() noexcept { return iterator::begin(*me()); }

  iterator end() noexcept { return iterator::end(*me()); }

  const_iterator begin() const noexcept
  {
    return const_iterator::cbegin(*const_me());
  }

  const_iterator end() const noexcept
  {
    return const_iterator::cend(*const_me());
  }

  const_iterator cbegin() const	noexcept
  {
    return const_iterator::cbegin(*const_me());
  }

  const_iterator cend()	const noexcept
  {
    return const_iterator::cend(*const_me());
  }

  const_iterator cbegin() noexcept
  {
    return const_iterator::cbegin(*const_me());
  }

  const_iterator cend() noexcept 
  {
    return const_iterator::cend(*const_me()); 
  }

  friend const_iterator cbegin(const SetName & s) noexcept { return s.begin(); }

  friend const_iterator cend(const SetName & s) noexcept { return s.end(); }

  friend const_iterator begin(const SetName & s) noexcept { return s.begin(); }

  friend const_iterator end(const SetName & s) noexcept { return s.end(); }

  friend iterator begin(SetName & s) noexcept { return s.begin(); }

  friend iterator end(SetName & s) noexcept { return s.end(); }
};


# endif // AH_ITERATOR_H
