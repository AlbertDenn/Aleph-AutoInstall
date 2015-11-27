
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


# include <stdlib.h>
# include <stdio.h>
# include <stdarg.h>
# include <ahAssert.H>
# include <ahDefs.H>

/*
 These debug functions are only defined in basic.H if DEBUG flag is
 defined. However, since basic.o is destined to be a library and that
 is not frequently compiled, we do not use the preprocessor for
 conditioning the functions definitions. Because they are in library,
 they are only used if they are inquired. So, we minimize misuse
 e.g. if we forget to define DEBUG, we get a "undefined reference"
 message while linking symbol using DEBUG
*/ 

void _assert(const char *exp, const char* fileName, unsigned lineNumber)
{
  MESSAGE("Iion %s in line %u file %s violated\n", 
	  exp, lineNumber, fileName);
  abort();
}

void _assert(const char *exp, 
	     const char*  fileName, 
	     unsigned int lineNumber,
	     const char*  format,
	     ...)
{
  MESSAGE("Iion %s in line %u file %s violated\n", 
	  exp, lineNumber, fileName);
  
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);

  abort();
}

void _Warning(const char *exp, const char* fileName, unsigned lineNumber)
{
  MESSAGE("Condition %s in line %u file %s has been violated\n"
	  "Programmer decided does not abort on that violation\n", 
	  exp, lineNumber, fileName);
}

void _Warning(const char *exp, 
	      const char*  fileName, 
	      unsigned int lineNumber,
	      const char*  format,
	      ...)
{
  MESSAGE("Warning %s in line %u file %s\n"
	 "(Programmer decided does not abort on that violation)\n",
	  exp, lineNumber, fileName);
  
  va_list ap;
  va_start(ap, format);
  vprintf(format, ap);
  va_end(ap);

  abort();
}

