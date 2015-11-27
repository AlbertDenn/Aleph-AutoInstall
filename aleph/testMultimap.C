
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
# include <Multimap.H>

void print(multimap<int,int> & _map)
{
  multimap<int,int>::iterator itor(_map.begin());
  while (itor not_eq _map.end())
    {
      cout << "(" << itor->first << "," << itor->second << ")," ;

      itor++;
    }
  cout << endl;
}

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  int m = 10;
  if (argn > 2)
    m = atoi(argc[2]);

  unsigned int t = time(0);

  if (argn > 3)
    t = atoi(argc[3]);

  srand(t);

  cout << argc[0] << " " << n << " " << m << " " << t << endl;

  multimap<int,int> map1;


  cout << "insert((_pair);" << endl;
  for (int i = 0; i < n; i +=2)
    {
        for (int j = 0; j < m; j++)
	  {
	    std::pair<int,int> _pair(i,j);
	    map1.insert(_pair);
	  }
    } 
  
  int valor = n/2;
  cout << endl << "Buscando valor " << valor << " ... " ;
  unsigned int counter = map1.count(valor);
  if (counter == 0)
    cout << valor << " no existe en el multimap";
  else
    {
      cout << endl << counter << " pares encontrados" << endl;
      std::pair<int,int> _pair;
      _pair = *map1.find(valor);
      cout << "El primer par es: (" << _pair.first << "-" 
	   << _pair.second << ")" << endl;
      _pair = * map1.lower_bound(valor);
      cout << "El menor par es: (" << _pair.first << "-" 
 	   << _pair.second << ")"; 
      
    }
  cout << endl 
       << "lower_bound(" << valor << ") = (" 
       << map1.lower_bound(valor)->first 
       << "," << map1.lower_bound(valor)->second << ")" << endl
       << "upper_bound(" << valor << ") = (" 
       << map1.upper_bound(valor)->first 
       << "," << map1.upper_bound(valor)->second << ")" << endl;

  std::pair<multimap<int,int>::iterator, multimap<int,int>::iterator> 
    itors = map1.equal_range(valor);
  
  cout << endl 
       << "lower_bound(" << valor << ") = (" 
       << itors.first->first
       << "," << itors.first->second << ")" << endl
       << "upper_bound(" << valor << ") = (" 
       << itors.second->first
       << "," << itors.second->second << ")" << endl;

  print(map1);

  multimap<int,int> map2(map1);

  print(map2);

  multimap<int,int> map3(map1);

  print(map2);

  map2 = map3;

  print(map2);

  cout << "map1 == map2: ";
  if (map1 == map2)
    cout << "true";
  else
    cout << "false";
  cout << endl;

  map2.insert(pair<int, int>(3, int()));

  cout << "map1 == map2: ";
  if (map1 == map2)
    cout << "true";
  else
    cout << "false";
  cout << endl;

  cout << endl << "size map1: " << map1.size() << endl;

  map1.erase(valor);

  print(map1);
  cout << endl << "size map1: " << map1.size() << endl;

  cout << "map1.erase(map1.begin(), map1.end()); ..." << endl;

  map1.erase(map1.begin(), map1.end());

  cout << " listo" << endl;

  print(map1);
  cout << endl << "size map1: " << map1.size() << endl;

  map1 = map2;

  std::pair<int,int> __pair(5,50);

  cout << endl << "Hay " << map1.count(5) << " valores 5 en map1" << endl;

  __pair = *map1.lower_bound(5);

  cout << endl << "lower_bound de 5 es: " << __pair.first  << endl;

  __pair = *map1.upper_bound(5);

  cout << endl << "upper_bound de 5 es: " << __pair.first  << endl;

  cout << endl << "Borrando valor 5..." << endl;

  map1.erase(5);

  print(map1);

  cout << endl << "Borrando lower_bound 5..." << endl;

  __pair = *map1.lower_bound(5);

  cout << "lower_bound de 5 es: " << __pair.first  << endl;

  map1.erase(map1.lower_bound(5));

  print(map1);

  cout << endl << "size map1: " << map1.size() << endl;

}

