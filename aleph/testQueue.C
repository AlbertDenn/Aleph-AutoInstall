
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
# include <string>
# include <tpl_arrayQueue.H>

using namespace std;

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
void print(const ArrayQueue<T> & q)
{
  cout << "capacity = " << q.capacity() << endl
       << "size     = " << q.size() << endl;

  for (int i = 0; i < q.size(); ++i)
    cout << (T) q.front(i) << " ";
  cout << endl ;

  for (int i = 0; i < q.size(); ++i)
    cout << (T) q.rear(i) << " ";
  cout << endl
       << endl;
}

template <typename T>
ArrayQueue<T> create_queue(int n)
{
  cout << "Creating rval queue ";
  ArrayQueue<T> q;
  for (int i = 0; i < n; ++i)
    cout << q.put(T(i)) << " ";
  cout << endl;

  return q;
}

int main(int, char * argc[])
{
  size_t n = atoi(argc[1]);
  ArrayQueue<int> q(n);

  print(q);

  cout << "Inserting " << n << " values ";
  for (int i = 0; i < n; i++)
    cout << q.put(i) << " ";
  cout << " done!" << endl << endl;

  print(q);

  cout << "Consulting all values until underflow ";
  for (int i = 0; 1; i++)
    {
      try
	{
	  int val = q.rear(i);
	  cout << val  << " " ;
	}
      catch (std::range_error & exc)
	{
	  cout << endl << exc.what() << endl;
	  break;
	}
    }
  cout << " done! " << endl << endl;

  cout << "Deleting all values in steps of 3 until underflow ";
  while (1)
    {
      try
	{
	  printf("%d ", q.getn(3));
	}
      catch (std::underflow_error & exc)
	{
	  cout << endl << exc.what() << endl;
	  break;
	}
    }
  cout << " done! " << endl << endl;

  print(q);

  cout << "Inserting " << n << " values " << endl;
  for (int i = 0; i < n; i++)
    cout << q.put(i) << " ";
  cout << " done!" << endl << endl;

  print(q);

  int m = atoi(argc[2]);

  cout << "Deleting " << m << " items" << endl;
  for (int i = 0; i < m; i++)
    cout << q.get() << " ";

  cout << "done!" << endl << endl;

  cout << "q = " << endl;
  print(q);

  cout << "Testing constructors ... " << endl;

  ArrayQueue<int> q1 = q;

  print(q1);

  ArrayQueue<int> q2 = create_queue<int>(n);
  print(q2);


  ArrayQueue<Foo> q3 = create_queue<Foo>(n);
  print(q3);

  printf("Ended\n");
}
