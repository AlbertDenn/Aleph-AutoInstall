
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


# ifndef TPL_TREE_H
# define TPL_TREE_H

# include <stdexcept>
# include <dlink.H>
# include <ahDry.H>
# include <ahIterator.H>
# include <ahFunction.H>
# include <ahFunctional.H>
# include <htlist.H>
# include <tpl_dynListStack.H>
# include <tpl_binNode.H>

# define ISROOT(p)      ((p)->is_root())
# define ISLEAF(p)      ((p)->is_leaf()) 
# define ISLEFTMOST(p)  ((p)->is_leftmost())
# define ISRIGHTMOST(p) ((p)->is_rightmost())

# define SIBLING_LIST(p) ((p)->get_sibling_list())
# define CHILD_LIST(p)   ((p)->get_child_list())
# define SIBLING_LINK(p) ((p)->get_sibling_list())
# define LCHILD(p)       ((p)->get_left_child())
# define RSIBLING(p)     ((p)->get_right_sibling())
# define IS_UNIQUE_SIBLING(p) (RSIBLING(p) == (p))

namespace Aleph
{

    template <class T> 
class Tree_Node 
{
  T data;
  Dlink child;
  Dlink sibling;

  struct Flags
  {
    unsigned int is_root          : 1;
    unsigned int is_leaf          : 1;
    unsigned int is_leftmost      : 1;
    unsigned int is_rightmost     : 1;
    Flags() noexcept 
      : is_root(1), is_leaf(1), is_leftmost(1), is_rightmost(1) {}
  };

  Flags flags;

  LINKNAME_TO_TYPE(Tree_Node, child);
  LINKNAME_TO_TYPE(Tree_Node, sibling);

  Tree_Node * upper_link() const noexcept
  {
    return child_to_Tree_Node(child.get_prev()); 
  }

  Tree_Node * lower_link() const noexcept
  {
    return child_to_Tree_Node(child.get_next()); 
  }

  Tree_Node * left_link() const noexcept
  {
    return sibling_to_Tree_Node(sibling.get_prev()); 
  }

  Tree_Node * right_link() const noexcept
  {
    return sibling_to_Tree_Node(sibling.get_next()); 
  }

public:

  T & get_key() noexcept { return get_data(); }

  const T & get_key() const  noexcept { return get_data(); }

  T & get_data() noexcept { return data; }

  const T & get_data() const noexcept { return data; }

  using key_type = T;

  Dlink * get_child_list() noexcept { return &child; }

  Dlink * get_sibling_list() noexcept { return &sibling; }

  bool is_root() const noexcept { return flags.is_root; }

  bool is_leaf() const noexcept { return flags.is_leaf; }

  bool is_leftmost() const noexcept { return flags.is_leftmost; }

  bool is_rightmost() const noexcept { return flags.is_rightmost; }

  void set_is_root(bool value) noexcept { flags.is_root = value; }

  void set_is_leaf(bool value) noexcept { flags.is_leaf = value; }

  void set_is_leftmost(bool value) noexcept { flags.is_leftmost = value; }

  void set_is_rightmost(bool value) noexcept { flags.is_rightmost = value; }

  Tree_Node() noexcept { /* empty */ }

  Tree_Node(const T & __data) 
  noexcept(std::is_nothrow_copy_constructible<T>::value)
    : data(__data) { /* empty */ }

  Tree_Node(T && __data) 
  noexcept(std::is_nothrow_move_constructible<T>::value)
  : data(std::move(__data)) { /* empty */ }

  Tree_Node * get_left_sibling() const noexcept
  {
    if (is_leftmost()) 
      return nullptr;

    return left_link();
  }

  Tree_Node * get_right_sibling() const noexcept
  {
    if (is_rightmost()) 
      return nullptr;  

    return right_link();
  }

  Tree_Node * get_left_child() const noexcept
  {
    if (is_leaf()) 
      return nullptr;

    return lower_link();
  }

  Tree_Node * get_right_child() const noexcept
  {
    if (is_leaf())
      return nullptr;

    Tree_Node * left_child = lower_link();

    assert(ISLEFTMOST(left_child));

    return left_child->left_link();
  }

  Tree_Node * get_child(const size_t i) const noexcept
  {
    Tree_Node * c = get_left_child();
    for (int j = 0; c != nullptr and j < i; ++j)
      c = c->get_right_sibling();

    return c;
  }

  Tree_Node * get_parent() const noexcept
  {
    if (is_root()) 
      return nullptr; 

    Tree_Node *  p = const_cast<Tree_Node*>(this);
    while (not ISLEFTMOST(p)) // baje hasta el nodo más a la izquierda
      p = p->left_link();

    assert(not ISROOT(p));
    assert(not CHILD_LIST(p)->is_empty()); 

    return p->upper_link();
  }

  void insert_right_sibling(Tree_Node * p) noexcept
  {
    if (p == nullptr) 
      return;

    assert(CHILD_LIST(p)->is_empty());
    assert(SIBLING_LIST(p)->is_empty());
    assert(p->is_rightmost() and p->is_leftmost() and p->is_root() and 
	   p->is_leaf());

    p->set_is_root(false);
    p->set_is_leftmost(false);

    Tree_Node * old_next_node = get_right_sibling(); 
    if (old_next_node != nullptr)
      {
        assert(not this->is_rightmost());
	p->set_is_rightmost(false);
      }
    else
      {
	assert(this->is_rightmost());
	p->set_is_rightmost(true);
      }

    this->set_is_rightmost(false);
    this->sibling.insert(SIBLING_LIST(p));
  }

  void insert_left_sibling(Tree_Node * p)
  {
    if (p == nullptr) 
      return;

    if (this->is_root())
      throw std::domain_error("Cannot insert sibling of a root");

    assert(CHILD_LIST(p)->is_empty());
    assert(SIBLING_LIST(p)->is_empty());
    assert(p->is_rightmost() and p->is_leftmost() and p->is_root() and 
	   p->is_leaf());

    p->set_is_root(false);
    p->set_is_rightmost(false);

    Tree_Node * old_prev_node = this->get_left_sibling();
    if (old_prev_node != nullptr) 
      {
        assert(not this->is_leftmost());
	p->set_is_leftmost(false);
      }
    else 
      { // this es más a la izq ==> p debe a ser primogénito
	assert(this->is_leftmost());

        Tree_Node * parent = this->get_parent();

	// Busca la raíz del árbol. Para ello buscamnos la hoja de this
	Tree_Node * leaf = this;
	while (not leaf->is_leaf())
	  {
	    leaf = leaf->get_left_child();
	    assert(leaf != nullptr);
	  }
	
	Tree_Node * root = leaf->lower_link();
	assert(root != nullptr);

	Dlink tree = CHILD_LIST(root)->cut_list(CHILD_LIST(this));
	tree.del();
	CHILD_LIST(parent)->insert(CHILD_LIST(p));
	p->set_is_leftmost(true);

	assert(p->get_parent() == parent);
      }

    this->set_is_leftmost(false);  
    this->sibling.append(SIBLING_LIST(p));
  }

  void insert_leftmost_child(Tree_Node * p) noexcept
  {
    if (p == nullptr) 
      return;

    assert(CHILD_LIST(p)->is_empty());
    assert(SIBLING_LIST(p)->is_empty());
    assert(p->is_rightmost() and p->is_leftmost() and p->is_root() and 
	   p->is_leaf());

    p->set_is_root(false);
    if (this->is_leaf())
      {
        this->set_is_leaf(false);
        CHILD_LIST(this)->insert(CHILD_LIST(p));
      } 
    else
      {
	Tree_Node * old_left_child = this->lower_link();
	Tree_Node * leaf = old_left_child;
	while (not leaf->is_leaf())
	  leaf = leaf->get_left_child();

	Tree_Node * root = leaf->lower_link();
	Dlink subtree = CHILD_LIST(root)->cut_list(CHILD_LIST(old_left_child));
	subtree.del();
	CHILD_LIST(this)->insert(CHILD_LIST(p));
	SIBLING_LIST(old_left_child)->append(SIBLING_LIST(p));
	old_left_child->set_is_leftmost(false);
	p->set_is_rightmost(false);
	assert(p->get_right_sibling() == old_left_child);
	assert(old_left_child->get_left_sibling() == p);
      }
    assert(p->is_leftmost());
  }

  void insert_rightmost_child(Tree_Node * p) noexcept
  {
    if (p == nullptr) 
      return;

    assert(CHILD_LIST(p)->is_empty());
    assert(SIBLING_LIST(p)->is_empty());
    assert(p->is_rightmost() and p->is_leftmost() and p->is_root() and 
	   p->is_leaf());

    p->set_is_root(false);

    if (this->is_leaf())
      {
        this->set_is_leaf(false);
        CHILD_LIST(this)->insert(CHILD_LIST(p));
      } 
    else
      {
        Tree_Node * old_right_child_node = this->lower_link()->left_link();
        old_right_child_node->set_is_rightmost(false);
        p->set_is_leftmost(false);
        SIBLING_LIST(old_right_child_node)->insert(SIBLING_LIST(p));
      }
  }

  Tree_Node * join(Tree_Node * tree) 
  {
    assert(this->is_root());
    assert(tree != nullptr);
    assert(tree->is_root() and tree->is_leftmost() and tree->is_rightmost());
    
    tree->set_is_root(false);
      
    if (this->is_leaf())
      {
	assert(CHILD_LIST(this)->is_empty() and SIBLING_LIST(this)->is_empty());
	this->set_is_leaf(false);
	CHILD_LIST(this)->splice(CHILD_LIST(tree));
      }
    else
      {
	Tree_Node * right_child = this->lower_link()->left_link();
	right_child->set_is_rightmost(false);
	tree->set_is_leftmost(false);
	SIBLING_LINK(right_child)->splice(SIBLING_LINK(tree));
      }

    return this;
  }

  void insert_tree_to_right(Tree_Node * tree) 
  {
    if (tree == nullptr) 
      return;

    if (not this->is_root()) 
      throw std::domain_error("\"this\" is not root");

    tree->set_is_leftmost(false);
    Tree_Node * old_next_tree = this->get_right_tree();
    if (old_next_tree != nullptr) 
      {
        assert(not this->is_rightmost());
        tree->set_is_rightmost(false);
      }

    this->set_is_rightmost(false);  
    SIBLING_LIST(this)->insert(SIBLING_LIST(tree));
  }

  Tree_Node * get_left_tree() const noexcept
  {
    if (is_leftmost()) 
      return nullptr;
    assert(not is_leftmost()); 
    return left_link();
  }

  Tree_Node * get_right_tree() const noexcept
  {
    if (is_rightmost()) 
      return nullptr;

    assert(not is_rightmost());
    return right_link();
  }

  Tree_Node * get_last_tree() const
  {
    if (not is_leftmost())
      throw std::range_error("\"this\" is not the leftmost tree in the forest");

    return left_link();
  }

  template <typename Operation>
  void for_each_child(Operation & op) const 
  {
    for (Tree_Node * child = get_left_child(); child != nullptr; 
	 child = child->get_right_sibling())
      op(child);
  }

  template <typename Operation>
  void for_each_child(Operation && op = Operation()) const
  {
    for_each_child<Operation>(op);
  }

  template <template <typename> class Container = DynList>
  Container<Tree_Node*> children_nodes() const
  {
    Container<Tree_Node*> ret_val;
    this->for_each_child([&ret_val] (Tree_Node * p) { ret_val.append(p); });
    return ret_val;
  }

  template <template <typename> class Container = DynList>
  Container<T> children() const
  {
    Container<T> ret_val;
    this->for_each_child([&ret_val] (Tree_Node * p) 
			 {
			   ret_val.append(p->get_key());
			 });
    return ret_val;
  }

private:

  template <class Operation> static
  bool preorder(Tree_Node * root, Operation & op)
  {
    if (root == nullptr)
      return true;

    if (not op(root))
      return false;

    for (Tree_Node * child = root->get_left_child(); child != nullptr; 
	 child = child->get_right_sibling())
      if (not preorder(child, op))
	return false;
	
    return true;
  }

public:

  template <class Operation>
  bool traverse(Operation & op)
  {
    return preorder(this, op);
  }

  template <class Operation>
  bool traverse(Operation && op = Operation())
  {
    return traverse(op);
  }

  template <class Operation>
  bool traverse(Operation & op) const
  {
    return const_cast<Tree_Node*>(this)->traverse(op);
  }

  template <class Operation>
  bool traverse(Operation && op = Operation()) const
  {
    return traverse(op);
  }

  Functional_Methods(Tree_Node*);

  class Children_Iterator
  {
    Tree_Node * curr = nullptr;

  public:

    Children_Iterator(const Tree_Node & p) 
    noexcept : curr(p.get_left_child()) {}

    Children_Iterator(Tree_Node & p) noexcept 
    : curr(p.get_left_child()) {}

     Children_Iterator(Tree_Node * p) noexcept 
    : curr(p->get_left_child()) {}

    bool has_curr() const noexcept { return curr != nullptr; }

    Tree_Node * get_curr() const noexcept { return curr; }

    void next() 
    {
      if (curr == nullptr)
	throw std::overflow_error("Iterator overflow");

      curr = curr->get_right_sibling();
    }
  };

  class Iterator
  {
    Tree_Node * root = nullptr;
    Tree_Node * curr = nullptr;
    long pos = 0;
    DynListStack<Tree_Node*> s;

  public:

    void swap(Iterator & it)
    {
      std::swap(root, it.root);
      std::swap(curr, it.curr);
      std::swap(pos, it.pos);
      s.swap(it.s);
    }

    Iterator(Tree_Node * __root = nullptr) noexcept
    : root(__root), curr(root)
    {
      // empty
    }

    Iterator(const Iterator & it) 
      : root(it.root), curr(it.curr), pos(it.pos), s(it.s) 
    {
      // empty
    }

    Iterator(Iterator && it) { swap(it); }

    Iterator & operator = (const Iterator & it)
    {
      if (this == &it)
	return *this;

      root = it.root;
      curr = it.curr;
      pos = it.pos;
      s = it.s;
      return *this;
    }
      
    Iterator & operator = (Iterator && it)
    {
      swap(it);
      return *this;
    }

    void reset_first() noexcept
    {
      s.empty();
      curr = root;
    }

    bool has_curr() const noexcept { return curr != nullptr; }

    bool has_current() const noexcept { return has_curr(); }

    Tree_Node * get_curr() const
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      return curr;
    }

    auto get_current() const { return get_curr(); }

    void next()
    {
      if (not has_curr())
	throw std::overflow_error("Iterator overflow");

      Tree_Node * lchild = curr->get_left_child();

      if (lchild == nullptr)
	{
	  if (s.is_empty())
	    curr = nullptr;
	  else
	    curr = s.pop();

	  return;
	}

      for (auto p = lchild->get_right_sibling(); p != nullptr; 
	   p = p->get_right_sibling())
	s.push(p);
      
      curr = lchild;
    }

    void end()
    {
      curr = nullptr;
      s.empty();
      pos = -1;
    }

    // has_curr() == true
    size_t get_pos() const { return pos; }
  };

  Iterator get_it() const
  {
    return Iterator(const_cast<Tree_Node*>(this));
  }

  STL_ALEPH_ITERATOR(Tree_Node);
};


  template <typename T>
struct Tree_Node_Vtl : public Tree_Node<T>
{
  virtual ~Tree_Node_Vtl() {}
};


  template <class Node> static inline
void clone_tree(Node * src, Node * tgt)
{
  using It = typename Node::Children_Iterator;
  for (It it(src); it.has_curr(); it.next())
    tgt->insert_rightmost_child(new Node(it.get_curr()->get_key()));

  for (Pair_Iterator<Node, It> it(*src, *tgt); it.has_curr(); it.next()) 
      {
	auto p = it.get_curr();
	clone_tree(p.first, p.second);
      }
}
  
template <class Node>
Node * clone_tree(Node * root)
{
  Node * ret = new Node(root->get_key());
  clone_tree(root, ret);
  return ret;
}

    template <class Node> static inline
void __tree_preorder_traversal(Node * root, const int & level, 
                               const int & child_index,
                               void (*visitFct)(Node *, int, int))
{
  (*visitFct)(root, level, child_index);
  Node * child = root->get_left_child(); 
  for (int i = 0; child != nullptr; ++i, child = child->get_right_sibling())
    __tree_preorder_traversal(child, level + 1, i, visitFct);
}

    template <class Node> inline
void tree_preorder_traversal(Node * root, void (*visitFct)(Node *, int, int))
{

  if (not root->is_root())
    throw std::domain_error("root is not root");

  __tree_preorder_traversal(root, 0, 0, visitFct);
}

    template <class Node> inline
void forest_preorder_traversal(Node * root, void (*visitFct)(Node *, int, int))
{

  if (not root->is_root())
    throw std::domain_error("root is not root");

  for (/* nada */; root != nullptr; root = root->get_right_tree())
    {
      assert(root->is_root());
    __tree_preorder_traversal(root, 0, 0, visitFct);
    }
}

    template <class Node> static inline
void __tree_postorder_traversal(Node * node, const int & level, 
                                const int & child_index,
                                void (*visitFct)(Node *, int, int))
{
  Node * child = node->get_left_child();

  for (int i = 0; child not_eq nullptr; i++, child = child->get_right_sibling())
    __tree_postorder_traversal(child, level + 1, i, visitFct);

  (*visitFct)(node, level, child_index);
}

    template <class Node> inline
void tree_postorder_traversal(Node * root, void (*visitFct)(Node *, int, int))
{
  __tree_postorder_traversal(root, 0, 0, visitFct);
}

    template <class Node> inline
void forest_postorder_traversal(Node * root, void (*visitFct)(Node *, int, int))
{
  if (not root->is_leftmost())
    throw std::domain_error("root is not the leftmost node of forest");

  if (not root->is_root())
    throw std::domain_error("root is not root");

  for (/* nada */; root not_eq nullptr; root = root->get_right_sibling())
    {
      assert(root->is_root());
      __tree_postorder_traversal(root, 0, 0, visitFct);
    }
}

  template <class Node, class Eq>
inline bool are_tree_equal(Node * t1, Node * t2, Eq & eq) 
    noexcept(noexcept(eq(t1->get_key(), t2->get_key())))
{
  if (t1 == nullptr)
    return t2 == nullptr;

  if (t2 == nullptr)
    return false;

  if (not eq(t1->get_key(), t2->get_key()))
    return false;

  try
    {
      return zipEq(t1->children_nodes(), t2->children_nodes()).all([] (auto p) 
        { 
	  return are_tree_equal(p.first, p.second);
	});
    }
  catch (std::length_error)
    {
      return false;
    }
}

  template <class Node, 
	    class Eq = std::equal_to<typename Node::key_type>>
inline bool are_tree_equal(Node * t1, Node * t2, Eq && eq = Eq()) 
	      noexcept(noexcept(are_tree_equal<Node, Eq>(t1, t2, eq)))
{
  return are_tree_equal<Node, Eq>(t1, t2, eq);
}

    template <class Node> inline 
void destroy_tree(Node * root)
{
  if (not IS_UNIQUE_SIBLING(root))
    SIBLING_LIST(root)->del(); // no ==> sacarlo de lista hermanos

      // recorrer los subárboles de derecha a izquierda
  for (Node * p = (Node*) root->get_right_child(); p != nullptr; /* nada */)
    {
      Node * to_delete = p;      // respaldar subárbol a borrar p
      p = (Node*) p->get_left_sibling(); // Avanzar p a hermano izquierdo
      destroy_tree(to_delete);   // eliminar recursivamente árbol
    }

  if (root->is_leftmost()) // ¿sacar lista hijos?
    CHILD_LIST(root)->del(); 

  delete root;
}

    template <class Node> inline 
void destroy_forest(Node * root)
{
  if (not root->is_leftmost())
    throw std::domain_error("root is not the leftmost tree of forest");

  if (not root->is_root())
    throw std::domain_error("root is not root");

  while (root != nullptr) // recorre los árboles de izquierda a derecha
    {
      Node * to_delete = root;          // respalda raíz 
      root = root->get_right_sibling(); // avanza a siguiente árbol 
      SIBLING_LIST(to_delete)->del();   // elimine de lista árboles
      destroy_tree(to_delete);          // Borre el árbol
    }
}

    template <class Node>
size_t compute_height(Node * root) 
{
  size_t temp_h, max_h = 0;
  for (Node * aux = root->get_left_child(); aux != nullptr;
       aux = aux->get_right_sibling()) 
    if ((temp_h = compute_height(aux)) > max_h)
      max_h = temp_h;

  return max_h + 1;
}

    template <class Node> static inline
Node * __deway_search(Node * node, int path [], 
                      const int & idx, const size_t & size)
{
  if (node == nullptr) 
    return nullptr;

  if (idx > size)
    throw std::out_of_range("index out of maximum range");

  if (path[idx] < 0) // verifique si se ha alcanzado el nodo
    return node; 
      // avance hasta el próximo hijo path[0]
  Node * child = node->get_left_child();
  for (int i = 0; i < path[idx] and child != nullptr; ++i)
    child = child->get_right_sibling();

  return __deway_search(child, path, idx + 1, size); // próximo nivel
}

    template <class Node> inline
Node * deway_search(Node * root, int path [], const size_t & size)
{
  for (int i = 0; root != nullptr; i++, root = root->get_right_sibling())
    if (path[0] == i) 
      return __deway_search(root, path, 1, size);

  return nullptr;
}

    template <class Node, class Equal> inline static
Node * __search_deway(Node * root, const typename Node::key_type & key,
                      const size_t & current_level, int deway [], 
                      const size_t & size, size_t & n);

    template <class Node, 
              class Equal = Aleph::equal_to<typename Node::key_type> > inline
Node * search_deway(Node * root, const typename Node::key_type & key,
                    int deway [], const size_t & size, size_t & n)
{
  n = 1; // valor inicial de longitud de número de Deway

  if (size < n)
    throw std::overflow_error("there is no enough space for deway array");

  for (int i = 0; root != nullptr; i++, root = root->get_right_sibling())
    {
      deway[0] = i;
      Node * result =
        __search_deway <Node, Equal> (root, key, 0, deway, size, n);
      if (result != nullptr) 
        return result;
    }

  return nullptr;
}

    template <class Node, class Equal> inline static
Node * __search_deway(Node * root,  
                      const typename Node::key_type & key,
                      const size_t & current_level, int deway [], 
                      const size_t & size, size_t & n)
{

  if (current_level >= size) 
    throw std::overflow_error("there is no enough space for deway array");

  if (root == nullptr) 
    return nullptr;

  if (Equal () (KEY(root), key))
    {
      n = current_level + 1; // longitud del arreglo deway
      return root;
    }

  Node * child = root->get_left_child();
  for (int i = 0; child != nullptr; 
       i++, child = child->get_right_sibling())
    {
      deway[current_level + 1] = i;
      Node * result = __search_deway <Node, Equal> 
        (child, key, current_level + 1, deway, size, n);

      if (result!= nullptr) 
        return result;
    }

  return nullptr;
}

    template <class TNode, class BNode>
BNode * forest_to_bin(TNode * root)
{
  if (root == nullptr) 
    return BNode::NullPtr;  

  BNode * result = new BNode (root->get_key());
  LLINK(result) = forest_to_bin<TNode,BNode>(root->get_left_child());
  RLINK(result) = forest_to_bin<TNode,BNode>(root->get_right_sibling());

  return result;
}

    template <class TNode, class BNode> inline static
void insert_child(BNode * lnode, TNode * tree_node)
{
  if (lnode == BNode::NullPtr) 
    return;  

  TNode * child = new TNode(KEY(lnode));
  tree_node->insert_leftmost_child(child);
}

    template <class TNode, class BNode> inline static
void insert_sibling(BNode * rnode, TNode * tree_node)
{
  if (rnode == BNode::NullPtr) 
    return;  

  TNode * sibling = new TNode(KEY(rnode));
  tree_node->insert_right_sibling(sibling);
}

    template <class TNode, class BNode> inline static
void bin_to_tree(BNode * broot, TNode * troot)
{
  if (broot == BNode::NullPtr) 
    return;  

  insert_child(LLINK(broot), troot);
  TNode * left_child =  troot->get_left_child();

  bin_to_tree(LLINK(broot), left_child);

  insert_sibling(RLINK(broot), troot);
  TNode * right_sibling = troot->get_right_sibling();

  bin_to_tree(RLINK(broot), right_sibling);
}

    template <class TNode, class BNode> inline 
TNode * bin_to_forest(BNode * broot) 
{
  if (broot == BNode::NullPtr) 
    return nullptr;  

  TNode * troot = new TNode (KEY(broot));
  bin_to_tree(broot, troot);
  return troot;
}

} // fin namespace Aleph

# endif // TPL_TREE_H

