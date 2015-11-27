
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

#include <fstream>

#include <math.h>

#include <stdlib.h>



void printResults(char *file, char *entry, double average, double max,

                  double min, double desv, unsigned int counter,int param)

{

  ofstream out(file);



  out<<entry<<endl;



  if ( param > 0 )

    out << "Estadísticas para Log base 2 de N=" << param

        << endl 

        << endl ;



  out << "\tTiempo promedio: "     << average  << endl;



  out << "\tTiempo máximo:     "   << max << endl;



  out << "\tTiempo mínimo: "       << min << endl;



  out << "\tDesviación Estándar: " << desv 

      << endl

      << endl;



  out << "Número de valores tomados: " << counter << endl;



  out.close();

}





int main(int argc, char* argv[])

{



  int param = 0;



  if (argc < 3 ) 

    {

      cout << "syntax: < file1 > <file2> [ param ]  " << endl;

      exit(0);

    }



  if ( argc > 3 )

    param = atoi(argv[3]);

    



  ifstream in(argv[1]);

   

  char ite[51], t[51], buffer[100];

  unsigned int counter=0, nodes;

  double desv, tdiff, average=0, max, min, sum=0;

   



  in.getline(buffer, sizeof(buffer)); // get first line



  while(!in.eof())

    {

      in >> ite;



      in >> ite;

      nodes = atoi(ite);



      in >> t;

      in >> ite;



      tdiff = atof(t);



      if ( (param == 0 ) ? true : ( ( (int)(log(nodes)/log(2.0)) ) == param ) )

	{

	  average = average + tdiff;

	  sum = sum + pow(tdiff, 2);

	  counter++;



	  if (counter==1)

	    {

	      max = tdiff;

	      min = tdiff;

	    }

	  else

	    {

	      if ( tdiff>max ) max = tdiff;

	      if ( tdiff<min ) min = tdiff;

	    }

	}

    }



  in.close();





  if ( counter>0 )	

    {

      average = average / counter;

      desv = sqrt( (sum-( counter *  pow(average, 2) ) ) /counter);

		   

      printResults(argv[2], buffer, average, max, min, desv, counter, param);



    }



  return 0;

}













