
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
# include <fstream>
# include <tpl_binNodeUtils.H>
# include <Huffman.H>
# include <huffman_btreepic.H>

using namespace std;

char  poema1 [] = 
"             Las cosas\n"
"\n"
"El bastón, las monedas, el llavero,\n"
"la dócil cerradura, las tardías\n"
"notas que no leerán los pocos días\n" 
"que me quedan, los naipes y el tablero,\n"
"\n"
"un libro y en sus páginas la ajada\n"
"violeta, monumento de una tarde\n"
"sin duda inolvidable y ya olvidada,\n"
"el rojo espejo occidental en que arde\n"
"\n"
"una ilusoria aurora. ¡Cuántas cosas,\n"
"láminas, umbrales, atlas, copas, clavos,\n"
"nos sirven como tácitos esclavos,\n"
"\n"
"ciegas y extrañamente sigilosas!\n"
"Durarán más allá de nuestro olvido;\n" 
"no sabrán nunca que nos hemos ido.\n"
"\n"
"            Jorge Luis Borges\n";


char  poema [] = 
"El enamorado\n"
"\n"
"Lunas, marfiles, instrumentos, rosas,\n"
"lámparas y la línea de Durero,\n"
"las nueve cifras y el cambiante cero,\n"
"debo fingir que existen esas cosas.\n"
"\n"
"Debo fingir que en el pasado fueron\n"
"Persépolis y Roma y que una arena\n"
"sutil midió la suerte de la almena\n"
"que los siglos de hierro deshicieron.\n" 
"\n"
"Debo fingir las armas y la pira\n"
"de la epopeya y los pesados mares\n"
"que roen de la tierra los pilares.\n"
"\n"
"Debo fingir que hay otros. Es mentira.\n"
"Sólo tú eres. Tú, mi desventura\n"
"y mi ventura, inagotable y pura.\n"
"\n"
"            Jorge Luis Borges\n";


const size_t read_and_encode(char *                   str, 
			     Huffman_Encoder_Engine & huffman_engine,
			     BitArray &               bit_stream)
{
  huffman_engine.read_input(str, true);

  const size_t bit_stream_len = huffman_engine.encode(str, bit_stream);

  return bit_stream_len;
}


void print_node(BinNode<string> * p, int, int)
{
  cout << p->get_key() << " ";
}

void print_code(BitArray & cod, const size_t & len)
{
  for (int i = 0; i < len; ++i)
    cout << cod[i] << " ";
  cout << endl << endl;
}

int main()
{
  ofstream output("borges.Tree", ios::out);
  output_ptr = &output;

  Huffman_Encoder_Engine encoder;

  size_t code_len = 2048;

  BitArray code(code_len);

  code_len = read_and_encode(poema, encoder, code);

  print_code(code, code_len);

  Huffman_Decoder_Engine decoder(encoder.get_root(), "");

  huffman_to_btreepic(encoder.get_freq_root(), true);

  decoder.decode(code, std::cout);

  std::cout << std::endl;

  destroyRec(encoder.get_root());
}
