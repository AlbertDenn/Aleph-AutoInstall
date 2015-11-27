
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



# include <stdlib.h>
# include <iostream>
# include <time.h>

# include <iostream>
# include <tpl_dnode.H>
# include <tpl_sort_utils.H>

using namespace std;
using namespace Aleph;


// busca el índice del menor elemento del arreglo que sea mayor a x
static int search_min(int a[], const size_t & n, const int & x)
{
  int i_min = -1;

  for (int i = 0; i < n; ++i) // buscar primer elemento mayor que x
    if (a[i] > x)
      {    // encontrado ==> memorizar índice 
	i_min = i;

	// continuar con el resto  y localizar el menor mayor que x
	for (int j = i + 1; j < n; ++j) 
	  if (a[j] < a[i_min] and a[j] > x)
	    i_min = j;

	break;
      }

  return i_min;
}

int select(int a[], const size_t & n, const int & i)
{
  int i_min = search_min(a, n, INT_MIN); // índice menor elemento del arreglo

  for (int k = 1; k <= i; ++k)
    i_min = search_min(a, n, a[i_min]);

  return i_min;
}



void permutar_pares(Dlink * lista)
{
  Dlink aux; 

  while (true)
    {
      if (lista->is_empty())
	break;

      Dlink * p = lista->remove_next();

      if (lista->is_empty())
	break;

      Dlink * q = lista->remove_next();

      aux.append(q);
      aux.append(p);
    }

  aux.swap(lista);
}


void conmuta_pares(Dlink * l1, Dlink * l2)
{
  Dlink aux1, aux2;

  for (int i = 0; (not l1->is_empty()) and (not l2->is_empty()); ++i)
    {
	Dlink * p = l1->remove_next(); 
	Dlink * q = l2->remove_next();

	if (i % 2 == 0) // ¿posición es par?
	  {
	    aux1.append(q);
	    aux2.append(p);
	  }
	else
	  {
	    aux1.append(p);
	    aux2.append(q);
	  }
    }

  // Una de las dos listas a lo mejor no quedó vacía, concatenarla a su aux

  if (not l1->is_empty())
    aux1.concat_list(l1);
  else
    aux2.concat_list(l2);

  aux1.swap(l1);
  aux2.swap(l2);
}

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 20;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  int a[n];

  Dnode<int> list;

  for (int i = 0; i < n; ++i)
    {
      a[i] = (int) (10.0*n*rand()/(RAND_MAX+1.0));

      list.append(new Dnode<int>(a[i]));

      cout << a[i] << " ";
    }

  cout << endl;
  
  cout << "El 0 menor = " << a[select(a, n, 0)] << endl;
  cout << "El 1 menor = " << a[select(a, n, 1)] << endl;
  cout << "El 2 menor = " << a[select(a, n, 2)] << endl;
  cout << "El 3 menor = " << a[select(a, n, 3)] << endl;
  cout << "El 4 menor = " << a[select(a, n, 4)] << endl;
  cout << "El 5 menor = " << a[select(a, n, 5)] << endl;

  selection_sort(a, n);

  for (int i = 0; i < n; ++i)
    cout << a[i] << " ";
  cout << endl;


  permutar_pares(&list);
  
  for (Dnode<int>::Iterator it(list); it.has_current(); it.next())
    cout << it.get_current()->get_data() << " ";
  cout << endl;

  cout << "================" << endl;

  Dnode<int> l1,l2;

  for (int i = 0; i < n; ++i)
    {
      int value = (int) (10.0*n*rand()/(RAND_MAX+1.0));

      l1.append(new Dnode<int>(value));

      cout << value << " ";
    }
  cout << endl;

  for (int i = 0; i < n+3; ++i)
    {
      int value = (int) (10.0*n*rand()/(RAND_MAX+1.0));

      l2.append(new Dnode<int>(value));

      cout << value << " ";
    }
  cout << endl;

  conmuta_pares(&l1, &l2);

  cout << "================" << endl;

  for (Dnode<int>::Iterator it(l1); it.has_current(); it.next())
    cout << it.get_current()->get_data() << " ";
  cout << endl;

  for (Dnode<int>::Iterator it(l2); it.has_current(); it.next())
    cout << it.get_current()->get_data() << " ";
  cout << endl;

  cout << endl;
}
