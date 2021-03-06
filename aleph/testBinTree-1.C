
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


# include <aleph.H>
# include <tpl_binTree.H>
# include <tpl_binNodeUtils.H>
# include <stdlib.h>
# include <iostream>
# include <time.h>


using namespace Aleph;

static void printNode(BinTreeVtl<int>::Node* node, int, int)
{
  cout << node->get_key() << " ";
}

int main(int argn, char *argc[])
{
  int i, n = argc[1] ? atoi(argc[1]) : 1000;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  BinTreeVtl<int>  tree;
  BinTreeVtl<int>::Node *node;
  int value;

  cout << "Inserting " << n << " random values in treee ...\n";

  int ins_count = 0;

  for (i = 0; i < n; i++)
    {
      do 
	{
	  value = (int) (10.0*n*rand()/(RAND_MAX+1.0));
	  node = tree.search(value);
	  }
      while (node not_eq NULL);

      node = new BinTreeVtl<int>::Node (value);
      tree.insert(node);
      ins_count++;
    }

  assert(tree.verifyBin());
  cout << endl << ins_count << " insertions" << endl
       << "prefijo: " << endl;
  preOrderRec(tree.getRoot(), printNode);
  cout << endl << endl;

  cout << "sufijo: " << endl;
  postOrderRec(tree.getRoot(), printNode);
  cout << endl << endl;

  int ipl = compute_path_length(tree.getRoot());
  
  cout << "IPL = " << ipl << endl
       << "EPL = " << ipl + 2*n << endl;

  int del_count = 0;

  for (i = 0; i < n/2; i++)
    {
      while (1)
	{
	  value = 1+(int) (n*10.0*rand()/(RAND_MAX+1.0));
	  node = tree.remove(value);
	  if (node != NULL)
	    {
	      del_count++;
	      cout << node->get_key() << " ";
	      delete node;
	      break;
	    }
	}
    }

  cout << endl << del_count << " deletions" << endl
       << "prefijo: ";
  preOrderRec(tree.getRoot(), printNode);
  cout << endl;
  cout << endl;

  assert(tree.verifyBin());

  destroyRec(tree.getRoot());

  cout << "testBinTreeVtl " << n << " " << t << endl;
}

// 1018058241
