
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


# ifndef TPL_DYNSETTREE_H
# define TPL_DYNSETTREE_H

# include <ahIterator.H>
# include <tpl_binNodeUtils.H>
# include <tpl_binNodeXt.H>
# include <tpl_binTree.H>
# include <tpl_treap.H>
# include <tpl_treapRk.H>
# include <tpl_avl.H>
# include <tpl_rand_tree.H>
# include <tpl_rb_tree.H>
# include <tpl_splay_tree.H>
# include <tpl_nodePool.H>

using namespace Aleph;

namespace Aleph { 

    template <
      typename Key, 
      template <typename, class> class Tree = Avl_Tree, 
      class Compare = Aleph::less<Key>
      >
class DynSetTree
      : public LocateFunctions<DynSetTree<Key, Tree, Compare>, Key>,
	public FunctionalMethods<DynSetTree<Key, Tree, Compare>, Key>,
	public GenericKeys<DynSetTree<Key, Tree, Compare>, Key>,
	public EqualToMethod<DynSetTree<Key, Tree, Compare>>,
	public StlAlephIterator<DynSetTree<Key, Tree, Compare>>
{
  using Node = typename Tree<Key, Compare>::Node;

  using Tree_Type = Tree<Key, Compare>;

private:

  static const size_t dim = 13;

  mutable Tree<Key, Compare> tree; 
  size_t num_nodes; 
  Node_Pool<Node> node_pool;

public:

  typedef DynSetTree Set_Type;

  typedef Key Item_Type;

  typedef Key Key_Type;

  void swap(DynSetTree & dset)
  {
    tree.swap(dset.tree);
    std::swap(num_nodes, dset.num_nodes);
  }

  DynSetTree(Compare && cmp = Compare()) 
    : tree(std::forward<Compare>(cmp)), num_nodes(0), node_pool(dim)
  {
    /* empty */ 
  }

  DynSetTree(Compare & cmp) 
    : tree(cmp), num_nodes(0), node_pool(dim)
  { 
    /* empty */ 
  }

  DynSetTree(const DynSetTree & srcTree) 
    : tree(srcTree.tree.get_compare()), num_nodes(srcTree.num_nodes),
      node_pool(srcTree.dim)
  { 
    Node * srcRoot = srcTree.tree.getRoot(); 
    try 
      {
	tree.getRoot() = copyRec(srcRoot);
      }
    catch (...)
      {
	num_nodes = 0;
	throw;
      }
  }

  Special_Ctors(DynSetTree, Key);

  DynSetTree(DynSetTree && srcTree) : num_nodes(0), node_pool(dim)
  { 
    swap(srcTree);
  }

  void empty()
  {
    destroyRec(tree.getRoot());
    num_nodes = 0;
  }

  DynSetTree & operator = (const DynList<Key> & list)
  {
    return *this = DynSetTree(list);
  }

  DynSetTree & operator = (const DynSetTree & srcTree)
  {
    if (this == &srcTree)
      return *this;

    Node *src_root = (Node*) const_cast<DynSetTree&>(srcTree).tree.getRoot();

    empty();

    tree.getRoot() = copyRec(src_root);
    num_nodes       = srcTree.num_nodes;

    return *this;
  }

  DynSetTree & operator = (DynSetTree && srcTree)
   {
     swap(srcTree);
     return *this;
   }

  virtual ~DynSetTree() 
  {
    empty();
  }

private:

  Key * __insert(Node * p)
  {
    if (tree.search_or_insert(p) != p)
      {
	node_pool.deallocate(p);
	return nullptr;
      }

    ++num_nodes;

    return &p->get_key();
  }

public:

  Key * insert(const Key & key)
  {
    return __insert(node_pool.allocate(key));
  }

  Key * insert(Key && key)
  {
    return __insert(node_pool.allocate(std::forward<Key>(key)));
  }
  
  Key * append(const Key & key)
  {
    return __insert(node_pool.allocate(key));
  }

  Key * append(Key && key)
  {
    return __insert(node_pool.allocate(std::forward<Key>(key)));
  }

private:

  Key * __search_or_insert(Node * p)
  {
    Node * q = tree.search_or_insert(p);
    if (q != p)
      node_pool.deallocate(p);
    else
      ++num_nodes;

    return &q->get_key();
  }

  pair<Node*, bool> __contains_or_insert(Node * p)
  {
    Node * q = tree.search_or_insert(p);
    if (q != p)
      { // KEY(p) is already inside the tree
	node_pool.deallocate(p);
	return make_pair(q, true);
      }
    ++num_nodes;
    return make_pair(p, false);
  }

public:

  Key * search_or_insert(const Key & key)
  {
    return __search_or_insert(node_pool.allocate(key));
  }

  Key * search_or_insert(Key && key)
  {
    return __search_or_insert(node_pool.allocate(std::forward<Key>(key)));
  }

  /* Busca la clave <code>key</code> en el árbol binario de búsqueda y
     eventualmente lo inserta en caso de no encontrarse.

     <code>contains_or_insert(key)</code> busca en el árbol la clave
     <code>key</code>. Si la clave ya se encuentra, entonces se retorna
     true. De lo contrario la clave es insertada y se retorna false.

     @param[in] key clave a encontrar o a insertar
     @return a pair whose first field is a pointer to the found or
     inserted key and the second field is a boolean whose value is
     `false` if the key was inserted; `true` otherwise, that is if the
     key is already present in the tree
   */
  pair<Key*, bool> contains_or_insert(const Key & key)
  {
    auto p = __contains_or_insert(node_pool.allocate(key));
    return make_pair(&p.first->get_key(), p.second);
  }

  pair<Key*, bool> contains_or_insert(Key && key)
  {
    auto p = __contains_or_insert(node_pool.allocate(std::forward<Key>(key)));
    return make_pair(&p.first->get_key(), p.second);
  }

private:

  Key * __insert_dup(Node * q)
  {
    Node * p = tree.insert_dup(q);
    ++num_nodes;
    return &p->get_key();
  }

public:

  Key * insert_dup(const Key & key)
  {
    return __insert_dup(node_pool.allocate(key));
  }

  Key * insert_dup(Key && key)
  {
    return __insert_dup(node_pool.allocate(std::forward<Key>(key)));
  }

  Key * put(const Key & key)
  {
    return insert(key);
  }

  Key * put(Key && key)
  {
    return insert(std::forward<Key>(key));
  }

  size_t remove(const Key & key)
  {
    Node * p = static_cast<Node*>(tree.remove(key));

    if (p == nullptr)
      return num_nodes;

    node_pool.deallocate(p);

    return --num_nodes;
  }

  Key remove_pos(const size_t i)
  {
    Node * p = static_cast<Node*>(tree.remove_pos(i));
    const Key ret_val = KEY(p);

    node_pool.deallocate(p);

    --num_nodes;

    return ret_val;
  }

  bool exist(const Key & key) const
  {
    return const_cast<DynSetTree&>(*this).search(key) != nullptr;
  }

  bool has(const Key & key) const
  {
    return exist(key);
  }

  bool contains(const Key & key) const
  {
    return exist(key);
  }

  Key & find(const Key & key) const
    throw(std::exception, std::domain_error)
  {
    Node * node = static_cast<Node*>(tree.search(key));

    if (node == nullptr)
      throw std::domain_error("key not found");

    return node->get_key();
  }

  std::pair<int, Key*> find_position(const Key & key) const
  { 
    if (num_nodes == 0)
      return std::pair<int, Key*> (0, nullptr);

    std::pair<int, Node*> p = tree.find_position(key); 

    return std::pair<int, Key*> (p.first, &p.second->get_key()); 
  }

  Key * search(const Key & key) const
    throw(std::exception, std::domain_error)
  {
    Node * node = static_cast<Node*>(tree.search(key));

    if (node == nullptr)
      return nullptr;

    return &(node->get_key());
  }

  const Key & min() const
  {
    if (num_nodes == 0)
      throw std::domain_error("set is empty");

    return find_min(tree.getRoot())->get_key();
  }

  const Key & get_first() const { return min(); }

  const Key & max() const
  {
    if (num_nodes == 0)
      throw std::domain_error("set is empty");

    return find_max(tree.getRoot())->get_key();
  }

  const Key & get_last() const { return max(); }

  const Key & get() const { return max(); }

  const size_t & size() const { return num_nodes; }

  bool is_empty() const { return num_nodes == 0; }

  size_t internal_path_length() const 
  {
    return Aleph::internal_path_length(tree.getRoot()); 
  }

  Node * get_root_node() const { return tree.getRoot(); }

  const Key & get_root() const
  {
    if (num_nodes == 0)
      throw std::domain_error("Tree is empty");
    return KEY(tree.getRoot());
  }

  const Key & get_item() const { return get_root(); }

  size_t height() const { return computeHeightRec(tree.getRoot()); }

      template <class Op>
  void for_each_in_preorder(void (*visitFct)(Node*, int, int))
  {
    Node * root = static_cast<Node*>(tree.getRoot());
    preOrderRec(root, visitFct); 
  }

  long position(const Key & key) const
  {
    std::pair<long, Node*> p = tree.position(key);
    return p.first; 
  }

  Key & select(const size_t & i) 
  {
    return tree.select(i)->get_key();
  }

  const Key & select(const size_t & i) const
  {
    return tree.select(i)->get_key();
  }

  Key & operator () (const size_t & i)
  {
    return select(i);
  }

  const Key & operator [] (const Key & key) const
  {
    return find(key);
  }

  const Key & operator [] (const Key & key)
  {
    return *search_or_insert(key);
  }

  Key & access (const size_t & i)
  {
    return select(i);
  }

  bool verify() 
  {
    return tree.verify() and check_bst(tree.getRoot()); 
  }

private:

    template <class Key_Op>
struct Node_Op
{
  Key_Op & key_op;

  Node_Op(Key_Op & __key_op) : key_op(__key_op) { /* empty */ }

  Node_Op(Key_Op && __key_op) : key_op(__key_op)
  {
    /* empty */ 
  }

  void operator () (Node * root) 
  { 
    assert(root != nullptr);
    key_op(KEY(root));
  }
};

public:

      template <class Key_Op> 
  void for_each_preorder(Key_Op & key_op) const
  {
    Node * root = (Node *) tree.getRoot();

    Node_Op <Key_Op> node_op(const_cast<Key_Op&>(key_op));

    For_Each_Preorder<Node> () (root, node_op);
  }

      template <class Key_Op> 
  void for_each_preorder(Key_Op && key_op = Key_Op()) const
  {
    for_each_preorder<Key_Op>(key_op);
  }

      template <class Key_Op> 
  void for_each_inorder(Key_Op & key_op) const
  {
    Node * root = (Node *) tree.getRoot();

    Node_Op <Key_Op> node_op(const_cast<Key_Op&>(key_op));

    For_Each_In_Order<Node> () (root, node_op);
  }

      template <class Key_Op> 
  void for_each_inorder(Key_Op && key_op = Key_Op()) const
  {
    for_each_inorder<Key_Op>(key_op);
  }

      template <class Key_Op> 
  void for_each_postorder(Key_Op & key_op) 
  {
    Node * root = (Node *) tree.getRoot();

    Node_Op <Key_Op> node_op(const_cast<Key_Op&>(key_op));

    For_Each_Postorder<Node> () (root, node_op);
  }

      template <class Key_Op> 
  void for_each_postorder(Key_Op && key_op = Key_Op()) 
  {
    for_each_postorder<Key_Op>(key_op);
  }

  DynSetTree & join(DynSetTree & t, DynSetTree & dup)
  {
    tree.join(t.tree, dup.tree);
    return *this;
  }

  DynSetTree & join(DynSetTree & t, DynSetTree && dup = DynSetTree())
  {
    return join(t, dup);
  }

  DynSetTree & join_dup(DynSetTree & t)
  {
    tree.join_dup(t.tree);
    t.num_nodes = 0;
    t.tree.getRoot() = Node::NullPtr;
    num_nodes = COUNT(tree.getRoot());
    return *this;
  }

  bool split_key(const Key & key, DynSetTree & l, DynSetTree & r)
  {
    if (not tree.split_key(key, l.tree, r.tree))
      return false;

    tree.getRoot() = Node::NullPtr;
    num_nodes = 0;
    l.num_nodes = COUNT(l.tree.getRoot());
    r.num_nodes = COUNT(r.tree.getRoot());

    return true;
  }

  void split_pos(const size_t pos, DynSetTree & l, DynSetTree & r)
  {
    tree.split_pos(pos, l.tree, r.tree);
    num_nodes = 0;
    l.num_nodes = COUNT(l.tree.getRoot());
    r.num_nodes = COUNT(r.tree.getRoot());
  }

  void split_key_dup(const Key & key, DynSetTree & l, DynSetTree & r)
  {
    tree.split_key_dup(key, l.tree, r.tree);
    tree.getRoot() = Node::NullPtr;
    num_nodes = 0;
    l.num_nodes = COUNT(l.tree.getRoot());
    r.num_nodes = COUNT(r.tree.getRoot());
  }

  struct Iterator : public Tree_Type::Iterator
  {
    using Base = typename Tree_Type::Iterator;

    Iterator(const DynSetTree & tree) : Base(tree.tree) { /* empty */ }

    const Key & get_curr() const { return Base::get_curr()->get_key(); }

    Key & get_curr() { return Base::get_curr()->get_key(); }

    auto get_current() const { return get_curr(); }
  };

  template <class Operation>
  bool traverse(Operation & op)
  {
    return Aleph::traverse(tree.getRoot(), [&op] (Node * p)
			   {
			     return op(p->get_key());
			   });
  }

  template <class Operation>
  bool traverse(Operation && op = Operation())
  {
    return traverse<Operation>(op);
  }

  template <class Operation>
  bool traverse(Operation & op) const
  {
    return Aleph::traverse(tree.getRoot(), [&op] (Node * p)
			   {
			     return op(p->get_key());
			   });
  }

  template <class Operation>
  bool traverse(Operation && op = Operation()) const
  {
    return traverse<Operation>(op);
  }
};


# define SETTREE_ITOR(Name, Key, Cmp)					\
  class Iterator : public DynSetTree<Key, Name, Cmp>::Iterator		\
  {									\
  public:								\
    Iterator() : DynSetTree<Key, Name, Cmp>::Iterator()			\
    { /* empty */ }							\
    									\
    Iterator(DynSetTree<Key, Name, Cmp> & tree)				\
      : DynSetTree<Key, Name, Cmp>::Iterator(tree)			\
    { /* empty */ }							\
  };


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetBinTree : public DynSetTree<Key, BinTree, Compare>
    { /* empty */ };


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetAvlTree : public DynSetTree<Key, Avl_Tree, Compare>
    { /* empty */ };


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetSplayTree : public DynSetTree<Key, Splay_Tree, Compare>
    { /* empty */ };


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetRandTree : public DynSetTree<Key, Rand_Tree, Compare>
{ 
public:

  class Iterator : public DynSetTree<Key, Rand_Tree, Compare>::Iterator 
  { 
  public: 
    Iterator() : DynSetTree<Key, Rand_Tree, Compare>::Iterator() 
    { /* empty */ } 

    Iterator(DynSetTree<Key, Rand_Tree, Compare> & tree) 
      : DynSetTree<Key, Rand_Tree, Compare>::Iterator(tree) 
    { /* empty */ } 
  }; 
};


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetTreap : public DynSetTree<Key, Treap, Compare>
    { /* empty */ };

    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetTreapRk : public DynSetTree<Key, Treap_Rk, Compare>
{
public:
  SETTREE_ITOR(Treap_Rk, Key, Compare);
};


    template  <typename Key, class Compare = Aleph::less<Key>>
class DynSetRbTree : public DynSetTree<Key, Rb_Tree, Compare>
    { /* empty */ }; 


} // end namespace Aleph

# endif /* TPL_DYNSETTREE_H */

