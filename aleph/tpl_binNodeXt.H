
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


# ifndef TPL_BINNODEXT_H
# define TPL_BINNODEXT_H

# include <tpl_binNode.H>
# include <tpl_binNodeUtils.H>

using namespace Aleph;

namespace Aleph {

class BinNodeXt_Data
{
  size_t count; // cardinalidad del árbol

public:

  BinNodeXt_Data() noexcept : count(1) {}
  BinNodeXt_Data(SentinelCtor) noexcept : count(0) {}

  size_t & getCount() noexcept { return count; }
  size_t size() const noexcept { return count; }

  void reset() noexcept { count = 1; }
};

DECLARE_BINNODE_SENTINEL(BinNodeXt, 255, BinNodeXt_Data);

template <class Node> inline auto & COUNT(Node * p) noexcept
{
  return p->getCount(); 
}


    template <class Node> static inline 
Node * __select_rec(Node * r, const size_t i) noexcept
{ 
  assert(r != Node::NullPtr);
  assert(COUNT(Node::NullPtr) == 0);

  if (i == COUNT(LLINK(r))) 
    return r;

  if (i < COUNT(LLINK(r))) 
    return __select_rec(LLINK(r), i);

  return __select_rec(RLINK(r), i - COUNT(LLINK(r)) - 1);
}

    template <class Node> inline 
Node * select_rec(Node * r, const size_t i) 
      throw(std::exception, std::out_of_range) 
{ 
  if (i >= COUNT(r))
    throw std::out_of_range("infix position out of range");
  
  return __select_rec(r, i);
}

    template <class Node> inline 
Node * select(Node * r, const size_t pos) 
  throw(std::exception, std::out_of_range)
{
  assert(COUNT(Node::NullPtr) == 0);

  if (pos >= COUNT(r))
    throw std::out_of_range("infix position out of range");

  for (size_t i = pos; i != COUNT(LLINK(r)); /* nada */)
    {
      assert(i < COUNT(r) and 
	     COUNT(LLINK(r)) + COUNT(RLINK(r)) + 1 == COUNT(r));

      if (i < COUNT(LLINK(r)))
        r = LLINK(r);
      else
        {
          i -= COUNT(LLINK(r)) + 1;
          r = RLINK(r);
        }
    }

  return r;
}

    template <class Node> inline 
Node * select(Node * root, const size_t pos, Node *& parent) 
  throw(std::exception, std::out_of_range)
{
  assert(COUNT(Node::NullPtr) == 0);

  if (pos >= COUNT(root))
    throw std::out_of_range("infix position out of range");

  parent = Node::NullPtr;
  for (size_t i = pos; i != COUNT(LLINK(root)); /* nada */)
    {
      assert(i < COUNT(root) and 
	     COUNT(LLINK(root)) + COUNT(RLINK(root)) + 1 == COUNT(root));

      parent = root;
      if (i < COUNT(LLINK(root)))
	root = LLINK(root);
      else
        {
          i -= COUNT(LLINK(root)) + 1;
          root = RLINK(root);
        }
    }

  return root;
}

    template <class Node, class Compare> inline
long inorder_position(Node * r, 
		      const typename Node::key_type & key, 
		      Node *& p, Compare & cmp) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  if (r == Node::NullPtr)
    return -1;

  if (cmp(key, KEY(r))) 
    return inorder_position(LLINK(r), key, p, cmp);
  else if (cmp(KEY(r), key)) 
    {
      long ret = inorder_position(RLINK(r), key, p, cmp);
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

template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline long inorder_position(Node * r, 
			     const typename Node::key_type & key, 
			     Node *& p, Compare && cmp = Compare()) noexcept
{
  return inorder_position(r, key, p, cmp);
}

    template <class Node, class Compare> inline
long inorder_position(Node * r, const typename Node::key_type & key, 
		      Compare & cmp) noexcept
{
  Node * p = nullptr;
  return inorder_position(r, key, p, cmp);
}

    template <class Node, class Compare = Aleph::less<typename Node::key_type>>
inline long inorder_position(Node * r, const typename Node::key_type & key, 
			     Compare && cmp = Compare()) noexcept
{
  return inorder_position(r, key, cmp);
}

    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
long find_position(Node * r, const typename Node::key_type & key, 
		   Node *& p, Compare & cmp) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  Node * parent = nullptr;
  long pos = COUNT(LLINK(r));

  while (r != Node::NullPtr)
    if (cmp(key, KEY(r)))  
      {
        parent = r;
        r = LLINK(r);
        pos -= (COUNT(RLINK(r)) + 1);
      }
    else if (cmp(KEY(r), key)) 
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

template <class Node, 
	  class Compare = Aleph::less<typename Node::key_type>> inline
long find_position(Node * r, const typename Node::key_type & key, 
		   Node *& p, Compare && cmp = Compare()) noexcept
{
  return find_position(r, key, p, cmp);
}

    template <class Node, class Compare> inline
Node * insert_by_key_xt(Node *& r, Node * p, Compare & cmp) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  if (r == Node::NullPtr) 
    return r = p;

  Node * q = Node::NullPtr;
  if (cmp(KEY(p), KEY(r)))
    {
      q = insert_by_key_xt(LLINK(r), p, cmp);
      if (q != Node::NullPtr)
        ++COUNT(r); 
    }
  else if (cmp(KEY(r), KEY(p)))
    {
      q = insert_by_key_xt(RLINK(r), p, cmp);
      if (q != Node::NullPtr)
        ++COUNT(r); 
    }
  // else return Node::NullPtr; is not needed

  return q;
}

    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_by_key_xt(Node *& r, Node * p, Compare && cmp = Compare()) 
      noexcept
{
  return insert_by_key_xt(r, p, cmp);
}

    template <class Node, class Compare> inline
Node * insert_dup_by_key_xt(Node *& r, Node * p, Compare & cmp) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  if (r == Node::NullPtr) 
    return r = p;

  Node * q;
  if (cmp(KEY(p), KEY(r)))
    q = insert_dup_by_key_xt(LLINK(r), p, cmp);
  else 
    q = insert_dup_by_key_xt(RLINK(r), p, cmp);

  ++COUNT(r); 

  return q;
}

    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_dup_by_key_xt(Node *& r, Node * p, Compare && cmp = Compare()) 
      noexcept
{
  return insert_by_key_xt(r, p, cmp);
}

    template <class Node, class Compare> inline
Node * search_or_insert_by_key_xt(Node *& r, Node * p, Compare & cmp) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  if (r == Node::NullPtr) 
    return r = p;

  Node * q;
  if (cmp(KEY(p), KEY(r)))
    {
      q = insert_by_key_xt(LLINK(r), p, cmp);
      if (q != Node::NullPtr)
        ++COUNT(r); 
    }
  else if (cmp(KEY(r), KEY(p)))
    {
      q = insert_by_key_xt(RLINK(r), p, cmp);
      if (q != Node::NullPtr)
        ++COUNT(r); 
    }
  else
    return r;

  return q;
}

template <class Node, 
	  class Compare = Aleph::less<typename Node::key_type>> inline
Node * search_or_insert_by_key_xt(Node *& r, Node * p, 
				  Compare && cmp = Compare()) noexcept
{
  return search_or_insert_by_key_xt(r, p, cmp);
}


    template <class Node, class Compare> static inline
bool __split_key_rec_xt(Node * root, const typename Node::key_type & key, 
			Node *& l, Node *& r, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    {
      l = r = Node::NullPtr;
      return true;
    }

  if (cmp(key, KEY(root)))
    {
      if (not __split_key_rec_xt(LLINK(root), key, l, LLINK(root), cmp)) 
        return false;

      r = root;
      COUNT(r) -= COUNT(l);
    }
  else if (cmp(KEY(root), key))
    {
      if (not __split_key_rec_xt(RLINK(root), key, RLINK(root), r, cmp)) 
        return false;

      l = root;
      COUNT(l) -= COUNT(r);
    }
  else
    return false;

  return true;
}

template <class Node, class Compare> inline
bool split_key_rec_xt(Node *& root, const typename Node::key_type & key, 
		      Node *& l, Node *& r, Compare & cmp) noexcept
{
  bool ret = __split_key_rec_xt(root, key, l, r, cmp);
  if (ret)
    root = Node::NullPtr;
  return ret;
}

template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
bool split_key_rec_xt(Node *& root, const typename Node::key_type & key, 
		      Node *& l, Node *& r, Compare && cmp = Compare()) noexcept
{
  return split_key_rec_xt(root, key, l, r, cmp);
}

   
    template <class Node, class Compare> static inline
void __split_key_dup_rec_xt(Node * root, const typename Node::key_type & key, 
			    Node *& l, Node *& r, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    {
      l = r = Node::NullPtr;
      return;
    }

  if (cmp(key, KEY(root)))
    {
      __split_key_dup_rec_xt(LLINK(root), key, l, LLINK(root), cmp);
      r = root;
      COUNT(r) -= COUNT(l);
    }
  else
    {
      __split_key_dup_rec_xt(RLINK(root), key, RLINK(root), r, cmp);
      l = root;
      COUNT(l) -= COUNT(r);
    }
}


 template <class Node, class Compare> inline
void split_key_dup_rec_xt(Node *& root, const typename Node::key_type & key, 
                          Node *& l, Node *& r, Compare & cmp) noexcept
{
  __split_key_dup_rec_xt<Node, Compare>(root, key, l, r, cmp);
  root = Node::NullPtr;
}

 template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
void split_key_dup_rec_xt(Node *& root, const typename Node::key_type & key, 
                          Node *& l, Node *& r, Compare && cmp = Compare())
   noexcept
{
  return split_key_dup_rec_xt(root, key, l, r, cmp);
}

    template <class Node, class Compare> inline
Node * insert_root_xt(Node *& root, Node * p, Compare & cmp) noexcept
{
  if (root == Node::NullPtr) 
    return p;

  if (not split_key_rec_xt(root, KEY(p), LLINK(p), RLINK(p), cmp))
    return Node::NullPtr;

  COUNT(p) = COUNT(LLINK(p)) + COUNT(RLINK(p)) + 1;
  root = p;

  return p; 
}

    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_root_xt(Node *& root, Node * p, Compare && cmp = Compare()) 
      noexcept
{
  return insert_root_xt(root, p, cmp);
}

    template <class Node, class Compare> inline
Node * insert_dup_root_xt(Node *& root, Node * p, Compare & cmp) noexcept
{
  if (root == Node::NullPtr) 
    return p;

  split_key_dup_rec_xt(root, KEY(p), LLINK(p), RLINK(p), cmp);
  COUNT(p) = COUNT(LLINK(p)) + COUNT(RLINK(p)) + 1;

  return root = p;
}

template <class Node, 
	  class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_dup_root_xt(Node *& root, Node * p, Compare && cmp = Compare()) 
   noexcept
{
  return insert_dup_root_xt(root, p, cmp);
}

   
    template <class Node> static inline
void __split_pos_rec(Node * r, size_t i, Node *& ts, Node *& tg) noexcept
{
  if (i == COUNT(LLINK(r)))
    {
      ts = LLINK(r);
      tg = r;
      LLINK(tg) = Node::NullPtr;
      COUNT(tg) -= COUNT(ts);
      return;
    }

  if (i < COUNT(LLINK(r)))
    {
      __split_pos_rec(LLINK(r), i, ts, LLINK(r));
      tg = r;
      COUNT(r) -= COUNT(ts);
    }
  else
    {
      __split_pos_rec(RLINK(r), i - (COUNT(LLINK(r)) + 1), RLINK(r), tg);
      ts = r;
      COUNT(r) -= COUNT(tg);
    }
}


template <class Node> inline
void split_pos_rec(Node *& r, const size_t i, Node *& ts, Node *& tg)
{
  if (i > COUNT(r))
    throw std::out_of_range("infix position out of range");

  if (i == COUNT(r)) // ¿Es la última posición?
    {
      ts = r;
      r = tg = Node::NullPtr;
      return;
    }

  __split_pos_rec<Node>(r, i, ts, tg);

  r = Node::NullPtr;
}

    template <class Node>  inline
void insert_by_pos_xt(Node *& r, Node * p, size_t pos) noexcept
{
  assert(COUNT(Node::NullPtr) == 0);

  split_pos_rec(r, pos, LLINK(p), RLINK(p));
  COUNT(p) = COUNT(LLINK(p)) + 1 + COUNT(RLINK(p));
  r = p;
}

    template <class Node> inline 
Node * join_exclusive_xt(Node *& ts, Node *& tg) noexcept
{
  if (ts == Node::NullPtr) 
    return tg;

  if (tg == Node::NullPtr) 
    return ts;

  LLINK(tg) = join_exclusive_xt(RLINK(ts), LLINK(tg));
  RLINK(ts) = tg;

      // actualizar contadores
  COUNT(tg) = COUNT(LLINK(tg)) + 1 + COUNT(RLINK(tg)); 
  COUNT(ts) = COUNT(LLINK(ts)) + 1 + COUNT(RLINK(ts));

  Node * ret_val = ts;
  ts = tg = Node::NullPtr; // deben quedar vacíos después del join

  return ret_val;
}  


    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline 
Node * remove_by_key_xt(Node *& root, const typename Node::key_type & key,
			Compare & cmp) noexcept
{
  assert(root != Node::NullPtr);

  if (root == Node::NullPtr) 
    return Node::NullPtr; 

  Node * ret_val = Node::NullPtr; 
  if (cmp(key, KEY(root)))
    {
      ret_val = remove_by_key_xt(LLINK(root), key, cmp);
      if (ret_val != Node::NullPtr)
        --COUNT(root);

      return ret_val;
    }
  else if (cmp(KEY(root), key))
    {
      ret_val = remove_by_key_xt(RLINK(root), key, cmp);
      if (ret_val != Node::NullPtr) 
        --COUNT(root);

      return ret_val;
    }

  ret_val = root;
  root = join_exclusive_xt(LLINK(root), RLINK(root));

  ret_val->reset();

  return ret_val;
}

template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline Node * remove_by_key_xt(Node *& root, 
			       const typename Node::key_type & key,
			       Compare && cmp = Compare()) noexcept
{
  return remove_by_key_xt(root, key, cmp);
}


    template <class Node> static inline
Node * __remove_by_pos_xt(Node *& root, size_t pos) noexcept
{
  if (COUNT(LLINK(root)) == pos) // found position?
    {
      Node * ret_val = root;
      root = join_exclusive_xt(LLINK(root), RLINK(root));

      ret_val->reset();

      return ret_val;
    }

  Node * ret_val; 
  if (pos < COUNT(LLINK(root)))
    ret_val = __remove_by_pos_xt(LLINK(root), pos);
  else
    ret_val = __remove_by_pos_xt(RLINK(root), pos - (COUNT(LLINK(root)) + 1));

  if (ret_val != Node::NullPtr) // removal was done?
    --COUNT(root);

  return ret_val;
}

    template <class Node> inline
Node * remove_by_pos_xt(Node *& root, size_t pos)
{
  if (pos >= COUNT(root))
    throw std::out_of_range("infix position out of range");

  return __remove_by_pos_xt(root, pos);
}  

    template <class Node> inline
bool check_rank_tree(Node * root) noexcept
{
  if (root == Node::NullPtr) 
    return true;

  if (COUNT(LLINK(root)) + COUNT(RLINK(root)) + 1 != COUNT(root)) 
    return false;

  return check_rank_tree(LLINK(root)) and check_rank_tree(RLINK(root));
}

    template <class Node> inline 
Node * rotate_to_right_xt(Node* p) noexcept
{
  assert(p != Node::NullPtr);
  assert(COUNT(LLINK(p)) + 1 + COUNT(RLINK(p)) == COUNT(p));

  Node * q = LLINK(p);
  LLINK(p) = RLINK(q);
  RLINK(q) = p;
  COUNT(p) -= 1 + COUNT(LLINK(q));
  COUNT(q) += 1 + COUNT(RLINK(p));
  assert(COUNT(LLINK(q)) + 1 + COUNT(RLINK(q)) == COUNT(q));
  return q;           
} 

    template <class Node> inline
Node * rotate_to_left_xt(Node* p) noexcept
{
  assert(p != Node::NullPtr);
  assert(COUNT(LLINK(p)) + 1 + COUNT(RLINK(p)) == COUNT(p));

  Node * q = RLINK(p);
  RLINK(p) = LLINK(q);
  LLINK(q) = p;
  COUNT(p) -= 1 + COUNT(RLINK(q));
  COUNT(q) += 1 + COUNT(LLINK(p));
  assert(COUNT(LLINK(q)) + 1 + COUNT(RLINK(q)) == COUNT(q));
  return q;                  
} 


    template <class Node, class Key, class Compare> inline
Node * search_or_insert_root_rec_xt(Node * root, Node * p, Compare & cmp) 
      noexcept
{
  if (root == Node::NullPtr)
    return p; // insertion in empty tree

  if (cmp(KEY(p), KEY(root)))
    { // insert in left subtree 
      Node * left_branch = search_or_insert_root_rec_xt(LLINK(root), p, cmp);
      if (left_branch == p) // p was inserted?
        {
          ++COUNT(root);
          LLINK(root) = left_branch;
          root        = rotate_to_right_xt(root);
          return p;
        }

        return left_branch;
    }
  else if (cmp(KEY(root), KEY(p)))
    { // insert in right subtree 
      Node * right_branch = search_or_insert_root_rec_xt(RLINK(root), p, cmp);
      if (right_branch == p) // p was inserted?
        {
          ++COUNT(root);
          RLINK(root) = right_branch;
          root        = rotate_to_left_xt(root);
          return p;
        }

      return right_branch;
    }

  return root;
}

template <class Node, class Key, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * search_or_insert_root_rec_xt(Node * root, Node * p,
				    Compare && cmp = Compare()) noexcept
{
  return search_or_insert_root_rec_xt(root, p, cmp);
}


} // end namespace Aleph

# endif // TPL_BINNODEXT_H

