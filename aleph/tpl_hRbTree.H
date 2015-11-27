
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
	   Hybrid Top down and botton up red black trees 
*/

# ifndef TPL_HTDRBTREE_H
# define TPL_HTDRBTREE_H

# ifdef DEBUG
# include <math.h>
# endif
# include <tpl_binNode.H>
# include <tpl_binNodeUtils.H>
# include <tpl_arrayStack.H>
# include <rbNode.H>

# define COLOR(p)     (p->getColor())

using namespace BinNode_Utils;

    template <class Key>
class HtdRbTree 
{
public:

  typedef unsigned char Color;

private:

  /*
    Node definition of a red black tree.

    It's a binary node (BinNode<Key>) with an attribute called color.
    It uses a *BLACK* sentinel node for null nodes (Node::NullPtr)
  */
  friend class _Node : public BinNode<Key>
  {
  private:
    friend class HtdRbTree<Key>;
  
    Color  color; // color is either BLACK or RED 

    static const _Node<Key> nodeSentinel;

    _Node(EmptyCtor)
      : BinNode<Key>(emptyCtor), color(BLACK)
      {
	getL() = _Node<Key>::Node::NullPtr; 
	getR() = _Node<Key>::Node::NullPtr; 
      }   

  public:

    static const unsigned int MaxHeight = 128; 
    static _Node<Key> * const Node::NullPtr   
      = const_cast<_Node<Key>*>(&nodeSentinel);

    _Node(Color c = RED)  
      : BinNode<Key>(emptyCtor), color(c) 
      {
	getL() = _Node<Key>::Node::NullPtr; 
	getR() = _Node<Key>::Node::NullPtr; 
      }

    _Node(const Key& _key)   
      : BinNode<Key>(emptyCtor), color(RED) 
      {
	get_key() = _key;
	getL()   = _Node<Key>::Node::NullPtr; 
	getR()   = _Node<Key>::Node::NullPtr; 
      }

    Color& getColor() { return color; }

    Color getColor() const { return color; }
  }; // end of _Node

public:

  typedef MetaBinNode<_Node, Key> Node;

private:

  static Node * const Node::NullPtr = Node::Node::NullPtr;

  Node                               headNode; 

  Node                               headParent;

  Node                               headGrandParent;

  Node                              *head;

  Node                              *fHead;

  Node                              *ffHead;

  Node                             *&root;

  FixedStack<Node*> path(Node::MaxHeight);

# ifdef DEBUG
  unsigned int n;
# endif /* DEBUG */

  Node* getSibling(Node *p, Node *fp)
    {
      assert(LLINK(fp) == p || RLINK(fp) == p);
      return LLINK(fp) == p ? RLINK(fp) : LLINK(fp);
    }


  /* Routines exclusively used by insertion procedure */

  void restoreRedCondition(Node *p, 
			   Node *&fp, 
			   Node *ffp,
			   Node *fffp)
    {
      /* Sanity checks */
      assert(LLINK(fp) == p || RLINK(fp) == p);
      assert(COLOR(fp) == RED);
      assert(COLOR(p) == RED);

      if (fp == root)
	{
	  COLOR(fp) = BLACK;
	  return;
	}

      assert(LLINK(ffp) == fp || RLINK(ffp) == fp);
      assert(COLOR(ffp) == BLACK);
      assert(LLINK(fffp) == ffp || RLINK(fffp) == ffp);

      COLOR(ffp) = RED;

      if (LLINK(fp) == p && LLINK(ffp) == fp)
	{
	  COLOR(fp) = BLACK;
	  rotate_to_right(ffp, fffp);
	}
      else if (RLINK(fp) == p && RLINK(ffp) == fp)
	{
	  COLOR(fp) = BLACK;
	  rotate_to_left(ffp, fffp);
	}
      else 
	{
	  COLOR(p) = BLACK;
	  if (RLINK(fp) == p)
	    {
	      rotate_to_left(fp, ffp);
	      rotate_to_right(ffp, fffp);
	    }
	  else
	    {
	      rotate_to_right(fp, ffp);
	      rotate_to_left(ffp, fffp);
	    }
	  fp = fffp;
	}
    }
  
  static void flipColors(Node* p)
    {
      assert(p != Node::NullPtr);
      assert(COLOR(p) == BLACK);
      assert(COLOR(LLINK(p)) == RED && COLOR(RLINK(p)) == RED);

      COLOR(p)                          = RED;
      COLOR(LLINK(p)) = COLOR(RLINK(p)) = BLACK;
    }

  Node* searchFlipColorsAndInsert(Node *q)
    {
      assert(q != Node::NullPtr);
      assert(root != Node::NullPtr);
      assert(COLOR(q) == RED);
      assert(LLINK(q) == Node::NullPtr && RLINK(q) == Node::NullPtr);

      Node *p    = root;     // Current node
      Node *fp   = head;     // p's parent
      Node *ffp  = fHead;    // p's grand parent
      Node *fffp = ffHead;  // p's great grand parent
      Node *nextNode;

      while (1)
	{
	  if (KEY(q) == KEY(p))
	    return nullptr; /* Duplicated key, insertion is not possible */

	  if (COLOR(p) == BLACK && COLOR(LLINK(p)) == RED 
	      && COLOR(RLINK(p)) == RED)
	    {
	      flipColors(p); // Rends p red
	      if (COLOR(fp) == RED) // violation of red condition
		{
		  assert(fffp != Node::NullPtr);
		  restoreRedCondition(p, fp, ffp, fffp);
		}
	    }

	  if (KEY(q) < KEY(p))
	    { 
	      if (LLINK(p) == Node::NullPtr)
		break;
	      nextNode = LLINK(p);
	    }
	  else 
	    {
	      if (RLINK(p) == Node::NullPtr)
		break;
	      nextNode = RLINK(p);
	    }

	  fffp = ffp;
	  ffp  = fp;
	  fp   = p;
	  p    = nextNode;
	}

#   ifdef DEBUG
      n++;
#   endif

      /* Insertion act */
      if (KEY(q) < KEY(p))
	LLINK(p) = q;

      else 
	RLINK(p) = q;

      if (COLOR(p) == RED) // Violation of red condition
	restoreRedCondition(q, p, fp, ffp);

      return q;
    }
	  
  /* The following routines are exclusively used by remove procedure */

  Node* searchAndBuildPath(const Key& key)       
    {
      assert(root != Node::NullPtr);
      
      Node *p = root;
      path.push(head);
    
      while (1) 
        {
          path.push(p);
 
          if (key == KEY(p))
            {
              assert(path.getLen() - 1.0 <= 2*log(n + 1)/log(2));
              return p;
            }
           
          if (key < KEY(p))
            {
              if (LLINK(p) == Node::NullPtr)
                {
                  assert(path.getLen() - 1.0 <= 2*log(n + 1)/log(2));
                  return p;
                }
 
              p = LLINK(p);
              continue;
            }
 
          if (RLINK(p) == Node::NullPtr)
            {
              assert(path.getLen() - 1.0 <= 2*log(n + 1)/log(2));
              return p;
            }

          p = RLINK(p);
        }
    }

  void findSuccAndSwap(Node *p, Node *&fp)
    {
      assert(p != Node::NullPtr);
      assert(RLINK(p) != Node::NullPtr);
      assert(fp != Node::NullPtr);
      assert(LLINK(fp) == p || RLINK(fp) == p);

      /*
        We save a reference to current stack content because p will be
        swapped and p's position in stack should be occupied by
        successor of p
      */
      Node *&refToSearchPath = path.lookAt();

      assert(refToSearchPath == p);

      /* Find succesor while updating searchPath */
      Node *fSucc = p;        // succesor's parent
      Node *succ  = RLINK(p); // Searching starts from p's right child 

      path.push(succ);
      while (LLINK(succ) != Node::NullPtr)  // go down to leftmost
        {
          fSucc = succ;
          succ  = LLINK(succ);
          path.push(succ);
        }
      assert(path.getLen() - 1.0 <= 2*log(n + 1)/log(2));

      /* 
         update old stack entry occupied by p These operations are
         equivalents to swap old top with current top 
      */  
      refToSearchPath     = succ; 
      path.lookAt() = p; 

      /* Setting of parent of p to its new child (succ) */ 
      if (LLINK(fp) == p)
        LLINK(fp) = succ;
      else
        RLINK(fp) = succ;
    
      /* Swaps left branches */
      LLINK(succ) = LLINK(p);
      LLINK(p)    = Node::NullPtr; 

      /* For right branches there are two cases */
      if (RLINK(p) == succ) 
        { /* successor is just right's child of p */   
          RLINK(p)    = RLINK(succ); 
          RLINK(succ) = p;
          fp          = succ;
        }
      else
        { /* 
             successor is the leftmost nodo descending from right child of p 
          */
          Node *succr  = RLINK(succ); 
          RLINK(succ)  = RLINK(p);
          LLINK(fSucc) = p;
          RLINK(p)     = succr;
          fp           = fSucc;
        }

      // Swap of colors
      Color tmp   = COLOR(succ);
      COLOR(succ) = COLOR(p); 
      COLOR(p)    = tmp;
    }


  void balanceDownAndColor(Node *p, Node *&fp, Node *&sp)
    {
      assert(LLINK(fp) == p || RLINK(fp) == p);
      assert(LLINK(fp) == sp || RLINK(fp) == sp);
      assert(COLOR(fp) == BLACK);
      assert(COLOR(sp) == RED);
      assert(COLOR(p) == BLACK);
      assert(!path.is_empty());

      /* needed by rotation for links' update. We save a reference to search
       stack because stack's head will cahnge after rotation */
      Node *&ffp = path.lookAt(); 

      assert(LLINK(ffp) == fp || RLINK(ffp) == fp);

      /* balancing down of p, update of out parameters and update of stack
         entry corresponding to fp */
      if (LLINK(fp) == p)
        {
          sp  = LLINK(sp);
          ffp = rotate_to_left(fp, ffp);
        }
      else
        {
          sp  = RLINK(sp);
          ffp = rotate_to_right(fp, ffp);
        }

      assert(LLINK(fp) == sp || RLINK(fp) == sp);
      assert(COLOR(ffp) == RED);

      /* coloring  for ensuring to apply sibling black cases */
      COLOR(ffp) = BLACK;
      COLOR(fp)  = RED;
    }

  void rotateNephewAndColor(Node *fp, Node *sp, Node* np)
    {
      assert(LLINK(fp) == sp || RLINK(fp) == sp);
      assert(LLINK(sp) == np || RLINK(sp) == np);
      assert(COLOR(sp) == BLACK);
      assert(COLOR(np) == RED);
      assert(!path.is_empty());
      
      Node *ffp = path.lookAt();

      assert(LLINK(ffp) == fp || RLINK(ffp) == fp);

      /* rotation for downing low fp */
      if (RLINK(sp) == np)
        rotate_to_left(fp, ffp);
      else
        rotate_to_right(fp, ffp);

      /* coloring for fixing up red black conditions */
      COLOR(sp) = COLOR(fp);
      COLOR(fp) = BLACK;
      COLOR(np) = BLACK;
    }
     
  void doubleRotateNephewAndColor(Node *fp, Node *sp, Node *snp)
    {
      assert(LLINK(fp) == sp || RLINK(fp) == sp);
      assert(LLINK(sp) == snp || RLINK(sp) == snp);      
      assert(COLOR(sp) == BLACK);
      assert(COLOR(snp) == RED);
      assert(!path.is_empty());

      Node *ffp = path.lookAt();

      assert(LLINK(ffp) == fp || RLINK(ffp) == fp);

      /* double rotation for raising up of snp. snp becomes the new root of
         sub tree fp  */
      if (LLINK(sp) == snp)
        {
          rotate_to_right(sp, fp);
          rotate_to_left(fp, ffp);
        }
      else
        {
          rotate_to_left(sp, fp);
          rotate_to_right(fp, ffp);
        }

      /* coloring for restoring all red black tree conditions */
      COLOR(snp) = COLOR(fp);
      COLOR(fp)  = BLACK;
    }

  static void colorSiblingAsRed(Node *sp)
    {
      assert(COLOR(sp) == BLACK);
      COLOR(sp) = RED;
    }

  static void colorParentAndSibling(Node *fp, Node *sp)
    {
      assert(LLINK(fp) == sp || RLINK(fp) == sp);
      assert(COLOR(fp) == RED);
      assert(COLOR(sp) == BLACK);

      COLOR(fp) = BLACK;
      COLOR(sp) = RED;
    }

void removeAndFixBlackCondition(Node* q)
    {
      assert(path.lookAt() == q);

      /* look at second item after stack's head. We cannot it pop because
         succesor finding would require stack pushing */
      Node* fq = path.lookAt(1); 
      Node *p; /* Saves the new child of fp after p has been deleted, this
                  node can be a violating black condition black node */

      assert(fq != Node::NullPtr);
      assert( LLINK(fq) == q || RLINK(fq) == q);

      /* Deletion step: by pass if there is a Node::NullPtr link or swap
         with successor */
      while (1)
        {
          if (LLINK(q) == Node::NullPtr) // by pass to the left side
            {
              if (LLINK(fq) == q)
                p = LLINK(fq) = RLINK(q);
              else
                p = RLINK(fq) = RLINK(q);
              break;
            }

          if (RLINK(q) == Node::NullPtr) // by pass to the right side
            {
              if (LLINK(fq) == q)
                p = LLINK(fq) = LLINK(q);
              else
                p = RLINK(fq) = LLINK(q);
              break;
            }
          
          findSuccAndSwap(q, fq);
        }

      /* if color of deleted node is red, then all red black conditions are
         met and adjust is not necessary */
      if (COLOR(q) == RED)
        { 
          assert(COLOR(p) == BLACK);
          path.empty();
          return; 
        }

      /* if color of p is black, then it misses a black node and four condition
         is violated. However, if p's child is red we can recoloring it black
         for restoring the missing black node */
      if (COLOR(p) == RED)
        { 
          COLOR(p) = BLACK;
          path.empty();
          return;
        }

      /* Bad luck we must do recoloring and/or balancing for restoring the
         four condition */
      Node *sp;       // p's sibling
      Node *np, *snp; // p's nephew and nephewg's sibling
      Node *fp = fq;  // we process in function of p
      
      path.pop(2); // pops deleted node and fp

      /* we examine p and we restore red black properties */
      while (1)
        {
          if (p == root)
            break;

          sp = getSibling(p, fp);

          /* if sibling is red, we rotate down for assuring that p' sibling
             to be black */
          if (COLOR(sp) == RED)
            balanceDownAndColor(p, fp, sp);

          assert(COLOR(sp) == BLACK); 

          /* Compute nephews */
          if (LLINK(fp) == p)
            {
              np  = RLINK(sp);
              snp = LLINK(sp);
            }
          else
            {
              np  = LLINK(sp);
              snp = RLINK(sp);
            }

          if (COLOR(np) == RED)
            {
              rotateNephewAndColor(fp, sp, np);
              break;
            }

          if (COLOR(snp) == RED)
            {
              doubleRotateNephewAndColor(fp, sp, snp);
              break;
            }
          
          if (COLOR(fp) == RED)
            {
              colorParentAndSibling(fp, sp);
              break;
            }

          /* color and restart process with fp */
          colorSiblingAsRed(sp);
          p  = fp;
          fp = path.pop(); // colorSiblingAsRed(sp) does not pop it
        }
      path.empty();
    }

public:

  HtdRbTree()
    : head(&headNode),
      fHead(&headParent),
      ffHead(&headGrandParent),
      root(headNode.getR())
    {
      RLINK(fHead)   = head;
      RLINK(ffHead)  = fHead;
      COLOR(Node::NullPtr) = BLACK;
      COLOR(head)    = BLACK;
      COLOR(fHead)   = BLACK;
      COLOR(ffHead)  = BLACK;

#   ifdef DEBUG
      n      = 0;
#   endif /* DEBUG */
    }

  virtual ~HtdRbTree()
    {
      // Empty
    }

  Node* insert(Node *p)
    {
      assert(p != Node::NullPtr);
      assert(COLOR(p) == RED);
      assert(LLINK(p) == Node::NullPtr && RLINK(p) == Node::NullPtr);

      if (root == Node::NullPtr)
        {
          root = p;
#       ifdef DEBUG
	  n++;
#       endif
          return p;
        }

      return searchFlipColorsAndInsert(p);
    }

  Node* search(const Key& key)
    {
      Node* retVal = searchInBinTree(root, key);

      return retVal == Node::NullPtr ? nullptr : retVal;
    }

  /* 
     Remove a key from a HtdRbTree.

     Searches a key in a Rbltree and remove the containing the key if this
     is found.

     @return a pointer to node containing the removed key.
     @param key to search
   */
  Node* remove(const Key& key)
     {
      if (root == Node::NullPtr)
        return nullptr;
      
      Node *p = searchAndBuildPath(key);

      if (KEY(p) != key)
        {
          path.empty();
          return nullptr;
        }

      removeAndFixBlackCondition(p);

#   ifdef DEBUG
      n++;
#   endif

      return p;
    }


  Node*& getRoot() { return root; }

private:

  static void blackHeight(Node *p, int &max, int bh = 0)
    {
      if (COLOR(p) == BLACK)
	bh++; // Another seen black node

      /* if a leaf is reached, we must verify max with current bh (number of
       visited black nodes */
      if (LLINK(p) == Node::NullPtr && RLINK(p) == Node::NullPtr)
	{
	  if (max == -1) // This is onbly for the first leaf reached
	    max = bh;
	  assert(bh == max);
	  return;
	}

      if (LLINK(p) != Node::NullPtr) /* continue counting in the left side */
	blackHeight(LLINK(p), max, bh); 

      if (RLINK(p) != Node::NullPtr) /* continue counting in the right side */     
	blackHeight(RLINK(p), max, bh);
    }

  static void testNode(Node* p)
    {
      /* verify red condition */
      COND_assert(COLOR(p) == RED,
		  COLOR(LLINK(p)) == BLACK && COLOR(RLINK(p)) == BLACK);

      int max = -1;
      int bh  = 0;

      blackHeight(p, max, bh);
    }

public:

  void verifyRedBlack()
    {
      /* traverse all tree and verify black height for each visited node */
      preOrderRec(root, testNode);
    }
};


template <class Key>
const HtdRbTree<Key>::_Node HtdRbTree<Key>::_Node::nodeSentinel;


# undef COLOR
# undef RED
# undef BLACK

#endif /* TPL_HTDRBTREE_H */




