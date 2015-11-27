
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

# define P1 1 
# define P2 2

struct Activation_Record
{
  int n;
  int f1;
  int result;
  char return_point;
};
# define NUM(p)          ((p)->n) 
# define F1(p)           ((p)->f1)
# define RESULT(p)       ((p)->result)
# define RETURN_POINT(p) ((p)->return_point)
int fib_st(const int & n) 
{
  ArrayStack<Activation_Record> stack;
  Activation_Record * caller_ar  = &stack.pushn();
  Activation_Record * current_ar = &stack.pushn();
  NUM(current_ar) = n;
  start:
   if (NUM(current_ar) <= 1)
     {
       RESULT(caller_ar) = 1;
       goto return_from_fib;
     }
   RETURN_POINT(current_ar) = P1;
   NUM(&stack.pushn()) = NUM(current_ar) - 1; // crea reg. act.
   caller_ar  = &stack.top(1);
   current_ar = &stack.top();
   goto start;
  p1: /* punto de retorno de fib(n - 1) */
   F1(current_ar) = RESULT(current_ar);
     NUM(&stack.pushn()) = NUM(current_ar) - 2; // crea reg. act. 
   RETURN_POINT(current_ar) = P2;
   caller_ar  = &stack.top(1);
   current_ar = &stack.top();
   goto start;
  p2: /* punto de retorno de fib(n - 2) */
   RESULT(caller_ar) = F1(current_ar) + RESULT(current_ar);
  return_from_fib:
   stack.pop(); /* cae en el registro del invocante */
   if (stack.size() == 1) 
     return RESULT(caller_ar);

   caller_ar  = &stack.top(1);
   current_ar = &stack.top();

   switch (RETURN_POINT(current_ar))
     {
     case P1: goto p1;
     case P2: goto p2;
     default: ERROR("Invalid return point case");
     } 

   return 0; // never reached!
}
