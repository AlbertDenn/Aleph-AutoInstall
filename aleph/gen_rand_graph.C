
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

# include <sys/resource.h>
# include <tpl_sgraph.H>
# include <tpl_agraph.H>
# include <io_graph.H>
# include <random_graph.H>
# include <euclidian-graph-common.H>

bool verbose = false;

using namespace Aleph;


typedef Array_Graph<Graph_Anode<My_P>, Graph_Aarc<int>> Graph;


void usage(int argn, char * argc[])
{
  if (argn >= 5)
    return;

  printf("usage: %s n m w h seed [file-name]\n", argc[0]);
  exit(1);
}
 
int main(int argn, char * argc[])
{
  usage(argn, argc);
  
  size_t n = atoi(argc[1]);
  size_t m = atoi(argc[2]);
  int w = atoi(argc[3]);
  int h = atoi(argc[4]);
  unsigned int seed = atoi(argc[5]);
  
  cout << argc[0] << " " << n << " " << m << " " << w << " " << h << " " 
       << seed << endl;

  cout << "Preparing system stack size to 256 Mb ... " << endl
       << endl;

  const rlim_t kStackSize = 256 * 1024 * 1024;   // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0)
      if (rl.rlim_cur < kStackSize)
        {
	  rl.rlim_cur = kStackSize;
	  result = setrlimit(RLIMIT_STACK, &rl);
	  if (result != 0)
	    fprintf(stderr, "setrlimit returned result = %d\n", result);
	  else
	    cout << "OK. done!" << endl
		 << endl;
        }

  cout << "Generating graph ... " << endl;

  Graph g = gen_random_euclidian_graph<Graph>(n, m, w, h, seed);

  if (argn == 7)
    {
      ofstream out(argc[6]);
      IO_Graph<Graph, Rnode<Graph>, Wnode<Graph>, Rarc<Graph>, Warc<Graph>> 
	(g).save_in_text_mode(out);
    }
  else
    IO_Graph<Graph, Rnode<Graph>, Wnode<Graph>, Rarc<Graph>, Warc<Graph>> 
      (g).save_in_text_mode(cout);
}


