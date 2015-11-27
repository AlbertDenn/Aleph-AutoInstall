
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
# include <tpl_bipartite.H>

typedef List_Graph< Graph_Node<string>, Graph_Arc<Empty_Class> > Grafo;

void crear_lazo(Grafo & g, const string & src_name, const string & tgt_name)
{
  Grafo::Node * src = g.find_node(src_name);
  if (src == NULL)
    src = g.insert_node(src_name);

  Grafo::Node * tgt = g.find_node(tgt_name);
  if (tgt == NULL)
    tgt = g.insert_node(tgt_name);

  g.insert_arc(src, tgt);
}


void crear_grafo(Grafo & g)
{
  crear_lazo(g, "A", "1");
  crear_lazo(g, "A", "2");
  crear_lazo(g, "A", "4");

  crear_lazo(g, "B", "1");
  crear_lazo(g, "B", "2");
  crear_lazo(g, "B", "5");

  crear_lazo(g, "C", "2");
  crear_lazo(g, "C", "4");
  crear_lazo(g, "C", "7");

  crear_lazo(g, "D", "3");
  crear_lazo(g, "D", "5");
  crear_lazo(g, "D", "6");

  crear_lazo(g, "E", "1");
  crear_lazo(g, "E", "4");
  crear_lazo(g, "E", "6");
  crear_lazo(g, "E", "7");
  

  crear_lazo(g, "F", "5");
  crear_lazo(g, "F", "6");
  crear_lazo(g, "F", "7");

  crear_lazo(g, "G", "4");
  crear_lazo(g, "G", "6");
  crear_lazo(g, "G", "7");

  //  crear_lazo(g, "G", "x");

//   crear_lazo(g, "x", "7");
//   crear_lazo(g, "x", "u");
//   crear_lazo(g, "x", "y");
//   crear_lazo(g, "x", "z");
//   crear_lazo(g, "y", "z");
//   crear_lazo(g, "u", "z");

}

int main()
{
  Grafo g;

  crear_grafo(g);

  DynDlist<Grafo::Node *> l, r;

  Compute_Bipartite <Grafo> () (g, l, r);

  for (DynDlist<Grafo::Node *>::Iterator it(l); it.has_current(); it.next())
    cout << it.get_current()->get_info() << ", ";
  cout << endl;

  for (DynDlist<Grafo::Node *>::Iterator it(r); it.has_current(); it.next())
    cout << it.get_current()->get_info() << ", ";
  cout << endl;

  DynDlist<Grafo::Arc *> m;

  Compute_Maximum_Cardinality_Bipartite_Matching <Grafo> () (g, m);

  for (DynDlist<Grafo::Arc *>::Iterator it(m); it.has_current(); it.next())
    {
      Grafo::Arc * a = it.get_current();
      Grafo::Node * src = g.get_src_node(a);
      Grafo::Node * tgt = g.get_tgt_node(a);
      cout << src->get_info() << "--" << tgt->get_info() << endl;
    }
  cout << endl;

}
