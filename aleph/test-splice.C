
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
# include <tpl_dnode.H>

using namespace std;

using Node = Dnode<long>;

static long counter = 0;

Node * create_list_with_header(size_t n)
{
  Node * head = new Node;
  for (auto i = 0; i < n; ++i)
    head->append(new Node(counter++));

  return head;
}

Node * create_list_without_header(size_t n)
{
  Node * p = new Node(1000 + counter++);
  for (auto i = 1; i < n; ++i)
    p->append(new Node(1000 + counter++));
  return p;
}

Node * access(Node * list, size_t n)
{
  Node::Iterator it(list); 
  for (auto i = 0; i < n; ++i)
    it.next();
  return it.get_curr();
}

ostream & operator << (ostream & out, Node * p)
{
  for (Node::Iterator it(p); it.has_curr(); it.next())
    out << it.get_curr()->get_data() << " ";
  return out;
}


int main(int argc, char *argv[])
{
  if (argc != 4)
    {
      cout << "usage: " << argv[0] << " n m i" << endl
	   << endl
	   << "Where n: number of items of big list" << endl
	   << "      m: number of items of sublist to be inserted" << endl
	   << "      i: position where the sublist will be inserted" << endl;
      exit(0);
    }

  size_t n = atoi(argv[1]);
  size_t m = atoi(argv[2]);
  size_t i = atoi(argv[3]);

  Node * list = create_list_with_header(n);

  access(list, i)->splice(create_list_without_header(m));

  cout << list << endl;

  list->remove_all_and_delete();
  delete list;
}
