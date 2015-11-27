
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


# ifndef TPL_OLHASH_H
# define TPL_OLHASH_H

# include <iostream>
# include <primes.H>
# include <dlink.H>
# include <ahDry.H>
# include <hash-dry.H>
# include <hashDry.H>
# include <hash-fct.H>


using namespace Primes;

using namespace Aleph;

# ifdef N
# define NBACKUP N
# undef N
# endif

# ifdef M
# define MBACKUP M
# undef M
# endif

namespace Aleph {

  template <typename Key, class Cmp = Aleph::equal_to<Key>>
class OLhashTable
    : public OhashCommon<OLhashTable<Key, Cmp>, Key>,
      public GenericTraverse<OLhashTable<Key, Cmp>>,
      public LocateFunctions<OLhashTable<Key, Cmp>, Key>,
      public FunctionalMethods<OLhashTable<Key, Cmp>, Key>,
      public EqualToMethod<OLhashTable<Key, Cmp>>,
      public StlAlephIterator<OLhashTable<Key, Cmp>>
{
  friend class OhashCommon<OLhashTable<Key, Cmp>, Key>;

public:

  using Key_Type = Key;

  using Item_Type = Key;

  using Hash_Fct = size_t (*) (const Key&);

  enum Status { EMPTY, BUSY, DELETED };

  struct Bucket
  {
    Key key;
    char status = EMPTY;

    Bucket() noexcept : status(EMPTY) {}
    void reset() noexcept { status = EMPTY; }
  };

  static Bucket * key_to_bucket(Key * rec) noexcept
  {
    Bucket * ret_val = 0;
    long offset = (long) &ret_val->key;
    return (Bucket*) ((long) rec - offset); 
  }
  
  Bucket *    table = nullptr;
  size_t      N = 0;

protected:

  size_t len;
  float  lower_alpha;
  float  upper_alpha;
  Cmp &  cmp;

private:

  Hash_Fct hash_fct;
  bool     with_resize;

  bool is_valid_bucket(Bucket * bucket) const noexcept
  {
    if (bucket < &table[0] or bucket >= &table[len]) 
      return false;

    int offset_with_base = (char*) bucket - (char*) &table[0];

    return offset_with_base % sizeof(*bucket) == 0;
  }

public:

  const Cmp & get_compare() const { return cmp; }

  Cmp & get_compare() { return cmp; }

  OLhashTable(size_t __len, Hash_Fct __hash_fct, Cmp & __cmp,
 	      float __lower_alpha, float __upper_alpha, bool __with_resize)
    : table(nullptr), N(0), len(Primes::next_prime(__len)),
      lower_alpha(__lower_alpha), upper_alpha(__upper_alpha), cmp(__cmp),
      hash_fct(__hash_fct), with_resize(__with_resize)
  {
    table = new Bucket [len];
  }

  OLhashTable(size_t len        = Primes::DefaultPrime,
	      Hash_Fct hash_fct = Aleph::dft_hash_fct<Key>, 
	      Cmp && cmp        = Cmp(),
 	      float lower_alpha = hash_default_lower_alpha,
	      float upper_alpha = hash_default_upper_alpha,
	      bool with_resize  = true)
    : OLhashTable(len, hash_fct, cmp, lower_alpha, upper_alpha, with_resize)
    {
      // empty
    }

  OLhashTable(size_t len, Hash_Fct hash_fct, Hash_Fct, 
 	      const float & lower_alpha, const float & upper_alpha,
	      const bool with_resize)
    : OLhashTable(len, hash_fct, lower_alpha, upper_alpha, with_resize)
  {
    // empty
  }

  Special_Ctors(OLhashTable, Key);

  ~OLhashTable()
  {
    if (table != nullptr)
      delete [] table;
  }

  void swap(OLhashTable & other) noexcept
  {
    std::swap(table, other.table);
    std::swap(N, other.N);
    std::swap(len, other.len);
    std::swap(cmp, other.cmp);
    std::swap(hash_fct, other.hash_fct);
    std::swap(lower_alpha, other.lower_alpha);
    std::swap(upper_alpha, other.upper_alpha);
    std::swap(with_resize, other.with_resize);
  }

  OLhashTable(const OLhashTable & other)
    : OLhashTable(other.len, other.hash_fct, other.cmp,
		  other.lower_alpha, other.upper_alpha, other.with_resize)
  {
    this->copy_from_table(other);
  }

  OLhashTable(OLhashTable && other) noexcept : OLhashTable(other)
  {
    swap(other);
  }

  OLhashTable & operator = (const OLhashTable & other)
  {
    if (this == &other)
      return *this;

    if (len > other.N)
      this->clean_table();
    else
      {
	Bucket * new_table = new Bucket [other.len];
	delete [] table;
	table = new_table;
	N = 0;
	len = other.len;
	hash_fct = other.hash_fct;
	cmp = other.cmp;
	lower_alpha = other.lower_alpha;
	upper_alpha = other.upper_alpha;
      }

    this->copy_from_table(other);

    return *this;
  }

  OLhashTable & operator = (OLhashTable && other) noexcept
  {
    swap(other);
    return *this;
  }

  Key * search(const Key & key) const noexcept
  {   
    long i = (*hash_fct)(key) % len, c = 0;
    while (c < len and table[i].status != EMPTY)
      {
	if (table[i].status == BUSY and cmp(table[i].key, key))
	  return &table[i].key;

	++c;
	if (++i == len)
	  i = 0;
      }

    return nullptr; // No se encuentra la clave
  }

protected:

  Bucket * allocate_bucket(const Key & key) noexcept
  {
    assert(N < len);

    size_t i = (*hash_fct)(key) % len; 

    while (table[i].status == BUSY) // find a slot that is not busy
      {
	if (cmp(key, table[i].key))
	  return nullptr;

	if (++i == len)
	  i = 0;
      }

    Bucket * bucket = &table[i];

        // i contiene celda con DELETED o EMPTY ==> ocuparla
    bucket->status = BUSY;
    N++;

    return bucket;
  }

  tuple<Bucket*, bool> hard_allocate_bucket(const Key & key) noexcept
  {
    assert(N < len);

    size_t i = (*hash_fct)(key) % len; 
    while (table[i].status == BUSY) // find a slot that is not busy
      {
	if (cmp(key, table[i].key))
	  return make_tuple(&table[i], true);

	if (++i == len)
	  i = 0;
      }

    Bucket * bucket = &table[i];

        // i contiene celda con DELETED o EMPTY ==> ocuparla
    bucket->status = BUSY;
    N++;

    return make_tuple(bucket, false);
  }

  void deallocate_bucket(Bucket * bucket) noexcept
  {
    if (not is_valid_bucket(bucket))
      throw std::invalid_argument("record address is not inside table's range");

    if (bucket->status != BUSY)
      throw std::domain_error("Bucket containing record is not busy");

    --N;
    int i = bucket - &table[0]; // índice de brecha
    table[i].status = DELETED;
  }

  public:

  
  void remove(const Key & key)
  {
    Key * key_ptr = search(key);
    if (key_ptr == nullptr)
      throw std::domain_error("Key not in hash table");

    this->remove_ptr(key_ptr);
  }

  OHASH_COMMON(OLhashTable);

  using Stats = typename OhashCommon<OLhashTable<Key, Cmp>, Key>::Stats;

  Stats stats() const
  {									
    DynArray<size_t> lens;						
    size_t num_busy = 0, num_deleted = 0, num_empty = 0;			
    size_t max_len = std::numeric_limits<size_t>::min();			
    for (int i = 0; i < len; ++i)					
      switch (table[i].status)						
	{								
	case BUSY:							
	  {								
	    ++num_busy;							
	    const Key & key = table[i].key;				
	    long i = (*hash_fct)(key) % len;				
	    size_t count = 1;						
									
	    while (true)							
	      {								
		if (table[i].status == BUSY and cmp(table[i].key, key)) 
		  break;							
		++count;							
		if (++i == len)						
		  i = 0;							
	      }								
									
	    max_len = std::max(max_len, count);				
	    update_stat_len(lens, count);				
	    break;							
	  }								
	case EMPTY:							
	  ++num_empty;							
	  update_stat_len(lens, 0);					
	  break;								
	case DELETED:							
	  ++num_deleted;							
	  break;								
	}								
 									
    float avg = 0, sum = 0;						
    for (int i = 0; i < lens.size(); ++i)				
      {									
	avg += lens(i)*i;						
	sum += lens(i);							
      }									
									
    avg /= sum;								
    float var = 0;							
    for (int i = 0; i < lens.size(); ++i)				
      {									
	float s = i - avg;						
	var += lens(i)*s*s;						
      }									
    var /= sum;								
   									
    Stats stats;								
    stats.num_busy = num_busy;						
    stats.num_deleted = num_deleted;					
    stats.num_empty = num_empty;						
    std::swap(lens, stats.lens);						
    stats.avg = avg;							
    stats.var = var;							
    stats.max_len = max_len;						
									
    return stats;						
  }
};



template <typename Key, class Cmp = Aleph::equal_to<Key>>
using SetOLhash = OLhashTable<Key, Cmp>;

								
}
# endif // TPL_OLHASH_H

