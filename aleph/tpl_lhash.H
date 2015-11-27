
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


# ifndef TPL_LHASH_H
# define TPL_LHASH_H

# include <iostream>
# include <primes.H>
# include <dlink.H>
# include <htlist.H>
# include <tpl_dynArray.H>
# include <hashDry.H>
# include <hash-fct.H>
# include <tpl_dynArray.H>
# include <hash-dry.H>

# ifdef N
# define NBACKUP N
# undef N
# endif

# ifdef M
# define MBACKUP M
# undef M
# endif

using namespace Primes;

using namespace Aleph;

namespace Aleph {

    template <typename Key, class BucketType, class Cmp> 
class GenLhashTable : public HashStats<GenLhashTable<Key, BucketType, Cmp>>
{
  friend class HashStats<GenLhashTable<Key, BucketType, Cmp>>;

public:

  using Bucket = BucketType;

  typedef size_t (*Hash_Fct)(const Key &);

  using Key_Type = Key;

  using Item_Type = Key;

protected:

  Hash_Fct hash_fct;

private:

  using BucketList = Dnode<Key>;// Tipo lista cubetas
  using BucketItor = typename Dnode<Key>::Iterator; // Iterador cubetas
  using Node = Dnode<Key>;      // Sinónimo nodo

  BucketList *                          table;

protected:

  size_t len;                  // Tamaño de la tabla
  Cmp &  cmp;
  float  lower_alpha;
  float  upper_alpha;

private:

  size_t N;                  // Número de elementos de la tabla
  size_t busy_slots_counter; // Cantidad de entradas ocupadas
  bool remove_all_buckets;   // liberar cubetas en destructor
  bool with_resize;

public:

  Cmp & get_compare() { return cmp; }

  const Cmp & get_compare() const { return cmp; }

  GenLhashTable(size_t table_size, Hash_Fct __hash_fct, Cmp & __cmp,
		float __lower_alpha, float __upper_alpha, 
                bool __remove_all_buckets, bool __with_resize) 
    throw(std::exception, std::bad_alloc) 
    : hash_fct(__hash_fct), table(nullptr),
      len(Primes::next_prime(table_size)), cmp(__cmp),
      lower_alpha(__lower_alpha), upper_alpha(__upper_alpha),
      N(0), busy_slots_counter(0), remove_all_buckets(__remove_all_buckets),
      with_resize(__with_resize)
  { 
    table = new BucketList [len];
  }

  GenLhashTable(size_t   table_size  = Primes::DefaultPrime,
		Hash_Fct hash_fct    = Aleph::dft_hash_fct<Key>, 
		Cmp &&   cmp         = Cmp(),
		float    lower_alpha = hash_default_lower_alpha,
		float    upper_alpha = hash_default_upper_alpha,
                bool     remove_all_buckets = true,
		bool     with_resize        = true) 
    throw(std::exception, std::bad_alloc) 
    : GenLhashTable(table_size, hash_fct, cmp, lower_alpha, upper_alpha,
		    remove_all_buckets, with_resize)
    {
      // empty
    }

  void swap(GenLhashTable & other) noexcept
  {
    std::swap(hash_fct, other.hash_fct);
    std::swap(table, other.table);
    std::swap(len, other.len);
    std::swap(cmp, other.cmp);
    std::swap(N, other.N);
    std::swap(busy_slots_counter, other.busy_slots_counter);
    std::swap(remove_all_buckets, other.remove_all_buckets);
    std::swap(with_resize, other.with_resize);
  }

  void empty() noexcept
  {
    for (int i = 0; i < len; ++i)
      for (BucketItor itor(table[i]); itor.has_curr(); /* nada */)
        delete (Bucket*) itor.del();
    busy_slots_counter = N = 0;
  }

private:

  Bucket * 
  search_in_bucket_list(BucketList & list, const Key & key) const noexcept
  {
    for (BucketItor it(list); it.has_curr(); it.next())
      {
        Bucket * bucket = static_cast<Bucket*>(it.get_current());
        if (cmp (key, bucket->get_key())) 
          return bucket; 
      }

    return nullptr;
  }

public:

  Hash_Fct get_hash_fct() const noexcept { return hash_fct; }

  void set_hash_fct(Hash_Fct fct) noexcept
  {
    hash_fct = fct;
  }

  float current_alpha() const noexcept { return 1.0*N/len; }

  Bucket * insert(Bucket * bucket) noexcept
  {
    const auto i = (*hash_fct)(bucket->get_key()) % len;
    auto & list = table[i];

    if (list.is_empty()) // ¿está vacía la lista table[i]?
      ++busy_slots_counter;  // sí ==> actualizar contador ocupación

    if (search_in_bucket_list(list, bucket->get_key()) != nullptr)
      return nullptr;

    list.append(bucket); // insertar cubeta al final
    ++N;

    if (with_resize and current_alpha() >= upper_alpha)
      resize(next_prime(2*len));

    return bucket;
  }

  // Busca bucket->key. Si la encuentra, retorna la dirección de bucket
  // dentro de la tabla. De lo contrario inserta y retorna bucket
  Bucket * search_or_insert(Bucket * bucket) noexcept
  {
    const auto i = (*hash_fct)(bucket->get_key()) % len;
    auto & list = table[i];

    if (list.is_empty()) // ¿está vacía la lista table[i]?
      ++busy_slots_counter;  // sí ==> actualizar contador ocupación

    auto * p = search_in_bucket_list(list, bucket->get_key());
    if (p != nullptr)
      return p;

    list.append(bucket); // insertar cubeta al final
    ++N;

    if (with_resize and current_alpha() >= upper_alpha)
      resize(next_prime(2*len));

    return bucket;
  }

  Bucket * search(const Key & key) const noexcept
  {
    const auto i = (*hash_fct)(key) % len;
    return search_in_bucket_list(table[i], key);
  }

private:

  // Remove without perform resizing. This is strictly required inside
  // the del() method of Iterator. In addtion dries the code
  Bucket * remove_bucket(Bucket * bucket) noexcept
  {     // guarda próxima colisión
    Bucket * next = static_cast<Bucket*>(bucket->get_next());
    bucket->del(); // eliminar de su lista de colisiones
    if (next->is_empty()) // ¿la lista devino vacía?
      --busy_slots_counter;// sí ==> actualizar contador listas ocupadas

    --N;

    return bucket;
  }

public:

  Bucket * remove(Bucket * bucket) noexcept
  {     // guarda próxima colisión
    remove_bucket(bucket);

    if (with_resize and current_alpha() < lower_alpha)
      resize(next_prime(len/2));

    return bucket;
  }

  size_t resize(size_t new_size) throw(std::exception, std::bad_alloc)
  {
    assert(len > 0);

    if (new_size == len or new_size == 0)
      return len; 

    BucketList * new_table = new BucketList [new_size];
    BucketList * old_table = table; // guardar estado  tabla actual
    const size_t old_size  = len;
    table                  = new_table; // tabla vacía con nuevo arreglo
    len                    = new_size;
    busy_slots_counter     = N = 0;

    for (int i = 0; i < old_size; ++i) // reinsertar cubetas
          // recorrer lista colisiones en old_table[i]
      for (BucketItor it(old_table[i]); it.has_curr(); /* Nada */)
        insert((Bucket*) it.del()); // eliminar e insertar en table[]

    delete [] old_table; // liberar memoria antigua tabla

    return len;
  }

  virtual ~GenLhashTable()
  {
    if (remove_all_buckets)
      empty();

    delete [] table; 
  }

  Bucket * search_next(Bucket * bucket) const noexcept
  {
    assert(bucket != nullptr);

    const auto i = (*hash_fct)(bucket->get_key()) % len;

    BucketItor itor(table[i]); 
    itor.set(bucket);

    while (true)
      {
        itor.next();

        if (not itor.has_curr()) 
          return nullptr;

        Bucket * node = static_cast<Bucket*>(itor.get_curr());

        if (cmp(bucket->get_key(), node->get_key())) 
          return node;
      }
  }

  const size_t & capacity() const noexcept { return len; }

  const size_t & size() const noexcept { return N; }

  const size_t & 
  get_num_busy_slots() const noexcept { return busy_slots_counter; }

  bool is_empty() const noexcept { return N == 0; }

  class Iterator
  {
  private:

    long            curr_index = -1; // índice en table
    long            curr_pos   = 0;
    BucketItor      curr_itor;  // Iterador sobre table[curr_index]
    GenLhashTable * hash_table = nullptr; 

        // Avanza a próxima entrada de table
    void locate_next_available_entry() 
    {
      while (true)
        {
          if (curr_index == hash_table->len)
            throw std::overflow_error ("hash table iterator overflow");

          if (curr_index == hash_table->len - 1)
            {  // quedar en estado overflow
              curr_index = hash_table->len;
              return;
            }

          ++curr_index;

          if (not hash_table->table[curr_index].is_empty())
            {
              BucketItor itor(hash_table->table[curr_index]);
              curr_itor = itor;
              return;
            }
        }
    }

    void locate_next_available_bucket() 
    {
      curr_itor.next();

      if (not curr_itor.has_curr())
        locate_next_available_entry();
      curr_pos++;
    }

    void locate_prev_available_entry() 
    {
      while (true)
        {
          if (curr_index == -1)
            throw std::underflow_error ("hash table iterator underflow");

          if (curr_index == 0)
            { // quedar en estado underflow
              curr_index = -1;
              return;
            }

          --curr_index;

          if (not hash_table->table[curr_index].is_empty())
            {
              BucketItor itor(hash_table->table[curr_index]);
              curr_itor = itor;
              curr_itor.reset_last();
              return;
            }
        }
    }
    
    void locate_prev_available_bucket() 
    {
      curr_itor.prev();

      if (not curr_itor.has_curr())
        locate_prev_available_entry();
      curr_pos--;
    }

public:

    using Set_Type = GenLhashTable;

    using Item_Type = Bucket *;

    Iterator(const GenLhashTable & table) noexcept
      : hash_table(&const_cast<GenLhashTable&>(table))
    {
      try
        {
          locate_next_available_entry();
        }
      catch (std::overflow_error)
        { /* nothing to do */ }
    }
    
    Iterator() noexcept { /* Empty */ }

    void reset_first() noexcept
    {
      curr_index = -1;
      curr_pos = 1;
      locate_next_available_entry();
    }

    void reset_last() noexcept
    { 
      curr_index = hash_table->len;
      curr_pos = hash_table->N - 1;
      locate_prev_available_entry();
    }

    void end() noexcept
    {
      reset_last();
      next();
    }

    bool has_curr() const noexcept
    {
      return curr_index >= 0 and curr_index < hash_table->len;
    }
    
    Bucket * get_curr() 
      throw(std::exception, std::overflow_error, std::underflow_error)
    {
      if (curr_index == -1)
        throw std::underflow_error ("hash table iterator underflow");

      if (curr_index == hash_table->len) 
        throw std::overflow_error ("hash table iterator overflow");

      return static_cast<Bucket*>(curr_itor.get_curr());
    }

    long get_pos() const noexcept { return curr_pos; }

    void next() 
    {
      if (curr_index == hash_table->len) 
        throw std::overflow_error ("hash table iterator overflow");

      locate_next_available_bucket();
    }    

    void prev() 
    {
      if (curr_index == -1) 
        throw std::underflow_error ("hash table iterator underflow");

      locate_prev_available_bucket();
    }    

    Bucket * del()
    {
      Bucket * ret_val = get_curr();
      next();
      hash_table->remove_bucket(ret_val);
      return ret_val;
    }
  };
};

template <typename Key> using LhashBucket = Dnode<Key>;


template <typename Key> 
struct LhashBucketVtl : public Dnode<Key>
{
  using Base = Dnode<Key>;
  using Base::Base;

  virtual ~LhashBucketVtl() {}
};

    template <typename Key, class Cmp = Aleph::equal_to<Key>> 
struct LhashTable : public GenLhashTable<Key, LhashBucket<Key>, Cmp>
{
  using Base = GenLhashTable<Key, LhashBucket<Key>, Cmp>;
  using Base::Base;
};

    template <typename Key, class Cmp = Aleph::equal_to<Key> > 
struct LhashTableVtl : public GenLhashTable<Key, LhashBucketVtl<Key>, Cmp>
{
  using Base = GenLhashTable<Key, LhashBucketVtl<Key>, Cmp>;
  using Base::Base;
};


} // end namespace Aleph

# ifdef NBACKUP
# define N NBACKUP
# undef NBACKUP
# endif

# ifdef MBACKUP
# define M MBACKUP
# undef MBACKUP
# endif

# endif/* TPL_LHASH_H */

