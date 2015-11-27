
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

# include <tpl_agraph.H>
# include <random_graph.H>
# include <generate_graph.H>
# include <graph-traverse.H>

# include <tpl_dynSetTree.H>

unsigned long node_count = 0;
unsigned long arc_count = 0;

using Net = Array_Graph<>;

struct Init_Node
{
  void operator () (Net &, Net::Node * p) const
  {
    p->get_info() = node_count++;
  }
};

struct Init_Arc
{
  void operator () (Net &, Net::Arc * a) const
  {
    a->get_info() = arc_count++;
  }
};

Net create_graph(size_t n, double prob, unsigned long seed)
{
  return Random_Graph<Net, Init_Node, Init_Arc>(seed)(n, prob);
}

void usage()
{
  cout << "test n prob seed" << endl;
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
    usage();

  size_t n = atoi(argv[1]);
  double prob = atof(argv[2]);
  unsigned long seed = atoi(argv[3]);

  Net g = create_graph(n, prob, seed);

  {
    DynMapTree<unsigned long, Net::Node*> table;

    size_t n = (Graph_Traverse_DFS<Net, Node_Arc_Iterator<Net>>(g))
     		(g.get_first_node(), [&table] (Net::Node * p)
      {
     	table.insert(p->get_info(), p);
	return true;
      });

    // size_t n = (Graph_Traverse<Net, Node_Arc_Iterator<Net>>(g))
    // 		(g.get_first_node(), [&table] (Net::Node * p)
    //   {
    // 	table.insert(p->get_info(), p);
    // 	return true;
    //   });

		assert(g.vsize() == n);

     using Pair = std::pair<unsigned long, Net::Node*>;

     cout << "List = " << endl;
   
     table.for_each([] (const Pair & p)
		    {
		      cout << p.first << " " << p.second->get_info() << endl;
		    });
  }
  {
    DynMapTree<unsigned long, Net::Node*> ntable;
    DynMapTree<unsigned long, Net::Arc*> atable;
    size_t n = Graph_Traverse_DFS<Net, Node_Arc_Iterator<Net>>(g).
      exec(g.get_first_node(), [&ntable, &atable]
	   (Net::Node * p, Net::Arc * a)
      {
	if (a)
	  atable.insert(a->get_info(), a);
	else
	  cout << "Arco NULO" << endl;
	ntable.insert(p->get_info(), p);
	return true;
      });
    using Pair = std::pair<unsigned long, Net::Arc*>;
    atable.for_each([] (const Pair &p)
      {
	cout << p.first << " " << p.second->get_info() << endl;
      });
    cout << "of " << g.esize() << endl;
  }

      {
	DynMapTree<unsigned long, Net::Node*> table;

	size_t n = (Graph_Traverse<Net, Out_Iterator<Net>>(g))
		    (g.get_first_node(), [&table] (Net::Node * p)
	  {
	    table.insert(p->get_info(), p);
	    return true;
	  });

        using Pair = std::pair<unsigned long, Net::Node*>;

        cout << "List = " << endl;
        table.for_each([] (const Pair & p)
	  {
	    cout << p.first << " " << p.second->get_info() << endl;
	  });

		cout << "FIN" << endl 
		<< n << " nodes seen" << endl;
  } 

  {
    DynMapTree<unsigned long, Net::Node*> node_table;
    DynMapTree<unsigned long, Net::Arc*> arc_table;

    auto n = (Graph_Traverse<Net, Node_Arc_Iterator<Net>>(g))
     (g.get_first_node(), [&node_table] (Net::Node * p)
      {
	node_table.insert(p->get_info(), p);
	return true;
      }, [&arc_table] (Net::Arc * a)
      {
	arc_table.insert(a->get_info(), a);
	return true;
      });
	   assert(g.vsize() == get<0>(n));
	   assert(g.esize() == get<1>(n));

	      using P = pair<unsigned long, Net::Arc*>;
    arc_table.for_each([] (const P & p)
      {
	cout << p.first<< " " << p.second->get_info() << endl;
      });
  }   
}
