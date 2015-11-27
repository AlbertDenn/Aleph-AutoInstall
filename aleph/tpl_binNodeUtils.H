
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

# ifndef TPL_BINNODEUTILS_H
# define TPL_BINNODEUTILS_H 

# include <ahFunction.H>
# include <tpl_arrayStack.H> 
# include <tpl_arrayQueue.H>
# include <tpl_dynListQueue.H>
# include <bitArray.H>
# include <tpl_dynDlist.H>
# include <tpl_binNode.H>

using namespace Aleph;
namespace Aleph {

    template <class Node> inline static
void __inorder_rec(Node * node, const int& level, int & position, 
                   void (*visitFct)(Node *, int, int)) 
{
  if (node == Node::NullPtr) 
    return;

  __inorder_rec(LLINK(node), level + 1, position, visitFct);
 
 (*visitFct)(node, level, position);
  ++position;

  __inorder_rec(RLINK(node), level + 1, position, visitFct); 
}

    template <class Node> inline
int inOrderRec(Node * root, void (*visitFct)(Node*, int, int))
{
  int position = 0;
  __inorder_rec(root, 0, position, visitFct); 
  return position;
}

    template <class Node> inline static
void __preorder_rec (Node * p, const int & level, int & position,
                     void (*visitFct)(Node*, int, int))
{
  if (p == Node::NullPtr) 
    return;

  (*visitFct)(p, level, position);
  ++position;

  __preorder_rec(LLINK(p), level + 1, position, visitFct);
  __preorder_rec(RLINK(p), level + 1, position, visitFct);
} 

    template <class Node> inline 
int preOrderRec(Node * root, void (*visitFct)(Node*, int, int))
{
  int position = 0;
  __preorder_rec(root, 0, position, visitFct); 
  return position;
}

    template <class Node> inline static
void __postorder_rec(Node * node, const int & level, int & position, 
                     void (*visitFct)(Node*, int, int))
{
  if (node == Node::NullPtr) 
    return;

  __postorder_rec(LLINK(node), level + 1, position, visitFct);
  __postorder_rec(RLINK(node), level + 1, position, visitFct); 

  (*visitFct)(node, level, position);
  ++position;
}

    template <class Node> inline
int postOrderRec(Node * root, void (*visitFct)(Node*, int, int))
{
  int position = 0;
  __postorder_rec(root, 0, position, visitFct); 
  return position;
}

    template <class Node> 
class For_Each_In_Order
{
  template <class Op>
  static void for_each_inorder(Node * root, Op & op) 
  {
    if (root == Node::NullPtr)
      return;

    for_each_inorder(LLINK(root), op);
    op(root);
    for_each_inorder(RLINK(root), op);
  }

public:

  template <class Op>
  void traverse(Node * root, Op & op) const { for_each_inorder<Op>(root, op); }

  template <class Op>
  void operator () (Node * root, Op & op) const
  {
    for_each_inorder<Op>(root, op);
  }
  
  template <class Op>
  void operator () (Node * root, Op && op) const
  {
    for_each_inorder<Op>(root, op);
  }
};

  template <class Node, class Op> inline
void for_each_in_order(Node * root, Op && op)
{
  return For_Each_In_Order<Node>().template traverse<Op>(root, op);
}

  template <class Node, class Operation> inline
bool traverse(Node * root, Operation & operation)
{
  if (root == Node::NullPtr)
    return true;

  return traverse<Node, Operation>(LLINK(root), operation) and
    operation(root) and 
    traverse<Node, Operation>(RLINK(root), operation);
}

  template <class Node, class Operation> inline
bool traverse(Node * root, Operation && operation)
{
  return traverse<Node, Operation>(root, operation);
}

    template <class Node> 
class For_Each_Preorder
{
  template <class Op>
  static void preorder(Node * root, Op & op) 
  {
    if (root == Node::NullPtr)
      return;

    op(root);
    preorder(LLINK(root), op);
    preorder(RLINK(root), op);
  }

public:

  template <class Op>
  void traverse(Node * root, Op & op) const { return preorder(root, op); }
  
  template <class Op>
  void operator () (Node * root, Op & op) 
  {
    preorder<Op>(root, op);
  }

  template <class Op>
  void operator () (Node * root, Op && op = Op()) 
  {
    preorder<Op>(root, op);
  }
};

  template <class Node, class Op> 
void for_each_preorder(Node * root, Op && op)
{
  For_Each_Preorder<Node>().template traverse<Op>(root, op);
}


    template <class Node> 
class For_Each_Postorder
{
  template <class Op>
  static void postorder(Node * root, Op & op) 
  {
    if (root == Node::NullPtr)
      return;

    postorder(LLINK(root), op);
    postorder(RLINK(root), op);
    op(root);
  }

public:

  template <class Op>
  void traverse(Node * root, Op & op) const { return postorder(root, op); }
  
  template <class Op>
  void operator () (Node * root, Op & op) 
  {
    postorder<Op>(root, op);
  }

  template <class Op>
  void operator () (Node * root, Op && op = Op())
  {
    postorder<Op>(root, op);
  }
};

  template <class Node, class Op> 
void for_each_postorder(Node * root, Op && op)
{
  For_Each_Postorder<Node>().template traverse<Op>(root, op);
}


template <class Node>
static void prefix(Node * root, DynList<Node*> & acc)
{
  if (root == Node::NullPtr)
    return;

  acc.append(root);
  prefix(LLINK(root), acc);
  prefix(RLINK(root), acc);
}

template <class Node>
static void infix(Node * root, DynList<Node*> & acc)
{
  if (root == Node::NullPtr)
    return;

  infix(LLINK(root), acc);
  acc.append(root);
  infix(RLINK(root), acc);
}

template <class Node>
static void suffix(Node * root, DynList<Node*> & acc)
{
  if (root == Node::NullPtr)
    return;

  sufffix(LLINK(root), acc);
  siffix(RLINK(root), acc);
  acc.append(root);
}

template <class Node>
DynList<Node*> prefix(Node * root)
{
  DynList<Node*> ret_val;
  prefix(root, ret_val);
  return ret_val;
}

template <class Node>
DynList<Node*> infix(Node * root)
{
  DynList<Node*> ret_val;
  infix(root, ret_val);
  return ret_val;
}

template <class Node>
DynList<Node*> suffix(Node * root)
{
  DynList<Node*> ret_val;
  suffix(root, ret_val);
  return ret_val;
}


    template <class Node> inline 
size_t compute_cardinality_rec(Node * root) noexcept
{
  if (root == Node::NullPtr) 
    return 0;

  return (compute_cardinality_rec(LLINK(root)) + 1 + 
          compute_cardinality_rec(RLINK(root)));
}

    template <class Node> inline 
size_t size(Node * root) noexcept
{
  return compute_cardinality_rec(root);
}

template <class Node> inline size_t computeHeightRec(Node * root) noexcept
{
  if (root == Node::NullPtr) 
    return 0;

  const size_t left_height  = computeHeightRec(LLINK(root));
  const size_t right_height = computeHeightRec(RLINK(root));

  return 1 + std::max(left_height, right_height);
}

    template <class Node> inline 
void destroyRec(Node *& root) noexcept
{
  if (root == Node::NullPtr) 
    return;

  destroyRec(LLINK(root));
  destroyRec(RLINK(root));
  delete root;
  root = Node::NullPtr;
}

    template <class Node> inline 
Node * copyRec(Node * root) throw(std::exception, std::bad_alloc) 
{
  if (root == Node::NullPtr) 
    return Node::NullPtr;

  Node * tgt_root = new Node(*root); 

  try 
    {
      LLINK(tgt_root) = copyRec<Node>(LLINK(root));
      RLINK(tgt_root) = copyRec<Node>(RLINK(root));

    }
  catch (...)
    {
      assert(RLINK(tgt_root) == Node::NullPtr);

      if (LLINK(tgt_root) != Node::NullPtr) 
        destroyRec(LLINK(tgt_root)); // TODO: diff de Node*&

      delete tgt_root;

      throw;
    }

  return tgt_root;
}      

    template <class Node> inline 
bool areSimilar(Node * t1, Node * t2) noexcept
{
  if (t1 == t2) // that include the case when t1 and t2 are the same or
		// bot are Node::NullPtr 
    return true;

  if (t1 == Node::NullPtr or t2 == Node::NullPtr) 
    return false;

  return (areSimilar(LLINK(t1), LLINK(t2)) and 
          areSimilar(RLINK(t1), RLINK(t2)));
}

    template <class Node, class Equal> inline
bool areEquivalents(Node * t1, Node * t2, Equal & op) noexcept
{
  if (t1 == t2) 
    return true;

  if (t1 == Node::NullPtr or t2 == Node::NullPtr) 
    return false;

  if (not op(KEY(t1), KEY(t2))) 
    return false;

  return (areEquivalents(LLINK(t1), LLINK(t2), op) and 
          areEquivalents(RLINK(t1), RLINK(t2), op));
}

template <class Node, class Equal = std::equal_to<typename Node::key_type>> 
inline bool areEquivalents(Node * t1, Node * t2, Equal && op = Equal()) noexcept
{
  return areEquivalents(t1, t2, op);
}

    template <class Node> inline
void levelOrder(Node * root, void (*visitFct)(Node*, int, bool))
{
  if (root == Node::NullPtr)
    return;

  DynListQueue<std::pair<Node*, bool> > queue; 
  queue.put(std::pair<Node*, bool>(root, bool()));

  for (int pos = 0; not queue.is_empty(); pos++)
    {
      std::pair<Node*, bool> pr = queue.get();
      Node *& p = pr.first;

      (*visitFct) (p, pos, pr.second);

      if (LLINK(p) != Node::NullPtr)
        queue.put(std::pair <Node*, bool> (LLINK(p), true));

      if (RLINK(p) != Node::NullPtr)
        queue.put(std::pair <Node*, bool> (RLINK(p), false));
    }
}


  template <class Node, class Operation> inline
bool level_traverse(Node * root, Operation & operation)
{
  if (root == Node::NullPtr)
    return true;

  DynListQueue<Node*> queue; 
  queue.put(root);
  while (not queue.is_empty())
    {
      Node * p = queue.get();
      if (not operation(p))
	return false;

      if (LLINK(p) != Node::NullPtr)
        queue.put(LLINK(p));

      if (RLINK(p) != Node::NullPtr)
        queue.put(RLINK(p));
    }
  return true;
}

  template <class Node, class Operation> inline
bool level_traverse(Node * root, Operation && operation)
{
  return level_traverse<Node, Operation>(root, operation);
}

    template <template <class> class Node, typename Key> inline
Node<Key> * build_tree(const DynArray<Key> & preorder, long l_p, long r_p,
                       const DynArray<Key> & inorder, long l_i, long r_i)
{
  if (l_p > r_p)
    {
      assert(l_i > r_i);
      return Node<Key>::NullPtr;
    }

  assert(r_p - l_p == r_i - l_i); 

  Node<Key> * root = new Node<Key>(preorder[l_p]);
  if (r_p == l_p) 
    return root;
  
  assert(l_i <= r_i);

  int i = 0;
  for (int j = l_i; j <= r_i; ++j)
    if (inorder[j] == preorder[l_p])
      {
        i = j - l_i;
        break;
      }

  assert(i <= r_i);

  LLINK(root) = build_tree<Node, Key>(preorder, l_p + 1, l_p + i, 
				      inorder, l_i, l_i + (i - 1));
  RLINK(root) = build_tree<Node, Key>(preorder, l_p + i + 1, r_p, 
				      inorder, l_i + i + 1, r_i);
  return root;
}

    template <template <class> class Node, typename Key> inline
Node<Key> * build_postorder(const DynArray<Key> & post, long lp, long rp,
			    const DynArray<Key> & in, long li, long ri)
{
  assert(rp - lp == ri - li);
  if (lp > rp)
    return Node<Key>::NullPtr;

  Node<Key> * root = new Node<Key>(post[rp]);
  
  int i = li;
  for (; i <= ri; ++i) // search in inorder array the index of root
    if (in[i] == post[rp])
      break;

  assert(i <= ri);

  LLINK(root) = build_postorder<Node, Key>(post, lp, lp + (i - li) - 1,
					   in, li, i - 1);
  RLINK(root) = build_postorder<Node, Key>(post, rp - (ri - i), rp - 1, 
					   in, i + 1, ri);
  return root;
}

    template <class Node> inline static void 
__compute_nodes_in_level(Node * root, long level, long current_level, 
                         DynDlist<Node*> & level_list)
{
  if (root == Node::NullPtr) 
    return;

  if (current_level == level)
    {
      level_list.append(root);
      return; // no vale la pena descender mï¿½s
    }

  __compute_nodes_in_level(LLINK(root), 
			   level, current_level + 1, level_list);
  __compute_nodes_in_level(RLINK(root), 
			   level, current_level + 1, level_list);
}

    template <class Node> inline
DynDlist<Node*> compute_nodes_in_level(Node * root, const int & level)
{
  DynDlist<Node*> list;
  __compute_nodes_in_level(root, level, 0, list);
  return list;
}

    template <class Node> inline
void inOrderThreaded(Node * root, void (*visitFct)(Node*))
{
  if (root == Node::NullPtr) 
    return;

  Node *p = root, *r = Node::NullPtr, *q;
  while (p != Node::NullPtr)
    {
      q = LLINK(p); 
      if (q == Node::NullPtr) 
        { // No hay rama izq ==> visitar p
          (*visitFct)(p);  
          r = p;          
          p = RLINK(p);   
          continue;       
        }

          // avanzar hacia el nodo mï¿½s a la derecha de la rama izquierda
      while (q != r and RLINK(q) != Node::NullPtr)
        q = RLINK(q);

      if (q != r) // tiene p un predecesor? 
        { // si ==> dejar un hilo para luego subir a visitar p
          RLINK(q) = p; // Aquï¿½ se coloca el hilo
          p = LLINK(p); // Seguir bajando por la izquierda
          continue;     
        }

      (*visitFct)(p);

      RLINK(q) = Node::NullPtr; // Borrar hilo
      r = p;            
      p = RLINK(p); // avanzar a la rama derecha
    }
}

    template <class Node> inline
void preOrderThreaded(Node * node, void (*visitFct)(Node*))
{
  if (node == Node::NullPtr) 
    return;

  Node * p = node, * r = Node::NullPtr, *q;
  while (p != Node::NullPtr)
  {
    q = LLINK(p); 

    if (q == Node::NullPtr) 
      { 
        (*visitFct)(p);  
        r = p;
        p = RLINK(p);
        continue;    
      }

        // avanzar hacia el nodo mï¿½s a la derecha de la rama izquierda
    while (q != r and RLINK(q) != Node::NullPtr)
      q = RLINK(q);

    if (q != r) 
      { 
        RLINK(q) = p;
        (*visitFct)(p);
        p = LLINK(p);
        continue;    
      }

    RLINK(q) = Node::NullPtr; /* delete thread */
    r = p;                  
    p = RLINK(p);       /* advance to right branch */
  }
} 

    template <class Node> inline  static
size_t __internal_path_length(Node * p, const size_t & level) noexcept
{
  if (p == Node::NullPtr) 
    return 0;

  return level + __internal_path_length(LLINK(p), level + 1) +
    __internal_path_length(RLINK(p), level + 1);
}

    template <class Node> inline 
size_t internal_path_length(Node * p) noexcept
{
  return __internal_path_length(p, 0);
}

    template <class Node> inline 
void tree_to_bits(Node * root, BitArray & array)
{
  if (root == Node::NullPtr)
    {
      array.push(1);
      return;
    }

  array.push(0);
  tree_to_bits(LLINK(root), array);
  tree_to_bits(RLINK(root), array);
}

template <class Node> inline 
BitArray tree_to_bits(Node * root)
{
  BitArray ret_val;
  tree_to_bits(root, ret_val);
  return ret_val;
}

template <class Node> inline string code(Node * root)
{
  BitArray bits = tree_to_bits(root);
  const size_t n = bits.size();
  string str(""); str.reserve(n);
  for (size_t i = 0; i < n; ++i)
    str.push_back(bits(i) ? 'b' : 'a');

  return str;
}

     template <class Node> static inline 
Node * __bits_to_tree(const BitArray & array, int & i)
{
  int bit = array.read_bit(i++);
  if (bit == 1)
    return Node::NullPtr;

  Node * p = new Node;
  LLINK(p) = __bits_to_tree<Node>(array, i);
  RLINK(p) = __bits_to_tree<Node>(array, i);

  return p;
}

template <class Node> inline
Node * bits_to_tree(const BitArray & array, int idx = 0)
{
  return __bits_to_tree <Node>(array, idx);
}

  template <class Node> inline 
void save_tree_keys_in_prefix(Node * root, ostream & output)
{
  if (root == Node::NullPtr)
    return;

  output << root->get_key() << " ";

  save_tree_keys_in_prefix(LLINK(root), output);
  save_tree_keys_in_prefix(RLINK(root), output);
}

  template <class Node> inline 
void load_tree_keys_in_prefix(Node * root, istream & input)
{
  if (root == Node::NullPtr)
    return;

  input >> root->get_key();

  load_tree_keys_in_prefix(LLINK(root), input);
  load_tree_keys_in_prefix(RLINK(root), input);
}


  template <class Node> inline 
void save_tree(Node * root, ostream & output)
{
  BitArray prefix;
  tree_to_bits(root, prefix); 
  prefix.save(output);
  save_tree_keys_in_prefix(root, output);
}


  template <class Node> inline 
Node * load_tree(istream & input)
{
  BitArray prefix;
  prefix.load(input);
  Node * root = bits_to_tree <Node> (prefix);
  load_tree_keys_in_prefix(root, input); 
  return root;
}

   
   template <class Node, class Get_Key> inline 
void put_tree_keys_in_array(Node * root, ostream & out)
{
  if (root == Node::NullPtr)
    return;

  const string str = Get_Key () (root);

  if (str == "\t")
    out << "\"\t\"";
  else if (str == "\n")
    out << "\"\\n\""; 
  else
    out << "\"" << str << "\"";
  out << ", ";

  put_tree_keys_in_array<Node, Get_Key>(LLINK(root), out);
  put_tree_keys_in_array<Node, Get_Key>(RLINK(root), out);
}


    template <class Node, class Load_Key> inline 
void load_tree_keys_from_array(Node * root, const char * keys[], int & idx)
{
  if (root == Node::NullPtr)
    return;

  if (Load_Key()(root, keys[idx]))
    ++idx;

  load_tree_keys_from_array<Node, Load_Key>(LLINK(root), keys, idx);
  load_tree_keys_from_array<Node, Load_Key>(RLINK(root), keys, idx);
}

   template <class Node, class Get_Key> inline 
   void save_tree_in_array_of_chars(Node *         root, 
				    const string & array_name, 
				    ostream &     output)
   {
     BitArray prefix;
     tree_to_bits(root, prefix); 
     prefix.save_in_array_of_chars(array_name + "_cdp", output);
     output << "const char * " << array_name << "_k[] = { " << endl;
     put_tree_keys_in_array <Node, Get_Key> (root, output);
     output << "nullptr };" << endl;
   }


    template <class Node, class Load_Key> inline 
Node * load_tree_from_array(const unsigned char bits[], 
                            const size_t &      num_bits, 
                            const char *        keys[])
{
  BitArray prefix;
  prefix.load_from_array_of_chars(bits, num_bits);
  Node * root = bits_to_tree <Node> (prefix);
  int i = 0;
  load_tree_keys_from_array <Node, Load_Key> (root, keys, i);
  return root;
}


    template <class Node, class Compare> inline
bool check_bst(Node * p, Compare & cmp)
{
  if (p == Node::NullPtr) 
    return true;

  if (LLINK(p) != Node::NullPtr)
    {
      if (less_or_equal_than(KEY(LLINK(p)), KEY(p), cmp))
        return check_bst(LLINK(p), cmp);
      else 
        return false;
    }

  if (RLINK(p) != Node::NullPtr)
    {
      if (less_or_equal_than(KEY(p), KEY(RLINK(p)), cmp))
        return check_bst(RLINK(p), cmp);
      else
        return false;
    }
  return true;
}

 template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
bool check_bst(Node * p, Compare && cmp = Compare())
{
  return check_bst(p, cmp); 
}

    template <class Node> inline Node * 
preorder_to_bst(DynArray<typename Node::key_type> & preorder, int l, int r)
{
  if (l > r) 
    return Node::NullPtr;

  Node * root = new Node(preorder[l]);

  if (l == r) 
    return root;

  int first_greater = l + 1;
  while ((first_greater <= r) and (preorder[first_greater] < preorder[l]))
    ++first_greater; 

  LLINK(root) = preorder_to_bst<Node>(preorder, l + 1, first_greater - 1);
  RLINK(root) = preorder_to_bst<Node>(preorder, first_greater, r);

  return root;
}

    template <class Node, class Compare> inline 
Node * searchInBinTree(Node * root, const typename Node::key_type & key,
		       Compare & cmp) noexcept
{
  while (root != Node::NullPtr)
    if (cmp(key, KEY(root)))
      root = LLINK(root);
    else if(cmp(KEY(root), key))
      root = RLINK(root);
    else
      break;

  return root;
}

  template <class Node, 
            class Compare = Aleph::less<typename Node::key_type> > inline 
Node * searchInBinTree(Node * root,
		       const typename Node::key_type & key,
		       Compare && cmp = Compare()) noexcept
{  
  return searchInBinTree <Node, Compare> (root, key, cmp);
}

    template <class Node> inline 
Node * find_min(Node * root) noexcept
{
  while (LLINK(root) != Node::NullPtr)
    root = LLINK(root);

  return root;
}

    template <class Node> inline 
Node * find_max(Node * root) noexcept
{
  while (RLINK(root) != Node::NullPtr)
    root = RLINK(root);

  return root;
}

    template <class Node> inline 
Node * find_successor(Node * p, Node *& pp) noexcept
{ 
  assert(p != Node::NullPtr);
  assert(RLINK(p) != Node::NullPtr);

  pp = p;
  p  = RLINK(p);
  while (LLINK(p) != Node::NullPtr)
    {
      pp = p;
      p  = LLINK(p);
    }

  return p;
}

    template <class Node> inline
Node* find_predecessor(Node * p, Node *& pp) noexcept
{
  assert(p != Node::NullPtr);
  assert(LLINK(pp) != Node::NullPtr);

  pp = p;
  p  = LLINK(p);

  while (RLINK(p) != Node::NullPtr)
    {
      pp = p;
      p  = RLINK(p);
    }

  return p;
}

  template <class Node, class Compare> inline
Node * search_parent(Node * root, const typename Node::key_type & key, 
                     Node *& parent, Compare & cmp) noexcept
{
  assert((LLINK(parent) == root) or (RLINK(parent) == root));
  assert(root != Node::NullPtr);

  while (true)
    if (cmp(key, KEY(root)))
      { 
        if (LLINK(root) == Node::NullPtr)
          return root;

        parent = root;
        root = LLINK(root);
      }
    else if (cmp(KEY(root), key))
      {
        if (RLINK(root) == Node::NullPtr) 
          return root;

        parent = root;
        root = RLINK(root);
      }
    else
      return root;
}

  template <class Node, 
            class Compare = Aleph::less<typename Node::key_type> > inline
Node * search_parent(Node * root, const typename Node::key_type & key, 
                     Node *& parent, Compare && cmp = Compare()) noexcept
{
  return search_parent<Node, Compare>(root, key, parent, cmp); 
}

    template <class Node, 
	      class Compare = Aleph::less<typename Node::key_type> > 
    inline Node * 
search_rank_parent(Node * root, const typename Node::key_type & key,
		   Compare & cmp) noexcept
{
  assert(root != Node::NullPtr);

  while (true)
    if (cmp(key, KEY(root)))
      { 
        if (LLINK(root) == Node::NullPtr) 
          return root; 
        
        root = LLINK(root);
      }
    else if (cmp(KEY(root), key))
      {
        if (RLINK(root) == Node::NullPtr) 
          return root;

        root = RLINK(root);
      }
    else
      return root;
}

template <class Node, class Compare = Aleph::less<typename Node::key_type>>
inline Node * 
search_rank_parent(Node * root, const typename Node::key_type & key,
		   Compare && cmp = Compare()) noexcept
{
  return search_rank_parent<Node, Compare>(root, key, cmp);
}

template <class Node, class Compare> inline
Node * insert_in_bst(Node *& r, Node * p, Compare & cmp) noexcept
{
  if (r == Node::NullPtr) 
    return r = p;

  if (cmp(KEY(p), KEY(r)))
    return insert_in_bst<Node,Compare>(LLINK(r), p, cmp);
  else if (cmp(KEY(r), KEY(p)))
    return insert_in_bst<Node,Compare>(RLINK(r), p, cmp);

  return Node::NullPtr;
}

template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline Node * insert_in_bst(Node *& r, Node * p, Compare && cmp = Compare()) 
  noexcept
{
  return insert_in_bst(r, p, cmp);
}


  template <class Node, class Compare> inline
Node * insert_dup_in_bst(Node *& root, Node * p, Compare & cmp) noexcept
{
  if (root == Node::NullPtr) 
    return root = p;

  if (cmp(KEY(p), KEY(root)))
    return insert_dup_in_bst(LLINK(root), p, cmp);
  
  return insert_dup_in_bst(RLINK(root), p, cmp);
}

template <class Node, 
	  class Compare = Aleph::less<typename Node::key_type> > inline
Node * insert_dup_in_bst(Node *& root, Node * p, Compare && cmp = Compare()) 
  noexcept
{
  return insert_dup_in_bst<Node, Compare>(root, p, cmp);
}


  template <class Node, class Compare> inline
Node * search_or_insert_in_bst(Node *& r, Node * p, Compare & cmp) noexcept
{
  if (r == Node::NullPtr) 
    return r = p;

  if (cmp(KEY(p), KEY(r)))
    return search_or_insert_in_bst<Node, Compare>(LLINK(r), p, cmp);
  else if (cmp(KEY(r), KEY(p)))
    return search_or_insert_in_bst<Node, Compare>(RLINK(r), p, cmp);

  return r; 
}

template <class Node, 
	  class Compare = Aleph::less<typename Node::key_type>> inline
Node * search_or_insert_in_bst(Node *& r, Node * p, Compare && cmp = Compare())
noexcept
{
  return search_or_insert_in_bst<Node, Compare>(r, p, cmp); 
}

  
  template <class Node, class Compare> static inline
bool __split_key_rec(Node * root, const typename Node::key_type & key, 
		     Node *& ts, Node *& tg, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    {    // key no se encuentra en árbol ==> split tendrï¿½ ï¿½xito
      ts = tg = Node::NullPtr;
      return true;
    }

  if (cmp(key, KEY(root)))
    if (__split_key_rec(LLINK(root), key, ts, LLINK(root), cmp))
      {
        tg = root;
        return true;
      }
    else
      return false;

  if (cmp(KEY(root), key))
    if (__split_key_rec(RLINK(root), key, RLINK(root), tg, cmp))
      {
        ts = root;
        return true;
      }

  return false; // key exists in the tree
}


  template <class Node, class Compare> inline
bool split_key_rec(Node *& root, const typename Node::key_type & key, 
                   Node *& ts, Node *& tg, Compare & cmp) noexcept
{
  bool ret = __split_key_rec(root, key, ts, tg, cmp);
  if (ret)
    root = Node::NullPtr;
  return ret;
}

  template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline bool split_key_rec(Node *& root, const typename Node::key_type & key, 
			  Node *& ts, Node *& tg, Compare && cmp = Compare())
    noexcept
{
  return split_key_rec(root, key, ts, tg, cmp);
}


     template <class Node, class Compare> static inline
void __split_key_dup_rec(Node * root, const typename Node::key_type & key, 
			 Node *& ts, Node *& tg, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    {
      ts = tg = Node::NullPtr;
      return;
    }

  if (cmp(KEY(root), key))
    __split_key_dup_rec(RLINK(root), key, RLINK(root), tg, cmp);
  else 
    __split_key_dup_rec(LLINK(root), key, ts, LLINK(root), cmp);
}


  template <class Node, class Compare> inline
void split_key_dup_rec(Node *& root, const typename Node::key_type & key, 
                       Node *& ts, Node *& tg, Compare & cmp) noexcept
{
  __split_key_dup_rec(root, key, ts, tg, cmp);
  root = Node::NullPtr;
}

  template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline void 
split_key_dup_rec(Node *& root, const typename Node::key_type & key, 
		  Node *& ts, Node *& tg, Compare && cmp = Compare()) noexcept
{
  split_key_dup_rec(root, key, ts, tg, cmp);
}

template <class Node> inline 
Node * join_exclusive(Node *& ts, Node *& tg) noexcept
{
  if (ts == Node::NullPtr) 
    return tg;

  if (tg == Node::NullPtr) 
    return ts;

  LLINK(tg) = join_exclusive(RLINK(ts), LLINK(tg));

  RLINK(ts) = tg;
  Node * ret_val = ts;
  ts = tg = Node::NullPtr; // empty the trees

  return ret_val;
}  


  template <class Node, class Compare> inline 
Node * remove_from_bst(Node *&                         root, 
		       const typename Node::key_type & key,
		       Compare &                       cmp) noexcept
{
  if (root == Node::NullPtr) 
    return Node::NullPtr;

  if (cmp(key, KEY(root)))
    return remove_from_bst(LLINK(root), key);
  else if (cmp(KEY(root), key))
    return remove_from_bst(RLINK(root), key);

  Node * ret_val = root; // respaldar root que se va a borrar
  root = join_exclusive(LLINK(root), RLINK(root));

  ret_val->reset();

  return ret_val;
}


template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline Node * remove_from_bst(Node *& root, 
			      const typename Node::key_type & key,
			      Compare && cmp = Compare()) noexcept
{
  return remove_from_bst(root, key, cmp); 
}


  template <class Node, class Compare> inline
Node * insert_root(Node *& root, Node * p, Compare & cmp) noexcept
{
  Node * l = Node::NullPtr, * r = Node::NullPtr;

  if (not split_key_rec(root, KEY(p), l, r, cmp)) 
    return Node::NullPtr; 

  LLINK(p) = l;
  RLINK(p) = r;
  root = p;

  return root; 
}

  template <class Node, 
            class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_root(Node *& root, Node * p, Compare && cmp = Compare()) noexcept
{
  return insert_root(root, p, cmp); 
}

  template <class Node, class Compare> inline
Node * insert_dup_root(Node *& root, Node * p, Compare & cmp) noexcept
{
  split_key_dup_rec(root, KEY(p), LLINK(p), RLINK(p), cmp);
  return root = p;
}

  template <class Node, 
            class Compare = Aleph::less<typename Node::key_type> > inline
Node * insert_dup_root(Node *& root, Node * p, Compare && cmp = Compare()) 
    noexcept
{
  return insert_dup_root(root, p, cmp);
}


    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * join_preorder(Node * t1, Node * t2, Node *& dup, Compare & cmp) noexcept
{
  if (t2 == Node::NullPtr) 
    return t1;

  Node * l = LLINK(t2);
  Node * r = RLINK(t2);

  if (insert_in_bst(t1, t2, cmp) == Node::NullPtr)
    insert_in_bst(dup, t2, cmp); // insertion has failed

  join_preorder(t1, l, dup, cmp);
  join_preorder(t1, r, dup, cmp);

  return t1;
}

    template <class Node, 
              class Compare = Aleph::less<typename Node::key_type>> inline
Node * join_preorder(Node * t1, Node * t2, Node *& dup, 
		     Compare && cmp = Compare()) noexcept
{
  return join_preorder(t1, t2, dup, cmp);
}

    template <class Node, class Compare> inline
Node * join(Node * t1, Node * t2, Node *& dup, Compare & cmp) noexcept
{
  if (t1 == Node::NullPtr) 
    return t2;

  if (t2 == Node::NullPtr) 
    return t1;

  Node * l = LLINK(t1);
  Node * r = RLINK(t1);

  t1->reset();

  while (t1 != Node::NullPtr and insert_root(t2, t1, cmp) == Node::NullPtr) 
    {
      Node * p = remove_from_bst(t1, KEY(t1), cmp);
      l = LLINK(t1);
      r = RLINK(t1);

      assert(p != Node::NullPtr);

      insert_in_bst(dup, p, cmp);
    }

  LLINK(t2) = join(l, LLINK(t2), dup, cmp);
  RLINK(t2) = join(r, RLINK(t2), dup, cmp);

  return t2;
}

template <class Node, class Compare = Aleph::less<typename Node::key_type>> 
inline Node * join(Node * t1, Node * t2, Node *& dup, 
		   Compare && cmp = Compare()) noexcept
{
  return join(t1, t2, dup, cmp);
} 

    template <class Node> inline 
Node * rotate_to_right(Node * p) noexcept
{
  assert(p != Node::NullPtr);

  Node * q  = LLINK(p);
  LLINK(p) = RLINK(q);
  RLINK(q) = p;

  return q;           
} 

    template <class Node> inline 
Node * rotate_to_right(Node * p, Node * pp) noexcept
{
  assert(p != Node::NullPtr);
  assert(pp != Node::NullPtr);
  assert(LLINK(pp) == p or RLINK(pp) == p);

  Node * q = LLINK(p);
  LLINK(p) = RLINK(q);
  RLINK(q) = p;

  if (LLINK(pp) == p) // actualización del padre
    LLINK(pp) = q;
  else
    RLINK(pp) = q;

  return q;           
} 

    template <class Node> inline
Node* rotate_to_left(Node * p) noexcept
{
  assert(p != Node::NullPtr);

  Node *q  = RLINK(p);
  RLINK(p) = LLINK(q);
  LLINK(q) = p;

  return q;                  
} 

    template <class Node> inline
Node* rotate_to_left(Node * p, Node * pp) noexcept
{
  assert(p != Node::NullPtr);
  assert(pp != Node::NullPtr);
  assert(LLINK(pp) == p or RLINK(pp) == p);

  Node *q = RLINK(p);
  RLINK(p) = LLINK(q);
  LLINK(q) = p;

      // actualización del padre
  if (LLINK(pp) == p)
    LLINK(pp) = q;
  else
    RLINK(pp) = q;        
      
  return q;                  
} 

    template <class Node, class Key, class Compare> inline
void split_key(Node *& root, const Key & key, Node *& l, Node *& r, 
	       Compare & cmp) noexcept
{
  assert(l == Node::NullPtr and r == Node::NullPtr);
  if (root == Node::NullPtr) 
    {
      l = r = Node::NullPtr;
      return;
    }

  Node ** current_parent = nullptr;
  Node ** pending_child  = nullptr;
  char current_is_right;

  if (cmp(key, KEY(root)))
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

template <class Node, class Key, 
	  class Compare = Aleph::less<typename Node::key_type>> inline
void split_key(Node *& root, const Key & key, Node *& l, Node *& r, 
	       Compare && cmp = Compare()) 
	    noexcept
{
  split_key(root, key, l, r, cmp);
}

    template <class Node> inline
void swap_node_with_successor(Node *  p,  // Node for swapping
                              Node *& pp, // parent of p
                              Node *  q,  // Successor inorder of p 
                              Node *& pq) // parent of q
      noexcept
{
  assert(p != Node::NullPtr and q != Node::NullPtr and 
    pp != Node::NullPtr and pq != Node::NullPtr);
  assert(LLINK(pp) == p or RLINK(pp) == p); 
  assert(LLINK(pq) == q or RLINK(pq) == q); 
  assert(LLINK(q) == Node::NullPtr);        
     
  /* Set of pp to its new son q */ 
  if (LLINK(pp) == p)
    LLINK(pp) = q;
  else
    RLINK(pp) = q;
     
  LLINK(q) = LLINK(p);
  LLINK(p) = Node::NullPtr; 

  /* Checks if successor is right child of p. In this case, p will
     become q's son. This situation happens when p's son does not have
     a left branch */   
  if (RLINK(p) == q) 
    {
      RLINK(p) = RLINK(q);
      RLINK(q) = p;
      pq             = pp;
      pp             = q;
      return;
    }

  /* In this case, successor is the leftmost node descending from
     right son of p */ 
  Node *qr        = RLINK(q); 
  RLINK(q)  = RLINK(p);
  LLINK(pq) = p;
  RLINK(p)  = qr;

  std::swap(pp, pq);
}

    template <class Node> inline
void swap_node_with_predecessor(Node *  p,  // Node for swapping
                                Node *& pp, // p's parent
                                Node *  q,  // Predecessor inorder of p 
                                Node *& pq) // q's parent
      noexcept
{
  assert((p != Node::NullPtr) and (q != Node::NullPtr) and 
    (pp != Node::NullPtr) and (pq != Node::NullPtr));
  assert((RLINK(pp) == p) or (LLINK(pp) == p)); 
  assert((RLINK(pq) == q) or (LLINK(pq) == q)); 
  assert(RLINK(q) == Node::NullPtr);
     
  /* Set of pp to its new son q */ 
  if (RLINK(pp) == p)
    RLINK(pp) = q;
  else
    LLINK(pp) = q;
     
  RLINK(q) = RLINK(p);
  RLINK(p) = Node::NullPtr; 

  /* Checks if predecessor is left child of p. In this case, p will
     become q's son. This situation happens when p's son does not have
     a right branch */   
  if (LLINK(p) == q) 
    {
      LLINK(p) = LLINK(q);
      LLINK(q) = p;
      pq             = pp;
      pp             = q;
      return;
    }

  /* In this case, predecessor is the rightmost node descending from
     right son of p */ 
  Node *ql        = LLINK(q); 
  LLINK(q)  = LLINK(p);
  RLINK(pq) = p;
  LLINK(p)  = ql;
  std::swap(pp, pq);
}

    template <class Node, class Key, class Compare> inline
Node * insert_root_rec(Node * root, Node * p, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    return p; /* insertion in empty tree */

  if (cmp(KEY(p), KEY(root)))
    { /* insert in left subtree */
      Node *left_branch = insert_root_rec(LLINK(root), p, cmp);
      if (left_branch == Node::NullPtr)
        return Node::NullPtr;

      LLINK(root) = left_branch;
      root              = rotate_to_right(root);
    }
  else if (cmp(KEY(root), KEY(p)))
    { /* insert in right subtree */
      Node *right_branch = insert_root_rec(RLINK(root), p, cmp);
      if (right_branch == Node::NullPtr)
        return Node::NullPtr;

      RLINK(root) = right_branch;
      root              = rotate_to_left(root);
    }

  return Node::NullPtr; /* duplicated key */
}

 template <class Node, class Key, 
	   class Compare = Aleph::less<typename Node::key_type>> inline
Node * insert_root_rec(Node * root, Node * p, Compare && cmp = Compare()) 
	     noexcept
{
  return insert_root_rec(root, p, cmp);
}

    template <class Node, class Key, 
              class Compare = Aleph::less<typename Node::key_type> > inline
Node * search_or_insert_root_rec(Node * root, Node * p, Compare & cmp) noexcept
{
  if (root == Node::NullPtr)
    return p; // insertion in empty tree

  if (cmp(KEY(p), KEY(root)))
    { // insert in left subtree 
      Node * left_branch = search_or_insert_root_rec(LLINK(root), p, cmp);
      if (left_branch == p)
        {
          LLINK(root) = left_branch;
          root              = rotate_to_right(root);
          return p;
        }

        return left_branch; 
    }
  else if (cmp(KEY(root), KEY(p)))
    { // insert in right subtree 
      Node * right_branch = search_or_insert_root_rec(RLINK(root), p, cmp);
      if (right_branch == p)
        {
          RLINK(root) = right_branch;
          root              = rotate_to_left(root);
          return p;
        }

      return right_branch;
    }

  return root;
}

template <class Node, class Key, 
	  class Compare = Aleph::less<typename Node::key_type> > inline
Node * search_or_insert_root_rec(Node * root, Node * p, 
				 Compare && cmp = Compare()) noexcept
{
  return search_or_insert_root_rec(root, p, cmp);
}


template <class Node>
class BinNodePrefixIterator
{
  Node * root = nullptr;
  Node * curr = Node::NullPtr;
  ArrayStack<Node*> s;

public:

  void swap(BinNodePrefixIterator & it)
  {
    std::swap(root, it.root);
    std::swap(curr, it.curr);
    s.swap(it.s);
  }

  BinNodePrefixIterator() { /* empty */ }
  
  BinNodePrefixIterator(Node * __root) 
    : root(__root), curr(root), s(Node::MaxHeight) 
  {
    // empty
  }

  BinNodePrefixIterator(const BinNodePrefixIterator & it)
    : root(it.root), curr(it.curr), s(it.s)
  {
    // empty
  }

  BinNodePrefixIterator(BinNodePrefixIterator && it)
  {
    swap(it);
  }

  void reset_first() 
  {
    curr = root;
    s.empty();
  }

private:

  // Helper function for finding last node
  static Node * last(Node * p)
  {
    if (RLINK(p) != Node::NullPtr)
      return last(RLINK(p));

    if (LLINK(p) != Node::NullPtr)
      return last(LLINK(p));

    return p;
  }

public:

  void reset_last()
  {
    curr = last(root);
    s.empty();
  } 

  void end()
  {
    curr = Node::NullPtr;
    s.empty();
  }

  BinNodePrefixIterator & operator = (const BinNodePrefixIterator & it)
  {
    if (this == &it)
      return *this;

    root = it.root;
    curr = it.curr;
    s = it.s;
    return *this;
  }

  BinNodePrefixIterator & operator = (BinNodePrefixIterator && it)
  {
    swap(it);
    return *this;
  }

  bool has_curr() const noexcept { return curr != Node::NullPtr; }

  bool has_current() const noexcept { return has_curr(); }

  Node * get_curr() const
  {
    if (not has_curr())
      throw std::overflow_error("Iterator overflow");

    return curr;
  }

  Node * get_current() const { return get_curr(); }

  void next()
  {
    if (not has_curr())
      throw std::overflow_error("Iterator overflow");

    auto l = LLINK(curr), r = RLINK(curr);
    if (l != Node::NullPtr)
      {
	curr = l;
	if (r != Node::NullPtr)
	  s.push(r);
	return;
      }
      
    if (r != Node::NullPtr)
      {
	curr = r;
	return;
      }

    if (s.is_empty())
      curr = Node::NullPtr;
    else
      curr = s.pop();
  }
};


template <class Node>
class BinNodeInfixIterator
{
  mutable Node * root = Node::NullPtr;
  Node * curr         = Node::NullPtr;
  long pos            = 0;
  ArrayStack<Node*> s;

  Node * advance_to_min(Node * r)
  {
    while (LLINK(r) != Node::NullPtr)
      {
	s.push(r);
	r = LLINK(r);
      }
    return r;
  }

  static Node * advance_to_max(Node * r)
  {
    while (RLINK(r) != Node::NullPtr)
      r = RLINK(r);

    return r;
  }

  void init()
  {
    if (root != Node::NullPtr)
      curr = advance_to_min(root);
    pos = 0;
  }

public:

  void swap(BinNodeInfixIterator & it)
  {
    std::swap(root, it.root);
    std::swap(curr, it.curr);
    std::swap(pos, it.pos);
    s.swap(it.s);
  }

  BinNodeInfixIterator() { /* empty */ }

  BinNodeInfixIterator(Node * __root) : root(__root), s(Node::MaxHeight) 
  {
    init();
  }

  BinNodeInfixIterator(const BinNodeInfixIterator & it)
    : root(it.root), curr(it.curr), pos(it.pos), s(it.s)
  {
    // empty
  }

  BinNodeInfixIterator(BinNodeInfixIterator && it) { swap(it); }

  void reset_first()
  {
    s.empty();
    init();
  }

  void reset_last()
  {
    s.empty();
    curr = advance_to_max(root);
    pos = -2;
  }
  
  void end() 
  {
    s.empty();
    curr = Node::NullPtr;
    pos = -1;
  }

  BinNodeInfixIterator & operator = (const BinNodeInfixIterator & it)
  {
    if (this == &it)
      return *this;

    root = it.root;
    curr = it.curr;
    pos = it.pos;
    s = it.s;
    return *this;
  }

  BinNodeInfixIterator & operator = (BinNodeInfixIterator && it)
  {
    swap(it);
    return *this;
  }

  bool has_curr() const noexcept { return curr != Node::NullPtr; }

  bool has_current() const noexcept { return has_curr(); }

  Node * get_curr() const 
  {
    if (not has_curr())
      throw std::overflow_error("Iterator overflow");
    return curr;
  }

  Node * get_current() const { return get_curr(); }

  size_t get_pos() const { return pos; }

  void next()
  {
    if (not has_curr())
      throw std::overflow_error("Iterator overflow");

    ++pos;
    curr = RLINK(curr);
    if (curr != Node::NullPtr)
      {
	curr = advance_to_min(curr);
	return;
      }

    if (s.is_empty())
      curr = Node::NullPtr;
    else
      curr = s.pop();
  }
};


} // end Aleph

# endif // TPL_BINNODEUTILS_H 
