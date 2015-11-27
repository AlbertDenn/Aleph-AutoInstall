
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

# 1 "ran_array.c"
# 1 "<interno>"
# 1 "<l\355nea de orden>"
# 1 "ran_array.c"
# 21 "ran_array.c"
# 1 "ran_array.h" 1
# 36 "ran_array.h"
extern void ran_array(long aa[],int n);
# 47 "ran_array.h"
extern void ran_start(long seed);





extern long *ran_arr_ptr;

extern long ran_arr_cycle();
# 22 "ran_array.c" 2


long ran_x[100];


void ran_array(long aa[],int n)





{
  register int i,j;
  for (j=0;j<100;j++) aa[j]=ran_x[j];
  for (;j<n;j++) aa[j]=(((aa[j-100])-(aa[j-37]))&((1L<<30)-1));
  for (i=0;i<37;i++,j++) ran_x[i]=(((aa[j-100])-(aa[j-37]))&((1L<<30)-1));
  for (;i<100;i++,j++) ran_x[i]=(((aa[j-100])-(ran_x[i-37]))&((1L<<30)-1));
}





static long ran_arr_buf[1009];
static long ran_arr_dummy=-1, ran_arr_started=-1;
long *ran_arr_ptr=&ran_arr_dummy;





void ran_start(long seed)




{
  register int t,j;
  long x[100 +100 -1];
  register long ss=(seed+2)&((1L<<30)-2);
  for (j=0;j<100;j++) {
    x[j]=ss;
    ss<<=1; if (ss>=(1L<<30)) ss-=(1L<<30)-2;
  }
  x[1]++;
  for (ss=seed&((1L<<30)-1),t=70 -1; t; ) {
    for (j=100 -1;j>0;j--) x[j+j]=x[j], x[j+j-1]=0;
    for (j=100 +100 -2;j>=100;j--)
      x[j-(100 -37)]=(((x[j-(100 -37)])-(x[j]))&((1L<<30)-1)),
      x[j-100]=(((x[j-100])-(x[j]))&((1L<<30)-1));
    if (((ss)&1)) {
      for (j=100;j>0;j--) x[j]=x[j-1];
      x[0]=x[100];
      x[37]=(((x[37])-(x[100]))&((1L<<30)-1));
    }
    if (ss) ss>>=1; else t--;
  }
  for (j=0;j<37;j++) ran_x[j+100 -37]=x[j];
  for (;j<100;j++) ran_x[j-37]=x[j];
  for (j=0;j<10;j++) ran_array(x,100 +100 -1);
  ran_arr_ptr=&ran_arr_started;
}


long ran_arr_cycle()
{
  if (ran_arr_ptr==&ran_arr_dummy)
    ran_start(314159L);
  ran_array(ran_arr_buf,1009);
  ran_arr_buf[100]=-1;
  ran_arr_ptr=ran_arr_buf+1;
  return ran_arr_buf[0];
}
