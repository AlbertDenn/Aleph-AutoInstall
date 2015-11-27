
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
# include <tpl_maxflow_mincost.H>


struct Enunciado
{
  static long counter;
  string      enunciado;
  int         num;
  char        tipo;

  Enunciado(const string & str, char t = 'x') 
    : enunciado(str), num(counter++), tipo(t) { }

  Enunciado() : tipo('x') { }

  bool operator == (const Enunciado & enun) const
  {
    return enun.enunciado == enunciado;
  }
};


long Enunciado::counter = 0;


struct Plan
{
  string enunciado_necesidad;
  float  duracion;

  Plan() : enunciado_necesidad(""), duracion(0) { /* empty */ }

  Plan(const string & str, const float & dur) 
    : enunciado_necesidad(str), duracion(dur) { /* empty */ }
};

DynMapTree<Treap, string, DynDlist<Plan> > planificacion;


typedef Net_Cost_Arc<Empty_Class, float> Tubo;

typedef Net_Max_Flow_Min_Cost<Net_Node<Enunciado, float>, Tubo> Red;

struct Enun_Cmp
{
  bool operator () (Enunciado * e1, Enunciado * e2) const
  {
    return e1->enunciado < e2->enunciado;
  }
};

void crear_par(Red & r, const string & src_name, const string & tgt_name, 
	       const Red::Flow_Type & cap, const Red::Flow_Type & cost = 0)
{
  Red::Node * src = r.search_node(Enunciado(src_name));

  if (src == NULL)
    src = r.insert_node(Enunciado(src_name, 'c'));

  Red::Node * tgt = r.search_node(Enunciado(tgt_name));

  if (tgt == NULL)
    tgt = r.insert_node(Enunciado(tgt_name, 'n'));

  r.insert_arc(src, tgt, cap, cost);
}


void crear_red(Red & r)
{
  crear_par(r, "$C_5$", "$N_1$", 1, 42);
  crear_par(r, "$C_5$", "$N_2$", 1, 12);

  crear_par(r, "$C_1$", "$N_1$", 1, 38);
  crear_par(r, "$C_1$", "$N_2$", 1, 17.6);

  crear_par(r, "$C_2$", "$N_1$", 1, 33.6);
  crear_par(r, "$C_2$", "$N_2$", 1, 18);

  crear_par(r, "$C_0$", "$N_1$", 1, 12.8);
  crear_par(r, "$C_0$", "$N_2$", 1, 37.8);
  crear_par(r, "$C_0$", "$N_4$", 1, 9.6);

  crear_par(r, "$C_{10}$", "$N_1$", 1, 0.6);
  crear_par(r, "$C_{10}$", "$N_2$", 1, 0.4);
  crear_par(r, "$C_{10}$", "$N_3$", 1, 1.2);

  crear_par(r, "$C_4$", "$N_2$", 1, 9);
  crear_par(r, "$C_4$", "$N_0$", 1, 24);

  crear_par(r, "$C_3$", "$N_3$", 1, 15);
  crear_par(r, "$C_3$", "$N_0$", 1, 24);

  crear_par(r, "$C_7$", "$N_4$", 1, 60);
  crear_par(r, "$C_7$", "$N_5$", 1, 40);

  crear_par(r, "$C_6$", "$N_4$", 1, 2);
  crear_par(r, "$C_6$", "$N_5$", 1, 1.2);

  crear_par(r, "$C_8$", "$N_3$", 1, 2);
  crear_par(r, "$C_8$", "$N_5$", 1, 7.2);

  crear_par(r, "$C_9$", "$N_3$", 1, 4);
  crear_par(r, "$C_9$", "$N_0$", 1, 5);

  Red::Node * s = r.insert_node(Enunciado("S"));

  r.insert_arc(s, r.search_node(Enunciado("$C_0$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_1$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_2$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_3$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_4$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_5$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_6$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_7$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_8$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_9$")), 1, 0);
  r.insert_arc(s, r.search_node(Enunciado("$C_{10}$")), 1, 0);

  const float cap = s->num_arcs;

  Red::Node * t = r.insert_node(Enunciado("T"));

  r.insert_arc(r.search_node(Enunciado("$N_0$")), t, cap, 0);
  r.insert_arc(r.search_node(Enunciado("$N_1$")), t, cap, 0);
  r.insert_arc(r.search_node(Enunciado("$N_2$")), t, cap, 0);
  r.insert_arc(r.search_node(Enunciado("$N_3$")), t, cap, 0);
  r.insert_arc(r.search_node(Enunciado("$N_4$")), t, cap, 0);
  r.insert_arc(r.search_node(Enunciado("$N_5$")), t, cap, 0);
}


int eliminar_arcos_llenos(Red & r)
{
  cout << "--src = " << r.get_src_nodes().size() << endl
       << "--tgt = " << r.get_sink_nodes().size() << endl;


  DynDlist<Red::Arc *> arcs;
  for (Arc_Iterator<Red> it(r); it.has_current(); it.next())
    {
      Red::Arc * a = it.get_current();
      if (a->cost == 0)
	{
	  r.set_flow(a, 0);
	  continue;
	}

      if (a->flow == 0)
	continue;

      Red::Node * cap = r.get_tgt_node(a);
      Red::Node * nes = r.get_tgt_node(a);

      DynDlist<Plan>* l = planificacion.test(cap->get_info().enunciado);
      if (l == NULL)
	l = planificacion.insert(cap->get_info().enunciado, DynDlist<Plan>());

      l->append(Plan(nes->get_info().enunciado, a->cost));
      arcs.append(a);
    }

  cout << "--src = " << r.get_src_nodes().size() << endl
       << "--tgt = " << r.get_sink_nodes().size() << endl;

  const int ret_val = arcs.size();

  for (DynDlist<Red::Arc *>::Iterator it(arcs); it.has_current(); it.next())
    r.remove_arc(it.get_current());

  DynDlist<Red::Node *> nodes;
  for (Node_Iterator<Red> it(r); it.has_current(); it.next())
    {
       Red::Node * p = it.get_current();

       if (p->get_info().tipo == 'x')
       	 continue;

       if (r.get_in_degree(p) == 0 or r.get_out_degree(p) == 0)
	 nodes.append(p);
    }

  for (DynDlist<Red::Node *>::Iterator it(nodes); it.has_current(); it.next())
    {
      r.remove_node(it.get_current());
      cout << "src = " << r.get_src_nodes().size() << endl
	   << "tgt = " << r.get_sink_nodes().size() << endl;
    }


  cout << "--src = " << r.get_src_nodes().size() << endl
       << "--tgt = " << r.get_sink_nodes().size() << endl << endl;

  return ret_val;
}




struct Esc_Nodo
{
  string operator () (Red::Node * p) const 
  {
    return gnu::autosprintf("%s", p->get_info().enunciado.c_str());
  }
};


struct Esc_Arco
{
  string operator () (Red::Arc * a) const 
  {
    return gnu::autosprintf("%0.0f/%0.1f", a->cap, a->cost);
  }
};


struct Esc_Arco_F
{
  string operator () (Red::Arc * a) const 
  {
    return gnu::autosprintf("%0.0f/%0.2f/%0.1f", a->cap, a->flow, a->cost);
  }
};


struct Sombra
{
  bool operator () (Red::Node *) const 
  {
    return false;
  }

  bool operator () (Red::Arc * a) const 
  {
    return a->flow == 1.0 and a->cost != 0;
  }
};


void planificar(Red & r)
{
  {
     ofstream out("cxn-100.dot", ios::out);
     Generate_Graphviz <Red, Esc_Nodo, Esc_Arco, Dummy_Attr<Red>, 
      Dummy_Attr<Red>, Dummy_Attr<Red>, Dummy_Attr<Red> > 
      () (r, out, "LR");
  }

  for (int i = 0; true; i++)
    {
      max_flow_min_cost_by_cycle_canceling
	<Red, Ford_Fulkerson_Maximum_Flow> (r);

      {
	string s = gnu::autosprintf("cxn-100-%d.dot", i);
	ofstream out(s.c_str(), ios::out);
	Generate_Graphviz <Red, Esc_Nodo, Esc_Arco_F, Sombra,
	  Sombra, Dummy_Attr<Red>, Dummy_Attr<Red> > 
	  () (r, out, "LR");
      }

      if (eliminar_arcos_llenos(r) == 0)
	break;
    }
}



int main()
{
  Red r;

  crear_red(r);

  planificar(r);
}
