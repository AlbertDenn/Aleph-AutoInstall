
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


# include <tpl_binNode.H>
# include <tpl_avl.H>
# include <tpl_binNodeUtils.H>
# include <tpl_arrayQueue.H>
# include <iostream>

using namespace std;
using namespace Aleph;

unsigned int fib(unsigned int n)
{
  if (n == 1 || n == 0)
    return n;

  unsigned int fi_1 = 1, fi_2 = 0, fi = 0, i;

  for (i = 2; i <= n; i++, fi_2 = fi_1, fi_1 = fi)
    fi = fi_1 + fi_2;

  return fi;
}

static int sum = 0;

static void sum_node(BinNode<int> *node, int, int)
{
  node->get_key() = sum++;
}

void build_avl(Avl_Tree<int> &tree, BinNode<int> * p)
{
  ArrayQueue<BinNode<int> *> queue;

  queue.put(p);

  while (not queue.is_empty())
    {
      p = queue.get();

      Avl_Tree<int>::Node * node = new Avl_Tree<int>::Node(KEY(p));

      tree.insert(node);

      if (RLINK(p) not_eq NULL)
	queue.put(RLINK(p));

      if (LLINK(p) not_eq NULL)
	queue.put(LLINK(p));
    }
}


static BinNode<int> * __fibonacci(int k)
{             
  if (k <= 0)
    return NULL;

  if (k == 1)
    return new BinNode<int> (0);

  int label = fib(k + 1);

  BinNode<int> * p = new BinNode<int> (label - 1);

  p->getL() = __fibonacci(k - 2);
  p->getR() = __fibonacci(k - 1);

  return p;
}

static BinNode<int> * fibonacci(int k)
{             
  BinNode<int> * p = __fibonacci(k);

  sum = 0;

  if (k > 2)
    inOrderRec(p, sum_node);

  return p;
}


void print(Avl_Tree<int>::Node *p, int, int)
{
  cout << p->get_key() << " ";
}

int main(int, char *argc[])
{
  int n = atoi(argc[1]);

  BinNode<int> * p = fibonacci(n);

  Avl_Tree<int> tree;

  build_avl(tree, p);

  assert(is_avl(tree.getRoot()));

  cout << "**** " << fib(n) << endl;
  preOrderRec(tree.getRoot(), &print);
  cout << endl << endl;

  int k = 0;
  //  int k = fib(n + 2) - 2;

  cout << "Borrando " << k;

  Avl_Tree<int>::Node * q = tree.remove(k);

  if (q == NULL)
    cout << " ... no encontrado" << endl;
  else
    {
      cout << " ... borrado" << endl;
      delete q;
    }

  cout << "**** " << fib(n) << endl;
  preOrderRec(tree.getRoot(), &print);
  cout << endl << endl;

  destroyRec(p);
  destroyRec(tree.getRoot());
}
