
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


# ifndef TPL_TREAP_H
# define TPL_TREAP_H

# include <gsl/gsl_rng.h> 
# include <ahFunction.H>
# include <tpl_binNodeUtils.H>
# include <treapNode.H>
# include <tpl_binTreeOps.H>

using namespace Aleph;

namespace Aleph
{

  template <template <typename> class NodeType, typename Key, class Compare>
class Gen_Treap
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
    r = gsl_rng_alloc (gsl_rng_mt19937);

    if (r == nullptr)
      throw std::bad_alloc();

    gsl_rng_set(r, seed % gsl_rng_max(r));
  }

  public:

  void set_seed(unsigned long seed) noexcept { gsl_rng_set(r, seed); }

  void swap (Gen_Treap & tree) noexcept
  {
    std::swap(tree_root, tree.tree_root);
    std::swap(cmp, tree.cmp);
    std::swap(r, tree.r);
  }

  Compare & key_comp() noexcept { return cmp; }

  Compare & get_compare() noexcept { return key_comp(); }

  Gen_Treap(unsigned long seed, Compare & __cmp) 
    : head_ptr(&head), tree_root(RLINK(head_ptr)), r(nullptr), cmp(__cmp)
  {
    init(seed);
  }

  Gen_Treap(unsigned long seed, Compare && cmp) : Gen_Treap(seed, cmp)
  {
    // empty
  }

  Gen_Treap(Compare & cmp) : Gen_Treap(time(nullptr), cmp) { /* empty */ }

  Gen_Treap(Compare && cmp = Compare()) : Gen_Treap(cmp) { /* empty */ }  

  ~Gen_Treap() 
  {
    if (r != nullptr)
      gsl_rng_free(r);
  }

  gsl_rng * gsl_rng_object() noexcept { return r;}

  Node *& getRoot() noexcept { return tree_root; }

  Node * search(const Key & key) const noexcept
  {
    Node* ret_val = searchInBinTree(tree_root, key, cmp);
    return ret_val == Node::NullPtr ? nullptr : ret_val;
  }

private:

  Node * insert(Node * root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return p;

    Node * insertion_result = nullptr;
    if (cmp(KEY(p), KEY(root))) 
      {
        insertion_result = insert(LLINK(root), p);
        if (insertion_result == Node::NullPtr) 
          return Node::NullPtr;

        LLINK(root) = insertion_result;
        if (PRIO(insertion_result) < PRIO(root))
          return rotate_to_right(root);
        else
          return root;
      }
    else if (cmp(KEY(root), KEY(p)))
      {
        insertion_result = insert(RLINK(root), p);
        if (insertion_result == Node::NullPtr) 
          return Node::NullPtr;

        RLINK(root) = insertion_result;
        if (PRIO(insertion_result) < PRIO(root))
          return rotate_to_left(root);
        else
          return root;
      }

    return Node::NullPtr;
  }

  Node * search_or_insert (Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      return root = p;

    if (cmp(KEY(p), KEY(root)))
      {
        Node * ret = search_or_insert(LLINK(root), p);
        if (ret == p) // p was inserted?
          if (PRIO(LLINK(root)) < PRIO(root))
            root = rotate_to_right (root);

        return ret;
      }
    else if (cmp(KEY(root), KEY(p)))
      {
        Node * ret = search_or_insert(RLINK(root), p);
        if (ret == p) // p was inserted?
          if (PRIO(RLINK(root)) < PRIO(root))
            root = rotate_to_left (root);
            
        return ret;
      }

    return root;
  }

  Node * insert_dup(Node * root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return p;

    if (cmp(KEY(p), KEY(root))) 
      {
        Node * result = LLINK(root) = insert_dup(LLINK(root), p);
        if (PRIO(result) < PRIO(root))
          return rotate_to_right(root);
        else
          return root;
      }
    else
      {
	Node * result = RLINK(root) = insert_dup(RLINK(root), p);
	if (PRIO(result) < PRIO(root))
	  return rotate_to_left(root);
	else
	  return root;
      }
    }

  public:

  Node * insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p)       = gsl_rng_get(r); // selección aleatoria de prioridad
    Node * result = insert(tree_root, p);
    if (result == Node::NullPtr) 
      return nullptr;

    tree_root = result;

    return p;  
  }

  Node * search_or_insert(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p) = gsl_rng_get(r); // selección aleatoria de prioridad
    
    return search_or_insert(tree_root, p);  
  }

  Node * insert_dup(Node * p) noexcept
  {
    assert(p != Node::NullPtr);

    PRIO(p)   = gsl_rng_get(r); // selección aleatoria de prioridad
    tree_root = insert_dup(tree_root, p);

    return p;  
  }

  bool verify() const noexcept { return is_treap(tree_root); }

  Node * remove(const Key & key) noexcept
  {
    Node ** pp = &RLINK(head_ptr);
    Node * p   = tree_root;

    while (p != Node::NullPtr) // descender buscando la clave
      if (cmp(key, KEY(p)))
        {
          pp = &LLINK(p);
          p  = LLINK(p);
        }
      else if (cmp(KEY(p), key))
        {
          pp = &RLINK(p);
          p  = RLINK(p);
        }
      else
        break; // encontrada!

    if (p == Node::NullPtr) 
      return nullptr; // clave no fue encontrada

        // rotar p hasta que devenga hoja
    while (not (LLINK(p) == Node::NullPtr and RLINK(p) == Node::NullPtr))
      if (PRIO(LLINK(p)) < PRIO(RLINK(p)))
        {
          *pp = rotate_to_right(p);
          pp  = &RLINK(*pp);
        }
      else
        {
          *pp = rotate_to_left(p);
          pp  = &LLINK(*pp);
        }

    *pp = Node::NullPtr;

    p->reset();

    return p;
  }

private:

  static Node * join_exclusive(Node * t1, Node * t2) noexcept
  {
    if (t1 == Node::NullPtr)
      return t2;

    if (t2 == Node::NullPtr)
      return t1;

    if (PRIO(t1) < PRIO(t2))
      {
	RLINK(t1) = join_exclusive(RLINK(t1), t2);
	return t1;
      }
    else
      {
	LLINK(t2) = join_exclusive(t1, LLINK(t2));
	return t2;
      }
  }

  Node * remove(Node *& root, const Key & key) noexcept
  {
    if (root == Node::NullPtr)
      return Node::NullPtr;

    if (cmp(key, KEY(root)))
      return remove(LLINK(root), key);
    else if (cmp(KEY(root), key))
      return remove(RLINK(root), key);

    Node * ret = root;     // key found!
    root = join_exclusive(LLINK(root), RLINK(root));
    ret->reset();

    return ret;
  }

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

  void join(Gen_Treap & t, Gen_Treap & dup) noexcept
  {
    join(tree_root, t.tree_root, dup.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_dup(Gen_Treap & t) noexcept
  {
    join_dup(tree_root, t.tree_root);
    t.tree_root = Node::NullPtr;
  }

  void join_exclusive(Gen_Treap & t) noexcept
  {
    tree_root = join_exclusive(tree_root, t.tree_root);    
    t.tree_root = Node::NullPtr;
  }

  bool split_key(const Key & key, Gen_Treap & t1, Gen_Treap & t2)
  {
    return split_key_rec(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  void split_key_dup(const Key & key, Gen_Treap & t1, Gen_Treap & t2)
  {
    split_key_dup_rec(tree_root, key, t1.getRoot(), t2.getRoot());
  }

  struct Iterator : public BinNodeInfixIterator<Node>
  {
    Iterator(Gen_Treap & t) : BinNodeInfixIterator<Node>(t.getRoot()) {}
  };
};

    template <typename Key, class Compare = Aleph::less<Key>>
struct Treap : public Gen_Treap<TreapNode, Key, Compare> 
{
  using Base = Gen_Treap<TreapNode, Key, Compare>;
  using Base::Base;
};

  
    template <typename Key, class Compare = Aleph::less<Key>>
struct Treap_Vtl : public Gen_Treap<TreapNodeVtl, Key, Compare>
{
  using Base = Gen_Treap<TreapNodeVtl, Key, Compare>;
  using Base::Base;
};


} // end namespace Aleph
# endif // TPL_TREAP_H

