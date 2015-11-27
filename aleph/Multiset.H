
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
       Aleph implementation of multiset C++ standard container
*/

# ifndef AH_MULTISET_H
# define AH_MULTISET_H

# include <memory>
# include <ah_stdc++_utils.H>
# include <tpl_dnode.H>
# include <tpl_treapRk.H>
# include <tpl_nodePool.H>

namespace Aleph
{

    template <class T, 
	      class Compare = Aleph::less<T>,
	      template <class, class> class Tree = Treap_Rk
	      >
class multiset
{
  struct Node_Data
  {
    mutable T key;
    mutable size_t num_reps;

    Node_Data() : num_reps(0) { /* empty */ }

    Node_Data(const T & k) : key(k), num_reps(1)  { /* empty */ }
  };

  struct Cmp_Data
  {
    bool operator () (const Node_Data & op1, const Node_Data & op2) const
    {
      return Compare () (op1.key, op2.key);
    }
  };

public:

  typedef T value_type;

  typedef typename multiset::value_type & reference;

  typedef const typename multiset::value_type & const_reference;

  typedef size_t size_type;

  typedef T key_type;

private:

  typedef Tree<Node_Data, Cmp_Data> Tree_Type;

  typedef typename Tree_Type::Node Node;

  typedef typename Tree_Type::Iterator Tree_Iterator;

  mutable Tree_Type tree;

  mutable size_t num_elem;

  Node_Pool<Node> pool;

  static T & get_key(Node * p) { return KEY(p).key; }

  static size_t & get_num_reps(Node * p) { return KEY(p).num_reps; }

public:  

  class iterator
  {
  private:

    friend class multiset<T, Compare, Tree>;
    
    typedef typename Tree_Type::Iterator Iterator;
    
    mutable multiset * multiset_ptr;

    mutable Iterator tree_it;

    mutable int pos_in_node;

    bool overflow;
    bool underflow;

    iterator (multiset * mset_ptr, Node * curr_tree_node, int pos = 0)
      : multiset_ptr(mset_ptr), tree_it(mset_ptr->tree, curr_tree_node), 
	pos_in_node(pos), overflow (false), underflow (false)
    {
      // empty
    }

    void default_init ()
    {
      if (tree_it.has_curr ())
	{
	  underflow = overflow = false;
	  pos_in_node = 0;
	}
      else
	underflow = overflow = true;
    }

    iterator(const multiset & ms)
      : multiset_ptr(const_cast<multiset*>(&ms)), tree_it(ms.tree)
    {
      default_init();
    }

    Node * get_curr_node() { return tree_it.get_curr(); }

    bool has_curr() const 
    {
      return tree_it.has_curr();
    }

    Node_Data & get_data() { return KEY(get_curr_node()); }

    T & get_key() { return multiset::get_key(get_curr_node()); }

    size_t & get_num_reps() { return multiset::get_num_reps(get_curr_node()); }

  public:

    typedef typename multiset<T>::value_type      value_type;

    typedef typename multiset<T>::size_type       difference_type;

    typedef typename multiset<T>::value_type *    pointer;

    typedef typename multiset<T>::reference       reference;

    typedef const typename multiset<T>::reference const_reference;

    iterator(multiset * mset_ptr)
      : multiset_ptr(mset_ptr), tree_it(multiset_ptr->tree) 
    {
      default_init();
    }

    iterator(const iterator & it) 
      : multiset_ptr(it.multiset_ptr), tree_it(it.tree_it), 
	pos_in_node(it.pos_in_node), 
	overflow(it.overflow), underflow(it.underflow)
    {
      // empty
    }

    iterator () 
    {
      underflow = overflow = true;
      pos_in_node = -1;
    }

    const T & operator * ()
    {
      return get_key();
    }

    const T * operator -> ()
    {
      return & get_key();
    }

  private:

    void goto_begin ()
    {
      tree_it.reset_first ();
      if (tree_it.has_curr ())
	{
	  underflow = false;
	  pos_in_node = 0;
	}
      else
	{
	  underflow = true;
	  pos_in_node = -1;
	}
    }

    void goto_last ()
    {
      tree_it.reset_last ();
      if (tree_it.has_curr ())
	{
	  overflow = false;
	  pos_in_node = get_num_reps() - 1;
	}
      else
	{
	  overflow = true;
	  pos_in_node = -1;
	}
    }

    void goto_end ()
    {
      tree_it.reset_last();
      if (tree_it.has_curr()) 
	{     // el árbol no está vacío 
	  tree_it.next (); // coloca tree_it fuera de rango
	  underflow = false;
	}
      else
	underflow = true;

      pos_in_node = -1;
      overflow = true;
    }

    iterator compute_end() const
    {
      iterator it(*this);
      it.goto_end();
      return it;
    }

    bool is_at_end() const
    {
      return not tree_it.has_curr();
    }

    iterator compute_begin() const
    {
      iterator it(*this);
      return it;
    }

    void forward ()
    {
      if (underflow)
	{
	  goto_begin ();
	  return;
	}

      if (++pos_in_node == get_num_reps())
	{ 
	  tree_it.next (); 

	  if (tree_it.has_curr ())
	    pos_in_node = 0;
	  else
	    {
	      overflow = true;
	      pos_in_node = -1;
	      assert(*this == compute_end());
	    }
	}
    }

    void backward ()
    {
      if (overflow)
	{
	  goto_last ();
	  return;
	}

      if (pos_in_node-- == 0)
	{
	  tree_it.prev ();

	  if (tree_it.has_curr ())
	    pos_in_node = get_num_reps();
	  else
	    underflow = true;
	}
    }

    void del()
    {
      Node * tree_node = get_curr_node();
      size_t & num_reps = multiset::get_num_reps(tree_node);

      --num_reps;

      --multiset_ptr->num_elem;

      if (num_reps == 0)
	{
	  tree_it.del();
	  multiset_ptr->pool.deallocate(tree_node);

	  if (tree_it.has_curr ())
	    pos_in_node = 0;
	  else
	    {
	      overflow = true;
	      pos_in_node = -1;
	    }
	}
    }

  public:

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

    T & operator -- (int)
    {
      iterator ret_val = *this;
      backward ();
      return ret_val;
    }
    
    iterator operator += (size_type n)
    {
      for (int i = 0; i < n; ++i)
        forward ();

      return *this;
    } 

    iterator operator -= (size_type n)
    {
      for (int i = 0; i < n; ++i)
        backward ();
      
      return *this;
    } 

    bool operator == (const iterator & it) const
    {
      if (this->has_curr() and it.has_curr())
	return pos_in_node == it.pos_in_node;

      if (this->is_at_end() and it.is_at_end())
	{
	  assert(this->overflow and it.overflow);
	  return true;
	}
      
      return false;
    }

    bool operator != (const iterator & itor) const
    {
      return not (*this == itor);
    }

    bool verify (const multiset & _multiset) const
    {
      return tree_it.verify ( (Tree_Type*)& (_multiset.tree));
    }

    bool verify (const iterator & it) const
    {
      return tree_it.verify (it.tree_it);
    }
  }; // class iterator;

  typedef typename multiset::iterator const_iterator;

  typedef typename multiset::iterator reverse_iterator;
  
  typedef typename multiset::iterator const_reverse_iterator;

  multiset () : num_elem (0), pool(100) { /* empty */  }

private:

  void copy (const multiset & c)
  {
    tree.getRoot () = copyRec(c.tree.getRoot());
  }

public:
 
  multiset (const multiset & c) : num_elem (c.num_elem), pool(100)
  {
    copy (c);
  }

      template <typename Itor> 
  multiset (Itor beg, const Itor & end) : multiset()
  {
    while (beg != end)
      insert (*beg++);
  }

  ~multiset () 
  { 
    destroyRec(tree.getRoot());
  }  

  multiset & operator = (const multiset & c)
  {
    if (this == &c)
      return *this;

    destroyRec(tree.getRoot());

    copy(c);

    num_elem = c.num_elem;

    return *this;
  }

  size_type size () const { return num_elem; }

  bool empty () const
  {
    return COUNT(tree.getRoot ()) == 0;
  }

  bool operator == (const multiset & c) const
  {
    if (this == &c)
      return true;

    if (size () != c.size ())
      return false;

    Tree_Iterator itor1 (tree), itor2 (c.tree);

    while (itor1.has_curr() and itor2.has_curr())
      {
	Node * p1 = itor1.get_curr();
	Node * p2 = itor2.get_curr();

	if (no_equals <Node_Data, Cmp_Data> (KEY(p1), KEY(p2)))
	  return false;
	else if (get_num_reps(p1) != get_num_reps(p2))
	  return false;

	itor1.next ();
	itor2.next ();
      }
    
    return not itor1.has_curr() and not itor2.has_curr();
  }

  bool operator != (const multiset & c) const
  {
    return not (*this == c);
  }

  bool operator < (const multiset & c) const
  {
    if (this == &c)
      return false;

    iterator itor1 (*this), itor2 (c);

    while (itor1.has_curr() and itor2.has_curr())
      {
	if (Cmp_Data () (itor1.get_data(), itor2.get_data()))
	  return true;
	else if (Cmp_Data () (itor2.get_data(), itor1.get_data()))
	  return false; // no son iguales
	  
	itor1.forward();
	itor2.forward();
      }
    
    if (itor1.has_curr()) /* |*this| >= |c| */
      return false;

    return itor2.has_curr();
  }

  bool operator > (const multiset & c) const
  {
    return not (*this == c or *this < c);
  }

  bool operator <= (const multiset & c) const
  {
    return not (*this > c );
  }

  bool operator >= (const multiset & c) const
  {
    return not (*this < c);
  }

  size_type count (const T & value) const
  {
    Node * p = tree.search (Node_Data(value));

    if (p == nullptr)
      return 0;

    return get_num_reps(p);
  } 

  iterator find (const T & value) const
  { 
    Node * node = tree.search(Node_Data(value));

    if (node == nullptr)
      return end ();

    return iterator(const_cast<multiset*>(this), node);
  }

  iterator lower_bound (const T & value) const
  {
    if (size () == 0)
      throw std::underflow_error ("multiset is empty");

    Node * tree_node = tree.search(Node_Data(value));

    if (tree_node == nullptr)
      return end ();

    return iterator (this, tree_node);
  }

  iterator upper_bound (const T & value) const
  {
    if (size () == 0)
      throw std::underflow_error("multiset is empty");

    Node * tree_node = tree.search(Node_Data(value));

    if (tree_node == nullptr)
      return end ();

    iterator it (this, tree_node);
    it.list_it.reset_last ();

    return it;
  }

  void swap (multiset & c)
  {
    std::swap (tree.getRoot (), c.tree.getRoot ());
    std::swap (num_elem, c.num_elem);
  }

  iterator begin () const
  {
    return iterator (*this);
  }

  iterator end () const
  {
    return iterator(*this).compute_end();
  }

  iterator insert (const T & value) 
  {
    Node * p = pool.allocate(Node_Data(value));
    Node * ptr = tree.search_or_insert(p);

    if (ptr != p) // ya está value en el árbol
      pool.deallocate(p);

    ++num_elem;

    return iterator(this, ptr, get_num_reps(ptr)++);
  }
    
  iterator insert (iterator pos, const T & value)
  {
    Aleph::verify_container_and_iterator (this, pos);

    if (pos != pos.compute_end())
      {
	Node * p = pos.get_curr_node();

	if (are_equals <Node_Data, Cmp_Data> (KEY(p), Node_Data(value)))
	  {
	    get_num_reps(p)++;
	    pos.pos_in_node++;
	    
	    return pos;
	  }
      }

    return insert(value);
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
    Node * tree_node = tree.remove(Node_Data(value));

    if (tree_node == nullptr)
      return 0;

    const size_t ret_val = get_num_reps(tree_node);
	
    pool.deallocate(tree_node);

    num_elem -= ret_val;

    return ret_val;
  }

  void erase (iterator pos)
  {
    Aleph::verify_container_and_iterator (*this, pos);

    Node * tree_node = pos.get_curr_node();

    size_t & num_reps = get_num_reps(tree_node);

    --num_reps;
    --num_elem;

    if (num_reps == 0)
      {
	tree.remove(Node_Data(KEY(tree_node)));
	pool.deallocate(tree_node);
      }
  }
  
private:

      /* Borra el rango secuencialmente */
  iterator delete_range (iterator beg, const iterator & end)
  {
    while (beg != end)
      beg.del();

    return beg;
  }

public:

  iterator erase (iterator beg, const iterator & end)
  {
    Aleph::verify_container_and_iterator (*this, beg);
    Aleph::verify_iterators (beg, end);

    return delete_range(beg, end);
  }

  void clear ()
  {
    destroyRec (static_cast<Node *> (tree.getRoot ()));
    num_elem = 0;
  }
};


    template <typename T>
typename iterator_traits<typename multiset<T>::iterator>::difference_type 
distance (typename multiset<T>::iterator it1, 
	  typename multiset<T>::iterator it2)
{
  typename iterator_traits<typename multiset<T>::iterator>::difference_type
    counter = 0;

  
  while (it1 != it2)
    {
      ++counter;
      ++it1;
    }

  return counter;
}

} // end namespace Aleph

# endif // AH_MULTISET_H
