
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
       Aleph implementation of multimap C++ standard container
*/

# ifndef ALEPH_MULTIMAP_H
# define ALEPH_MULTIMAP_H

# include <ah_stdc++_utils.H>
# include <tpl_treapRk.H>
# include <tpl_nodePool.H>

namespace Aleph
{

template <typename Key, 
	  typename T, 
	  class Compare = Aleph::less<Key>,
	  template <class, class> class KTree = Treap_Rk,
	  template <class, class> class TTree = Treap_Rk
    >
class multimap
{
      // Definición de registro conteniendo elem de tipo T
  struct Tdata
  {
    T elem;          // elemento mapeado a una clave de tipo key
    size_t num_reps; // número de ocurrencias de elem relacionado a un
		     // valor de clave key

    Tdata() : num_reps(0) { /* empty */ }

    Tdata(const T & e) : elem(e), num_reps(0) { /* empty */ }

    Tdata(const Tdata & item) : elem(item.elem), num_reps(item.num_reps) 
    {
      // empty
    }
  };

  typedef Aleph::less<T> Tless;

  struct Cmpt // Clase de comparación para clave asociada
  {
    bool operator () (const Tdata & op1, const Tdata & op2) const
    {
      return Tless () (op1.elem, op2.elem);
    }
  };

      // definición de árbol de claves elementos mapeados
  typedef TTree<Tdata, Cmpt> T_Tree;

  typedef typename T_Tree::Node Tnode;

  typedef Node_Pool<Tnode> Tpool;

      // Definición de registro conteniendo elementos del multimapeo
  struct Kdata
  {
    Key    key;      // clave primaria
    size_t num_reps; // Número de repeticiones del valor de key
    T_Tree t_tree;   // árbol de elementos de tipo T asociados a key
    
    Kdata() : num_reps(0) { /* empty */ }

    Kdata(const Kdata & item) : key(item.key), num_reps(0)
    {
      t_tree.getRoot() = copyRec(const_cast<Kdata&>(item).t_tree.getRoot());
      num_reps = item.num_reps;
    }

    ~Kdata()
    {
      destroyRec(t_tree.getRoot());
    }

    Kdata & operator = (const Kdata & item) 
    {
      if (this == &item)
      	return *this;

      destroyRec(t_tree.getRoot());
      num_reps = 0;
      t_tree.getRoot() = copyRec(const_cast<Kdata&>(item).t_tree.getRoot());

      key      = item.key;
      num_reps = item.num_reps;

      return *this;
    }
  };

  struct Cmpk // clase de comparación para clave
  {
    bool operator () (const Kdata & op1, const Kdata & op2) const
    {
      return Compare () (op1.key, op2.key);
    }
  };

  typedef KTree<Kdata, Cmpk> K_Tree;

  typedef typename K_Tree::Node Knode;

      // miembros dato
  Node_Pool<Knode> kpool;
  Tpool            tpool;

  K_Tree           k_tree;

  size_t           num_elem;

  // Para ahorrar memoria, el árbol ktree indiza por registros de tipo
  // Kdata y la función de comparación siguiente se encarga de sólo
  // comparar la clave. Ahora bien, es muy importante, para no tener una
  // penalidad en desempeño, que no se desperdicie tiempo en el
  // constructor de Kdata cada vez que se requiera realizar una
  // búsqueda. Así el siguiente campo se crea una sola vez y se emplea
  // para las búsquedas. 
  Kdata                 searching_kdata; 

      // esta función se emplea cuando se desee buscar key y se requiera
      // obtener un Kdata
  Kdata & key_kdata(const Key & key) 
  {
    searching_kdata.key = key;
    return searching_kdata;
  }
  
public:

  multimap() : kpool(100), tpool(100), num_elem(0) { /* empty */ }

  ~multimap()
  {
    destroyRec(k_tree.getRoot());
  }

  void clear()
  {
    destroyRec(k_tree.getRoot());
    num_elem = 0;
  }

  typedef std::pair<Key, T> Pair;

  typedef Pair value_type;

  typedef typename multimap::value_type & reference;

  typedef const typename multimap::value_type & const_reference;

  typedef size_t size_type;

  typedef Key key_type;

  typedef T mapped_type;

  const size_t & size () const { return num_elem; }

  size_type max_size() const 
  { 
    int sizek = sizeof(Knode);
    int sizet = sizeof(Tnode);

    double total_mem = pow(2, __WORDSIZE);

    size_type ret_val = floor(total_mem / (sizek + sizet));

    return ret_val;
  }

  bool empty () const {return k_tree.is_empty(); }

private:

  typedef typename K_Tree::Iterator K_Itor;
  typedef typename T_Tree::Iterator T_Itor;

public:

  class iterator
  {
    friend class multimap<Key, T, Compare, KTree, TTree>;

    mutable multimap * multimap_ptr;
    mutable K_Tree *   k_tree_ptr;
    mutable K_Itor     k_it;
    mutable T_Tree *   t_tree_ptr;
    mutable T_Itor     t_it;
    mutable int        pos_in_k;
    mutable int        pos_in_t;
    bool               underflow;
    bool               overflow;
    Pair               ret_pair; // Usado por el operador ->

    void default_init()
    {
      assert(k_tree_ptr != nullptr);

      if (k_it.has_curr())
	{
	  assert(KEY(k_it.get_curr()).t_tree.size() > 0);
	  underflow = overflow = false;
	  pos_in_k = pos_in_t = 0;
	  t_tree_ptr = &KEY(k_it.get_curr()).t_tree;
	  t_it = T_Itor(*t_tree_ptr);
	}
      else
	put_in_overflow();
    }

    iterator(multimap * m, Knode * kp, Tnode * tp, int kpos = 0, int tpos = 0)
      : multimap_ptr(m), k_tree_ptr(&m->k_tree), k_it(*k_tree_ptr, kp),
	t_tree_ptr(&KEY(kp).t_tree), t_it(*t_tree_ptr, tp), 
	pos_in_k(kpos), pos_in_t(tpos), underflow(false), overflow(false)
    {
      assert(not KEY(kp).t_tree.is_empty());
      assert(KEY(tp).num_reps > 0 and tpos < KEY(tp).num_reps);
    }

    iterator(multimap * m, Knode * p)
      : multimap_ptr(m), k_tree_ptr(&m->k_tree), k_it(*k_tree_ptr, p),
	t_tree_ptr(&KEY(p).t_tree), t_it(*t_tree_ptr),
	pos_in_t(0), underflow(false), overflow(false)
    {
      assert(KEY(p).num_reps > 0);
      assert(not KEY(p).t_tree.is_empty());
    }

  public:

    typedef typename multimap::value_type   value_type;

    typedef typename multimap::size_type    difference_type;

    typedef typename multimap::value_type * pointer;

    typedef typename multimap::reference    reference;

    iterator(const multimap & mm) 
      : multimap_ptr(const_cast<multimap*>(&mm)), 
	k_tree_ptr(&multimap_ptr->k_tree), k_it(*k_tree_ptr)
    {
      default_init();
    }

    iterator(const iterator & it) 
      : multimap_ptr(it.multimap_ptr), k_tree_ptr(it.k_tree_ptr), k_it(it.k_it),
	t_tree_ptr(it.t_tree_ptr), t_it(it.t_it), 
	pos_in_k(it.pos_in_k), pos_in_t(it.pos_in_t), 
	underflow(it.underflow), overflow(it.overflow)
    {
      // empty
    }

    iterator()
      : multimap_ptr(nullptr), k_tree_ptr(nullptr), t_tree_ptr(nullptr),
	pos_in_k(-1), pos_in_t(-1), underflow(true), overflow(true)
    {
      // empty
    }

    iterator & operator = (const iterator & it) 
    {
      if (this == &it)
	return *this;

      multimap_ptr = it.multimap_ptr;
      k_tree_ptr   = it.k_tree_ptr;
      k_it         = it.k_it;
      t_tree_ptr   = it.t_tree_ptr;
      t_it         = it.t_it; 
      pos_in_k     = it.pos_in_k;
      pos_in_t     = it.pos_in_t;
      underflow    = it.underflow;
      overflow     = it.overflow;

      return *this;
    }

  private:

    bool has_curr() const { return k_it.has_curr(); }

    Knode * get_curr() { return k_it.get_curr(); }

    Kdata & get_curr_kdata() { return KEY(get_curr()); }

  public:

    value_type  operator * () 
    {
      return value_type(get_curr_kdata().key, KEY(t_it.get_curr()).elem);
    }

    const value_type * operator -> () 
    {
      ret_pair.first  = get_curr_kdata().key;
      ret_pair.second = KEY(t_it.get_curr()).elem;

      return &ret_pair;
    }

  private:

    void goto_begin()
    {
      k_it.reset_first();
      if (not has_curr())
	{
	  put_in_underflow();
	  return;
	}

      t_tree_ptr = &KEY(get_curr()).t_tree;
      t_it = T_Itor(*t_tree_ptr);
      pos_in_k = pos_in_t = 0;
    }

    void goto_last()
    {
      k_it.reset_last();
      if (not has_curr())
	{
	  put_in_overflow();
	  return;
	}

      overflow = false;
      Kdata & kdata = KEY(get_curr());
      t_tree_ptr = &kdata.t_tree;
      t_it = T_Itor(*t_tree_ptr);
      t_it.reset_last();
      pos_in_k = kdata.num_reps - 1;
      pos_in_t = KEY(t_it.get_curr()).num_reps - 1;
    }

    void goto_end()
    {
      k_it.reset_last();
      if (has_curr())
	{
	  k_it.next(); // lo coloca fuera de rango
	  underflow = false;
	}
      else
	put_in_underflow();

      put_in_overflow();
    }

    iterator compute_end() const
    {
      iterator it(*this);
      it.goto_end();
      assert(it.overflow);
      return it;
    }

    bool is_at_end() const { return not has_curr(); }

    iterator compute_begin() const
    {
      iterator it(*this);
      return it;
    }

    void put_in_overflow()
    {
      t_tree_ptr = nullptr;

      if (k_tree_ptr->is_empty())
	put_in_underflow();

      overflow = true;
    }
    
    void put_in_underflow()
    {
      t_tree_ptr = nullptr;
      
      pos_in_t = -1;
      underflow = true;
    }

         // avanza todas las claves de la posición actual
    void forward_k_it()
    {
      k_it.next();
      if (not has_curr())
	{
	  put_in_overflow();
	  return;
	}

      t_tree_ptr = &KEY(get_curr()).t_tree;
      t_it = T_Itor(*t_tree_ptr);
      pos_in_t = 0;
    }

    void forward_tree_iterators()
    {
      t_it.next();
      if (t_it.has_curr())
	{
	  pos_in_t = 0;
	  return;
	}
	  
      forward_k_it();
    }

    void forward()
    {
      if (underflow)
	{
	  goto_begin();
	  return;
	}

      if (overflow)
	{
	  assert(t_tree_ptr == nullptr);
	  throw std::overflow_error("Multimap::iterator is already "
				    "in overflow");
	}

      assert(t_it.has_curr() and t_tree_ptr != nullptr);

      Tdata & tdata = KEY(t_it.get_curr());
      if (++pos_in_t < tdata.num_reps) // alcanza el último elemento 
	return; // aún no!

      forward_tree_iterators();
    }

    void backward_k_it()
    {
      k_it.prev();
      if (not has_curr())
	{
	  put_in_underflow();
	  return;
	}

      t_tree_ptr = &KEY(get_curr()).t_tree;
      t_it = T_Itor(*t_tree_ptr);
      t_it.reset_last();
      pos_in_t = KEY(t_it.get_curr()).num_reps - 1;
    }

    void backward_tree_iterators()
    {
      t_it.prev();
      if (t_it.has_curr())
	{
	  pos_in_t = KEY(t_it.get_curr()).num_reps - 1;
	  return;
	}
	  
      backward_k_it();
    }

    void backward()
    {
      if (overflow)
	{
	  goto_last();
	  return;
	}

      if (underflow)
	{
	  assert(t_tree_ptr == nullptr);
	  throw std::underflow_error("Multimap::iterator is "
				     "already in underflow");
	}

      assert(t_it.has_curr() and t_tree_ptr != nullptr);

      if (--pos_in_t >= 0) // alcanza el primer elemento 
	return;

      backward_tree_iterators();
    }

    void del()
    {
      Kdata & kdata = get_curr_kdata();
      Tnode * tp = t_it.get_curr();
      Tdata & tdata = KEY(tp);

      --multimap_ptr->num_elem;
      --kdata.num_reps;
      if (--tdata.num_reps == 0)
	{
	  multimap_ptr->tpool.deallocate(t_it.del()); // elimine y libere tp
	  pos_in_t = 0;
	}
      else if (pos_in_t == tdata.num_reps)
	{
	  t_it.next();
	  pos_in_t = 0;
	}

      if (t_it.has_curr())
	{
	  assert(kdata.num_reps > 0);
	  return;
	}

      if (kdata.num_reps == 0)
	{
	  Knode * kp = k_it.del();
	  assert(KEY(kp).t_tree.is_empty());
	  multimap_ptr->kpool.deallocate(kp);
	}
      else
	k_it.next();

      if (not k_it.has_curr())
	{
	  put_in_overflow();
	  return;
	}

      t_tree_ptr = &KEY(get_curr()).t_tree;
      t_it = T_Itor(*t_tree_ptr);
      pos_in_k = pos_in_t = 0;
    }

  public:

    bool operator == (const iterator & it) const
    {
      if (this->has_curr() and it.has_curr())
	return (t_it.get_curr() == it.t_it.get_curr() and 
		pos_in_t == it.pos_in_t);

      if (this->is_at_end() and it.is_at_end())
	{
	  assert(this->overflow and it.overflow);
	  return true;
	}
      
      return false;
    }
     
    bool operator != (const iterator & itor) const
    {
      return not (*this == itor);
    }
 
    iterator operator ++ ()
    {
      forward();
      return *this;
    }

    iterator operator ++ (int)
    {
      iterator ret_val = *this;
      forward();
      return ret_val;
    }

    iterator operator -- ()
    {
      backward();
      return *this;
    }

    iterator operator -- (int)
    {
      iterator ret_val = *this;
      backward();
      return ret_val;
    }

    iterator operator += (size_type n)
    {
      if (n == 0)
	return *this;

      while (true) // avanzar por nodos en t_it
	{
	  assert(KEY(t_it.get_curr()).num_reps > 0);

	  const size_t treps = KEY(t_it.get_curr()).num_reps;
	  const int remain_in_t_node = treps - pos_in_t;
	  if (n < remain_in_t_node)
	    {
	      pos_in_k += n;
	      pos_in_t += n;
	      assert(pos_in_t < KEY(t_it.get_curr()).num_reps);

	      return *this;
	    }

	  n -= remain_in_t_node;
	  pos_in_k += treps;
	  t_it.next();
	  pos_in_t = 0;
	  if (t_it.has_curr())
	    continue;

	  assert(pos_in_k == KEY(k_it.get_curr()).num_reps);

	  while (true) // avanzar por nodos en k_it
	    {
	      k_it.next();
	      if (not has_curr())
		{
		  put_in_overflow();
		  throw std::overflow_error("Multimap::iterator is "
					    "already in overflow");
		}
	  	  
	      assert(KEY(get_curr()).num_reps > 0);

	      const int remain_in_k_node = KEY(get_curr()).num_reps;
	      if (n < remain_in_k_node)
		{
		  t_tree_ptr = &KEY(get_curr()).t_tree;
		  t_it = T_Itor(*t_tree_ptr);
		  pos_in_k = pos_in_t = 0;
		  break;
		}

	      n -= remain_in_k_node;
	    }
	}
    } 
  };

  typedef const iterator const_iterator;

  iterator insert(const value_type & value)
  {
    Knode * kp = kpool.allocate(key_kdata(value.first)); // nuevo nodo
    Knode * kq = k_tree.search_or_insert(kp); 

    if (kp != kq) // ¿Está ya la clave primaria en el multiconjunto?
      kpool.deallocate(kp); // sí está ==> mantenga el nodo para uso futuro

    assert(KEY(kq).key == value.first); // TMP

    T_Tree & t_tree = KEY(kq).t_tree;
    const Tdata tdata(value.second);
    
    try
      {
	Tnode * tp = tpool.allocate(tdata);
	Tnode * tq = t_tree.search_or_insert(tp);

	if (tp != tq)  // ¿Está el elemento ya asociado a la clave?
	  tpool.deallocate(tp);

	++num_elem;
	++KEY(kq).num_reps;

	return iterator(this, kq, tq, KEY(tq).num_reps++);
      }
    catch (...)
      {
	if (kp == kq) 
	  kpool.deallocate(kp);
	
	throw;
      }
  }

      template <class InputIterator>
  void insert(InputIterator first, const InputIterator & last)
  {
    while (first != last)
      insert(*first++);
  }

       template <class InputIterator>
 multimap(InputIterator first, const InputIterator & last) 
   : multimap()
  {
    this->insert(first, last);
  }

  multimap(const multimap & m) 
    : multimap::multimap()
  {
    k_tree.getRoot() = copyRec(const_cast<multimap&>(m).k_tree.getRoot());
    num_elem = m.num_elem;
  }

  multimap & operator = (const multimap & m)
  {
    if (this == &m)
      return *this;

    destroyRec(k_tree.getRoot());
    num_elem = 0;

    k_tree.getRoot() = copyRec(const_cast<multimap&>(m).k_tree.getRoot());
    num_elem = m.num_elem;

    return *this;
  }

  iterator insert(const_iterator & hint, const value_type & value)
  {
    if (hint.has_curr()) // mirar lo que contiene el iterador
      {
	Knode * kp = const_cast<iterator&>(hint).get_curr();
	Kdata & kdata = KEY(kp);

	    // clave primaria de hint  coincide con value.first
	if (Aleph::are_equals <Key, Compare> (kdata.key, value.first))
	  {     // sí ==> no hay necesidad de buscar en árbol k_tree

	    Tnode * tq = hint.t_it.get_curr(); // mirar clave asociada
	    const Tdata & tdata = KEY(tq);
    	        // ¿clave asociada distinta?
	    if (Aleph::no_equals <T, Tless> (tdata.elem, value.second)) 
	      {     // sí == > buscar o insertar en subárbol de clave asociada
		Tnode * tp = tpool.allocate(tdata);
		tq = kdata.t_tree.search_or_insert(tp);
		if (tp != tq)  // ¿Está el elemento ya asociado a la clave?
		  tpool.deallocate(tp); // sí ==> manténgalo para uso futuro
	      }
	    
	    ++num_elem;
	    ++kdata.num_reps;

	    return iterator(this, kp, tq, KEY(tq).num_reps++);
	  }
      }
	
        // hint no tiene nada que ver con value ==> inserción normal
    return insert(value);
  }

  iterator erase(const_iterator & position)
  {
    iterator ret_val = position;
    
    ret_val.del();

    return ret_val;
  }

  size_type erase(const key_type & key)
  {
    Knode * p = k_tree.remove(key_kdata(key));
    if (p == nullptr)
      return 0;

    size_type ret_val = KEY(p).num_reps;

    num_elem -= ret_val;

    delete p; // no hacemos kpool.deallocate(p) porque queremos liberar
	      // enteramente el árbol de claves asociadas KEY(p).t_tree

    return ret_val;
  }

  iterator erase(iterator first, const_iterator & last)
  {
    iterator it = first; 
    while (it != last)
      it = erase(it);

    return it;
  }

  iterator begin() const { return iterator(*this); }
  
  iterator end () const { return iterator(*this).compute_end(); }

  size_type count(const Key & key) const
  {
    Kdata & kdata = const_cast<multimap*>(this)->key_kdata(key);
    Knode * p = const_cast<multimap*>(this)->k_tree.search(kdata);
    if (p == nullptr)
      return 0;

    return KEY(p).num_reps;
  }

  iterator find(const Key & key)
  {
    Knode * p = k_tree.search(key_kdata(key));
    if (p == nullptr)
      return end();

    return iterator(this, p);
  }

  const_iterator find(const Key & key) const
  {
    return const_cast<multimap*>(this)->find(key);
  }

  iterator lower_bound(const Key & key)
  {
    if (k_tree.is_empty())
      return end();

    std::pair<int, Knode*> p = k_tree.find_position(key_kdata(key));
    Knode * kp = p.second;

    iterator ret = iterator(this, kp);
    
    if (Aleph::are_equals <Key, Compare> (KEY(kp).key, key))
      return ret;

    if (p.first == k_tree.size()) // ¿clave mayor que todas las contenidas?
      return end();

    if (p.first == -1)
      return begin();

    if (Compare () (ret->first, key)) // key > *ret
      ret.forward_k_it();

    return ret;
  }

  const_iterator lower_bound(const Key & key) const
  {
    return const_cast<multimap*>(this)->lower_bound(key);
  }

  iterator upper_bound(const Key & key)
  {
    if (k_tree.is_empty())
      return end();

    std::pair<int, Knode*> p = k_tree.find_position(key_kdata(key));
    Knode * kp = p.second;

    iterator ret = iterator(this, kp);
    
    if (Aleph::are_equals <Key, Compare> (KEY(kp).key, key))
	ret.forward_k_it();

    if (p.first == k_tree.size()) // ¿clave mayor que todas las contenidas?
      return end();

    if (p.first == -1)
      return begin();

    if (Compare () (ret->first, key)) // key > *ret
      ret.forward_k_it();

    return ret;
  }

  const_iterator upper_bound(const Key & key) const
  {
    return const_cast<multimap*>(this)->upper_bound(key);
  }

  std::pair<iterator,iterator> equal_range(const Key & key)
  {
    Knode * p = k_tree.search(key_kdata(key));
    if (p == nullptr)
      {
	iterator e = end();
	return std::pair<iterator,iterator>(e, e);
      }

    iterator first(this, p);
    iterator last(this, p);
    last += KEY(p).num_reps;
    
    return std::pair<iterator,iterator>(first, last);
  }

  std::pair<const iterator,const iterator> equal_range(const Key & key) const 
  {
    return const_cast<multimap*>(this)->equal_range(key);
  }

  void swap(multimap & other)
  {
    k_tree.swap(other.k_tree);
    std::swap(num_elem, other.num_elem);
  }  

  bool operator < (const multimap & rhs) const
  {
    K_Itor kit1(const_cast<multimap*>(this)->k_tree);
    K_Itor kit2(const_cast<multimap&>(rhs).k_tree);
    for (; kit1.has_curr() and kit2.has_curr(); kit1.next(), kit2.next())
      {
	Kdata & kdata1 = KEY(kit1.get_curr());
	Kdata & kdata2 = KEY(kit2.get_curr());

	const size_t & n1 = kdata1.num_reps;
	const size_t & n2 = kdata2.num_reps;

	if (n1 != n2)
	  return n1 > n2;

	const Key & key1 = kdata1.key;
	const Key & key2 = kdata2.key;
	if (Compare () (key1, key2))
	  return true;
	else if (Compare () (key2, key1))
	  return false;
      }
    
    if (kit1.has_curr() or kit2.has_curr())
      return kit2.has_curr();
    
    return false;
  }

  bool operator <= (const multimap & rhs) const
  {
    K_Itor kit1(const_cast<multimap*>(this)->k_tree);
    K_Itor kit2(const_cast<multimap&>(rhs).k_tree);
    for (; kit1.has_curr() and kit2.has_curr(); kit1.next(), kit2.next())
      {
	Kdata & kdata1 = KEY(kit1.get_curr());
	Kdata & kdata2 = KEY(kit2.get_curr());

	const size_t & n1 = kdata1.num_reps;
	const size_t & n2 = kdata2.num_reps;

	if (n1 != n2)
	  return n1 > n2;

	const Key & key1 = kdata1.key;
	const Key & key2 = kdata2.key;
	if (Compare () (key1, key2))
	  return true;
	else if (Compare () (key2, key1))
	  return false;
      }
    
    if (kit1.has_curr() or kit2.has_curr())
      return kit2.has_curr(); 
    
    return true;
  }
  
  bool operator == (const multimap & rhs) const
  {
    if (size() != rhs.size())
      return false;

    K_Itor kit1(const_cast<multimap*>(this)->k_tree);
    K_Itor kit2(const_cast<multimap&>(rhs).k_tree);
    for (; kit1.has_curr() and kit2.has_curr(); kit1.next(), kit2.next())
      {
	Kdata & kdata1 = KEY(kit1.get_curr());
	Kdata & kdata2 = KEY(kit2.get_curr());

	if (kdata1.num_reps != kdata2.num_reps)
	  return false;

	const Key & key1     = kdata1.key;
	const Key & key2     = kdata2.key;
	if (Compare () (key1, key2))
	  return false;
	else if (Compare () (key2, key1))
	  return false;
      }
    
    if (kit1.has_curr() or kit2.has_curr())
      return false;

    return true;
  }

  bool operator != (const multimap & rhs) const
  {
    return not (*this == rhs);
  }

  bool operator > (const multimap & rhs) const
  {
    return rhs < *this;
  }

  bool operator >=  (const multimap & rhs) const
  {
    return rhs <= *this;
  }

  const T & operator [] (const Key & key) 
  {
    iterator ret = insert(make_pair(key, T()));

    return ret->second;
  }

  const T & operator [] (const Key & key) const 
    Exception_Prototypes (std::domain_error)
  {
    iterator ret = find(key);
    if (ret == end())
      throw std::domain_error("key not found on constant multimap");

    return ret->second;
  }
};





} // end namespace Aleph


# endif // ALEPH_MULTIMAP_H
