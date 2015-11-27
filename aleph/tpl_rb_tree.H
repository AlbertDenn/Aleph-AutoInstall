
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


# ifndef TPL_RB_TREE_H
# define TPL_RB_TREE_H

# include <ahFunction.H>
# include <tpl_arrayStack.H>
# include <tpl_binNodeUtils.H>
# include <rbNode.H>

using namespace Aleph;

namespace Aleph {

  template <template <typename> class NodeType, typename Key, class Compare>
class Gen_Rb_Tree 
{
public:

  typedef NodeType<Key> Node;

private:

  Node              head_node; // cabecera centinela
  Node *            head;      // puntero a centinela
  Node *&           root; 
  FixedStack<Node*> rb_stack; 
  Compare &         cmp;

  Node * search_and_stack_rb(const Key & key)             
  {
    Node * p = root; 
    rb_stack.push(head);
    do 
      {
        rb_stack.push(p); 
        if (cmp(key, KEY(p)))
          p = LLINK(p);
        else if (cmp(KEY(p), key))
          p = RLINK(p);
        else
          return p;
      }
    while (p != Node::NullPtr);

    return rb_stack.top();
  }

  Node * search_dup_and_stack_rb(const Key & key)                 
  {
    Node * p = root; 
    rb_stack.push(head);
    do 
      {
        rb_stack.push(p); 
        if (cmp(key, KEY(p)))
          p = LLINK(p);
        else 
          p = RLINK(p);
      }
    while (p != Node::NullPtr);

    return rb_stack.top();
  }

  void fix_red_condition(Node * p)
  {
    assert(COLOR(p) == RED);

    while (p != root) 
      {
        Node * pp = rb_stack.pop(); // padre de p
        if (COLOR(pp) == BLACK) // ¿padre de p negro?
          break; // sí ==> no hay rojos consecutivos ==> terminar

        if (root == pp) // ¿p es hijo directo de la raíz?
          {    // sí ==> colorear raíz de negro y terminar
            COLOR(root) = BLACK; 
            break; 
          }

        Node * ppp = rb_stack.pop(); // abuelo de p
        Node * spp = LLINK(ppp) == pp ? RLINK(ppp) : LLINK(ppp); // tío
        if (COLOR(spp) == RED) // ¿tío de p rojo?
          {     // intercambiar colores entre los niveles
            COLOR(ppp) = RED;
            COLOR(pp)  = BLACK;
            COLOR(spp) = BLACK;
            p = ppp;
            continue; // ir próximo ancestro, verificar violaciones
          }

        Node * pppp = rb_stack.pop(); // bisabuelo de p
        if (LLINK(pp) == p and LLINK(ppp) == pp)
          {
            rotate_to_right(ppp, pppp);
            COLOR(pp) = BLACK;
          }
        else if (RLINK(pp) == p and RLINK(ppp) == pp)
          {
            rotate_to_left(ppp, pppp);
            COLOR(pp) = BLACK;
          }
        else 
          {
            if (RLINK(pp) == p)
              {
                rotate_to_left(pp, ppp);
                rotate_to_right(ppp, pppp);
              }
            else
              {
                rotate_to_right(pp, ppp);
                rotate_to_left(ppp, pppp);
              }
            COLOR(p) = BLACK;
          }

        COLOR(ppp) = RED;
        break; // árbol es rojo-negro ==> terminar
      }

     rb_stack.empty();
  }

  void find_succ_and_swap(Node * p, Node *& pp)
  {
    Node *& ref_rb_stack = rb_stack.top(); 

    /* Find successor while updating rb_stack */
    Node * fSucc = p;        // successor's parent
    Node * succ  = RLINK(p); // Searching starts from p's right child 
    rb_stack.push(succ);

    while (LLINK(succ) != Node::NullPtr)  // go down to leftmost
      {
        fSucc = succ;
        succ  = LLINK(succ);
        rb_stack.push(succ);
      }

    ref_rb_stack   = succ; /* swap old top with current top */
    rb_stack.top() = p; 

    if (LLINK(pp) == p) /* Setting of parent of p to new child(succ) */ 
      LLINK(pp) = succ;
    else
      RLINK(pp) = succ;

    LLINK(succ) = LLINK(p); /* Swaps left branches */
    LLINK(p)    = Node::NullPtr; 

    if (RLINK(p) == succ) /* For right branches there are two cases */
      { /* successor is just right child of p */   
	RLINK(p)    = RLINK(succ); 
	RLINK(succ) = p;
	pp          = succ;
      }
    else
      { /* Successor is leftmost node descending from right child of p */ 
	Node *succr  = RLINK(succ); 
	RLINK(succ)  = RLINK(p);
	LLINK(fSucc) = p;
	RLINK(p)     = succr;
	pp           = fSucc;
      }

    std::swap(COLOR(succ), COLOR(p));
  }

  void fix_black_condition(Node * p)
  {
    if (COLOR(p) == RED) // ¿p es rojo?
      {     // sí ==> lo pintamos de negro y terminamos
        COLOR(p) = BLACK; // esto compensa el déficit 
        return;
      }

    Node * pp = rb_stack.popn(2); // padre de p
    while (p != root)
      {
        assert(LLINK(pp) == p or RLINK(pp) == p);
        assert(LLINK(rb_stack.top()) == pp or RLINK(rb_stack.top()) == pp);

        Node * sp = LLINK(pp) == p ? RLINK(pp) : LLINK(pp); // hermano p
        if (COLOR(sp) == RED) // ¿hermano de p es rojo?
          {
            Node *& ppp = rb_stack.top(); // abuelo de p

            if (LLINK(pp) == p)
              {
                sp  = LLINK(sp);
                ppp = rotate_to_left(pp, ppp);
              }
            else
              {
                sp  = RLINK(sp);
                ppp = rotate_to_right(pp, ppp);
              }

            COLOR(ppp) = BLACK;
            COLOR(pp)  = RED;
          }

        Node * np, * snp; // sobrinos de nodo p
        if (LLINK(pp) == p) // ¿p es hijo izquierdo?
          {     // sí ==> que sp es hijo derechos
            np  = RLINK(sp);
            snp = LLINK(sp);
          }
        else
          {
            np  = LLINK(sp);
            snp = RLINK(sp);
          }

        if (COLOR(np) == RED) // ¿np es rojo?
          {
            Node * ppp = rb_stack.top();

            if (RLINK(sp) == np)
              rotate_to_left(pp, ppp);
             else
              rotate_to_right(pp, ppp);

            COLOR(sp) = COLOR(pp);
            COLOR(pp) = BLACK;
            COLOR(np) = BLACK;

            return;
          }

        if (COLOR(snp) == RED) // ¿snp es rojo?
          {
            Node * ppp = rb_stack.top();

            if (LLINK(sp) == snp)
              {
                rotate_to_right(sp, pp);
                rotate_to_left(pp, ppp);
              }
            else
              {
                rotate_to_left(sp, pp);
                rotate_to_right(pp, ppp);
              }

            COLOR(snp) = COLOR(pp);
            COLOR(pp)  = BLACK;;

            return;
          }

        if (COLOR(pp) == RED) // ¿pp es rojo?
          {
            COLOR(pp) = BLACK;
            COLOR(sp) = RED;;
            return;
          }

          // no hay nodo rojo en las adyacencias de p ==> desplazar el
          // déficit hacia pp y repetir la iteración
        COLOR(sp) = RED;
        p         = pp;
        pp        = rb_stack.pop(); 
      }
  }

public:

  typedef Key key_type;

  Compare & key_comp() { return cmp; }

  Compare & get_compare() { return key_comp(); }

  Gen_Rb_Tree(Compare & __cmp) 
    : head(&head_node), root(head_node.getR()), 
      rb_stack(Node::MaxHeight), cmp(__cmp)
  {
    /* empty */ 
  }

  Gen_Rb_Tree(Compare && cmp = Compare()) : Gen_Rb_Tree(cmp) { /* empty */ }

  void swap (Gen_Rb_Tree & tree)
  {
    std::swap(root, tree.root);
    std::swap(cmp, tree.cmp);
  }

  virtual ~Gen_Rb_Tree() { /* empty */ }

  Node * search(const Key & key)
  {
    Node * retVal = Aleph::searchInBinTree <Node, Compare> (root, key, cmp);
    return retVal == Node::NullPtr ? nullptr : retVal;
  }

  Node *& getRoot() { return root; }
  Node * insert(Node * p)
  {
    assert(COLOR(p) == RED);

    if (root == Node::NullPtr) 
      return root = p; // inserción en árbol vacío

    Node * q = search_and_stack_rb(KEY(p));
    if (cmp(KEY(p), KEY(q))) 
      LLINK(q) = p;
    else if (cmp(KEY(q), KEY(p))) 
      RLINK(q) = p;
    else
      {
        rb_stack.empty();
        return nullptr;  // clave duplicada
      }
    fix_red_condition(p);

    return p;
  }

  Node * search_or_insert(Node * p)
  {
    assert(COLOR(p) == RED);

    if (root == Node::NullPtr) 
      return root = p; // inserción en árbol vacío

    Node * q = search_and_stack_rb(KEY(p));
    if (cmp(KEY(p), KEY(q))) 
      LLINK(q) = p;
    else if (cmp(KEY(q), KEY(p))) 
      RLINK(q) = p;
    else
      {
        rb_stack.empty();
        return q;  // clave duplicada
      }
    fix_red_condition(p);

    return p;
  }

  Node * insert_dup(Node * p)
  {
    assert(COLOR(p) == RED);

    if (root == Node::NullPtr) 
      return root = p; // inserción en árbol vacío

    Node * q = search_dup_and_stack_rb(KEY(p));
    if (cmp(KEY(p), KEY(q))) 
      LLINK(q) = p;
    else 
      RLINK(q) = p;

    fix_red_condition(p);

    return p;
  }

  bool verify() const { return is_red_black_tree(root) ; }

  Node* remove(const Key & key)
  {
    if (root == Node::NullPtr) 
      return nullptr;

    Node * q = search_and_stack_rb(key);
    if (no_equals<Key, Compare> (KEY(q), key, cmp)) // ¿clave no encontrada?
      {    
        rb_stack.empty();
        return nullptr;
      }

    Node * pq = rb_stack.top(1); // padre de 1
    Node * p; // hijo de q luego de que éste ha sido eliminado
    while (true) // eliminación clásica árbol binario de búsqueda
      {
        if (LLINK(q) == Node::NullPtr) 
          {
            if (LLINK(pq) == q)
              p = LLINK(pq) = RLINK(q);
            else
              p = RLINK(pq) = RLINK(q);

            break; // goto end;
          }

        if (RLINK(q) == Node::NullPtr) 
          {
            if (LLINK(pq) == q)
              p = LLINK(pq) = LLINK(q);
            else
              p = RLINK(pq) = LLINK(q);

            break; // goto end;
          }

        find_succ_and_swap(q, pq);
      }

    if (COLOR(q) == BLACK) // ¿se eliminó un nodo negro?
      fix_black_condition(p);

    q->reset();
    rb_stack.empty();

    return q;
  }

  struct Iterator : public BinNodeInfixIterator<Node>
  {
    Iterator(Gen_Rb_Tree & t) : BinNodeInfixIterator<Node>(t.getRoot()) {}
  };
};
    template <typename Key, class Compare = Aleph::less<Key>>
struct Rb_Tree : public Gen_Rb_Tree<RbNode, Key, Compare> 
{
  using Base = Gen_Rb_Tree<RbNode, Key, Compare>;
  using Base::Base;
};

    template <typename Key, class Compare = Aleph::less<Key>>
struct Rb_Tree_Vtl : public Gen_Rb_Tree<RbNodeVtl, Key, Compare> 
{
  using Base = Gen_Rb_Tree<RbNodeVtl, Key, Compare>;
  using Base::Base;
};

} // end namespace Aleph

# endif // TPL_RB_TREE_H

