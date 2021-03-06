
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


# include <iostream>
# include <vector>
# include <ahSort.H>
# include <htlist.H>
# include <tpl_arrayHeap.H>
# include <tpl_dynArrayHeap.H>
# include <tpl_dynBinHeap.H>
# include <tpl_dynDlist.H>
# include <tpl_dynSetTree.H>
# include <tpl_olhash.H>
# include <tpl_odhash.H>
# include <tpl_dynSetHash.H>
# include <tpl_dynArray.H>
# include <tpl_arrayQueue.H>
# include <tpl_dynListStack.H>
# include <tpl_dynarray_set.H>
# include <tpl_random_queue.H>
 
using namespace std;

using namespace Aleph;

template <class C>
void find_test()
{
  C c = { 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 };

  const C a = c;

  c.traverse([] (auto i) { cout << " " << i; return true; }); cout << endl;

  a.traverse([] (auto i) { cout << " " << i; return true; }); cout << endl;

  assert(c.all([] (auto i) { return i >= 0; }));
  assert(a.all([] (auto i) { return i >= 0; }));

  int vals[11]; int k = 0;
  c.for_each([&k, &vals] (int i) { vals[k++] = i; });

  c.for_each([&c, vals] (int i) { assert(c.nth(i) == vals[i]); });

  k = 0;
  a.for_each([&k, &vals] (int i) { vals[k++] = i; });
  a.for_each([&a, vals] (int i) { assert(a.nth(i) == vals[i]); });

  assert(c.find_ptr([] (int i) { return i == 5; }));
  assert(a.find_ptr([] (int i) { return i == 5; }));
  assert(not c.find_ptr([] (int i) { return i == 15; }));
  assert(not a.find_ptr([] (int i) { return i == 15; }));
  assert(get<0>(c.find_item([] (int i) { return i == 5; })));
  assert(get<0>(c.find_item([] (int i) { return i == 5; })) and
  	 get<1>(c.find_item([] (int i) { return i == 5; })) == 5);
  assert(get<0>(a.find_item([] (int i) { return i == 5; })));
  assert(get<0>(a.find_item([] (int i) { return i == 5; })) and
  	 get<1>(a.find_item([] (int i) { return i == 5; })) == 5);

}

template <class C>
void ctors_test()
{
  C c = { 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 };
  const C a = c;
  c.traverse([] (auto i) { cout << " " << i; return true; }); cout << endl;
  a.traverse([] (auto i) { cout << " " << i; return true; }); cout << endl;

  C c1 = DynList<typename C::Item_Type>({ 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 });
  c1.for_each([] (int i) { cout << " " << i; }); cout << endl;

  const C c2 = DynList<typename C::Item_Type>({ 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 });
  c2.for_each([] (int i) { cout << " " << i; }); cout << endl;

  vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 };

  C c3(v.begin(), v.end());
  c3.for_each([] (int i) { cout << " " << i; }); cout << endl;

  const C c4(v.begin(), v.end());
  c4.for_each([] (int i) { cout << " " << i; }); cout << endl;
}

template <class C>
void functional_test()
{
  C c = { 0, 1, 2, 3, 4, 5, 6, 7, 8 ,9 };
  const C a = c;

  c.for_each([] (int i) { cout << " " << i; }); cout << endl;
  a.for_each([] (int i) { cout << " " << i; }); cout << endl;

  assert(c.all([&a] (int i) { return a.exists([i] (int k) 
						  { return k == i; }); }));
  assert(a.all([&c] (int i) { return c.exists([i] (int k)
						{ return k == i; }); }));

  assert(c.exists([] (int i) { return i == 9; }));
  assert(a.exists([] (int i) { return i == 9; }));

  assert(c.all([&c] (int i) 
   		 { return c.exists([i] (int k) { return i == k; }); }));
  assert(a.all([&a] (int i) 
		   { return a.exists([i] (int k) { return i == k; }); }));

  C cm = c.map([] (int i) { return 10*i; });
  assert(cm.all([&c] (int k) 
		{
		  return c.exists([k] (int i) { return 10*i == k; });
		}));
		
  const C ccm = a.map([] (int i) { return 10*i; });
  assert(ccm.all([a] (int k) 
		 {
		   return a.exists([k] (int i) { return 10*i == k; });
		 }));

  {
    auto m = c.template map<string>([] (int i) { return to_string(i); });
  }

  cout << "S1 = " 
       << c.template foldl<int>(0, [] (auto a, auto i) { return a + i; }) 
       << endl
       << "S2 = " 
       << ccm.template foldl<int>(0, [] (int a, int i) { return a + i; }) 
       << endl
       << "S3 = " << c.fold(0, [] (auto a, auto i) { return a + i; }) 
       << endl
       << "S4 = " << a.fold(0, [] (auto a, auto i) { return a + i; }) 
       << endl
       << endl;
  
  assert(eq({0, 1, 2, 3, 4, 5}, sort(c.filter([] (int i) { return i < 6; }))));
  assert(eq({0, 1, 2, 3, 4, 5}, sort(a.filter([] (int i) { return i < 6; }))));
  
  c.pfilter([] (int i) { return i < 6; }).for_each([] (auto p)
    {
      cout << "(" << get<0>(p) << "," << get<1>(p) << ")";
    });
  cout << endl;
  a.pfilter([] (int i) { return i < 6; }).for_each([] (auto p)
    {
      cout << "(" << get<0>(p) << "," << get<1>(p) << ")";
    });
  cout << endl
       << endl;

  auto cmp_tup = [] (std::tuple<size_t,size_t> t1, std::tuple<size_t,size_t> t2)
    {
      return get<0>(t1) < get<0>(t2);
    };

  auto l1 = sort(c.pfilter([] (int i) { return i < 6; }), cmp_tup);
  auto l2 = sort(a.pfilter([] (int i) { return i < 6; }), cmp_tup);

  l1.for_each([] (auto p)
    {
      cout << "(" << get<0>(p) << "," << get<1>(p) << ")";
    });
  cout << endl;
  l2.for_each([] (auto p)
    {
      cout << "(" << get<0>(p) << "," << get<1>(p) << ")";
    });
  cout << endl
       << endl;

  auto eq_tup = [] (std::tuple<size_t,size_t> t1, std::tuple<size_t,size_t> t2)
    {
      return get<0>(t1) == get<0>(t2);
    };
  assert(eq(l1 ,l2, eq_tup));

  auto p = c.partition([] (int i) { return i < 6; });
  assert(eq(sort(p.first), {0, 1, 2, 3, 4, 5}) and 
	 eq(sort(p.second), {6, 7, 8, 9}));
  p = a.partition([] (int i) { return i < 6; });
  assert(eq(sort(p.first), {0, 1, 2, 3, 4, 5}) and
	 eq(sort(p.second), {6, 7, 8, 9}));

  auto t = c.tpartition([] (int i) { return i < 6; });
  assert(eq(sort(get<0>(t)), {0, 1, 2, 3, 4, 5}) and
	 eq(sort(get<1>(t)), {6, 7, 8, 9}));
  t = a.tpartition([] (int i) { return i < 6; });
  assert(eq(sort(get<0>(t)), {0, 1, 2, 3, 4, 5}) and
	 eq(sort(get<1>(t)), {6, 7, 8, 9}));

  assert(c.length() == 10);
  assert(a.length() == 10);

  c.take(3).for_each([] (auto i) { cout << i << " "; });
  cout << endl;
  a.take(3).for_each([] (auto i) { cout << i << " "; });
  cout << endl
       << endl;

  auto cc = c;
  auto ca = a;

  c.take(3).for_each([] (auto i) { cout << i << " "; });
  cout << endl;
  cc.take(3).for_each([] (auto i) { cout << i << " "; });
  cout << endl
       << endl;

  assert(eq(sort(join(c.take(3), c.drop(3))), sort(c.items())));
  assert(eq(sort(join(a.take(3), a.drop(3))), sort(a.items())));

  cout << "All test were passed!" << endl
       << endl
       << endl;
}

template <class C>
void tests()
{
  cout << "Testing for " << typeid(C).name() << endl
       << endl;

  find_test<C>();
  ctors_test<C>();
  functional_test<C>();

  cout << "Ended tests for " << typeid(C).name() << endl
       << endl;
}

int main()
{
  DynList<int> c = {0, 1, 2, 3};
  DynDlist<int> cc = c;
  tests<DynList<int>>();
  tests<DynDlist<int>>();
  tests<DynSetTree<int>>();
  tests<OLhashTable<int>>();
  tests<ODhashTable<int>>();
  tests<DynSetHash<int>>();
  tests<ArrayHeap<int>>();
  tests<DynArray<int>>();
  tests<DynArrayHeap<int>>();
  tests<DynBinHeap<int>>();
  tests<ArrayQueue<int>>();
  tests<FixedQueue<int>>();
  tests<ArrayStack<int>>();
  tests<FixedStack<int>>();
  tests<DynListStack<int>>();
  tests<DynListQueue<int>>();
  tests<DynArray_Set<int>>();
  tests<Random_Set<int>>();
}




