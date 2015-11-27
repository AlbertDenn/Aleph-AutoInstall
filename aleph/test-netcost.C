
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


# include <generate_graph.H>
# include <tpl_netcost.H>

# include <autosprintf.h>

typedef Net_Cost_Node<string> Nodo;

typedef Net_Cost_Arc<Empty_Class> Tubo;


typedef Net_Cost_Graph<Nodo, Tubo> Red;


void crear_tubo(Red & red, const string & src_name, const string & tgt_name, 
		const Red::Flow_Type & cap, const Red::Flow_Type & cost = 0)
{
  Red::Node * src = red.find_node(src_name);
  if (src == NULL)
    src = red.insert_node(src_name);
 
  Red::Node * tgt = red.find_node(tgt_name);
  if (tgt == NULL)
    tgt = red.insert_node(tgt_name);

  red.insert_arc(src, tgt, cap, cost);
}


Red crear_red()
{
  Red red;
  crear_tubo(red, "S", "A", 70, 20); 
  //  crear_tubo(red, "S", "B", 70, 120);
  crear_tubo(red, "S", "C", 50, 50); 

  //  crear_tubo(red, "A", "E", 20, 20);
  crear_tubo(red, "A", "F", 60, 10);
  crear_tubo(red, "A", "B", 30, 20);
  crear_tubo(red, "C", "B", 30, 5);
  crear_tubo(red, "B", "E", 40, 20);
  crear_tubo(red, "B", "D", 10, 1);
  crear_tubo(red, "C", "D", 50, 40);
  crear_tubo(red, "C", "E", 30, 10);
  crear_tubo(red, "E", "G", 20, 8);
  crear_tubo(red, "E", "H", 50, 30);
  crear_tubo(red, "E", "I", 20, 30);
  crear_tubo(red, "D", "I", 30, 15);
  crear_tubo(red, "D", "E", 80, 30);
  crear_tubo(red, "I", "F", 30, 5);
  crear_tubo(red, "I", "G", 40, 90);
  crear_tubo(red, "I", "H", 50, 20);
  crear_tubo(red, "K", "J", 30, 2);
  crear_tubo(red, "F", "J", 60, 50);
  crear_tubo(red, "D", "H", 40, 35);
  crear_tubo(red, "H", "G", 15, 4);
  crear_tubo(red, "H", "K", 50, 25);
  //  crear_tubo(red, "I", "J", 30, 25);
  crear_tubo(red, "J", "T", 70, 20);
  crear_tubo(red, "G", "K", 50, 30);
  crear_tubo(red, "K", "T", 60, 15);
  return red;
}


int main()
{
  Red red1 = crear_red();

  max_flow_min_cost_by_cycle_canceling<Red, Edmonds_Karp_Maximum_Flow>(red1);

  // Residual_Net<Red::Flow_Type> res_net = build_residual_net(red1);

  // print(res_net, cout);

  Red red2 = red1;
  Red red3 = red1;
  Red red4 = red1;
  Red red5 = red1;


}
