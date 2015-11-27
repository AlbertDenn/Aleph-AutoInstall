
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


# include <cassert>
# include <tpl_dynSetTree.H>

template <template <typename, class> class Tree = Rand_Tree> 
inline ostream & operator << (ostream & s, const DynSetTree<long, Tree> & t)
{
  t.for_each([&s] (long i) { s << i << " "; });
  cout << "(" << t.size() << ")";
  return s;
}

template <template <typename, class> class Tree = Rand_Tree> 
void test_split_pos(size_t n)
{
  DynSetTree<long, Tree> t;
  for (int i = 0; i < n; ++i)
    t.insert(i);

  DynSetTree<long, Tree> t1, t2;

  try
    {
      t.split_pos(n, t1, t2);
      cout << "ERROR" << endl;
    }
  catch (out_of_range)
    {
      cout << "Exception ok" << endl;
    }

  t.split_pos((n-1)/2, t1, t2);

  assert(t.is_empty());
  assert(t1.size() == (n-1)/2 or t1.size() == (n-1)/2 + 1);
  assert(t2.size() == (n-1)/2 or t2.size() - 1);
  assert(check_rank_tree(t1.get_root_node()));
  assert(check_rank_tree(t2.get_root_node()));

  t1.join_dup(t2);
  t.swap(t1);
  
  assert(t1.is_empty() and t2.is_empty());
  assert(t.size() == n);
  assert(check_rank_tree(t.get_root_node()));

  t.split_pos(0, t1, t2);
  assert(t1.min() == t1.max() and t1.min() == 0);
  t.join_dup(t1.join_dup(t2));

  t.split_pos(n - 1, t1, t2);
  cout << t1 << endl
       << t2 << endl;
}

template <template <typename, class> class Tree = Rand_Tree> 
void test_split_key(size_t n)
{
  DynSetTree<long, Tree> t;
  for (int i = 0; i < n; ++i)
    t.insert(i);

  DynSetTree<long, Tree> t1, t2;

  t.split_key_dup(n, t1, t2);
  assert(t1.size() == n and t2.is_empty());

  t.join_dup(t1);
  assert(t.size() == n and t1.is_empty());

  t.split_key_dup(n/2, t1, t2);
  
  cout << "split_key(" << n/2 << ")" << endl
       << t1 << endl
       << t2 << endl;

  assert(t.is_empty());
  assert(t1.size() == (n-1)/2 or t1.size() == (n-1)/2 + 1);
  assert(t2.size() == (n-1)/2 or t2.size() - 1);
  assert(check_rank_tree(t1.get_root_node()));
  assert(check_rank_tree(t2.get_root_node()));

  t1.join_dup(t2);
  t.swap(t1);
  
  assert(t1.is_empty() and t2.is_empty());
  assert(t.size() == n);
  assert(check_rank_tree(t.get_root_node()));


  t.split_key_dup(0, t1, t2);

  cout << "split_key(0)" << endl
       << t1 << endl
       << t2 << endl;

  assert(t1.is_empty() and t2.size() == n);
  t.join_dup(t1.join_dup(t2));

  t.split_key_dup(n - 1, t1, t2);
  cout << "split_key(" << n - 1 << ")" << endl
       << t1 << endl
       << t2 << endl;

  t.join_dup(t1.join_dup(t2));

  t.split_key_dup(n, t1, t2);
  cout << "split_key(" << n << ")" << endl
       << t1 << endl
       << t2 << endl;
}


int main(int, char *argv[])
{
  size_t n = atoi(argv[1]);

  // test_split_pos<Rand_Tree>(n);

  // test_split_pos<Treap_Rk>(n); 

  test_split_key<Rand_Tree>(n);

  return 0;
}
