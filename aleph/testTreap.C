
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
# include <time.h>
# include <aleph.H>
# include <tpl_dynArray.H>
# include <tpl_sort_utils.H>
# include <tpl_treap.H>
# include <tpl_binNodeUtils.H>

using namespace std;
using namespace Aleph;

DynArray<unsigned long> rand_sequence;

long aleatorio()
{
  while (true)
    {
      // entre 1 y 1000
      unsigned long r = 1+ (int) (1000.0*rand()/(RAND_MAX+1.0));

      if (sequential_search(rand_sequence, r, 0, rand_sequence.size() - 1) ==
	  -1)
	{
	  rand_sequence[rand_sequence.size()] = r;
	  
	  return r;
	}
    }
}

void print_aleatorio_and_reset_dynarray()
{
  cout << endl
       << "Secuencia aleatorios: ";
  for (int i = 0; i < rand_sequence.size(); i++)
    cout << " " << (long) rand_sequence[i];
  
  cout << endl;

  rand_sequence.cut(0);
}

void printNode(Treap<int>::Node *node, int, int)
{
  cout << node->get_key() << " ";
}

void printNode(Treap<int>::Node *node, int, bool)
{
  cout << node->get_key() << " ";
}

void printPrio(Treap<int>::Node *node, int, int)
{
  cout << node->getPriority() << " ";
}

void printPair(Treap<int>::Node *node, int, int)
{
  cout << "(" << node->get_key() << "," << node->getPriority() << ") ";
}

int main(int argn, char *argc[])
{
  int n = 10;
  unsigned int t = time(0);
  int value;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "testTreapRec " << n << " " << t << endl;

  Treap<int> tree;
  Treap<int>::Node *node;
  int i;

  cout << "Inserting " << n << " random values in treee ...\n";

  for (i = 0; i < n; i++)
    { 
      do
	{
	  value = (int) (10.0*n*rand()/(RAND_MAX+1.0));
	  node = tree.search(value);
	}
      while (node not_eq NULL);

      node = new Treap<int>::Node (value);
      tree.insert(node);
      cout << "(" << value << "," << PRIO(node) << ") ";
    }

  cout << endl << endl
       << "level order" << endl;
  
  level_traverse(tree.getRoot(), [] (Treap<int>::Node * p)
		 {
		   cout << p->get_key() << " ";
		   return true;
		 });

  assert(is_treap(tree.getRoot()));

  cout << endl << endl
       << "Preorden" << endl;

  preOrderRec(tree.getRoot(), printNode);

  cout << endl << endl;

  cout << "inorden prio" << endl;
  inOrderRec(tree.getRoot(), printPrio);
  cout << endl << endl;

  print_aleatorio_and_reset_dynarray(); 

  cout << endl << endl;


  cout << endl << endl
       << "Preorden prio" << endl;

  preOrderRec(tree.getRoot(), printPrio);

  cout << endl << endl;

  cout << "inorden prio" << endl;
  inOrderRec(tree.getRoot(), printPrio);
  cout << endl << endl;

  //  print_aleatorio_and_reset_dynarray(); 

  cout << endl << endl;

  for (i = 0; i < n/2; i++)
    { 
      do
	{
	  value = (int) (10.0*n*rand()/(RAND_MAX+1.0));
	  node = tree.remove(value);
	} 
      while (node == NULL);

      cout << value << " ";
      delete node;
    }

  cout << endl << "verifying Treap after deletions ... " 
       << endl;
  assert(is_treap(tree.getRoot()));
  cout << " done" << endl;

  cout << "Preorden" << endl;
  preOrderRec(tree.getRoot(), printNode);
  cout << endl;

  cout << "inorden prio" << endl;
  inOrderRec(tree.getRoot(), printPrio);
  cout << endl;

  cout << "The path length is " << internal_path_length(tree.getRoot())
       << endl;

  destroyRec(tree.getRoot()); 

  cout << endl << "testTreapRec " << n << " " << t << endl;
}







