
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
# include <autosprintf.h>
# include <tpl_matgraph.H>
# include <mat_latex.H>
# include <warshall.H>

using namespace std;

typedef List_Digraph<Graph_Node<int>, Graph_Arc<Empty_Class> > Grafo;

void build_graph(Grafo & g)
{
  Grafo::Node * n1 = g.insert_node(1);
  Grafo::Node * n2 = g.insert_node(2);
  Grafo::Node * n3 = g.insert_node(3);
  Grafo::Node * n4 = g.insert_node(4);
  Grafo::Node * n5 = g.insert_node(5);
  Grafo::Node * n6 = g.insert_node(6);
  Grafo::Node * n7 = g.insert_node(7);
  Grafo::Node * n8 = g.insert_node(8);
  Grafo::Node * n9 = g.insert_node(9);
  Grafo::Node * n10 = g.insert_node(10);
  Grafo::Node * n11 = g.insert_node(11);
  Grafo::Node * n12 = g.insert_node(12);
  Grafo::Node * n13 = g.insert_node(13);
  Grafo::Node * n14 = g.insert_node(14);

  g.insert_arc(n1, n3, Empty_Class());

  g.insert_arc(n2, n4, Empty_Class());

  g.insert_arc(n3, n5, Empty_Class());
  g.insert_arc(n3, n6, Empty_Class());

  g.insert_arc(n4, n1, Empty_Class());
  g.insert_arc(n4, n6, Empty_Class());

  g.insert_arc(n5, n7, Empty_Class());

  g.insert_arc(n6, n10, Empty_Class());

  g.insert_arc(n7, n4, Empty_Class());
  g.insert_arc(n7, n9, Empty_Class());
  g.insert_arc(n7, n8, Empty_Class());

  g.insert_arc(n8, n12, Empty_Class());
  
  g.insert_arc(n9, n10, Empty_Class());
  g.insert_arc(n9, n12, Empty_Class());

  g.insert_arc(n10, n13, Empty_Class());

  g.insert_arc(n11, n13, Empty_Class());

  g.insert_arc(n12, n11, Empty_Class());

  g.insert_arc(n13, n14, Empty_Class());

  g.insert_arc(n14, n12, Empty_Class());
}




    template <class Mat>
struct Write_i
{
  string operator () (Mat & , const long & i)
  {
    return gnu::autosprintf("%ld", i + 1);
  }
};

    template <class Mat>
struct Write_ij
{
  string operator () (Mat & mat, const long & i, const long & j)
  {
    int d = mat(i,j);
    return gnu::autosprintf("%d", d);
  }
};


typedef Bit_Mat_Graph<Grafo> Bitmat;

int main()
{
    ofstream out1("warshall-mat-0.tex", ios::trunc);

    Grafo g;

    build_graph(g);

    Bitmat m(g);

    mat_to_latex <Bitmat, Write_i<Bitmat>, Write_i<Bitmat>, Write_ij<Bitmat> >
      (m, g.get_num_nodes(), g.get_num_nodes(), out1);


    Bitmat w;

    Warshall_Compute_Transitive_Clausure <Grafo> () (g, w);

    ofstream out2("warshall-mat-1.tex", ios::trunc);

    mat_to_latex <Bitmat, Write_i<Bitmat>, Write_i<Bitmat>, Write_ij<Bitmat> >
      (w, g.get_num_nodes(), g.get_num_nodes(), out2);
}
