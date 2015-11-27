
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


/*
  This file is part of Aleph system

  Copyright (c) 1999, 2000, 2001, 2002, 2003, 2004
  UNIVERSITY LOS ANDES (ULA) Merida - REPÚBLICA BOLIVARIANA DE VENEZUELA  

  - Center of Studies in Microelectronics & Distributed Systems (CEMISID) 
  - ULA Computer Science Department

  PERMISSION TO USE, COPY, MODIFY AND DISTRIBUTE THIS SOFTWARE AND ITS
  DOCUMENTATION IS HEREBY GRANTED, PROVIDED THAT BOTH THE COPYRIGHT
  NOTICE AND THIS PERMISSION NOTICE APPEAR IN ALL COPIES OF THE
  SOFTWARE, DERIVATIVE WORKS OR MODIFIED VERSIONS, AND ANY PORTIONS
  THEREOF, AND THAT BOTH NOTICES APPEAR IN SUPPORTING DOCUMENTATION.

  Aleph is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.

  UNIVERSIDAD DE LOS ANDES requests users of this software to return to 

  Leandro Leon
  CEMISID 
  Ed La Hechicera 
  3er piso, ala Este
  Facultad de Ingenieria 
  Universidad de Los Andes 
  Merida - REPÚBLICA BOLIVARIANA DE VENEZUELA    or

  lrleon@ula.ve

  any improvements or extensions that they make and grant Universidad 
  de Los Andes (ULA) the rights to redistribute these changes. 

  Aleph is (or was) granted by: 
  - Consejo de Desarrollo Cientifico, Humanistico, Tecnico de la ULA 
    (CDCHT)
  - Consejo Nacional de Investigaciones Cientificas y Tecnologicas 
    (CONICIT)
*/

# include <List.H>
# include <Multiset.H> 
# include <iostream>
# include <now.h>

using namespace std;

int main(int argn, char *argc[])
{
  int n = argc[1] ? atoi(argc[1]) : 10000000;
  
  unsigned int t = time(0);
  
  if (argn > 2)
    t = atoi(argc[2]);

  srand(t);

  cout << "#" << argc[0] << " " << n << " " << t << endl;

  double ret_val, menor, mayor, promedio, inserta, elimina;

  unsigned int elementos;

  cout << endl << "n_elem \t menor \t mayor \t promedio \t inserta \t elimina \t mediana" << endl;
  
  for (int n_elem = 100; n_elem < n; n_elem = n_elem*10)
    {
      menor = 1000000000;
      mayor = 0;
      promedio = 0;
      multiset<double> mediana;
      
      
      for (int rep = 0; rep < 11; rep++)
	{ 
	  ret_val = now();

	  list<int> c;

	  inserta = now();

	  for (int i = 0; i < n_elem; i++)
	    {    
	      c.push_front(i);
	      c.push_back(i);
	    } 

	  inserta = now_delta(&inserta);

	  elementos = c.size();

	  elimina = now();
  
	  for (int j = n_elem; j > 0; j--)
	    {    
	      c.erase(c.begin());
	    }

	  elimina = now_delta(&elimina);

	  ret_val = now_delta(&ret_val);

	  if (ret_val < menor)
	    menor = ret_val;

	  if (ret_val > mayor)
	    mayor = ret_val;

	  promedio = promedio + ret_val;

	  mediana.insert(ret_val);
	}

      multiset<double>::iterator media(mediana.begin());

      for(int n = 0; n < 6; n++)
	media++;

cout << elementos << "\t" << menor << "\t" << mayor << "\t" << promedio/11 << "\t" << inserta << "\t" << elimina << "\t" << *media << endl;      }
}
