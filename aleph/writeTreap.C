
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/

# include <iostream>
# include <fstream>
# include <time.h>
# include <aleph.H>
# include <tpl_treap.H>
# include <gmpfrxx.h> 
# include <tpl_binNodeUtils.H>



using namespace std;
using namespace Aleph;

long randomLong()
{
  return (long) (100.0*rand()/(RAND_MAX+1.0));
}

ofstream output("treap-aux.Tree", ios::out); 
fstream fig_file;
fstream tex_file;

Treap<int>::Node * last_node = NULL;


gsl_rng * rand_gen = NULL;

void print_pair(Treap<int>::Node *node, int, int) 
{
  mpz_class pri = 
    mpz_class(gsl_rng_max(rand_gen)) + mpz_class(node->getPriority()) ;

  pri = pri / 10000000;

  tex_file << "$(" << node->get_key() << "," << pri.get_si();

  if (node != last_node)
    tex_file << ")$, ";
  else
    tex_file << ")$ ";
}

void print_key(Treap<int>::Node *node, int, int)
{
  fig_file << node->get_key() << " ";
}

void print_treap(Treap<int>::Node *node, int, int)
{
  output << node->get_key() << " ";
}



void print_prio(Treap<int>::Node *node, int, int)
{
  mpz_class pri = 
    mpz_class(gsl_rng_max(rand_gen)) + mpz_class(node->getPriority()) ;

  pri = pri / 10000000;

  fig_file << pri.get_si() << " "; 
} 


int main(int argn, char *argc[])
{
  int n = 10;
  unsigned int t = time(0);
  int value;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "writeTreap " << n << " " << t << endl;

  Treap<int> tree;
  Treap<int>::Node *node;
  int i;

  cout << "Inserting " << n << " random values in treee ...\n";

  for (i = 0; i < 30; i++)
    {
      while (true)
	{
	  value = 1 + (int) (100.0*rand()/(RAND_MAX+1.0));
	  node = tree.search(value);
	  if (node == NULL)
	    break;
	}
      node = new Treap<int>::Node (value);
      tree.insert(node);
    }

  assert(is_treap(tree.getRoot()));

  rand_gen = tree.gsl_rng_object();

  cout << "Min = " << gsl_rng_min(rand_gen) << endl
       << "Max = " << gsl_rng_max(rand_gen) << endl;

  fig_file.open("bal-04-aux.Tree", ios::out);  
  preOrderRec(tree.getRoot(), print_key); 
  fig_file << endl << "START-AUX " << endl;
  inOrderRec(tree.getRoot(), print_prio); 

  last_node = find_max(tree.getRoot());

  tex_file.open("treap-aux.tex", ios::out); 
  tex_file << "~\\ ";
  inOrderRec(tree.getRoot(), print_pair); 
  tex_file << "~\\ ";

  destroyRec(tree.getRoot()); 

  for (i = 0; i < n; i++)
    {
      value = (int) (n*10.0*rand()/(RAND_MAX+1.0));
      node = tree.search(value);
      if (node == NULL)
	{
          node = new Treap<int>::Node (value); 
          tree.insert(node);
        }
    }

  assert(is_treap(tree.getRoot()));
  fstream output("treap-aux.Tree");
  preOrderRec(tree.getRoot(), print_treap);
}







