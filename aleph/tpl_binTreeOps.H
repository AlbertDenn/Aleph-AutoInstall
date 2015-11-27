
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

# ifndef TPL_BINTREEOPS_H
# define TPL_BINTREEOPS_H

# include <tpl_binNodeUtils.H>
# include <tpl_binNodeXt.H>

namespace Aleph {

  template <class Node, class Cmp = Aleph::less<typename Node::key_type>>
class BinTree_Operation
{
protected:

  Cmp & cmp;

public:

  Cmp & key_comp() noexcept { return cmp; }

  Cmp & get_compare() noexcept { return cmp; }

  using Key = typename Node::key_type; 

  using key_type = typename Node::key_type; 

  BinTree_Operation(Cmp & __cmp) noexcept : cmp(__cmp) { /* empty */ }

  BinTree_Operation(Cmp && __cmp = Cmp()) noexcept : cmp(__cmp) { /* empty */ }

  Node * search(Node * root, const Key & key) noexcept
  {
    return searchInBinTree(root, key, cmp);
  }

  Node * search_parent(Node * root, const Key & key, Node *& parent) noexcept
  {
    return Aleph::search_parent<Node, Cmp>(root, key, parent, cmp);
  }

  Node * search_rank_parent(Node * root, const Key & key) noexcept
  {
    return Aleph::search_rank_parent<Node, Cmp>(root, key, cmp);
  }

  Node * insert(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return root = p;

    if (cmp(KEY(p), KEY(root)))        // ¿p < root?
      return insert(LLINK(root), p);
    else if (cmp(KEY(root), KEY(p)))   // ¿p > root?
      return insert(RLINK(root), p);

    return Node::NullPtr; // clave repetida ==> no hay inserción
  }

  Node * insert_dup(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return root = p;

    if (cmp(KEY(p), KEY(root)))        // ¿p < root?
      return insert_dup(LLINK(root), p);
  
    return insert_dup(RLINK(root), p);
  }

  Node * search_or_insert(Node *& r, Node * p) noexcept
  {
    if (r == Node::NullPtr) 
      return r = p;

    if (cmp(KEY(p), KEY(r)))        // ¿p < root?
      return search_or_insert(LLINK(r), p);
    else if (cmp (KEY(r), KEY(p)))   // ¿p > root?
      return search_or_insert(RLINK(r), p);

    return r; 
  }

private:

  bool __split_key_rec(Node * root, const Key & key, Node *& ts, Node *& tg)
    noexcept
  {
    if (root == Node::NullPtr)
      { 
	ts = tg = Node::NullPtr;
	return true;
      }

    if (cmp(key, KEY(root)) ) // ¿key < KEY(root)?
      if (__split_key_rec(LLINK(root), key, ts, LLINK(root)))
	{
	  tg = root;
	  return true;
	}
      else
	return false;

    if (cmp(KEY(root), key) ) // ¿key > KEY(root)?
      if (__split_key_rec(RLINK(root), key, RLINK(root), tg))
	{
	  ts = root;
	  return true;
	}
    
    return false; 
  }

public:

  bool split_key_rec(Node *& root, const Key & key, Node *& ts, Node *& tg)
    noexcept
  {
    bool ret = __split_key_rec(root, key, ts, tg);
    if (ret)
      root = Node::NullPtr;
    return ret;
  }

private:

  void __split_key_dup_rec(Node * root, const typename Node::key_type & key, 
			   Node *& ts, Node *& tg) noexcept
  {
    if (root == Node::NullPtr)
      {    // key no se encuentra en árbol ==> split tendrá éxito
	ts = tg = Node::NullPtr;
	return;
      }

    if (cmp(key, KEY(root))) // ¿key < KEY(root)?
      __split_key_dup_rec(LLINK(root), key, ts, LLINK(root));
    else if (cmp(KEY(root), key)) // ¿key > KEY(root)?
      __split_key_dup_rec(RLINK(root), key, RLINK(root), tg);
    else // key == KEY(root)
      __split_key_dup_rec(LLINK(root), key, ts, LLINK(root));
  }

public:

  void split_key_dup_rec(Node * root, const typename Node::key_type & key, 
			 Node *& ts, Node *& tg) noexcept
  {
     __split_key_dup_rec(root, key, ts, tg);
     root = Node::NullPtr;
  }

  Node * remove(Node *& root, const Key & key) noexcept
  {
    if (root == Node::NullPtr) 
      return Node::NullPtr;

    if (cmp(key, KEY(root)))       // ¿key < KEY(root)?
      return remove(LLINK(root), key);
    else if (cmp(KEY(root), key))  // ¿key > KEY(root)?
      return remove(RLINK(root), key);

    Node * ret_val = root; 
    root = join_exclusive(LLINK(root), RLINK(root));

    ret_val->reset();

    return ret_val;
  }

  Node * insert_root(Node *& root, Node * p) noexcept
  {
    Node * l = Node::NullPtr, * r = Node::NullPtr;

    if (not split_key_rec(root, KEY(p), l, r)) 
      return Node::NullPtr; 

    LLINK(p) = l;
    RLINK(p) = r;
    root = p;

    return root; 
  }

  Node * insert_dup_root(Node *& root, Node * p) noexcept
  {
    split_key_dup_rec(root, KEY(p), LLINK(p), RLINK(p));

    return root = p;
  }

  Node * join_preorder(Node * t1, Node * t2, Node *& dup) noexcept
  {
    if (t2 == Node::NullPtr) 
      return t1;

    Node * l = LLINK(t2);
    Node * r = RLINK(t2);

    if (insert(t1, t2) == Node::NullPtr)
      insert(dup, t2);

    join_preorder(t1, l, dup);
    join_preorder(t1, r, dup);

    return t1;
  }

  Node * join(Node * t1, Node * t2, Node *& dup) noexcept
  {
    if (t1 == Node::NullPtr) 
      return t2;

    if (t2 == Node::NullPtr) 
      return t1;

    Node * l = LLINK(t1);
    Node * r = RLINK(t1);

    t1->reset();

    while (insert_root(t2, t1) == Node::NullPtr) 
      {
	Node * p = remove(t1, KEY(t1));

	assert(p != Node::NullPtr);

	insert(dup, t1);
      }

    LLINK(t2) = join(l, LLINK(t2), dup);
    RLINK(t2) = join(r, RLINK(t2), dup);

    return t2;
  }

  void split_key(Node * root, const Key & key, Node *& l, Node *& r) noexcept
  {
    if (root == Node::NullPtr) 
      {
	l = r = Node::NullPtr;
	return;
      }

    Node ** current_parent = nullptr;
    Node ** pending_child  = nullptr;
    char current_is_right;
    if (cmp (key, KEY(root)))
      {
	r = root;
	pending_child    = &l;
	current_is_right = true;
      }
    else
      {
	l = root;
	pending_child    = &r;
	current_is_right = false;
      }

    Node * current = root;
    while (current != Node::NullPtr) 
      {
	if (cmp (key, KEY(current)))
	  { /* current must be in right side */
	    if (not current_is_right)
	      { 
		current_is_right = not current_is_right;
		*pending_child   = *current_parent; /* change of side */
		pending_child    = current_parent;
	      }

	    current_parent = &LLINK(current);
	  }
	else
	  { /* current must be in left side */
	    if (current_is_right)
	      { 
		current_is_right = not current_is_right;
		*pending_child   = *current_parent; /* change of side */
		pending_child    = current_parent;
	      }
	    current_parent = &RLINK(current);
	  }

	current = *current_parent;
      }

    *pending_child = Node::NullPtr;
    root = Node::NullPtr;
  }

  Node * insert_root_rec(Node * root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      return p; /* insertion in empty tree */

    if (cmp(KEY(p), KEY(root)))
      { /* insert in left subtree */
	Node *left_branch = insert_root_rec(LLINK(root), p);
	if (left_branch == Node::NullPtr)
	  return Node::NullPtr;

	LLINK(root) = left_branch;
	root        = rotate_to_right(root);
      }
    else if (cmp(KEY(root), KEY(p)))
      { /* insert in right subtree */
	Node *right_branch = insert_root_rec(RLINK(root), p);
	if (right_branch == Node::NullPtr)
	  return Node::NullPtr;

	RLINK(root) = right_branch;
	root        = rotate_to_left(root);
      }
    else
      return Node::NullPtr; /* duplicated key */

    return root;
  }

  Node * search_or_insert_root_rec(Node * root, Node * p) noexcept
  {
    if (root == Node::NullPtr)
      return p; // insertion in empty tree

    if (cmp(KEY(p), KEY(root)))
      {     // insert in left subtree 
	Node * left_branch = search_or_insert_root_rec(LLINK(root), p);
	if (left_branch == p) // ¿hubo inserción?
	  {
	    LLINK(root) = left_branch;
	    root        = rotate_to_right(root);

	    return p;
	  }

        return left_branch; // no la hubo
      }
    else if (cmp(KEY(root), KEY(p)))
      {     // insert in right subtree 
	Node * right_branch = search_or_insert_root_rec(RLINK(root), p);
	if (right_branch == p) // ¿hubo inserción?
	  {
	    RLINK(root) = right_branch;
	    root        = rotate_to_left(root);

	    return p;
	  }

	return right_branch; // no la hubo
      }

    return root;
  }
};


    template <class Node,
	      class Cmp = Aleph::less<typename Node::key_type>>
class BinTreeXt_Operation : public BinTree_Operation<Node, Cmp>
{
public:

  using Key = typename Node::key_type;

  BinTreeXt_Operation(Cmp & cmp) noexcept
    : BinTree_Operation<Node, Cmp>(cmp)
  {
    // empty
  }

  BinTreeXt_Operation(Cmp && cmp = Cmp()) noexcept
    : BinTree_Operation<Node, Cmp>(std::forward<Cmp>(cmp))
  {
    // empty
  }

  long inorder_position(Node * r, const Key & key, Node *& p) noexcept
  {
    assert(COUNT(Node::NullPtr) == 0);

    if (r == Node::NullPtr)
      return -1;

    if (this->cmp(key, KEY(r)))
      return inorder_position(LLINK(r), key, p);
    else if (this->cmp(KEY(r), key)) 
      {
	long ret = inorder_position(RLINK(r), key, p);
	if (ret != -1)
	  return ret + COUNT(LLINK(r)) + 1;
	return ret;
      }
    else
      {
	p = r;
	return COUNT(LLINK(r));
      }
  }

  long find_position(Node * r, const Key & key, Node *& p) noexcept
  {
    assert(COUNT(Node::NullPtr) == 0);

    Node * parent = nullptr;
    long pos = COUNT(LLINK(r));

    while (r != Node::NullPtr)
      if (this->cmp(key, KEY(r)))  
	{
	  parent = r;
	  r = LLINK(r);
	  pos -= (COUNT(RLINK(r)) + 1);
	}
      else if (this->cmp(KEY(r), key)) 
	{
	  parent = r;
	  r = RLINK(r);
	  pos += COUNT(LLINK(r)) + 1;
	}
      else
	{
	  p = r;
	  return pos;
	}

    p = parent;

    return pos;
  }

  bool split_key_rec(Node * root, const Key & key, Node *& l, Node *& r) 
    noexcept
  {
    if (root == Node::NullPtr)
      {
	l = r = Node::NullPtr;
	return true;
      }

    if (this->cmp(key, KEY(root)))
      {
	if (not split_key_rec(LLINK(root), key, l, LLINK(root))) 
	  return false;

	r = root;
	COUNT(r) -= COUNT(l);
      }
    else if (this->cmp(KEY(root), key))
      {
	if (not split_key_rec(RLINK(root), key, RLINK(root), r)) 
	  return false;

	l = root;
	COUNT(l) -= COUNT(r);
      }
    else
      return false; // clave duplicada

    return true;
  }

  void split_key_dup_rec(Node * root, const Key & key, Node *& l, Node *& r)
    noexcept
  {
    if (root == Node::NullPtr)
      {
	l = r = Node::NullPtr;
	return;
      }

    if (this->cmp(key, KEY(root)))
      {
	split_key_dup_rec(LLINK(root), key, l, LLINK(root));
	r = root;
	COUNT(r) -= COUNT(l);
      }
    else
      {
	split_key_dup_rec(RLINK(root), key, RLINK(root), r);
	l = root;
	COUNT(l) -= COUNT(r);
      }
  }

  Node * insert_root(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return p;

    if (not split_key_rec(root, KEY(p), LLINK(p), RLINK(p)))
      return Node::NullPtr;

    COUNT(p) = COUNT(LLINK(p)) + COUNT(RLINK(p)) + 1;
    root = p;

    return p; 
  }

  Node * insert_dup_root(Node *& root, Node * p) noexcept
  {
    if (root == Node::NullPtr) 
      return p;

    split_key_dup_rec(root, KEY(p), LLINK(p), RLINK(p));

    COUNT(p) = COUNT(LLINK(p)) + COUNT(RLINK(p)) + 1;

    return root = p;
  }
};

} // end namespace Aleph

# endif // TPL_BINTREEOPS_H
