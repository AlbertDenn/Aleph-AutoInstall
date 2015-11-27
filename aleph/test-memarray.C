
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
# include <tpl_memArray.H>

using namespace std;
using namespace Aleph;

int count = -1;

struct Foo
{
  int * ptr;

  void swap(Foo & f)
  {
    std::swap(ptr, f.ptr);
  }

  Foo() : ptr(NULL)
  {
    ptr = new int;
    *ptr = ::count--;
  }

  Foo(int i) : ptr(NULL)
  {
    ptr = new int;
    *ptr = i;
  }

  Foo(const Foo & f) : ptr(NULL)
  {
    ptr = new int;
    *ptr = *f.ptr;
  }

  Foo(Foo && f) : ptr(NULL)
  {
    std::swap(ptr, f.ptr);
  }

  Foo & operator = (const Foo & f)
  {
    if (this == &f)
      return *this;

    *ptr = *f.ptr;

    return *this;
  }

  Foo & operator = (Foo && f)
  {
    swap(f);
    return *this;
  }

  ~Foo()
  {
    if (ptr != NULL)
      {
	delete ptr;
	ptr = NULL;
      }
  }

  // operator int () { return *ptr; }

  operator int () const { return *ptr; }
};

    template <typename T>
void print(const MemArray<T> & s)
{
  cout << "capacity = " << s.capacity() << endl
       << "size     = " << s.size() << endl;

  for (int i = 0; i < s.size(); ++i)
    {
      int val = s[i];
      cout << val << " ";
    }
  cout << endl
       << endl;
}

template <typename T>
MemArray<T> create_memarray(int n)
{
  MemArray<T> ret(n);

  for (int i = 0; i < n; ++i)
    ret.put(T(i));

  return ret;
}

int main(int, char * argc[])
{
  MemArray<int> s;

  print(s);

  int n = atoi(argc[1]);

  for (int i = 0; i < n; ++i)
    s.put(i);

  print(s);

  int m = n/4;
  if (argc[2] != NULL)
    m = atoi(argc[2]);
  
  cout << "Extracting " << m << " items" << endl;
  for (int i = 0; i < m; ++i)
    cout << s.get() << " ";
  cout << endl;

  print(s);

  MemArray<int> c(s);

  print(c);

  print(create_memarray<int>(n));

  print(create_memarray<Foo>(n));
}
