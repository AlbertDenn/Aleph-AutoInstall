
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


# include <Simplex.H>

int main()
{
  Simplex<float> s(4);

  s.put_objetive_function_coef(0, 40);
  s.put_objetive_function_coef(1, 50);
  s.put_objetive_function_coef(2, 60);
  s.put_objetive_function_coef(3, 30);

  float * r1 = s.put_restriction();
  float * r2 = s.put_restriction();
  float * r3 = s.put_restriction();
  float * r4 = s.put_restriction();

  r1[0] = 2; r1[1] = 1; r1[2] = 2; r1[3] = 2; r1[4] = 205;

  r2[0] = 1; r2[1] = 1; r2[2] = 3; r2[3] = 1; r2[4] = 205;

  r3[0] = 1; r3[1] = 3; r3[2] = 4; r3[4] = 255;

  r4[0] = 3; r4[1] = 2; r4[2] = 2; r4[3] = 2; r4[4] = 250;

  s.latex_linear_program("sistema-simplex.tex");

  s.prepare_linear_program();

  Simplex<float>::State  state = s.latex_solve("mat");

  if (state == Simplex<float>::Unbounded)
    cout << "No se puede resolver el sistema" << endl;

  s.load_solution();

  s.print_matrix();

  for (int i = 0; i < 4; i++)
    cout << "x" << i << " = " << s.get_solution(i) << endl;

  assert(s.verify_solution());
}
