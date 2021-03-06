
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/

# ifndef TPL_SINGLELIST_H
# define TPL_SINGLELIST_H

# include <ahDefs.H>

    template <class T>
class Single_List 
{
private:

  struct Node
  {
    T      data;
    Node * next; 

    Node * get_by_pos(int pos)
    {
      int i;
      Node * node = this;
      for (i = 0; i < pos; ++i)
        {
          if (node->next == nullptr)
            throw std::overflow_error("overflow in access by position");
          node = node->next;
        }
      
      return node;
    }
    Node() : next (nullptr) { /* empty */ }

    Node(const T& _data) : data(_data), next (nullptr) { /* empty */ }
  };
  int    num_items;
  Node   head_node;
  Node * head;

public:

  unsigned int get_num_items() const { return num_items; }
  T& operator [] (unsigned int pos) Exception_Prototypes(std::overflow_error) 
  { 
    if (pos > num_items) 
      throw std::overflow_error("List overflow");
    return head->get_by_pos(pos + 1)->data;
  }
  void insert(unsigned int pos, const T& t) 
    Exception_Prototypes(std::bad_alloc, std::overflow_error)  
  {
    if (pos > num_items) 
      throw std::overflow_error("List overflow");
    Node * node = new Node (t);
    Node * prev = head->get_by_pos(pos - 1); 
    node->next = prev->next;
    prev->next = node;
    ++num_items; 
  }
  void remove(unsigned int pos) Exception_Prototypes(std::overflow_error)
  { 
    if (pos > num_items) 
      throw std::overflow_error("List overflow");

    Node * prev = head->get_by_pos(pos - 1);
    Node * node_to_delete = prev->next;
    prev->next = node_to_delete->next;
    num_items--;
    delete node_to_delete;
  }
};

# endif
