
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
# include <xml_graph.H>

struct Node_Info
{
  size_t level;
  std::string name;
};

typedef Graph_Node <Node_Info> Node;
typedef Graph_Arc <double> Arc;
typedef List_Graph <Node, Arc> Graph;

struct Node_Reader
{
  void operator () (Graph &, Graph::Node * p, DynArray <Attr> & attrs)
  {
    Node_Info & info = p->get_info();
    info.level = std::atol(attrs.access(0).value.c_str());
    info.name = attrs.access(1).value;
  }
};

struct Arc_Reader
{
  void operator () (Graph &, Graph::Arc * a, DynArray <Attr> & attrs)
  {
    a->get_info() = std::atof(attrs.access(0).value.c_str());
  }
};

struct Node_Writer
{
  void operator () (Graph &, Graph::Node * p, DynArray <Attr> & attrs)
  {
    Attr & level = attrs.append();
    level.name = "level";
    std::string value = gnu::autosprintf("%ld", p->get_info().level); 
    level.value = value;
    Attr & name = attrs.append();
    name.name = "name";
    name.value = p->get_info().name;
  }
};

struct Arc_Writer
{
  void operator () (Graph &, Graph::Arc * a, DynArray <Attr> & attrs)
  {
    Attr & quantity = attrs.append();
    quantity.name = "quantity";
    std::string value = gnu::autosprintf("%g", a->get_info());
    quantity.value = value;
  }
};

void generate_dot(Graph & g)
{
  DynMapTreap <Graph::Node *, size_t> map;
  ofstream file("graph.dot");

  file << "digraph\n"
       << "{\n"
       << "  rankdir = LR;\n\n";

  size_t i = 0;

  for (Graph::Node_Iterator it(g); it.has_current(); it.next(), ++i)
    {
      Graph::Node * p = it.get_current();
      Node_Info & info = p->get_info();

      std::string color = info.level == 0 ? "green" : "yellow";

      file << "  " << i << "[label = \"" << info.name << "\""
           << "style = \"filled\" color = \"" << color << "\"];\n";
      map.insert(p, i);
    }

  for (Graph::Arc_Iterator it(g); it.has_current(); it.next())
    {
      Graph::Arc * a = it.get_current();
      const size_t & s = map.find(g.get_src_node(a));
      const size_t & t = map.find(g.get_tgt_node(a));

      file << "  " << s << "->" << t
           << "[label = \"" << a->get_info() << "\"];\n";
    }

  file << "}";

  file.close();
}

int main(int argc, char const * argv[])
{
  std::string input_file_name = argc > 1 ? argv[1] : "test.xml";
  std::string output_file_name = argc > 2 ? argv[2] : "output_test.xml";

  Xml_Graph<Graph, Node_Reader, Arc_Reader, Node_Writer, Arc_Writer> xml_graph;

  Graph g = xml_graph(input_file_name);
  generate_dot(g);
  xml_graph(g, output_file_name);

  return 0;
}
