
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


# ifndef TPL_BINHEAP_H
# define TPL_BINHEAP_H

# include <ahDefs.H>
# include <ahUtils.H>
# include <ahFunction.H>
# include <htlist.H>
# include <tpl_arrayStack.H>
# include <tpl_binNode.H>
# include <tpl_dynListQueue.H>

using namespace Aleph;

namespace Aleph {

class BinHeapNode_Data
{
  struct Control_Fields // Definición de banderas de control
  {
    int is_leaf : 4; // true si el nodo es hoja
    int is_left : 4; // true si el nodo es hijo izquierdo
  };
  

  BinHeapNode_Data * pLink; // puntero al padre
  Control_Fields control_fields;

public:

  BinHeapNode_Data() noexcept : pLink(nullptr)
  { 
    control_fields.is_leaf = true;
    control_fields.is_left = true;
  }

  BinHeapNode_Data *& getU() noexcept { return pLink; }

  Control_Fields & get_control_fields() noexcept { return control_fields; }

  void reset() noexcept
  {
    control_fields.is_leaf = true;
    control_fields.is_left = true;
  }
};

DECLARE_BINNODE(BinHeapNode, 64, BinHeapNode_Data);

# define PREV(p)      (p->getL())
# define NEXT(p)      (p->getR())
# define ULINK(p)     reinterpret_cast<Node*&>((p)->getU())
# define IS_LEAF(p)   ((p)->get_control_fields().is_leaf)
# define IS_LEFT(p)   ((p)->get_control_fields().is_left)
# define CTRL_BITS(p) ((p)->get_control_fields())

  template <template <class> class NodeType, typename Key, 
            class Compare = Aleph::less<Key>>
class GenBinHeap
{
protected:

  Compare & cmp;

public:

  using Node = NodeType<Key>;

  Compare & key_comp() noexcept { return cmp; }

  Compare & get_compare() noexcept { return cmp; }

private:

    Node     head_node;
    Node *   head;
    Node *&  root;
    Node *   last;
    size_t   num_nodes;

public:

  void swap(GenBinHeap & h) noexcept
  {
    std::swap(root, h.root);
    std::swap(last, h.last);
    std::swap(num_nodes, h.num_nodes);
    std::swap(cmp, h.cmp);
  }

private:

  static bool is_in_list(Node * p) noexcept
  {
    if (IS_LEAF(p)) 
      return true;

    return ULINK(LLINK(p)) == RLINK(LLINK(p));
  }

  static bool has_sibling(Node * p) noexcept
  {
    return ULINK(p) != RLINK(p);
  }

  void swap_with_parent(Node * p) noexcept
  {
    assert(num_nodes >= 2);
    assert(p != root);

    Node *pp = ULINK(p); // padre de p

    const bool p_has_sibling = has_sibling(p); 
    const bool p_is_in_list  = is_in_list(p);  // p está en último nivel?
    const bool pp_is_in_list = is_in_list(pp); // p == last & LLINK(pp) == last?
    const bool p_has_child   = not IS_LEAF(p); // p tiene hijos?

    std::swap(CTRL_BITS(pp), CTRL_BITS(p)); // swap de banderas

    if (pp == root) 
      root = p;

    Node *ppp = ULINK(pp); // abuelo de p; padre de pp

    ULINK(pp) = p;  // Actualizar ULINK
    ULINK(p) = ppp;

    if (LLINK(ppp) == pp)
      LLINK(ppp) = p;
    else
      RLINK(ppp) = p;

    Node *sp = nullptr;  // guarda hermano de p
    if (p_has_sibling) 
      {
        sp = p == LLINK(pp) ? RLINK(pp) : LLINK(pp); // hermano de p
        assert(ULINK(sp) == pp);
        ULINK(sp) = p;
      }

    if (p == last) // ¿actualizar last?
      last = pp;
    
    if (num_nodes == 2) 
      return;

    Node *lcp = LLINK(p); // respaldo de hijos de p
    Node *rcp = RLINK(p);
   
    if (num_nodes == 3)
      {
        if (RLINK(pp) == p)
          {
            LLINK(lcp) = RLINK(lcp) = pp;
            RLINK(pp)  = lcp;
            RLINK(p)   = pp;
          }
        else
          {
            LLINK(rcp) = RLINK(rcp) = pp;
            LLINK(pp)  = rcp;
            LLINK(p)   = pp;
          }

        return;
      }

    if (not p_is_in_list)
      {
        ULINK(lcp) = ULINK(rcp) = pp;

        if (LLINK(pp) == p)
          {
            assert(RLINK(pp) == sp);
            LLINK(p) = pp;
            RLINK(p) = RLINK(pp);
          }
        else
          {
            assert(LLINK(pp) == sp);
            RLINK(p) = pp;
            LLINK(p) = LLINK(pp);
          }

        LLINK(pp) = lcp;
        RLINK(pp) = rcp;

        return;
      }
      
    if (not pp_is_in_list)
      {
        if (p_has_child)
          ULINK(LLINK(p)) = pp;

        RLINK(lcp) = LLINK(rcp) = pp;

        if (LLINK(pp) == p)
          {
            assert(RLINK(pp) == sp);
            LLINK(p) = pp;
            RLINK(p) = RLINK(pp);
          }
        else
          {
            assert(LLINK(pp) == sp);
            RLINK(p)  = pp;
            LLINK(p)  = LLINK(pp);
          }

        LLINK(pp) = lcp;
        RLINK(pp) = rcp;

        return;
      }
      
    RLINK(lcp)       = pp;
    LLINK(RLINK(pp)) = p;
    LLINK(pp)        = lcp;
    RLINK(p)         = RLINK(pp);
    RLINK(pp)        = p;
    LLINK(p)         = pp;
  }

  virtual void sift_up(Node * p) noexcept
  {
    while (p != root and cmp (KEY(p), KEY(ULINK(p))))
      swap_with_parent(p);
  }

  virtual void sift_down(Node * p) noexcept
  {
    while (not IS_LEAF(p))
      {
        Node * cp = LLINK(p);  // guarda el menor hijo de p
        if (has_sibling(cp))
          if (cmp (KEY(RLINK(p)), KEY(LLINK(p))))
            cp = RLINK(p);

        if (cmp (KEY(p), KEY(cp))) 
          return;

        swap_with_parent(cp);
      }
  }

  void swap_root_with_last() noexcept
  {
    assert(num_nodes > 1);
    assert(ULINK(root) == head);
    assert(not IS_LEAF(root));
    assert(IS_LEAF(last));

    if (num_nodes > 3) // caso general
      {
        Node * lRoot     = LLINK(root);
        Node * rRoot     = RLINK(root);
        Node * f_last    = ULINK(last);
        Node * prev_last = LLINK(last);
        Node * next_last = RLINK(last);

        if (LLINK(f_last) == last)
          LLINK(f_last) = root;
        else
          RLINK(f_last) = root;       
          
        if (RLINK(root) != last)
          std::swap(ULINK(root), ULINK(last));
        else
          {
            ULINK(root) = last;
            ULINK(root) = head;
          }

        std::swap(LLINK(root), LLINK(last));
        std::swap(RLINK(root), RLINK(last));
        
        ULINK(lRoot) = ULINK(rRoot) = last;

        LLINK(last) = lRoot;
        RLINK(last) = rRoot;

        PREV(root) = prev_last;
        NEXT(root) = next_last;

        NEXT(prev_last) = PREV(next_last) = root;
      }      
    else if (num_nodes == 3) // caso particular con 3 nodos
      {
        assert(RLINK(root) == last);
        assert(LLINK(last) == LLINK(root) and RLINK(last) == LLINK(root));

        ULINK(last) = ULINK(root);
        ULINK(root) = last;

        Node *s_last  = LLINK(last);
        ULINK(s_last) = last;

        LLINK(last) = s_last;
        RLINK(last) = root;

        LLINK(root) = RLINK(root) = s_last;
        RLINK(s_last) = LLINK(s_last) = root;
      }
    else // casos particulares con num_nodes < 3
      {
        assert(LLINK(root) == last);

        ULINK(last) = ULINK(root);
        ULINK(root) = last;
        RLINK(last) = LLINK(last) = root;
        RLINK(root) = LLINK(root) = last;
      }

    std::swap(CTRL_BITS(root), CTRL_BITS(last));
    std::swap(root, last);
  }

  Node * remove_last() noexcept
  {
    assert(last != root and num_nodes > 0);
    assert(IS_LEAF(last));

    Node * ret_val  = last;
    Node * pp       = ULINK(last);
    Node * new_last = LLINK(last);

    if (IS_LEFT(last))
      {
        IS_LEAF(pp) = true;
        LLINK(pp)   = new_last;
      }
    else
      {
        RLINK(pp)          = RLINK(last);
        LLINK(RLINK(last)) = pp;
      }

    RLINK(LLINK(last)) = pp;
    last = new_last;
    num_nodes--;
    ret_val->reset();

    return ret_val;
  }

  void replace_node(Node * node, Node * new_node) noexcept
  {
    assert(node != new_node);
    assert(node != last);

        // guardar los parientes inmediatos de node
    Node * parent = ULINK(node);
    Node * left_child  = LLINK(node);
    Node * right_child = RLINK(node);
      
        // actualizar los punteros pertenecientes a new_node
    ULINK(new_node) = parent;
    LLINK(new_node) = left_child;
    RLINK(new_node) = right_child;

        // actualizar padre
    if (IS_LEFT(node))
      {
        assert(LLINK(parent) == node);
        LLINK(parent) = new_node;
      }
    else 
      {
        assert(RLINK(parent) == node);
        RLINK(parent) = new_node;
      }

        // actualizar hijos
    if (IS_LEAF(node))
      {
        RLINK(left_child)  = new_node;
        LLINK(right_child) = new_node;
      }
    else 
      {
        ULINK(left_child) = new_node;

        if (ULINK(right_child) == node) // node pudiera tener sólo un hijo
          ULINK(right_child) = new_node;
        else
          {
            assert(left_child == last);
            RLINK(left_child)  = new_node;
            LLINK(right_child) = new_node;
          }
      }
      
    CTRL_BITS(new_node) = CTRL_BITS(node);
  }

  static void __postorder_delete(Node * p, Node * incomplete_node) noexcept
  {
    if (IS_LEAF(p))
      {
        delete p;
        return;
      }

    __postorder_delete(LLINK(p), incomplete_node);

    if (p != incomplete_node)
      __postorder_delete(RLINK(p), incomplete_node); 

    delete p;
  }

public:

  Node * getRoot() noexcept { return root; }

  Node * getRoot() const noexcept { return const_cast<Node*>(root); }

private:

  template <class Operation> static
  void __for_each_in_preorder(Node * p, Operation & operation) 
  {
    if (p == nullptr)
      return;

    operation(p);
    __for_each_in_preorder(advance_left(p), operation);
    __for_each_in_preorder(advance_right(p), operation);
  }

  template <class Operation> static
  void __for_each_in_inorder(Node * p, Operation & operation) 
  {
    if (p == nullptr)
      return;

    __for_each_in_inorder(advance_left(p), operation);
    operation(p);
    __for_each_in_inorder(advance_right(p), operation);
  }

public:

  template <class Operation>
  void for_each_in_preorder(Operation & operation) const
  {
    return __for_each_in_preorder<Operation>(getRoot(), operation);
  }

  template <class Operation>
  void for_each_in_preorder(Operation && operation = Operation()) const
  {
    return for_each_in_preorder(operation);
  }  

  template <class Operation>
  void for_each_in_inorder(Operation & operation) const
  {
    return __for_each_in_inorder<Operation>(getRoot(), operation);
  }

  template <class Operation>
  void for_each_in_inorder(Operation && operation = Operation()) const
  {
    return for_each_in_inorder(operation);
  }  

private:

  template <class Op>
  bool __level_traverse(Node * root, Op & operation) const
  {
    if (root == nullptr)
      return true;

    DynListQueue<Node*> queue; 
    queue.put(root);

    while (not queue.is_empty())
      {
	Node * p = queue.get();

	if (not operation(p))
	  return false;

	Node * c = advance_left(p);
	if (c == nullptr)
	  continue;

	queue.put(c);

	c = advance_right(p);
	if (c != nullptr)
	  queue.put(c);
      }

    return true;
  }

public:

  template <class Op>
  bool level_traverse(Op & operation) const
  {
    return __level_traverse(getRoot(), operation);
  }

  template <class Op>
  bool level_traverse(Op && operation = Op())  const
  {
    return level_traverse(operation);
  }

  GenBinHeap(Compare & __cmp) noexcept
    : cmp(__cmp), head(&head_node), root(RLINK(head)), last(&head_node), 
      num_nodes(0) 
  {
    // empty
  }

  GenBinHeap(Compare && __cmp = Compare()) noexcept
    : GenBinHeap(__cmp)
  {
    // empty
  }

  virtual ~GenBinHeap() noexcept { /* empty */ }

  Node * insert(Node * p) noexcept
  {
    assert(IS_LEAF(p));

    if (root == nullptr) // ¿heap está vacío?
      { // Sí, inicialice

        assert(num_nodes == 0);

        root       = p;
        LLINK(p)   = RLINK(p) = p;
        ULINK(p)   = head;
        IS_LEAF(p) = true;
        IS_LEFT(p) = false; /* root is right child of header node */
        last      = root;
        num_nodes = 1;
        return p;
      }
               // inserción general
    Node * pp = RLINK(last); // padre de actual last
    LLINK(p) = last;
    ULINK(p) = pp;

    if (IS_LEFT(last)) 
      { // p será hijo derecho
        IS_LEFT(p)       = false;
        RLINK(p)         = RLINK(pp);
        LLINK(RLINK(pp)) = p;
        RLINK(pp)        = p;
      }
    else
      { // p será hijo izquierdo
        IS_LEFT(p) = true;
        RLINK(p)   = pp;
        IS_LEAF(pp) = false; // si p es izquierdo ==> pp era hoja
        LLINK(pp)   = p;
      }

    assert(not IS_LEAF(pp));

    RLINK(last) = p;
    last        = p;
    num_nodes++;
    sift_up(last);
    return p;
  }

  Node * getMin() throw(std::exception, std::underflow_error)
  {
    if (root == nullptr)
      throw std::underflow_error ("Heap is empty");

    Node *ret_val = root;

    if (num_nodes == 1)
      {
        root = nullptr;
        ret_val->reset();
        num_nodes = 0;

        return ret_val;
      }

    swap_root_with_last();
    remove_last();
    sift_down(root);
    ret_val->reset();

    return ret_val;
  }

  Node * getMax() throw(std::exception, std::underflow_error)
  {
    return getMin();
  }

  void update(Node * p) noexcept
  {
    sift_down(p);
    sift_up(p);
  }

  Node * remove(Node * node) throw(std::exception, std::underflow_error)
  {
    if (root == nullptr)
      throw std::underflow_error ("Heap is empty");

    if (node == root) 
      return getMin();

    if (node == last) 
      return remove_last();

    Node * p = remove_last();

    if (node == last)
      {
        remove_last();
        insert(p);

        return node;
      }
    replace_node(node, p);
    update(p);
    node->reset();

    return node;
  }

  void remove_all_and_delete() noexcept
  {
    if (root == nullptr) 
      return;

    if (num_nodes <= 3)
      {
        while (not this->is_empty()) 
          delete getMin(); 
        return;
      }

    if (IS_LEFT(last))
      __postorder_delete(root, ULINK(last));
    else
      __postorder_delete(root, nullptr);

    root = nullptr; // reiniciar como si fuese constructor
    last = &head_node;
    num_nodes = 0;
  }

  Node * top() const throw(std::exception, std::underflow_error)
  {
    if (root == nullptr)
      throw std::underflow_error ("Heap is empty");

    return root;
  }

  const size_t & size() const noexcept { return num_nodes; }

  bool is_empty() const noexcept { return size() == 0; }

protected:

  static Node * advance_left(Node * p) noexcept
  {
    if (IS_LEAF(p)) 
      return nullptr;

    return LLINK(p);
  }

  static Node * advance_right(Node * p) noexcept
  {
    if (IS_LEAF(p))
      return nullptr;

    if (not has_sibling(LLINK(p))) 
      return nullptr;

    return RLINK(p);
  }

  virtual bool verify_heap(Node * p) const
  {
    Node * left_link = advance_left(p);
    if (left_link == nullptr)
      {
        assert(IS_LEAF(p));
        return true;
      }

    if (cmp (KEY(left_link), KEY(p))) 
      return false;

    Node * right_link = advance_right(p);
    if (right_link == nullptr) 
      return verify_heap(left_link);

    if (cmp (KEY(right_link), KEY(p))) 
      return false;

    return verify_heap(right_link);
  }

  public:

  bool verify_heap() const
  {
    if (root == nullptr) 
      return true;

    return verify_heap(root);
  }

  class Iterator
  {
    static const size_t Stack_Size = 64;

    FixedStack<Node*> s;
    Node * curr = nullptr;

  public:
    
    Iterator(const GenBinHeap & h) : s(Stack_Size)
    {
      if (h.is_empty())
	return;

      curr = h.root;
    }

    bool has_curr() const noexcept { return curr != nullptr; }

    Node * get_curr() const
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      return curr;
    }

    void next()
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      auto l = curr->advance_left(curr), r = curr->advance_right(curr);
      if (l != nullptr)
	{
	  curr = l;
	  if (r != nullptr)
	    s.push(r);
	  return;
	}
      
      if (r != nullptr)
	{
	  curr = r;
	  return;
	}

      if (s.is_empty())
	curr = nullptr;
      else
	curr = s.pop();
    }
  };
};

    template <class Key, typename Compare = Aleph::less<Key> >
struct BinHeap : public GenBinHeap<BinHeapNode, Key, Compare>
{
  using Node = BinHeapNode<Key>;
  using GenBinHeap<BinHeapNode, Key, Compare>::GenBinHeap;
};

    template <class Key, typename Compare = Aleph::less<Key> >
struct BinHeapVtl : public GenBinHeap<BinHeapNodeVtl, Key, Compare>
{
  using Node = BinHeapNodeVtl<Key>;
  using GenBinHeap<BinHeapNodeVtl, Key, Compare>::GenBinHeap;
};

# undef PREV
# undef NEXT
# undef ULINK
# undef IS_LEAF
# undef IS_LEFT
# undef CTRL_BITS
} // end namespace Aleph
# endif //  TPL_BINHEAP_H

