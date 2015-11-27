
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


# include <tpl_dynDlist.H>

class Polinomio
{
  struct Termino
  {
    int    coef;
    size_t pot;
    Termino() : coef(0), pot(0) {}

    Termino(const int & c, const size_t & p) : coef(c), pot(p) {}
    Termino& operator += (const Termino & der)
    {

      assert(pot == der.pot);

      if (this == &der)
        return *this;

      coef += der.coef;
      return *this;
    }
    Termino operator * (const Termino & der) const
    {
      return Termino(coef*der.coef, pot + der.pot);
    }
  };
  DynDlist<Termino> terminos;
  Polinomio(const Polinomio::Termino & termino)
  {
    terminos.append(termino);
  }
  Polinomio multiplicado_por(const Termino & term) const
  {
    Polinomio result;
    if (terminos.is_empty() or term.coef == 0) 
      return result;

    for (DynDlist<Termino>::Iterator it((DynDlist<Termino>&) terminos); 
         it.has_current(); it.next())
      result.terminos.append(Termino(it.get_current().coef * term.coef,
                                     it.get_current().pot + term.pot) );
    return result;
  }

public:

  Polinomio();
  Polinomio(const int& coef, const size_t & pot);
  Polinomio operator + (const Polinomio&) const;
  Polinomio& operator += (const Polinomio&);
  Polinomio operator * (const Polinomio&) const;
  Polinomio operator - (const Polinomio&) const;

  Polinomio operator / (const Polinomio&) const;

  Polinomio operator % (const Polinomio&) const;
  const size_t & size() const;
  size_t get_power(const size_t & i) const;
  const int & get_coef(const size_t & i) const;
  const size_t & get_degree() const;
  void print() const
  {
    if (terminos.is_empty())
      {
        printf("0\n\n");
        return;
      }
    Termino current_term;
    for (DynDlist<Termino>::Iterator it(const_cast<DynDlist<Termino>&>(terminos)); 
         it.has_current(); it.next())
      {
        current_term = it.get_current();
        printf(" %c ", current_term.coef < 0 ? '-' : '+');

        if (abs(current_term.coef) not_eq 1)
          printf("%u", abs(current_term.coef));

        printf("X^%zu", current_term.pot);
      }

    printf("\n\n");
  }
};
Polinomio::Polinomio(const int & coef, const size_t & pot)
{
  terminos.append(Termino(coef, pot));
}
Polinomio::Polinomio() { /* empty */ }
Polinomio Polinomio::operator + (const Polinomio & der) const
{
  Polinomio ret_val(*this); // valor de retorno operando derecho
  ret_val += der; // súmele operando derecho
  return ret_val;
}
Polinomio & Polinomio::operator += (const Polinomio& der)
{
  if (der.terminos.is_empty())
    return *this;

  if (terminos.is_empty())
    {
      *this = der;
      return *this;
    }
  DynDlist<Termino>::Iterator it_izq(terminos);
  DynDlist<Termino>::Iterator it_der(const_cast<DynDlist<Termino>&>(der.terminos));
  while (it_izq.has_current() and it_der.has_current())
    {
      const size_t & izq = it_izq.get_current().pot;
      const size_t & der = it_der.get_current().pot;
      if (izq < der)
        {     // insertar a la izquierda del actual de it_izq 
          it_izq.append(Termino(it_der.get_current().coef, der)); 
          it_der.next(); // ver próximo término de polinomio derecho
          continue;
        }
      if (izq == der)
        {      // calcular coeficiente resultado
          it_izq.get_current() += it_der.get_current(); // += Termino
          it_der.next(); // avanzar a sig término polinomio derecho
          if (it_izq.get_current().coef == 0) // ¿suma anula término?
            {    // sí, borrarlo de polinomio izquierdo (coeficiente 0)
              it_izq.del();
              continue;
            }
        }
      it_izq.next();
    }
  while (it_der.has_current())
    {    // copia términos restantes derecho a izquierdo
      terminos.append(Termino(it_der.get_current().coef, 
                              it_der.get_current().pot));
      it_der.next();
    }
  return *this;
}
Polinomio Polinomio::operator * (const Polinomio & der) const
{
  Polinomio result;
  if (terminos.is_empty() or der.terminos.is_empty()) 
    return result;

  for (DynDlist<Termino>::Iterator 
         it_izq(const_cast<DynDlist<Termino>&>(terminos)); 
       it_izq.has_current(); it_izq.next())
    result += der.multiplicado_por(it_izq.get_current());

  return result;
}
int main()
{
  Polinomio p1, p2;
  int i;

  for (i = 0; i < 20; i++)
    if (i%2)
      {
        Polinomio p(1, i);
        p1 += p;
      }

  for (i = 0; i < 40; i++)
    {
      Polinomio p(1, i);
      p2 += p;
    }

  printf("\np1 =\n\t"); p1.print();
  printf("\np2 =\n\t"); p2.print();
  p1 += p2;
  printf("\np1 += p2:\n\t"); p1.print();
  Polinomio p3(p1*p2);
  printf("\np1*p2 =\n\t"); p3.print();
}
