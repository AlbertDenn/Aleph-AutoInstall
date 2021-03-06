
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/


# ifndef PROTECTED_ODHASH_H
# define PROTECTED_ODHASH_H

# include <ahExceptions.H>
# include <tpl_odhash.H>
# include <useMutex.H>

    template <class Key, class Record>
class Protected_OdHashTable
{
  struct Bucket
  {
    Record record;
    bool   locked;

    Bucket() : locked(false) { /* empty */ }

    Bucket(const Record& _record) : 
      record(_record),
      locked(false) 
    { /* empty */ }
  };

  static Bucket * record_to_bucket(Record * record)
  {
    Bucket * ret_val = 0;
    size_t offset = reinterpret_cast<size_t>(&ret_val->record);

    return 
      reinterpret_cast<Bucket*>(reinterpret_cast<size_t>(record) - offset);
    }

  ODhashTable<Key, Bucket> hash_table;

  pthread_mutex_t mutex;

public:

  typedef const unsigned long (*HashFctType)(const Key&);

  const bool is_locked(Record * record)
  {
    Bucket * bucket = record_to_bucket(record);

    return bucket->locked;
  }

  void lock(Record * record)
  {
    Bucket * bucket = record_to_bucket(record);
    bucket->locked = true;
  }

  void unlock(Record * record)
  {
    Bucket * bucket = record_to_bucket(record);
    bucket->locked = false;
  }

  const size_t get_table_size() 
  {
    CRITICAL_SECTION(mutex);

    return hash_table.get_table_size();
  }

  const size_t get_num_items() 
  {
    CRITICAL_SECTION(&mutex);

    return hash_table.get_num_items();
  }

  Protected_OdHashTable (HashFctType    _first_hash_fct, 
			 HashFctType    _second_hash_fct, 
			 const size_t & len) :
    hash_table (_first_hash_fct, _second_hash_fct, len)
  {
    init_mutex(mutex);
  }

  ~Protected_OdHashTable()
  {
    destroy_mutex(mutex);
  }
   
  Record* insert(const Key & key, const Record & record)
  {
    Bucket bucket(record);

    CRITICAL_SECTION(mutex);

    Bucket * ret_val = hash_table.insert(key, bucket);

    return &ret_val->record;
  }

  Record* search(const Key & key)
  {
    CRITICAL_SECTION(mutex);

    Bucket * bucket = hash_table.search(key);

    if (bucket == nullptr)
      return nullptr;

    if (bucket->locked)
      Throw (Lock) ();

    return &bucket->record;
  }

  void remove(Record * record) 
  {
    Bucket * bucket = record_to_bucket(record);

    if (bucket->locked)
      Throw (Lock) ();

    CRITICAL_SECTION(mutex);

    hash_table.remove(bucket);
  }

  void remove_all_and_delete()
  {
    CRITICAL_SECTION(mutex);

    hash_table.remove_all_and_delete();
  }

  const int get_table_size() const 
  { 
    CRITICAL_SECTION(mutex);
    return hash_table.get_table_size(); 
  }

  class Iterator
  {
  private:

    typename ODhashTable<Key, Bucket>::Iterator itor;
    UseMutex                                    critical_section;
    
  public:

    Iterator(Protected_OdHashTable & table) :
      itor(table.hash_table), CTOR_USE_MUTEX(critical_section, table.mutex)
    {
      // mutex will remain locked until destructor be performed
    }

    const bool has_current() const { return itor.has_current(); }

    Record * get_current() { return &(itor.get_current()->record); }

    void next() { itor.next(); }

    void prev() { itor.prev(); }
  };


};


# endif //  PROTECTED_ODHASH_H

