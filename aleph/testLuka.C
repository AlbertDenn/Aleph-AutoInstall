
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
# include <string.h>
# include <iostream>
# include <tpl_binNode.H>
# include <tpl_binNodeUtils.H>

using namespace std;

gsl_rng * r = NULL;


void init_random(int seed)
{
  r = gsl_rng_alloc (gsl_rng_mt19937);

  if (r == NULL)
    throw std::bad_alloc();

  gsl_rng_set(r, seed % gsl_rng_max(r));
}


size_t get_rand(const size_t & n)
{
  return gsl_rng_uniform_int(r, n) + 1;
}


    template <class Node>
Node * random_tree(const size_t & n)
{
  if (n == 0) 
    return Node::NullPtr;

  Node * root = new Node; // apartar memoria nodo 

      /* sorteo que define aleatoriamente la posición infija de la raíz */
  const size_t i = get_rand(n);

  LLINK(root) = random_tree<Node>(i - 1); // aleatorio de r - 1 nodos
  RLINK(root) = random_tree<Node>(n - i); // aleatorio de n - r nodos

  return root;
}


    template <class Node>
string luka(Node * p)
{
  if (p == Node::NullPtr)
    return string("b");

  return string("a") + luka(LLINK(p)) + luka(RLINK(p));
}

    template <class Node>
Node * luka_to_tree(char *& cod)
{
  if (*cod == '\0')
    return Node::NullPtr;

  if (*cod++ == 'b')
    return Node::NullPtr;

  Node * p = new Node;

  LLINK(p) = luka_to_tree<Node>(cod);
  RLINK(p) = luka_to_tree<Node>(cod);

  return p;
}

int main(int argn, char *argc[])
{ 
  const size_t n = argc[1] ? atol(argc[1]) : 10;

  int t = time(NULL);

  if (argn > 2)
    t = atol(argc[2]);

  cout << "testLuka " << n << " " << t << endl;

  init_random(t);

  BinNode<int> * r = random_tree< BinNode<int> >(n);

  cout << "luka(r) = " << luka(r) << endl;

  char * cod = strdup(luka(r).c_str());

  BinNode<int> * aux =  luka_to_tree< BinNode<int> >(cod);

  assert(areSimilar(aux, r));

  destroyRec(aux);
  destroyRec(r);
}
