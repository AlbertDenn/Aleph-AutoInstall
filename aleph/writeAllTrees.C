
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
# include <time.h>
# include <iostream>
# include <fstream>
# include <aleph.H>
# include <tpl_splay_tree.H>
# include <tpl_avl.H>
# include <tpl_binTree.H>
# include <tpl_rb_tree.H>
# include <tpl_rand_tree.H>
# include <tpl_treap.H>
# include <tpl_binNodeUtils.H>

using namespace Aleph;
using namespace std;


ofstream output_splay("splay.Tree", ios::out);

void print_splay(Splay_Tree<int>::Node * p, int, int)
{
  output_splay << p->get_key() << " ";
}


ofstream output_bin("bin.Tree", ios::out);

void print_bin(BinTree<int>::Node * p, int, int)
{
  output_bin << p->get_key() << " ";
}


ofstream output_rb("rb.Tree", ios::out);

void print_rb(Rb_Tree<int>::Node * p, int, int)
{
  output_rb << p->get_key() << " ";
}


ofstream output_avl("avl.Tree", ios::out);

void print_avl(Avl_Tree<int>::Node * p, int, int)
{
  output_avl << p->get_key() << " ";
}


ofstream output_rand("rand.Tree", ios::out);

void print_rand(Rand_Tree<int>::Node * p, int, int)
{
  output_rand << p->get_key() << " ";
}


ofstream output_treap("treap.Tree", ios::out);

void print_treap(Treap<int>::Node * p, int, int)
{
  output_treap << p->get_key() << " ";
}



int main(int argn, char *argc[])
{
  int n = 512;
  unsigned int t = time(0);
  int value;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "writeAllTrees " << n << " " << t << endl;

  {
    srand(t);
    Splay_Tree<int> tree;
    Splay_Tree<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new Splay_Tree<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_splay);

    destroyRec(tree.getRoot());
  }

  {
    srand(t);
    Rand_Tree<int> tree;
    Rand_Tree<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new Rand_Tree<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_rand);

    destroyRec(tree.getRoot());
  }

  {
    srand(t);
    Rb_Tree<int> tree;
    Rb_Tree<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new Rb_Tree<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_rb);

    destroyRec(tree.getRoot());
  }

  {
    srand(t);
    BinTree<int> tree;
    BinTree<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new BinTree<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_bin);

    destroyRec(tree.getRoot());
  }

  {
    srand(t);
    Avl_Tree<int> tree;
    Avl_Tree<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new Avl_Tree<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_avl);

    destroyRec(tree.getRoot());
  }

  {
    srand(t);
    Treap<int> tree;
    Treap<int>::Node *node;
    int i;

    cout << "Inserting " << n << " random values in treee ...\n";

    for (i = 0; i < n; i++)
      {
	while (true)
	  {
	    value = 1+(int) (n*1.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	    if (node == NULL)
	      break;
	  }
	node = new Treap<int>::Node (value);
	tree.insert(node);
      }

    preOrderRec(tree.getRoot(), print_treap);

    destroyRec(tree.getRoot());
  }
}
