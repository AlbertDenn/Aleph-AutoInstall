
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

# include <fstream>
# include <iostream>
# include <topological_sort.H>


typedef List_Digraph<Graph_Node<string>, Graph_Arc<Empty_Class> > Digrafo;

Digrafo::Node * nodo(Digrafo & g, const string & str)
{
  Digrafo::Node * p = g.search_node(str);

  if (p == NULL)
    return g.insert_node(str);

  return p;
}

void build_digraph(Digrafo & g)
{
  g.insert_arc(nodo(g, "A"), nodo(g, "B"));
  g.insert_arc(nodo(g, "A"), nodo(g, "I"));
  g.insert_arc(nodo(g, "A"), nodo(g, "F"));
  g.insert_arc(nodo(g, "B"), nodo(g, "F"));
  g.insert_arc(nodo(g, "C"), nodo(g, "F"));
  g.insert_arc(nodo(g, "C"), nodo(g, "G"));
  g.insert_arc(nodo(g, "D"), nodo(g, "H"));
  g.insert_arc(nodo(g, "F"), nodo(g, "I"));
  g.insert_arc(nodo(g, "F"), nodo(g, "J"));
  g.insert_arc(nodo(g, "G"), nodo(g, "J"));
  g.insert_arc(nodo(g, "H"), nodo(g, "J"));
  g.insert_arc(nodo(g, "H"), nodo(g, "K"));
  g.insert_arc(nodo(g, "H"), nodo(g, "L"));
  g.insert_arc(nodo(g, "I"), nodo(g, "P"));
  g.insert_arc(nodo(g, "J"), nodo(g, "M"));
  g.insert_arc(nodo(g, "J"), nodo(g, "N"));
  g.insert_arc(nodo(g, "K"), nodo(g, "N"));
  g.insert_arc(nodo(g, "K"), nodo(g, "O"));
  g.insert_arc(nodo(g, "L"), nodo(g, "O"));
  g.insert_arc(nodo(g, "L"), nodo(g, "S"));
  g.insert_arc(nodo(g, "M"), nodo(g, "P"));
  g.insert_arc(nodo(g, "M"), nodo(g, "Q"));
  g.insert_arc(nodo(g, "N"), nodo(g, "R"));
  g.insert_arc(nodo(g, "O"), nodo(g, "R"));
  g.insert_arc(nodo(g, "O"), nodo(g, "S"));
}


void imprime_lista(DynDlist<Digrafo::Node*> & l, const string & nom)
{
  ofstream out(nom.c_str(), ios::out);

  for (DynDlist<Digrafo::Node*>::Iterator it(l); it.has_current(); it.next())
    {
      out << it.get_current()->get_info();

      if (not it.is_in_last())
	out << "$\\rightarrow$";
    }

  out << endl;
}


int main()
{
  Digrafo g;

  build_digraph(g);

  DynDlist<Digrafo::Node*> list;

  Topological_Sort <Digrafo> () (g, list);

  imprime_lista(list, "top-1.tex");  

  list.empty();

  Topological_Sort <Digrafo> () (g, list);

  imprime_lista(list, "top-2.tex");  
}
