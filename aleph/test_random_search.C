
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
# include <tpl_sort_utils.H>

using namespace std;

struct Cmp
{
  bool operator () (Dlink * p, Dlink * q) const
  {
    return (static_cast<Dnode<int>*>(p)->get_data() < 
	    static_cast<Dnode<int>*>(q)->get_data());
  }
};


int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  Dnode<int> list;

  for (int i = 0; i < n; ++i)
    {
      int value = 1 + (int) (1.0 * n * (rand() / (RAND_MAX + 1.0)));
      
      list.append(new Dnode<int>(value));
    }

  for (Dnode<int>::Iterator it(list); it.has_current(); it.next())
    cout << it.get_current()->get_data() << " ";
  cout << endl;

# ifdef nada
  if (Aleph::random_search<int, Cmp>(list, n/2) != NULL)
    cout << n/2 << " se encuentra en la lista" << endl;
  else
    cout << n/2 << " no se encuentra en la lista" << endl;
# endif

  assert(list.check());

  if (Aleph::random_search(list, n/2) != NULL)
    cout << n/2 << " se encuentra en la lista" << endl;
  else
    cout << n/2 << " no se encuentra en la lista" << endl;

  assert(list.check());

  for (Dnode<int>::Iterator it(list); it.has_current(); it.next())
    cout << it.get_current()->get_data() << " ";
  cout << endl;

  assert(list.check());

  Dnode<int> * ptr = Aleph::random_select <int> (list, n/2);

  cout << "El elemento " << n/2 << " es: " << ptr->get_data() << endl;

  list.remove_all_and_delete();

}
