
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


# ifndef TPL_RAND_TREE_H
# define TPL_RAND_TREE_H

# include <limits.h>
# include <gsl/gsl_rng.h>
# include <ahUtils.H>
# include <ahFunction.H>
# include <tpl_randNode.H>
# include <tpl_binTreeOps.H>

using namespace Aleph;

namespace Aleph { 

  template <template <typename> class NodeType, typename Key, class Compare>
class Gen_Rand_Tree
{
public:

  using Node = NodeType<Key>;  

private:

  Node *    tree_root; 
  gsl_rng * r;
  Compare & cmp;

  Node * random_insert(Node * root, Node * p) noexcept
  {
    const long & n = COUNT(root); 
    const size_t rn = gsl_rng_uniform_int(r, n + 1);
    if (rn == n) // p wins the raffle?
      return BinTreeXt_Operation<Node, Compare>(cmp).insert_root(root, p);

    Node * result;
    if (cmp(KEY(p), KEY(root))) // KEY(p) < KEY(root) ?
      {
        result = random_insert(LLINK(root), p); 
        if (result != Node::NullPtr) // p was inserted?
          {
            LLINK(root) = result;
            ++COUNT(root); 
            return root;
          }
      }
    else if (cmp(KEY(root), KEY(p))) // KEY(p) > KEY(root) ?
      {
        result = random_insert(RLINK(root), p); 
        if (result != Node::NullPtr) // p was inserted?
          { 
            RLINK(root) = result;
            ++COUNT(root); 
            return root;
          }
      }

    return Node::NullPtr; // duplicated key ==> no insertion
  }

  Node * random_insert_dup(Node * root, Node * p) noexcept
  {
    const long & n = COUNT(root); 
    const size_t rn = gsl_rng_uniform_int(r, n + 1);
    if (rn == n) // p wins the raffle?
      return BinTreeXt_Operation<Node, Compare>(cmp).insert_dup_root(root, p); 

    if (cmp(KEY(p), KEY(root))) // KEY(p) < KEY(root) ?
      LLINK(root) = random_insert_dup(LLINK(root), p); 
    else
      RLINK(root) = random_insert_dup(RLINK(root), p); 
      
    ++COUNT(root); 

    return root;
  }

  Gen_Rand_Tree& operator = (const Gen_Rand_Tree&);

  void init(unsigned long seed) noexcept
  {
    r = gsl_rng_alloc (gsl_rng_mt19937);

    if (r == nullptr)
      throw std::bad_alloc();

    gsl_rng_set(r, seed % gsl_rng_max(r));
  }

public:

  Compare & key_comp() noexcept { return cmp; }

  Compare & get_compare() noexcept { return key_comp(); }

  gsl_rng * gsl_rng_object() noexcept { return r;}

  void set_seed(unsigned long seed) noexcept { gsl_rng_set(r, seed); }

  Gen_Rand_Tree(unsigned int seed, Compare & __cmp) noexcept
    : tree_root(Node::NullPtr), r(nullptr), cmp(__cmp)
  {
    init(seed);
  }

  Gen_Rand_Tree(unsigned int seed, Compare && cmp = Compare()) noexcept
    : Gen_Rand_Tree(seed, cmp)
  {
    // empty 
  }

  Gen_Rand_Tree(Compare & cmp) noexcept 
  : Gen_Rand_Tree(time(nullptr), cmp) { /* empty */ }

  Gen_Rand_Tree(Compare && cmp = Compare()) noexcept
    : Gen_Rand_Tree(time(nullptr), std::forward<Compare>(cmp)) { /* empty */ }

  void swap(Gen_Rand_Tree & tree) noexcept
  {
    std::swap(tree_root, tree.tree_root);
    std::swap(r, tree.r);
    std::swap(cmp, tree.cmp);
  }

  virtual ~Gen_Rand_Tree() noexcept
  {
    if (r != nullptr)
      gsl_rng_free(r);
  }

  Node * insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);
    assert((LLINK(p) == Node::NullPtr) and (RLINK(p) == Node::NullPtr));
    assert(COUNT(p) == 1);

    Node * result = random_insert(tree_root, p);
    if (result == Node::NullPtr) 
      return nullptr;

    return tree_root = result;
  }

  Node * insert_dup(Node * p) noexcept
  {
    assert(p != Node::NullPtr);
    assert((LLINK(p) == Node::NullPtr) and (RLINK(p) == Node::NullPtr));
    assert(COUNT(p) == 1);

    return tree_root = random_insert_dup(tree_root, p);
  }

private:

  Node * random_join_exclusive(Node * tl, Node * tr) noexcept
  {
    if (tl == Node::NullPtr) 
      return tr;

    if (tr == Node::NullPtr) 
      return tl;

    const size_t & m = COUNT(tl);
    const size_t & n = COUNT(tr);
    const size_t rn = 1 + gsl_rng_uniform_int(r, m + n);  
    if (rn <= m) 
      {    // left subtre wins
        COUNT(tl) += COUNT(tr);
        RLINK(tl) = random_join_exclusive(RLINK(tl), tr);
        return tl;
      }
    else 
      {
        COUNT(tr) += COUNT(tl);
        LLINK(tr) = random_join_exclusive(tl, LLINK(tr));
        return tr;
      }
  }

  Node * random_remove(Node *& root, const Key & key) noexcept
  {
    if (root == Node::NullPtr) 
      return Node::NullPtr;

    Node * ret_val;
    if (cmp(key, KEY(root)))
      {
        ret_val = random_remove(LLINK(root), key);
        if (ret_val != Node::NullPtr)
          COUNT(root)--;

        return ret_val;
      }
    else if (cmp(KEY(root), key))
      {
        ret_val = random_remove(RLINK(root), key);
        if (ret_val != Node::NullPtr)
          COUNT(root)--;

        return ret_val;
      }

        // key was found
    ret_val = root;
    root = random_join_exclusive(LLINK(root), RLINK(root));
    ret_val->reset();

    return ret_val;
  }

public:

  Node * remove(const Key & key) noexcept
  {
    Node * ret_val = random_remove(tree_root, key);

    return ret_val != Node::NullPtr ? ret_val : nullptr;
  }

  Node * search(const Key & key) const noexcept
  {
    Node * retVal = searchInBinTree<Node, Compare>(tree_root, key, cmp);
    return retVal == Node::NullPtr ? nullptr : retVal;
  }

  Node * search_or_insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);
    assert(COUNT(p) == 1);

    Node * result = search(KEY(p));
    if (result != nullptr)
      return result;

    tree_root = random_insert(tree_root, p);

    return p;
  }

  bool verify() const
  {
    return check_rank_tree(tree_root);
  }

  Node *& getRoot() noexcept { return tree_root; }

  Node * select(const size_t i) const noexcept
  {
    return Aleph::select(tree_root, i); 
  }

  size_t size() const noexcept { return COUNT(tree_root); }

  std::pair<long, Node*> position(const Key & key) noexcept
  {
    std::pair<long, Node*> ret_val;
    ret_val.first = BinTreeXt_Operation<Node, Compare> (cmp).
      inorder_position(tree_root, key, ret_val.second);

    return ret_val;
  }

  std::pair<long, Node*> find_position(const Key & key) noexcept
  {
    std::pair<long, Node*> r(-2, nullptr);
    
    r.first = BinTreeXt_Operation<Node, Compare> (cmp) .
      find_position(tree_root, key, r.second);

    return r;
  }

private:

  Node * remove_pos(Node *& root, const size_t pos) noexcept
  {
    if (pos == COUNT(LLINK(root)))
      {
	Node * ret_val = root;
	root = random_join_exclusive(LLINK(ret_val), RLINK(ret_val));
	return ret_val;
      }

    --COUNT(root);
    if (pos < COUNT(LLINK(root)))
      return remove_pos(LLINK(root), pos);
    else
      return remove_pos(RLINK(root), pos - COUNT(LLINK(root)) - 1);
  }

public:

  Node * remove_pos(const size_t i) noexcept
  {
    if (i >= COUNT(tree_root))
      throw std::out_of_range("infix position out of range");

    return remove_pos(tree_root, i);
  }

  bool split_key(const Key & key, Gen_Rand_Tree & t1, Gen_Rand_Tree & t2)
    noexcept
  {
    return split_key_rec_xt(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  void split_key_dup(const Key & key, Gen_Rand_Tree & t1, Gen_Rand_Tree & t2) 
    noexcept
  {
    split_key_dup_rec_xt(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  void split_pos(size_t pos, Gen_Rand_Tree & t1, Gen_Rand_Tree & t2) noexcept
  {
    split_pos_rec(tree_root, pos, t1.getRoot(), t2.getRoot());
  }

private:

  Node * random_join(Node * t1, Node * t2, Node *& dup)
  {
    if (t1 == Node::NullPtr) 
      return t2;

    if (t2 == Node::NullPtr) 
      return t1;

    Node * ret = Node::NullPtr;
    const size_t & m = COUNT(t1);
    const size_t & n = COUNT(t2);
    const size_t rn = 1 + gsl_rng_uniform_int(r, m + n);
    if (rn <= m)
      {      // root of t1 wins
	Node * l = LLINK(t1), * r = RLINK(t1);
	t1->reset();
      t1_wins:
	ret = insert_root_xt(t2, t1, cmp);
	if (ret != Node::NullPtr)
	  {
	    LLINK(ret) = random_join(LLINK(ret), l);
	    RLINK(ret) = random_join(RLINK(ret), r);
	    COUNT(ret) = COUNT(LLINK(ret)) + 1 + COUNT(RLINK(ret));
	  }
	else
	  {
	    dup = random_insert_dup(dup, random_remove(t2, KEY(t1)));
	    goto t1_wins;
	  }
      }
    else 
      {     // root of t2 wins
	Node * l = LLINK(t2), * r = RLINK(t2);
	t2->reset();
      t2_wins:
	ret = insert_root_xt(t1, t2, cmp);
	if (ret != Node::NullPtr)
	  {
	    LLINK(ret) = random_join(l, LLINK(ret));
	    RLINK(ret) = random_join(r, RLINK(ret));
	    COUNT(ret) = COUNT(LLINK(ret)) + 1 + COUNT(RLINK(ret));
	  }
	else
	  {
	    dup = random_insert_dup(dup, random_remove(t1, KEY(t2)));
	    goto t2_wins;
	  }
      }
    
    return ret;
  }

  Node * random_join(Node * t1, Node * t2)
  {
    if (t1 == Node::NullPtr) 
      return t2;

    if (t2 == Node::NullPtr) 
      return t1;

    Node * ret = Node::NullPtr;
    const size_t & m = COUNT(t1);
    const size_t & n = COUNT(t2);
    const size_t total = m + n;
    const size_t rn = 1 + gsl_rng_uniform_int(r, total);
    if (rn <= m) 
      {     // root of t1 wins
	Node * l = LLINK(t1), * r = RLINK(t1);
	t1->reset();
	ret  = insert_dup_root_xt(t2, t1, cmp);
	LLINK(ret) = random_join(LLINK(ret), l);
	RLINK(ret) = random_join(RLINK(ret), r);
      }
    else 
      {     // root of t2 wins
	Node * l = LLINK(t2), * r = RLINK(t2);
	t2->reset();
	ret  = insert_dup_root_xt(t1, t2, cmp);
	LLINK(ret) = random_join(l, LLINK(ret));
	RLINK(ret) = random_join(r, RLINK(ret));
      }

    COUNT(ret) = total;
    return ret;
  }

public:

  void join(Gen_Rand_Tree & t, Gen_Rand_Tree & dup) noexcept
  {
    tree_root = random_join(tree_root, t.tree_root, dup.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_dup(Gen_Rand_Tree & t) noexcept
  {
    tree_root = random_join(tree_root, t.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_exclusive(Gen_Rand_Tree & t) noexcept
  {
    tree_root = random_join_exclusive(tree_root, t.tree_root);    
    t.tree_root = Node::NullPtr;
  }

  struct Iterator : public BinNodeInfixIterator<Node>
  {
    Iterator(Gen_Rand_Tree & t) : BinNodeInfixIterator<Node>(t.getRoot()) {}
  };
};

    template <typename Key, class Compare = Aleph::less<Key>>
struct Rand_Tree : public Gen_Rand_Tree<RandNode, Key, Compare> 
{
  using Base = Gen_Rand_Tree<RandNode, Key, Compare>;
  using Base::Base;
};


    template <typename Key, class Compare = Aleph::less<Key>>
struct Rand_Tree_Vtl : public Gen_Rand_Tree<RandNodeVtl,Key,Compare>
{
  using Base = Gen_Rand_Tree<RandNodeVtl,Key,Compare>;
  using Base::Base;
};


} // end namespace Aleph

# endif // TPL_RAND_TREE_H 

