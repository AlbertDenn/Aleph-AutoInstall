
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


# include <time.h>
# include <gsl/gsl_rng.h> 
# include <cassert>
# include <iostream>
# include <ahSearch.H>
# include <ahSort.H>
# include <tpl_sort_utils.H>
# include <tpl_dynMapTree.H>

# define NumItems 10000

using namespace std;

gsl_rng * r = NULL;

unsigned long insert_n_random_items_in_set(DynSetTree<unsigned long> & table, 
					   DynArray<unsigned long> & keys,
					   unsigned long n)
{
  unsigned long dup_counter = 0;

  cout << "Testing simple insertions and searches ...." << endl;

  for (int i = 0; i < n; i++)
    {
      keys[i] = gsl_rng_get(r);
      if (not table.has(keys(i)))
	table.insert(keys(i));
      else
	++dup_counter;
    }

  return dup_counter;
}

DynSetTree<unsigned long> create_table(const DynSetTree<unsigned long> & other)
{
  DynSetTree<unsigned long> table;
  other.for_each([&table] (unsigned long item)
		 {
		   table.insert(item);
		 });

  return table;
}

void test_DynSet(size_t n)
{
  DynSetTree<unsigned long> table; 
  DynArray<unsigned long> keys;
  unsigned int dup_counter = insert_n_random_items_in_set(table, keys, n);

  unsigned long removed_counter = 0;
  size_t num_inserted = table.size();
  for (int i = 0; i < n; i++)
    if (table.search(keys(i)) != NULL)
      { 
	table.remove(keys(i));
	++removed_counter;
      }
 
  assert(removed_counter == num_inserted);
  assert(table.size() == 0);

  cout << removed_counter << " items removed" << endl;

  cout << "testing empty() method ...." << endl;
  dup_counter = insert_n_random_items_in_set(table, keys, n);

  table.empty();

  assert(table.size() == 0);

  unsigned long repeated_counter = 0;
  cout << "Reinserting keys ...." << endl;
  for (int i = 0; i < n; ++i)
    if (table.insert(keys(i)) == NULL) 
      ++repeated_counter;

  cout << repeated_counter << " duplicated numbers" << endl
       << dup_counter << " was the previus value" << endl;

  assert(dup_counter == repeated_counter);
  
  cout << "Done!" << endl;

  {
    cout << "Testing iterator and map ...." << endl;  

    DynList<unsigned long> l = table.template map<unsigned long>
      (/* Lambda */ [] (const unsigned long & k) -> unsigned long
      {
	return k;
      });

    for (DynList<unsigned long>::Iterator it(l); it.has_curr(); it.next())
      assert(table.search(it.get_curr()) != NULL);

    cout << "done!" << endl;
  }

  {
    cout << "testing lvalue copy constructor ...." << endl;
    DynSetTree<unsigned long> tmp = table;

    assert(table.equal_to(tmp));
  }

  {
    cout << "testing lvalue assigment...." << endl;
    DynSetTree<unsigned long> aux;
    for (int i = 0; i < n/2; ++i)
      {
	unsigned long key = gsl_rng_get(r);
	while (aux.has(key))
	  key = gsl_rng_get(r);
	aux.insert(key);
      }

    aux = table;

    assert(aux == table);
  }

  {
    cout << "Testing rvalue constructor ...." << endl;
    DynSetTree<unsigned long> tmp = create_table(table);
    assert(tmp == table);
    cout << "done!" << endl
	 << endl
	 << "Testing rvalue assign = .... " << endl
	 << endl;
    tmp = create_table(table);
    cout << "done!" << endl
	 << endl;
  }

  {
    DynArray<size_t> dups; // stores indexes of duplicated keys of keys array

    cout << "done" << endl
	 << "Reinserting ...." << endl;
    for (int i = 0; i < n; ++i)
      if (table.insert(keys(i)) == NULL)
	dups.append(i);

    cout << "Searching inserted keys ...." << endl;
    for (int i = 0; i < n; ++i)
      {
	unsigned long * ptr = table.search(keys(i));
	assert(ptr != NULL);
	if (dups.size() > 0 and dups(binary_search<size_t>(dups, i)) == i) 
	  continue; // keys[] contains a dup entry
      }
  }

  {
    cout << "Testing keys() in set ...." << endl
	 << endl;
    DynList<unsigned long> the_keys = table.keys();
    assert(the_keys.size() == table.size());
    assert(all(the_keys, /* Lambda */ [&table] (const size_t & key)
	  {
	    return table.has(key);
	  }));
  }

  {
    cout << endl
	 << "Testing filter of keys multiples of 13" << endl;
    
    DynList<unsigned long> v13 = 
      filter(table, [](const unsigned long & key)
	     {
	       return key % 13 == 0;
	     });

    table.filter(/* Lambda */ [] (const unsigned long & key)
		 {
		   return key % 13 == 0;
		 }).for_each(/* Lambda */ [&v13] (const unsigned long & key)
                 {
		   cout << key << " ";
		   assert(contains(v13, key));
		 });
    cout << endl;
  }
}


unsigned long 
insert_n_random_items_in_map(DynMapTree<unsigned long, long> & table, 
			     DynArray<unsigned long> & keys,
			     unsigned long n)
{
  unsigned long dup_counter = 0;
  cout << "Testing simple insertions and searches ...." << endl;
  for (long i = 0; i < n; i++)
    {
      keys[i] = gsl_rng_get(r);
      if (not table.has(keys(i)))
	assert(table.insert(keys(i), i));
      else
	++dup_counter;
    }
  return dup_counter;
}

void test_DynMap(size_t n)
{
  typedef DynMapTree<unsigned long, long> MapType;
  MapType table; 
  DynArray<unsigned long> keys;
  unsigned int dup_counter = insert_n_random_items_in_map(table, keys, n);

  unsigned long removed_counter = 0;
  size_t num_inserted = table.size();
  for (int i = 0; i < n; i++)
    if (table.search(keys(i)) != NULL)
      { 
	table.remove(keys(i));
	++removed_counter;
      }
 
  assert(removed_counter == num_inserted);
  assert(table.size() == 0);

  cout << removed_counter << " items removed" << endl;

  cout << "testing empty() method ...." << endl;
  dup_counter = insert_n_random_items_in_map(table, keys, n);

  table.empty();

  assert(table.size() == 0);

  unsigned long repeated_counter = 0;
  cout << "Reinserting keys ...." << endl;
  for (int i = 0; i < n; ++i)
    if (table.insert(keys(i), i) == NULL) 
      ++repeated_counter;

  cout << repeated_counter << " duplicated numbers" << endl
       << dup_counter << " was the previus value" << endl;

  assert(dup_counter == repeated_counter);
  
  cout << "Done!" << endl
       << endl
       << "Testing for_each and a battery of other tests ...." << endl;

  assert(table.all(/* Lambda */ [&table, &keys] 
		   (const std::pair<const unsigned long, long> & p)
		   {
		     auto * ptr = table.search(p.first);
		     assert(ptr != nullptr);
		     assert(table.get_data(p.first) == ptr->second);
		     return table.has(p.first);
		   })
	 );

  cout << "done!" << endl
       << endl
       << "testing keys() method and other tests ...." << endl;
  DynList<unsigned long> the_keys = table.keys();
  assert(all(the_keys, /* Lambda */ [&table] (unsigned long k) 
	{ return table.has(k); }));

  cout << "done!" << endl
       << endl
       << "Testing items() method and othet stuff ...." << endl;
  DynList<std::pair<unsigned long, long>> items = table.items();
  assert(all(items, /* Lambda */ [&table] 
	(std::pair<unsigned long, long> p)
	{ return table.find(p.first) == p.second; } ));
  cout << "done!" << endl
       << endl;
}

int main(int argn, char *argc[])
{
  unsigned long n = argn > 1 ? atoi(argc[1]) : NumItems;
  unsigned int  t = argn > 2 ? atoi(argc[2]) : time(NULL);

  cout << argc[0] << " " << n << " " << t << endl;

  r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, t % gsl_rng_max(r));

  test_DynSet(n);

  test_DynMap(n);

  gsl_rng_free(r);
}
