
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


# ifndef HASHDRY_H
# define HASHDRY_H

template <class HashTbl, typename Key>
class OhashCommon
{
  HashTbl * me() { return static_cast<HashTbl*>(this); }

  const HashTbl * const_me() const { return static_cast<const HashTbl*>(this); }

public:

  void copy_from_table(const HashTbl & other)
  {
    assert(me()->N == 0 and me()->len >= other.N);
    for (long i = 0, c = 0; c < other.N; ++i)
      {
	auto & bucket = other.table[i];
	if (bucket.status != HashTbl::BUSY)
	  continue;

	me()->insert(bucket.key);
	++c;
      }
    assert(me()->N == other.N);
  }

  void clean_table()
  {
    for (long i = 0; i < me()->len; ++i)
      me()->table[i].reset();
    me()->N = 0;
  }

  Key * insert(const Key & key)
  {
    auto bucket = me()->allocate_bucket(key);
    if (bucket == nullptr)
     return nullptr;

   bucket->key = key;

   return me()->test_resize(bucket, key);
 }

 Key * insert(Key && key)
 {
   auto bucket = me()->allocate_bucket(key);
   if (bucket == nullptr)
     return nullptr;

   std::swap(bucket->key, key);

   return me()->test_resize(bucket, bucket->key);
 }

  Key * search_or_insert(const Key & key)
  {
    auto r = me()->hard_allocate_bucket(key);
    auto bucket = get<0>(r);
    if (get<1>(r))
      return &bucket->key;

    bucket->key = key;
    
    return me()->test_resize(bucket, bucket->key);
  }

  Key * search_or_insert(Key && key)
  {
    auto r = me()->hard_allocate_bucket(key);
    auto bucket = get<0>(r);
    if (get<1>(r))
      return &bucket->key;

    std::swap(bucket->key, key);

    return me()->test_resize(bucket, bucket->key);
  }

  bool contains_or_insert(const Key & key)
  {
    auto r = me()->hard_allocate_bucket(key);
    auto bucket = get<0>(r);
    if (get<1>(r))
      return true;

    bucket->key = key;
    me()->test_resize(bucket, bucket->key);

    return false;
  }

 bool contains_or_insert(Key && key)
 {
   auto r = me()->hard_allocate_bucket(key);
   auto bucket = get<0>(r);
   if (get<1>(r))
     return true;

   std::swap(bucket->key, key);
   me()->test_resize(bucket, bucket->key);

   return false;
 }

  Key * append(const Key & key)
  {
    return this->insert(key);
  }

  Key * append(Key && key)
  {
    return this->insert(std::forward<Key>(key));
  }

  bool has(const Key & key) const noexcept
  {
    return const_me()->search(key) != nullptr;
  }

  bool contains(const Key & key) const noexcept { return has(key); }

  Key & find(const Key & key)
  {
    auto key_ptr = me()->search(key);
    if (key_ptr == nullptr)
      throw std::domain_error("Key not found in hash");

    return *key_ptr;
  }

  const Key & find(const Key & key) const
  {
    return const_cast<OhashCommon*>(this)->find(key);
  }

  const Key & operator [] (const Key & key) const
  {
    return find(key);
  }

  const Key & operator [] (const Key & key)
  {
    return *search_or_insert(key);
  }

  void remove_ptr(Key * key)
  {
    auto bucket = HashTbl::key_to_bucket(key);
    me()->deallocate_bucket(bucket);

    if (me()->with_resize and me()->current_alpha() < me()->lower_alpha)
      resize(Primes::next_prime(me()->len/2 + 1));
  }

  size_t resize(const size_t & new_size)
  {
    assert(me()->len > 0);

    if (new_size == 0 or new_size == me()->len)
      return me()->len;

    if (me()->N > new_size)
      throw std::overflow_error ("New size is not enough for current" 
				 " number of entries"); 

    auto new_table   = new typename HashTbl::Bucket [new_size];
    auto old_table   = me()->table;
    const size_t old_len = me()->len;

    int old_N = me()->N;

    me()->table = new_table;
    me()->len   = new_size;
    me()->N     = 0;

    for (int i = 0, c = 0; i < old_len and c < old_N; ++i)
      if (old_table[i].status == HashTbl::BUSY)
	{
	  Key & key = old_table[i].key;
	  auto bucket = me()->allocate_bucket(key);
	  std::swap(bucket->key, key);
	  ++c;
	}

    assert(old_N == me()->N);

    delete [] old_table;

    return me()->len;
  }

  void rehash()
  {
    auto new_table = new typename HashTbl::Bucket [me()->len];
    auto old_table = me()->table;

    int old_N = me()->N;

    me()->table = new_table;
    me()->N     = 0;

    for (int i = 0, c = 0; i < me()->len and c < old_N; ++i)
      if (old_table[i].status == HashTbl::BUSY)
	{
	  Key & key = old_table[i].key;
	  auto bucket = me()->allocate_bucket(key);
	  std::swap(bucket->key, key);
	  ++c;
	}

    assert(old_N == me()->N);

    delete [] old_table;
  }

  void empty()
  {
    delete [] me()->table;
    me()->N = 0;
    me()->len = Primes::DefaultPrime;
    me()->table = new typename HashTbl::Bucket [me()->len];
  }

  size_t size() const noexcept { return const_me()->N; }

  bool is_empty() const noexcept { return const_me()->N == 0; }

  size_t capacity() const noexcept { return const_me()->len; }

  class Iterator
  {
    HashTbl * table_ptr = nullptr;
    long curr_idx          = 0;
    long ordinal           = 0;

    bool check() const
    {
      assert(table_ptr != nullptr);
      return (curr_idx >= 0 and curr_idx < (long) table_ptr->len) and
	(ordinal >= -1 and ordinal <= (long) table_ptr->size());
    }

    void locate_next_available_entry()
    {
      assert(check());

      if (ordinal >= table_ptr->size())
	throw std::overflow_error("Iterator next() overflow"); 

      if (++ordinal == table_ptr->size())
	return;

      while (table_ptr->table[++curr_idx].status != HashTbl::BUSY);
    }

    void locate_prev_available_entry()
    {
      assert(check());

      if (ordinal <= -1)
	throw std::underflow_error("Iterator next() underflow");

      if (--ordinal == -1)
	return;

      while (table_ptr->table[--curr_idx].status != HashTbl::BUSY);
    }

  public:

    void reset_first() noexcept
    {
      assert(table_ptr != nullptr);
      if (table_ptr->is_empty())
	{
	  curr_idx = table_ptr->len;
	  ordinal = 0;
	  return;
	}

      while (table_ptr->table[curr_idx].status != HashTbl::BUSY)
	++curr_idx;

      ordinal = 0;
      assert(check());
    }

    void reset_last() noexcept
    {
      assert(table_ptr != nullptr);
      if (table_ptr->is_empty())
	{
	  curr_idx = -1;
	  ordinal = -1;
	  return;
	}

      curr_idx = table_ptr->len - 1;
      while (table_ptr->table[curr_idx].status != HashTbl::BUSY)
	--curr_idx;

      ordinal = table_ptr->size() - 1;
      assert(check());
    }

    void end() noexcept
    {
      reset_last();
      next();
    }

    long get_pos() const noexcept { return ordinal; }

    Key & get_curr()
    {
      assert(check());

      if (ordinal == table_ptr->size())
	throw std::overflow_error("O hash::Iterator next() overflow");

      if (ordinal == -1)
	throw std::underflow_error("O hash::Iterator next() underflow");

      assert(table_ptr->table[curr_idx].status == HashTbl::BUSY);

      return table_ptr->table[curr_idx].key;
    }

    const Key & get_curr() const
    {
      return const_cast<Iterator*>(this)->get_curr();
    }

    Key & get_current() { return get_curr(); }

    const Key & get_current() const { return get_curr(); }

    bool has_curr() const noexcept
    {
      assert(table_ptr != nullptr);
      if (table_ptr->is_empty())
	return false;
      return ordinal >= 0 and ordinal < table_ptr->size();
    }

    bool has_current() const noexcept { return has_curr(); }

    void next() { locate_next_available_entry(); }

    void prev() { locate_prev_available_entry(); }

    Iterator(const HashTbl & table) noexcept
    : table_ptr(&const_cast<HashTbl&>(table)), curr_idx(0), ordinal(-1)
    {
      reset_first();
    }

    void del()
    {
      if (not has_curr())
	throw std::overflow_error("Overflow in del() of iterator");

      table_ptr->deallocate_bucket(&table_ptr->table[curr_idx]);
      if (table_ptr->size() == 0)
	return;

      while (table_ptr->table[++curr_idx].status != HashTbl::BUSY);
    }
  };

  DynList<Key> keys() const
  {
    return const_me()->template map<Key> ([] (const Key & key) { return key; });
  }

  auto items() const { return keys(); }

  struct Stats
  {
    size_t num_busy;
    size_t num_deleted;
    size_t num_empty;
    DynArray<size_t> lens;
    float avg, var;
    size_t max_len;

    Stats() : max_len(std::numeric_limits<size_t>::min())
      {
	assert(lens.size() == 0);
      }
  };

  void print_stats(const Stats & stats) const
  {
    std::cout << "M             = " << this->capacity() << std::endl
	      << "N             = " << this->size() << std::endl
	      << "busy slots    = " << stats.num_busy << std::endl
	      << "deleted slots = " << stats.num_deleted << std::endl
	      << "empty slots   = " << stats.num_empty << std::endl
	      << "alpha         = " << const_me()->current_alpha() << std::endl
	      << "max lenght    = " << stats.max_len << std::endl;
    for (int i = 0; i < stats.lens.size(); ++i)
      cout << "    " << i << " = " << stats.lens(i) << endl;
  }

  float current_alpha() const noexcept 
  {
    return 1.0*const_me()->N/const_me()->len; 
  }
};


template <class HashTbl>
class HashStats
{
  HashTbl * me() { return static_cast<HashTbl*>(this); }

  const HashTbl * const_me() const { return static_cast<const HashTbl*>(this); }

  float  lower_alpha;
  float  upper_alpha;
  size_t busy_slots_counter = 0;
  size_t N = 0;

public:

  struct Stats
  {
    float avg;
    float var;
    DynArray<size_t> lens;
  };

private:

  static void update_stat_len(DynArray<size_t> & lens, size_t i) noexcept
  {
    if (lens.exist(i))
      lens(i) += 1;
    else
      lens.touch(i) = 1;
  }
  
public:

  Stats stats() const
  { 
    DynArray<size_t> lens;
    for (int i = 0; i < const_me()->capacity(); ++i)
      {
	Dlink list = const_me()->table[i];
	size_t count = 0;
	for (typename HashTbl::BucketItor it(list); it.has_curr(); 
	     it.next(), ++count)
	  ;

	update_stat_len(lens, count);
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
    stats.avg = avg;
    stats.var = var;
    std::swap(lens, stats.lens);

    return stats;
  }

  void print_stats(const Stats & stats) const
  {
    cout << "M          = " << const_me()->capacity() << endl
	 << "N          = " << const_me()->size() << endl
	 << "busy slots = " << busy_slots_counter << endl
	 << "Average    = " << stats.avg << endl
	 << "Desv       = " << sqrt(stats.var) << endl
	 << "alpha      = " << (1.0*const_me()->size())/const_me()->capacity() 
	 << endl;
    for (int i = 0; i < stats.lens.size(); ++i)
      cout << "    " << i << " = " << stats.lens(i) << endl;
  }

  void set_upper_alpha(const float &  __upper_alpha)
  {
    if (__upper_alpha <= lower_alpha)
      throw std::domain_error("upper_alpha lower than lower_alpha");
    
    upper_alpha = __upper_alpha;
  }

  void set_lower_alpha(const float &  __lower_alpha)
  {
    if (__lower_alpha >= upper_alpha)
      throw std::domain_error("lower_alpha greater than upper_alpha");

    lower_alpha = __lower_alpha;
  }

  float get_lower_alpha() const noexcept { return lower_alpha; }

  float get_upper_alpha() const noexcept { return upper_alpha; }
};


# endif // HASHDRY_H
