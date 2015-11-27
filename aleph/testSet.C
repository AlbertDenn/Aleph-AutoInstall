
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
# include <Set.H>
//# include <set>
# include <print_container.H>

 
struct Foo
{
  struct Tata {};
};


void test()
{
  set<Foo::Tata>::iterator it;
}

using namespace std;

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 10;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  set<int> set1;

  for (int i = 1; i <= n; i++)
    set1.insert(i);

  cout << "size de set1: " << set1.size() << endl;

  cout << endl << "set1: ";

  print_container(set1);

  cout << endl << "Prueba count(value)..." << endl;

  cout << "Hay " << set1.count(11) << " valores 1" << endl;

  cout << endl << "Prueba find(value)..." << endl;

  set<int>::iterator itor1(set1.find(n));

  cout << *itor1 << endl;

  set<int> set2;

  for (int k = 1; k <= n; k++)
    {
      set2.insert(k);
      set2.insert(k);
    }

  cout << "size de set2: " << set2.size() << endl;

  cout << endl << "set2: ";

  print_container(set2);

  cout << endl << "Prueba lower_bound en set2(5)" << endl;

  cout << *set2.lower_bound(5) << endl;

  cout << endl << "Prueba upper_bound en set2(5)" << endl;

  cout << *set2.upper_bound(5) << endl;

  cout << endl << "Insertando valor 11 en set1..." << endl;

  cout << *set1.insert(set1.upper_bound(5), 11) << endl;

  {
    set<int> set3;

    cout << *set3.insert(set3.begin(), 11) << endl;
  }

  cout << "size de set1: " << set1.size() << endl;

  cout << endl << "Swaping set1 y set2..." << endl;

  //set1.swap(set2);

  cout << endl << "set1: ";

  print_container(set1);

  cout << endl << "set2: ";

   print_container(set2);

  cout << endl << "Prueba erase(value)..." << endl;
  cout << set1.erase(5) << endl;

  cout << "size de set1: " << set1.size() << endl; 

  cout << endl << "Prueba erase(pos)..." << endl;
  set1.erase(set1.begin());

  set1.erase(set1.begin(), set1.end());

  cout << "size de set1: " << set1.size() << endl;

  print_container(set1);
}
