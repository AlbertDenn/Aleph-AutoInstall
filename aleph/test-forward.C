
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
# include <tpl_dynSetTree.H>
# include <tpl_dynMapTree.H>
# include <tpl_arrayStack.H>

using namespace std;
using namespace Aleph;

template <template <class> class Container, typename T>
void test_container(int n)
{
  cout << "Probando con contenedor tipo lista" << endl;
  Container<T> l;

  cout << "append de R values\n";
  for (int i = 0; i < n; ++i)
    l.append(T(i));
  cout << endl;

  cout << "append de L values\n";
  for (int i = 0; i < n; ++i)
    {
      T t = i;
      l.append(t);
    }
  cout << endl;

  cout << "insert de R values\n";
  for (int i = 0; i < n; ++i)
    l.insert(T(i));
  cout << endl;

  cout << "insert de L values\n";
  for (int i = 0; i < n; ++i)
    {
      T t = i;
      l.insert(t);
    }
  cout << endl;

  for (typename Container<T>::Iterator it(l); it.has_curr(); it.next())
    cout << it.get_curr() << " ";
  cout << endl;
}

template <template <class, class> class Tree, typename T>
void test_tree(int n)
{
  cout << "Probando con contenedor tipo arbol" << endl;
  DynSetTree<T, Tree> l;

  cout << "insert de R values\n";
  for (int i = 0; i < n; ++i)
    l.insert(T(i));
  cout << endl;

  cout << "insert de L values (no debe insertar en arbol)\n";
  for (int i = 0; i < n; ++i)
    {
      T t = i;
      l.insert(t);
    }
  cout << endl;

  cout << "insert_dup de R values\n";
  for (int i = 0; i < n; ++i)
    l.insert_dup(T(i));
  cout << endl;

  cout << "insert_dup de L values\n";
  for (int i = 0; i < n; ++i)
    l.insert_dup(i);
  cout << endl;

  l.for_each([/* Lambda */] (const T & key) { cout << key << " "; });
  cout << endl;
}

template <class Tree>
void test_map_tree(int n)
{
  Tree (*create)(int) = [/* Lambda */] (int n) -> Tree
    {
      Tree t;
      for (int i = 0; i < n; ++i)
	t.insert(i, i + 2);
      return std::move(t);
    };

  Tree tree = (*create)(n);
} 

int main(int, char * argc[])
{
  int n = atoi(argc[1]);

  test_map_tree<DynMapTree<int, int, Avl_Tree>>(n);

  return 0;

  test_container <DynList, int> (n);

  test_container <DynDlist, int> (n);

  test_tree <Avl_Tree, int> (n);
}
