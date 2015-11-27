
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

# ifndef AHITERATOR_H
# define AHITERATOR_H

# include <type_traits>

namespace Aleph
{

    template <class Itor>
struct iterator_traits
{
  typedef typename Itor::value_type        value_type;
  typedef typename Itor::difference_type   difference_type;
  typedef typename Itor::pointer           pointer;
  typedef typename Itor::reference         reference;
};

# define STL_ITOR_SPEC(It_Name)						\
  public:								\
									\
  using T = typename Set_Type::Item_Type;				\
  using Itor = typename Set_Type::Iterator;				\
									\
  using Itor::Itor;							\
									\
  Itor operator ++ ()							\
  {									\
    this->next();							\
    return *this;							\
  }									\
									\
  Itor operator ++ (int)						\
  {									\
    Itor ret_val = *this;						\
    this->next();							\
    return ret_val;							\
  }									\
									\
  bool operator == (const Itor & it) const noexcept			\
  {									\
    if (this->has_curr() and it.has_curr())				\
      return this->get_pos() == it.get_pos();				\
									\
    if (not this->has_curr() and not it.has_curr())			\
      return true;							\
									\
    return false;							\
  }									\
									\
  bool operator != (const Itor & it) const noexcept			\
  {									\
    return not (*this == it);						\
  }


  // Define an general iterator whose interface is minimally std-c++ compliant
    template <class Set_Type>
struct __iterator : public Set_Type::Iterator
{
  STL_ITOR_SPEC(__iterator);

  T & operator * () noexcept { return const_cast<T&>(this->get_curr()); }

  T * operator -> () noexcept
  { 
    return &const_cast<T&>(this->get_curr()); 
  }

  static __iterator begin(const Set_Type & s) noexcept
  {
    return __iterator(s);
  }

  static __iterator end(const Set_Type & s) noexcept
  {
    __iterator it(s);
    it.Itor::end();
    return it;
  }
};

    template <class Set_Type>
struct __const_iterator : public Set_Type::Iterator
{
  STL_ITOR_SPEC(__const_iterator);

  const T & operator * () const noexcept { return this->get_curr(); }

  T const * operator -> () const noexcept
  { 
    return const_cast<T*>(&this->get_curr()); 
  }

  static __const_iterator cbegin(const Set_Type & s) noexcept
  {
    return __const_iterator(s);
  }

  static __const_iterator cend(const Set_Type & s) noexcept
  {
    __const_iterator it(s);
    it.Itor::end();
    return it;
  }
};


# define STL_ALEPH_ITERATOR(Set_Name)					\
  using iterator = __iterator<Set_Name>;				\
									\
  using const_iterator = __const_iterator<Set_Name>;			\
									\
  iterator begin() noexcept { return iterator::begin(*this); }		\
									\
  iterator end() noexcept { return iterator::end(*this); }		\
									\
  const_iterator begin() const noexcept					\
  {									\
    return const_iterator::cbegin(*this);				\
  }									\
									\
  const_iterator end() const noexcept					\
  {									\
    return const_iterator::cend(*this);					\
  }									\
  									\
  const_iterator cbegin() const	noexcept				\
  {									\
    return const_iterator::cbegin(*this);				\
  }									\
									\
  const_iterator cend()	const noexcept					\
  {									\
    return const_iterator::cend(*this);					\
  }									\
									\
  const_iterator cbegin() noexcept					\
  {									\
    return const_iterator::cbegin(*this);				\
  }									\
  									\
  const_iterator cend() noexcept					\
  {									\
    return const_iterator::cend(*this);					\
  }									\
									\
  friend const_iterator cbegin(const Set_Name & s) noexcept		\
  { return s.begin(); }							\
									\
  friend const_iterator cend(const Set_Name & s) noexcept		\
  { return s.end(); }							\
  									\
  friend const_iterator begin(const Set_Name & s) noexcept		\
  { return s.begin(); }							\
									\
  friend const_iterator end(const Set_Name & s) noexcept		\
  { return s.end(); }							\
									\
  friend iterator begin(Set_Name & s) noexcept { return s.begin(); }	\
									\
  friend iterator end(Set_Name & s) noexcept { return s.end(); }

# ifdef nada
    inline template <class Itor>
typename iterator_traits<Itor>::difference_type distance(Itor it1, Itor it2)
{
  typename iterator_traits<Itor>::difference_type d = 0;

  while (it1 not_eq it2)
    {
      d++;
      it1++;
    }

  return d;
}

# endif

}; // end namespace Aleph

# endif // AHITERATOR_H
