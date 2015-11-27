
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


# include <gsl/gsl_rng.h>

# include <cassert>
# include <iostream>
# include <aleph.H>
# include <tpl_dynArray.H>
# include <tpl_sort_utils.H>
# include <tpl_olhash.H>
# include <tpl_odhash.H>

using namespace std;

static unsigned long tbl_size = 0;

gsl_rng * r = NULL;

# define DEFAULT_N 100

template <template <typename,class> class HashTable, typename Key,
	  class Cmp = Aleph::equal_to<Key>> 
HashTable<Key, Cmp> create_table(const HashTable<Key, Cmp> & s)
{
  typedef HashTable<Key, Cmp> Hset;
  Hset ret_val;
  for (typename Hset::Iterator it(s); it.has_curr(); it.next())
    ret_val.insert(it.get_curr());
  
  return ret_val;
}

template <template <typename, class> class HashTable, typename Key,
	  class Cmp = Aleph::equal_to<Key>>
void test_hash_table(size_t n)
{
  typedef HashTable<Key, Cmp> Hset;
  DynArray<Key> keys(n);
  
  Hset table;

  tbl_size = table.size();

  for (int k = 0; k < 4; k++)
    {  
      cout << "k = " << k << endl
	   << "testing insertions and initial searches" << endl;

      for (int i = 0; i < n; i++)
	{
	  while (true)
	    {
	      keys[i] = gsl_rng_get(r);
	      if (table.search(keys(i)) == NULL)
		break;
	    }
	  
	  table.insert(keys(i));
	}

      cout << "done" << endl
	   << endl;

      table.print_stats(table.stats());

      cout << endl
	   << "testing searches or previous inserted keys" << endl;
      Key * ptr;
      for (int i = 0; i < n; i++)
	{
	  const Key k = keys(i);

	  ptr = table.search(k);

	  assert(ptr != NULL);
	  assert(*ptr == keys(i));
	}
      cout << "done!" << endl
	   << endl
	   << "testing deletion ...." << endl;
      for (int i = 0; i < n; i += 2)
	{
	  ptr = table.search(keys(i));
	  
	  assert(ptr != NULL);

	  table.remove_ptr(ptr);
	}
      cout << "done!" << endl
	   << endl
	   << "Reinserting others keys ...." << endl;
      for (int i = 0; i < n; i += 2)
	{
	  while (true)
	    {
	      keys[i] = gsl_rng_get(r);
	      if (table.search(keys(i)) == NULL)
		break;
	    }
	  table.insert(keys(i));
	}
      cout << "done!" << endl
	   << endl
	   << "Removing all the keys ...." << endl;
      for (int i = 0; i < n; i++)
	{
	  const Key & key = keys(i);
	  ptr = table.search(key);
	  assert(ptr != NULL);
	  table.remove_ptr(ptr);
	}

      assert(table.size() == 0);
      cout << "done! k = " << k << endl
	   << endl;
    }

  cout << "Sorting keys backup ...." << endl;
  quicksort_op(keys);

  cout << "done!" << endl
       << endl 
       << "Testing iterator ...." << endl
       << endl
       << "Reinserting the keys ...."
       << endl;
  for (int i = 0; i < n; ++i)
    table.insert(keys(i));

  int count = 0;
  for (typename Hset::Iterator it(table);
       it.has_curr(); it.next(), ++count)
    {
      const Key & curr = it.get_curr();
      int idx = binary_search(keys, curr);
      assert(idx >= 0);
      assert(curr == keys(idx));
    }
  assert(count == table.size());
  cout << "done!" << endl
       << endl
       << "Testing backward iterator ...." << endl;
  {
    typename Hset::Iterator it(table);
    count = 0;
    it.reset_last();
    for (int i = 0; it.has_curr(); it.prev(), ++i, ++count)
      {
	const Key & curr = it.get_curr();
	int idx = binary_search(keys, curr);
	assert(idx >= 0);
	assert(curr == keys(idx));
      }
    assert(count == table.size());
    cout << "done!" << endl
	 << endl; 
  }
  cout << "done!" << endl
       << endl
       << "Testing del() of iterator ...." << endl
       << "Deleting all the keys via del() of iterator"
       << endl;
  count = 0;
  for (typename Hset::Iterator it(table); it.has_curr(); ++count)
    it.del();
  
  cout << "done!. Deleted " << count << " entries " << endl
       << endl;

  assert(count == n);
  assert(table.is_empty());

  cout << "Inserting again all keys ...." << endl
       << endl;
  for (int i = 0; i < n; ++i)
    table.insert(keys(i));

  cout << "done!" << endl
       << endl
       << "Deleting a 10% of keys for causing deleted entries ...." << endl
       << endl;
  for (int i = 0; i < n/10; ++i)
    {
      unsigned long idx = gsl_rng_uniform_int(r, keys.size());
      const Key & key = keys(idx);
      
      Key * ptr = table.search(key);
      if (ptr == NULL)
	continue;

      table.remove_ptr(ptr);
    }

  table.print_stats(table.stats());

  {
    cout << "Testing copy constructor" << endl;
    Hset aux = table;
    assert(aux.size() == table.size());
    for (typename Hset::Iterator it(table); it.has_curr(); it.next())
      {
	Key * key_ptr = aux.search(it.get_curr());
	assert(key_ptr != NULL);
	assert(*key_ptr == it.get_curr());
      }
    cout << "done!" << endl;
  }

  {
    cout << "Testing rvalue copy constructor ...." << endl;
    Hset aux = create_table(table);

    assert(aux == table);

    aux = create_table(table);

    assert(aux == table);

    cout << "done!" << endl
	 << endl;
  }
}

int main(int argn, char *argc[])
{ 
  int n = argc[1] ? atoi(argc[1]) : DEFAULT_N;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  cout << "testOhash " << n << " " << t << endl;

  r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, t % gsl_rng_max(r));

  test_hash_table<ODhashTable, unsigned int>(n);

  //test_hash_table<OLhashTable, unsigned int>(n);
  
  gsl_rng_free(r);
}
