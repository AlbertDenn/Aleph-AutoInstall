
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
# include <tpl_tree_node.H>

using namespace std;


using Node = Tree_Node<long>;

static long count = 0;

ostream & operator << (ostream & out, Node * p)
{
  p->traverse([&out] (auto p) 
	      {
		out << p->get_key() << " ";
		return true;
	      });
  // for (auto it = p->get_it(); it.has_curr(); it.next())
  //   out << it.get_curr()->get_key() << " ";
  return out;
}


Node * test()
{
  Node * leaf1 = new Node(::count++);
  Node * leaf2 = new Node(::count++);
  Node * leaf3 = new Node(::count++);
  Node * leaf4 = new Node(::count++);

  Node * root = new Node(::count++);

  return root->join(leaf1)->join(leaf2)->join(leaf3)->join(leaf4);

  // root->join(leaf1)->join(leaf2);

  // cout << "Root = " << root << endl
  //      << endl;

  // root->join(leaf3);

  // cout << "Root = " << root << endl
  //      << endl;

  // cout << "****";
  // root->mutable_for_each([] (auto p) { cout << " " << p->get_key(); });
  // cout << endl;

  // return root;
}

int main()
{
  auto tree1 = test();
  
  auto tree2 = test();

  cout << "tree1: " << tree1 << endl
       << endl
       << "tree2: " << tree2 << endl
       << endl;

  Node * root = new Node(::count++); 
  root->join(tree1)->join(tree2);

  cout << "root =" << root << endl
       << endl;

  destroy_tree(root);
}
