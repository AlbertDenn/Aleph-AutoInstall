
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
# include <tpl_arrayHeap.H>

int keys [] = { 36, 32, 4, 12, 52, 59, 2, 2, 26, 1 };

using namespace std;
using namespace Aleph;


# define NIL (-1)
# define LLINK(i, n) (2*i)
# define RLINK(i, n) (2*i + 1)


void preorder(int v[], int n, int i)
{
  if (i > n)
    return;

  cout << " " << v[i];

  preorder(v, n, LLINK(i, n));
  preorder(v, n, RLINK(i, n));
}


void inorder(int v[], int n, int i)
{
  if (i > n)
    return;

  inorder(v, n, LLINK(i, n));
  cout << " " << v[i];
  inorder(v, n, RLINK(i, n));
}


int main(int argn, char* argc[])
{
  srand(time(0));
  unsigned int n = 10;

  if (argn > 1)
    n = atoi(argc[1]);

  {
    ArrayHeap<int> heap(n);
    unsigned int i, value;

    for (i = 0; i < n; i++)
      {
	value = 1 + (int) (100.0*rand()/(RAND_MAX+1.0));
	// value = keys[i];
	cout << value << " ";
	heap.insert(value);
      }

    cout << endl 
	 << "Preorder ";

    preorder(&heap[0], heap.size(), 1);
 
    cout << "\n\n";

    cout << endl 
	 << "inorder ";

    inorder(&heap[0], heap.size(), 1);

    cout << "\n\n";

    for (i = 1; i <= heap.size(); i++)
      cout << heap[i] << " ";

    cout << "\n\n";

    for (i = 0; i < n; i++)
      {
	value = heap.getMin();
	cout << value << " ";
      }

    cout << "\n\n";
  }

  {
    int * array1 = new int [n];

    int * array2 = new int [n];

    for (int i = 0; i < n; i++)
      {
	int value = 1 + (int) (100.0*rand()/(RAND_MAX+1.0));
	array1[i] = array2[i] = value;
      }
    heapsort(array1, n);

    for (int i = 0; i < n; i++)
      cout << array1[i] << " ";

    cout << endl;

    delete [] array1;

    faster_heapsort(array2, n);

    for (int i = 0; i < n; i++)
      cout << array2[i] << " ";

    cout << endl;

    delete [] array2;
  }

  return 0;

  { 
    cout << "testing with pointers" << endl;

    ArrayHeap<unsigned int*> heap(n);
    unsigned int i, value;
    unsigned int *ptr;

    for (i = 0; i < n; i++)
      {
	value = rand() % 100;
	ptr = new unsigned int;
	*ptr = value;
	cout << value << ", ";
	heap.insert(ptr);
      }

    cout << "\n\n";

    for (i = 0; i < heap.size(); i++)
      cout << *heap[i] << " ";

    cout << "\n\n";

    for (i = 0; i < n; i++)
      {
	ptr = heap.getMin();
	value = *ptr;
	cout << value << " ";
	delete ptr;
      }

    cout << "\n\n";
  }


}
