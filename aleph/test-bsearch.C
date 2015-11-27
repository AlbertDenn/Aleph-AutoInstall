
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
# include <tpl_sort_utils.H>
# include <tpl_random_queue.H>

struct Foo
{
  unsigned long p;
  unsigned long s;

  friend ostream & operator << (ostream & out, const Foo & f)
  {
    return out << "(" << f.p << "," << f.s << ")";
  }
};

DynArray<Foo> create(size_t n)
{
  assert(n % 10 == 0);
  DynArray<Foo> ret; ret.reserve(n);

  for (long i = 0; i < n/10; ++i)
    {
      Foo f; 
      f.s = i;
      for (long j = 0; j < 10; ++j)
	{
	  f.p = 10*i + j;
	  ret(f.p) = f;
	}
    }

  assert(is_sorted(ret, [] (auto r1, auto r2) { return r1.p < r2.p; })); 
  assert(is_sorted(ret, [] (auto r1, auto r2) { return r1.s < r2.s; }));

  {
    auto u = shuffle(ret);
    auto index = build_index(u, [] (auto p1, auto p2) { return p1.s < p2.s; });
    assert(is_sorted(index, [&u] (auto i1, auto i2) 
		     { return u(i1).s < u(i2).s; }));
  }

  DynArray<size_t> index; index.reserve(n);
  for (long i = 0; i < index.size(); ++i)
    index(i) = i;
      
  for (long i = 0; i < n/10; ++i)
    {
      Foo f; f.s = i;
      {
	DynList<Foo*> dups = 
	  bsearch_dup(ret, f, [] (auto r1, auto r2) { return r1.s < r2.s; });
	assert(dups.all([i] (auto ptr) { return ptr->s == i; }));
      }
    }

  return ret;
}


void usage()
{
  cout << "test-bsearch n" << endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    usage();

  const size_t n = atoi(argv[1]);
  
  auto a = create(n);

  cout << "Every thing is ok" << endl;
}

