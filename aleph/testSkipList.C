
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

# include <tpl_skipList.H>
# include <stdlib.h>
# include <time.h>
# include <iostream>

using namespace std;

typedef SkipList<long,long>       LongSkipList;
typedef SkipList<long,long>::Node LongSkipListNode;


double doubleRand()
{
  return rand()/(RAND_MAX+1.0);
}

void DeleteSkipNodes(LongSkipListNode* node )
{

  while( node != NULL )
    {
      delete node; 
      node = node->get_next();
     }   
}


template <>
long SkipList<long, long>::Node::computeMaxKey()
{
  return LONG_MAX; 
}


int main(int argn, char *argc[])
{
  unsigned long seed, n, val, i = 0;

  
  if ( argn < 2 )
    {
      cout << "Usage: testSkipList [number of nodes] <[seed]> " << endl;
      return 0;
    }

  n    = atol(argc[1]);


  if (argn > 2 )
    seed = atol(argc[2]);
  else
    seed = time(NULL);
 
   srand( seed );
   
  cout << "./testSkipList " << n << " " << seed << endl;
  cout << "Inserting..." << endl;

  LongSkipListNode  *node;

  unsigned long keys[n];

  LongSkipList list(doubleRand);

  for( i = 0; i < n; i++)
    {
      val =  1+(unsigned long) ((n*20.0)*rand()/(RAND_MAX+1.0));
      
      int level = list.generateRandomLevel();

      assert( ( (sizeof(LongSkipListNode*)*level) & 1) == 0 );

//       node = NewSize(LongSkipListNode, sizeof(LongSkipListNode)*level)
//                      (val, val, level);
      
      node = list.new_node();

      keys[i] = val;
      list.insert( node );
      cout << "(" << val << ")";
    }

  cout << endl << "Searching..." << endl;

  for( i = 0; i < n; i++)
    {
      node = list.search(keys[i]);
      if (node == NULL)
	{
	  cout << "Error: Key " << keys[i] << " not found! " << endl;
          DeleteSkipNodes( list.get_first() ); 
	  exit( 0 );
	}
    }  

  cout << "Ok! " << endl;

  cout << endl << "Deleting..." << endl;
  
  for( i = 0; i < n; i++)
    {
      val    =  1+(unsigned long) ((n*20.0)*rand()/(RAND_MAX+1.0));
       node = list.remove(val);
      if ( node == NULL )
	{
	  cout << ".";
	  continue;
	}      
      cout << "(" << val << ")";
      delete node;
    }
 
  cout << endl;

  DeleteSkipNodes( list.get_first() ); 

 return 0;

}






































