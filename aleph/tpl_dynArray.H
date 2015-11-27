
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


# ifndef TPL_DYNARRAY_H
# define TPL_DYNARRAY_H

# include <math.h>

# include <string>
# include <aleph.H>
# include <ahIterator.H>
# include <ahDry.H>
# include <tpl_dynDlist.H>
# include <htlist.H>
# include <ah-dry.H>

using namespace std;

using namespace Aleph;

namespace Aleph {
 
    template <typename T> 
class DynArray :
    public LocateFunctions<DynArray<T>, T>,
    public FunctionalMethods<DynArray<T>, T>,
    public GenericKeys<DynArray<T>, T>,
    public EqualToMethod<DynArray<T>>,
    public StlAlephIterator<DynArray<T>>
{
  // look at the end of this file for seeing the values
public:

  using Item_Type = T; 
  using Key_Type = T; 

  static const size_t Default_Pow_Dir; 
  static const size_t Default_Pow_Seg; 
  static const size_t Default_Pow_Block;

 private:

  static const size_t Max_Bits_Allowed; 

 public:

  static const unsigned long long Max_Dim_Allowed;

private:

  static const size_t Max_Pow_Block;

  mutable size_t pow_dir              = Default_Pow_Dir;
  mutable size_t pow_seg              = Default_Pow_Seg;
  mutable size_t pow_block            = 
    std::min(Default_Pow_Block, Max_Pow_Block);
  mutable size_t seg_plus_block_pow   = pow_seg + pow_block;
  mutable size_t mask_seg_plus_block = two_raised(seg_plus_block_pow) - 1;
  mutable size_t dir_size            = two_raised(pow_dir); // = 2^pow_dir
  mutable size_t seg_size            = two_raised(pow_seg); // = 2^pow_seg
  mutable size_t block_size          = two_raised(pow_block); // = 2^pow_block

      // 2^(pow_dir + pow_seg + pow_block) - 1
  unsigned long long max_dim         = two_raised(seg_plus_block_pow + pow_dir);

  static size_t two_raised(const size_t n) 
  {
    if (n >= Max_Bits_Allowed)
      throw std::overflow_error ("number of bits exceeds maximum allowed");

    return 1 << n;
  }

  static size_t compute_dim(size_t d, size_t s, size_t b) noexcept
  {
    return two_raised(d)*two_raised(s)*two_raised(b);
  }

public:

  static void compute_sizes(const size_t n, size_t & d, size_t & s, size_t & b)
    noexcept
  {
    d = Default_Pow_Dir;
    s = Default_Pow_Seg;
    b = Default_Pow_Block;
    if (compute_dim(d, s, b) >= n)
      return;

    while (true)
      {
	if (compute_dim(++d, s, b) >= n)
	  break;

	if (compute_dim(d, ++s, b) >= n)
	  break;

	if (compute_dim(d, s, ++b) >= n)
	  break;
      }
  }

  static std::tuple<size_t, size_t, size_t> compute_sizes(const size_t n)
    noexcept
  {
    size_t d, s, b;
    compute_sizes(n, d, s, b);
    return make_tuple(d, s, b);
  }

private:

  size_t mask_seg   = seg_size - 1;
  size_t mask_block = block_size - 1;

  size_t index_in_dir(const size_t i) const noexcept
  {
    assert( (pow_block + pow_seg) == seg_plus_block_pow );
    assert( seg_size * block_size == two_raised(seg_plus_block_pow) );
    assert( i >> seg_plus_block_pow == i/(seg_size*block_size) );

    return i >> seg_plus_block_pow;
  }

  size_t modulus_from_index_in_dir(const size_t i) const noexcept
  {
    assert( mask_seg_plus_block == seg_size*block_size - 1 );
    assert( (i & mask_seg_plus_block) == i%(seg_size*block_size) );

    return (i & mask_seg_plus_block);
  }

  size_t index_in_seg(const size_t & i) const noexcept
  {
    assert( two_raised(pow_block) == block_size );
    assert( (modulus_from_index_in_dir(i) >> pow_block) == 
            (i%(seg_size*block_size))/block_size );

    return modulus_from_index_in_dir(i) >> pow_block;
  }

  size_t index_in_block(const size_t i) const noexcept
  {
    assert( mask_block == block_size - 1 );
    assert( (modulus_from_index_in_dir(i) & mask_block) == 
            ((i%(seg_size*block_size))%block_size) );

    return modulus_from_index_in_dir(i) & mask_block;
  }

  size_t current_dim;  
  size_t num_segs   = 0;   
  size_t num_blocks = 0;
  T *** dir         = nullptr;

  void fill_dir_to_null() noexcept
  {
    assert(dir != nullptr);

    for (size_t i = 0; i < dir_size; ++i)
      dir[i] = nullptr;
  }

  void fill_seg_to_null(T ** seg) noexcept
  {
    assert(seg != nullptr);

    for (size_t i = 0; i < seg_size; ++i)
      seg[i] = nullptr;
  }

  void allocate_dir()
  {
    dir = (T***) malloc(dir_size*sizeof(T**)); 
    if (dir == nullptr)
      throw std::bad_alloc();
    
    fill_dir_to_null();
  }

  void resize_dir(const size_t i) // resize dir to fit index i
  {
    assert(i >= max_dim);

    size_t new_pow_dir = pow_dir + 1;
    while (compute_dim(new_pow_dir, pow_seg, pow_block) <= i)
      ++new_pow_dir;
    
    const size_t new_dir_sz = two_raised(new_pow_dir);
    T *** new_dir = (T***) realloc(dir, new_dir_sz*sizeof(T**));
    if (new_dir == nullptr)
      throw std::bad_alloc();

    dir = new_dir;
    for (size_t k = dir_size; k < new_dir_sz; ++k)
      dir[k] = nullptr;

    pow_dir = new_pow_dir;
    dir_size = new_dir_sz;

    max_dim = compute_dim(pow_dir, pow_seg, pow_block);
  }

  void allocate_segment(T **& seg) 
  {
    assert(seg == nullptr);

    seg = new T* [seg_size];
    fill_seg_to_null(seg);
    ++num_segs;
  }

  T   default_initial_value = T();
  T * default_initial_value_ptr = &default_initial_value;

  void allocate_block(T *& block)
  {
    assert(block == nullptr);

    block = new T [block_size];
    ++num_blocks;

    if (default_initial_value_ptr == nullptr) 
      return;

    for (size_t i = 0; i < block_size; ++i)
      block[i] = *default_initial_value_ptr;
  }

  void release_segment(T **& seg) noexcept
  {
    assert(seg != nullptr);

    delete [] seg;
    seg = nullptr;
    --num_segs;
  }

  void release_block(T *& block) noexcept
  {
    assert(block != nullptr);

    delete [] block;
    block = nullptr;
    --num_blocks;
  }

  void release_blocks_and_segment(T ** & seg) noexcept
  {
    assert(seg != nullptr);

    for(size_t i = 0; i < seg_size ; ++i)
      if (seg[i] != nullptr) 
        release_block(seg[i]);

    release_segment(seg);
  }

  void release_all_segments_and_blocks() noexcept
  {
    assert(dir != nullptr);

    for(size_t i = 0; i < dir_size ; ++i)
      if (dir[i] != nullptr)
        release_blocks_and_segment(dir[i]); 

    current_dim = 0;
  }

  void release_dir() noexcept
  {
    if (dir == nullptr)
      return;

    release_all_segments_and_blocks();
    if (dir != nullptr)
      free(dir);

    dir = nullptr;
    current_dim = 0;
  }

  static size_t next2Pow(const size_t number)  noexcept
  {
    return (size_t) ceil( log((float) number)/ log(2.0) );
  }

  size_t divide_by_block_size(const size_t number) const noexcept
  {
    assert(number/block_size == number >> pow_block);

    return number >> pow_block;
  }

  size_t modulus_by_block_size(const size_t number) const noexcept
  {
    assert((number%block_size) == (number & mask_block));

    return number & mask_block;
  }

  void advance_block_index(size_t block_index, size_t seg_index, 
                           const size_t len) const noexcept
  {
    if (block_index + len < block_size)
      {
        block_index += len;
        return;
      }

    seg_index += divide_by_block_size(len);
    block_index = modulus_by_block_size(len); 
  }

  void allocate_block(T *& block, T * src_block)
  {
    allocate_block(block);
    for (int i = 0; i < block_size; i++)
      block[i] = src_block[i];
  }

  void allocate_segment(T **& seg, T ** src_seg)
  {
    allocate_segment(seg);
    for (int i = 0; i < seg_size; i++)
      if (src_seg[i] != nullptr)
        allocate_block(seg[i], src_seg[i]);
  }

  void allocate_dir(T *** src_dir)
  {
    allocate_dir();
    for (int i = 0; i < dir_size; i++)
      if (src_dir[i] != nullptr)
        allocate_segment(dir[i], src_dir[i]);
  }

  class Proxy
  {
    size_t     index;
    size_t     pos_in_dir;
    size_t     pos_in_seg;
    size_t     pos_in_block;
    T **&      ref_seg;
    T *        block;
    DynArray & array;

  public:

    Proxy(DynArray<T> & _array, const size_t i) noexcept : 
      index(i), pos_in_dir(_array.index_in_dir(index)),
      pos_in_seg(_array.index_in_seg(index)), 
      pos_in_block(_array.index_in_block(index)),
      ref_seg(_array.dir[pos_in_dir]), block (nullptr), array(_array)
    {
      if (ref_seg != nullptr)
        block = ref_seg[pos_in_seg]; // ya existe bloque para entrada i
    }

    operator T & () 
    {
      if (block == nullptr)
        throw std::invalid_argument("accessed entry not been still written");
      return block[pos_in_block];
    }

    T * operator -> ()
    {
      bool seg_was_allocated_in_current_call = false;

      if (ref_seg == nullptr) // hay segmento?
	{     // No ==> apartarlo!
	  array.allocate_segment(ref_seg);
	  seg_was_allocated_in_current_call = true; 
	}

      if (block == nullptr) // test if block is allocated
        {
	  try // tratar apartar bloque
	    {
	      array.allocate_block(block);
	      ref_seg[pos_in_seg] = block;

	      assert(array.dir[pos_in_dir] == ref_seg); 
	    }
	  catch (...) // Ocurre una falla en el apartado del bloque
	    {
	      if (seg_was_allocated_in_current_call)
		array.release_segment(ref_seg);

	      throw;
	    }
        }

      if (index >= array.current_dim)
        array.current_dim = index + 1; 

      return &block[pos_in_block]; 
    }

    Proxy & operator = (const T & data)
    {
      bool seg_was_allocated_in_current_call = false;
      if (ref_seg == nullptr) // hay segmento?
	{     // No ==> apartarlo!
	  array.allocate_segment(ref_seg);
	  seg_was_allocated_in_current_call = true; 
	}

      if (block == nullptr) // test if block is allocated
        {
	  try // tratar apartar bloque
	    {
	      array.allocate_block(block);
	      ref_seg[pos_in_seg] = block;

	      assert(array.dir[pos_in_dir] == ref_seg); 
	    }
	  catch (...) // Ocurre una falla en el apartado del bloque
	    {
	      if (seg_was_allocated_in_current_call)
		array.release_segment(ref_seg);

	      throw;
	    }
        }

      if (index >= array.current_dim)
        array.current_dim = index + 1; 

      block[pos_in_block] = data;

      return *this;
    }

    Proxy & operator = (const Proxy & proxy)
    {
      if (proxy.block == nullptr) // ¿operando derecho puede leerse?
        throw std::domain_error("right entry has not been still written");

      if (&proxy == this) 
	return *this;

      bool seg_was_allocated_in_current_call = false;
      if (ref_seg == nullptr) // hay segmento?
	{     // No ==> apartarlo!
	  array.allocate_segment(ref_seg);
	  seg_was_allocated_in_current_call = true; 
	}

      if (block == nullptr) // test if block is allocated
        {
	  try // tratar apartar bloque
	    {
	      array.allocate_block(block);
	      ref_seg[pos_in_seg] = block;

	      assert(array.dir[pos_in_dir] == ref_seg); 
	    }
	  catch (...) // Ocurre una falla en el apartado del bloque
	    {
	      if (seg_was_allocated_in_current_call)
		array.release_segment(ref_seg);

	      throw;
	    }
        }

      if (index >= array.current_dim)
        array.current_dim = index + 1; 

      block[pos_in_block] = proxy.block[proxy.pos_in_block];

      return *this;
    }
  };

public:

  size_t get_dir_size() const noexcept { return dir_size; }

  size_t get_seg_size() const noexcept { return seg_size; }

  size_t get_block_size() const noexcept { return block_size; } 

  size_t size() const noexcept { return current_dim; }

  size_t max_size() const noexcept { return max_dim; }

  size_t get_num_blocks() const noexcept { return num_blocks; }

  void set_default_initial_value(const T & value) noexcept
  {
    default_initial_value     = value;
    default_initial_value_ptr = &default_initial_value;
  }

  void set_default_initial_value(T && value = T()) 
    noexcept(noexcept(std::swap(default_initial_value, value)))
  {
    std::swap(default_initial_value, value);
    default_initial_value_ptr = &default_initial_value;
  }

  DynArray(size_t _pow_dir, size_t _pow_seg, size_t _pow_block) 
    throw (std::exception, std::bad_alloc, 
           std::length_error, std::overflow_error) 
    : pow_dir             ( _pow_dir                                 ),
      pow_seg             ( _pow_seg                                 ),
      pow_block           ( _pow_block                               ),
      seg_plus_block_pow  ( _pow_seg + _pow_block                    ),
      mask_seg_plus_block ( two_raised(seg_plus_block_pow) - 1       ),
      dir_size            ( two_raised(pow_dir)                      ), 
      seg_size            ( two_raised(pow_seg)                      ), 
      block_size          ( two_raised(pow_block)                    ), 
      max_dim             ( two_raised(seg_plus_block_pow + pow_dir) ), 
      mask_seg            ( seg_size - 1                             ),
      mask_block          ( block_size - 1                           ),
      current_dim         ( 0                                        ),
      num_segs            ( 0                                        ),
      num_blocks          ( 0                                        )
  {
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
    static_assert(std::is_move_constructible<T>::value,
		  "No move constructor for T");
    static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    static_assert(std::is_move_assignable<T>::value,
		  "No move assign for T");
    assert(Max_Dim_Allowed > 0);

    if (max_dim > Max_Dim_Allowed)
      throw std::length_error ("Dimension too large"); 

    allocate_dir();
  }

  DynArray(const size_t dim = 0) 
    throw (std::exception, std::bad_alloc, 
	   std::length_error, std::overflow_error) 
    : current_dim(dim)
  {
    static_assert(std::is_default_constructible<T>::value, 
		  "No default constructor for T");
    static_assert(std::is_copy_constructible<T>::value, 
		  "No copy constructor for T");
    static_assert(std::is_move_constructible<T>::value,
		  "No move constructor for T");
    static_assert(std::is_copy_assignable<T>::value,
		  "No copy assign for T");
    static_assert(std::is_move_assignable<T>::value,
		  "No move assign for T");
    assert(Max_Dim_Allowed > 0);

    if (max_dim > Max_Dim_Allowed)
      throw std::length_error ("Dimension too large"); 

    allocate_dir();
  }

  Special_Ctors(DynArray, T);

  ~DynArray() 
  { 
    release_dir();
  }

  void copy_array(const DynArray<T> & src_array)
  {
    for (int i = 0; i < src_array.current_dim; ++i)
      if (src_array.exist(i))
        (*this)[i] = src_array.access(i);
  }

  DynArray(const DynArray<T> & array) 
      throw (std::exception, std::bad_alloc)
    : pow_dir                   (array.pow_dir),
      pow_seg                   (array.pow_seg),
      pow_block                 (array.pow_block),
      seg_plus_block_pow        (array.seg_plus_block_pow),
      mask_seg_plus_block       (array.mask_seg_plus_block),
      dir_size                  (array.dir_size),
      seg_size                  (array.seg_size),
      block_size                (array.block_size),
      max_dim                   (array.max_dim),
      mask_seg                  (array.mask_seg),
      mask_block                (array.mask_block),
      current_dim               (0),
      num_segs                  (0),
      num_blocks                (0),
      dir                       (nullptr),
      default_initial_value_ptr (array.default_initial_value_ptr)
  {
    allocate_dir(array.dir);
    copy_array(array);
  }

  DynArray<T> & operator = (const DynArray<T> & array) 
    throw (std::exception, std::bad_alloc)
  {
    if (this == &array) 
      return *this;

    copy_array(array);
    
    if (array.current_dim < current_dim)
      cut(array.current_dim);

    current_dim = array.current_dim;

    return *this;
  }

  void swap(DynArray<T> & array) noexcept
  {
    std::swap(dir, array.dir);
    std::swap(pow_dir,  array.pow_dir);
    std::swap(pow_seg, array.pow_seg);
    std::swap(pow_block, array.pow_block);
    std::swap(seg_plus_block_pow, array.seg_plus_block_pow);
    std::swap(mask_seg_plus_block, array.mask_seg_plus_block);
    std::swap(dir_size, array.dir_size);
    std::swap(seg_size, array.seg_size);
    std::swap(block_size, array.block_size);
    std::swap(mask_seg, array.mask_seg);
    std::swap(mask_block, array.mask_block);
    std::swap(max_dim, array.max_dim);
    std::swap(current_dim, array.current_dim);
    std::swap(num_segs, array.num_segs);
    std::swap(num_blocks, array.num_blocks);
  }

  DynArray(DynArray && other) noexcept
    : pow_dir             ( Default_Pow_Dir                          ),
      pow_seg             ( Default_Pow_Seg                          ),
      pow_block           (std::min(Default_Pow_Block, Max_Pow_Block)), 
      seg_plus_block_pow  ( pow_seg + pow_block                      ),
      mask_seg_plus_block ( two_raised(seg_plus_block_pow) - 1       ),
      dir_size            ( two_raised(pow_dir)                      ), 
      seg_size            ( two_raised(pow_seg)                      ), 
      block_size          ( two_raised(pow_block)                    ), 
      max_dim             ( two_raised(seg_plus_block_pow + pow_dir) ),
      mask_seg            ( seg_size - 1                             ),
      mask_block          ( block_size - 1                           ),
      current_dim         ( 0                                        ),
      num_segs            ( 0                                        ),
      num_blocks          ( 0                                        ),
      dir                 (nullptr                                      )
  {
    swap(other);
  }

  DynArray & operator = (DynArray && other) noexcept
  {
    swap(other);
    return *this;
  }

  T & access(const size_t i) const noexcept
  {
    assert(dir[index_in_dir(i)] != nullptr);
    assert(dir[index_in_dir(i)][index_in_seg(i)] != nullptr);

    return dir[index_in_dir(i)][index_in_seg(i)][index_in_block(i)];
  }

  T & operator () (const size_t i) const noexcept
  {
    return access(i);
  }
    
  bool exist(const size_t i) const 
  {
    if (i >= max_dim)
      return false;

    const size_t pos_in_dir = index_in_dir(i);

    assert(pos_in_dir < dir_size);

    if (dir[pos_in_dir] == nullptr) 
      return false;

    const size_t pos_in_seg = index_in_seg(i);

    assert(pos_in_seg < seg_size);

    if (dir[pos_in_dir][pos_in_seg] == nullptr) 
      return false;

    return true;
  }  

  T * test(const size_t i) const noexcept
  {
    if (i >= max_dim)
      return nullptr;

    const size_t pos_in_dir = index_in_dir(i);
    if (dir[pos_in_dir] == nullptr) 
      return nullptr;

    const size_t pos_in_seg = index_in_seg(i);
    if (dir[pos_in_dir][pos_in_seg] == nullptr) 
      return nullptr;

    return &dir[index_in_dir(i)][index_in_seg(i)][index_in_block(i)];
  }

  T & touch(const size_t i) throw (std::exception, std::bad_alloc)
  {
    if (i >= max_dim)
      resize_dir(i);

    const size_t pos_in_dir = index_in_dir(i);
    if (dir[pos_in_dir] == nullptr)
      allocate_segment(dir[pos_in_dir]);

    const size_t pos_in_seg = index_in_seg(i);
    if (dir[pos_in_dir][pos_in_seg] == nullptr)
      {
        try
          {
	    allocate_block(dir[pos_in_dir][pos_in_seg]);
          }
        catch (...)
          {
            release_segment(dir[pos_in_dir]);
            throw;
          }
      }

    if (i >= current_dim)
      current_dim = i + 1;

    return dir[pos_in_dir][pos_in_seg][index_in_block(i)];
  }

  void reserve(const size_t l, const size_t r) 
    throw (std::exception, std::bad_alloc, std::domain_error)
  {
    if (l > r)
      throw std::domain_error("invalid range");

    if (r >= max_dim)
      resize_dir(r);

    const size_t first_seg   = index_in_dir(l);
    const size_t last_seg    = index_in_dir(r);
    const size_t first_block = index_in_seg(l);
    const size_t last_block  = index_in_seg(r);

    try
      {
	for (size_t seg_idx = first_seg; seg_idx <= last_seg; ++seg_idx)
	  {
	    if (dir[seg_idx] == nullptr)
	      allocate_segment(dir[seg_idx]);
	    
	    size_t block_idx = (seg_idx == first_seg) ? first_block : 0;
	    const size_t final_block = 
	      (seg_idx == last_seg) ? last_block : seg_size - 1;

	    while (block_idx <= final_block)
	      {
		if (dir[seg_idx][block_idx] == nullptr)
		  allocate_block(dir[seg_idx][block_idx]);

		++block_idx; 
	      }
	  } // end for (...) 

	if (r + 1 > current_dim)
	  current_dim = r + 1;
      }
    catch (...)
      {
        if (r + 1 > current_dim)
          current_dim = r + 1;

        throw;
      }
  }

  void reserve(const size_t dim) 
  {
    if (dim > 0)
      reserve(0, dim - 1);
  }

  void cut(const size_t new_dim = 0) throw(std::exception, std::domain_error) 
  {
    if (new_dim > current_dim)
      throw std::domain_error("new dimension greater that current dimension");

    if (new_dim == 0) 
      {
        release_all_segments_and_blocks();
        current_dim = 0;
        return;
      }

    const size_t old_dim = current_dim; // old dimension

        // segment and block first indexes
    const long idx_first_seg   = index_in_dir(old_dim - 1);
    const long idx_first_block = index_in_seg(old_dim - 1);

        // segment and block last indexes
    const long idx_last_seg    = index_in_dir(new_dim - 1); 
    const long idx_last_block  = index_in_seg(new_dim - 1);    
    for (long idx_seg = index_in_dir(old_dim - 1); idx_seg >= idx_last_seg; 
         --idx_seg) // recorre descendentemente los segmentos 
      {
        if (dir[idx_seg] == nullptr) // ¿hay un segmento?
          continue; // no ==> avance al siguiente

        long idx_block =  // primer bloque a liberar 
          idx_seg == idx_first_seg ? idx_first_block : seg_size - 1;

            // Libera descendentemente los bloques reservados del segmento
        while ( (idx_seg > idx_last_seg and idx_block >= 0) or
                (idx_seg == idx_last_seg and idx_block > idx_last_block) )
        {
          if (dir[idx_seg][idx_block] != nullptr) // ¿Hay un bloque aquí?
            release_block(dir[idx_seg][idx_block]);

          --idx_block; 
        }

        if (idx_block < 0)
          release_segment(dir[idx_seg]);
      }

    current_dim = new_dim; // actualiza nueva dimensión
  }

  void adjust(const size_t dim)
  {
    if (dim > current_dim)
      reserve(dim);
    else
      cut(dim);
  }

  void empty() noexcept { cut(0); }

  Proxy operator [] (const size_t i) const
    throw (std::exception, std::bad_alloc, std::length_error, 
	   std::invalid_argument) 
  {
    if (i >= max_dim)
      throw std::out_of_range ("index out of maximun range");

    return Proxy (const_cast<DynArray<T>&>(*this), i);
  }

  Proxy operator [] (const size_t i) 
    throw (std::exception, std::length_error, 
           std::invalid_argument, std::bad_alloc)
  {
    if (i >= max_dim)
      resize_dir(i);

    return Proxy (const_cast<DynArray<T>&>(*this), i);
  }

  T & append() 
  {
    return touch(this->size()); 
  }
  
  T & append(const T & data) 
  { 
    T & ref = this->append();
    ref = data;
    return ref;
  }

  T & append(T && data) 
  { 
    T & ref = this->append();
    ref = std::move(data);
    return ref;
  }
  
  void push(const T & data) { this->append(data); }

  T & push(T && data) { return this->append(std::forward<T>(data)); }

  void put(const T & data) { this->append(data); }

  T & put(T && data) { return this->append(std::forward<T>(data)); }

  void remove(T & item) noexcept
  {
    std::swap(item, this->access(this->size() - 1));
    this->cut(this->size() - 1);
  }

  void erase(T & item) noexcept { remove(item); }

  bool is_empty() const noexcept { return this->size() == 0; }

  T pop() 
  {
    T ret_val = std::move(this->access(this->size() - 1));
    cut(size() - 1); 

    return ret_val;
  }

  T & top() const { return (*this)[size() - 1]; }

  T & get_first() const { return (*this)[0]; } 

  T & get_last() const { return (*this)[size() - 1]; }

  class Iterator
  {
  protected:

    DynArray * array_ptr = nullptr;
    long curr_idx = 0;

  public:

    Iterator(const DynArray & array) noexcept
      : array_ptr(const_cast<DynArray*>(&array)), curr_idx(0)
    {
      // empty
    }

    bool has_curr() const noexcept
    {
      return curr_idx >= 0 and curr_idx < array_ptr->size(); 
    }
    
    bool has_current() const noexcept { return has_curr(); }

    T & get_curr() const
    {
      if (curr_idx == array_ptr->size())
	throw std::overflow_error("not current item in iterator");
      return array_ptr->access(curr_idx); 
    }

    long get_pos() const noexcept { return curr_idx; }

    void next() 
    {
      if (curr_idx == array_ptr->size())
	throw std::overflow_error("not current item in iterator");
	  
      ++curr_idx;
    }

    void prev()
    {
      if (curr_idx == -1)
	throw std::underflow_error("not current item in iterator");

      --curr_idx;
    }

    void reset_last() noexcept { curr_idx = array_ptr->size() - 1; }

    void end() noexcept { curr_idx = array_ptr->size(); }

    void reset_first() noexcept { curr_idx = 0; }
  };

private:

    // super fast array traversal
  template <class Operation>
  bool __traverse(Operation & operation) noexcept(noexcept(operation))
  {
    size_t dir_idx = 0, seg_idx = 0, block_idx = 0;
    for (size_t i = 0; i < current_dim; ++i)
      {
	if (not operation(dir[dir_idx][seg_idx][block_idx]))
	  return false;
	
	if (++block_idx == block_size)
	  {
	    block_idx = 0;
	    if (++seg_idx == seg_size)
	      {
		seg_idx = 0;
		++dir_idx;
	      }
	  }
      }
    return true;
  }

public:

  template <class Operation>
  bool traverse(Operation & operation) const noexcept(noexcept(operation))
  {
    return const_cast<DynArray&>(*this).__traverse(operation);
  }

  template <class Operation>
  bool traverse(Operation & operation) noexcept(noexcept(operation))
  {
    return __traverse(operation); 
  }

  template <class Operation>
  bool traverse(Operation && operation) const noexcept(noexcept(operation))
  {
    return traverse<Operation>(operation);
  }

  template <class Operation> bool traverse(Operation && operation) 
    noexcept(noexcept(operation))
  {
    return traverse<Operation>(operation);
  }
};

    template <typename T> 
const size_t DynArray<T>::Default_Pow_Dir   = 6;  /* 64   */

    template <typename T> 
const size_t DynArray<T>::Default_Pow_Seg   = 8;  /* 256   */

    template <typename T> 
const size_t DynArray<T>::Default_Pow_Block = 12; /* 4096 */

    template <typename T> 
const size_t DynArray<T>::Max_Bits_Allowed  = 8 * sizeof(size_t); 

    template <typename T> 
const unsigned long long  DynArray<T>::Max_Dim_Allowed = 
      256*1024*1024*1024ull; // 256 Gb

    template <typename T> 
const size_t DynArray<T>::Max_Pow_Block =
  (Max_Bits_Allowed - Default_Pow_Dir  - Default_Pow_Seg  - 1);

} // end namespace Aleph

# endif /* TPL_DYNARRAY_H */

