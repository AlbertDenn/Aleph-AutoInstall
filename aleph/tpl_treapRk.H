
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

# ifndef TPL_TREAPRK_H
# define TPL_TREAPRK_H

# include <gsl/gsl_rng.h>
# include <ahFunction.H>
# include <tpl_binTreeOps.H>
# include <ran_array.h>
# include <treapNode.H>

using namespace Aleph;

namespace Aleph {

class TreapRkNode_Data
{
  unsigned long priority; 

  unsigned long count;

public:

  TreapRkNode_Data() noexcept : priority(Max_Priority), count(1) 
  { 
    /* empty */ 
  }

  TreapRkNode_Data(SentinelCtor) noexcept : priority(Max_Priority), count(0) 
  { 
    /* empty */ 
  }

  unsigned long & getPriority() noexcept { return priority; }

  unsigned long & getCount() noexcept { return count; }

  void reset() noexcept { count = 1; }
};

DECLARE_BINNODE_SENTINEL(Treap_Rk_Node, 80, TreapRkNode_Data);


    template <template <class> class NodeType, class Key, class Compare>
class Gen_Treap_Rk
{
public:

  using Node = NodeType<Key>; 

private:

  Node      head;
  Node *    head_ptr;
  Node *&   tree_root;
  gsl_rng * r;
  Compare & cmp;

  void init(unsigned int seed)
  {
    PRIO(head_ptr) = Min_Priority;
    r = gsl_rng_alloc(gsl_rng_mt19937);

    if (r == nullptr)
      throw std::bad_alloc();

    gsl_rng_set(r, seed % gsl_rng_max(r));
  }

public:

  void set_seed(unsigned long seed) noexcept { gsl_rng_set(r, seed); }

  Compare & key_comp() noexcept { return cmp; }

  Compare & get_compare() noexcept { return key_comp(); }

  Gen_Treap_Rk(unsigned long seed, Compare & __cmp) 
    : head_ptr(&head), tree_root(RLINK(head_ptr)), r(nullptr), cmp(__cmp)
  {
    init(seed);
  }

  Gen_Treap_Rk(unsigned long seed, Compare && cmp) 
    : Gen_Treap_Rk(seed, cmp)
  {
    init(seed);
  }

  Gen_Treap_Rk(Compare & cmp) : Gen_Treap_Rk(time(nullptr), cmp)
  {
    // empty
  }

  Gen_Treap_Rk(Compare && cmp = Compare()) 
    : Gen_Treap_Rk(time(nullptr), std::forward<Compare>(cmp))
  {
    // empty
  }

  ~Gen_Treap_Rk()
  {
    if (r != nullptr)
      gsl_rng_free(r);
  }

  void swap(Gen_Treap_Rk & tree) noexcept
  {
    std::swap(tree_root, tree.tree_root);
    std::swap(cmp, tree.cmp);
    std::swap(r, tree.r);
  }

  Node *& getRoot() noexcept { return tree_root; }

  Node * search(const Key & key) const noexcept
  {
    Node* ret_val = searchInBinTree(tree_root, key, cmp);
    return ret_val == Node::NullPtr ? nullptr : ret_val;
  }

private:

  bool insert(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      {
	root = p;
	return true;
      }

    if (cmp(KEY(p), KEY(root)))
      {
	if (insert(LLINK(root), p))
	  {
	    ++COUNT(root);
	    if (PRIO(LLINK(root)) < PRIO(root) )
	      root = rotate_to_right_xt(root);

	    return true;
	  }
      }
    else if (cmp(KEY(root), KEY(p)))
      {
	if (insert(RLINK(root), p))
	  {
	    ++COUNT(root);
	    if (PRIO(RLINK(root)) < PRIO(root) )
	      root = rotate_to_left_xt(root);

	    return true;
	  }
      }

    return false;
  }

  // Search or insert p. Return p if KEY(p) is not in the tree;
  // otherwise, it returns a pointer to a node containing KEY(p) 
  Node * search_or_insert(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      return root = p;

    if (cmp(KEY(p), KEY(root)))
      {
	Node * ret = search_or_insert(LLINK(root), p);
        if (ret == p) // insertion done?
	  { // yes ==> increase counter and perhaps rotate
	    ++COUNT(root);
	    if (PRIO(LLINK(root)) < PRIO(root))
	      root = rotate_to_right_xt(root);
	    
	    assert(PRIO(root) <= PRIO(LLINK(root)) and
		   PRIO(root) <= PRIO(LLINK(root)));
	  }

	return ret;
      }
    else if (cmp(KEY(root), KEY(p)))
      {
	Node * ret = search_or_insert(RLINK(root), p);
        if (ret == p) // insertion done?
	  { // yes ==> increase counter and perhaps rotate
	    ++COUNT(root);
	    if (PRIO(RLINK(root)) < PRIO(root))
	      root = rotate_to_left_xt(root);

	    assert(PRIO(root) <= PRIO(LLINK(root)) and
		   PRIO(root) <= PRIO(LLINK(root)));
	  }

	return ret;
      }

    assert(PRIO(root) <= PRIO(LLINK(root)) and PRIO(root) <= PRIO(LLINK(root)));

    return root;
  }

  // Return p; root could be modified
  Node * insert_dup(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      return root = p;

    if (cmp(KEY(p), KEY(root)))
      {
        insert_dup(LLINK(root), p);
	++COUNT(root);
        if (PRIO(LLINK(root)) < PRIO(root))
          root = rotate_to_right_xt(root);
      }
    else
      {
	insert_dup(RLINK(root), p);
	++COUNT(root);
	if (PRIO(RLINK(root)) < PRIO(root))
	  root = rotate_to_left_xt(root);
      }

    return p;
  }

public:

  Node * insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p) = gsl_rng_get(r);

    return insert(tree_root, p) ? p : nullptr;
  }

  Node * insert_dup(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p) = gsl_rng_get(r);

    return insert_dup(tree_root, p);
  }

 
  Node * search_or_insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p) = gsl_rng_get(r);

    return search_or_insert(tree_root, p);
  }

  bool verify() const { return is_treap(tree_root); }

private:

  static Node * join_exclusive(Node * t1, Node * t2) noexcept
  {
    if (t1 == Node::NullPtr)
      return t2;

    if (t2 == Node::NullPtr)
      return t1;

    if (PRIO(t1) < PRIO(t2))
      {
	COUNT(t1) += COUNT(t2);
	RLINK(t1) = join_exclusive(RLINK(t1), t2);
	return t1;
      }
    else
      {
	COUNT(t2) += COUNT(t1);
	LLINK(t2) = join_exclusive(t1, LLINK(t2) );
	return t2;
      }
  }

  Node * remove(Node *& root, const Key & key) noexcept
  {
    if (root == Node::NullPtr)
      return Node::NullPtr;

    Node * ret_val;
    if (cmp(key, KEY(root) ))
      ret_val = remove(LLINK(root), key);
    else if (cmp(KEY(root), key))
      ret_val = remove(RLINK(root), key);
    else
      {
	ret_val = root;
	root = join_exclusive(LLINK(root), RLINK(root) );
	
	return ret_val;
      }

    if (ret_val == Node::NullPtr)
      return Node::NullPtr;
      
    --COUNT(root);

    return ret_val;
  }

public:

  Node * remove(const Key & key) noexcept
  {
    Node * ret_val = remove(tree_root, key);
    if (ret_val == Node::NullPtr)
      return nullptr;

    ret_val->reset();

    return ret_val;
  } 

  Node * remove(const size_t beg, const size_t end) noexcept
  {
    if (beg > end or end > COUNT(tree_root))
      throw std::range_error("remove of TreapRk out of range");

    Node * before_beg, * after_end;

    Node * ret_val = tree_root;

    split_pos_rec(ret_val, end + 1, ret_val, after_end);
    split_pos_rec(ret_val, beg, before_beg, ret_val);

    tree_root = join_exclusive(before_beg, after_end);

    return ret_val;
  }

private:

  static Node * remove_pos(Node *& root, const size_t pos) noexcept
  {
    if (pos == COUNT(LLINK(root)))
      {
	Node * ret_val = root;
	root = join_exclusive(LLINK(ret_val), RLINK(ret_val));
	return ret_val;
      }

    --COUNT(root);
    if (pos < COUNT(LLINK(root)))
      return remove_pos(LLINK(root), pos);
    else
      return remove_pos(RLINK(root), pos - COUNT(LLINK(root)) - 1);
  }
  
public:

  Node * remove_pos(const size_t pos)
  {
    if (pos >= COUNT(tree_root))
      throw std::out_of_range("infix position out of range");

    return remove_pos(tree_root, pos);
  }

  Node * select(const size_t i) const
  {
    return Aleph::select(tree_root, i); 
  }

  size_t size() const noexcept { return COUNT(tree_root); }

  bool is_empty() const noexcept { return tree_root == Node::NullPtr; }

  std::pair<int, Node*> position(const Key & key) const noexcept
  {
    std::pair<int, Node*> ret_val;

    ret_val.first = BinTreeXt_Operation<Node, Compare>(cmp).
      inorder_position(tree_root, key, ret_val.second);

    return ret_val;
  }

  std::pair<int, Node*> find_position(const Key & key) const noexcept
  {
    std::pair<int, Node*> r(-2, nullptr);

    r.first = BinTreeXt_Operation <Node, Compare>(cmp) .
      find_position(tree_root, key, r.second);

    return r;
  }

  bool split_key(const Key & key, Gen_Treap_Rk & t1, Gen_Treap_Rk & t2) noexcept
  {
    return split_key_rec_xt(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  void
  split_key_dup(const Key & key, Gen_Treap_Rk & t1, Gen_Treap_Rk & t2) noexcept
  {
    split_key_dup_rec_xt(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  void split_pos(size_t pos, Gen_Treap_Rk & t1, Gen_Treap_Rk & t2)
  {
    split_pos_rec(tree_root, pos, t1.getRoot(), t2.getRoot());
  }

private:

  void join_dup(Node *& t1, Node * t2) noexcept
  {
    if (t2 == Node::NullPtr)
      return;

    Node * l = LLINK(t2), * r = RLINK(t2);
    t2->reset();
    t1 = insert_dup(t1, t2);
    join_dup(t1, l);
    join_dup(t1, r);
  }

  void join(Node *& t1, Node * t2, Node *& dup) noexcept
  {
    if (t2 == Node::NullPtr)
      return;

    Node * l = LLINK(t2), * r = RLINK(t2);
    t2->reset();
  ins:
    Node * ret = insert(t1, t2);
    if (ret == Node::NullPtr)
      {
	dup = insert_dup(dup, remove(t1, KEY(t2)));
	goto ins;
      }

    t1 = ret;
    join(t1, l, dup);
    join(t1, r, dup);
  }

public:

  void join(Gen_Treap_Rk & t, Gen_Treap_Rk & dup) noexcept
  {
    join(tree_root, t.tree_root, dup.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_dup(Gen_Treap_Rk & t) noexcept
  {
    join_dup(tree_root, t.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_exclusive(Gen_Treap_Rk & t) noexcept
  {
    tree_root = join_exclusive(tree_root, t.tree_root);    
    t.tree_root = Node::NullPtr;
  }

  class Iterator
  {
  protected:
    
    mutable Gen_Treap_Rk * tree_ptr; 
    mutable Node *         curr;
    mutable int            curr_pos;

    static const int Pos_Not_Current     = -1;
    static const int Pos_Empty_Container = -2;
    static const int Pos_Not_Updated     = -3;

  private:

    bool is_container_empty() const noexcept
    {
      return COUNT(tree_ptr->getRoot()) == 0;
    }

    bool pos_updated() const noexcept
    {
      return curr_pos != Pos_Not_Updated;
    }

    bool curr_updated() const noexcept
    {
      return curr != nullptr;
    }

    bool is_updated() noexcept
    {
      return pos_updated() and curr_updated();
    }

    void update_pos() const noexcept
    {
      assert(curr != nullptr);

      curr_pos = BinTreeXt_Operation<Node, Compare>(tree_ptr->cmp).
	inorder_position(tree_ptr->getRoot(), KEY(curr), curr);
    }

    void update_curr() const noexcept
    {
      assert(curr_pos != Pos_Not_Updated);

      if (curr_pos == Pos_Empty_Container or curr_pos == Pos_Not_Current or
	  curr_pos == COUNT(tree_ptr->getRoot()))
	return;

      curr = Aleph::select(tree_ptr->getRoot(), curr_pos);
    }

  public:

    Iterator() noexcept
    : tree_ptr(nullptr), curr(nullptr), curr_pos(Pos_Not_Current)
    {
      /* empty */ 
    }

    Iterator(Gen_Treap_Rk & __tree) noexcept
      : tree_ptr(&__tree), curr(nullptr)
    {
      curr_pos = is_container_empty() ? Pos_Empty_Container : 0;
    }

    Iterator(Gen_Treap_Rk & __tree, Node * __curr) noexcept
      : tree_ptr(&__tree), curr(__curr), curr_pos(Pos_Not_Updated)
    {
      // empty
    }

    Iterator(Gen_Treap_Rk & __tree, const size_t pos) noexcept
      : tree_ptr(&__tree), curr(nullptr), curr_pos(pos)
    {
      // empty
    }

    Iterator(const Iterator & itor) noexcept
      : tree_ptr(itor.tree_ptr), curr(itor.curr), curr_pos(itor.curr_pos)
    {
      // Empty
    }

    Iterator & operator = (const Iterator & itor) noexcept
    {
      if (this == &itor)
	return *this;

      tree_ptr = itor.tree_ptr;
      curr = itor.curr;
      curr_pos = itor.curr_pos;

      return *this;
    }

    void reset_first() noexcept
    {
      curr = nullptr;
      curr_pos = is_container_empty() ? Pos_Empty_Container : 0;
    }

    void reset_last() noexcept
    {
      curr = nullptr;
      curr_pos = (is_container_empty() ? Pos_Empty_Container : 
		  COUNT(tree_ptr->getRoot()) - 1);
    }

    void end() noexcept
    {
      reset_last();
      next();
    }

    void reset_to_key(const Key & key) noexcept
    {
      std::pair<int, Node*> p = tree_ptr->find_position(key);
      curr_pos = p.first;
    }

    void reset_to_node(Node * node) noexcept
    {
      curr = node;
      curr_pos = -2;
    }

    void reset_to_pos(size_t pos) noexcept
    {
      curr = nullptr;
      curr_pos = pos;
    }

    Node * get_current() const noexcept
    {
      if (not curr_updated())
	update_curr();

      return curr; 
    }

    Node * get_curr() const noexcept
    {
      return get_current();
    }

    size_t get_current_position() const
      throw(std::exception, std::underflow_error, std::overflow_error)
    {
      if (not pos_updated())
	update_pos();

      if (curr_pos < -1 )
	throw std::range_error("TreapRk iterator has not current");
	
      if (curr_pos > COUNT(tree_ptr->getRoot() ) )
	throw std::range_error("TreapRk iterator has not current");

      return curr_pos;
    }

    size_t get_pos() const { return get_current_position(); }

    bool has_current() const noexcept
    {
      if (not pos_updated())
	update_pos();

      return curr_pos >= 0 and curr_pos < COUNT(tree_ptr->getRoot());
    }

    bool has_curr() const noexcept
    {
      return has_current();
    }

    void prev() throw(std::exception, std::underflow_error)
    {
      if (not has_current() )
	throw std::underflow_error("TreapRk iterator has not current");

      --curr_pos;
      curr = nullptr;
    }

    void next() throw(std::exception, std::overflow_error)
    {
      if (not has_current() )
	throw std::underflow_error("TreapRk iterator has not current");

      ++curr_pos;
      curr = nullptr;
    }

    Node * del()
    {
      if (not has_current() )
	throw std::underflow_error("TreapRk iterator has not current");

      if (not curr_updated())
	update_curr();

      Node * ret_val = tree_ptr->remove(KEY(curr) );
      
      curr = nullptr;

      return ret_val;
    }

    bool operator == (const Iterator & itor) const noexcept
    {
      if (is_container_empty() and itor.is_container_empty())
	return true;

      if (pos_updated() and itor.pos_updated())
	return curr_pos == itor.curr_pos;

      if (curr_updated() and itor.curr_updated())
	return curr == itor.curr;

      if (not pos_updated())
	{
	  update_pos();
	  return curr_pos == itor.curr_pos;
	}

      itor.update_pos();

      return curr_pos == itor.curr_pos;
    }
  
    bool operator != (const Iterator & itor) const
    {
      return not (*this == itor);
    }

    bool verify(Gen_Treap_Rk * r) const noexcept
    {
      return tree_ptr->getRoot() == r->getRoot();
    }

    bool verify(const Iterator & it) const noexcept
    {
      return tree_ptr->getRoot() == it.tree_ptr->getRoot();
    }
  }; // end class Iterator
};

    template <typename Key, class Compare = Aleph::less<Key>>
struct Treap_Rk : public Gen_Treap_Rk<Treap_Rk_Node, Key, Compare> 
{
  using Base = Gen_Treap_Rk<Treap_Rk_Node, Key, Compare>;
  using Base::Base;
};

    template <typename Key, class Compare = Aleph::less<Key>>
struct Treap_Rk_Vtl : public Gen_Treap_Rk<Treap_Rk_NodeVtl, Key, Compare> 
{
  using Base = Gen_Treap_Rk<Treap_Rk_NodeVtl, Key, Compare>;
  using Base::Base;
};


} // end namespace Aleph

# endif // TPL_TREAPRK_H
