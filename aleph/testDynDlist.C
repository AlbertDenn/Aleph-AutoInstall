
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

# include <ahFunctional.H>
# include <htlist.H>
# include <tpl_dynDlist.H>

using namespace std;


void imprime(DynDlist<int> & l)
{
  for (DynDlist<int>::Iterator it(l); it.has_curr(); it.next())
    cout << it.get_curr() << " ";

  cout << "size = " << l.size() << " **" << endl;
}


void imprime(DynDlist< DynDlist<int> > & l)
{
  int i = 0;
  for (DynDlist<DynDlist<int> >::Iterator it(l); it.has_curr(); it.next())
    {
      cout << i++ << " : ";
      imprime(it.get_curr());
      cout << endl;
    }
}

int main(int argn, char *argc[])
{
  int n = 1000;
  if (argn > 1)
    n = atoi(argc[1]);

  int m = 1000;
  if (argn > 2)
    m = atoi(argc[2]);

  unsigned int t = time(0);

  if (argn > 3)
    t = atoi(argc[3]);

  srand(t);

  cout << argc[0] << " " << n << " " << m << " " << t << endl;

  DynDlist< DynDlist<int>> list;
  int i;

  for (i = 0; i < n; i++)
    {
      DynDlist<int> & l = list.insert(DynDlist<int>());

      for (int k = 0; k < m; ++k)
	l.append(k);
    }

  imprime(list);

  //  DynDlist<int> te ( DynDlist<int>(list.get_first()) ); 
  DynDlist<int> te = DynDlist<int>(); 

  te = DynDlist<int>(list.get_first());	

  for (DynDlist<int>::Iterator it(te); it.has_curr(); it.next())
    cout << it.get_curr() << endl;

  //  imprime(list);

  {
    DynDlist<int> l1 = range(10);
    // DynDlist<int> l2 = range(10, 20);
    l1.append_list(range(10, 20));
    l1.for_each([] (auto i) { cout << i << " "; }); cout << endl;
    // assert(l2.is_empty() and l2.size() == 0);
    int i = 0;
    assert(l1.all([&i] (auto k) { return k == i++; }));
  }

}

