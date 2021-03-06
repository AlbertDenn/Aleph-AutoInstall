
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/


# include <stdlib.h>
# include <iostream>
# include <fstream>
# include <time.h>
# include <tpl_dynArray.H>
# include <tpl_rand_tree.H>
# include <tpl_treap.H>
# include <tpl_binNodeUtils.H>

using namespace Aleph;

gsl_rng * r = nullptr;

void test()
{
  using Node = Treap<unsigned long>::Node;
  Node 
    * p1 = new Node(5),
    * p2 = new Node(10),
    * p3 = new Node(15),
    * p4 = new Node(6),
    * p5 = new Node(12),
    * p6 = new Node(14);
  PRIO(p1) = 4;
  PRIO(p2) = 2;
  PRIO(p3) = 5;
  PRIO(p4) = 7;
  PRIO(p5) = 3;
  PRIO(p6) = 8;

  LLINK(p2) = p1; RLINK(p2) = p3;
  LLINK(p5) = p4; RLINK(p5) = p6;

  assert(is_treap(p2));
  assert(is_treap(p5));

  using Tree = Treap<unsigned long>;
  Tree t1, t2, dup;
  t1.getRoot() = p2;
  t2.getRoot() = p5;

  t1.join(t2, dup);
  assert(t1.verify() and check_bst(t1.getRoot()));
  assert(size(t2.getRoot()) == 0);
  assert(size(t1.getRoot()) + size(dup.getRoot()) == 6);
}


template <class Tree>
Tree create_tree(unsigned long n)
{
  Tree tree;
  using Node = typename Tree::Node;

  for (long i = 0; i < n; ++i)
    tree.insert_dup(new Node(gsl_rng_get(r)));

  assert(tree.verify());
  assert(check_bst(tree.getRoot()));

  return tree;
}

template <class Tree>
void test_join_dup(unsigned long n)
{
  cout << "Testing join_dup()" << endl
       << typeid(Tree).name() << endl
       << endl;
  Tree t1 = create_tree<Tree>(n);
  Tree t2 = create_tree<Tree>(n);

  t1.join_dup(t2);

  size_t count = 0;
  for (typename Tree::Iterator it(t1); it.has_curr(); it.next())
    count++;
  assert(count == 2*n);
  count = 0;
  for (typename Tree::Iterator it(t2); it.has_curr(); it.next())
    count++;
  assert(count == 0);

  assert(t1.verify() and t2.verify());

  destroyRec(t1.getRoot());
  destroyRec(t2.getRoot());

  cout << "Done!" << endl
       << endl;
}


template <class Tree>
void test_join(unsigned long n)
{
  using Node = typename Tree::Node;
  cout << "Testing join_dup()" << endl
       << typeid(Tree).name() << endl
       << endl;
  Tree t1 = create_tree<Tree>(n);
  Tree t2 = create_tree<Tree>(n);
  Tree dup;

  for_each_preorder(t1.getRoot(), [&t2] (auto p)
		    { t2.insert_dup(new Node(p->get_key())); });
  
  assert(size(t1.getRoot()) == n);
  assert(size(t2.getRoot()) == 2*n);

  t1.join(t2, dup);

  auto s1 = size(t1.getRoot());
  auto s2 = size(t2.getRoot());
  auto s3 = size(dup.getRoot());

  cout << "t1.size() = " << s1 << endl
       << "t2.size() = " << s2 << endl
       << "dup.size() = " << s3 << endl;

  assert(s2 == 0);

  assert(s1 + s3 == 3*n);

  assert(t1.verify() and t2.verify() and dup.verify());

  destroyRec(t1.getRoot());
  destroyRec(t2.getRoot());
  destroyRec(dup.getRoot());

  cout << "Done!" << endl
       << endl;
}


void usage()
{
  cout << "usage: " << endl
       << "    ./test n seed" << endl
       << endl;
  exit(0);
}

int main(int argn, char *argc[])
{
  // test(); exit(0);

  if (argn != 3)
    usage();

  unsigned long n = atoi(argc[1]);
  unsigned long seed = atoi(argc[2]);
  r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  cout << argc[0] << " " << n << " " << seed << endl;

  // test_join_dup<Rand_Tree<unsigned long>>(n);
  // test_join_dup<Treap<unsigned long>>(n);

  // test_join<Rand_Tree<unsigned long>>(n);
  test_join<Treap<unsigned long>>(n);


  gsl_rng_free(r);
}
