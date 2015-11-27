
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

# include <stdexcept>
# include <iostream>
# include <fstream>
# include <math.h>


using namespace std;

struct Quebrado
{
  int e; // parte entera
  int n; // numerador
  int d; // denominador
};


void asignar_quebrado(const Quebrado & q1, Quebrado & q2)
{
  q2.e = q1.e;
  q2.n = q1.n;
  q2.d = q1.d;
}


Quebrado quebrado_cero()
{
  Quebrado q;

  q.e = 0;
  q.n = 0;
  q.d = 1;

  return q;
}


Quebrado quebrado_uno()
{
  Quebrado q;

  q.e = 1;
  q.n = 0;
  q.d = 1;

  return q;
}


bool es_propio(const Quebrado & q)
{
  return q.d > q.n;
}


bool es_primo (int n)
{
  int max = ceil(sqrt(n));

  for (int i = 2; i < max; i += 2)
    if ((n % i) == 0)
      return false;
  
  return true;
}	


void hacer_propio(Quebrado & q)
{
  if (es_propio(q))
    return;

  q.e = q.e + q.n/q.d;
  q.n    = q.n % q.d;
} 


void reducir_al_menor_denominador(Quebrado & q)
{
  while ((q.n % 2 == 0) and (q.d % 2 == 0))
    {
      q.n   = q.n/2;
      q.d = q.d/2;
    }
  
  int ultimo_divisor = ceil(sqrt(q.d));
  for (int divisor = 3; divisor <= ultimo_divisor; divisor += 2)
    if ((q.n % divisor == 0) and (q.d % divisor == 0))
      {
	q.n   = q.n/divisor;
	q.d = q.d/divisor; 
      }
}


void simplificar_quebrado(Quebrado & q)
{
  hacer_propio(q);
  reducir_al_menor_denominador(q);
}	


void leer_quebrado_por_consola(Quebrado & q)
{
  cout << "ingrese entero, numerador y denominador ";
  cin >> q.e;
  cin >> q.n;
  cin >> q.d;
}


void escribir_quebrado_por_consola(const Quebrado & q)
{
  cout << q.e << " " << q.n << "/" << q.d <<endl;
}


Quebrado suma(const Quebrado & q1, const Quebrado & q2)
{
  Quebrado r;

  r.e = q1.e + q2.e;

  if (q1.n == 0)
    {
      r.n = q2.n;
      r.d = q2.d;
    }
  else if (q2.n == 0)
    {
      r.n = q1.n;
      r.d = q1.d;
    }
  else
    {
      r.d = q1.d * q2.d;
      r.n = (r.d / q1.d)*q1.n + (r.d / q2.d)*q2.n;
    }
  
  simplificar_quebrado(r);

  return r;
}


Quebrado negado((const Quebrado & q)
{
  Quebrado r;
  asignar_quebrado(q, r);
  r.e = -r.e;

  return r;
}

Quebrado resta(const Quebrado & q1, const Quebrado & q2)
{
  return suma(q1, negado(q2));
}

Quebrado multiplica(const Quebrado & q1, const Quebrado & q2)
{
  Quebrado r1 = quebrado_cero();
  r1.e = q1.e*q2.e;

  Quebrado r2 = quebrado_cero();
  r2.n = q1.e*q2.n;
  r2.d = q2.d;
  hacer_propio(r2);

  Quebrado r3 = quebrado_cero();
  r3.n = q1.n*q2.e;
  r3.d = q1.d;
  hacer_propio(r3);

  Quebrado r4 = quebrado_cero();
  r4.n = q1.n*q2.n;
  r4.d = q1.d*q2.d;
  hacer_propio(r4);

  return suma(r1, suma(r2, suma(r3, r4)));
}


Quebrado invertido(const Quebrado & q)
{
  if (q.e == 0 and q.n == 0)
    throw domain_error("División por cero");

  Quebrado r = quebrado_cero();
  r.n = q.d;
  r.d = q.e*q.d + q.n;

  hacer_propio(r);

  return r;
}


Quebrado divide(const Quebrado & q1, const Quebrado & q2)
{
  return multiplica(q1, invertido(q2));
}


int main()
{
  
}
