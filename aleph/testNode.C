
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
# include <tpl_binNodeAux.H>

class Altura 
	{
	private:
	  size_t altura;
	public:
	  Altura() {}
	  Altura(size_t a) : altura(a) { /* empty */ }
	  size_t dar_altura() const { return altura; }
	};


using namespace std;

DECLARE_BINNODE(Nodo, 255, Altura);
SET_BINNODE_NULL_POINTER(NULL, Nodo);

using namespace std;

int main()
{

  {
    BinNode<int> node1;
    BinNode<int> node2(5);

    BinNode<int>::NullPtr = &node1;

    cout << "sizeof(BinNode<int>) = " << sizeof(BinNode<int>) << endl;

    cout << "BinNode<int>::MaxHeight = " << BinNode<int>::MaxHeight  
	 << endl;
  }

  {
    BinNodeVtl<int> node1;
    BinNodeVtl<int> node2(5);

    BinNodeVtl<int>::NullPtr = &node1;

    cout << "sizeof(BinNodeVtl<int>) = " << 
      sizeof(BinNodeVtl<int>) << endl;
    cout << "BinNodeVtl<int>::MaxHeight = " 
	 << BinNodeVtl<int>::MaxHeight << endl;
  }

  {
    Nodo<int> node1;
    Nodo<int> node2(5);
    Nodo<int> node3(Altura(10));

    Nodo<int>::NullPtr = &node1;

    cout << "sizeof(Nodo<int>) = " << sizeof(Nodo<int>) << endl;
    cout << "Nodo<int>::MaxHeight = " << Nodo<int>::MaxHeight << endl;
  }

}
  
