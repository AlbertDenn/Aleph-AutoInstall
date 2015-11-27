
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


# include <tpl_dynLhash.H>
# include <tpl_dynArray.H>
# include <iostream>
# include <time.h>
# include <stdlib.h>

# define NumItems 10000

using namespace std;

typedef DynLhashTable<unsigned, unsigned> HTable;

static size_t hashFct(const unsigned & key)
{
  return key;
}

static void testResize(HTable& table)
{
  if (table.get_num_busy_slots() > (99*table.capacity())/100 
      && table.size()/table.capacity() > 3)
    {
      unsigned long currSize = table.capacity();
      cout << "Resizing hash table from " << currSize << " ... ";
      cout << table.resize(1.5 * table.capacity()) << endl;
    }
}

static void printPars(const HTable& table)
{
  cout << "Table length = " << table.capacity() << endl
       << "Busy slots   = " << table.get_num_busy_slots() << endl
       << "Num items    = " << table.size() << endl;
}

int main(int argn, char *argc[])
{
  Primes::check_primes_database();

  unsigned long n = argn > 1 ? atoi(argc[1]) : NumItems;
  unsigned int  t = argn > 2 ? atoi(argc[2]) : time(NULL);
  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  HTable table(1.15*n, hashFct);
  DynArray<unsigned int> keys(n);
  unsigned int i;
  unsigned int foundCounter = 0;

  for (i = 0; i < n/2; i++)
    {
      keys[i] = rand();
      testResize(table);
      if (table.search(keys(i)) == NULL)
	  assert(table.insert(keys[i], i) != NULL);
      else
	foundCounter++;
    }

  cout << foundCounter << " duplicated numbers" << endl;

  assert( table.size() + foundCounter == n/2);
  printPars(table);

  for (i = n/2; i < n; i++)
    {
      keys[i] = rand();
      testResize(table);
      table[keys[i]] = i;
      table[keys[i]] = table[keys[i]];
    }
    
  printPars(table);
    
  unsigned * ptr;
  foundCounter = 0;
  for (i = 0; i < n; i++)
    {
      ptr = table.search(keys[i]);
      if (ptr != NULL)
	table.remove(ptr);
      else
	foundCounter++;
    }
}
