
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
# include <gsl/gsl_rng.h>
# include <tpl_dynarray_set.H>
# include <tpl_splay_tree.H>

using namespace std;


void print_node(Splay_Tree<int>::Node * p, int, int)
{
  cout << KEY(p) << " " ;
}


void write_node(Splay_Tree<int>::Node * p, int, int)
{
  cout << KEY(p) << " " ;
}


int main(int argn, char *argc[])
{
  int n = 1000;
  unsigned int seed = 0;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    seed = atoi(argc[2]);

  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);;
  gsl_rng_set(r, seed % gsl_rng_max(r));

  cout << "test_Splay_Tree " << n << " " << seed << endl;

  Splay_Tree<int> tree;
  Splay_Tree<int>::Node * p;

  DynArray_Set<int> values;

  cout << "Testing for insertions" << endl;

  for (int i = 0; i < n; i++)
    {
      int value;
      do
	{
	  value = gsl_rng_uniform_int(r, 100*n);
	  p = tree.search(value);
	}
      while (p != NULL);

      cout << value << " ";
      p = new Splay_Tree<int>::Node (value);
      tree.insert(p);
      values.append(value);
    }

  cout << endl << "Inorder " << endl;
  inOrderRec(tree.getRoot(), print_node);

  cout << endl << "Preorder " << endl;
  preOrderRec(tree.getRoot(), print_node);

  cout << endl << endl;

  cout << "Testing for random searches" << endl;
  for (int i = 0; i < n; ++i)
    {
      int idx = gsl_rng_uniform_int(r, values.size());
      int value = values(idx);
      cout << value << " ";
      if (tree.search(value) == NULL)
	ERROR("BUG detected ins searching");
    }
  cout << "Done" << endl;
  
  cout << "Removing test" << endl;

  for (int i = 0; i < n/7; i++)
    {       
      int & value = values(gsl_rng_uniform_int(r, values.size()));
      p = tree.remove(value);
      cout << value << " ";
      assert(p != NULL);
      assert(value == p->get_key());
      delete p;
      values.remove(value);
    }

  cout << endl << "Inorder " << endl << endl;
  inOrderRec(tree.getRoot(), print_node);

  cout << endl;

  destroyRec(tree.getRoot());

  DynList<int>({122, 363, 1247, 510, 701, 1565, 1157, 728, 1564, 492, 861, 422}).
    for_each([&tree] (int k)
	     {
	       tree.insert(new Splay_Tree<int>::Node(k));
	     });
  cout << endl << "Preorder " << endl;
  preOrderRec(tree.getRoot(), print_node);

  destroyRec(tree.getRoot());
  gsl_rng_free(r);

  cout << endl;
}
