
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

# ifndef ALEPH_GRAPH_H
# define ALEPH_GRAPH_H

# include <string.h>

# include <cassert>
# include <stdexcept>
# include <memory>
# include <string>

namespace Aleph {


static const long No_Visited = 0; // nodo o arco no ha sido visitado

enum Graph_Bits 
{
  Depth_First,   
  Breadth_First, 
  Test_Cycle,    
  Find_Path,     
  Euler,         
  Maximum_Flow,  
  Spanning_Tree, 
  Build_Subtree, 
  Convert_Tree,  
  Cut,           
  Min,           

  Num_Bits_Graph 
};


  extern const unsigned char Processed; 

  extern const unsigned char Processing;

  extern const unsigned char Unprocessed;


class Bit_Fields
{
public:

  unsigned int depth_first    : 1; 
  unsigned int breadth_first  : 1; 
  unsigned int test_cycle     : 1; 
  unsigned int find_path      : 1; 
  unsigned int euler          : 1; 
  unsigned int maximum_flow   : 1; 
  unsigned int spanning_tree  : 1; 
  unsigned int build_subtree  : 1; 
  unsigned int convert_tree   : 1; 
  unsigned int cut            : 1; 
  unsigned int min            : 1; 
  unsigned int state          : 2; 

  Bit_Fields() noexcept
  {
    memset(this, 0, sizeof *this);
  }

  bool get_bit(int bit) const noexcept
  {
    switch (bit)
      {
      case Aleph::Depth_First:   return depth_first;
      case Aleph::Breadth_First: return breadth_first;
      case Aleph::Test_Cycle:    return test_cycle;
      case Aleph::Find_Path:     return find_path;
      case Aleph::Euler:         return euler;
      case Aleph::Maximum_Flow:  return maximum_flow;
      case Aleph::Spanning_Tree: return spanning_tree;
      case Aleph::Build_Subtree: return build_subtree;
      case Aleph::Convert_Tree:  return convert_tree;
      case Aleph::Cut:           return cut;
      case Aleph::Min:           return min;
      default: assert(false);
      }
    return false;
  }

  void set_bit(int bit, int value) noexcept
  {
    assert(value == 0 or value == 1);
    switch (bit)
      {
      case Aleph::Depth_First:   depth_first   = value; break;
      case Aleph::Breadth_First: breadth_first = value; break;
      case Aleph::Test_Cycle:    test_cycle    = value; break;
      case Aleph::Find_Path:     find_path     = value; break;
      case Aleph::Euler:         euler         = value; break;
      case Aleph::Maximum_Flow:  maximum_flow  = value; break;
      case Aleph::Spanning_Tree: spanning_tree = value; break;
      case Aleph::Build_Subtree: build_subtree = value; break;
      case Aleph::Convert_Tree:  convert_tree  = value; break;
      case Aleph::Cut:           cut           = value; break;
      case Aleph::Min:           min           = value; break;
      default: assert(false);
      }
  }

  unsigned int get_state() const noexcept { return state; }

  std::string str_state() const 
  {
    switch (state)
      {
      case 0: return std::string("Unprocessed");
      case 1: return std::string("Processing");
      case 2: return std::string("Processed");
      default: break;
      }
    return std::string("Undefined");
  }

  void set_state(unsigned char s) noexcept
  {
    assert(s < 4);
    state = s;
  }

  void reset(int bit) noexcept { set_bit(bit, 0); }

  void reset() noexcept
  {
    memset(this, 0, sizeof *this);
  }
};


struct Graph_Attr
{
  Bit_Fields control_bits; 
  long       counter = 0;
  void *     cookie = nullptr; 
  
  void reset()
  {
    control_bits.reset();
    counter = 0;
    cookie = nullptr;
  }  
};


# define NODE_BITS(p)            ((p)->attrs.control_bits)

# define NODE_COUNTER(p)         ((p)->attrs.counter)

# define NODE_COLOR(p)         ((p)->attrs.counter)

# define IS_NODE_VISITED(p, bit) (NODE_BITS(p).get_bit(bit))

# define NODE_COOKIE(p)          ((p)->attrs.cookie) 

# define ARC_COUNTER(p)          ((p)->attrs.counter)

# define ARC_COLOR(p)          ((p)->attrs.counter)

# define ARC_BITS(p)             ((p)->attrs.control_bits)

# define IS_ARC_VISITED(p, bit)  (ARC_BITS(p).get_bit(bit))

# define ARC_COOKIE(p)           ((p)->attrs.cookie) 



} // end namespace Aleph





# endif
