
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


# include "tpl_dnode.H"



Dnode<int>* merge(Dnode<int>* l1, Dnode<int>* l2)
{
  Dnode<int> tmp, *p = &tmp;

  while (l1 != NULL && l2 != NULL)
    if (l1->get_data() <= l2->get_data())
      {
	p->get_next()  = l1; 
	l1->get_prev() = p;
	p             = l1; 
	l1            = l1->get_next();
      } 
    else 
      {
	p->get_next()  = l2; 
	l2->get_prev() = p;
	p             = l2; 
	l2            = l2->get_next();
      } 
  
  if (l1 == NULL)
    {
      p->get_next()  = l2;
      l2->get_prev() = p;
    }
  else
    {
      p->get_next()  = l1;
      l1->get_prev() = p;
    }
  return tmp.get_next();
}


Dnode<int>* mergeSort(Dnode<int>* list)
{
  Dnode<int> *l1, *l2;

  if (list->get_next() == NULL)
    return list;

  l1 = list;
  l2 = list->get_next();

  while  (l2 != NULL && l2->get_next() != NULL)
    {
      list = list->get_next();
      l2 = l2->get_next()->get_next();
    }

  l2              = list->get_next();
  l2->get_prev()   = NULL;
  list->get_next() = NULL;

  return merge( mergeSort(l1), mergeSort(l2) );  
}


void sortVector(Dnode<int>* vector[], int m, int n, Dnode<int>*& list)
{
  int i, j, l = n/m;
  Dnode<int> *p = list->get_next(), *first;

  list->get_prev()->get_next() = NULL;
  p->get_prev()           = NULL;

  first = p = mergeSort(p);

  for (i = 0; i < m; i++)
    {
      vector[i] = p;
      for (j = 0; j < l; j++, p = p->get_next());
    }

  while (p->get_next() != NULL)
    p = p->get_next();

  p->get_next()     = list;
  list->get_next()  = first;
  first->get_prev() = list;
}


# define N 100

# include <stdlib.h>
# include <iostream>

int main()
{
  int i, num;
  Dnode<int> *list = new Dnode<int>, *p;
  Dnode<int> *vector[3];

  for (i = 0; i < N; i++)
    {
      num = 1+(int) (1000.0*rand()/(RAND_MAX+1.0));
      p = new Dnode<int>;
      p->get_data() = num;
      list->insert(p);
      cout << num << " ";
    }      

  cout << "\n\n\n";
  
  sortVector(vector, 3, N, list);

  for (p = list->get_next(); p != list; p = p->get_next())
    {
      cout << p->get_data() << " ";
    }

  cout << "\n";
}
