
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
//# include <list.h>
# include <print_container.H>

using namespace std;

int main()
{
  unsigned int n_elem = 7;

  int valor = 4;

  list<int> list1;

  list<int> list2(5);

  list<int> list3(n_elem, valor);

  list<int> list4(list3.begin(), list3.end());
  
  cout << "size list1: " << list1.size() << endl;

  cout << "list1 esta: "; 
  if (list1.empty()) cout << "vacia" << endl;
  else cout << "llena" << endl;
 
  cout << "size list2: " << list2.size() << endl;

  cout << "list2 esta: "; 
  if (list2.empty()) 
    cout << "vacia" << endl;
  else 
    cout << "llena" << endl;

  cout << "size list3: " << list3.size() << endl;
  
  cout << "size list4: " << list4.size() << endl;
  
  cout << "Swaping list1 y list2..." << endl;

  list1.swap(list2);

  cout << "size list1: " << list1.size() << endl;
  cout << "size list2: " << list2.size() << endl;

  
  cout << "ultimo valor lista3: " << list3.back() << endl;

  cout << "prueba push_front()..." << endl;
  list3.push_front(100);
  cout << "primer valor lista3: " << list3.front() << endl;
  cout << "ultimo valor lista3: " << list3.back() << endl;
  
  cout << "prueba push_back()..." << endl;
  list3.push_back(5000);
  cout << "ultimo valor lista3: " << list3.back() << endl;
  cout << "size list3: " << list3.size() << endl;
  
  cout << "Prueba remove()..." << endl;
  list4.remove(4);
  cout << "size list4: " << list4.size() << endl;

  cout << "Prueba pop_front()..." << endl;
  list3.pop_front();
  cout << "primer valor lista3: " << list3.front() << endl;
  cout << "size list3: " << list3.size() << endl;

  cout << "Prueba pop_back()..." << endl;
  list3.pop_back();
  cout << "ultimo valor lista3: " << list3.back() << endl;
  cout << "size list3: " << list3.size() << endl;

  cout << "Prueba clear()..." << endl;
  list3.clear();
  cout << "size list3: " << list3.size() << endl;

  cout << "Llenando list3..." << endl;
  for (int i = 1; i <= 5; i++)
    {  
      for (int n = 1; n <= 10; n++)
	list3.push_back(n);
    }
  
  //list3.reverse();
  cout << "primer valor lista3: " << list3.front() << endl;
  cout << "ultimo valor lista3: " << list3.back() << endl;
  cout << "size list3: " << list3.size() << endl;

  
  cout << "Prueba unique..." << endl;
  list<int> l4((size_t) 10, (int) -35);
  list<int>::iterator it4 = l4.end(); 
  --it4;
  it4 = list3.end(); --it4;
  list3.insert(it4, l4.begin(), l4.end());
  list3.unique();
  Aleph::print_container(list3);
  cout << "size list3: " << list3.size() << endl;

  cout << "Prueba sort() " << list3.size() << endl;
  list3.sort();
  Aleph::print_container(list3);

  list<int> list5;
  cout << endl <<"Llenando list5..." << endl;
  for (int n = 1; n <= 5; n++)
    list5.push_back(n);

  cout << "primer valor lista5: " << list5.front() << endl;
  cout << "ultimo valor lista5: " << list5.back() << endl;
  cout << endl <<"Prueba reverse()..." << endl;

  list5.merge(list3);

  list5.reverse();

  cout << "primer valor lista5: " << list5.front() << endl;
  cout << "ultimo valor lista5: " << list5.back() << endl;
  Aleph::print_container(list5);
  cout << "size list5: " << list5.size() << endl;

  cout << "Prueba erase(iteratos pos)..." << endl;
  cout << *list5.erase(list5.begin()) << endl;
  cout << "size list5: " << list5.size() << endl;

  Aleph::print_container(list5);

  cout << "Prueba resize(num)..." << endl;
  list5.resize(4);
        
  Aleph::print_container(list5);

  list5.resize(2);
  
  Aleph::print_container(list5);
  
  list5.resize(10);
  
  Aleph::print_container(list5);
  
  cout << "Prueba resize(num, value)..." << endl;

  list5.resize(4,8);
   
  Aleph::print_container(list5);

  list5.resize(4,8);
  
  Aleph::print_container(list5);
  
  list5.resize(10,8);

  Aleph::print_container(list5);

}
