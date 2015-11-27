
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
# include <cassert>
# include <iostream>
# include <ahSort.H>
# include <tpl_sort_utils.H>

using namespace std;

gsl_rng * r = NULL;

template <template <typename T> class List,
	  typename T = long>
List<T> build_int_list(int n)
{
  List<T> ret_val;
  for (int i = 0; i < n; ++i)
    ret_val.append(gsl_rng_get(r));
  
  return ret_val;
}

template <template <typename T> class List,
	  typename T>
bool verify_sort(const List<T> & list)
{
  T value = numeric_limits<T>::min();
  return list.all([&value] (const T & item)
		  {
		    bool ret = value <= item;
		    value = item;
		    return ret;
		  });    
}

template <template <typename T> class List,
	  typename T = long>
List<T*> build_ptr_list(int n)
{
  List<T*> ret_val;
  for (int i = 0; i < n; ++i)
    ret_val.append(new T(gsl_rng_get(r)));
  
  return std::move(ret_val);
}

template <template <typename T> class List,
	  typename T >
bool verify_sort(const List<T*> & list)
{
  T value = numeric_limits<T>::min();
  return list.all([&value] (T * ptr)
		  {
		    bool ret = value <= *ptr;
		    value = *ptr;
		    return ret;
		  });    
}

template <template <typename T> class List, typename T>
void free_ptr_list(List<T*> & list)
{
  list.for_each([] (T * ptr)
		{
		  delete ptr;
		});
}

int main(int argc, char *argv[])
{
  unsigned long n = argc > 1 ? atoi(argv[1]) : 1000;
  unsigned int  t = argc > 2 ? atoi(argv[2]) : time(NULL);

  cout << argv[0] << " " << n << " " << t << endl;

  r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, t % gsl_rng_max(r));

  {
    cout << "Testing quicksort on single lists" << endl
	 << "Building list ... " << endl;
    DynList<long> l = build_int_list<DynList>(n);
    cout << "sorting it ..." << endl;
    quicksort(l);
    cout << "done! " << endl
	 << "Verifying ... " << endl;
    assert(verify_sort(l));
    assert(l.length() == n);
    cout << "done!" << endl
	 << endl;
  }

  {
    cout << "Testing quicksort on single lists of pointers" << endl
	 << "Building list ... " << endl;
    DynList<long*> l = build_ptr_list<DynList>(n);
    cout << "sorting it ..." << endl;
    quicksort(l, [] (long * x, long * y)
	      {
		return *x < *y;
	      });
    cout << "done! " << endl
	 << "Verifying ... " << endl;
    assert(verify_sort(l));
    assert(l.length() == n);
    cout << "done!" << endl
	 << endl;
    free_ptr_list(l);
  }

 {
    cout << "Testing mergesort on single lists" << endl
	 << "Building list ... " << endl;
    DynList<long> l = build_int_list<DynList>(n);
    cout << "sorting it ..." << endl;
    mergesort(l);
    cout << "done! " << endl
	 << "Verifying ... " << endl;
    assert(verify_sort(l));
    assert(l.length() == n);
    cout << "done!" << endl
	 << endl;
  }

 {
   cout << "Testing mergesort on single lists of pointers" << endl
	<< "Building list ... " << endl;
   DynList<long*> l = build_ptr_list<DynList>(n);
   cout << "sorting it ..." << endl;
   mergesort(l, [] (long * x, long * y)
	     {
	       return *x < *y;
	     });
   cout << "done! " << endl
	<< "Verifying ... " << endl;
   assert(verify_sort(l));
   assert(l.length() == n);
   cout << "done!" << endl
	<< endl;
   free_ptr_list(l);
 }

 {
   cout << "Testing default sort method on single lists" << endl
	<< "Building list ... " << endl;
   DynList<long> l = build_int_list<DynList>(n);
   cout << "sorting it ..." << endl;
   cout << "done! " << endl
	<< "Verifying ... " << endl;
   assert(verify_sort(sort(DynList<long>(l))));
   assert(sort(l).length() == n);
   cout << "done!" << endl
	<< endl;
 }

  gsl_rng_free(r); 
  return 0;
}
