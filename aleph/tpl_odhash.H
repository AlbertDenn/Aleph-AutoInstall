
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


# ifndef TPL_ODHASH_H
# define TPL_ODHASH_H

# include <primes.H>
# include <dlink.H>
# include <ahDry.H>
# include <hash-dry.H>
# include <hashDry.H>
# include <hash-fct.H>

using namespace Aleph;

namespace Aleph {

# ifdef N
# define NBACKUP N
# undef N
# endif

# ifdef M
# define MBACKUP M
# undef M
# endif


  template <typename Key, class Cmp = Aleph::equal_to<Key>>
class ODhashTable
    : public OhashCommon<ODhashTable<Key, Cmp>, Key>,
      public GenericTraverse<ODhashTable<Key, Cmp>>,
      public LocateFunctions<ODhashTable<Key, Cmp>, Key>,
      public FunctionalMethods<ODhashTable<Key, Cmp>, Key>,
      public EqualToMethod<ODhashTable<Key, Cmp>>,
      public StlAlephIterator<ODhashTable<Key, Cmp>>
{
  friend class OhashCommon<ODhashTable<Key, Cmp>, Key>;

  public:

  using Key_Type = Key;

  using Item_Type = Key;

  using Hash_Fct = size_t (*) (const Key&);

  enum Status { EMPTY, BUSY, DELETED };

  enum Probe { NO_PROBED, FIRST_PROBE, SECOND_PROBE, LINEAR_PROBE };

  struct Bucket
  {
    Key      key;               // clave
    unsigned status     : 4;    // status EMPTY, DELETED o BUSY
    unsigned probe_type : 4;    // FIRST_PROBE SECOND_PROBE LINEAR_PROBE 
    unsigned int probe_counter; // contador de sondeos

    Bucket() noexcept : status(EMPTY), probe_type(NO_PROBED), probe_counter(0)
    { /* empty */ }

    void reset() noexcept // put all as when constructed
    {
      status = EMPTY;
      probe_type = NO_PROBED;
      probe_counter = 0;
    }
  };

  Bucket * table           = nullptr; // arreglo de cubetas
  Hash_Fct hash_fct        = nullptr; // primera funci�n hash
  Hash_Fct second_hash_fct = nullptr; // segunda funci�n hash

  Cmp & cmp;

protected:

  size_t len;                       // tama�o de la tabla
  float  lower_alpha;
  float  upper_alpha;

private:

  size_t   N;                       // n�mero de cubetas ocupadas
  bool     with_resize;

  Bucket * allocate_bucket(Bucket & bucket, unsigned char probe_type) noexcept
  {
    assert(bucket.status != BUSY);

    ++N;
    bucket.status     = BUSY;
    bucket.probe_type = probe_type;
    bucket.probe_counter++;

    return &bucket;
  }

  void decrease_probe_counter(Bucket * bucket) noexcept
  {
    assert(bucket->status == BUSY or bucket->status == DELETED);

    bucket->probe_counter--;
    if (bucket->probe_counter == 0) // <marcar EMPTY sobre la cubeta?
	bucket->status = EMPTY;
  }

  void deallocate_bucket(Bucket * bucket) noexcept
  {
    assert(bucket->status == BUSY);

    bucket->status = DELETED;
    const Key & key = bucket->key;

    const size_t i_fst = (*hash_fct)(key) % len;
    if (&table[i_fst] == bucket)
      {
	assert(Cmp () (table[i_fst].key, key)); 
	assert(table[i_fst].probe_type == FIRST_PROBE);
      }
    else
      {
	const size_t i_snd = (*second_hash_fct)(key) % len;
	if (&table[i_snd] == bucket)
	  {
	    assert(Cmp () (table[i_snd].key, key));
	    assert(table[i_snd].probe_type == SECOND_PROBE);
	    decrease_probe_counter(&table[i_fst]);
	  }
	else
	  {
	    decrease_probe_counter(&table[i_fst]);
	    decrease_probe_counter(&table[i_snd]);
	    size_t i = i_snd;
	    for (index_forward(i); &table[i] != bucket; index_forward(i))
	      {
		assert(table[i].status != EMPTY);
		decrease_probe_counter(&table[i]);
	      }
	    assert(Cmp () (table[i].key, key));
	    assert(table[i].probe_type == LINEAR_PROBE);
	  }
      }
    
    decrease_probe_counter(bucket);
    --N;
  }

  size_t index_forward(size_t & i) const noexcept
  {
    assert(i < len);
    if (++i == len)
      i = 0;
    return i;
  }

  size_t index_backward(size_t & i) const noexcept
  {
    assert(i < len);
    if (i-- == 0)
      i = len - 1;
    return i;
  }

  static Bucket * key_to_bucket(Key * rec) noexcept
  {
    Bucket * ret_val = 0;
    const long offset = (long) &ret_val->key;
    return (Bucket*) ((long) rec - offset); 
  }

  bool is_valid_bucket(Bucket * bucket) noexcept
  {
    if (bucket < &table[0] or bucket >= &table[len])
      return false;

    const int offset_with_base = (char*) bucket - (char*) &table[0];
    return offset_with_base % sizeof(*bucket) == 0;
  }

  size_t bucket_to_index(Bucket * bucket) noexcept
  {
    assert(is_valid_bucket(bucket));
    return bucket - &table[0];
  }

public:

  const Cmp & get_compare() const { return cmp; }

  Cmp & get_compare() { return cmp; }

  void swap(ODhashTable & other) noexcept
  {
    std::swap(table, other.table);
    std::swap(hash_fct, other.hash_fct);
    std::swap(second_hash_fct, other.second_hash_fct);
    std::swap(cmp, other.cmp);
    std::swap(N, other.N);
    std::swap(len, other.len);
  }

  ODhashTable(size_t  __len, Hash_Fct __first_hash_fct,
	      Hash_Fct __second_hash_fct, Cmp & __cmp,
 	      float __lower_alpha, float  __upper_alpha, bool __with_resize)
    : table(nullptr), hash_fct(__first_hash_fct), 
      second_hash_fct(__second_hash_fct), cmp(__cmp), 
      len(Primes::next_prime(__len)),  
      lower_alpha(__lower_alpha), upper_alpha(__upper_alpha),
      N(0), with_resize(__with_resize)
  {
    table = new Bucket[len];
  }

  ODhashTable(size_t len               = Primes::DefaultPrime,
	      Hash_Fct first_hash_fct  = Aleph::dft_hash_fct<Key>, 
	      Hash_Fct second_hash_fct = Aleph::snd_hash_fct<Key>, 
	      Cmp && cmp               = Cmp(),
 	      float lower_alpha        = hash_default_lower_alpha,
	      float upper_alpha        = hash_default_upper_alpha,
	      bool  with_resize        = true)
    : ODhashTable(len, first_hash_fct, second_hash_fct, cmp, 
		  lower_alpha, upper_alpha, with_resize)
  {
    // empty
  }

  ~ODhashTable()
  {
    if (table != nullptr)
      delete [] table;
  }

  ODhashTable(const ODhashTable & other)
    : ODhashTable(other.len, other.hash_fct, other.second_hash_fct, other.cmp,
		  other.lower_alpha, other.upper_alpha, other.with_resize)
  {
    assert(table != nullptr);
    this->copy_from_table(other);
  }

  ODhashTable(ODhashTable && other) noexcept : ODhashTable(other)
  {
    assert(table != nullptr);
    swap(other);
  }

  Special_Ctors(ODhashTable, Key);

  ODhashTable & operator = (const ODhashTable & other)
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
	hash_fct        = other.hash_fct;
	second_hash_fct = other.second_hash_fct;
	cmp             = other.cmp;
	lower_alpha     = other.lower_alpha;
	upper_alpha     = other.upper_alpha;
	with_resize     = other.with_resize;
      }

    this->copy_from_table(other);

    return *this;
  }

  ODhashTable & operator = (ODhashTable && other) noexcept
  {
    swap(other);
    return *this;
  }

  OHASH_COMMON(ODhashTable);

  Key * search(const Key & key) const noexcept
  {
    const size_t i_fst = (*hash_fct)(key) % len; // 1er sondeo (1ra fun hash)
    if (table[i_fst].status == EMPTY) 
      return nullptr;

    if (table[i_fst].status == BUSY and cmp(table[i_fst].key, key))
      {
	assert(table[i_fst].probe_type == FIRST_PROBE);
	assert(table[i_fst].probe_counter > 0);
	return &table[i_fst].key;
      }

    const size_t i_snd = (*second_hash_fct)(key) % len; // 2do sondeo

    if (table[i_snd].status == EMPTY)
      return nullptr;

    if (table[i_snd].status == BUSY and cmp(table[i_snd].key, key)) 
      {
	assert(table[i_snd].probe_type == SECOND_PROBE);
	assert(table[i_snd].probe_counter > 0);
	return &table[i_snd].key;
      }

    size_t i = i_snd; 
        // sondeo lineal a partir de �ndice de 2da funci�n hash
    for (int count = 0; count < len; ++count)
      {
	index_forward(i);
	switch (table[i].status)
	  {
	  case EMPTY:
	    assert(table[i].probe_counter == 0);
	    return nullptr;
	  case BUSY:
	    assert(table[i].probe_counter > 0);
	    if (cmp(table[i].key, key))
	      {
		assert(table[i].probe_type == LINEAR_PROBE);
		return &table[i].key;
	      }
	    break;
	  case DELETED:
	    assert(table[i].probe_counter > 0);
	    break;
	  default: ERROR("ODhashTable search: inconsistent bucket status");
	  }
      }
   
    return nullptr; 
  }

  Hash_Fct get_second_hash_fct() const noexcept { return second_hash_fct; }

  void set_second_hash_fct(Hash_Fct fct) noexcept
  {
    second_hash_fct = fct;
  }

private:

  Bucket * allocate_bucket(const Key & key) noexcept
  {
    assert(N < len);

    const size_t i_fst = (*hash_fct)(key) % len; // sondeo con 1ra funci�n hash
    if (table[i_fst].status != BUSY) // �cubeta disponible?
      return allocate_bucket(table[i_fst], FIRST_PROBE);

    if (cmp(table[i_fst].key, key)) // test if key is alredy present
      return nullptr;

    const size_t i_snd = (*second_hash_fct)(key) % len; // 2do hash
    if (table[i_snd].status != BUSY) // �cubeta disponible?
      {
	table[i_fst].probe_counter++;
	return allocate_bucket(table[i_snd], SECOND_PROBE);
      }

    if (cmp(table[i_snd].key, key)) // test if key is alredy present
      return nullptr;

    size_t i = i_snd;
    for (int c = 0; c < len; ++c)
      {	 
	index_forward(i);
	switch (table[i].status)
	{
	case BUSY: 
	  if (cmp(table[i].key, key))
	    { // duplicated key ==> rollback previous probe_counter increases
	      for (int k = 0; k < c; ++k)
		{
		  index_backward(i);
		  table[i].probe_counter--;
		}
	      return nullptr;
	    }
	  break;
	case DELETED: 
	case EMPTY:
	  table[i_fst].probe_counter++;
	  table[i_snd].probe_counter++;
	  return allocate_bucket(table[i], LINEAR_PROBE); 
	default: ERROR("ODhashTable: Invalid bucket status"); break;
	}
	table[i].probe_counter++;
      }

    return nullptr;
  }

  // search key. If found return true, otherwise false and allocates the bucket
  tuple<Bucket*, bool> hard_allocate_bucket(const Key & key) noexcept
  {
    assert(N < len);

    const size_t i_fst = (*hash_fct)(key) % len; // sondeo con 1ra funci�n hash
    if (table[i_fst].status != BUSY) // �cubeta disponible?
      return make_tuple(allocate_bucket(table[i_fst], FIRST_PROBE), false);

    if (cmp(table[i_fst].key, key)) // test if key is alredy present
      return make_tuple(&table[i_fst], true);

    const size_t i_snd = (*second_hash_fct)(key) % len; // 2do hash
    if (table[i_snd].status != BUSY) // �cubeta disponible?
      {
	table[i_fst].probe_counter++;
	return make_tuple(allocate_bucket(table[i_snd], SECOND_PROBE), false);
      }

    if (cmp(table[i_snd].key, key)) // test if key is alredy present
      return make_tuple(&table[i_snd], true);

    size_t i = i_snd;
    for (int c = 0; c < len; ++c)
      {	 
	index_forward(i);
	switch (table[i].status)
	  {
	  case BUSY: 
	    if (cmp(table[i].key, key))
	      { // duplicated key ==> rollback previous probe_counter increases
		const size_t idx = i;
		for (int k = 0; k < c; ++k)
		  {
		    index_backward(i);
		    table[i].probe_counter--;
		  }
		return make_tuple(&table[idx], true);
	      }
	    break;
	  case DELETED: 
	  case EMPTY:
	    table[i_fst].probe_counter++;
	    table[i_snd].probe_counter++;
	    return make_tuple(allocate_bucket(table[i], LINEAR_PROBE), false);
	  default: ERROR("ODhashTable: Invalid bucket status"); break;
	  }
	table[i].probe_counter++;
      }
    assert(false); // never must reach this point
    return make_tuple((Bucket*)nullptr, false);
  }

  void remove_bucket(Bucket * bucket) 
  {
    if (not is_valid_bucket(bucket))
      throw std::invalid_argument("key pty does not belong to hash table");

    if (bucket->status != BUSY)
      throw std::domain_error("Bucket containing key is not BUSY");

    deallocate_bucket(bucket);
  }

  // returns true if the bucket contains the key
  bool process_entry_for_remove(Bucket * bucket, const Key & key) 
  {
    switch (bucket->status)
      {
      case EMPTY:
	throw std::domain_error("Key not in hash table");
      case BUSY:
	if (cmp(bucket->key, key))
	  {
	    bucket->status = DELETED;
	    decrease_probe_counter(bucket);
	    return true;
	  }
      case DELETED:
	decrease_probe_counter(bucket);
	break;
      default: ERROR("Inconsistent bucket status");
      }
    return false;
  }

public:

  void remove(const Key & key)
  {
    --N;
    try
      {
	const size_t i_fst = (*hash_fct)(key) % len;
	if (process_entry_for_remove(&table[i_fst], key))
	  return;
    
	const size_t i_snd = (*second_hash_fct)(key) % len;
	if (process_entry_for_remove(&table[i_snd], key))
	  return;

	size_t i = i_snd;
	for (int c = 0; i < len; ++c)
	  {
	    index_forward(i);
	    if (process_entry_for_remove(&table[i], key))
	      return;
	  }
      }
    catch (...)
      {
	++N;
	this->rehash(); // in order to leave the table in a consistent state
	throw;
      }
  }
  
  using Stats = typename OhashCommon<ODhashTable<Key, Cmp>, Key>::Stats;

  Stats stats()	const							
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
	    size_t count = 1;
	    const size_t i_fst = (*hash_fct)(key) % len;
	    if (cmp(table[i_fst].key, key))
		{
		  assert(table[i_fst].probe_type == FIRST_PROBE);
		  assert(table[i_fst].probe_counter > 0);
		  ;
		}
	    else 
	      {
		++count;
		size_t i_snd = (*second_hash_fct)(key) % len;
		if (cmp(table[i_snd].key, key))
		{
		  assert(table[i_snd].probe_type == SECOND_PROBE);
		  assert(table[i_snd].probe_counter > 0);
		  ;
		}
		else
		  {
		    for (size_t i = index_forward(i_snd); true; 
			 index_forward(i))
		      {
			if (table[i].status == BUSY and cmp(table[i].key, key))
			  break;
			++count;
		      }
		  }
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
using SetODhash = ODhashTable<Key, Cmp>;


# ifdef NBACKUP
# define N NBACKUP
# undef NBACKUP
# endif

# ifdef MBACKUP
# define M MBACKUP
# undef MBACKUP
# endif

# undef EMPTY
# undef BUSY
# undef DELETED
# undef NO_PROBED    
# undef FIRST_PROBE  
# undef SECOND_PROBE 
# undef LINEAR_PROBE 

} // end namespace Aleph

# endif // TPL_ODHASH_H

