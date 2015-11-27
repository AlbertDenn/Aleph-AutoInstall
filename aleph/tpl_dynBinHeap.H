
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

# ifndef TPL_DYNBINHEAP_H
# define TPL_DYNBINHEAP_H

# include <ahDry.H>
# include <htlist.H>
# include <ah-dry.H>
# include <tpl_binNodeUtils.H>
# include <tpl_binHeap.H>

using namespace Aleph;

namespace Aleph {

    template <class T, class Compare = Aleph::less<T>>
class DynBinHeap 
      : public BinHeap<T, Compare>,
	public LocateFunctions<DynBinHeap<T, Compare>, T>,
	public FunctionalMethods<DynBinHeap<T, Compare>, T>,
	public GenericKeys<DynBinHeap<T, Compare>, T>,
	public EqualToMethod<DynBinHeap<T, Compare>>,
	public StlAlephIterator<DynBinHeap<T, Compare>>
{
public:

  using Set_Type = DynBinHeap;

  using Item_Type = T;

  using Key_Type = T;

private:

  using Base = BinHeap<T, Compare>;

  using Node = typename BinHeap<T, Compare>::Node;

  T & __insert(Node * p) noexcept
  {
    return BinHeap<T, Compare>::insert(p)->get_key();
  }

  void copy(const DynBinHeap & src)
  {
    src.for_each_in_preorder([this] (Node * p)
	{
	  __insert(new Node (p->get_key()));
	});
  }

public:

  DynBinHeap(Compare & cmp) noexcept : Base(cmp) { /* empty */ }

  DynBinHeap(Compare && cmp = Compare()) noexcept : BinHeap<T, Compare>(cmp) 
  { /* empty */ }

  DynBinHeap(const DynBinHeap & h) : Base(h.cmp)
  {
    copy(h);
  }

  DynBinHeap(DynBinHeap && h) : Base(h.cmp)
  {
    this->swap(h);
  }

  Special_Ctors(DynBinHeap, T);

  DynBinHeap & operator = (const DynBinHeap & h)
  {
    if (this == &h)
      return *this;

    empty();

    copy(h);

    return *this;
  }

  DynBinHeap & operator = (DynBinHeap && h) noexcept
  {
    this->swap(h);
    return *this;
  }

  T & insert(const T & item) throw(std::exception, std::bad_alloc)
  {
    return __insert(new Node (item));
  }

  T & insert(T && item) throw(std::exception, std::bad_alloc)
  {
    return __insert(new Node (std::forward<T>(item)));
  }

  T & append(const T & item) throw(std::exception, std::bad_alloc)
  {
    return __insert(new Node (item));
  }

  T & append(T && item) throw(std::exception, std::bad_alloc)
  {
    return __insert(new Node (std::forward<T>(item)));
  }
  
  T & put(const T & item) throw(std::exception, std::bad_alloc)
  {
    return insert(item);
  }

  T & put(T && item) throw(std::exception, std::bad_alloc)
  {
    return insert(std::forward<T>(item));
  }

  T getMin() throw(std::exception, std::underflow_error)
  {
    Node * node = BinHeap<T, Compare>::getMin();

    T return_value = std::move(node->get_key());

    delete node;

    return return_value;
  }

  T getMax() throw(std::exception, std::underflow_error)
  {
    return getMin();
  }

  T get() throw(std::exception, std::underflow_error)
  {
    return getMin();
  }

  void update(T & data) noexcept
  {
    Node * node = Node::key_to_node(data);
    BinHeap<T, Compare>::update(node);
  }

  void remove(T & data) noexcept
  {
    Node * node = BinHeap<T, Compare>::Node::key_to_node(data);
    BinHeap<T, Compare>::remove(node);
  }
  
  void erase(T & data) noexcept
  {
    remove(data);
  }

  T & top() const throw(std::exception, std::underflow_error)
  {
    return BinHeap<T, Compare>::top()->get_key();
  }

  void empty() noexcept
  {
    this->remove_all_and_delete();
  }

  ~DynBinHeap()
  {
    empty();
  }

  template <class Operation>
  bool traverse(Operation & op) noexcept(noexcept(op))
  {
    return this->level_traverse([&op] (Node * p) { return op(p->get_key()); });
  }

  template <class Operation>
  bool traverse(Operation && op = Operation()) noexcept(noexcept(op))
  {
    return traverse(op);
  }

  template <class Operation>
  bool traverse(Operation & op) const noexcept(noexcept(op))
  {
    return this->level_traverse([&op] (Node * p) { return op(p->get_key()); }); 
  }

  template <class Operation>
  bool traverse(Operation && op = Operation()) const noexcept(noexcept(op))
  {
    return traverse<Operation>(op);
  }

  struct Iterator : Base::Iterator
  {
    const T & get_curr() const { return KEY(Base::get_curr()); }
  };
};

} // end namespace Aleph

# endif // TPL_DYNBINHEAP_H



