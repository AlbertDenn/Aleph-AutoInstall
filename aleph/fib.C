
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
# include <iostream>
# include <tpl_arrayStack.H>

using namespace std;

int fib_it(int n)
{
  if (n <= 1)
    return 1;

  int fi_1 = 1, fi_2 = 1, fi = 0, i;

  for (i = 2; i <= n; i++, fi_2 = fi_1, fi_1 = fi)
    fi = fi_1 + fi_2;

  return fi;
}

int fib_rec(int n)
{
  if (n <= 1)
    return 1;

  int f1 = fib_rec(n - 1);
  int f2 = fib_rec(n - 2);
  return f1 + f2;
}


# define P0 0
# define P1 1
# define P2 2

struct Item
{
  int n;
  int f1;
  int result;
  char return_point;
};

# define N  (stack.lookAt().n)
# define F1 (stack.lookAt().f1)
# define RESULT (stack.lookAt().result)
# define RETURN_POINT (stack.lookAt().return_point)

int fib_st(int n)
{
  ArrayStack<Item> stack;
  Item first_activation_register;

  first_activation_register.n = n;

  stack.push(first_activation_register);

  while (1)
    {
      if (N <= 1)
	{
	  current_activation_register.result = 1;
	  goto exit_from_fib;
	}
       
      { /* Esta es la llamada a fib(n - 1) */
	Item new_activation_register;
	new_activation_register.n     = current_activation_register.n - 1;
	new_activation_register.point = P1;
	stack.push(new_activation_register);
	continue;
      }

    p1: /* Esta es la salida de fib(n - 1) */ 
      current_activation_register.f1 = 
	current_activation_register.result; /* int f1 = fib(n - 1) */

      { /* Esta es la llamada a fib(n - 2) */
	Item new_activation_register;
	new_activation_register.n = current_activation_register.n - 2;
	new_activation_register.point = P2;
	stack.push(new_activation_register);
	continue;
      }

    p2: /* Esta es la salida de fib(n - 2) */ 
      current_activation_register.result = 
	current_activation_register.f1 + current_activation_register.result;

    exit_from_fib:
      if (stack.is_empty())
	return current_activation_register.result;

      char return_point = current_activation_register.point;
      stack.pop();
      current_activation_register = stack.lookAt();
      switch (return_point)
	{
	case P1: goto p1;
	case P2: goto p2;
	}
    }
}



int main(int argn, char * argv[])
{
  int n = atoi(argv[1]);

  cout << "fib(" << n << ") = " << fib_rec(n) << " = " << fib_it(n) 
       << " = " << fib_st(n) << endl;
}
