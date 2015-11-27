
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
# include <aleph.H>
# include <tpl_dynSlist.H>

# define Num_Items 10

using namespace std;

int main()
{
  DynSlist<int> list;
  DynSlist<int*> list_ptr;

  for (int i = 0; i < Num_Items; i++)
    {
      list.insert(i, i);
      list_ptr.insert(i, &i);
    }

  DynSlist<int>::Iterator itor(list);

  DynSlist<int>::Iterator itor_aux(list);

  itor_aux = itor;

  for (/* nothing */; itor.has_current(); itor.next())
    cout << itor.get_current() << " ";

  cout << endl;

  for (int i = Num_Items; i < 2*Num_Items; i++)
    list.insert(i - Num_Items, i);

  for (itor.reset_first(); itor.has_current(); itor.next())
    cout << itor.get_current() << " ";

  cout << endl;

  for (int i = 2*Num_Items; i < 3*Num_Items; i++)
    list.insert(0, i);

  for (itor.reset_first(); itor.has_current(); itor.next())
    cout << itor.get_current() << " ";

  cout << endl;

  for (int i = 3*Num_Items; i < 4*Num_Items; i++)
    list.insert(list.size(), i);

  for (int i = 0; i < list.size(); i++)
    cout << list[i] << " ";

  cout << endl;

  for (int i = 0; i < list.size(); i++)
    cout << list[list.size() - i - 1] << " ";

  cout << endl;

  list.remove(list.size() - 1);

  list.remove(0);

  for (itor.reset_first(); itor.has_current(); itor.next())
    cout << itor.get_current() << " ";

  cout << endl;

}
