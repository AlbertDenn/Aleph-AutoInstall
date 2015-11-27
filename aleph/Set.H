
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

/*
	Aleph implementation of set C++ standard container
*/

# ifndef AH_SET_H
# define AH_SET_H

# include <ah_stdc++_utils.H>
# include <tpl_treapRk.H>
# include <tpl_nodePool.H>

namespace Aleph
{

    template <typename T, 
	      class Compare = Aleph::less<T>,
	      template <class, class> class Tree = Treap_Rk>
class set
{
private:

  Tree<T, Compare> tree;

  typedef typename Tree<T, Compare>::Node Node;

  Node_Pool<Node> node_pool;

public:

  typedef T value_type;

  typedef typename set::value_type * pointer;

  typedef typename set::value_type & reference;

  typedef const typename set::value_type & const_reference;

  typedef size_t size_type;

  typedef T key_type;

  class iterator
  {
  private:

    friend class set<T, Compare, Tree>;
    
    typedef typename Tree<T, Compare>::Iterator Iterator;
    
    Tree<T, Compare> * tree;
    Iterator  itor;
    bool underflow;
    bool overflow;

    iterator (Tree<T, Compare> & _tree, Node * node) 
      : tree (&_tree), itor (_tree, node), underflow (false), overflow (false)
    {
      /* empty */
    }

    void init_flags ()
    {
      if (tree->size () == 0)
	underflow = overflow = true;
      else
	underflow = overflow = false;
    }

    iterator (Tree<T, Compare> & _tree) : tree (&_tree), itor (_tree)
    {
      init_flags ();
    }

    void goto_begin ()
    {
      itor.reset_first ();
      init_flags ();
    }

    void goto_last ()
    {
      itor.reset_last ();
      init_flags ();
    }

    void goto_end ()
    {
      itor.reset_last ();
      init_flags ();
      if (not overflow) 
	itor.next ();
      overflow = true;
    }

    void forward ()
    {
      if (underflow)
	{
	  goto_begin ();
	  return;
	}
      
      itor.next ();

      if (not itor.has_current() )
	overflow = true;
    }

    void backward ()
    {
      if (overflow)
	{
	  goto_last ();
	  return;
	}
      
      itor.prev ();

      if (not itor.has_current() )
	underflow = true;
    }

  public:

    typedef typename set<T>::value_type   value_type;

    typedef typename set<T>::size_type       difference_type;

    typedef typename set<T>::value_type *    pointer;

    typedef typename set<T>::reference       reference;

    typedef const typename set<T>::reference const_reference;

    iterator () : tree (nullptr), underflow (true), overflow (true) 
    {
      /* empty */ 
    }
    
    const T & operator * ()
    {
      return KEY (itor.get_current () );
    }

    const T * operator -> ()
    {
      return & KEY (itor.get_current () );
    }

    iterator operator ++ ()
    {
      forward ();
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator return_value = *this;
      forward ();
      return return_value;
    }

    iterator operator -- ()
    {
      backward ();
      return *this;
    }

    iterator operator -- (int)
    {
      iterator return_value = *this;
      backward ();
      return return_value;
    }
    
    iterator operator += (const size_type & n)
    {
      itor.reset_to_pos(itor.get_current_position () + n);
      return *this;
    } 

    iterator operator -= (const size_type & n)
    {
      itor.reset_to_pos(itor.get_current_position () - n);
      return *this;
    } 

    bool operator == (const iterator & _itor) const
    {
      return itor == _itor.itor;
    }

    bool operator != (const iterator & _itor) const
    {
      return not (itor == _itor.itor);
    }

    bool verify (const set & _set) const
    {
      return itor.verify ( (Tree<T, Compare>*) &_set.tree);
    }

    bool verify (const iterator & it) const
    {
      return itor.verify (it.itor);
    }
  };

  set () : node_pool(100) { /* empty */  }

  set (const set & c) : set()
  {
    void * ptr = c.tree.getRoot();
    tree.getRoot () = copyRec ((Node*) ptr);
  }

  ~set () 
  { 
    destroyRec(tree.getRoot () );
  }  

  size_type size () { return COUNT (tree.getRoot () ); }

  bool empty () 
  {
    return COUNT (tree.getRoot () ) == 0;
  }

  bool operator == (const set & c) const
  {
    if (this == &c)
      return true;

    if (size () != c.size () )
      return false;

    typename Tree<T, Compare>::Iterator it1 (tree), it2 (c.tree);

    while (it1.has_current () and it2.has_current () )
      {
	if (Aleph::no_equals<T, Compare> (KEY(it1.get_curr()), 
					  KEY(it2.get_curr())))
	  return false;

	it1.next ();
	it2.next ();
      }
    
    return true;
  }

  bool operator != (const set & c) const
  {
    return not (*this == c);
  }

  bool operator < (const set & c) const
  {
    if (this == &c)
      return false;

    typename Tree<T, Compare>::Iterator itor1 (tree), itor2 (c.tree);

    while (itor1.has_current () and itor2.has_current () )
      {
	if (Compare () (KEY(itor1.get_curr()), KEY(itor2.get_curr())))
	  return true;
	else if (Compare () (KEY(itor2.get_curr()), KEY(itor1.get_curr())))
	  return false; 

	itor1.next ();
	itor2.next ();
      }
    
    if (itor1.has_current () ) /* |*this| >= |c| */
      return false;

    return itor2.has_current ();
  }

  bool operator > (const set & c) const
  {
    return not (*this == c or *this < c);
  }

  bool operator <= (const set & c) const
  {
    return not (*this > c );
  }

  bool operator >= (const set & c) const
  {
    return not (*this < c);
  }
  
  size_type count (const T & value) 
  {
    if (tree.search (value) == nullptr)
      return 0;

    return 1;
  } 

  iterator find(const T & value) 
  { 
    Node * node = tree.search (value);

    if (node == nullptr)
      return end();

    iterator itor (tree);
    itor.itor.reset_to_node (node);

    return itor;
  }

  iterator lower_bound(const T & value)
  {
    if (size() == 0)
      return end ();

    Node * p = search_rank_parent(tree.getRoot(), value);

    return iterator (tree, p);
  }

  iterator upper_bound(const T & value)
  {
    if (size () == 0)
      return end ();

    Node * p = search_rank_parent(tree.getRoot(), value);

    iterator upper(tree, p);
    
    if (KEY(p) == value)
      upper.itor.next();

    return upper;
  }

  set & operator = (set & c)
  {
    if (this == &c)
      return *this;

    destroyRec (tree.getRoot () );

    tree.getRoot () = copyRec (c.tree.getRoot () ); 

    return *this;
  }

  void swap (set & c)
  {
    std::swap (tree.getRoot (), c.tree.getRoot () );
  }

  iterator begin () 
  {
    return iterator (tree);
  }

  iterator end ()
  {
    iterator last (tree);
    last.goto_end ();

    return last;
  }

  std::pair<iterator, bool> insert (const T & value)
  {
    Node * p = node_pool.allocate(value);

    iterator itor (tree);

    if (tree.search_or_insert(p) != p) // se inserta o ya está en el conjunto?
      {    // value ya está dentro del conjunto
	node_pool.deallocate(p);
	itor.itor.reset_to_key (value);
	
	return std::pair<iterator, bool> (itor, false);
      }

    itor.itor.reset_to_node(p);

    return std::pair<iterator, bool> (itor, true);
  }


      template <typename Itor> 
  set (Itor beg, const Itor & end) : set()
  {
    Aleph::verify_iterators (beg, end);

    while (beg != end)
      insert(*beg++) ;
  }
    
  iterator insert (const iterator & /* pos */, const T & value)
  {
    Node * p = node_pool.allocate(value);

    iterator _itor(tree);

    if (tree.search_or_insert(p) != p)
      { // clave duplicada
	node_pool.deallocate(p);
	_itor.itor.reset_to_key(value);
      }
    else
      _itor.itor.reset_to_node(p);   
  
    return _itor;    
  }

      template <typename Itor>
  void insert (Itor beg, const Itor & end)
  {
    Aleph::verify_iterators (beg, end);

    while (beg != end)
      insert(*beg++);
  }

  size_type erase (const T & value)
  {
    Node * p = tree.remove (value);

    if (p == nullptr)
      return 0;
	
    node_pool.deallocate(p);

    return 1;
  }

  void erase (iterator pos)
  {
    Aleph::verify_container_and_iterator (*this, pos);

    node_pool.deallocate(pos.itor.del());
  }
  
  iterator erase (const iterator & beg, const iterator & end)
  {
    Aleph::verify_container_and_iterator (*this, beg);
    Aleph::verify_iterators (beg, end);

    iterator ret_val = end;

    const size_t pos_beg = beg.itor.get_current_position ();
    const size_t pos_end = end.itor.get_current_position ();

    Node * removed_tree = tree.remove(pos_beg, pos_end - 1);

    destroyRec(removed_tree);

    return ret_val;
  }

  void clear ()
  {
    destroyRec(tree.getRoot());
  }
};


    template <typename T>
typename iterator_traits<typename set<T>::iterator>::difference_type
distance(typename set<T>::iterator it1, typename set<T>::iterator it2)
{
  Aleph::verify_iterators(it1, it2);

  return it2.itor.get_current_position() - it1.itor.get_current_position();
}

}

# endif // AH_SET_H
