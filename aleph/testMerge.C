
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


# include <stdio.h>
# include <tpl_sort_utils.H>


# define NumItems 200

typedef Dnode<unsigned> List;

void printList(List & list)
{
  printf("\n");

  List::Iterator itor(list);
  Dnode<unsigned>* node;

  for (itor.reset_first(); itor.has_current(); itor.next())
    {
      node = itor.get_current();
      printf("%u ", node->get_data());
    }

  printf("\n");
}      


struct Menor_Que
{
  bool operator() (Dlink * l1, Dlink * l2) const
  {
    return static_cast<Dnode<unsigned>*>(l1)->get_data() <
      static_cast<Dnode<unsigned>*>(l2)->get_data();
  }
};

int main()
{
  printf("Starting ........\n\n");

  List list1;
  List list2;

  Dnode<unsigned>* node;

  unsigned long i;

  for (i = 0; i < NumItems; i++)
    {
      node = new Dnode<unsigned>;
      node->get_data() = i;
      list1.insert(node); 
      printf("%lu ", i);
    }

  printf("\n");

  for (i = 0; i < NumItems; i++)
    {
      node = new Dnode<unsigned>;
      node->get_data() = i;
      list2.append(node); 
      printf("%lu ", i);
    }

  List list3;

  merge_lists<List, Menor_Que>(list1, list2, list3);

  printList(list1); 
  printList(list2); 

  quicksort(list1);

  printf("Lists apparently merged and sorted\n");

  printList(list1); 

  list1.remove_all_and_delete();

  printf("Ending .... \n\n");
}
      

