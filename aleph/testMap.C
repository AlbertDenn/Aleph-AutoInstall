
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
# include <Map.H>

using namespace Aleph;

void print_container(map<int,int> & m)
{
  map<int,int>::iterator it = m.begin();
  while (it not_eq m.end())
    {
      cout << "(" << it->first << "," << it->second << ") ";
      it++;
    }
  cout << endl;
}

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 10;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  map<int,int> map1;
  
  for (int i = 0; i < n; i++)
    {
      map1.insert(make_pair(i, i*10));
      map1.insert(make_pair(i, i*100));
    }
    
  cout << endl << "size map1: " << map1.size() << endl;
  
  print_container(map1);

  cout << endl << "Hay " << map1.count(n/2) << " valores de " << n/2
       << " en map1" << endl;

  cout << endl << "Prueba find(" << n/2 << ")..." << endl;

  std::pair<int, int> _pair;

  _pair = *map1.find(n/2);
  
  cout << _pair.first << endl;
  
  _pair = *map1.lower_bound(n/2);

  cout << endl << "lower_bound de " << n/2 << " es: " << _pair.first  << endl;

  _pair = *map1.upper_bound(n/2);

  cout << endl << "upper_bound de " << n/2 << " es: " << _pair.first  << endl;

  cout << endl << "Borrando valor " << n/2 << " ..." << endl;

  map1.erase(n/2);

  print_container(map1);

  cout << endl << "Borrando lower_bound " << n/2 << " ..." << endl;

  _pair = *map1.lower_bound(n/2);

  cout << "lower_bound de " << n/2 << " es: " << _pair.first  << endl;

  map1.erase(map1.lower_bound(n/2));

  print_container(map1);

  cout << "prueba de insercion mediante operador []" << endl;

  for (int i = 0; i < n; i++)
    map1[i] = i;

  print_container(map1);

  cout << "prueba de copia mediante operador []" << endl;

  map<int, int> map2;

  for (int i = 0; i < n; i++)
    map2[i] = map1[i];

  print_container(map2);

  cout << "prueba de lectura  mediante operador []" << endl;
  for (int i = 0; i < n; i++)
    cout << "map2[" << i << "] = " << map2[i] << " ";
  cout << endl;

  cout << "prueba de lectura fallida mediante operador []" << endl;
  try
    {
      cout << "Esta es una lectura fallida map1[" << n << "] = " 
	   << map1[n] << endl;
    }
  catch (exception & e)
    {
      cout << e.what() << endl;
    }

  cout << "prueba de constructor copia " << endl;
  map<int, int> map3 = map2;
  print_container(map3);

  cout << "prueba de operador asignacion " << endl;
  map3 = map1;
  print_container(map3);

  cout << "map1 == map2: " << (map1 == map2 ? "true" : "false") << endl;
  
  map1[n/2] = n;

  cout << "map1 == map2: " << (map1 == map2 ? "true" : "false") << endl;

  map1[n/2] = n/2;

  cout << "map1 == map2: " << (map1 == map2 ? "true" : "false") << endl;

  map1[n] = n;

  cout << "map1 == map2: " << (map1 == map2 ? "true" : "false") << endl;

  map2[n] = n;

  cout << "map1 == map2: " << (map1 == map2 ? "true" : "false") << endl;

  map1[n] = n -1;

  cout << "map1 < map2: " << (map1 < map2 ? "true" : "false") << endl;

  map1[n] = n + 1;

  cout << "map1 < map2: " << (map1 < map2 ? "true" : "false") << endl;

  map1[n] = map1[n-1];

  print_container(map1);
  cout << "Prueba de erase(map1.begin(), map1.end())" << endl;

  map1.erase(map1.begin(), map1.end());

  print_container(map1);

}

