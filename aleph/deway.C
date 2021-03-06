
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
# include <tpl_binNodeUtils.H>
# include <tpl_tree_node.H>
# include <generate_tree.H>

using namespace std;
using namespace Aleph;

void deway(Tree_Node<int> * p, int prefix[], const int & len, 
	   const size_t & dim)
{
  int i = 1;

  if (len == 0)
    cout << "Root ";
  else
    cout << "Node " << prefix[i++];
  
  while (i <= len)
    cout << "." << prefix[i++];

  cout << " \"" << p->get_key() << "\"" << endl;
  
  if (len >= dim)
    throw std::overflow_error("Array dimension es smaller than Deway chain");
  
  Tree_Node<int> * child = p->get_left_child(); 
  
  for (int i = 0; child != NULL; ++i, child = child->get_right_sibling())
    {
      prefix[len + 1] = i;
      deway(child, prefix, len + 1, dim);
    }
}

struct Convert
{
  const string operator () (Tree_Node<int> * p)
  {
    char buf[512];
    snprintf(buf, 512, "%d", p->get_key());
    
    return string(buf);
  }
};


void deway(Tree_Node<int> * p, const int & h)
{
  const size_t dim = 10*h;

  int * prefix = new int [dim];

  for (int i = 0; p != NULL; ++i, p = p->get_right_sibling())
    {
      prefix[0] = i;
      deway(p, prefix, 0, dim);
    }
}


    template <class Node>
static void printNode(Node * node, int, int)
{ 
  cout << " " << node->get_key();
}


int random(int l, int r)
{
  assert(l <= r);

  int n = r - l;

  int rd = 1 + (int) (1.0*n*rand()/(RAND_MAX+1.0));

  return l + rd - 1;
}


BinNode<int> * random_tree(int l, int r)
{
  if (l > r)
    return NULL;

  BinNode<int> * root = new BinNode<int> (random(l, r));

  LLINK(root) = random_tree(l, KEY(root) - 1);
  RLINK(root) = random_tree(KEY(root) + 1, r);

  return root;
}


int main(int argn, char * argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 2;

  unsigned int t = time(0);

  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << argc[0] << " " << n << " " << t << endl;

  BinNode<int> * bp = random_tree(1, n);

  cout << "Prefijo:";
  preOrderRec(bp,  printNode);
  cout << endl << endl;
  
  cout << "Infijo:";
  inOrderRec(bp,  printNode);
  cout << endl << endl;
  
  Tree_Node<int> * tree = bin_to_forest< Tree_Node<int>, BinNode<int> > (bp);

  cout << "Preorder:" << endl;
  forest_preorder_traversal(tree, printNode);
  cout << endl << endl;

  cout << "Postorder:" << endl;
  forest_postorder_traversal(tree, printNode);
  cout << endl << endl;


  BinNode<int> * prb = forest_to_bin< Tree_Node<int>, BinNode<int> > (tree);

  assert(areEquivalents(prb, bp));

  deway(tree, computeHeightRec(bp));
}
