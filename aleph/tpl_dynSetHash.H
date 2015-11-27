
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

# ifndef TPL_DYNSETHASH_H
# define TPL_DYNSETHASH_H

# include <algorithm>
# include <typeinfo>
# include <ahDry.H>
# include <ahIterator.H>
# include <primes.H>
# include <htlist.H>
# include <tpl_dynArray.H>
# include <tpl_dynSetOhash.H>
# include <tpl_dynLhash.H>
# include <tpl_linHash.H>


namespace Aleph  
{

  template <typename                          Key, 
	    class                             Cmp       = Aleph::equal_to<Key>,
	    template <typename,  class> class HashTable = LhashTable>
class DynHashTable 
    : public HashTable<Key, Cmp>,
      public GenericTraverse<DynHashTable<Key, Cmp, HashTable>>,
      public LocateFunctions<DynHashTable<Key, Cmp, HashTable>, Key>,
      public FunctionalMethods<DynHashTable<Key, Cmp, HashTable>, Key>,
      public GenericKeys<DynHashTable<Key, Cmp, HashTable>, Key>,
      public EqualToMethod<DynHashTable<Key, Cmp, HashTable>>,
      public StlAlephIterator<DynHashTable<Key, Cmp, HashTable>>
{
protected:

  using Base = HashTable<Key, Cmp>;

  using Bucket = typename HashTable<Key, Cmp>::Bucket;

public:

  using Hash_Fct = typename Base::Hash_Fct;

  using Key_Type = Key;

  using Item_Type = Key;

  DynHashTable(const size_t len, Hash_Fct hash_fct,
	       Cmp & cmp, const float lower_alpha, const float upper_alpha) 
    : Base(len, hash_fct, cmp, lower_alpha, upper_alpha, true, true)
  {
    // empty
  }

  DynHashTable(size_t len        = Primes::DefaultPrime,
	       Hash_Fct hash_fct = Aleph::dft_hash_fct<Key>,
	       Cmp && cmp        = Cmp(),
	       float lower_alpha = hash_default_lower_alpha,
	       float upper_alpha = hash_default_upper_alpha) 
    : DynHashTable(len, hash_fct, cmp, lower_alpha, upper_alpha)
    {
      // empty
    }

private:

  void copy(const DynHashTable & other)
  {
    for (typename Base::Iterator it(other); it.has_curr(); it.next())
      {
	Bucket * bucket = (Bucket*) it.get_curr();
	insert(bucket->get_key());
	assert(*search(bucket->get_key()) == bucket->get_key());
      }
  }

public:

  DynHashTable(const DynHashTable & other)
    : Base(other.len, other.hash_fct, 
	   const_cast<DynHashTable&>(other).get_compare(),
	   other.lower_alpha, other.upper_alpha, true, true)
  {
    copy(other);
  }
  
  DynHashTable(DynHashTable && other) noexcept
    : Base(other.len, other.hash_fct, other.get_compare(),
	   other.lower_alpha, other.upper_alpha, true, true)
  {
    this->swap(other);
  }

  Special_Ctors(DynHashTable, Key);

  ~DynHashTable()
  {
    this->empty();
  }

  DynHashTable & operator = (const DynHashTable & other)
  {
    if (this == &other)
      return *this;

    this->empty();
    copy(other);

    return *this;
  }

  DynHashTable & operator = (DynHashTable && other) noexcept
  {
    this->swap(other);
    return *this;
  }

protected:

  Key * insert_bucket(Bucket * bucket) 
  {
    Bucket * ret_val = (Bucket*) this->Base::insert(bucket);
    if (ret_val == nullptr) // is the key in the table?
      {    // yes! ==> free bucket
	delete bucket;
	return nullptr;
      }

    return &ret_val->get_key();
  }

  tuple<Key*, bool> search_or_insert_bucket(Bucket * bucket)
  {
    Bucket * ret_val = (Bucket*) this->Base::search_or_insert(bucket);
    if (ret_val != bucket) // is the key in the table?
      {    // yes! ==> free bucket
	delete bucket;
	return make_tuple(&ret_val->get_key(), true);
      }

    return make_tuple(&ret_val->get_key(), false);
  }

public:

  Key * insert(const Key & key)
  {
    return insert_bucket(new Bucket(key));
  }

  Key * insert(Key && key)
  {
    return insert_bucket(new Bucket(std::forward<Key>(key)));
  }

  Key * search_or_insert(const Key & key)
  {
    return get<0>(search_or_insert_bucket(new Bucket(key)));
  }

  Key * search_or_insert(Key && key)
  {
    return get<0>(search_or_insert_bucket(new Bucket(std::forward<Key>(key))));
  }

  // Retorna true si key ya se encuentra en la tabla. De lo contrario,
  // se inserta key y se retorna false
  pair<Key*, bool> contains_or_insert(const Key & key)
  {
    return search_or_insert_bucket(new Bucket(key));
  }

  pair<Key*, bool> contains_or_insert(Key && key)
  {
    return search_or_insert_bucket(new Bucket(std::forward<Key>(key)));
  }

  Key * add(const Key & key)
  {
    return insert_bucket(new Bucket(key));
  }
 
  Key * add(Key && key)
  {
    return insert_bucket(new Bucket(std::forward<Key>(key)));
  }

  Key * append(const Key & key)
  {
    return insert_bucket(new Bucket(key));
  }
 
  Key * append(Key && key)
  {
    return insert_bucket(new Bucket(std::forward<Key>(key)));
  }

  Key * search(const Key & key) const
  {
    Bucket * bucket = (Bucket*) this->Base::search(key);
    return bucket != nullptr ? &bucket->get_key() : nullptr;
  }

  bool has(const Key & key) const 
  {
    return this->Base::search(key) != nullptr;
  }

  bool contains(const Key & key) const { return has(key); }

  const Key & find(const Key & key) const 
  {
    Bucket * bucket = (Bucket*) this->Base::search(key);
    if (bucket == nullptr)
      throw std::domain_error("Key not found in hash");

    return bucket->get_key();
  }

  Key & find(const Key & key) 
  {
    Bucket * bucket = (Bucket*) this->Base::search(key);

    if (bucket == nullptr)
      throw std::domain_error("Key not found in hash");

    return bucket->get_key();
  }

protected:

  static Bucket * key_to_bucket(Key * key)
  {
    Bucket * ret_val = 0;
    size_t offset = reinterpret_cast<size_t>(&ret_val->get_key());
    
    return reinterpret_cast<Bucket*>(reinterpret_cast<size_t>(key) - offset);
  }

public:

  void remove(Key * key)
  {
    Bucket * bucket = key_to_bucket(key);
    this->Base::remove(bucket);
    delete bucket;
  }

  void remove(const Key & key)
  {
    Bucket * bucket = (Bucket*) this->Base::search(key);
    if (bucket == nullptr)
      throw std::domain_error("Key not in hash table");

    this->Base::remove(bucket);
    delete bucket;
  }

  class Iterator : public Base::Iterator
  { 
  public: 

    using Item_Type = Key;

    using Set_Type = DynHashTable;

    Iterator(const DynHashTable & table) : Base::Iterator(table) {} 

    const Key & get_curr() const
    { 
      return const_cast<Iterator*>(this)->get_curr();
    } 
    
    Key & get_curr() 
    { 
      return this->Base::Iterator::get_curr()->get_key(); 
    } 
    
    void del() { delete this->Base::Iterator::del(); } 
  }; 
};

  template <typename Key, class Cmp = Aleph::equal_to<Key>>
struct DynSetLhash : public DynHashTable<Key, Cmp, LhashTable>
{
  typedef DynHashTable<Key, Cmp, LhashTable> Base;
  using Base::Base;
}; 

  template <typename Key, class Cmp = Aleph::equal_to<Key>>
struct DynSetLinHash : public DynHashTable<Key, Cmp, LinearHashTable>
{
  typedef DynHashTable<Key, Cmp, LinearHashTable> Base;
  using Base::Base;
}; 

template <typename Key, class Cmp = Aleph::equal_to<Key>>
  using DynSetHash = DynHashTable<Key, Cmp, LhashTable>;

template <typename Key, typename Data, 
	  class Cmp = Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>,
	  template <class, class> class HashTable = LhashTable>
class DynMapHashTable 
  : public DynHashTable<std::pair<Key, Data>, Cmp, HashTable>
{
  using Pair = std::pair<Key, Data>;

  using Base = DynHashTable<std::pair<Key, Data>, Cmp, HashTable>;

  using Bucket = typename Base::Bucket;

public:

  static Data & get_data(const Key & key)
  {
    return key_to_pair<Key, Data>(&const_cast<Key&>(key))->second;    
  }

  static const Key & get_key(Data * data_ptr)
  {
    return data_to_pair<Key, Data>(data_ptr)->first;
  }

  using Value_Type = Data;

  // using Base::Base;  // no more need. But I don't remember why I put it
  using Base::insert;
  using Iterator = typename Base::Iterator;

  template <size_t (*fct)(const Key & k)> static
  size_t wrapper(const std::pair<Key, Data> & p)
  {
    return (*fct)(p.first);
  }

  typedef size_t (*Hash_Fct)(const Pair&);

  DynMapHashTable(size_t len, Hash_Fct hash_fct, Cmp & cmp, 
		  float lower_alpha, float upper_alpha)
    : Base(len, hash_fct, cmp, lower_alpha, upper_alpha) {}

  DynMapHashTable(size_t len        = Primes::DefaultPrime, 
		  Hash_Fct hash_fct = wrapper<dft_hash_fct<Key>>, 
		  Cmp && cmp        = Cmp(), 
		  float lower_alpha = hash_default_lower_alpha, 
		  float upper_alpha = hash_default_upper_alpha)
    : DynMapHashTable(len, hash_fct, cmp, lower_alpha, upper_alpha) {}

  Pair * insert(const Key & key, const Data & data)
  {
    return this->insert_bucket(new typename Base::Bucket (Pair(key, data)));
  }

  Pair * insert(const Key & key, Data && data)
  {
    return this->insert_bucket(new typename Base::Bucket(Pair(key, 
							      move(data))));
  }

  Pair * insert(Key && key, Data && data)
  {
    return this->insert_bucket(new typename Base::Bucket(Pair(move(key), 
							      move(data))));
  }

  Pair * insert(Key && key, const Data & data)
  {
    return this->insert_bucket(new typename Base::Bucket(Pair(move(key), 
							      data)));
  }

  Pair * search(const Key & key) const
  {
    return this->Base::search(Pair(key, Data()));
  }

  Pair * search(Key && key) const
  {
    return this->Base::search(Pair(move(key), Data()));
  }

  bool has(const Key & key) const { return search(key) != nullptr; }

  bool has(Key && key) const { return search(move(key)) != nullptr; }

  bool contains(const Key & key) const { return has(key); }

  bool contains(Key && key) const { return has(move(key)); }

  Data & find(const Key & key) 
  {
    return Base::find(Pair(key, Data())).second;
  }

  const Data & find(const Key & key) const 
  {
    return Base::find(Pair(key, Data())).second;
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

  void remove_by_data(Data & data)
  {
    Base::remove(data_to_pair<Key, Data>(&data));
  }

  void remove(const Key & key)
  {
    Base::remove(Pair(key, Data()));
  }

  void remove(Key && key)
  {
    Base::remove(Pair(move(key), Data()));
  }

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
 	  class    Cmp = Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>>
using DynMapLinHash = DynMapHashTable<Key, Data, Cmp, LinearHashTable>;

template <typename Key, typename Data,
 	  class    Cmp = Dft_Pair_Cmp<Key, Data, Aleph::equal_to<Key>>>
using DynMapHash = DynMapHashTable<Key, Data, Cmp, LhashTable>;


// Implementation de coming from ahFunctional.H

template <typename T, template <typename> class Container> inline
DynList<T> join(const Container<T> & c1, const Container<T> & c2)
{
  DynSetLhash<T> table(c1);
  c2.for_each([&table] (const T & item)
	      {
		table.insert(item);
	      });
  return table.keys();
}

template <typename T, template <typename> class Container = DynList> inline
DynList<T> intercept(const Container<T> & c1, const Container<T> & c2)
{
  DynSetLhash<T> set1 = c1;
  DynSetLhash<T> set2 = c2;
  return set1.filter([&set2] (const T & i) { return set2.contains(i); });
}

template <typename T, template <typename> class Container> inline
DynList<T> unique(const Container<T> & c)
{
  return DynSetLhash<T>(c).keys();
}

template <typename T, template <typename> class Container> inline
DynList<T> repeated(const Container<T> & c)
{
  DynList<T> ret;
  DynSetLhash<T> table;
  
  c.for_each([&table, &ret] (const T & i)
    {
      auto * ptr = table.insert(i);
      if (ptr == nullptr)
	ret.append(i);
    });

  return ret;
}

template <typename T, template <typename> class Container> inline
DynList<std::pair<T, size_t>> repeated_with_index(const Container<T> & c)
{
  DynList<std::pair<T, size_t>> ret;
  DynSetLhash<T> table;
  
  size_t i = 0;
  c.for_each([&table, &ret, &i] (const T & item)
    {
      auto * ptr = table.insert(item);
      if (ptr == nullptr)
	ret.append(make_pair(item, i));
      ++i;
    });

  return ret;
}

} // end namespace Aleph

# endif // TPL_DYNSETHASH_H




