
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
# include <htlist.H>
# include <tpl_dynDlist.H>
# include <tpl_dynArray.H>
# include <tpl_dynSetTree.H>
# include <tpl_dynSetHash.H>
# include <tpl_dynBinHeap.H>
# include <tpl_dynArrayHeap.H>
# include <tpl_graph.H>
# include <tpl_sgraph.H>
# include <tpl_agraph.H>
# include <tpl_net.H>
# include <tpl_netcost.H>

using namespace std;
using namespace Aleph;

struct Foo
{
  int f1 = -1;
  string f2 = "hello";
  float f3 = 0;

  Foo() {}

  Foo(int __f1) : f1(__f1) {}

  Foo(string __f2) : f2(__f2) {}

  Foo(int __f1, const string & __f2) : f1(__f1), f2(__f2) {}

  Foo(int __f1, const string & __f2, float __f3) 
    : f1(__f1), f2(__f2), f3(__f3) {}

  friend ostream & operator << (ostream & s, const Foo & foo)
  {
    return s << "(" << foo.f1 << "," << foo.f2 << "," << foo.f3 << ")";
  }
};

template <template <typename> class C>
void test()
{
  int i = 7;
  string str = "salut";
  float f = 10e6;
  C<Foo> c;
  c.emplace(2);
  c.emplace(3, "hola");
  c.emplace(4, "adios", -1.0);
  c.emplace(5, str);
  c.emplace(6, str, f);
  c.emplace(i, str, 2.5);

  c.for_each([] (auto foo) { cout << foo; });
  cout << endl;
}

void test_graph()
{
  int i = 7;
  string str = "salut";
  float f = 10e6;

  {
    Array_Graph<Graph_Anode<Foo>, Graph_Aarc<Foo>> g;

    auto p1 = g.emplace_node(2);
    auto p2 = g.emplace_node(3, "hola");
    auto p3 = g.emplace_node(4, "adios", -1.0);
    auto p4 = g.emplace_node(5, str);
    auto p5 = g.emplace_node(6, str, f);
    auto p6 = g.emplace_node(i, str, 2.5);

    g.emplace_arc(p1, p2, 0);
    g.emplace_arc(p3, p4, i, str, f);
    g.emplace_arc(p5, p6, 0, "soyuz");
  }

  cout << endl
       << endl;

  {
    Net_Graph<Net_Node<Foo>, Net_Arc<Foo>> g;
    auto p1 = g.emplace_node(2);
    auto p2 = g.emplace_node(3, "hola");
    auto p3 = g.emplace_node(4, "adios", -1.0);
    auto p4 = g.emplace_node(5, str);
    auto p5 = g.emplace_node(6, str, f);
    auto p6 = g.emplace_node(i, str, 2.5);

    g.insert_arc(p1, p2, 0);
    g.emplace_arc(p3, p4, 0, 0, i, str, f);
    g.insert_arc(p5, p6, 0);
  }

  {
    Net_Cost_Graph<Net_Cost_Node<Foo>, Net_Cost_Arc<Foo>> g;
    auto p1 = g.emplace_node(2);
    auto p2 = g.emplace_node(3, "hola");
    auto p3 = g.emplace_node(4, "adios", -1.0);
    auto p4 = g.emplace_node(5, str);
    auto p5 = g.emplace_node(6, str, f);
    auto p6 = g.emplace_node(i, str, 2.5);

    g.insert_arc(p1, p2, 10, 10);
    g.emplace_arc(p3, p4, 0, 0, i, str, f);
    g.insert_arc(p5, p6, 20, 20);
 }
}

int main()
{
  test<DynList>();
  test<DynDlist>();
  test<DynArray>();


  test_graph();
}
