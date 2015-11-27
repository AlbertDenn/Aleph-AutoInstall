
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

# include <time.h>
# include <gsl/gsl_rng.h>
# include <cassert>
# include <al-vector.H>

gsl_rng * r;

const size_t N = 1000;

Vector<int> create_domain(size_t n)
{
  Vector<int>::Domain domain;
  for (int i = 0; i < n; ++i)
    domain.insert(i);

  return std::move(domain);
}

Vector<std::string> create_string_domain(size_t n)
{
  int m = 'Z' - 'A';
  int num = n/m;
  int rem = n%m;

  std::string str = "";
  Vector<string>::Domain domain;
  
  int k;
  for (int i = 0; i < num; i += m)
    {
      for (k = 0; k < m; ++k)
	domain.insert(str + string('A' + k, 1));

      str += string('A' + k, 1);
    }

  for (int i = 0; i < rem; ++i)
    domain.insert(str + string('A' + k + i, 1));

  return std::move(domain);
}

int main(int argn, char * argc[])
{
  size_t n = argn > 1 ? atoi(argc[1]) : N;
  unsigned long seed = argn > 2 ? atoi(argc[2]) : time(NULL);

  cout << argc[0] << " " << n << " " << seed << endl;

  r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

  Vector<int>::Domain d ({0,1,2,3,4,5,6,7,8,9});
    
  Vector<int> v1(d, {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9});
  Vector<int> v2(d, {1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9});
  Vector<int> v3(d, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
 
  cout << "v1 = " << endl
       << v1.to_str() << endl
       << endl
       << "v2 = " << endl
       << v2.to_str() << endl
       << endl
       << "(v1 + v2) = " << endl
       << (v1 + v2).to_str() << endl
       << endl
       << "2.0*v1 = " << endl
       << (2.0*v1).to_str() << endl
       << endl
       << "v2*-3 = " << endl
       << (v2*-3.0).to_str() << endl
       << endl
       << "v1*v2 = " << v1*v2 << endl
       << endl
       << "v3*v3 = " << v3*v3 << endl
       << endl
       << "Domain from list = ";
  v1.to_list().for_each(/* Lambda */ [] (const double & val)
			{
			  cout << val << ", ";
			});
  cout << endl;  

  assert(v1 + v2 == 
	 Vector<int>(d, {1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8}));

  assert(2.0*v1 == 
	 Vector<int>(d, {0, 0.2, 0.4, 0.6, 0.8, 1, 1.2, 1.4, 1.6, 1.8}));
  
  assert(v2*-3.0 ==
	 Vector<int>(d, {-3, -3.3, -3.6, -3.9, -4.2, -4.5, -4.8, -5.1, -5.4, -5.7}));

  assert(v1 - v1 == Vector<int>(d, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}));

  assert(v3*v3 == 385.0);

  assert((2.0*v1)/2.0 == v1);
  assert((2.0*v2)/2.0 == v2);

  cout << "Testing iterator on v2 ";
  for (Vector<int>::Iterator it(v2); it.has_curr(); it.next())
    cout << "(" << it.get_curr().first << "," << it.get_curr().second << ") ";
  cout << endl;

  cout << "Testing for_each for v2 vector ";
  v2.for_each([] (const std::pair<int, double> & p)
	      {
		cout << "(" << p.first << "," << p.second << ") ";
	      });

  Vector<int> v4(d, {0,1,2,3,4,5,6,7,8,9});
  for_each(d, [&v4] (const int & i)
	   {
	     assert(v4[i] == i); 
	   });

  Vector<int> zero(d);
  assert(all(d, [&zero] (const int i)
	     {
	       return zero[i] == 0;
	     }));

  Vector<int> v5(d);
  for_each(d, [&v5] (int i)
	   {
	     v5[i] = i;
	   });
  assert(all(d, [&v5] (int i)
	     {
	       return v5[i] == i;
	     }));

  for_each(d, [&v5,&v1] (int i)
	   {
	     v5[i] = v1[i];
	   });
  assert(all(d, [&v5,&v1] (int i)
	{
	  return v5[i] == v1[i];
	}));

  gsl_rng_free(r);
}
