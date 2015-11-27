
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


# include <stdlib.h>
# include <iostream>
# include <time.h>
# include <tpl_dynBinHeap.H>

using namespace std;

int keys [] = { 36, 32, 4, 12, 52, 59, 2, 2, 26, 1 };

# include <string.h>

struct Record
{
  char buf[10];
  int i;

  Record (int n) : i(n) 
  { 
    bzero(buf, sizeof(buf[10]));
  }

  virtual ~Record() { }

  virtual void foo() = 0;
};


struct Rec : public Record
{
  Rec(int j) : Record(j) {}

  virtual void foo() 
  {

  }

  virtual ~Rec() {}
};

struct Cmp_Rec
{
  bool operator () (const Record & l, const Record & r) const
  {
    return l.i < r.i; 
  }

  bool operator () (Record * l, Record * r) const
  {
    return l->i < r->i; 
  }
};

struct Cmp_Ptr
{
  bool operator () (unsigned int* p1, unsigned int* p2) const
  {
    return *p1 < *p2;
  }
};


int main(int argn, char* argc[])
{
  MESSAGE("main()");
  srand(time(0));
  unsigned int n = 10;

  if (argn > 1)
    n = atoi(argc[1]);

  {
    DynBinHeap<int> heap;
    unsigned int i, value;

    for (i = 0; i < n; i++)
      {
	value = (int) ((10.0*n*rand())/(RAND_MAX+1.0));
	cout << value << ", ";
	heap.insert(value);
      }

    assert(heap.verify_heap());

    cout << "\n\n"
	 << "Sum = " << heap.fold(0, [] (const int acu, const int i)
				  {
				    return acu + i;
				  });
    cout << endl
	 << endl;
  }
  
  { 
    cout << "testing with pointers" << endl;

    DynBinHeap<unsigned int*, Cmp_Ptr> heap;
    unsigned int i, value;
    unsigned int *ptr;

    for (i = 0; i < n; i++)
      {
	value = (int) ((10.0*n*rand())/(RAND_MAX+1.0));
	ptr = new unsigned int;
	*ptr = value;
	cout << value << ", ";
	heap.insert(ptr);
      }

    cout << "\n\n"
	 << "Extracting from heap" << endl;

    for (i = 0; i < n; i++)
      {
	ptr = heap.getMin();
	value = *ptr;
	cout << value << " ";
	delete ptr;
      }

    cout << "\n\n";

    heap.size();
  }

  { 
    cout << "testing with pointers to record" << endl;

    DynBinHeap<Record*, Cmp_Rec> heap;
    unsigned int i, value;
    Rec *ptr;

    for (i = 0; i < n; i++)
      {
	value = (int) ((10.0*n*rand())/(RAND_MAX+1.0));
	ptr = new Rec(value);
	cout << value << ", ";
	heap.insert(ptr);
      }

    cout << "\n\n"
	 << "Extracting from heap" << endl;

    for (i = 0; i < n; i++)
      {
	ptr = static_cast<Rec*>(heap.getMin());
	value = ptr->i;
	cout << value << " ";
	delete ptr;
      }

    cout << "\n\n";

    heap.size();
  }

}
