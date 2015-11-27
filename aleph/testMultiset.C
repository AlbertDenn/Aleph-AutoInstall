
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
# include <Multiset.H>
# include <print_container.H>

 
using namespace std;
using namespace Aleph;

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  size_t m = 2;
  if (argn > 2)
    m = atoi(argc[2]);

  if (argn > 3)
    t = atoi(argc[3]);

  srand(t);

  cout << argc[0] << " " << n << " " << m << " " << t << endl
       << endl
       << "Declarando multiset<int> s1;" << endl
       << endl 
       << endl;
  
  multiset<int> s1;

  cout << "Insertando " << n << " repeticiones de largo " << m << endl
       << endl;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      s1.insert(i);

  cout << "hecho!" << endl
       << endl
       << "Contando las courrencias de 1 ..." << endl
       << "halladas " << s1.count(1) << " ocurrencias de 1" << endl
       << endl
       << "Declarando multiset<int> s5;" << endl
       << endl;

  cout << "s1:" << endl;
  print_container(s1);

  multiset<int> s5;

  cout << "efectuando s5 = s1" << endl
       << endl;
  s5 = s1;

  cout << "s5:" << endl;
  print_container(s5);

  cout << "s1:" << endl;
  print_container(s1);

  cout << "Instanciando un iterador y mostrando los valores de s1 ..." << endl;
  multiset<int>::iterator fin(s1.end());
  for (multiset<int>::iterator it(s1.begin()); it != fin; it++)
    cout << *it << " ";

  cout << "Hecho!" << endl
       << endl
       << "Insertando s1.insert(s1.begin(), 1)" << endl
       << endl
       <<  * s1.insert(s1.begin(), 1) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << endl
       << "Insertando s1.insert(s1.begin(), 0)" << endl
       << endl
       <<  * s1.insert(s1.begin(), 0) << endl
       << endl
       << "s1:";
  print_container(s1);
  
  cout << endl
       << "Insertando s1.insert(s1.end(), 1)" << endl
       << endl
       <<  * s1.insert(s1.end(), 1) << endl
       << endl
       << "s1:";
  print_container(s1);

 cout << endl
       << "Insertando s1.insert(s1.end(), 5)" << endl
       << endl
       <<  * s1.insert(s1.end(), 5) << endl
       << endl
       << "s1:";
  print_container(s1);

 cout << endl
       << "Insertando s1.insert(s1.end(), 5)" << endl
       << endl
       <<  * s1.insert(s1.end(), 5) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << endl
       << "Insertando s1.insert(s1.find(2), 5)" << endl
       << endl
       <<  * s1.insert(s1.find(2), 5) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << endl
       << "Insertando s1.insert(s1.find(2), 5)" << endl
       << endl
       <<  * s1.insert(s1.find(2), 2) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << endl
       << "Insertando s1.insert(s1.find(2), 4)" << endl
       << endl
       <<  * s1.insert(s1.find(2), 4) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << endl
       << "Insertando s1.insert(s1.find(7), 4)" << endl
       << endl
       <<  * s1.insert(s1.find(2), 4) << endl
       << endl
       << "s1:";
  print_container(s1);

  cout << "Instanciando multiset<int> s2(s1.begin(), s1.end()); .." << endl
       << endl;
  multiset<int> s2(s1.begin(), s1.end());
  cout << "s2:";
  print_container(s2);

  cout << "Instanciando list<int> l1(s1.begin(), s1.end()); .." << endl
       << endl;
  list<int> l1(s1.begin(), s1.end());
  cout << "l1:";
  print_container(l1);

  cout << "Ejecutando s3.insert(l1.begin(), l1.end()); .." << endl
       << endl;
  multiset<int> s3(l1.begin(), l1.end());
  s3.insert(l1.begin(), l1.end());
  cout << "s3:";
  print_container(s3);

  cout << "Ejecutando s3.erase(4) .. " << s3.erase(4) 
       << " elementos borrados" << endl
       << "s3:";
  print_container(s3);

  return 0;

  s3.erase(s3.find(3));

  print_container(s3);

  s2 = s3;

  cout << "s2: " << endl;
  print_container(s2);

  cout << "s2.erase(s2.begin(), s2.end())" << endl;
  s2.erase(s2.begin(), s2.end());

  print_container(s2);

  multiset<int> s4(s1);

  cout << "s1 == s4 = " << (s1 == s4 ? "true" : "false") << endl;

  s1.insert(s1.find(4), 5);

  cout << "s1 == s4 = " << (s1 == s4 ? "true" : "false") << endl;

  cout << "s1: " << endl;
  print_container(s1);

  cout << "s4: " << endl;
  print_container(s4);

  cout << "s1 < s4 = " << (s1 < s4 ? "true" : "false") << endl;

  s1 = s4;

  cout << "s1 < s4 = " << (s1 < s4 ? "true" : "false") << endl;

  cout << argc[0] << " " << n << " " << m << " " << t << endl;
}
