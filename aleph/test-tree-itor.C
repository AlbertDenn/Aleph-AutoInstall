
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


# include <gsl/gsl_rng.h>
# include <iostream>
# include <tpl_dynBinHeap.H>
# include <tpl_dynSetTree.H>

using namespace std;


template <class Tree>
void test(size_t n, unsigned long seed)
{
  cout << "Testing for " << typeid(Tree).name() << endl
       << endl;
  auto r= gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  Tree tree;
  for (auto i = 0; i < n; ++i)
    {
      auto val = gsl_rng_get(r);
      auto p = tree.insert(new typename Tree::Node(val));
      if (p == nullptr)
	delete p;
    }

  {
    typename Tree::Iterator it = tree;
    auto it_c = it;
    auto it_m = move(it_c);
  }

  for (typename Tree::Iterator it(tree); it.has_curr(); it.next())
    cout << it.get_curr()->get_key() << " " << endl;
  cout << endl;

  destroyRec(tree.getRoot());
  gsl_rng_free(r);
}

void usage()
{
  cout << "test-tree-itor [n] [seed]" << endl
       << endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  size_t n = argc > 1 ? atoi(argv[1]) : 10;
  unsigned long seed = argc > 2 ? atoi(argv[2]) : 0;

  test<BinTree<long>>(n, seed); 
  test<Avl_Tree<long>>(n, seed); 
  test<Splay_Tree<long>>(n, seed); 
  test<Rb_Tree<long>>(n, seed);
  test<Rand_Tree<long>>(n, seed);
  test<Treap<long>>(n, seed);
  test<Treap_Rk<long>>(n, seed);
}

