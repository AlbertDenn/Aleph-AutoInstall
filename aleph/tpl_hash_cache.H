
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


# ifndef TPL_HASH_CACHE_H
# define TPL_HASH_CACHE_H

# include <memory>
# include <aleph.H>
# include <tpl_dnode.H>
# include <tpl_lhash.H>

namespace Aleph {


  template <typename  Key, typename Data, class Cmp = Aleph::equal_to<Key>>
class Hash_Cache
{

public:

  class Cache_Entry : public LhashTable<Key, Cmp>::Bucket
  { 

    friend class Hash_Cache<Key, Data>;

    Data    data; 
    Dlink   dlink_lru; // enlace a la cola lru 
    Dlink* link_lru() { return &dlink_lru; }
    bool locked; // indica si la entrada está trancada
    bool is_in_hash_table; // indica si entrada está contenida dentro 
                           // de tabla hash 
    void lock() 
    { 
      if (locked)
        throw std::runtime_error("Cache_Entry is already locked");

      locked = true; 
    }

    void unlock()
    { 
      if (not locked)
        throw std::runtime_error("Cache_Entry is not locked");

      locked = false; 
    }
    Dlink   dlink_inside; // enlace a la lista de entradas que 

    Dlink * link_inside() { return &dlink_inside; }

    LINKNAME_TO_TYPE(Cache_Entry, dlink_inside);

  public:
    Cache_Entry() : locked(false), is_in_hash_table(false) { /* empty */ }


    Data & get_data() { return data; }
    const bool & is_locked() const { return locked; }

    const bool & is_in_table() const { return is_in_hash_table; }
  }; // fin class Cache_Entry

private:

  LhashTable<Key, Cmp> hash_table;
  LINKNAME_TO_TYPE(Cache_Entry, dlink_lru);
  Dlink    lru_list; // cabecera de la lista lru
  size_t   num_lru;  // número de elementos en lista lru
  Dlink  inside_list; // lista de cubetas apartadas y metidas en tabla
  size_t cache_size;  // máx número de entradas que puede tener cache
  Dlink  locked_list; // lista de entradas trancadas
  size_t num_locked;  // número de elementos trancados
  typedef Dnode<Cache_Entry*> Chunk_Descriptor;
  Chunk_Descriptor chunk_list;  
  void insert_entry_to_lru_list(Cache_Entry * cache_entry)
  {
    num_lru++;
    lru_list.insert(cache_entry->link_lru());
  }
  void remove_entry_from_lru_list(Cache_Entry * cache_entry)
  {
    num_lru--;
    cache_entry->link_lru()->del();
  }

  void insert_entry_to_locked_list(Cache_Entry * cache_entry)
  {
    num_locked++;
    locked_list.insert(cache_entry->link_lru());
  }

  void remove_entry_from_locked_list(Cache_Entry * cache_entry)
  {
    num_locked--;
    cache_entry->link_lru()->del();
  }

  void move_to_inside_front(Cache_Entry * cache_entry)
  {
    cache_entry->link_inside()->del();
    inside_list.insert(cache_entry->link_inside());
  }

  void do_mru(Cache_Entry * cache_entry) 
  { 
    cache_entry->link_lru()->del(); // elimine de posición actual
    lru_list.insert(cache_entry->link_lru()); // llévela a trasero
  }
  void do_lru(Cache_Entry * cache_entry) 
  { 
    cache_entry->link_lru()->del(); // elimine de posición actual
    lru_list.append(cache_entry->link_lru()); // llévela al frente
  }
  void remove_entry_from_hash_table(Cache_Entry * cache_entry)
  {
    cache_entry->link_inside()->del();
    hash_table.remove(cache_entry);
    cache_entry->is_in_hash_table = false;
    do_lru(cache_entry);
  }
  Cache_Entry * get_lru_entry()
  {

    if (lru_list.is_empty())   // ¿existe una entrada disponible?
      throw std::underflow_error("All entries are locked"); // no ==> ¡excepción! 

      // obtenga  entrada más antigua; menos recientemente accedida
    Dlink * lru_entry_link = lru_list.get_prev();
    Cache_Entry * cache_entry = dlink_lru_to_Cache_Entry(lru_entry_link);

        // si cache_entry contenida en tabla ==> eliminarlo
    if (cache_entry->is_in_hash_table) 
      remove_entry_from_hash_table(cache_entry);

    do_mru(cache_entry); // entrada deviene más recientemente accedida

    return cache_entry;
  }

public:

  Hash_Cache(size_t (*hash_fct)(const Key&), 
             size_t __hash_size, const size_t & __cache_size) 
    throw (std::exception, std::bad_alloc)
    : hash_table(__hash_size, hash_fct),
      num_lru(0), cache_size(__cache_size), num_locked(0)
  {
        // apartar entradas del cache
    Cache_Entry * entries_array = new Cache_Entry [cache_size];

    try 
    { 


          // apartar el descriptor del arreglo
    std::unique_ptr<Chunk_Descriptor> 
        chunk_descriptor (new Chunk_Descriptor (entries_array));
    chunk_list.insert(chunk_descriptor.get());

        // insertar cada Cache_Entry en lista lru
    for (int i = 0; i < cache_size; i++) 
      insert_entry_to_lru_list(&entries_array[i]);

    chunk_descriptor.release();

    }
    catch (...)
      {
        delete [] entries_array;
        throw;
      }

  }

  virtual ~Hash_Cache() 
  {      // recorrer lista de bloques y liberarlos
    while (not chunk_list.is_empty())
      {
        Chunk_Descriptor * current_chunk = chunk_list.remove_next();

        delete [] current_chunk->get_data();
        delete current_chunk;
      }
  }      

  Cache_Entry * insert(const Key & key, const Data & data) 
  {
    Cache_Entry * cache_entry = get_lru_entry(); // entrada más antigua
    cache_entry->get_key()    = key;             // escribirle el par
    cache_entry->get_data()   = data;
    inside_list.insert(cache_entry->link_inside());
    hash_table.insert(cache_entry);            
    cache_entry->is_in_hash_table = true;
    return cache_entry;
  }
  Cache_Entry * search(const Key & key)
  {     // buscar en la tabla hash
    Cache_Entry * cache_entry = (Cache_Entry*) hash_table.search(key);
    if (cache_entry != nullptr) // ¿fue encontrada la clave?
      {     // sí ==> hacerla la más recientemente usada
        do_mru(cache_entry);
        move_to_inside_front(cache_entry);
      } 
    return cache_entry;
  }

  Cache_Entry * search_next(Cache_Entry * cache_entry)
  {
    Cache_Entry *next_entry = 
      static_cast<Cache_Entry*>(hash_table.search_next(cache_entry));
    if (next_entry != nullptr)
      {
        do_mru(next_entry);
        move_to_inside_front(cache_entry);
      } 
    return next_entry;
  }

  void lock_entry(Cache_Entry * cache_entry) 

    throw(std::exception, std::runtime_error, std::domain_error)

  {

    if (cache_entry->is_locked())
      throw std::runtime_error("Cache_Entry is already locked");

    if (not cache_entry->is_in_table())
      throw std::domain_error("Cache_Entry is not in the cache");

    remove_entry_from_lru_list(cache_entry);
    insert_entry_to_locked_list(cache_entry);
    cache_entry->lock();
  }
  void unlock_entry(Cache_Entry * cache_entry) 

    throw(std::exception, std::runtime_error)

  { 

    if (not cache_entry->is_locked())
      throw std::runtime_error("Cache_Entry is not locked");

    remove_entry_from_locked_list(cache_entry);
    insert_entry_to_lru_list(cache_entry);
    cache_entry->unlock();
  }
  void remove(Cache_Entry * cache_entry) 

    throw(std::exception, std::runtime_error, std::domain_error)

  {

    if (cache_entry->is_locked())
      throw std::runtime_error("Cache_Entry is already locked");
    if (not cache_entry->is_in_table())
      throw std::domain_error("Cache_Entry is not in the cache");

    remove_entry_from_hash_table(cache_entry);
  }
  void expand(const size_t & plus_size) 

    throw(std::exception, std::range_error, std::bad_alloc)

  {

    if (plus_size == 0)
      throw std::range_error ("bad plus_size");

    const size_t new_cache_size = cache_size + plus_size;

        // apartar plus_size nuevas entradas
    Cache_Entry * entries_array = new Cache_Entry [plus_size];

    try
      {

    std::unique_ptr<Chunk_Descriptor>     // apartar el descriptor
      chunk_descriptor (new Chunk_Descriptor (entries_array));

       // Calcular nuevo tamaño de tabla y relocalizar sus entradas  
    const float curr_hash_ratio = 1.0*cache_size/hash_table.capacity();
    const size_t new_hash_capacity = new_cache_size/curr_hash_ratio;

    hash_table.resize(new_hash_capacity);

        // meter nuevas entradas en lru_list
    for (int i = 0; i < plus_size; i++) 
      insert_entry_to_lru_list(&entries_array[i]);

    chunk_list.insert(chunk_descriptor.release());
    cache_size = new_cache_size;

      }
    catch (...)
      {
        delete []  entries_array;
        throw;
      }

  }


  const size_t & capacity() const { return cache_size; }

  const size_t & size() const { return hash_table.size(); }

  const size_t & get_num_locked() const { return num_locked; }

  const size_t & get_num_busy_slots() const 
  { 
    return hash_table.get_num_busy_slots(); 
  }

  const size_t & get_hash_capacity() const 
  {
    return hash_table.capacity();
  }

  class Iterator : public Dlink::Iterator
  {

  public:
    typedef Hash_Cache Set_Type;
    typedef Cache_Entry * Item_Type;

    Iterator(Hash_Cache & cache) : Dlink::Iterator(&cache.inside_list) {}

    Cache_Entry * get_current()  
    { 
      Dlink * dl = Dlink::Iterator::get_current();
      return Cache_Entry::dlink_inside_to_Cache_Entry(dl); 
    } 
  };
};


} // end namespace Aleph

# endif // TPL_HASH_CACHE_H 

