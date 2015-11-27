
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

# include <ahDefs.H>
# include <pthread.h>
# include <useMutex.H>
# include <syslog.h>

# define STR_LOG_LEN 256

namespace Aleph
{
  bool daemonized = false;

  const IPv4_Address Null_IPv4_Address = 0;

  bool verbose = false;

  double max_time = 300; // seconds
}

void Aleph::syslog(const char * format, va_list ap)
{
  int str_log_len = STR_LOG_LEN;

 repeat:
  //TODO: apartar el arreglo con obstack
  char log_str[str_log_len + 1];
  int written_bytes = vsnprintf(log_str, str_log_len, format, ap);
  if (written_bytes < str_log_len && written_bytes > -1)
    {
      ::syslog(LOG_NOTICE, "%s", log_str);
      return;
    }

  str_log_len = written_bytes;
  
  goto repeat;
}


# ifdef _PTHREAD_H
static pthread_mutex_t aleph_message_mutex = PTHREAD_MUTEX_INITIALIZER; 
# endif

# ifdef MESSAGES
void Aleph::message(const char* file, int line, const char* format, ...)
{
# ifdef _PTHREAD_H
  CRITICAL_SECTION(aleph_message_mutex);
# endif

  va_list ap;
  
  va_start(ap, format);

  if (Aleph::daemonized)
    {
      ::syslog(LOG_NOTICE, "%s: %u\n", file, line); 
      Aleph::syslog(format, ap);
    }
  else
    {
      printf("%s:%u\n", file, line);
      vfprintf(stdout, format, ap);
      printf("\n");
    }
  va_end(ap);
}
# else

void Aleph::message(const char*, int, const char*, ...) { }

# endif


void Aleph::error(const char* file, int line, const char* format, ...)
{
# ifdef _PTHREAD_H
  CRITICAL_SECTION(aleph_message_mutex);
# endif 

  va_list ap;
  
  va_start(ap, format);

  if (Aleph::daemonized)  
    {
      ::syslog(LOG_ERR, "Fatal error detected at %s %u\n", file, line);
      Aleph::syslog(format, ap);
    }
  else
    {
      printf("Fatal error detected in %s %u\n", file, line); 
      vfprintf(stdout, format, ap);
      printf("\n");
    }

  va_end(ap);

  abort();
}

void Aleph::exit(const char* file, int line, const char* format, ...) 
{
# ifdef _PTHREAD_H
  CRITICAL_SECTION(aleph_message_mutex);
# endif

  va_list ap;
  
  va_start(ap, format);

  if (Aleph::daemonized)  
    {
      ::syslog(LOG_NOTICE, "%s: %u\n", file, line); 
      Aleph::syslog(format, ap);
    }
  else
    {
      vfprintf(stdout, format, ap);
      printf("%s:%u\n", file, line);
      printf("\n");
    }
  
  va_end(ap);

  ::exit(0);
}


void Aleph::warning(const char* file, int line, const char* format, ...)
{
# ifdef _PTHREAD_H
  CRITICAL_SECTION(aleph_message_mutex);
# endif

  va_list ap;
  
  va_start(ap, format);
  
  if (daemonized)  
    {
      ::syslog(LOG_WARNING, "Warning detected in %s %u\n", file, line); 
      Aleph::syslog(format, ap);
    }
  else
    {
      printf("Warning detected in %s %u\n", file, line); 
      vfprintf(stdout, format, ap);
      printf("\n");
    }
  
  va_end(ap);
}
