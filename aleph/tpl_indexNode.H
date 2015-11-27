
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

# ifndef TPL_INDEXNODE_H
# define TPL_INDEXNODE_H

# include <tpl_dynSetTree.H>
# include <tpl_graph.H>

using namespace Aleph;

namespace Aleph
{

  template <class GT>
struct Dft_Node_Cmp
{
      bool
  operator () (typename GT::Node * p1, typename GT::Node * p2) const
  {
    return p1->get_info() < p2->get_info();
  }
};

  template
  <class GT, 
   class Compare = Dft_Node_Cmp<GT>,
   template <class /* Key */, class /* Compare */> class Tree = Treap,
   class SN = Dft_Show_Node<GT> >  
class IndexNode
{
private:

  typedef typename GT::Node GT_Node;
  typedef typename GT::Node_Type GT_Node_Type;
	
  DynSetTree<GT_Node*, Tree, Compare> index;

  GT & g;
  SN & sn;
		
public:

  GT_Node * insert(GT_Node * p)
  {
    index.put(p);
    return p;
  }

  GT_Node * insert_in_graph(const GT_Node_Type & info)
  {
    GT_Node * ret_val = g.insert_node(info);

    try
      {
	insert(ret_val);

	return ret_val;
      }
    catch (...)
      {
	g.remove_node(ret_val);
	throw;
      }
  }

  GT_Node * insert_in_graph(GT_Node_Type && info = GT_Node_Type())
  {
    GT_Node * ret_val = g.insert_node(std::forward<GT_Node_Type>(info));

    try
      {
	insert(ret_val);

	return ret_val;
      }
    catch (...)
      {
	g.remove_node(ret_val);
	throw;
      }
  }

  GT_Node * search(GT_Node * p)
  {
    GT_Node ** ret_val = index.search(p);

    if (ret_val == nullptr)
      return nullptr;

    return *ret_val;
  }

  GT_Node * search(const GT_Node_Type & info)
  {
    GT_Node dummy_node(info);
    GT_Node * dummy_node_ptr = &dummy_node;

    return search(dummy_node_ptr);
  }

  void remove(GT_Node * p)
  {
    index.remove(p);
  }

  void remove_from_graph(GT_Node * p) throw(std::exception, std::domain_error)
  {
    index.find(p); // dispara excepción si p no está en el índice
    index.remove(p);
    g.remove_node(p);
  }

  void clear_index()
  {
    index.empty();
  }

  void build_index()
  {
    for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
      {
	GT_Node * p = it.get_curr();

	if (search(p) != p)
	  insert(p);
      }
  }

  void clear_graph()
  {
    clear_index();
    g.clear_graph();
  }

private: 

  void init()
  {
    for (Node_Iterator<GT, SN> it(g, sn); it.has_curr(); it.next())
      insert(it.get_curr());
  }

public:

  IndexNode(GT & __g, SN && __sn = SN()) : g(__g), sn(__sn)
  {
    init();
  }

  IndexNode(GT & __g, SN & __sn) : g(__g), sn(__sn) 
  {
    init();
  }

  size_t size() const { return index.size(); }
};

}

# endif

