
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


#include <iostream>
#include <stdlib.h>
#include <signal.h>

void handler(int signo, siginfo_t *info, void *ptr)
{
  cout << "Recibida senal " << signo << " (" << info->si_signo 
       << ") desde el proceso " << info->si_pid << endl
       << "info = " << info << endl;

  if (info->si_code == SEGV_MAPERR)
    cout  << "Se~nal enviada porque la direccion " 
	  << info->si_addr << endl
	  << "no esta mapeada a algun objeto" << endl;
  else if (info->si_code == SEGV_ACCERR)
    cout  << "Se~nal enviada porque el objeto almacenado" << endl
	  << "la direccion " << info->si_addr << endl
	  << "no autoriza el acceso" << endl;
  else
    cout  << "Motivo de la senal (" << info->si_code 
	  << "): indeterminado" << endl;

  cout  << "La direccion " << info->si_addr << endl
	<< "no esta mapeada a algun objeto" << endl;

  exit(0);
}

int main(void)
{
  struct sigaction action;

  //  action.sa_handler = NULL;
  action.sa_sigaction = &handler;
  sigemptyset (&action.sa_mask);
  action.sa_flags = SA_SIGINFO;

  sigaction(SIGSEGV, &action, NULL);

  char *ptr = NULL;

  *ptr = '\0';
}
