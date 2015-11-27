
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
# include <tpl_net.H>

# include <autosprintf.h>


typedef Net_Node<string> Nodo;

typedef Net_Arc<Empty_Class> Tubo;

typedef Net_Graph<Nodo, Tubo> Red;


void crear_tubo(Red & red, const string & src_name, const string & tgt_name, 
		const Red::Flow_Type & cap)
{
  Red::Node * src = red.find_node(src_name);
  if (src == NULL)
    src = red.insert_node(src_name);

  Red::Node * tgt = red.find_node(tgt_name);
  if (tgt == NULL)
    tgt = red.insert_node(tgt_name);

  red.insert_arc(src, tgt, cap);
}


Red crear_red()
{
  Red red;

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
  // crear_tubo(red, "D", "I", 5);

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

  return red;
}

struct Nodo_String
{
  string operator () (Red::Node * p) const { return p->get_info(); }
};

struct Arco_Normal
{
  string operator () (Red::Arc * a) const
  { 
    return gnu::autosprintf("%.0f/%.0f", a->cap, a->flow);
  }
};

int main()
{
  Net_Graph<Nodo, Tubo> red1 = crear_red();

  Path<Red> p = find_aumenting_path_dfs(red1, 5.0);
  increase_flow(red1, p);
  cout << p << endl;
  assert(red1.check_network()); 

  SemiPath<Red> pa = find_aumenting_semi_path_dfs(red1, 2);
  if (get<0>(pa))
    {
      cout << "slack = " << get<1>(pa) << endl;
      const DynList<Parc<Red>> & semi_path = get<2>(pa);
      semi_path.for_each([] (const Parc<Red> & p)
      {
	typename Red::Arc * a = get<0>(p);
	typename Red::Node * s = (typename Red::Node*) a->src_node;
	typename Red::Node * t = (typename Red::Node*) a->tgt_node;
	const string res = get<1>(p) ? " normal" : " reduced";
	cout << s->get_info() << "(" << a->cap << "," << a->flow << ")" 
	     << t->get_info() << res << endl;
      });

      increase_flow(red1, semi_path, get<1>(pa));
      cout << "Red 1 = " << red1.flow_value() << endl;
      assert(red1.check_network());
    }

  pa = find_decrementing_path_dfs(red1, 2);
  if (get<0>(pa))
    {
      cout << "slack = " << get<1>(pa) << endl;
      const DynList<Parc<Red>> & semi_path = get<2>(pa);
      semi_path.for_each([] (const Parc<Red> & p)
      {
	typename Red::Arc * a = get<0>(p);
	typename Red::Node * s = (typename Red::Node*) a->src_node;
	typename Red::Node * t = (typename Red::Node*) a->tgt_node;
	const string res = get<1>(p) ? " normal" : " reduced";
	cout << s->get_info() << "(" << a->cap << "," << a->flow << ")" 
	     << t->get_info() << res << endl;
      });
      decrease_flow(red1, semi_path, get<1>(pa));
      cout << "Red 1 = " << red1.flow_value() << endl;
      assert(red1.check_network());
    }

  pa = find_decrementing_path_bfs(red1, 1);
  if (get<0>(pa))
    {
      cout << "slack = " << get<1>(pa) << endl;
      const DynList<Parc<Red>> & semi_path = get<2>(pa);
      semi_path.for_each([] (const Parc<Red> & p)
      {
	typename Red::Arc * a = get<0>(p);
	typename Red::Node * s = (typename Red::Node*) a->src_node;
	typename Red::Node * t = (typename Red::Node*) a->tgt_node;
	const string res = get<1>(p) ? " normal" : " reduced";
	cout << s->get_info() << "(" << a->cap << "," << a->flow << ")" 
	     << t->get_info() << res << endl;
      });
      decrease_flow(red1, semi_path, get<1>(pa));
      cout << "Red 1 = " << red1.flow_value() << endl;
      assert(red1.check_network());
    }

  // increase_flow(red1, semi_path, slack);
  // cout << "Red 1 = " << red1.flow_value() << endl;

  // assert(red1.check_network());

  // slack = find_decrementing_path_dfs(red1, semi_path, 2);
  // cout << "slack = " << slack << endl;
  // semi_path.for_each([] (const Parc<Red> & p)
  // {
  //   typename Red::Arc * a = get<0>(p);
  //   typename Red::Node * s = (typename Red::Node*) a->src_node;
  //   typename Red::Node * t = (typename Red::Node*) a->tgt_node;
  //   const string res = get<1>(p) ? " forward" : " backward";
  //   cout << s->get_info() << "(" << a->cap << "," << a->flow << ")" 
  // 	 << t->get_info() << res << endl;
  // });
  

  ford_fulkerson_maximum_flow(red1);

  Net_Graph<Nodo, Tubo> red2 = crear_red();
  edmonds_karp_maximum_flow(red2);
   assert(red2.check_network());

  cout << "Red 1 = " << red1.flow_value() << endl
       << "Red 2 = " << red2.flow_value() << endl;

  Net_Graph<Nodo, Tubo> red3 = crear_red();
  fifo_preflow_maximum_flow(red3);
  cout << "Red 3 = " << red3.flow_value() << endl;

  Net_Graph<Nodo, Tubo> red4 = crear_red();
  heap_preflow_maximum_flow(red4);
  cout << "Red 4 = " << red4.flow_value() << endl;

  Net_Graph<Nodo, Tubo> red5 = crear_red();
  random_preflow_maximum_flow(red5);
  cout << "Red 5 = " << red5.flow_value() << endl;

  
  DynSetTree<Red::Node*> vs, vt;
  DynList<Red::Arc*> cuts, cutt;

  Min_Cut<Red> () (red1, vs, vt, cuts, cutt);

  cout << "Vs =";
  vs.for_each([] (Red::Node * p) { cout << " " << p->get_info(); });
  cout << endl
       << endl
       << "Vt ";
  vt.for_each([] (Red::Node * p) { cout << " " << p->get_info(); });
  cout << endl
       << endl
       << "cuts =" << endl;
  cuts.for_each([&red1] (Red::Arc * a) 
		{
		  cout << "    " << red1.get_src_node(a)->get_info() << "("
		       << a->flow << "/" << a->cap << ")"
		       << red1.get_tgt_node(a)->get_info() << endl;
		});
  cout << endl
       << "cuts =" << endl;
  cutt.for_each([&red1] (Red::Arc * a) 
		{
		  cout << "    " << red1.get_src_node(a)->get_info() << "("
		       << a->flow << "/" << a->cap << ")"
		       << red1.get_tgt_node(a)->get_info() << endl;
		});
  cout << endl;


  // ofstream out("red.mp", ios::out);
  // generate_cross_graph<Red, Nodo_String, Arco_Normal> (red2, 5, 100, 100, out);


  //  fifo_preflow_maximum_flow(red);
}
