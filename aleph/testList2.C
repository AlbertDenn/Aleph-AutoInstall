
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
//# include <list>

using namespace std;

int main()
{
  list<int> list1, list2;
  
  cout << endl << "Llenando listas..." << endl;

  for (int i = 1; i <= 10; i++)
    {
      list1.push_front(i);
      list2.push_front(i*10);
    }

  cout << "list1: "; 
  list1.print();
  cout << "list2: ";
  list2.print();

  cout << endl << "swaping list1 y list2..." << endl;

  list1.swap(list2);

  cout << "list1: ";
  list1.print();
  cout << "list2: ";
  list2.print();

  cout << endl;

  cout << "Prueba insert(pos, value)..." << endl;

  list<int>::iterator itor1(list2.begin());

  itor1++;

  cout << *list2.insert(itor1,999) << endl;

  list2.print();
  cout << "size list2: " << list2.size() << endl;
  cout << endl;

  cout << "Prueba insert(pos, num, value)..." << endl;

  list<int>::iterator itor2(list2.begin());

  itor2++;

  list2.insert(itor2, 5, 800);

  list2.print();
  cout << "size list2: " << list2.size() << endl;
  cout << endl;

  cout << "Prueba insert(pos, beg, end)..." << endl;

  list<int>::iterator itor3(list1.begin());

  itor3++;

  list1.insert(itor3, list2.begin(),list2.end());

  list1.print();
  cout << "size list1: " << list1.size() << endl;
  cout << endl;

  list<int>::iterator itor4(list2.begin());
  itor4++; itor4++; itor4++; itor4++;
  cout << endl << "Prueba erase (beg, end)..." << endl;
  cout << *list2.erase(list2.begin(), itor4) << endl;
  list2.print();

  cout << endl << "Prueba splice(pos, list)..." << endl;
  list<int> list3(3,3);
  list3.splice(list3.begin(), list2);
  list3.print();
  cout << "size lis2: " << list2.size() << endl;

  cout << endl << "Prueba splice(pos, list, sourcePos)..." << endl;
  list3.splice(list3.begin(), list1, list1.begin());
  list3.print();
  cout << "size lis2: " << list1.size() << endl;

  cout << endl << "Prueba sort..." << endl;
  list3.sort();
  list3.print();

  cout << endl << "Prueba merge..." << endl;
  list3.merge(list1);
  list3.print();
  list1.print();
  list1.push_back(7);

  cout << endl << "Prueba assign..." << endl;
  list1.assign(list3.begin(), list3.end());
  list1.print();
  
  list1.pop_front();
  list3.pop_back(); 
  
  list1.print();
  list3.print();
  
  /*if (list1 > list2)
    cout << "list1 es mayor" << endl;
  else
  cout << "list1 es menor o igual" << endl;*/
  
}

