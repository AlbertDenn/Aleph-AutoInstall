
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
# include <List.H>
# include <Vector.H>


using namespace std;

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  list<int> l1; 

  int i;
  for (i = 0; i < n; i++)
    l1.push_back(i);

  cout << "l1"; l1.print();

  list<int> l2(n); 

  cout << "l2"; l2.print();

  list<int> l3(n, 40); 

  cout << "l3"; l3.print();

  l1.insert(l1.begin(), 4, 100);

  l1.print();

  l1.insert(l1.begin(), l3.begin(), l3.end());

  l1.print();

  l1.remove(40);

  l1.print();

  l1.erase(l1.erase(l1.begin()));

  l1.print();

  list<int>::iterator it1 = l1.begin();
  list<int>::iterator it2 = l1.end();
  
  it1 += 3;

  it2 -= 2;

  l1.erase(it1, it2);
 
  l1.print();

  l1.resize(l1.size() + 3);

  l1.print();

  l1.unique();

  l1.print();

  for (list<int>::iterator it(l2.begin()); it not_eq l2.end(); )
    it++ = i++;

  l2.print();

  l1.splice(l1.begin(), l2);

  l1.print();

  l2.print();

  l3.print();

  while (not l3.empty())
    l1.splice(l1.begin(), l3, l3.begin());

  l1.print();
  
  l3.print();

  list<int> l4(n, 55);

  i = 0;
  for (list<int>::iterator beg(l4.begin()); beg not_eq l4.end(); )
    beg++ = i++;

  l4.print();

  list<int>::iterator it4_beg(l4.begin()), it4_end(l4.end());

  for (i = 0; i < n/4; i++)
    {
      it4_beg++;
      --it4_end;
    }

  for (list<int>::iterator beg(it4_beg); beg not_eq it4_end; )
    cout << " " << beg++;

  cout << endl << endl;

  l1.splice(l1.begin(), l4, it4_beg, it4_end);

  l1.print();
  l4.print();

  vector<int> v1(l1.begin(), l1. end());

  for (i = 0; i < v1.size(); i++)
    cout << v1[i] << " ";

  cout << endl;
}
