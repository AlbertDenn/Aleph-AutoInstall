
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


# include <limits>
# include <iostream>
# include <string>
# include <tpl_graph_utils.H>
# include <Floyd.H>
# include <mat_path.H>

using namespace std;

using namespace Aleph;

typedef List_Digraph<Graph_Node<string>, Graph_Arc<int>> Grafo;

void insertar_arco(Grafo &       grafo, 
		   const string & src_name, 
		   const string & tgt_name,
		   const int      distancia)
{
  Grafo::Node * n1 = grafo.find_node(src_name);

  if (n1 == NULL)
    n1 = grafo.insert_node(src_name);

  Grafo::Node * n2 = grafo.find_node(tgt_name);

  if (n2 == NULL)
    n2 = grafo.insert_node(tgt_name);

  grafo.insert_arc(n1, n2, distancia);
}



void build_test_graph(Grafo & g)
{
  g.insert_node("A");
  g.insert_node("B");
  g.insert_node("C");
  g.insert_node("D");
  g.insert_node("E");
  g.insert_node("F");
  g.insert_node("G");
  g.insert_node("H");
  g.insert_node("I");

  insertar_arco(g, "A", "B", 2);
  insertar_arco(g, "A", "F", 5);
  insertar_arco(g, "B", "F", 1);
  insertar_arco(g, "B", "D", 3);
  insertar_arco(g, "C", "A", 1);  
  insertar_arco(g, "C", "E", 4);  
  insertar_arco(g, "F", "D", -2);
  insertar_arco(g, "F", "C", -1);
  insertar_arco(g, "F", "E", 2);
  insertar_arco(g, "D", "F", 2);
  insertar_arco(g, "D", "H", 4); 
  insertar_arco(g, "E", "G", 2);
  insertar_arco(g, "E", "I", -2);
  insertar_arco(g, "G", "D", 3);
  insertar_arco(g, "G", "F", -1);
  insertar_arco(g, "G", "H", 2);
  insertar_arco(g, "H", "D", -2);
  insertar_arco(g, "H", "G", -1);
  insertar_arco(g, "I", "G", 4);
  insertar_arco(g, "I", "H", 3);

}

int main() 
{
  Grafo g;

  build_test_graph(g);

  const size_t & N = g.get_num_nodes();

  DynMatrix<int> cost_mat(N, N);

  DynMatrix<long> path_mat(N, N);

  bool ret = Floyd_All_Shortest_Paths <Grafo> (g) (cost_mat, path_mat);

  if (ret)
    cout << "All ok" << endl;
  else
    cout << "Negative cyles" << endl;

  for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
	cout << cost_mat.access(i,j) << " "; 
      cout << endl;
    }
}
