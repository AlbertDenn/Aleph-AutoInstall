
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

# ifndef TPL_UNION_H
# define TPL_UNION_H

# include <tpl_dynArray.H>
# include <tpl_dynSetTree.H>
		

class Fixed_Relation 
{
protected:

  DynArray<size_t> id;
  DynArray<size_t> sz;
  size_t           num_blocks;

  virtual size_t root(size_t i)
  {
    while (i != id(i))
      {
	id(i) = id(id(i));
	i = id(i);
      }
    
    return i;			
  }

  size_t depth(size_t i) noexcept
  {
    const int & parent = id(i);
    if (parent == i)
      return 1;

    return 1 + depth(parent);
  }

public:

  Fixed_Relation(size_t n = 0) : num_blocks(n)
  {
    if (num_blocks == 0)
      return;

    id.reserve(num_blocks);
    sz.reserve(num_blocks);
    for (size_t i = 0; i < num_blocks; ++i)
      {
	id(i) = i;
	sz(i) = 1;
      }
  }

  void set_n(size_t n) { new (this) Fixed_Relation(n); }

  size_t size() const noexcept { return id.size(); }

  size_t get_num_blocks() const noexcept { return num_blocks; }

  bool are_connected(size_t i, size_t j) noexcept
  {
    return root(i) == root(j);
  }

  void join(size_t i, size_t j)	noexcept
  {
    i = root(i);
    j = root(j);
    if (i == j)
      return;

    if (sz(i) < sz(j))
      {
	id(i) = j;
	sz(j) += sz(i);
      }
    else
      {
	id(j) = i;
	sz(i) += sz(j);
      }
    --num_blocks;
  }
};


class Relation : public Fixed_Relation
{
  void verify_if_add_new_points(size_t n)
  {
    size_t l = size();
    if (n < l)
      return;
    
    id.reserve(l, n);
    sz.reserve(l, n);
    for (int i = l; i <= n; ++i)
      {
	id(i) = i;
	sz(i) = 1;
      }
    num_blocks += n - l + 1;
  }

  virtual size_t root(size_t i)
  {
    verify_if_add_new_points(i);
    return Fixed_Relation::root(i);
  }

public:

  Relation(size_t n = 0) : Fixed_Relation(n) {}
};


    template <typename T, class Compare = Aleph::less<T>>
class Relation_T : public Relation
{
  struct Pair
  {
    T      item;
    size_t i;

    Pair() noexcept {}

    Pair(const T & __item, size_t __i) 
    noexcept(std::is_nothrow_copy_assignable<T>::value)
      : item(__item), i(__i) { /* empty */ }
  };

  struct Cmp
  {
    bool operator () (const Pair & p1, const Pair & p2) const noexcept
    {
      return Compare() (p1.item, p2.item);
    }
  };

  DynSetAvlTree<Pair, Cmp> items_tree;

  // retorna el id del item; sea porque lo encuentra o porque lo inserta
  size_t test_and_insert_new_item(const T & item) 
  {
    size_t N = size();
    Pair p(item, N);
    Pair * ptr = items_tree.search_or_insert(p);
    return ptr->i;
  }

public:

  bool are_connected(const T & p, const T & q)
  {
    size_t i = test_and_insert_new_item(p);
    size_t j = test_and_insert_new_item(q);

    return Relation::are_connected(i, j);
  }

  void join(const T & p, const T & q)
  {
    size_t i = test_and_insert_new_item(p);
    size_t j = test_and_insert_new_item(q);
    Relation::join(i, j);
  }
};

# endif // TPL_UNION_H
