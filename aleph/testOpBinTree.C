
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
# include <tpl_binTree.H>
# include <tpl_binNodeUtils.H>
# include <opBinTree.H>

using namespace std;
using namespace Aleph;


void print(BinNode<int>* node, int, int)
{
  cout << node->get_key() << " ";
}


int * k;
int counter = 0;

void assign(BinNode<int>* node, int, int)
{
  cout << node->get_key() << " ";
  k[counter++] = node->get_key();
}


double bin_coef(int n, int m)
{
  double result = 1;

  if (m == 0 or n == m)
    return result;

  int nm = n - m;

  int max, min;
  if (nm > m)
    {
      max = nm; min = m;
    }
  else
    {
      max = m; min = nm;
    }

  while (n > max and min > 1)
    {
      result *= (1.0*n) / (1.0*min);
      n--; min--;
    }

  while (n > max)
    {
      result *= n;
      n--;
    }

  while (min > 1)
    {
      result /= min;
      min--;
    }

  return result;
}


int main(int argn, char * argc[])
{
  unsigned int t;
  int n = 10;
  if (argn > 1)
    n = atoi(argc[1]);

  double prob = 0.5;
  if (argn > 2)
    prob = atof(argc[2]);

  t = time(NULL);
  if (argn > 3)
    t = atoi(argc[3]);

  srand(t);

  cout << argc[0] << " " << n << " " << prob << " " << t << " " << endl;

  int keys[n];

  BinTree<int> tree;
  BinTree<int>::Node * node;
  int value;

  for (int i = 0; i < n; i++)
    {
      do
	{
	  value = (int) (10.0*n*rand()/(RAND_MAX+1.0));
	}
      while (tree.search(value) not_eq NULL);

      node = new BinTree<int>::Node (value);
      tree.insert(node);
    }

  /* Coloca las claves del arbol en el arreglo keys */
  k = &keys[0];
  inOrderRec(tree.getRoot(), assign);
  cout << endl;

  /* construye arreglo de probabilidades segun dist binomial */
  double p[n];
  for (int i = 0; i < n; i++)
    {
      p[i] = bin_coef(n, i) * pow(prob, i) * pow((1 - prob), n - i);
      printf("%.5f ", p[i]);
    }

  BinNode<int> * optimal_tree = 
    build_optimal_tree<BinNode<int>, int>(keys, p, n);

  cout << endl;
  preOrderRec(optimal_tree, print); cout << endl;

  destroyRec(tree.getRoot());
  destroyRec(optimal_tree);
}
