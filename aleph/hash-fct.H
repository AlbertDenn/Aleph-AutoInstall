
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

# ifndef HASH_FCT_H
# define HASH_FCT_H

# include <stdint.h>
# include <string.h>
# include <cstdint>
# include <primes.H>

namespace Aleph
{

/* These group of hash functions are taken from Julienne Walker at

   http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx

   My only contribution was to copy and paste them and to adapt them to
   unsigned long instead of unsigned int. Apart thar, all the credit
   for these functions must be adwarded to Julienne Walker

   The quoted comments correspond to direct cites of Julienne comments
*/

extern const unsigned Default_Hash_Seed;

inline size_t add_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  for (int i = 0; i < len; i++)
    h += p[i];
  
  return h;
}

inline size_t xor_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  for (int i = 0; i < len; i++)
    h ^= p[i];
  
  return h;
}


inline size_t rot_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  for (int i = 0; i < len; i++)
    h = (h << 4) ^(h >> 28) ^ p[i];
  
  return h;
}


inline size_t djb_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  for (int i = 0; i < len; i++)
    h = 33 * h ^ p[i];
  
  return h;
}

inline size_t sax_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;  
  
  for (int i = 0; i < len; i++)
    h ^= (h << 5) +(h >> 2) + p[i];
  
  return h;
}

inline size_t fnv_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 2166136261;  
  
  for (int i = 0; i < len; i++)
    h = (h * 16777619) ^ p[i];
  
  return h;
}


inline size_t oat_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;  
  
  for (int i = 0; i < len; i++) 
    {
      h += p[i];
      h += (h << 10);
      h ^= (h >> 6);
    }
 
  h += (h << 3);
  h ^= (h >> 11);
  h += (h << 15);
 
  return h;
}

extern size_t jsw_hash(void * key, size_t len) noexcept;


inline size_t elf_hash(void * key, size_t len) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0, g;
  
  for (int i = 0; i < len; i++) 
    {
      h = (h << 4) + p[i];
      g = h & 0xf0000000L;
 
      if(g != 0)
	h ^= g >> 24;
 
      h &= ~g;
    }
 
  return h;
}


#define hashsize(n)(1U << (n))
#define hashmask(n)(hashsize(n) - 1)
  
inline void  mix(size_t & a, size_t & b, size_t & c) noexcept
{ 
  a -= b; a -= c; a ^=(c >> 13); 
  b -= c; b -= a; b ^=(a << 8);			
  c -= a; c -= b; c ^=(b >> 13); 
  a -= b; a -= c; a ^=(c >> 12);
  b -= c; b -= a; b ^=(a << 16);
  c -= a; c -= b; c ^=(b >> 5);
  a -= b; a -= c; a ^=(c >> 3);
  b -= c; b -= a; b ^=(a << 10);
  c -= a; c -= b; c ^=(b >> 15);
}


inline size_t jen_hash(void * key, size_t length, 
		       unsigned initval = Default_Hash_Seed) noexcept
{
  unsigned char * k = (unsigned char *) key;
  size_t a, b;
  size_t c = initval;
  size_t len = length;
 
  a = b = 0x9e3779b9;
 
  while (len >= 12) 
    {
      a += (k[0] +((size_t)k[1] << 8) + ((size_t)k[2] << 16) +
	    ((size_t)k[3] << 24));
      b += (k[4] +((size_t)k[5] << 8) + ((size_t)k[6] << 16) +
	    ((size_t)k[7] << 24));
      c += (k[8] +((size_t)k[9] << 8) +((size_t)k[10] << 16) +
	    ((size_t)k[11] << 24));
 
      mix(a, b, c);
 
      k += 12;
      len -= 12;
    }
 
  c += length;
 
  switch(len) 
    {
    case 11: c += ((size_t)k[10] << 24);
    case 10: c += ((size_t)k[9] << 16);
    case 9 : c += ((size_t)k[8] << 8);
      /* First byte of c reserved for length */
    case 8 : b += ((size_t)k[7] << 24);
    case 7 : b += ((size_t)k[6] << 16);
    case 6 : b += ((size_t)k[5] << 8);
    case 5 : b += k[4];
    case 4 : a += ((size_t)k[3] << 24);
    case 3 : a += ((size_t)k[2] << 16);
    case 2 : a += ((size_t)k[1] << 8);
    case 1 : a += k[0];
    }
 
  mix(a, b, c);
 
  return c;
}

/* The following three functions were taken from 

   https://github.com/PeterScott/murmur3

   They are a port of MurmurHash created by Austin Appleby and written
   by Peter Scott

   See at

   http://en.wikipedia.org/wiki/MurmurHash

   For more details
*/

void MurmurHash3_x86_32 (const void *key, int len, uint32_t seed, void *out);

void MurmurHash3_x86_128(const void *key, int len, uint32_t seed, void *out);

void MurmurHash3_x64_128(const void *key, int len, uint32_t seed, void *out);

struct Buf128Bits
{
  std::uint32_t a[4];
};

  template <typename Key> inline
size_t murmur3hash(const Key & key, unsigned long seed)
{
# ifdef __x86_64__
  Buf128Bits buf;
  MurmurHash3_x64_128(&key, sizeof(key), seed, &buf);
# else
  size_t buf;
  MurmurHash3_x86_32(&key, sizeof(key), seed, &buf);
# endif

  size_t ret;
  memcpy(&ret, &buf, sizeof(ret));

  return ret;  
}


inline size_t murmur3hash(char * key, unsigned long seed)
{
  size_t len = strlen(key);
# ifdef __x86_64__
  Buf128Bits buf;
  MurmurHash3_x64_128(key, len, seed, &buf);
# else
  size_t buf;
  MurmurHash3_x86_32(key, len, seed, &buf);
# endif

  size_t ret;
  memcpy(&ret, &buf, sizeof(ret));

  return ret;  
}

template <> inline
size_t murmur3hash(const std::string & key, unsigned long seed)
{
# ifdef __x86_64__
  Buf128Bits buf;
  MurmurHash3_x64_128(key.c_str(), key.size(), seed, &buf);
# else
  size_t buf;
  MurmurHash3_x86_32(key.c_str(), key.size(), seed, &buf);
# endif

  size_t ret;
  memcpy(&ret, &buf, sizeof(ret));

  return ret;  
}




/* Paul Hsieh super fast hash function.

   Taken from

   http://www.azillionmonkeys.com/qed/hash.html
 */

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

inline size_t SuperFastHash(void * key, size_t len) noexcept 
{
  const unsigned char * data = (unsigned char *) key;
  size_t hash = len, tmp;
  int rem;

  if (len <= 0 || data == nullptr) 
      return 0;

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (;len > 0; len--) 
    {
      hash  += get16bits (data);
      tmp    = (get16bits (data+2) << 11) ^ hash;
      hash   = (hash << 16) ^ tmp;
      data  += 2*sizeof (uint16_t);
      hash  += hash >> 11;
    }

    /* Handle end cases */
  switch (rem) 
    {
    case 3: 
      hash += get16bits (data);
      hash ^= hash << 16;
      hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
      hash += hash >> 11;
      break;
    case 2: 
      hash += get16bits (data);
      hash ^= hash << 11;
      hash += hash >> 17;
      break;
    case 1: 
      hash += (signed char)*data;
      hash ^= hash << 10;
      hash += hash >> 1;
    }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

template <typename Key> inline
size_t add_hash(const Key & key) noexcept
{
  return add_hash((void *) &key, sizeof(key));
}

template <typename Key> inline
size_t xor_hash(const Key & key) noexcept
{
  return xor_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t rot_hash(const Key & key) noexcept
{
  return rot_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t djb_hash(const Key & key) noexcept
{
  return djb_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t sax_hash(const Key & key) noexcept
{
  return sax_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t fnv_hash(const Key & key) noexcept
{
  return fnv_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t oat_hash(const Key & key) noexcept
{
  return oat_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t jsw_hash(const Key & key) noexcept
{
  return jsw_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t elf_hash(const Key & key) noexcept
{
  return elf_hash((void *) &key, sizeof(key));
}


template <typename Key>
inline size_t jen_hash(const Key &  key, unsigned initval) noexcept
{
  return jen_hash((void *) &key, sizeof(key), initval);
}


template <typename Key>
inline size_t SuperFastHash(const Key &  key) noexcept 
{
  return SuperFastHash((void *) &key, sizeof(key));
}



inline size_t add_hash(char * key) noexcept
{
  unsigned char * p = (unsigned char *) key;
  size_t h = 0;
  
  while (*p)
    h += *p++;
  
  return h;
}

inline size_t xor_hash(const char * key) noexcept
{
  unsigned char * p = (unsigned char *) key;
  size_t h = 0;
  
  while (*p)
    h ^= *p++;
  
  return h;
}


inline size_t rot_hash(const char * key) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  while (*p)
    h = (h << 4) ^(h >> 28) ^ *p++;
  
  return h;
}


inline size_t djb_hash(const char * key) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;
  
  while (*p)
    h = 33 * h ^ *p++;
  
  return h;
}


inline size_t sax_hash(const char * key)
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;  
  
  while (*p)
    h ^= (h << 5) +(h >> 2) + *p++;
  
  return h;
}


inline size_t fnv_hash(const char * key) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 2166136261;  
  
  while (*p)
    h = (h * 16777619) ^ *p++;
  
  return h;
}


inline size_t oat_hash(const char * key) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0;  
  
  while (*p)
    {
      h += *p++;
      h += (h << 10);
      h ^= (h >> 6);
    }
 
  h += (h << 3);
  h ^= (h >> 11);
  h += (h << 15);
 
  return h;
}


extern size_t jsw_hash(const char * key);


inline size_t elf_hash(const char * key) noexcept
{
  unsigned char *p = (unsigned char *) key;
  size_t h = 0, g;
  
  while (*p)
    {
      h =(h << 4) + *p++;
      g = h & 0xf0000000L;
 
      if(g != 0)
	h ^= g >> 24;
 
      h &= ~g;
    }
 
  return h;
}


inline size_t SuperFastHash(const char * key) noexcept 
{
  return SuperFastHash((void*) key, strlen(key));
}

template <>
inline size_t add_hash(const std::string & key) noexcept
{
  return add_hash(key.c_str());
}


inline size_t xor_hash(const std::string & key) noexcept
{
  return xor_hash(key.c_str());
}


inline size_t rot_hash(const std::string & key) noexcept
{
  return rot_hash(key.c_str());
}


inline size_t djb_hash(const std::string & key) noexcept
{
  return djb_hash(key.c_str());
}


inline size_t sax_hash(const std::string & key) noexcept
{
  return sax_hash(key.c_str());
}


inline size_t fnv_hash(const std::string & key) noexcept
{
  return fnv_hash(key.c_str());
}


inline size_t oat_hash(const std::string & key) noexcept
{
  return oat_hash(key.c_str());
}


inline size_t jsw_hash(const std::string & key) noexcept
{
  return jsw_hash(key.c_str());
}


inline size_t elf_hash(const std::string & key) noexcept
{
  return elf_hash(key.c_str());
}


inline size_t jen_hash(const std::string & key, unsigned initval) noexcept
{
  return jen_hash(key.c_str(), initval);
}


inline size_t SuperFastHash(const std::string & key) noexcept 
{
  return SuperFastHash(key.c_str());
}


template <typename Key> inline
size_t dft_hash_fct(const Key & key) noexcept
{
  return SuperFastHash(key);
}

template <typename Key> inline
size_t snd_hash_fct(const Key & key) noexcept
{
  //  return elf_hash(key);
  return jen_hash(key, 52679987);
}


template <typename Key> inline
size_t dft_hash_fct(const Key & key, unsigned long seed) noexcept
{
  return murmur3hash(key, seed);
}



} // end namespace Aleph


# endif // HASH_FCT_H
