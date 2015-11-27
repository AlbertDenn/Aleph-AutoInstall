
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


# include <tpl_linHash.H>
# include <aleph.H>
# include <iostream>
# include <time.h>
# include <stdlib.h>

using namespace std;

static unsigned long hashFct(const unsigned long& key)
{
  return (unsigned long) key;
}

struct Entry :  public LinearHashTableVtl<unsigned long>::Bucket
{
  unsigned long val;

  Entry(unsigned long k, unsigned long v) 
    : LinearHashTableVtl<unsigned long>::Bucket(k), val( v )
    {
      /* EMPTY */
    }
};

int main(int argn, char *argc[])
{ 
  const unsigned long numNodes = 10000;

  unsigned long  i, n = argc[1] ? atol(argc[1]) : numNodes;

  unsigned long  value;

  DynArray<unsigned int> keys(n);

  unsigned int t = time(NULL);

  if (argn > 2)
    t = atol(argc[2]);

  cout << "testDynamicHash " << n << " " << t << endl;

  srand(t);

  LinearHashTableVtl<unsigned long> hashTable(hashFct, 3, 7);
  Entry* bucket;

  cout << "Inserting..." << endl;

  for (i = 0; i < n; i++)
    {
      value = keys[i] = (unsigned long)  (n * (rand() /(RAND_MAX + 1.0))) ;
      bucket = new Entry (keys[i], i);
      hashTable.insert(bucket);
    }
  
  cout << endl << "Number of Expansions  : " << hashTable.getNumExpansions() 
       << endl << "Number of Contractions: "
       << hashTable.getNumContractions() << endl;

  cout << endl << "Searching..." << endl;

  for (i = 0; i < n; i++)
    {
      bucket = static_cast<Entry*>(hashTable.search(keys[i]));
      value = keys[i];
      if (bucket == NULL)
	{
	  cout << endl << "Error key " << keys[i] << " not found" <<  endl;
	  abort();
	}
    }
  
  cout << endl << "Deleting..." << endl;

  try
    {
      for (i = 0; i < n; i++)
	{
	  bucket = static_cast<Entry*>(hashTable.search(keys[i]));
	  if (bucket != NULL)
	    {
	      hashTable.remove(bucket);
	      delete bucket;
	    }
	  else
	    ERROR("%u th key %u not found\n", (int) i, (int) keys[i]);
	}  
    }
  catch (exception& exc)
    {
      cout << exc.what() << " exception has been thrown" << endl;       
    }
  catch (...)
    {
      cout << " unknown exception has been thrown" << endl; 
    }

  cout << endl
       << "Number of Expansions  : " << hashTable.getNumExpansions() << endl
       << "Number of Contractions: " << hashTable.getNumContractions() 
       << endl;
  
  assert(hashTable.getNumItems() == 0);
  
}


