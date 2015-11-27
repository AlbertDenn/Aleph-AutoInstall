
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
# include <pointer_table.H>

using namespace std;

# define NUM_PTR 100000

void * random_pointer() 
{
  long ret_val = rand();

  while (ret_val == 0)
    ret_val = rand();

  return reinterpret_cast<void*>(ret_val);
}

long random_index()
{
  return (long) (NUM_PTR*1.0*rand()/(RAND_MAX+1.0));
}


int main(int argn, char * argc[])
{
  if (argn > 1)
    srand(atoi(argc[1]));
  else
    srand(time(0));

  try
    {
      DynArray<void*> pointers;
      Pointer_Table table;
      long r;
      int i;
      int dups = 0;
      int outs = 0;
      
      for (i = 0; i < NUM_PTR; i++)
	{
	  pointers[i] = random_pointer();
	  table.insert_pointer(pointers[i]);
	}

      for (i = 0; i < NUM_PTR; i++)
	{
	  r = random_index();
	  
	  assert(table.verify_pointer(r, pointers[r]) == pointers[r]);
	}

      for (i = 0; i < NUM_PTR; i++)
	{
	  r = random_index();

	  try
	    {
	      table.remove_pointer(r);
	      pointers[r] = NULL;
	    }
	  catch (domain_error)
	    { 
	      dups++;
	      assert(pointers[r] == NULL);
	    }
	  catch (range_error)
	    {
	      outs++;
	    }
	}

      cout << endl
	   << "Primera parte del test" << endl
	   << "  Se realizaron " << NUM_PTR << " inserciones" << endl
	   << "  Se eliminaron " << NUM_PTR << " indices aleatoriamente" 
	   << endl
	   << "    de los cuales " << dups << " índices fueron duplicados" 
	   << endl
	   << "    y " << outs << " estuvieron fuera de rango" << endl
	   << "  Size   = " << table.size() << endl
	   << "  Busies = " << table.busies() << endl
	   << "  Frees  = " << table.frees() << endl
	   << endl;

      int ins_counter = 0;
      for (i = 0; i < NUM_PTR; i++)
	{
	  if (pointers[i] == NULL)
	    {
	      pointers[i] = random_pointer();
	      table.insert_pointer(pointers[i]);
	      ins_counter++;
	    }
	}

      dups = 0;
      outs = 0;
      int del_counter = 0;
      for (i = 0; i < NUM_PTR; i++)
	{
	  r = random_index();


	  try
	    {
	      table.remove_pointer(r);
	      del_counter++;
	      pointers[r] = NULL;
	    }
	  catch (domain_error)
	    { 
	      dups++;
	      assert(pointers[r] == NULL);
	    }
	  catch (range_error)
	    {
	      outs++;
	    }
	}

      cout << endl
	   << "Segunda parte del test" << endl
	   << "  Se realizaron " << ins_counter << " inserciones" << endl
	   << "  Se intentaron " << NUM_PTR << " indices aleatoriamente" 
	   << endl
	   << "    de los cuales " << dups << " índices fueron duplicados" 
	   << endl
	   << "    " << outs << " estuvieron fuera de rango" << endl
	   << "    y " << del_counter << " fueron eliminados " << endl
	   << "  Size   = " << table.size() << endl
	   << "  Busies = " << table.busies() << endl
	   << "  Frees  = " << table.frees() << endl
	   << endl;

      cout << "Probando acceso fuera de rango ..." << endl;

      for (i = NUM_PTR - 1; i >= 0; i--)
	{
	  try
	    {
	      table.verify_pointer(i, pointers[i]);
	      break;
	    }
	  catch (range_error) {}
	  catch (domain_error) {}
	}

      cout << "Heap detectado en " << i + 1 << endl;
    }
  catch (exception & e)
    {
      cout << "Excepción inesperada: " << e.what() << endl;
    }
}
