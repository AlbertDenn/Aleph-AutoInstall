
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


# ifndef TPL_LINHASH_H
# define TPL_LINHASH_H

# include <iostream>
# include <primes.H>
# include <dlink.H>
# include <tpl_dynArray.H>
# include <tpl_dnode.H>
# include <htlist.H>
# include <hashDry.H>
# include <hash-fct.H>
# include <hash-dry.H>


# ifdef N
# define NBACKUP N
# undef N
# endif

# ifdef M
# define MBACKUP M
# undef M
# endif


using namespace Aleph;

namespace Aleph {

# define LINBUCKET_BODY(BUCKETNAME)					\
									\
  Dlink link;								\
									\
public:									\
									\
 BUCKETNAME(const BUCKETNAME & bucket)					\
 noexcept(std::is_nothrow_constructible<Dnode<Key>>::value)		\
 : Dnode<Key>(bucket) {}						\
									\
 BUCKETNAME() noexcept(std::is_nothrow_constructible<Key>::value) {}	\
									\
 BUCKETNAME(const Key & key) noexcept(noexcept(Dnode<Key>(key)))	\
 : Dnode<Key>(key) {}							\
									\
 Key & get_key() noexcept { return this->get_data(); }			\
									\
 Dlink * get_link() noexcept { return &link; }				\
									\
 DLINK_TO_BASE(BUCKETNAME, link);

 
    template <typename Key>
class LinHashBucket : public Dnode<Key>
{
  LINBUCKET_BODY(LinHashBucket);
};

    template <typename Key>
class LinHashBucketVtl : public Dnode<Key>
{
  LINBUCKET_BODY(LinHashBucketVtl);

  virtual ~LinHashBucketVtl() {}
};

 

    template <typename Key, template <class> class BucketType, 
              class Cmp = Aleph::equal_to<Key>>
class GenLinearHashTable 
      : public HashStats<GenLinearHashTable<Key, BucketType, Cmp>>
{
  friend class HashStats<GenLinearHashTable<Key, BucketType, Cmp>>;

public:

  typedef size_t (*Hash_Fct)(const Key &);

  using Bucket = BucketType<Key>;

  using Key_Type = Key;

  using Item_Type = Key;

private:

  using BucketList = Dnode<Key>;

  using BucketItor = typename Dnode<Key>::Iterator;

  static size_t multiply_by_two(const size_t & n) noexcept { return n << 1; }

  static size_t divide_by_two(const size_t & n) noexcept { return n >> 1; }

  DynArray<BucketList> table;
  Dlink                entries_list;

protected:

  Hash_Fct             hash_fct; // puntero a función hash

private:

  Cmp &               cmp;

  size_t               M; // Tamaño de la tabla
  size_t               N; // Número de elementos que tiene la tabla
  size_t               busy_slots_counter; // Cantidad de entradas ocupadas 
  bool                 remove_all_buckets; // Indica si destructor debe liberar
                                           // las cubetas  

protected:

  float upper_alpha; // factor de carga superior
  float lower_alpha; // factor de carga inferior

private:

  size_t p; // índice de la lista que se particiona (o aumenta)
  size_t l; // cantidad de veces que se ha duplicado la tabla
  size_t MP; // guarda el valor p + M
  size_t MM; // producto 2*M

protected:

  mutable size_t len;

private:

  size_t call_hash_fct(const Key & key) const 
    noexcept(noexcept((*hash_fct)(key)))
  {
    const auto hash = (*hash_fct)(key);
    const auto i = hash % M;
    return i < p ? hash % MM : i;
  }

  void expand()
  { // expandir la tabla hasta que la carga esté debajo de upper_alpha
    for (float alpha = 1.0*N/MP; alpha >= upper_alpha; alpha = 1.0*N/MP)
      {
        BucketList * src_list_ptr = table.test(p);
        if (src_list_ptr != nullptr) // ¿table[p] está escrita?
          if (not src_list_ptr->is_empty()) // ¿table[p] no está vacía'
            {
              BucketList * tgt_list_ptr = nullptr; 

                  // recorrer lista colisiones y mover cubetas de table[p+M] 
              for (BucketItor it(*src_list_ptr); it.has_current(); /* nada */)
                {
                  Bucket * bucket = static_cast<Bucket*>(it.get_current());

                  it.next(); // avance al siguiente elemento de la lista

                  const Key & key = bucket->get_key();
                  const int i = (*hash_fct)(key) % MM;
                  if (i == p) // ¿pertenece esta clave a table[p]?
                    continue; // sí ==> clave sigue en table[p] ==> siguiente

                  if (tgt_list_ptr == nullptr)
                    tgt_list_ptr = &table.touch(MP);

                    // bucket no pertenece a table[p] sino a table[p+m] ==>
                    // eliminar bucket de table[i] e insertarlo en table[p+m]
                  bucket->del();
                  tgt_list_ptr->append(bucket);
                }

              if (src_list_ptr->is_empty()) // ¿table[p] quedó vacía?
                --busy_slots_counter; // sí ==> un slot vacío 

              ++busy_slots_counter; // uno nuevo por table[p+M]
            } 
        ++p;
        ++MP;
        if (p == M) // (p == 2*M) ¿debe duplicarse el tamaño de la tabla?
          {     // sí ==> modificar el tamaño de la tabla a 2*M
            ++l;   // Cantidad de veces que se ha duplicado la tabla
            p = 0;
            MP = M = MM; // se les asigna 2*M
            MM = multiply_by_two(MM);
          }
      } 
  }

  void contract() noexcept
  { // contraer la tabla hasta que carga esté debajo de lower_alpha
    for (float alpha = (1.0*N)/MP; alpha <= lower_alpha and MP > len; 
         alpha = (1.0*N)/MP)
      {
        if (p == 0) // ¿debe dividirse entre 2 el tamaño de la tabla?
          {     // sí ==> actualizar tamaño de la tabla a M/2
            --l; // Cantidad de veces que se ha duplicado la tabla disminuye
            MM = M; // se divide entre dos
            M = divide_by_two(M);
            p = M - 1;
          }
        else
          --p;   // no ==> sólo reducir índice p

        --MP;
        if (MP < table.size()) // ¿Existe table[MP]]? 
          {
            BucketList * src_list_ptr = table.test(MP);
            if (src_list_ptr != nullptr) // ¿existe entrada para table[p+M]?
              {
                if (not src_list_ptr->is_empty()) // ¿table[p+M] está vacía?
                  {     // no ==> fusionar las listas
                    BucketList & tgt_list = table.touch(p);// aparta table[p]
                    tgt_list.concat_list(src_list_ptr);
                    --busy_slots_counter; // table[p+M] devino vacía
                  }
                table.cut(MP); // eventualmente liberar memoria de table[p+M]
              }
          }
      }
  }

public:

  void set_hash_fct(Hash_Fct fct) noexcept
  {
    hash_fct = fct;
  }

  Hash_Fct get_hash_fct() const noexcept { return hash_fct; }

  Cmp & get_compare() noexcept { return cmp; }

  const Cmp & get_compare() const noexcept { return cmp; }

  float current_alpha() const noexcept { return 1.0*N/MP; }

  GenLinearHashTable(size_t __len, Hash_Fct __hash_fct, Cmp & __cmp,
                     float __lower_alpha, float __upper_alpha,
		     bool __remove_all_buckets, 
		     bool __with_resize/* fictitious */)
    throw(std::exception, std::length_error, std::domain_error,
          std::bad_alloc, std::overflow_error) 
  : table(__len), hash_fct(__hash_fct), cmp(__cmp), M(__len), N(0), 
      busy_slots_counter(0), remove_all_buckets(__remove_all_buckets), 
      upper_alpha(__upper_alpha), lower_alpha(__lower_alpha),
      p(0), l(0), MP(M), MM(multiply_by_two(M)), len(__len)
  {
    if (M == 0)
      std::length_error("table's length is zero");

    if (MM > table.max_size())
      throw std::length_error("table's length too big");
      
    if (upper_alpha <= lower_alpha)
      throw std::domain_error("lower alpha is greater than lower alpha");
  }

  GenLinearHashTable(size_t len              = Primes::DefaultPrime, 
		     Hash_Fct hash_fct       = Aleph::dft_hash_fct<Key>,
		     Cmp && cmp              = Cmp(), 
                     float lower_alpha       = hash_default_lower_alpha,
                     float upper_alpha       = hash_default_upper_alpha,
		     bool remove_all_buckets = true,
		     bool with_resize        = true/* fictitious */)
  : GenLinearHashTable(len, hash_fct, cmp, lower_alpha, upper_alpha,
		       remove_all_buckets, with_resize)
  {
    // empty
  }

  void swap(GenLinearHashTable & other) noexcept
  {
    std::swap(table, other.table);
    std::swap(hash_fct, other.hash_fct);
    std::swap(M, other.M);
    std::swap(N, other.N);
    std::swap(busy_slots_counter, other.busy_slots_counter);
    std::swap(remove_all_buckets, other.remove_all_buckets);
    std::swap(upper_alpha, other.upper_alpha);
    std::swap(lower_alpha, other.lower_alpha);
    std::swap(p, other.p);
    std::swap(l, other.l);
    std::swap(MP, other.MP);
    std::swap(MM, other.MM);
    std::swap(len, other.len);
  }



  void empty() noexcept
  {
    while (not entries_list.is_empty())
      {    
	Bucket * bucket = Bucket::dlink_to_base(entries_list.remove_first());
	bucket->del();             // borra de la lista en el arreglo
	bucket->get_link()->del(); // borra de entries list
	delete bucket;
      }
    
    M = MP = len;
    MM = multiply_by_two(M);
    N = p = l = 0;
    table.cut(len);
  }

  ~GenLinearHashTable()
  {
    if (remove_all_buckets)
      empty();
  }

private:

  Bucket *
  search_in_bucket_list(BucketList * list, const Key & key) const noexcept
  {
    for (BucketItor it(*list); it.has_curr(); it.next())
      {
        Bucket * bucket = static_cast<Bucket*>(it.get_current());
        if (cmp (key, bucket->get_key())) 
          return bucket; 
      }

    return nullptr;
  }

public:

  Bucket * search(const Key & key) const noexcept
  {
    auto i = call_hash_fct(key);
    BucketList * list = table.test(i);
    if (list == nullptr) // ¿Ha sido escrita alguna vez table[i]?
      return nullptr; // No ==> el elemento no se encuentra en la tabla

    if (list->is_empty()) 
      return nullptr;

    return search_in_bucket_list(list, key);
  }

  const size_t & size() const noexcept { return N; }

  bool is_empty() const noexcept { return N == 0; }

  const size_t & capacity() const noexcept { return MP; }

  const size_t & busy_slots() const noexcept { return busy_slots_counter; }

  const size_t & expansions() const noexcept { return l; }

  Bucket * insert(Bucket * bucket) 
  {
    const int i       = call_hash_fct(bucket->get_key());
    BucketList & list = table.touch(i); // allocates memory for table[i]
    if (list.is_empty())
      ++busy_slots_counter;

    if (search_in_bucket_list(&list, bucket->get_key()) != nullptr)
      return nullptr; // duplicated key

    list.append(bucket);
    entries_list.append(bucket->get_link());
    ++N;
    expand();

    return bucket;
  }

  Bucket * search_or_insert(Bucket * bucket)
  {
    const int i       = call_hash_fct(bucket->get_key());
    BucketList & list = table.touch(i); // allocates memory for table[i]
    if (list.is_empty())
      ++busy_slots_counter;

    Bucket * p = search_in_bucket_list(&list, bucket->get_key());
    if (p != nullptr)
      return p; // duplicated key

    list.append(bucket);
    entries_list.append(bucket->get_link());
    ++N;
    expand();

    return bucket;
  }

  size_t resize(size_t) noexcept { return MP; }

private:

  // This routine deletes bucket from hash table EXCEPT from
  // entries_list. The end of this routine is to dry the deletion and to
  // allow remove from other places; ofor instance, from the del()
  // method of Iterator class
  Bucket * remove_bucket(Bucket * bucket) noexcept
  {
    assert(bucket != nullptr);
    assert(search(bucket->get_key()) == bucket);

    Bucket * next = static_cast<Bucket*>(bucket->get_next());

    bucket->del();             // elimine de lista de colisiones

    if (next->is_empty()) // ¿lista de colisiones vacía?
      --busy_slots_counter; // sí ==> un slot vacío

    --N;
    contract();

    return bucket;
  }

public:

  Bucket * remove(Bucket * bucket) noexcept
  {
    bucket->get_link()->del(); // elimine de entries_list
    return remove_bucket(bucket);
  }

  void print()
  {
    for (int i = 0; i < MP; ++i)
      {
        cout << "table[" << i << "] = [ ";
        
        if (table.exist(i))
          {
            BucketList & list = table.access(i);

            if (not list.is_empty())
              for (BucketItor it(list); it.has_current(); it.next())
                {
                  Bucket * bucket = static_cast<Bucket*>(it.get_current());
                  const Key & key = bucket->get_key();
                  cout << key << ",";
                }
          }
        cout << "]" << endl;
      }
  }

  class Iterator : public Dlink::Iterator
  {
  private:

    GenLinearHashTable * hash_table = nullptr;
    long pos = 0;

  public:

    using Set_Type = GenLinearHashTable;

    using Item_Type = Bucket *;

    Iterator(const GenLinearHashTable & table) noexcept
      : Dlink::Iterator(const_cast<Dlink &>(table.entries_list)), 
	hash_table(& const_cast<GenLinearHashTable &>(table))
    {
      // Empty
    }
    
    Iterator() noexcept { /* Empty */ }

    Bucket * get_curr() 
    {
      return Bucket::dlink_to_base(this->Dlink::Iterator::get_curr());
    }

    Bucket * del()
    {
      Bucket * bucket = Bucket::dlink_to_base(this->Dlink::Iterator::del());
      return (Bucket *) hash_table->remove_bucket(bucket);
    }

    void next() 
    {
      this->Dlink::Iterator::next();
      pos++;
    }

    void prev()
    {
      this->Dlink::Iterator::prev();
      pos--;
    }

    long get_pos() const noexcept { return pos; }
  };
};


template <typename Key, class Cmp = Aleph::equal_to<Key>>
using LinearHashTable = GenLinearHashTable<Key, LinHashBucket, Cmp>;


  template <typename Key, class Cmp = Aleph::equal_to<Key>>
using LinearHashTableVtl = GenLinearHashTable<Key, LinHashBucketVtl, Cmp>;


} // end namespace Aleph

# ifdef NBACKUP
# define N NBACKUP
# undef NBACKUP
# endif

# ifdef MBACKUP
# define M MBACKUP
# undef MBACKUP
# endif

# endif // TPL_LINHASH_H

