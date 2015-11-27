
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

  
# include <stdlib.h>
# include <time.h>
# include <iostream>
# include <aleph.H>
# include <tpl_avlTreeThRkC.H>
# include <tpl_threadNodeUtils.H>

using namespace std;
using namespace ThreadNode_Utils;

long long int sumaPorHilos = 0;
int cont = 0;
long long int sumaInOrderRec = 0; 

static void print(AvlTreeThRk<int>::Node *p) 
{
  cout << "[" << p->get_key() << "]";
}


int main(int argn, char *argc[])
{
  cout << "testAvlThRkTree ";

  AvlTreeThRk<int> treei1,treei2;
  AvlTreeThRk<int>::Node *node;
  AvlTreeThRk<int> treed1,treed2;
  int i;
  for (i = 1; i <= 33; i+=2)
    {  
      node = new AvlTreeThRk<int>::Node (i);
      treei1.insert(node);
      assert(treei1.verifyRank());
      assert(treei1.verifyAvl());
      node = new AvlTreeThRk<int>::Node (i);
      treei2.insert(node);
    }
  for (i = 22; i <= 52; i+=2)
    {  
      node = new AvlTreeThRk<int>::Node (i);
      treed1.insert(node);
      node = new AvlTreeThRk<int>::Node (i);
      treed2.insert(node);
    }
  
  node = new AvlTreeThRk<int>::Node (3);
  treed2.insert(node);
  node = new AvlTreeThRk<int>::Node (9);
  treed2.insert(node);
  node = new AvlTreeThRk<int>::Node (13);
  treed2.insert(node);
  node = new AvlTreeThRk<int>::Node (28);
  treei2.insert(node);
   node = new AvlTreeThRk<int>::Node (32);
  treei2.insert(node);
  cout<<endl;
  cout<< "Union of two trees with interseption of their domains and no common elements:"<<endl;
  cout<< "Left Tree:"<<endl;
  inOrderRec(treei1.getRoot(), print);
  cout<< endl<<"Right Tree:"<<endl;
  inOrderRec(treed1.getRoot(), print);
  treei1.unionWithNoInterception(treed1);
  cout<<endl <<"Union Tree: "<<endl;
  inOrderRec(treei1.getRoot(), print);
  cout<<endl<<endl;

  cout<< "Union of two trees with interseption of their domains and  common elements:"<<endl;
  cout<< "Left Tree:"<<endl;
  inOrderRec(treei2.getRoot(), print);
  cout<< endl<<"Right Tree:"<<endl;
  inOrderRec(treed2.getRoot(), print);
  treei2.unionWithInterception(treed2);
  cout<< endl<<"Union Tree: "<<endl;
  inOrderRec(treei2.getRoot(), print);
  cout<<endl <<"Common elements: "<<endl;
  inOrderRec(treed2.getRoot(), print);
  cout<<endl;


  AvlTreeThRk<int> treei3,treei4;
  AvlTreeThRk<int> treed3,treed4;
 
  for (i = 21; i <= 55; i+=2)
    {  
      node = new AvlTreeThRk<int>::Node (i);
      treei3.insert(node);
      assert(treei3.verifyRank());
      assert(treei3.verifyAvl());
      node = new AvlTreeThRk<int>::Node (i);
      treei4.insert(node);
    }
  for (i = 2; i <= 66; i+=2)
    {  
      node = new AvlTreeThRk<int>::Node (i);
      treed3.insert(node);
      node = new AvlTreeThRk<int>::Node (i);
      treed4.insert(node);
    }
  
  node = new AvlTreeThRk<int>::Node (31);
  treed4.insert(node);
  node = new AvlTreeThRk<int>::Node (29);
  treed4.insert(node);
  node = new AvlTreeThRk<int>::Node (41);
  treed4.insert(node);
  node = new AvlTreeThRk<int>::Node (28);
  treei4.insert(node);
  node = new AvlTreeThRk<int>::Node (32);
  treei4.insert(node);
  cout<<endl;
  cout<< "Union of one tree contein in the domain of other tree with"<<endl
      <<"that tree and no common elements:"<<endl;
  cout<< "Left Tree:"<<endl;
  inOrderRec(treei3.getRoot(), print);
  cout<< endl<<"Right Tree:"<<endl;
  inOrderRec(treed3.getRoot(), print);
  treei3.unionWithNoInterception(treed3);
  cout<<endl <<"Union Tree: "<<endl;
  inOrderRec(treei3.getRoot(), print);
  cout<<endl<<endl;

  cout<< "Union of one tree contein in the domain of other tree with"<<endl
      <<"that tree and common elements:"<<endl;
   cout<< "Left Tree:"<<endl;
  inOrderRec(treei4.getRoot(), print);
  cout<< endl<<"Right Tree:"<<endl;
  inOrderRec(treed4.getRoot(), print);
  treei4.unionWithInterception(treed4);
  cout<< endl<<"Union Tree: "<<endl;
  inOrderRec(treei4.getRoot(), print);
  cout<<endl <<"Common elements: "<<endl;
  inOrderRec(treed4.getRoot(), print);
  cout<<endl;


  cout<<endl;
  destroyRec(treei1.getRoot());
  destroyRec(treed1.getRoot());
  destroyRec(treei2.getRoot());
  destroyRec(treed2.getRoot());
  destroyRec(treei3.getRoot());
  destroyRec(treed3.getRoot());
  destroyRec(treei4.getRoot());
  destroyRec(treed4.getRoot());

}




