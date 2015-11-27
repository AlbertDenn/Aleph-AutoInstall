
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


# include <stdio.h>
# include <iostream>
# include <dlink.H>

using namespace std;


struct Test //: public Dlink
{
  unsigned n;
  Dlink link;

  Test(unsigned __n = 0) : n(__n) {}
};


DLINK_TO_TYPE(Test, link);

void print_list(Dlink* link)
{
  Test* ptr;
  for (Dlink::Iterator it(link); it.has_current(); it.next())
    {
      ptr = dlink_to_Test(it.get_current()); 
      cout << ptr->n << " ";
    }
  cout << endl;
}

void delete_list(Dlink* link)
{
  Test* ptr;
  while (not link->is_empty())
    {
      ptr = dlink_to_Test(link->remove_next()); 
      delete ptr;
    }
}

void create_random_list(Dlink* link, int n)
{
  Test* testPtr;
  for (int i = 0; i < n; i++)
    {
      testPtr = new Test;
      testPtr->n = 1 + (int) (10.0*n*rand()/(RAND_MAX+1.0));
      link->append(&testPtr->link);
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
  Dlink head;

  unsigned i;

  for (i = 0; i < n; i++)
    {
      testPtr = new Test;

      testPtr->n = i;
      head.append(&testPtr->link);
      cout << testPtr->n << " ";
    }

  cout << endl;  

  Dlink l1, l2;

  head.split_list(l1, l2);

  print_list(&l1);

  head.swap(&l2);

  head.reverse_list();
  print_list(&head);

  Dlink * ptr = head.get_next();
  for (i = 0; i < n/4; i++)
    ptr = ptr->get_next();

  ptr->insert_list(&l1);

  assert(l1.is_empty());

  print_list(&l1);

  print_list(&head);

  for (ptr = head.get_next(), i = 0; i < n/4; i++)
    ptr = ptr->get_next();

  delete_list(&l1);

  l1 = head.cut_list(ptr);

  print_list(&head);

  print_list(&l1);

  delete_list(&head);

  head = l1.cut_list(l1.get_prev());

  cout << "head: "; print_list(&head);

  cout << "l1: "; print_list(&l1);

  {
    Dlink list;
    for (int i = 1; i < 10; ++i)
      list.append(&(new Test(i))->link);

    Dlink split = list.cut_list(list.get_next());
    assert(list.is_empty());
    Dlink::Iterator it(split); int i = 0;
    for (; it.has_curr(); i++, it.next())
      assert(dlink_to_Test(it.get_curr())->n == i);
    assert(i == 10);
  }

  printf("Ending .... \n\n");
}
      


