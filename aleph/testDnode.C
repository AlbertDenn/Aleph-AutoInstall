
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


# include <iostream>
# include <tpl_dnode.H>

typedef Dnode<int> Test;

using namespace std;

void print_list(Test * link)
{
  Test* ptr;
  for (Test::Iterator it(link); it.has_current(); it.next())
    {
      ptr = it.get_current(); 
      cout << ptr->get_data() << " ";
    }
  cout << endl;
}


void create_random_list(Test * link, int n)
{
  Test* testPtr;
  for (int i = 0; i < n; i++)
    {
      testPtr = new Test (1 + (int) (10.0*n*rand()/(RAND_MAX+1.0)));
      link->append(testPtr);
    }
}

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  Test* testPtr;
  Test* headPtr;
  Test* tmpPtr;
  Test  head;

  headPtr = &head;

  unsigned long i;

  for (i = 0; i < n; i++)
    {
      testPtr = new Test;

      testPtr->get_data() = i;
      headPtr->insert(testPtr);
      cout << i << " ";
    }

  cout << endl;

  for (i = 0, testPtr = headPtr->get_next(); i < n; 
       i++, testPtr = testPtr->get_next())
    cout << testPtr->get_data() << " ";

  cout << endl;

  for (testPtr = headPtr->get_prev(); testPtr != headPtr; 
       testPtr = testPtr->get_prev())
    cout << testPtr->get_data() << " ";

  cout << endl;

  while (not headPtr->is_empty())
    {
      testPtr = headPtr->get_next();
      cout << testPtr->get_data() << " ";
      headPtr->remove_next();
      delete testPtr;
    }

  cout << endl;

  for (i = 0; i < n; i++)
    {
      testPtr = new Test;

      testPtr->get_data() = i;
      headPtr->insert(testPtr);
      cout << i << " ";
    }

  cout << endl;

  for (testPtr = headPtr->get_next(); testPtr not_eq headPtr;)
    {
      tmpPtr = testPtr;
      testPtr = testPtr->get_next();
      cout << testPtr->get_data() << " ";
      tmpPtr->del();
      delete tmpPtr;
    }

  cout << endl;

  for (i = 0; i < n; i++)
    {
      testPtr = new Test;

      testPtr->get_data() = i;
      headPtr->insert(testPtr);
      cout << i << " ";
    }

  for (Dnode<unsigned>::Iterator itor(headPtr); itor.has_current(); )
    delete itor.del();

  assert(headPtr == headPtr->get_prev() && headPtr == headPtr->get_next());

  cout << endl << endl;

  Test list;

  create_random_list(&list, n);

  print_list(&list);

  print_list(&list);

  list.remove_all_and_delete();

  printf("Ending .... \n\n");
}
      

