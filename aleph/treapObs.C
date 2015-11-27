
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
# include <fstream>
# include <time.h>
# include <aleph.H>
# include <tpl_binNodeUtils.H>


using namespace std;
using namespace Aleph;


unsigned long randomLong()
{
  return (long) (100.0*rand()/RAND_MAX);
}


fstream * file_ptr;


# include <tpl_treap.H>


void print_key(TreapTreeVtl<int>::Node *node)
{
  *file_ptr << node->get_key() << " ";
}

void print_prio(TreapTreeVtl<int>::Node *node)
{
  *file_ptr << node->getPriority() << " ";
}


fstream file[12];


int main(int argn, char *argc[])
{
  int n = 15;
  unsigned int t = time(0);
  int value;
  int val_del = 0;

  if (argn > 1)
    n = atoi(argc[1]);

  if (argn > 2)
    t = atoi(argc[2]);

  if (argn > 3)
    val_del = atoi(argc[3]);

  srand(t);

  cout << "treapObs " << n << " " << t << endl;

  TreapTreeVtl<int> tree( randomLong );
  TreapTreeVtl<int>::Node *node;
  int i;

  cout << "Inserting " << n << " random values in treee ...\n";

  for (i = 0; i < n; i++)
    {
      value = 1+(int) (n*5.0*rand()/(RAND_MAX+1.0));
      node = tree.search(value);
      if (node == NULL)
	{
          node = new TreapTreeVtl<int>::Node (value);
          tree.insert(node);
        }
    }

  file[0].open("treap-00.treap", ios::out);
  file[1].open("treap-01.treap", ios::out);
  file[2].open("treap-02.treap", ios::out);
  file[3].open("treap-03.treap", ios::out);
  file[4].open("treap-04.treap", ios::out);
  file[5].open("treap-05.treap", ios::out);
  file[6].open("treap-06.treap", ios::out);
  file[7].open("treap-07.treap", ios::out);
  file[8].open("treap-08.treap", ios::out);
  file[9].open("treap-09.treap", ios::out);
  file[10].open("treap-10.treap", ios::out);
  file[11].open("treap-11.treap", ios::out);

  file_ptr = &file[0];
  *file_ptr << "-b ";
  preOrderRec(tree.getRoot(), print_key);
  *file_ptr << "-D ";
  preOrderRec(tree.getRoot(), print_prio);
  file_ptr->close();

  cout << endl << "verifying TreapTreeVtl after insertions ... " 
       << endl;
  assert(is_treap(tree.getRoot()));
  cout << " done" << endl;

  tree.remove(val_del);

  cout << "The path length is " << compute_path_length(tree.getRoot())
       << endl;

  destroyRec(tree.getRoot()); 

  cout << "treapObs " << n << " " << t << endl;
}







