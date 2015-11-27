
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
# include <aleph.H>
# include <tpl_dynArray.H>
# include <tpl_binHeap.H>
# include <tpl_binNodeUtils.H>

using namespace std;
using namespace Aleph;

static void printNode(BinHeap<int>::Node* node, int, int)
{
  cout << node->get_key() << " ";
}


int main(int argn, char *argc[])
{
  int n = 1000;
  unsigned int t = time(0);

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "testBinHeap " << n << " " << t << endl;

  BinHeap<int> heap;
  BinHeap<int>::Node *node;
  int i;

  for (i = n - 1; i >= 0; i--)
    {
      node = new BinHeap<int>::Node (i);
      heap.insert(node);
    }

  assert(heap.verify_heap());

  for (i = 0; i < n; i++)
    {
      node = heap.getMin();
      delete node;
    }

  assert(heap.verify_heap());  

  int value;

  for (i = n - 1; i >= 0; i--)
    {
      value = (int) (n*100.0*rand()/(RAND_MAX+1.0));
      node = new BinHeap<int>::Node (value);
      heap.insert(node);
      cout << value << " ";
    }
  cout << endl;

  assert(heap.verify_heap());

  for (i = 0; i < n; i++)
    {
      node = heap.getMin();
      delete node;
    }

  assert(heap.verify_heap());  
  assert(heap.size() == 0);

  for (i = n - 1; i >= 0; i--)
    {
      value = (int) (n*100.0*rand()/(RAND_MAX+1.0));
      node = new BinHeap<int>::Node (value);
      heap.insert(node);
    }

  assert(heap.verify_heap());

  for (i = 0; i < n/2; i++)
    {
      node = heap.getMin();
      delete node;
    }
  assert(heap.verify_heap());  

  for (i = n - 1; i >= 0; i--)
    {
      value = (int) (n*100.0*rand()/(RAND_MAX+1.0));
      node = new BinHeap<int>::Node (value);
      heap.insert(node);
    }
  assert(heap.verify_heap());  

  for (i = 0; i <= n + n/2; i++)
    {
      try 
	{
	  node = heap.getMin();
	  delete node;
	}
      catch (exception & e)
	{
	  cout << e.what() << endl;
	}
    }
    assert(heap.verify_heap());  
    assert(heap.size() == 0);


    DynArray<BinHeap<int>::Node*> nodes(n);
    //    BinHeap<int>::Node* nodes[n];
    n = 4;
    for (i = 2*n - 1; i >= 0; i--)
      {
	node = new BinHeap<int>::Node (i);
	heap.insert(node);
	nodes[i] = node;
      }

    assert(heap.verify_heap());  

    for (i = 0; i < n/2; i++)
      {
	value = (int) (1.0*n*rand()/(RAND_MAX+1.0));
	if (nodes[value] != NULL)
	  {
	    node = heap.remove(nodes[value]);
	    delete node;
	    nodes[value] = NULL;
	  }
      }

    assert(heap.verify_heap());  

    heap.remove_all_and_delete();

    for (i = 0; i < n/2; i++)
      {
	value = (int) (n*100.0*rand()/(RAND_MAX+1.0));
	node = new BinHeap<int>::Node (value);
	heap.insert(node);
      }
    assert(heap.verify_heap());  

    

    while (heap.size() > 0)
      {
	node = heap.getMin();
	delete node;
      }

    assert(heap.verify_heap());  
    assert(heap.size() == 0);

    for (i = n - 1; i >= 0; i--)
      {
	node = new BinHeap<int>::Node (i);
	heap.insert(node);
      }
    assert(heap.verify_heap());  

    for (i = 0; i < n; i++)
      {
	node = heap.getMin();
	delete node;
      }
    assert(heap.verify_heap());  
    assert(heap.size() == 0);

    cout << "End" << endl;
}
