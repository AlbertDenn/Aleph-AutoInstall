
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

# include <primes.H>

namespace Primes
{

const unsigned long primeList[] =
  {
    5,          11,           23,        47,        97,
    197,        397,          797,       1597,      3203,
    6421,       12853,        25717,     51437,     102877,
    205759,     411527,       823117,    1646237,   3292489,
    6584983,    13169977,     26339969,  52679969,  105359939, 
    210719881,  421439783,    842879579, 1685759167, 3371518343 /*, 
								  6743036717ul, 13486073473ul, 26972146961ul, 53944293929ul, 107888587883ul */
  };

const unsigned int numPrimes = 30;

static int nextPrimeIndex(unsigned long n)
{
  unsigned int i;

  for (i = 0; i < numPrimes; i++)
    if (primeList[i] >= n)
      return i;
  return -1;
}

static int prevPrimeIndex(unsigned long n)
{
  unsigned int i;

  for (i = 1; i < numPrimes; i++)
    if (primeList[i] > n)
      return i - 1;
            
  return -1;
}

const unsigned long DefaultPrime = primeList[0];

unsigned long next_prime(unsigned long n)
{ 
  unsigned int prime_index = nextPrimeIndex(n);

  return primeList[prime_index];
}

bool check_primes_database()
{
  for (int i = 1; i < numPrimes; ++i)
    if (primeList[i] <= 2*primeList[i - 1])
      return false;
  return true;
}


}
