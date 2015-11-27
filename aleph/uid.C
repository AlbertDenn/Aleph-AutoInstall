
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


# include <uid.H>


typedef unsigned char Byte;

static char * hexadecimalize(const Byte & byte, char *& str)
{
  Byte l = byte & 0x0F;
  Byte h = (byte & 0xF0) >> 4;

  *str++ = nibble_to_char(h);
  *str++ = nibble_to_char(l);

  return str;
}


static char unhexadecimalize(char *& str)
{
  Byte h = char_to_nibble(*str++) << 4;
  Byte l = char_to_nibble(*str++);

  return h | l;
}


char * Uid::stringficate(char *         buffer,
			 const size_t & buf_size) const
{
  if (buf_size < 2*sizeof(Uid) + 1)
    throw std::range_error("Buffer size is not enough");

  char * this_str = (char*) this;
  char * ret_val = buffer;

      // copiar nibbles convertidas a ascii a buffer
  for (int i = 0; i < sizeof(Uid); ++i)
    hexadecimalize(*this_str++, buffer);

  assert(this_str - (char*) this == sizeof(Uid));
  
  *buffer = '\0';

  return ret_val;
}

void Uid::destringficate(char * str)
{
  char * this_str = (char*) this;

      // convertir el string en ascii a la representación en nibbles
  for (int i = 0; i < sizeof(Uid); ++i)
    *this_str++ = unhexadecimalize(str);
}


Uid::Uid(const IPv4_Address & _ipAddr, 
	 const unsigned int & _counter, 
	 const unsigned int & _port_number)
  : ipAddr(_ipAddr), port_number(_port_number), counter(_counter)
{
  random_number = rand();
}


Uid::Uid(char* str)
{
  destringficate(str);
  
}

bool Uid::operator == (const Uid& uid) const
{
  return (ipAddr == uid.ipAddr and 
	  port_number == uid.port_number and
	  counter == uid.counter and
	  random_number == uid.random_number);
}

char * Uid::getStringUid(char * str, const size_t & str_size) const
{
  if (str_size < stringSize)
    return NULL;

  return stringficate(str, str_size);
}



