
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


# include <typeinfo>
# include <ahIterator.H>
# include <htlist.H>
# include <tpl_dynDlist.H>
# include <tpl_dynSetTree.H>
# include <tpl_dynSetHash.H>
# include <tpl_dynArray.H>
# include <tpl_dynArrayHeap.H>
# include <tpl_memArray.H>
# include <tpl_arrayStack.H>
# include <tpl_arrayQueue.H>
# include <tpl_dynListQueue.H>
# include <tpl_dynListStack.H>
# include <tpl_arrayHeap.H>
# include <tpl_dynArrayHeap.H>
# include <tpl_dynBinHeap.H>

struct Wrapper
{
  size_t i = 0;
  size_t j;
  Wrapper(size_t ii) : i(ii), j(i+1) {}
  Wrapper() {}

  bool operator < (const Wrapper & w) const { return i < w.i; }
  bool operator == (const Wrapper & w) const { return i == w.i; }

  void swap(Wrapper & w) { std::swap(i, w.i); std::swap(j, w.j); }

  Wrapper(const Wrapper & w) : i(w.i), j(w.j) {}
  Wrapper(Wrapper && w) { swap(w); }

  Wrapper & operator = (const Wrapper & w) { i = w.i; j = w.j; return *this; }
};



template <class Container>
void test(size_t n)
{
  cout << "Building and inserting for type" << typeid(Container).name()
       << endl;
  Container c;
  for (size_t i = 0; i < n; i++)
    c.append(Wrapper(i));

  size_t sum = 0;
  cout << "Inserted " << n << " items in container" << endl
       << endl
       << endl
       << "Testing aleph-w get_itor()" << endl;
  for (auto it = c.get_itor(); it.has_curr(); it.next())
    {
      auto i = it.get_curr();
      cout << "(" << i.i << ",";
      sum += i.i;
      i.j++;      
      cout << i.j << ")";
    }

  cout << endl 
       << endl
       << "Testing range-based loop" << endl;
  for (auto i : c)
    {
      cout << "(" << i.i << ",";
      sum += i.i;
      i.j++;
      
      cout << i.j << ")";
    }
  cout << endl
       << "done" << endl
       << "Effective sum = " << sum << endl
       << "Pretended sum = " << n*(n - 1)/2 << endl
       << endl;
  for (auto it = c.begin(); it != c.end(); ++it)
    cout << "(" << it->i << "," << it->j << ")";
  cout << endl;

  const Container aux = c;
  for (auto & i : aux) 
    {
      cout << "(" << i.i << ",";
      sum += i.i;
      // i.j++; // it should give a compilation error
      
      cout << i.j << ")";
    }
  cout << endl;

  for (auto it = c.cbegin(); it != c.cend(); ++it)
    cout << "(" << it->i << "," << it->j << ")";
  cout << endl;

  for (auto it = aux.cbegin(); it != aux.cend(); ++it)
    cout << "(" << it->i << "," << it->j << ")";
  cout << endl;
} 

int main(int argn, char *argv[])
{
  size_t n = argn > 1 ? atoi(argv[1]) : 10;

  test<DynArray<Wrapper>>(n);
  test<DynList<Wrapper>>(n);
  test<DynDlist<Wrapper>>(n);
  test<DynSetTree<Wrapper, Rand_Tree>>(n);
  test<DynSetTree<Wrapper, BinTree>>(n);
  test<DynSetTree<Wrapper, Avl_Tree>>(n);
  test<DynSetTree<Wrapper, Splay_Tree>>(n);
  test<DynSetTree<Wrapper, Treap>>(n);
  test<DynSetTree<Wrapper, Treap_Rk>>(n);
  test<DynSetHash<Wrapper>>(n);
  test<DynSetLinHash<Wrapper>>(n);
  test<OLhashTable<Wrapper>>(n);
  test<ODhashTable<Wrapper>>(n);
  test<DynArrayHeap<Wrapper>>(n);
  // test<MemArray<Wrapper>>(n); // deleted because now MemArray does
                                 // not suport functional operations
  test<ArrayStack<Wrapper>>(n);
  test<FixedStack<Wrapper>>(n);
  test<ArrayQueue<Wrapper>>(n);
  test<FixedQueue<Wrapper>>(n);
  test<DynListQueue<Wrapper>>(n);
  test<DynListStack<Wrapper>>(n);
  test<ArrayHeap<Wrapper>>(n);

  DynList<int> l = { 0, 1, 2, 3 };
  for (auto & i : l) i = 1;
  for (auto i : l) cout << i << " "; cout << endl;
}

