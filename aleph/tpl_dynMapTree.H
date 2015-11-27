
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

# ifndef TPL_DYNMAPTREE_H
# define TPL_DYNMAPTREE_H

# include <tpl_dynSetTree.H>
 
using namespace Aleph;

namespace Aleph {

template <
  typename Key, typename Data,
  template <typename, class> class Tree = Avl_Tree,
  class Compare                         = Aleph::less<Key>>
class DynMapTree : 
public DynSetTree<std::pair<Key, Data>, Tree, Dft_Pair_Cmp<Key, Data, Compare>>
{
  using Pair = std::pair<Key, Data>;

  using Base = DynSetTree<Pair, Tree, Dft_Pair_Cmp<Key, Data, Compare>>;

public:

  using Key_Type = Key;

  using Item_Type = Key;

  using Value_Type = Data ;

  // using Base::Base;  // no more need. But I don't remember why I put it
  using Base::insert;

  static Data & get_data(const Key & key) noexcept
  {
    return key_to_pair<Key, Data>(&const_cast<Key&>(key))->second;    
  }

  static const Key & get_key(Data * data_ptr) noexcept
  {
    return data_to_pair<Key, Data>(data_ptr)->first;
  }

  Pair * insert(const Key & key, const Data & data)
  {
    return this->Base::insert(Pair(key, data));
  }

  Pair * insert(const Key & key, Data && data = Data())
  {
    return this->Base::insert(Pair(key, std::forward<Data>(data)));
  }
  
  Pair * insert(Key && key, const Data & data)
  {
    return this->Base::insert(Pair(std::forward<Key>(key), data));
  }

  Pair * insert(Key && key, Data && data = Data())
  {
    return this->Base::insert(Pair(std::forward<Key>(key), 
				   std::forward<Data>(data)));
  }

  Pair * put(const Key & key, const Data & data)
  {
    return insert(key, data);
  }

  Pair * put(const Key & key, Data && data)
  {
    return insert(key, std::forward<Data>(data));
  }

  Pair * put(Key && key, const Data & data)
  {
    return insert(std::forward<Key>(key), data);
  }

  Pair * put(Key && key, Data && data)
  {
    return insert(std::forward<Key>(key), std::forward<Data>(data));
  }

  size_t remove(const Key & key) noexcept 
  {
    return this->Base::remove(Pair(key, Data()));
  }

  size_t remove(Key && key) noexcept 
  {
    return this->Base::remove(Pair(move(key), Data()));
  }

  Pair * search(const Key & key) const noexcept
  {
    return this->Base::search(Pair(key, Data()));
  }

  Pair * search(Key && key) const noexcept
  {
    return this->Base::search(Pair(move(key), Data()));
  }

  bool has(const Key & key) const noexcept { return search(key) != nullptr; }

  bool has(Key && key) const noexcept { return search(move(key)) != nullptr; }

  bool contains(const Key & key) const noexcept { return has(key); }

  bool contains(Key && key) const noexcept { return has(move(key)); }
  
  Data & find(const Key & key) 
  { 
    return this->Base::find(Pair(key, Data())).second;
  }

  const Data & find(const Key & key) const
  { 
    return this->Base::find(Pair(key, Data())).second;
  }

  Data & operator [] (const Key & key)
  { 
    return this->search_or_insert(Pair(key, Data()))->second;
  }

  const Data & operator [] (const Key & key) const
  {
    return this->find(key);
  }

  Data & operator [] (Key && key)
  { 
    return this->search_or_insert(Pair(move(key), Data()))->second;
  }

  const Data & operator [] (Key && key) const
  {
    return this->find(move(key));
  }

  using Iterator = typename Base::Iterator;

  DynList<Key> keys() const
  {
    return this->template map<Key>([] (auto p) { return p.first; });
  }

  DynList<Key> values() const
  {
    return this->template map<Data>([] (auto p) { return p.second; });
  }

  DynList<Data*> values_ptr()
  {
    DynList<Data*> ret;
    for (Iterator it(*this); it.has_curr(); it.next())
      ret.append(&it.get_curr().second);
    return ret;
  }

  DynList<Pair*> items_ptr()
  {
    DynList<Pair*> ret;
    for (Iterator it(*this); it.has_curr(); it.next())
      ret.append(&it.get_curr());
    return ret;
  }
};
} // end namespace Aleph


# include <tpl_binTree.H>
# include <tpl_avl.H>
# include <tpl_rb_tree.H>
# include <tpl_rand_tree.H>
# include <tpl_treap.H>
# include <tpl_treapRk.H>
# include <tpl_splay_tree.H>

namespace Aleph {
  template <typename Key, typename Type, class Compare = Aleph::less<Key> >
  class DynMapBinTree : public DynMapTree<Key, Type, BinTree, Compare> {};

  template <typename Key, typename Type, class Compare = Aleph::less<Key> >
  class DynMapAvlTree : public DynMapTree<Key, Type, Avl_Tree, Compare> {};
    template <typename Key, typename Type, class Compare = Aleph::less<Key> >
    class DynMapRbTree : public DynMapTree<Key, Type, Rb_Tree, Compare>
    { /* empty */ };

    template <typename Key, typename Type, class Compare = Aleph::less<Key> >
    class DynMapRandTree : public DynMapTree<Key, Type, Rand_Tree, Compare>
    { /* empty */ };

    template <typename Key, typename Type, class Compare = Aleph::less<Key> >
    class DynMapTreap : public DynMapTree<Key, Type, Treap, Compare>
    { /* empty */ };

    template <typename Key, typename Type, class Compare = Aleph::less<Key> >
    class DynMapTreapRk : public DynMapTree<Key, Type, Treap_Rk, Compare>
    { /* empty */ };

    template <typename Key, typename Type, class Compare = Aleph::less<Key> >
    class DynMapSplayTree : public DynMapTree<Key, Type, Splay_Tree, Compare>
    { /* empty */ };

} // end namespace Aleph

# endif /* TPL_DYNMAPTREE_H */

