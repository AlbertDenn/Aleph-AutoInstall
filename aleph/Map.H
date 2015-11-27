
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


# ifndef ALEPH_MAP_H
# define ALEPH_MAP_H
/*
	   Aleph implementation of map C++ standard container
*/

# include <stdlib.h>
# include <ahFunction.H>
# include <ah_stdc++_utils.H>
# include <tpl_treapRk.H>
# include <tpl_nodePool.H>


namespace Aleph
{


    template <class Key, 
	      class Elem, 
	      class Compare = Aleph::less<Key>,
	      template <class, class> class Tree = Treap_Rk
    >
class map
{
  struct Cmp
  {
    bool
    operator () (const std::pair<Key, Elem> & __x, 
		 const std::pair<Key, Elem> & __y) const noexcept
    { 
      return Compare () (__x.first, __y.first); 
    }
  };

public:

  typedef std::pair<Key, Elem> Pair;

  typedef Pair value_type;

  typedef typename map::value_type & reference;

  typedef const typename map::value_type & const_reference;

  typedef size_t size_type;

  typedef Key key_type;

  typedef Elem mapped_type;

private:

  typedef Tree<Pair, Cmp> Tree_Type;

  typedef typename Tree_Type::Node Node;

  Node_Pool<Node> node_pool;

  mutable Tree_Type tree;

  Node * search_in_tree (const Key & k)
  {
    return tree.search (Pair(k, Elem()));
  }

public:

  class iterator
  {
  private:

    friend class map<Key, Elem>;
    
    typedef typename Tree_Type::Iterator Iterator;
    
    Iterator itor;
    bool underflow;
    bool overflow;

    void init_flags ()
    {
      if (itor.has_current () )
	underflow = overflow = false;
      else
	underflow = overflow = true;
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

      if (not itor.has_current () )
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

      if (not itor.has_current () )
	underflow = true;
    }

    iterator (Tree_Type & _tree, Node * node) 
      : itor (_tree, node), underflow (false), overflow (false)
    {
      /* empty */
    }

  public:

    typedef typename map::value_type   value_type;

    typedef typename map::size_type    difference_type;

    typedef typename map::value_type * pointer;

    typedef typename map::reference    reference;

    iterator () { /* empty */ }

    iterator (Tree_Type & tree) : itor (tree)
    {
      init_flags ();
    }

    const Pair & operator * ()
    {
      return KEY(itor.get_current());
    }

    const Pair * operator -> ()
    {
      return &KEY(itor.get_current());
    }

    iterator operator ++ ()
    {
      forward ();
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator ret_val = *this;
      forward ();
      return ret_val;
    }

    iterator operator -- ()
    {
      backward ();
      return *this;
    }

    iterator operator -- (int)
    {
      iterator ret_val = *this;
      backward ();
      return ret_val;
    }
    
    iterator operator += (const size_type & n)
    {
      itor.reset_to_pos (itor.get_current_position () + n);
      return *this;
    } 

    iterator operator -= (const size_type & n)
    {
      itor.reset_to_pos (itor.get_current_position () - n);
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

    bool verify (const map & _map) const
    {
      return itor.verify ( (Tree_Type*) &_map.tree); 
    }

    bool verify (const iterator & it) const
    {
      return itor.verify (it.itor);
    }
  };

  map () : node_pool(100) { /* empty */ }
 
  map (const map & c) : node_pool(100)
  {
    tree.getRoot () = copyRec (c.tree.getRoot () );
  }


  ~map () 
  { 
    destroyRec (tree.getRoot () );
  }  

  map & operator = (const map& c)
  {
    if (this == &c)
      return *this;

    destroyRec (tree.getRoot ());

    tree.getRoot () = copyRec (c.tree.getRoot ()); 

    return *this;
  }

  size_t size () const { return COUNT (tree.getRoot () ); }

  bool empty () const 
  {
    return size() == 0;
  }

  bool operator == (const map & c) const
  {
    if (size () != c.size () )
      return false;

    typename Tree_Type::Iterator itor1 (tree), itor2 (c.tree);
    
    while (itor1.has_current () and itor2.has_current () )
    {
      if (KEY (itor1.get_current () ) != KEY (itor2.get_current () ) )
	return false;

      itor1.next ();
      itor2.next ();
    }
    
    assert(not itor1.has_current () and not itor2.has_current () );

    return true;
  }

  bool operator != (const map & c) const
  {
    return not (*this == c);
  }

  bool operator < (const map & c) const
  {
    typename Tree_Type::Iterator itor1 (tree), itor2 (c.tree);

    while (itor1.has_current () and itor2.has_current () )
    {
      if (KEY (itor1.get_current () ) < KEY (itor2.get_current () ))
	return true;
      else if (KEY (itor2.get_current () ) < KEY (itor1.get_current () ))
	return false;
      
      // En este caso las claves sons iguales
      itor1.next ();
      itor2.next ();
    }
    
    if (not itor1.has_current ())
      return true;

    return false;
  }

  bool operator > (const map & c)
  {
    return not (*this == c or *this < c);
  }

  bool operator <= (const map & c)
  {
    return not (*this > c );
  }

  bool operator >= (const map & c)
  {
    return not (*this < c);
  }

  size_type count (const Key & key)
  {
    if (search_in_tree (key) != nullptr)
      return 1;

    return 0;
  } 

  iterator find (const Key & key) 
  { 
    Node * p = search_in_tree (key);

    if (p == nullptr)
      return end ();

    return iterator (tree, p);
  }
  iterator lower_bound (const Key & key)
  {
    Node * p = 
      search_rank_parent <Node, Cmp> (tree.getRoot(), Pair(key, Elem()));

    return iterator(tree, p);
  }

  iterator upper_bound (const Key & key)
  {
    Node * p = 
      search_rank_parent <Node, Cmp> (tree.getRoot(), Pair(key, Elem()));

    iterator upper(tree, p);
    
    if (KEY (p).first == key)
      upper.itor.next();

    return upper;
  }

  void swap (map & c)
  {
    tree.swap (c.tree);
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

  std::pair<iterator, bool> insert (const Pair & key)
  {
    Node * p = node_pool.allocate(key);

    if (tree.search_or_insert(p) != p)
    {
      node_pool.deallocate(p);
      return std::pair<iterator, bool> (iterator (tree), false);
    }

    return std::pair<iterator, bool> (iterator(tree, p), true);
  }

      template <typename Itor> 
  map (Itor beg, const Itor & end) : map()
  {
    while (beg != end)
      insert(*beg++);
  }
    
  iterator insert (iterator /* pos */, const Pair& key)
  {
    return insert(key);
  }

      template <typename Itor>
  void insert (Itor beg, const Itor & end)
  {
    while (beg != end)
      insert (*beg++);
  }

  size_type erase (const Key & key)
  {
    Node * p = tree.remove (Pair(key, Elem()));

    if (p == nullptr)
      return 0;
	
    node_pool.deallocate(p);

    return 1;
  }

  void erase (iterator pos)
  {
    erase (KEY (pos.itor.get_current ()).first);
  }

  iterator erase (const iterator & beg, const iterator & end)
  {
    Aleph::verify_iterators (beg, end);
    Aleph::verify_container_and_iterator (*this, beg);

    iterator ret_val = end;

    const size_t pos_beg = beg.itor.get_current_position ();
    const size_t pos_end = end.itor.get_current_position ();

    Node * removed_tree = tree.remove (pos_beg, pos_end - 1);

    destroyRec (removed_tree);

    return ret_val;
  }

  void clear ()
  {
    destroyRec (tree.getRoot());
  }
  
private:
  
  struct Proxy
  {
    map  * map_ptr;
    Node * node;
    Key    key;

    Proxy (map * m_ptr, const Key & _key) : map_ptr (m_ptr), key (_key)
    {
      node = map_ptr->search_in_tree(key);
    }

    Proxy & operator = (const Elem & elem)
    {
      if (node == nullptr)
        {
	  node = map_ptr->node_pool.allocate(std::make_pair(key, elem));
	  map_ptr->tree.insert(node);
	}
      else
        KEY(node).second = elem;

      return *this;
    }

    Proxy & operator = (const Proxy & proxy)
    {
      if (this == &proxy)
	return *this;

      if (proxy.node == nullptr)
        throw std::domain_error("key not found");;

      if (map_ptr == proxy.map_ptr and key == proxy.key)
        return *this; // They are the same

      if (node == nullptr)
        {
	  node = map_ptr->node_pool.allocate(KEY(proxy.node));
	  map_ptr->tree.insert(node);
	} 
      else
        KEY(node).second = KEY(proxy.node).second;

      return *this;
    }

    operator Elem & () const
    {
      if (node == nullptr)
        throw std::domain_error ("key not found");;

      return KEY (node).second;
    }    
  };

public:

  const Proxy operator [] (const Key & key) const 
    Exception_Prototypes (std::domain_error)
  {
    return Proxy (this, key);
  }

  Proxy operator [] (const Key & key) 
    Exception_Prototypes (std::domain_error)
  {
    return Proxy (this, key);
  }
};



} // end namespace Aleph


# endif // ALEPH_MAP_H
