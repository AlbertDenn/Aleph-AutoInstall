
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

# ifndef TPL_DYNSETOHASH_H
# define TPL_DYNSETOHASH_H

# include <tpl_olhash.H>
# include <tpl_odhash.H>

namespace Aleph
{

template <typename Key, typename Data, 
	  class Cmp = Aleph::Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>,
	  template <typename, class> class HashTable = ODhashTable>
struct MapOpenHash : public HashTable<std::pair<Key, Data>, Cmp>
{
  using Pair = std::pair<Key, Data>;
  using Base = HashTable<Pair, Cmp>;
  using Base::Base;
  using Base::insert;

  static Pair * key_to_pair(Key * ptr) noexcept { return (Pair*) ptr; }

  static Pair * data_to_pair(Data * ptr) noexcept
  {
    Pair * zero = 0;
    return (Pair*) ((long) ptr - (long) &zero->second); 
  }

  static Data & get_data(const Key & key) noexcept
  {
    return key_to_pair(&const_cast<Key&>(key))->second;    
  }

  static const Key & get_key(Data * data_ptr) noexcept
  {
    return data_to_pair(data_ptr)->first;
  }  

  using Value_Type = Data;

  Pair * insert(const Key & key, const Data & data)
  {
    return this->Base::insert(Pair(key, data));
  }

  Pair * insert(const Key & key, Data && data)
  {
    return this->Base::insert(Pair(key, std::forward<Data>(data)));
  }

  Pair * insert(Key && key, Data && data)
  {
    return this->Base::insert(Pair(std::forward<Key>(key), 
				   std::forward<Data>(data)));
  }

  Pair * insert(Key && key, const Data & data)
  {
    return this->Base::insert(Pair(std::forward<Key>(key), data));
  }

  Pair * search(const Key & key) const noexcept
  {
    return this->Base::search(Pair(key, Data()));
  }

  Pair * search(Key && key) const noexcept
  {
    return this->Base::search(Pair(move(key), Data()));
  }

  bool has(const Key & key) const noexcept
  {
    return search(key) != nullptr;
  }

  bool has(Key && key) const noexcept
  {
    return search(move(key)) != nullptr;
  }

  bool contains(const Key & key) const noexcept { return has(key); }

  bool contains(Key && key) const noexcept { return has(move(key)); }

  Data & find(const Key & key)
  {
    return Base::find(Pair(key, Data())).second;
  }

  Data & find(Key && key)
  {
    return Base::find(Pair(move(key), Data())).second;
  }

  const Data & find(const Key & key) const
  {
    return Base::find(Pair(key, Data())).second;
  }

  const Data & find(Key && key) const
  {
    return Base::find(Pair(move(key), Data())).second;
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

  void remove_by_data(Data & data) noexcept
  {
    Base::remove_ptr(data_to_pair(&data));
  }

  void remove(const Key & key) noexcept
  {
    Base::remove(Pair(key, Data()));
  }

  void remove(Key && key) noexcept
  {
    Base::remove(Pair(move(key), Data()));
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



template <typename Key, typename Data, 
	  class Cmp = Aleph::Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>>
struct MapOLhash : public MapOpenHash<Key, Data, Cmp, OLhashTable>
{
  using Base = MapOpenHash<Key, Data, Cmp, OLhashTable>;  
  using Base::Base;  
};


template <typename Key, typename Data, 
	  class Cmp = Aleph::Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>>
struct MapODhash : public MapOpenHash<Key, Data, Cmp, ODhashTable>
{
  using Base = MapOpenHash<Key, Data, Cmp, ODhashTable>;
  using Base::Base;  
};




} // end namespace Aleph

# endif // TPL_DYNSETOHASH_H
