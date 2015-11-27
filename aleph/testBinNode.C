
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


# include <iostream>
# include <tpl_binNode.H>
# include <tpl_binNodeUtils.H>

using namespace Aleph;

void printInt(BinNode<int>* p, int, int)
{
  cout << p->get_key() << " ";
}

BinNode<int> nodes[15];

int main()
{
  int i, n;
  n = 15;

  for (i = 0; i < n; i++)
    nodes[i].get_key() = i;

  nodes[0].getL() = &nodes[1];
  nodes[0].getR() = &nodes[2];
  nodes[1].getL() = &nodes[3];
  nodes[1].getR() = &nodes[4];
  nodes[2].getL() = &nodes[5];
  nodes[2].getR() = &nodes[6];
  nodes[3].getL() = &nodes[7];
  nodes[3].getR() = &nodes[8];
  nodes[4].getL() = &nodes[9];
  nodes[4].getR() = &nodes[10];
  nodes[5].getL() = &nodes[11];
  nodes[5].getR() = &nodes[12];
  nodes[6].getL() = &nodes[13];
  nodes[6].getR() = &nodes[14];

  BinNode<int>* p = copyRec(&nodes[0]);

  printf("\n");

  destroyRec(p);

  preOrderRec(&nodes[0], &printInt);
  printf("\n");

  inOrderRec(&nodes[0], &printInt);
  printf("\n");

  postOrderRec(&nodes[0], &printInt);
  printf("\n");
}
	  
