
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

// this file implements the routines required by gmpfrxx.h
// it is modeled on mpfr-2.2.1/out_str.c
//                   gmp-4.2.1/mpz/set_f.c
//                   gmp-4.2.1/mpq/set_f.c

// (written by Jon Wilkening on Aug 8, 2007)

# include <stddef.h> // workaround of gcc 3.9 until libraries get updated 
#include "gmpfrxx.h"
using namespace std;


// MpFrC is a dummy class holding the static members of mpfr_class
mpfr_rnd_t  MpFrC::rnd = GMP_RNDN;
int         MpFrC::base = 10;


istream & operator>>(istream &s, mpfr_ptr a) {
    string tmp;
    s >> tmp;
    mpfr_set_str(a, tmp.c_str(),
		 mpfr_class::get_base(),
		 mpfr_class::get_rnd());
    // a = tmp.c_str();
    return s;
}

// modeled on mpfr-2.2.1/out_str.c
ostream & operator<<(ostream &os, mpfr_srcptr a)
{
    char *s, *t, *s0;
    mp_exp_t  e;

    // for debugging:
    // mpfr_out_str(stdout, 10, 0, a, RND); printf("\n");

    if (mpfr_nan_p(a)) {
	os << "@NaN@";
	return os;
    }

    if (mpfr_inf_p(a)) {
	if (MPFR_SIGN(a) > 0)
	    os << "@Inf@";
	else
	    os << "-@Inf@";
	return os;
    }

    if (mpfr_zero_p(a)) {
	if (MPFR_SIGN(a) > 0)
	    os << "0";
	else
	    os << "-0";
	return os;
    }

    s = mpfr_get_str (NULL, &e,
		      mpfr_class::get_base(),
		      0, a,
		      mpfr_class::get_rnd());

    t = mpfr_get_str (NULL, &e,
		      mpfr_class::get_base(),
		      os.precision(), a,
		      mpfr_class::get_rnd());

    if (strlen(s)<strlen(t))
	mpfr_free_str(t);
    else {
	mpfr_free_str(s);
	s = t;
    }

    s0 = s;
    /* for a=3.1416 we have s = "31416" and e = 1 */

    if (*s == '-')
	os.put(*s++);

    /* outputs mantissa */
    os.put(*s++); e--; /* leading digit */
    os.put('.');
    while (*s != '\0')
	os.put(*s++);       /* rest of mantissa */

    mpfr_free_str(s0);

    /* outputs exponent */
    if (e) {
	os << (mpfr_class::get_base() <= 10 ? 'e' : '@') << (long) e;
    }

    return os;
}

// fixme: it would be more efficient to work directly
// with the limbs, but I don't want to deal with the
// internal representations of both mpfr and gmp
void mpz_set_mpfr(mpz_ptr w, mpfr_srcptr u)
{
    char *s, *t;
    long k, sz;
    mp_exp_t  e;
  
    // abs(u)<1 truncates to zero
    if ( mpfr_get_exp(u)<=0 || mpfr_zero_p(u)
	 || mpfr_nan_p(u) || mpfr_inf_p(u) ) {
	mpz_set_ui(w, 0);
	return;
    }

    // note: this is done in hex to represent u exactly
    // example: for u=3.1416 we have s = "31416" and e = 1
    s = mpfr_get_str (NULL, &e, 16, 0, u, GMP_RNDN);
    sz = strlen(s);

    if (*s == '-')
	e++;

    if (e<=sz) {
	s[e] = '\0'; // truncate
	mpz_set_str(w, s, 16);
	mpfr_free_str(s);
	return;
    }

    t = (char *) calloc(e+1, sizeof(char));
    
    for (k=0; k<sz; k++)
	t[k] = s[k];
    for (k=sz; k<e; k++)
	t[k] = '0';
    t[e] = '\0';

    mpz_set_str(w, t, 16);
    free(t);
    mpfr_free_str(s);
}


void mpq_set_mpfr(mpq_ptr w, mpfr_srcptr u)
{
    char *s, *t;
    long k, sz;
    mp_exp_t  e, bits;
    
    if ( mpfr_zero_p(u) || mpfr_nan_p(u) || mpfr_inf_p(u) ) {
	mpq_set_ui(w, 0, 1);
	return;
    }

    // note: this is done in binary to represent u exactly
    // example:   u=10.1001  -->  s = "101001", e = 2
    s = mpfr_get_str (NULL, &e, 2, 0, u, GMP_RNDN);
    sz = strlen(s);

    t = s+sz;
    while (*(--t) == '0')
	*t = '\0'; // trim trailing zeros

    sz = strlen(s);
    bits = (*s == '-') ? sz-1 : sz; // bits>0 since u!=0

    if (e>=bits) {
	e = sz+e-bits;
	// no denominator needed
	t = (char *) malloc((e+1)*sizeof(char));
	strcpy(t,s);
	for (k=sz; k<e; k++)
	    t[k] = '0';
	t[e] = '\0';
	mpq_set_str(w, t, 2);
	free(t);
	mpfr_free_str(s);
	return;
    }

    e = sz+bits-e+2;
    t = (char *) malloc((e+1)*sizeof(char));
    strcpy(t,s);
    t[sz++] = '/';
    t[sz++] = '1';
    for (k=sz; k<e; k++)
	t[k] = '0';
    t[e] = '\0';
    mpq_set_str(w, t, 2);
    free(t);
    mpfr_free_str(s);
}
