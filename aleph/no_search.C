
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
# include <tpl_dynSetTree.H>
# include <iostream>

using namespace std;

// busca en el arreglo a con n números comprendidos entre 0..m un
// número que no esté en el arreglo
int not_in(int a[], int n, int m)
{
  int s, e, b, mid;
  int l = 0, r = m;

  while (true)
    {
      s = e = b = 0;
      mid = (l + r) / 2;

      for (int i = 0; i < n; ++i)
	if (a[i] >= l and a[i] <= r)
	  if (a[i] < mid) 
	    ++s;
	  else if (mid < a[i])
	    ++b;
	  else 
	    ++e;

      if (s == 0 and l < mid)
	return l;
      else if (b == 0 and r > mid)
	return r;
      else if (e == 0)
	return mid;

      if (s < b)
	r = mid - 1;
      else
	l = mid + 1;
    }
}


int main(int argn, char * argc[])
{
  int m = 101;
  int n;
  unsigned long seed;
  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);

  if (argn > 1)
    m = atoi(argc[1]);

  if (argn > 2)
    n = std::min(atoi(argc[2]), m -1);
  else
    n = m - 1;

  if (argn > 3)
    seed = atoi(argc[3]);
  else
    seed = time(NULL) % gsl_rng_max(r);

  cout << argc[0] << " " << m << " " << n << " " << seed << endl;

  gsl_rng_set(r, seed);

  int * a = new int [n];

  DynSetBinTree<int> s;

  for (int i = 0; i < n; ++i)
    {
    repeat:
      int value = gsl_rng_uniform_int(r, m + 1);
      if (s.exist(value))
	goto repeat;

      a[i] = value;
      s.put(value);
      //      cout << value << " ";
    }

  cout << endl;

  int val = not_in(a, n, m);

  cout << endl << endl
       << val  << " no esta en el arreglo" << endl;

  if (s.exist(val))
    cout << "Algo esta mal pues " << val << " esta en el arreglo" << endl;
  else
    cout << "La conclusion es correcta" << endl;

  delete [] a;

  gsl_rng_free(r);
}
