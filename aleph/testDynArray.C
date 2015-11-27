
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
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <iostream>

# include <tpl_dynarray_set.H>


using namespace std;

# define RAND( x  ) (unsigned long) ( x * ( rand() / (RAND_MAX+1.0) ) ) 


using namespace Aleph;
using namespace std;

struct Test
{
  char a1;
  char a2;
};


int main(int argc,char *argv[])
{
  unsigned long seed,index;
  unsigned long i, value, val;

  const unsigned long NUM_ITE = argc > 1 ? atoi(argv[1]) : 1000;
  
  if (argc > 2)
    seed = atol(argv[1]);
  else
    seed = time(NULL);

  srand (seed);

  DynArray<Test> t;

  t[10]->a1 = 'a';
  t[10]->a2 = 'b';

  cout << t[10]->a1 << t[10000]->a1 << endl;

  cout << "./testDynArray " << NUM_ITE << " " << seed << endl;

  try
  {
    {
      DynArray<unsigned long> v1(8,8,8), v2(NUM_ITE);

      for(i = 0; i < NUM_ITE; i++)
	{
	  value =  RAND(NUM_ITE);
	  index =  RAND(NUM_ITE);
	  v1[index] = value;
	  v2[index] = v1[index];
	  val = v1[index];
	  value = v2[index];
	  cout << "(" << val << ") (" << value << ") ";
	  assert(v2[index] == v1[index]);
	}

      {
	DynArray<unsigned long> v3(v2);

	DynArray<unsigned long> v4(v3);

	v4 = v1;

	v4.swap(v2);
      }

      v2.reserve(0, 2*NUM_ITE);

    }
    DynArray<unsigned long> v(10, 10, 6);

    v.reserve(0, 2*NUM_ITE);

    v.reserve(NUM_ITE, 4*NUM_ITE);
  }
  catch(std::overflow_error)
    {
      cout << "Overflow!" << endl;
    }
  catch(std::bad_alloc)
    {
      cout << "Not enough memory!" << endl;
    }
  catch(std::invalid_argument)
    {
      cout << "invalid_argument!" << endl;  
    }
  catch (std::exception & e)
    {
      cout << e.what() << endl;
    }
  cout << endl;

  {
    DynArray<int> s(12,10,4);

    s.append(10);

    cout << s.access(0) << " ****************" << endl;

    cout << s.access(0) << " ****************" << endl;

    s.append(16);

    cout << s[1] << " ****************" << endl;

  }

  {
    DynArray_Set<int> s(12,10,4);

    s.append(10);

    cout << s[0] << " ****************" << endl;

    cout << s.access(0) << " ****************" << endl;

    s.append(16);

    cout << s[1] << " ****************" << endl;

  }
 
  DynArray<int> a;
  for (int i = 0; i < NUM_ITE; ++i)
    a.touch(i) = i;

  for (DynArray<int>::Iterator it(a); it.has_curr(); it.next())
    cout << it.get_curr() << " ";
  cout << endl;

  DynArray<int>::Iterator it(a);
  for (it.reset_last(); it.has_curr(); it.prev())
    cout << it.get_curr() << " ";
  cout << endl;
}












