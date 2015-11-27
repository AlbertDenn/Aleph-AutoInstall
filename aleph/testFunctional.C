
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
# include <stdlib.h>
# include <cassert>
# include <iostream>
# include <ahFunctional.H>
# include <ahSearch.H>
# include <htlist.H>
# include <tpl_dynDlist.H>
# include <tpl_dynSetTree.H>
# include <tpl_hash.H>

using namespace std;

gsl_rng * r;

template <template <class> class C, typename T>
void print_seq(const C<T> & c)
{
  for (typename C<T>::Iterator it(c); it.has_curr(); it.next())
    cout << it.get_curr() << " ";

  cout << endl;
}

int main(int argc, char *argv[])
{
  int n = argc > 1 ? atoi(argv[1]) : 100;
  unsigned long seed = argc > 2 ? atoi(argv[2]) : time(NULL);

  cout << argv[0] << " " << n << " " << seed << endl;

  r = gsl_rng_alloc (gsl_rng_mt19937);

  DynList<int> l1({1,2,3,4,5,6,7,8,9});

  cout << "** indexes(l1) = ";
  indexes(l1).for_each([] (pair<size_t, const int &> t)
	    {
	      cout << "(" << t.first << "," << t.second << ")";
	    });
  cout << endl;

  DynDlist<int> l2({1,2,3,4,5,6,7,8,9});
  HashSet<int> l3({0, 1,2,3,4,5,6,7,8,9});

  auto to_str = /* Lambda */ [&l1] (int i) -> std::string
    {					    
      return std::to_string(i);
    };

  DynList<std::string> l1_str = 
    map_items<int, DynList, std::string>(l1, to_str);

  print_seq(l1);
  print_seq(l2);
  print_seq(l3.keys());
  print_seq(l1_str);

  DynList<std::pair<int, int>> lz1 = zip(DynList<int>(l1), DynDlist<int>(l2));

  for (DynList<std::pair<int,int>>::Iterator it(lz1); it.has_curr(); it.next())
    cout << it.get_curr().first << "," << it.get_curr().second << endl;

  {
    cout << "Building random list" << endl;
    DynList<long> l;
    for (long i = 0; i < n; ++i)
      l.append(gsl_rng_get(r));

    cout << endl
	 << "building array copy of previuos list" << endl;
    DynArray<long> a;
    l.for_each(/* Lambda */ [&a](long i)
	       {
		 //cout << i << endl;
		 a.append(i);
	       });

    cout << endl
	 << "Copying array to a tree" << endl;
    DynSetTree<long> tree;
    a.for_each(/* Lambda */ [&tree] (long i)
	       {
		 tree.append(i);
	       });

    cout << endl
	 << "Sorting array for ulterior fast searching";
    quicksort_op(a);

    cout << endl
	 << "traversing all the keys of tree and verifiying them with array" 
	 << endl;
    assert(tree.all(/* Lambda */ [&a] (long i)
		    {
		      return a(binary_search(a, i)) == i;
		    }));
    cout << "done!" << endl
	 << endl
	 << "Traversing all the keys of array and verifiying then with the tree"
	 << endl;
    assert(a.all(/* Lambda */ [&tree] (long i)
		 {
		   return tree.search(i) != NULL;
		 }));
    cout << "done!" << endl
	 << endl;
  }

  cout << "Take 1/4:";
  range(10).take(3).for_each([] (long i) { cout << " " << i; }); 
  cout << endl
       << "Drop 1/4";
  range(10).drop(3).for_each([] (long i) { cout << " " << i; }); 
  cout << endl;

  gsl_rng_free(r);
  return 0;
}
