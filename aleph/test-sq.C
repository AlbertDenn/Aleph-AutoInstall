
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
# include <ahSort.H>
# include <tpl_arrayStack.H>
# include <tpl_dynListStack.H>
# include <tpl_dynListQueue.H>
# include <tpl_arrayQueue.H>

template <template <typename __T> class Container, typename T> 
Container<T> copy(const Container<T> & s)
{
  Container<T> ret(s);
  return ret;
}

template <template <typename __T> class Container, typename T> 
void print(const Container<T> & s)
{
  cout << "(" << s.size() << ")";
  s.items().for_each([] (const T & d) { cout << " " << d; });
  cout << endl;
}

template <template <typename __T> class Stack, typename T> 
void check_lifo()
{
  Stack<T> s;
  for (int i = 0; i < 10; ++i)
    s.push(i);

  assert(s.top() == 9);

  const Stack<T> aux = s;
  assert(aux.top() == 9);
  assert(eq(s.items(), aux.items()));

  for (int i = 9; i >= 0; --i)
    {
      assert(s.top() == i);
      assert(s.pop() == i);
    }
  assert(s.size() == 0);
  assert(s.is_empty());
}

template <template <typename __T> class Stack, typename T> 
void test_stack()
{
  check_lifo<Stack, T>();

  Stack<T> s1;
  for (int i = 0; i < 10; ++i)
    s1.push(i);

  const Stack<T> s2 = copy(s1);

  Stack<T> s3;
  Stack<T> s4;

  s3 = s2;
  s4 = copy(s1);

  print(s1);
  print(s2);
  print(s3);
  print(s4);

  assert(eq(sort(s1.items()), sort(s2.items())));
  assert(eq(sort(s3.items()), sort(s4.items())));
}

template <template <typename __T> class Queue, typename T> 
void check_fifo()
{
  Queue<T> q;
  for (int i = 0; i < 10; ++i)
    q.put(i);

  assert(q.front() == 0);
  assert(q.rear() == 9);

  const Queue<T> aux = q;
  assert(aux.front() == 0);
  assert(aux.rear() == 9);
  assert(eq(q.items(), aux.items()));


  for (int i = 0; i < 10; ++i)
    {
      assert(q.front() == i);
      assert(q.get() == i);
    }
  assert(q.size() == 0);
  assert(q.is_empty());
}

template <template <typename __T> class Queue, typename T> 
void test_queue()
{
  check_fifo<Queue, T>();

  Queue<T> q1;
  for (int i = 0; i < 10; ++i)
    q1.put(i);

  Queue<T> q2 = q1;

  print(q1);
  print(q2);

  assert(eq(q1.items(), q2.items()));

  Queue<int> q3 = copy(q1);
  q3 = q2;
  Queue<int> q4 = q1;
  q4 = copy(q1);
  
  assert(eq(q3.items(), q3.items()));

  print(q3);
  print(q4);
}

template <template <typename T> class C>
void test_rank(size_t n, unsigned long seed)
{
  auto r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  C<unsigned long> l;

  cout << "Test rank for " << typeid(l).name() << endl
       << endl;

  for (auto i = 0; i < n; ++i)
    l.append(gsl_rng_get(r));

  auto ranks = Aleph::ranks(l);
  DynArray<unsigned long> s = sort(l);

  size_t i = 0;
  assert(l.all([&i, &s, &ranks] (auto item)
  	       {
  		 return s(ranks(i++)) == item;
  	       }));

  cout << "Original sequence" << endl;
  l.for_each([] (auto p) { cout << p << " "; });
  cout << endl
       << endl
       << "Ranks" << endl;

  // pair_ranks(l).for_each([] (auto p) 
  // 			 {
  // 			   cout << "(" << p.first << "," << p.second << ")";
  // 			 });
  // cout << endl
  //      << endl
  //      << "sorted" << endl;
  // s.for_each([] (auto p) { cout << p << " "; });
  // cout << endl
  //      << endl;

  // This line causes an assertion violation in llvm 3.6.1
  assert(pair_ranks(l).all([&s] (auto p) { return p.first == s(p.second); }));

  gsl_rng_free(r);

  cout << "end for " << typeid(l).name() << endl
       << endl;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    {
      cout << "usage" << endl
	   << "    " << argv[0] << " n seed" << endl;
      exit(0);
    }

  size_t n = atoi(argv[1]);
  unsigned long seed = atoi(argv[2]);

  cout << "DynListStack" << endl;
  test_stack<DynListStack, int>();

  cout << endl
       << "ArrayStack" << endl;
  test_stack<ArrayStack, int>();

  cout << endl
       << "FixedStack" << endl;
  test_stack<FixedStack, int>();

  cout << endl
       << "DynListQueue" << endl;
  test_queue<DynListQueue, int>();

  cout << endl
       << "ArrayQueue" << endl;
  test_queue<ArrayQueue, int>();

  cout << endl
       << "FixedQueue" << endl;
  test_queue<FixedQueue, int>();

  cout << endl;

  test_rank<DynArray>(n, seed);

  test_rank<DynList>(n, seed);

  test_rank<DynDlist>(n, seed);
}

