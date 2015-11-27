
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
# include <Vector.H> 
# include <print_container.H> 

using namespace Aleph;

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 10;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  int valor = 6;

  Aleph::vector<int> vector1(n - 1);
  Aleph::vector<int> vector2(static_cast<Aleph::vector<int>::size_type>(n), valor);

  cout << "size vector1: " << vector1.size() << endl;
  cout << "size vector2: " << vector2.size() << endl;
  cout << "capacity vector2: " << vector2.capacity() << endl;

  if (vector1.empty())
    cout << "vector1: vacio" << endl;
  else
    cout << "vector1: lleno" << endl;
  
  cout << "size maximo vector2: " << vector2.max_size() << endl;

  Aleph::vector<int> vector3;
  vector3 = vector2;
  for (int i = 0; i < vector3.capacity(); i++)
    cout << vector3[i] << " ";
  cout << endl;

  cout << "size de vector3 igualado a vector 2: " << vector3.size() << endl;
  cout << "capacity de vector3 igualado a vector 2: " 
       << vector3.capacity() << endl;

  unsigned int b = 3;
  vector2.assign(b, 9);
  cout << "size de vector2 despues de assign: " << vector2.size() << endl;
  cout << "capacity de vector2 despues de assign: " << vector2.capacity() 
       << endl;

  vector1.swap(vector3);
  cout << "capacidad vector1 despues de swap con vector 3: " 
       << vector1.capacity() << endl;
  cout << "capacidad vector3 despues de swap con vector 1: " 
       << vector3.capacity() << endl;
 
  cout << "tercer valor de vector2 es: " << vector2.at(2) << endl; 
  
  vector2[4] = 100;
  
  cout << "quinto valor de vector2 (100): " << vector2[4] << endl; 

  cout << "primer valor de vector2: " << vector2.front() << endl; 
  
  cout << "ultimo valor de vector2: " << vector2.back() << endl; 

  cout << "vector2: ";
  print_container(vector2);

  Aleph::vector<int> vector4(vector2.begin(),vector2.end());

  cout << "vector4: ";
  print_container(vector4);

  return 0;
  
  cout << "capacidad vector4: " << vector4.capacity() << endl;
 
  cout << "size vector4: " << vector4.size() << endl;
    
  vector3[2] = 5;

  cout << "Tercer elemento vector3 es (5): " << vector3[2] << endl;

  vector4.assign(vector3.begin(), vector3.end());

  cout << "capacidad vector4 despues de assign (4): " 
       << vector4.capacity() << endl;
 
  cout << "size vector4 despues de assign (4): " << vector4.size() << endl;


  cout << "capacidad vector1: " << vector1.capacity() << endl;

  cout << "size vector1: " << vector1.size() << endl;

  vector1.push_back(113);

  cout << "ultimo valor de vector1: " << vector1.back() << endl; 

  vector2.clear();
  cout << "capacidad de vector2 despues de clear(): " 
       << vector2.capacity() << endl;

  cout << "size de vector2 despues de clear(): " << vector2.size() << endl;

  vector2.insert(vector2.begin(), 20);

  cout << "size vector2 despues de insert: " << vector2.size() << endl;

  cout << "primer valor vector2: " << vector2.front() << endl;

  cout << "ultimo valor de vector2: " << vector2.back() << endl;

  vector2.insert(vector2.begin(), static_cast<Aleph::vector<int>::size_type>(n), -14);

  cout << "size vector2 despues de insert: " << vector2.size() << endl;

  cout << "primer valor vector2: " << vector2.front() << endl;

  cout << "ultimo valor de vector2: " << vector2.back() << endl;

  vector2.insert(vector2.begin(), vector1.begin(), vector1.end());

  cout << "size vector2 despues de insert: " << vector2.size() << endl;

  cout << "primer valor vector2: " << vector2.front() << endl;

  cout << "ultimo valor de vector2: " << vector2.back() << endl;


}
