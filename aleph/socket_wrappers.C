
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

# include <errno.h>
# include <aleph.H>
# include <socket_wrappers.H>

extern "C"
{

ssize_t readn(int fd, void *vptr, size_t n)
{
  ssize_t nread;

  char * ptr = static_cast<char *>(vptr);
  size_t nleft = n;

  while (nleft > 0) 
    {
      if ( (nread = read(fd, ptr, nleft)) < 0) 
	{
          if (errno == EINTR)
	    nread = 0; /* and call read() again */
	  else
	    return -1;
	} 
      else 
	if (nread == 0)
	  break; /* EOF */

      nleft -= nread;
      ptr   += nread;
    }

  return n - nleft;              /* return >= 0 */
}


ssize_t writen(int fd, const void *vptr, size_t n)
{
  ssize_t         nwritten;

  const char * ptr = static_cast<const char *>(vptr);
  size_t nleft = n;

  while (nleft > 0) 
    {
      nwritten = write(fd, ptr, nleft);
      if ( nwritten <= 0) 
	{
	  if (errno == EINTR)
	    nwritten = 0;           /* call write() again */
	  else
	    return -1;
	}
      nleft -= nwritten;
      ptr   += nwritten;
    }

  return n;
}


static const size_t buffer_size = 1024;

ssize_t flushn(int fd, size_t n)
{
  char buffer[buffer_size];

  int num_reads      = n / buffer_size;
  int last_read_size = n % buffer_size;

  int read_size;

  for (int i = 0; i < num_reads; i++)
    {
      read_size = readn(fd, &buffer[0], buffer_size);
      if (read_size < 0)
	return read_size;
    }

  if (last_read_size == 0)
    return n;

  read_size = readn(fd, &buffer[0], last_read_size);

  return read_size;
}


}



