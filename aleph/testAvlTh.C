
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

  
# include <stdlib.h>
# include <time.h>
# include <iostream>
# include <aleph.H>
# include <tpl_avlTreeTh.H>
# include <tpl_threadNodeUtils.H>

using namespace std;
using namespace ThreadNode_Utils;

long long int sumaPorHilos = 0;
int cont = 0;
long long int sumaInOrderRec = 0; 
static void print(AvlTreeTh<int>::Node *p) 
{
  if(!(cont%10))sumaInOrderRec += p->get_key();  
  signed char d = p->getDiff();
  cout << "(" << p->get_key() << "," << (int)d << ")";
  cont++;
}


int main(int argn, char *argc[])
{
  int n = 1000;
  unsigned int t = time(0);
  int value;

  if (argn > 1)
    n = atoi(argc[1]);
  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "testThAvlTree " << n << " " << t << endl;

  AvlTreeTh<int> tree;
  AvlTreeTh<int>::Node *node;
  int i;

  cout << "Inserting " << n << " random values in treee ...\n";
  int j,k,m;
  k = 0;
  for (i = 0; i < n; i++)
    {
      value = 1+(int) (n*100.0*rand()/(RAND_MAX+1.0));
      node = tree.search(value);
      if (node != NULL)
	cout << "(" << value << ")";
      else
	{
	  cout << ".";
	  node = new AvlTreeTh<int>::Node (value);
	  tree.insert(node);
          k++;
	}
    }
  cout << endl << "verifying avl tree after insertions ... " 
       << endl;
  assert(tree.verifyAvl());
  cout << " done" << endl;
  
  m = 0;
  for (i = 0; i < n; i++)
    {
      value = 1+(int) (n*100.0*rand()/(RAND_MAX+1.0));
      node = tree.remove(value);
      if (node == NULL)
	cout << "(" << value << ")" ;
      else
	{
	  delete node;
          m++;
	  // assert(tree.verifyAvl());
	  cout << "." ;
	}
    }
    
  cout << endl << "verifying avl tree after deletions ... " ;
  assert(tree.verifyAvl());
  cout << " done" << endl;
  
  AvlTreeTh<int>::Iterator itor(tree);

  cout<< " Recorrido de Arbol usando hilos...."<< endl;
  j = 0;

  for (itor.reset_first(); itor.has_current(); itor.next())
    {
      node = itor.get_current();
      cout << "[" << node->get_key() << "] ";
      if (!(j%10)) 
	sumaPorHilos += node->get_key();
      j++;
    }

  value = 2663; //n*50;
  printf("Iterador parte desde %d\n\n", value); 
  for (itor.resetToKey(value); itor.has_current(); itor.next())
    {
      node = itor.get_current();
      cout << "[" << node->get_key() << "] ";
      if (!(j%10)) 
	sumaPorHilos += node->get_key();
      j++;
    }

  // preOrderStack(tree.getRoot(), print);
  //  inOrderRec(tree.getRoot(), print);
  printf("\n\n"); 
  
cout<<endl; 
  cout<< "    Nodos recorridos:         "<<j<<endl
      << "    Nodos insertados:         "<<k<<endl
      << "    Nodos eliminados:         "<<m<<endl
      << "    Nodos restantes:          "<<(k-m)<<endl
      << "    Suma en recorrido rec:    "<<sumaInOrderRec<<endl
      << "    Suma en recorrido hilado: "<<sumaPorHilos<<endl<<endl;
  
  cout << endl << "verifying avl tree search codition ... " ;
  assert(check_bst(tree.getRoot())); 
  cout << " done" << endl<<endl;

  destroyRec(tree.getRoot());
  
  
  cout << "sizeof(AvlTreeTh::Node)=" << sizeof(AvlTreeTh<int>::Node) << endl;
  cout << "testThAvlTree " << n << " " << t << endl;
}




