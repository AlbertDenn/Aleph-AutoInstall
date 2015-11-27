
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

# include <autosprintf.h>
# include <generate_graph.H>
# include <tpl_netgraph.H>

typedef Net_Node<string, long> Nodo;

typedef Net_Arc<Empty_Class, long> Tubo; 


typedef Net_Graph<Nodo, Tubo> Red;


void crear_tubo(Red & red, const string & src_name, const string & tgt_name, 
		const Red::Flow_Type & cap)
{
  Red::Node * src = red.search_node(src_name);
  if (src == NULL)
    src = red.insert_node(src_name);

  Red::Node * tgt = red.search_node(tgt_name);
  if (tgt == NULL)
    tgt = red.insert_node(tgt_name);

  red.insert_arc(src, tgt, cap);
}


void crear_red(Red & red)
{
  red.insert_node("C");
  red.insert_node("G");
  red.insert_node("J");
  red.insert_node("L");
  red.insert_node("A");
  red.insert_node("B");
  red.insert_node("F");
  red.insert_node("H");
  red.insert_node("M");
  red.insert_node("D");
  red.insert_node("E");
  red.insert_node("I");
  red.insert_node("K");

  crear_tubo(red, "A", "C", 5);
  crear_tubo(red, "A", "B", 7);
  crear_tubo(red, "A", "E", 3);
  crear_tubo(red, "A", "D", 6);

  crear_tubo(red, "B", "C", 5);
  crear_tubo(red, "B", "F", 6);

  crear_tubo(red, "C", "F", 3);

  crear_tubo(red, "D", "E", 4);

  crear_tubo(red, "E", "B", 4);
  crear_tubo(red, "E", "F", 5);
  crear_tubo(red, "E", "I", 8);

  crear_tubo(red, "F", "G", 5);
  crear_tubo(red, "F", "J", 7);
  crear_tubo(red, "F", "L", 6);
  crear_tubo(red, "F", "H", 4);

  crear_tubo(red, "G", "C", 4);

  crear_tubo(red, "J", "G", 6);
  crear_tubo(red, "J", "L", 5);

  crear_tubo(red, "H", "E", 3);
  crear_tubo(red, "H", "I", 4);
  crear_tubo(red, "H", "M", 5);

  crear_tubo(red, "I", "K", 4);

  crear_tubo(red, "K", "H", 3);
  crear_tubo(red, "K", "M", 4);

  crear_tubo(red, "L", "M", 6);
  crear_tubo(red, "L", "H", 4);
}


Path<Red> * path_ptr = NULL;

Red * red_ptr = NULL;

struct Sombra_Nodo
{
  string operator () (Red::Node * p) const
  {
    if (path_ptr == NULL)
      return "";

    return path_ptr->contains_node(p) ? "SHADOW-NODE" : "";
  }
};

struct Sombra_Arco
{
  string operator () (Red::Arc * a) const
  {
    if (path_ptr == NULL)
      return "Arc";

    return path_ptr->contains_arc(a) ? "SHADOW-ARC" : "ARC"; 
  }
};

struct Nodo_String
{
  string operator () (Red::Node * p) const { return p->get_info(); }
};


struct Arco_Normal
{
  string operator () (Red::Arc * a) const
  { 
    return gnu::autosprintf("%ld/%ld", a->cap, a->flow);
  }
};



int main()
{
  typedef No_Res_Arc<Red> No_Res;
  {
    Net_Graph<Nodo, Tubo> red; 

    crear_red(red);

    Path<Red> path(red);
    path_ptr = &path;

    Find_Aumenting_Path <Red, Find_Path_Depth_First> () (red, path, 1);
    increase_flow <Red> (red, path, 1);
    {
      ofstream out1("aum-3.mf", ios::out);
      generate_cross_graph
	<Red, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco, No_Res>
	(red, 5, 100, 100, out1);
    }

    Find_Aumenting_Path <Red, Find_Path_Depth_First> () (red, path, 3);
    increase_flow <Red> (red, path, 3);
    {
      ofstream out1("aum-4.mf", ios::out);
      generate_cross_graph
	<Red, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco, No_Res>
	(red, 5, 100, 100, out1);
    }

 
    Find_Aumenting_Path <Red> () (red, path, 2);
    increase_flow <Red> (red, path, 2);
    {
      ofstream out1("aum-1.mf", ios::out);
      generate_cross_graph
	<Red, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco, No_Res>
	(red, 5, 100, 100, out1);
    }

    
    Find_Aumenting_Path <Red> () (red, path, 4);
    increase_flow <Red> (red, path, 4);
    {
      ofstream out1("aum-2.mf", ios::out);
      generate_cross_graph
	<Red, Nodo_String, Arco_Normal, Sombra_Nodo, Sombra_Arco, No_Res>
	(red, 5, 100, 100, out1);
    }


    red.check_network();
  }

}



