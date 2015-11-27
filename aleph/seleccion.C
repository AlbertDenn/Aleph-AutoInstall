
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
# include <tpl_dnode.H>


using namespace std;

# define DATA(p) ((p)->get_data())


void build_list(Dnode<int> * l, int n)
{
  for (int i = 0; i < n; i++)
    l->append(new Dnode<int> (rand()));
}


int select(Dnode<int> * l, int i)
{
  int lcount = 0, rcount = 0;
  Dnode<int> smaller, bigger;
  Dnode<int> * p = l->remove_next();

  /* 
     particiona la lista según pivote p en smaller, p, bigger y cuenta
     los elementos de manera que el total es lcount + 1 + rcount
     correspondientes a las longitudes de smaller, p y bigger 
  */
  while (not l->is_empty()) 
    {
      Dnode<int> *node = l->remove_next();
      if (DATA(node) < DATA(p))
	{
	  lcount++;
	  smaller.append(node);
	}
      else
	{
	  rcount++;
	  bigger.append(node);
	}
    } 

  /* En este punto lcount es la posicion del pivote */

  int result;
  if (i == lcount) 
    result = DATA(p); /* el pivote corresponde al i menor elemento */
  else if (i < lcount)
    result = select(&smaller, i); /* el i menor está en la lista smaller */
  else
    result = select(&bigger, i - (lcount + 1)); /* el i menor está en
						   la lista bigger */

      /* reconstruir lista */
  l->concat_list(&smaller);
  l->append(p);
  l->concat_list(&bigger);

  return result;
}

int acceder(Dnode<int> * l, int i)
{
  Dnode<int>::Iterator itor(l);

  for (int k = 0; k < i; k++)
    itor.next();

  return DATA(itor.get_current());
}

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  Dnode<int> list;

  cout << "Construyendo una lista de numeros aleatorios ...";

  build_list(&list, n);

  cout << " terminado" << endl << endl
       << "Buscando el " << n/4 << " menor elemento ...";

  int result = select(&list, n/4);

  cout << " terminado. El resultado es " << result << endl
       << "Ordenando arreglo para verificacion de correctitud ...";

  //  list.quicksort();

  cout << " terminado" << endl;

  if (result == acceder(&list, n/4))
    cout << "El resultado es " << result << endl;
  else
    cout << "Error" << endl;
}
