
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

# include <bloom-filter.H>

template <typename T>
ostream & operator << (ostream & s, const DynList<T> & l)
{
  if (l.is_empty())
    return s << "EMPTY";

  l.for_each([&s] (const T & i) { s << i << " "; });
  return s;
}

int main(int argc, char *argv[])
{
  const size_t n = atoi(argv[1]);
  const double p = atof(argv[2]);
  const unsigned long seed = atoi(argv[3]);

  Bloom_Filter<long> f(n, p, seed);
  Bloom_Filter<long> ff(f);

  ff.swap(f);

  auto t = f.estimate(n, p);
  auto m = get<0>(t);
  cout << "seeds = " << f.hash_seeds() << endl
       << "hashes(10) = " << f.hashes(10) << endl
       << "hashes(9)  = " << f.hashes(9) << endl
       << "common(10, 9) = " << f.common_hashes(10, 9) << endl
       << "m = " << m << " (" << m/8 + m% 8 << " bytes)" << endl
       << "k = " << f.get_k() << endl
       << "sizeof(size_t) = " << sizeof(size_t) << endl
       << endl
       << endl
       << "Inserting " << n << " items sequentially" << endl
       << endl;

  for (auto i = 0; i < n; ++i)
    f.append(i);

  auto x = f.get_x();
  cout << "Done!" << endl
       << "Bits set to 1 = " << x << " of " << m << " ("
       << 100.0*x/m << " %)" << endl    
       << endl
       << "Generating random searches" << endl;
  
  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));
  size_t false_positive_count = 0;
  size_t failed_search_count = 0;
  for (auto i = 0; i < 10*n; ++i)
    {
      long val = gsl_rng_get(r);
      if (val >= 0 and val < n)
	{
	  if (not f.contains(val))
	    cout << "ERROR: " << val << " was not found" << endl;
	}
      else
	{
	  ++failed_search_count;
	  if (f.contains(val))
	    ++false_positive_count;
	}
    }

  cout << "done!" << endl
       << "Total searches  = " << 10*n << endl
       << "Failed searches = " << failed_search_count << endl
       << "False positives = " << false_positive_count << endl
       << "Error = " << 100.0*false_positive_count/failed_search_count 
       << " % " << endl
       << endl;

  gsl_rng_free(r);
}
