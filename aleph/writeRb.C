
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
# include <fstream>
# include <aleph.H>
# include <tpl_rb_tree.H>
# include <tpl_binNodeUtils.H>

using namespace std;
using namespace Aleph;


ofstream output("rb-example-aux.Tree", ios::out); 

int position = 0;


ofstream file("rb-tree-aux.Tree", ios::out); 

static void print_key(Rb_Tree<int>::Node *p, int, int) 
{
  file << p->get_key() << " ";
}


static void print_color(Rb_Tree<int>::Node *p, int, int pos) 
{
  if (COLOR(p) == RED)
    file << pos << " ";
}


static void print_key_ex(Rb_Tree<int>::Node *p, int, int) 
{
  output << p->get_key() << " ";
}


static void print_color_ex(Rb_Tree<int>::Node *p, int, int pos) 
{
  if (COLOR(p) == RED)
    output << pos << " ";
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

  cout << "writeRb " << n << " " << t << endl;

  {
    Rb_Tree<int> tree;
    Rb_Tree<int>::Node *node;
    int i;

    for (i = 0; i < 30; i++)
      {
	do
	  {
	    value = (int) (100.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	  } while (node not_eq NULL);
	node = new Rb_Tree<int>::Node (value);
	tree.insert(node);
      }

    assert(is_red_black(tree.getRoot())); 

    preOrderRec(tree.getRoot(), print_key_ex);
    output << endl << "START-SHADOW "; 
    position = 0; inOrderRec(tree.getRoot(), print_color_ex);
    output << endl;
    
    destroyRec(tree.getRoot());
  }

  {
     Rb_Tree<int> tree;
    Rb_Tree<int>::Node *node;
    int i;
    for (i = 0; i < n; i++)
      {
	do
	  {
	    value = (int) (n*10.0*rand()/(RAND_MAX+1.0));
	    node = tree.search(value);
	  } while (node not_eq NULL);
	node = new Rb_Tree<int>::Node (value);
	tree.insert(node);
      }

    assert(is_red_black(tree.getRoot())); 

    preOrderRec(tree.getRoot(), print_key);
    file << "START-SHADOW ";
    position = 0; inOrderRec(tree.getRoot(), print_color);

    destroyRec(tree.getRoot());
  }
}

