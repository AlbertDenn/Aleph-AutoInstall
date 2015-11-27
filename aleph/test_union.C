
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
# include <iostream>
# include <tpl_union.H>

void usage(int argn, char * argc[])
{
  if (argn < 3)
    {
      cout << "usage is " << argc[0] << " n num-pairs [seed]" << endl;
      exit(0);
    }
}

int main(int argn, char * argc[])
{
  if (argn < 3 or argn > 4)
    usage(argn, argc);
  int n = atoi(argc[1]);

  int num_pairs = atoi(argc[2]);

  unsigned int seed = 0;
  if (argn > 3)
    seed = atoi(argc[3]);

  gsl_rng * r = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(r, seed % gsl_rng_max(r));

# ifdef NADA
  {
    Relation rel;

    cout << "Insertando " << num_pairs << " pares " << "(" 
	 << rel.get_num_blocks() << ")" << endl;
    int ins_count = 0;
    for (int i = 0; i < num_pairs; ++i)
      {
	int i = gsl_rng_uniform_int(r, n - 1);
	int j = gsl_rng_uniform_int(r, n - 1);

	if (i == j)
	  continue;

	if (rel.are_connected(i, j))
	  continue;

	cout << i << "-" << j << ", ";
	rel.join(i, j);
	++ins_count;
      }
    cout << ins_count << " pairs inserted" << endl
	 << endl;

    for (int i = 0; i < n; ++i)
      {
	for (int j = 0; j < n; ++j)
	  {
	    if (i == j)
	      continue;

	    //cout << "(" << i << "," << j << ")";
	    if (rel.are_connected(i, j))
	      cout << i << "-" << j << ", ";
	  }
	cout << i << endl;
      }
  
    cout << endl
	 << rel.size() << " items " << rel.get_num_blocks() 
	 << " blocks" << endl;
  }
# endif
  {
    Relation_T<int> rel;

    cout << "Insertando " << num_pairs << " pares " << "(" 
	 << rel.get_num_blocks() << ")" << endl;
    int ins_count = 0;
    for (int k = 0; k < num_pairs; ++k)
      {
	int i = gsl_rng_uniform_int(r, n - 1);
	int j = gsl_rng_uniform_int(r, n - 1);

	if (i == j)
	  continue;

	if (rel.are_connected(i, j))
	  continue;

	cout << i << "-" << j << ", ";
	rel.join(i, j);
	++ins_count;
      }
    cout << ins_count << " pairs inserted" << endl
	 << endl;

    for (int i = 0; i < n; ++i)
      {
	for (int j = 0; j < n; ++j)
	  {
	    if (i == j)
	      continue;

	    //cout << "(" << i << "," << j << ")";
	    if (rel.are_connected(i, j))
	      cout << i << "-" << j << ", ";
	  }
	cout << i << endl;
      }
  
    cout << endl
	 << rel.size() << " items " << rel.get_num_blocks() 
	 << " blocks" << endl;
  }
}
