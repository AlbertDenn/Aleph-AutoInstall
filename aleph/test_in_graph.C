
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

# include <tpl_indexGraph.H>


    template <class Tn>
struct My_Node : public Graph_Node<Tn>
{
  My_Node(const Tn& tn) : Graph_Node<Tn>(tn) { }

  My_Node() { }
};


    template <class Ta>
struct My_Arc : public Graph_Arc<Ta>
{
  My_Arc(const Ta& ta) : Graph_Arc<Ta>(ta) { }

  My_Arc(void * src, void * tgt, const Ta& ta) : Graph_Arc<Ta>(src, tgt, ta) { }

  My_Arc() { }
};


struct Ciudad
{
  string nombre;

  Ciudad() {}

  Ciudad(const string & n) : nombre(n) {}

  bool operator < (const Ciudad & c) const
  {
    return nombre < c.nombre;
  }
};

struct Via
{
  string nombre;

  Via() {}

  Via(const string & n) : nombre(n) {}

};

typedef My_Node<Ciudad> Nodo_Ciudad;

typedef My_Arc<Via> Arco_Via;


int main()
{

  List_Graph<Nodo_Ciudad, Arco_Via> g; 
  
  Index_Graph<List_Graph<Nodo_Ciudad, Arco_Via> > idx(g);

  Nodo_Ciudad * n1 = idx.insert_node(Ciudad("Merida"));
  Nodo_Ciudad * n2 = idx.insert_node(Ciudad("Valera"));
  Nodo_Ciudad * n3 = idx.insert_node(Ciudad("Caracas"));
  Nodo_Ciudad * n4 = idx.insert_node(Ciudad("Trujillo"));
  Nodo_Ciudad * n5 = idx.insert_node(Ciudad("Barinas"));

  assert(idx.search_node(Ciudad("Merida")) == n1);

  Via v("Prueba");

  cout << "Hay " << idx.get_num_arcs() << endl;

  DynArray<List_Graph<Nodo_Ciudad, Arco_Via>::Arc*> arcos;
  arcos.append(idx.insert_arc(n1, n2, v));
  arcos.append(idx.insert_arc(n1, n3, v));
  arcos.append(idx.insert_arc(n1, n4, v));
  arcos.append(idx.insert_arc(n1, n5, v));
  arcos.append(idx.insert_arc(n2, n3, v));
  arcos.append(idx.insert_arc(n2, n4, v));
  arcos.append(idx.insert_arc(n2, n5, v));
  arcos.append(idx.insert_arc(n3, n4, v));
  arcos.append(idx.insert_arc(n3, n5, v));
  arcos.append(idx.insert_arc(n4, n5, v));

  assert(idx.search_arc(n5, n4) != NULL);

  for (int i = 0; i < arcos.size(); i++)
    idx.remove_arc(arcos.access(i));

  idx.remove_node(n1);

}
