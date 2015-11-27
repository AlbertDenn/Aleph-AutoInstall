
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


/*
	Aleph implementation of vector C++ standard container
*/

# ifndef ALEPH_VECTOR_H
# define ALEPH_VECTOR_H

# include <tpl_dynArray.H>
# include <ahIterator.H>
# include <ah_stdc++_utils.H>

namespace Aleph
{

template <class T> class vector;

    template <typename T> 
typename iterator_traits<typename vector<T>::iterator>::difference_type 
distance(typename vector<T>::iterator, typename vector<T>::iterator);

    template <typename T>
class vector
{
public:

  typedef T value_type;

  typedef typename vector::value_type & reference;

  typedef const typename vector::value_type & const_reference;

  typedef size_t size_type;

private:

  mutable DynArray<T> array;

  mutable size_type num_elem;

public:

  class iterator 
  { 
    friend class vector<T>;
    
    static const int Invalid_Position = -1;

    mutable DynArray<T> * dyn_array_ptr;

    mutable size_type current_position;

    T cookie_data;

    iterator(const vector<T> & vec, const size_t & pos = 0)
      :  dyn_array_ptr(&vec.array), current_position(pos) 
    { 
      /* empty */ 
    }

    void set_pos(const size_t &)
    {
      current_position = this->num_elem;
    }

    int get_position() const { return current_position; }

    DynArray<T> * get_dyn_array_ptr() { return dyn_array_ptr; }

    T & access(const size_type & i)
    {
      if (i >= dyn_array_ptr->size())
	return cookie_data;

      return dyn_array_ptr->access(i);
    }

  public:

    typedef typename vector<T>::value_type   value_type;

    typedef int                              difference_type;

    typedef typename vector<T>::value_type * pointer;

    typedef typename vector<T>::reference    reference;

    iterator() : dyn_array_ptr(nullptr), current_position(Invalid_Position) 
    { 
      /* empty */ 
    }

    iterator(const iterator & itor)
      : dyn_array_ptr(itor.dyn_array_ptr),
	current_position(itor.current_position) 
    { 
      /* empty */ 
    }
    
    iterator & operator = (const iterator & itor)
    {
      if (&itor == this)
	return *this;

      dyn_array_ptr = itor.dyn_array_ptr;
      current_position  = itor.current_position;

      return *this;
    }

    T & operator [] (const size_type & index)
    {
      current_position = index;

      return access(index);
    }

    iterator & operator = (const T & key)
    {
      access(current_position) = key;

      return *this;
    }

    T & operator * ()
    {
      return dyn_array_ptr->access(current_position);
    }
    
    T * operator -> ()
    {
      return & dyn_array_ptr->access(current_position);
    }
    
    iterator operator ++ ()
    {
      current_position++;
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator return_value = *this;
      current_position++;
      return return_value;
    }

    iterator operator -- ()
    {
      current_position--;
      return *this;
    }

    iterator operator -- (int)
    {
      iterator return_value = *this;
      current_position--;
      return return_value;
    }

    iterator operator += (const size_type & n)
    {
      current_position += n;
      return *this;
    }

    iterator operator -= (const size_type & n)
    {
      current_position -= n;
      return *this;
    }

    bool operator < (const iterator & itor) const
    {
      return current_position < itor.current_position;
    }
    
    bool operator <= (const iterator & itor) const
    {
      return current_position <= itor.current_position;
    }

    bool operator > (const iterator & itor) const
    {
      return current_position > itor.current_position;
    }
    
    bool operator >= (const iterator& itor) const
    {
      return current_position >= itor.current_position;
    }
    
    bool operator == (const iterator & itor) const
    {
      return current_position == itor.current_position;
    }
    
    bool operator != (const iterator & itor) const
    {
      return current_position != itor.current_position;
    }    
    
    int operator - (const iterator & itor) const
    {
      return current_position - itor.current_position;
    }

    iterator operator + (const int & n) const
    {
      iterator ret_val(*this);

      ret_val.current_position += n;

      return ret_val;
    }

    bool verify(const DynArray<T> & array) const
    {
      return &array == dyn_array_ptr;
    }

    bool verify(const iterator & it) const
    {
      return dyn_array_ptr == it.dyn_array_ptr;
    }
  };

  vector() : array(0), num_elem(0) { /* empty */ }

  vector (const vector & c) : array(c.array), num_elem(c.num_elem)
  {
    // empty
  }

  vector (const size_type & num) : array(num), num_elem(num)
  {
    array.reserve(0, num_elem - 1);
  }

      template <typename Itor> explicit 
  vector (Itor beg, const Itor & end) : array(0), num_elem(0)
  {
    Aleph::verify_iterators(beg, end);

    while (beg != end)
      array[num_elem++] = *beg++;

    assert(num_elem == array.size());
  }

  vector (const size_type & num, const T & value) : array(num), num_elem(num)
  {
    array.reserve(0, num_elem - 1);
        
    for(size_type i = 0; i < num; i++)
      array.access(i) = value;
  }

  ~vector() { /* empty */ }

  const size_type & size() const { return num_elem; }

  bool empty() const { return num_elem == 0; }

  size_type max_size() const { return array.max_size(); }

  size_type capacity() const { return array.size(); }  

  void reserve (const size_type & num)
  {
    if (num < array.size())
      return;

    array.reserve(array.size(), array.size() + num);  
  }

  void resize (const size_type & num)
  {
    if (num < array.size())
      {
	num_elem = num;
	return;
      }

    reserve(num - array.size());
  }
    
  void resize (const size_type & num, const T & value)
  {
    if (num < array.size())
      {
	num_elem = num;
	return;
      }

    reserve(num - array.size());

    for (/* nothing */; num_elem < num; num_elem++)
      array.access(num_elem) = value;
  }

  vector & operator = (const vector & c)
  {
    if (this == &c)
      return *this;

    array = c.array;
    num_elem = c.num_elem;

    return *this;
  }

  void assign (const size_type & num, const T & value)
  {
    if (num > array.size())
      array.reserve(0, num - 1);

    num_elem = num;

    for(size_type i = 0; i < num; i++)
      array.access(i) = value;
  }

      template <typename Itor> 
  void assign (Itor beg, const Itor & end)
  {
    Aleph::verify_iterators(beg, end);

    num_elem = 0;
    while (beg < end)
      array[num_elem++] = *beg++;
  }

  void swap(vector & c)
  {
    std::swap(num_elem, c.num_elem);
    array.swap(c.array);
  }
    
  reference at(const size_type & idx) 
  { 
    return array[idx]; 
  }

  const_reference at(const size_type & idx) const 
  { 
    return array[idx]; 
  }

  reference operator [] (const size_type & idx) 
  { 
    return array.access(idx); 
  }

  const_reference operator [] (const size_type & idx) const 
  {
    return array.access(idx); 
  }

  reference front() const
  { 
    return array.access(0); 
  }

  reference back() const
  { 
    return array.access(num_elem - 1); 
  }

  iterator begin() const
  {
    return iterator (*this); 
  }
  
  iterator end() const
  { 
    return iterator (*this, num_elem); 
  }

private:

  void open_gap(const size_t & position, const size_type & gap_len = 1)
  {
    const size_type old_size = array.size();
    array.reserve(old_size, old_size + gap_len - 1);

    if (position >= old_size)
      return;

    const size_t final_pos = position + gap_len;
    for (int i = array.size() - 1; i > final_pos; i--)
      array.access(i) = array.access(i - gap_len);
  }

public:

  iterator insert(const iterator & pos, const T & value)
  {
    Aleph::verify_container_and_iterator(array, pos);

    open_gap(pos.get_position());

    array.access(pos.get_position()) = value;
  
    num_elem++;

    return pos;    
  }

  void insert(iterator pos, const size_type & len, const T & value)
  {
    Aleph::verify_container_and_iterator(array, pos);

    open_gap(pos.get_position(), len);

    for (int i = 0; i < len; i++, pos++, num_elem++)
      array.access(i) = value;
  }

      template <class Itor>
  void insert(const iterator & pos, Itor beg, const Itor & end)
  {
    Aleph::verify_container_and_iterators(array, pos, beg, end);
    
    size_type gap_len = Aleph::distance<T>(beg, end);

    open_gap(pos.get_position(), gap_len);

    num_elem += gap_len;

    for (int i = pos.get_position(); gap_len > 0; i++, gap_len--)
      array.access(i) = *beg++;
  }

  void push_back (const T & value)
  {
    array[num_elem] = value;
    num_elem++;
  }

private:

  void close_gap(const size_t & position, const size_type & len = 1)
  {
    for (int i = position; i < num_elem - len; i++)
      array.access(i) = array.access(i + len);

    num_elem -= len;
  }

public:

  iterator erase (const iterator & pos)
  {
    Aleph::verify_container_and_iterator(array, pos);

    close_gap(pos.get_position());

    return iterator(*this, pos.get_position());
  }

  iterator erase (const iterator & beg, const iterator & end)
  {
    Aleph::verify_container_and_iterators(array, beg, end);

    const size_t gap_last  = 
      end.get_position() <= num_elem ? end.get_position() : num_elem;
    const size_t gap_start = beg.get_position();
    const size_t gap_len   = gap_last - gap_start;

    if (gap_start > gap_last)
      return iterator(*this, num_elem);

    close_gap(gap_start, gap_len);

    return iterator(*this, gap_start);
  }

  void pop_back() { num_elem--; }

  void clear() { num_elem = 0; }

  bool operator == (const vector & r) const
  {
    if (this == &r)
      return true;

    if (num_elem != r.num_elem)
      return false;

    const size_t len = std::min( num_elem, r.num_elem);

    for (size_t i = 0; i < len; i++)
      if (array.access(i) != r.array.access(i))
	return false;

    return true;
  }

  bool operator != (const vector & r) const
  {
    return not (*this == r);
  }
  
  bool operator < (const vector & r) const
  {
    if (this == &r)
      return false;

    const bool l_smaller = num_elem < r.num_elem;

    const size_t len = l_smaller ? num_elem : r.num_elem;

    for (size_t i = 0; i < len; i++)
      if (array.access(i) < r.array.access(i))
	return true;
      else if (r.array.access(i) < array.access(i))
	return false; // ==> no son iguales 

    return l_smaller;
  }

  bool operator > (const vector & r) const
  {
    return r < *this;
  }

  bool operator <= (const vector & r) const
  {
    return not (r > *this);
  }

  bool operator >= (const vector & r) const
  {
    return not (*this < r);
  }
};



    template <typename T> 
typename iterator_traits<typename vector<T>::iterator>::difference_type 
distance(typename vector<T>::iterator it1, typename vector<T>::iterator it2)
{
  return it2 - it1;
}

} // end namespace Aleph

# endif // ALEPH_VECTOR_H
