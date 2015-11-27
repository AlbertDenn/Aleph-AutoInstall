
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

# ifndef TPL_BINNODE_H
# define TPL_BINNODE_H
 
# include <iostream>
# include <ahDefs.H>
# include <ahAssert.H>



namespace Aleph {
 
 
struct Empty_Node 
{
  Empty_Node() noexcept { /* empty */ }

  Empty_Node(SentinelCtor) noexcept { /* empty */ }

  void reset() noexcept { /* empty */ }

  Empty_Node & get_data() 
  { 
    throw std::domain_error("Empty_Node has not data"); 
  }
};

# define INIT_CLASS_BINNODE(Name, height, Control_Data)       \
  template <typename Key>						\
  class Name : public Control_Data					\
  {									\
public:									\
 static const size_t MaxHeight = height;				\
 static Name * const NullPtr;						\
 typedef Key key_type;							\
 typedef Key Key_Type;							\
									\
private:								\
									\
 Key    key;								\
 Name * lLink;								\
 Name * rLink;								\
									\
public:									\
									\
 Key & get_key() noexcept { return key; }				\
 const Key & get_key() const noexcept { return key; }			\
 Name *& getL() noexcept { return lLink; }				\
 Name *& getR() noexcept { return rLink; }				\
 const Name * getL() const noexcept { return lLink; }			\
 const Name * getR() const noexcept { return rLink; }			\
 Name(const Key& k) noexcept(noexcept(Key(key)))			\
 : key(k), lLink(NullPtr), rLink(NullPtr)				\
 {									\
   static_assert(std::is_copy_constructible<Key>::value,		\
		 "No copy constructor for Key");			\
 }									\
 Name(Key && k) noexcept(noexcept(std::swap(key, k)))			\
 : key(std::move(k)), lLink(NullPtr), rLink(NullPtr)			\
 {									\
   static_assert(std::is_move_constructible<Key>::value,		\
		 "No move constructor for Key");			\
}									\
 Name(const Control_Data & control_data, const Key & k)			\
 noexcept(noexcept(Key(k)))						\
 : Control_Data(control_data),						\
   key(k), lLink(NullPtr), rLink(NullPtr)				\
 {									\
   /* Empty */								\
 }									\
 Name(const Name & node) noexcept(noexcept(Key(node.key)))		\
 : Control_Data(node),							\
   key(node.key), lLink(NullPtr), rLink(NullPtr)			\
 {									\
   /* Empty */								\
 }									\
 Name(Name && node)							\
 noexcept(noexcept(std::forward<Key>(node).key))			\
 : Control_Data(std::forward<Control_Data>(node)),			\
   key(std::forward<Key>(node).key), lLink(NullPtr), rLink(NullPtr)	\
 {									\
   /* Empty */								\
 }									\
 Name(const Control_Data & control_data) noexcept :			\
 Control_Data(control_data),						\
   lLink(NullPtr), rLink(NullPtr)					\
 {									\
   /* Empty */								\
 }									\
 Name() noexcept(std::is_nothrow_constructible<Key>::value)		\
 : lLink(NullPtr), rLink(NullPtr)					\
 {									\
   static_assert(std::is_default_constructible<Key>::value,		\
		 "No default constructor for Key");			\
 }									\
 void reset() noexcept							\
 {									\
   Control_Data::reset();						\
   rLink = lLink = NullPtr;						\
 }									\
 static Name * key_to_node(Key & __key) noexcept			\
 {									\
   Name * node_zero = 0;						\
   size_t offset = (size_t) &(node_zero->key);				\
   unsigned long addr = (unsigned long)(&__key);			\
   return (Name*) (addr - offset);					\
 }


# define DECLARE_BINNODE(Name, height, Control_Data)		     \
  INIT_CLASS_BINNODE(Name, height, Control_Data)		     \
};								     \
template <typename Key> Name<Key> * const Name<Key>::NullPtr = nullptr; \
 INIT_CLASS_BINNODE(Name##Vtl, height, Control_Data)		     \
 virtual ~Name##Vtl() { /* empty */ }				     \
};								     \
 template <typename Key> Name##Vtl<Key> *			     \
 const Name##Vtl<Key>::NullPtr = nullptr


# define DECLARE_BINNODE_SENTINEL(Name, height, Control_Data)    \
INIT_CLASS_BINNODE(Name, height, Control_Data)                   \
  Name(SentinelCtor) :                                           \
    Control_Data(sentinelCtor), lLink(NullPtr), rLink(NullPtr) {}\
  static Name sentinel_node;                                     \
};                                                               \
  template <typename Key>					 \
Name<Key> Name<Key>::sentinel_node(sentinelCtor);                \
template <typename Key>                                          \
Name<Key> * const Name<Key>::NullPtr = &Name<Key>::sentinel_node;\
INIT_CLASS_BINNODE(Name##Vtl, height, Control_Data)              \
virtual ~Name##Vtl() { /* empty */ }                             \
private:                                                         \
  Name##Vtl(SentinelCtor) :                                      \
    Control_Data(sentinelCtor), lLink(NullPtr), rLink(NullPtr) {}\
  static Name##Vtl sentinel_node;                                \
};                                                               \
template <typename Key>                                          \
Name##Vtl<Key> Name##Vtl<Key>::sentinel_node(sentinelCtor);      \
template <typename Key>                                          \
Name##Vtl<Key> * const Name##Vtl<Key>::NullPtr =                 \
  &Name##Vtl<Key>::sentinel_node

template <class Node> inline Node *& LLINK(Node * p) noexcept 
{
  return p->getL(); 
}

template <class Node> inline Node *& RLINK(Node * p) noexcept
{
  return p->getR(); 
}


  template <class Node> inline
auto & KEY(Node * p) noexcept { return p->get_key(); }

DECLARE_BINNODE(BinNode, 2048, Empty_Node);


} // end namespace Aleph
# endif
