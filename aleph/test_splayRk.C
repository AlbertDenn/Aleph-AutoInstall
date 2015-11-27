
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
# include <gsl/gsl_rng.h>
# include <aleph.H>
# include <tpl_dynarray_set.H>
# include <tpl_splay_treeRk.H>
# include <tpl_binNodeUtils.H>
# include <tpl_sort_utils.H>

using namespace std;
using namespace Aleph;

DynArray<unsigned long> rand_sequence;


void printNode(Splay_Tree_Rk<int>::Node *node, int, int)
{
  cout << node->get_key() << " ";
}

int main(int argn, char *argc[])
{
  int n = 10;
  unsigned int seed = 0;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    seed = atoi(argc[2]);

  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);;
  gsl_rng_set(r, seed % gsl_rng_max(r));

  cout << argc[0] << " " << n << " " << seed << endl;

  DynArray_Set<int> keys; keys.reserve(n);
  Splay_Tree_Rk<int> tree;
  Splay_Tree_Rk<int>::Node *node;

  cout << "Inserting " << n << " random values in treee ...\n";

  for (int i = 0; i < n; i++)
    { 
      int value;
      do
	{
	  value = gsl_rng_uniform_int(r, 100*n);
	  node = tree.search(value);
	  assert(tree.verify());
	}
      while (node not_eq NULL);

      cout << value << " ";
      node = new Splay_Tree_Rk<int>::Node (value);
      tree.insert(node);
      assert(tree.verify());
      keys(i) = value;
    }
  cout << endl << endl;

  assert(check_rank_tree(tree.getRoot()));

  cout << "Sorting keys array" << endl;
  quicksort(keys);
  for (int i = 0; i < keys.size(); ++i)
    cout << keys(i) << " ";
  cout << endl
       << "done" << endl << endl;

  cout << "inorden traversal prio" << endl;
  inOrderRec(tree.getRoot(), printNode);
  cout << endl << endl;

  cout << "Testing select" << endl;

  for (int i = 0; i < n; i++)
    {
      node = tree.select(i);
      cout << node->get_key() << " ";
      assert(node->get_key() == keys(i));
    }

  cout << "done!" << endl << endl;

  assert(check_rank_tree(tree.getRoot()));

  cout << "testing random positions" << endl;
  for (int i = 0; i < n; ++i)
    {
      int idx = gsl_rng_uniform_int(r, keys.size());
      std::pair<int, Splay_Tree_Rk<int>::Node*> pos = 
	tree.position(keys(idx));
      assert(check_rank_tree(tree.getRoot()));
      assert(pos.second != NULL);
      assert(pos.second->get_key() == keys(idx));
      assert(idx == pos.first);
      cout << idx << "<-->" << pos.first << endl
	   << keys(i) << "<-->" << pos.second->get_key() << " " << endl;
    }

  for (int i = 0; i < n/2; i++)
    { 
      int idx = gsl_rng_uniform_int(r, keys.size());
      int & value = keys(idx);
      cout << value << " ";
      node = tree.remove(value);
      assert(node != NULL);
      assert(node->get_key() == value);
      delete node;
      keys.remove(value);
    }

  cout << endl << "verifying Splay_Rk after deletions ... " 
       << endl;
  assert(check_rank_tree(tree.getRoot()));
  cout << " done" << endl;

  cout << "Preorden" << endl;
  inOrderRec(tree.getRoot(), printNode);
  cout << endl;

  cout << "The path length is " << internal_path_length(tree.getRoot())
       << endl;

  destroyRec(tree.getRoot()); 

  cout << endl << argc[0] << " " << n << " " << seed << endl;
 
  gsl_rng_free(r);
}







