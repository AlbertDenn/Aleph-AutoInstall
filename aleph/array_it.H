
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

# ifndef ARRAY_IT_H
# define ARRAY_IT_H

# include <cassert>

namespace Aleph {

    template <class T>
class Array_Iterator
{
  T * ptr = NULL;
  size_t dim = 0;
  size_t num_items = 0;
  long idx = 0;
  long first = 0;
  long last = 0;
  long pos = 0;

public:

  using Item_Type = T;

  T * get_base() noexcept { return ptr; }

  Array_Iterator() noexcept {}

  Array_Iterator(T * p, size_t sz, size_t n) noexcept
    : ptr(p), dim(sz), num_items(n), last(sz - 1)
  {
    assert(num_items <= dim);
  }

  Array_Iterator(T * p, size_t sz, size_t n, long f, long l) noexcept
    : ptr(p), dim(sz), num_items(n), idx(f), first(f), last(l)
  {
    assert(first >= 0 and first < dim and last >= 0 and last <= dim and
           num_items < dim and idx == first);
  }

  bool has_curr() const noexcept { return pos >= 0 and pos < num_items; }

  bool has_current() const noexcept { return has_curr(); }

  long get_pos() const noexcept { return pos; }

  T & get_curr() const
  {
    if (pos < 0)
      throw std::underflow_error("");

    if (pos >= num_items)
      throw std::overflow_error("");

    return ptr[idx]; 
  }

  T & get_current() const { return get_curr(); }

  void next() noexcept
  {
    if (++idx == dim)
      idx = 0;
    ++pos; 
  }

  void prev() noexcept
  {
    if (--idx < 0)
      idx = dim - 1;
    ++pos; 
  }

  void reset() noexcept
  { 
    idx = first;
    pos = 0; 
  }

  void reset_first() noexcept { reset(); }

  void reset_last() noexcept
  { 
    idx = last; 
    pos = num_items - 1;
  }

  void end() noexcept
  {
    reset_last();
    next();
  }
};


} // end namespace Aleph





# endif // ARRAY_IT_H
