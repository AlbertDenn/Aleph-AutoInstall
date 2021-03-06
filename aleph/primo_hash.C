
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


# include <iostream>
# include <tpl_dynArray.H>

using namespace std;

int n = 7;
int primes[ /* 7 */ ] = {11, 13, 17, 23, 29, 31, 37, 0 };


int numbers[] = { 14, 22, 39, 46, 54, 79, 198, 0 };

int prime_hash(int keys[])
{
  int prime_index = 0;
  int current_prime = primes[0];
  int current_index = 0;
  int i;

  DynArray<int> hashes(100);

  while (true)
    {
    de_nuevo:
      hashes[current_index] = keys[current_index] % current_prime;

      cout << "Probando para primo " << current_prime << " y clave "
	   << keys[current_index] << " (resto = " << hashes[current_index]
	   << ")" << endl;

      /* Busca si existe una colision */
      i = 0; 
      while (i < current_index)
	{
	  if (hashes[current_index] == hashes[i])
	    {
	      cout << "    Colision de " << keys[current_index] << " con " 
		   << keys[i] << endl;
	      current_prime = primes[++prime_index];
	      if (current_prime == 0)
		{
		  cout << "No hay solucion" << endl;
		  return -1;
		}
	      else
		{
		  cout << "    Avanzando a primo " << current_prime << endl;
		  current_index = 0;
		  goto de_nuevo;
		}
	    }		  
	  i++;
	}

      /* no hubo colision, avance a la proxima clave */
      current_index++;
      if (keys[current_index] == 0)
	{
	  cout << "Encontrado numero primo = " << current_prime << endl;
	  for (i = 0; i < current_index; i++)
	    cout << keys[i] << " mod " << current_prime << " = " 
		 << keys[i] % current_prime << endl;
	  cout << endl;
	  return current_prime;
	}
    }
}


int main()
{
  prime_hash(numbers);
}
