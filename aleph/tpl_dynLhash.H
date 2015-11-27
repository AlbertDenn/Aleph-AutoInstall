
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


# ifndef TPL_DYNLHASH_H
# define TPL_DYNLHASH_H

# include <tpl_lhash.H>

using namespace Aleph;

namespace Aleph {

  template <typename Key, typename Record, class Cmp = Aleph::equal_to<Key>>
class DynLhashTable : public LhashTable<Key>
{
private:

  struct DLBucket : public LhashTable<Key>::Bucket
  {
    Record record;

    DLBucket(const Key & key, const Record & _record)
      : LhashTable<Key>::Bucket(key), record(_record) { /* Empty */ }

    
    DLBucket(const Key & key, Record && _record)
      : LhashTable<Key>::Bucket(key), record(std::forward<Record>(_record)) 
    { /* Empty */ }

    DLBucket(Key && key, const Record & _record)
      : LhashTable<Key>::Bucket(std::forward<Key>(key)), record(_record) 
    { /* Empty */ }
    
    DLBucket(Key && key, Record && _record)
      : LhashTable<Key>::Bucket(std::forward<Key>(key)),
	record(std::forward<Record>(_record)) { /* Empty */ }
  };

  static DLBucket * record_to_bucket(Record * rec)
  {
    DLBucket * ret_val = 0;
    size_t offset = (size_t) &ret_val->record;
    return (DLBucket*) (((size_t) rec) - offset);
  }

  class DLProxy
  {
    DynLhashTable & table;
    const Key &     key;
    DLBucket *      bucket;

  public:

    DLProxy(DynLhashTable & _table, const Key& _key)
      : table(_table), key(_key)
    {
      Record * record = table.search(key);
      bucket = record not_eq nullptr ? record_to_bucket(record) : nullptr;
    }

    operator const Record & () const 
      throw(std::exception, std::invalid_argument)
    {
      if (bucket == nullptr)
        throw std::invalid_argument("access to unexisting entry");
    
      return bucket->record;
    }

    DLProxy& operator = (const Record& record) 
      throw(std::exception, std::bad_alloc) 
    {
      if (bucket != nullptr)
        {
          bucket->record = record;
          return *this;
        }

      bucket = new DLBucket (key, record);
      table.LhashTable<Key>::insert(bucket);
      return *this;
    }

    DLProxy& operator = (const DLProxy& proxy) 
      throw(std::exception, std::bad_alloc, std::invalid_argument)
    {
      if (proxy.bucket == nullptr)
        throw std::invalid_argument("access to unexisting entry");

      if (bucket != nullptr)
        {
          bucket->record = proxy.bucket->record;
          return *this;
        }

      bucket = new DLBucket (key, proxy.bucket->record);
      table.LhashTable<Key>::insert(bucket);
      return *this;
    }
  };

public:

  typedef typename DynLhashTable<Key, Record>::Hash_Fct Hash_Fct;

  void swap(DynLhashTable & table) 
  {
    this->LhashTable<Key>::swap(table); 
  }

  DynLhashTable(size_t len        = DefaultPrime, 
		Hash_Fct hash_fct = dft_hash_fct<Key>) 
    throw (std::exception, std::bad_alloc) 
    : LhashTable<Key>(len, hash_fct)
  {
    // Empty 
  }

private:

  void copy(DynLhashTable & table)
  {
    for (typename LhashTable<Key>::Iterator it(table); it.has_curr(); it.next())
      {
	DLBucket * Bucket = (DLBucket*) it.get_curr();
	insert(Bucket->get_key(), Bucket->record);
      }
  }

public:

  DynLhashTable(const DynLhashTable & table)
    : LhashTable<Key>(table.hash_fct, table.len)
  {
    copy(table);
  }

  DynLhashTable(DynLhashTable && table)
  {
    swap(table);
  }

  DynLhashTable & operator = (const DynLhashTable & table)
  {
    if (this == &table)
      return *this;

    this->empty();
    copy(table);

    return *this;
  }

  DynLhashTable & operator = (DynLhashTable && table)
  {
    swap(table);
    return *this;
  }

private:

  Record * __insert(DLBucket * bucket)
  {
    LhashTable<Key>::insert(bucket);
    return &bucket->record;
  }

public:

  Record * insert(const Key & key, const Record & record)
    throw (std::exception, std::bad_alloc)
  {
    return __insert(new DLBucket (key, record));
  }

  Record * insert(const Key & key, Record && record = Record())
    throw (std::exception, std::bad_alloc)
  {
    return __insert(new DLBucket(key, std::forward<Record>(record)));
  }

  Record * insert(Key && key, const Record & record)
    throw (std::exception, std::bad_alloc)
  {
    return __insert(new DLBucket (std::forward<Key>(key), record));
  }

  Record * insert(Key && key, Record && record)
    throw (std::exception, std::bad_alloc)
  {
    return __insert(new DLBucket(std::forward<Key>(key), 
				 std::forward<Record>(record)));
  }

  Record * search(const Key & key)
  {
    DLBucket * bucket = (DLBucket*) LhashTable<Key>::search(key);
    return bucket != nullptr ? &bucket->record : nullptr;
  }

  void remove(Record * record)
  {
    DLBucket* bucket = record_to_bucket(record);
    LhashTable<Key>::remove(bucket);
    delete bucket;
  }

  DLProxy operator [] (const Key& key) const
    throw(std::exception, std::invalid_argument) 
  {
    return DLProxy ( const_cast<DynLhashTable&>(*this), key);
  }

  DLProxy operator [] (const Key& key) 
    throw(std::exception, std::bad_alloc, std::invalid_argument) 
  {
    return DLProxy (*this, key);
  }
};

} // end namespace Aleph

# endif // TPL_DYNLHASH_H

