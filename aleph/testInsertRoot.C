
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


# include <time.h>
# include <iostream>
# include <aleph.H>
# include <tpl_dynArray.H>
# include <tpl_binNode.H>
# include <tpl_binNodeUtils.H>

using namespace std;
using namespace Aleph;


static void printNode(BinNode<int>* node)
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

  DynArray<int> array;

  cout << argc[0] << " " << n << " " << t << endl;

  BinNode<int> *root = new BinNode<int> ((int) (10.0*n*rand()/(RAND_MAX+1.0)));
  cout << root->get_key() << " ";

  array[0] = root->get_key();

  BinNode<int> *p;
  int value;
  int ins_count = 1;

  for (i = 1; i < n; i++)
    {
      do 
	{
	  value = (int) (10.0*n*rand()/(RAND_MAX+1.0));
	  p = searchInBinTree(root, value);
	}
      while (p != NULL);

      p = new BinNode<int> (value);
      cout << value << " ";
      root = insert_root(root, p);

      array[i] = value;

      ins_count++;
    }
  
  cout << endl << ins_count << " items inserted" << endl;
  assert(check_bst(root));

  cout << "prefix: ";
  preOrderThreaded(root, printNode);
  cout << endl;

  for (i = 0; i < 10; i++)
    {
      int idx;
      do
	{
	  idx = (int) (1.0*n*rand()/(RAND_MAX+1.0));

	  value = array[idx];

	  p = searchInBinTree(root, value);

	}
      while (p == NULL);

      cout << value << " ";

      p = remove_from_bst(root, value);

      assert(p != NULL);
    }

  cout << endl;
  preOrderThreaded(root, printNode);
  cout << endl;
  destroyRec(root);
}
