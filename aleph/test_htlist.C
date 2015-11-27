
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
# include <htlist.H>
# include <filter_iterator.H>

using namespace std;

using namespace Aleph;

void print(HTList & l)
{
  cout << "l: ";
  for (HTList::Iterator it(l); it.has_curr(); it.next())
    {
      Snodenc<int> * p = (Snodenc<int>*) it.get_curr();
      cout << p->get_data() << " ";
    }
  cout << endl;
}

struct Par
{
  bool operator () (int i) const
  {
    return i % 2 == 0;
  }
};

typedef Filter_Iterator<DynList<int>, DynList<int>::Iterator, Par> It;


int main(int argn, char * argc[])
{
  size_t n = 10;
  if (argn > 1)
    n = atoi(argc[1]);

  HTList list;

  for (int i = 0; i < n; ++i)
    list.append(new Snodenc<int>(i));

  while (not list.is_empty())
    delete list.remove_first();


  for (int i = 0; i < n; ++i)
    list.append(new Snodenc<int>(i));

  HTList l1, l2;

  size_t sz = list.split_list(l1, l2);

  cout << "Lista de " << sz << " elementos partida en dos" << endl;
  print(l1);
  cout << endl;

  print(l2);
  cout << endl;

  list.append(l2);
  print(list);
  list.insert(l1);

  print(list);

  list.reverse();
  print(list);

  HTList::Iterator it(list);
  for (int i = 0; it.has_curr() and i < n/10; it.next(), ++i)
    ;

  list.cut(it.get_curr(), l1);

  print(list);
  cout << endl;

  print(l1);
  cout << endl;

  list.remove_all_and_delete();
  l1.remove_all_and_delete();

  cout << "****************" << endl;

  DynList<int> l(7);

  for (int i = 0; i < n; ++i)
    l.append(i);

  DynList<int> ll(2);

  ll = l ;

  cout << "Mostrando todos los elementos .. " << endl;
  for (DynList<int>::Iterator it(ll); it.has_curr(); it.next())
    cout << it.get_curr() << " " ;
  cout << endl;

  for (It it(ll); it.has_curr(); it.next())
     cout << it.get_curr() << " " ;
  cout << endl;

  DynList<int> lll = l;

  lll.reverse();

  l.append(l);

  l.append(ll);

  l.append(DynList<int>(l));

  l.insert(DynList<int>(lll));

  print(l);
}
