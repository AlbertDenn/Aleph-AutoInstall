
/*
  This file is part of Aleph-w system

  Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010,
                2011, 2012, 2013, 2014
  Leandro Rabindranath Le�n
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

     Copyright (c) 2002-2014 Leandro Rabindranath Le�n. See details of 
     licence.     

     This product includes software developed by the Hewlett-Packard
     Company, Free Software Foundation and Silicon Graphics Computer
     Systems, Inc. 

  4. Neither the name of the ULA nor the names of its contributors may
     be used to endorse or promote products derived from this software
     without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY Leandro Rabindranath Le�n ''AS IS'' AND ANY
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
  Merida - REP�BLICA BOLIVARIANA DE VENEZUELA    or

  leandro.r.leon@gmail.com

  any improvements or extensions that they make and grant me the rights
  to redistribute these changes.  
*/



# include <iostream>
# include <fstream>


# include <Huffman.H>
# include <huffman_btreepic.H>


using namespace std;


 


char  poema_2 [] = 
"             Las cosas\n"
"\n"
"El bast�n, las monedas, el llavero,\n"
"la d�cil cerradura, las tard�as\n"
"notas que no leer�n los pocos d�as\n" 
"que me quedan, los naipes y el tablero,\n"
"\n"
"un libro y en sus p�ginas la ajada\n"
"violeta, monumento de una tarde\n"
"sin duda inolvidable y ya olvidada,\n"
"el rojo espejo occidental en que arde\n"
"\n"
"una ilusoria aurora. �Cu�ntas cosas,\n"
"l�minas, umbrales, atlas, copas, clavos,\n"
"nos sirven como t�citos esclavos,\n"
"\n"
"ciegas y extra�amente sigilosas!\n"
"Durar�n m�s all� de nuestro olvido;\n" 
"no sabr�n nunca que nos hemos ido.\n"
"\n"
"            Jorge Luis Borges\n";

char  poema_1 [] = 
"Los Nacimientos\n"		
"\n"
"Nunca recordaremos haber muerto.\n"
"\n"
"Tanta paciencia\n"
"para ser tuvimos\n"
"anotando\n"
"los n�meros, los d�as,\n"
"los a�os y los meses,\n"
"los cabellos, las bocas que besamos,\n"
"y aquel minuto de morir\n"
"lo dejaremos sin anotaci�n:\n"
"se lo damos a otros de recuerdo\n"
"o simplemente al agua,\n"
"al agua, al aire, al tiempo.\n"
"Ni de nacer tampoco\n"
"guardamos la memoria,\n"
"aunque importante y fresco fue ir naciendo;\n"
"y ahora no recuerdas un detalle,\n"
"no has guardado ni un ramo\n"
"de la primera luz.\n"
"\n"
"Se sabe que nacemos.\n"
"\n"
"Se sabe que en la sala\n" 
"o en el bosque\n"
"o en el tugurio del barrio pesquero\n"
"o en los ca�averales crepitantes\n"
"hay un silencio enteramente extra�o,\n"
"un minuto solemne de madera\n"
"y una mujer se dispone a parir.\n"
"\n"
"Se sabe que nacimos.\n"
"\n"
"Pero de la profunda sacudida\n"
"de no ser a existir, a tener manos,\n"
"a ver, a tener ojos,\n"
"a comer y llorar y derramarse\n"
"y amar y amar y sufrir y sufrir,\n"
"de aquella transici�n o escalofr�o\n"
"del contenido el�ctrico que asume\n"
"un cuerpo m�s como una copa viva,\n"
"y de aquella mujer deshabitada,\n"
"la madre que all� queda con su sangre\n"
"y su desgarradora plenitud\n"
"y su fin y comienzo, y el desorden\n"
"que turba el pulso, el suelo, las frazadas,\n"
"hasta que todo se recoge y suma\n"
"un nudo m�s el hilo de la vida,\n"
"nada, no qued� nada en tu memoria\n"
"del mar brav�o que elev� una ola\n"
"y derrib� del �rbol una manzana oscura.\n"
"\n"
"No tienes m�s recuerdo que tu vida.\n"
"\n"
"					Pablo Neruda.\n";


char  poema_3 [] = 
"Muerte De Anto�ito El Camborio\n"
"    Federico Garc�a Lorca\n"
"\n"
"   Voces de muerte sonaron\n"
"cerca del Guadalquivir.\n"
"Voces antiguas que cercan\n"
"voz de clavel varonil.\n"
"Les clav� sobre las botas\n"
"mordiscos de jabal�.\n"
"En la lucha daba saltos\n"
"jabonados de delf�n.\n"
"Ba�� con sangre enemiga\n"
"su corbata carmes�,\n"
"pero eran cuatro pu�ales\n"
"y tuvo que sucumbir.\n"  
"Cuando las estrellas clavan\n"
"rejones al agua gris,\n"
"cuando los erales sue�an\n"
"ver�nicas de alhel�,\n"
"voces de muerte sonaron\n"
"cerca del Guadalquivir.\n"
"\n"
"   Antonio Torres Heredia,\n"
"Camborio de dura crin,\n"
"moreno de verde luna,\n"
"voz de clavel varonil:\n"
"�qui�n te ha quitado la vida\n"
"cerca del Guadalquivir?\n"
"Mis cuatro primos Heredias\n"
"hijos de Benamej�.\n"
"Lo que en otros no envidiaban,\n"
"ya lo envidiaban en m�.\n"
"Zapatos color corinto,\n"
"medallones de marfil,\n"
"y este cutis amasado\n"
"con aceituna y jazm�n.\n"
"�Ay Anto�ito el Camborio,\n"
"digno de una Emperatriz!\n"
"Acu�rdate de la Virgen\n"
"porque te vas a morir.\n"
"�Ay Federico Garc�a,\n"
"llama a la Guardia Civil!\n"
"Ya mi talle se ha quebrado\n"
"como ca�a de ma�z.\n"
"\n"
"   Tres golpes de sangre tuvo\n"
"y se muri� de perfil.\n"
"Viva moneda que nunca\n"
"se volver� a repetir.\n"
"Un �ngel marchoso pone\n"
"su cabeza en un coj�n.\n"
"Otros de rubor cansado,\n"
"encendieron un candil.\n"
"Y cuando los cuatro primos\n"
"llegan a Benamej�,\n"
"voces de muerte cesaron\n"
"cerca del Guadalquivir.\n"
  ;


size_t read_and_encode(char *                   str, 
		       Huffman_Encoder_Engine & huffman_engine,
		       BitArray &               bit_stream)
{
  huffman_engine.read_input(str, true);

  const size_t bit_stream_len = huffman_engine.encode(str, bit_stream);

  return bit_stream_len;
}

static int count = 0;

struct Lee
{
  bool operator() (BinNode<string> * p, const char * str) const
  {
    //    if (str == NULL)
    //      return false;

    cout << "** " << ++::count << " (" << p << " " << LLINK(p) << " " 
	 << RLINK(p) << ")" << endl;
    //    if (LLINK(p) == NULL and RLINK(p) == NULL)
      {
	p->get_key() = str;
	cout << "    put " << "\"" << str << "\"" << endl;
	return true;
      }

    return false;
  }
};

//# include "lorca.tree"

int main(int argn, char *argc[])
{
  ofstream huf_output("huffman.Tree", ios::out);
  output_ptr = &huf_output;

  Huffman_Encoder_Engine encoder;

  size_t code_len = 0;

  BitArray code(0);

  if (argn == 1)
    {
      code_len = read_and_encode(poema_3, encoder, code);

      huffman_to_btreepic(encoder.get_freq_root());

      {
	ofstream output("lorca.tree");
	encoder.save_tree_in_array_of_chars("lorca", output);
      }      

//       {
// 	ofstream output("h.tree");
// 	encoder.save_tree(output);
//       }     

//       {
// 	ifstream input("h.tree");
// 	Huffman_Encoder_Engine e;
// 	e.load_tree(input);
// 	assert(areSimilar(encoder.get_root(), e.get_root()));
//       }

//       {
// 	BinNode<string> * root = 
// 	  load_tree_from_array<BinNode<string>, Lee>(lorca_cdp, 215, lorca_k);

// 	assert(areSimilar(encoder.get_root(), root));

// 	destroyRec(root);


//       cout << "|r1| = " << compute_cardinality_rec(encoder.get_root()) << endl
// 	   << "|r2| = " << compute_cardinality_rec(root) << endl;

//       }
      
      Huffman_Decoder_Engine decoder(encoder.get_root(), "");
      //      Huffman_Decoder_Engine decoder(root, "");

      decoder.decode(code, std::cout);

      std::cout << std::endl;

      destroyRec(encoder.get_root());

      destroyRec(encoder.get_freq_root());

      ofstream verso("huffman-verso.tex", ios::out);
      verso << "\\begin{verse}" << std::endl;
  
      char * curr = poema_3;

      while (*curr != '\0')
	{
	  if (*curr == '\n')
	    {
	      if (curr[1] == '\n')
		{
		  verso << " \\\\" << endl
			<< "\\ \\" << endl
			<< endl;
		  curr++;
		}
	      else if (curr[1] == '\0')
		;
	      else
		verso << " \\\\" << endl;
	    }
	  else 
	    verso << *curr; 

	  curr++;
	}
      verso << "\\end{verse}" << std::endl;
  
      ofstream stat1("huffman-stat1.tex", ios::out);
      stat1 << "$" << strlen(poema_3)*8 << "$";

      ofstream stat2("huffman-stat2.tex", ios::out);
      stat2 << "$" << code_len << "$";

      return 0;
    }


  ifstream input1(argc[1], ios::in);

  encoder.read_input(input1, true);
     
  huffman_to_btreepic(encoder.get_freq_root(), true);


  ifstream input2(argc[1], ios::in);

  BitArray bit_stream(1024*1024*500);

  encoder.encode(input2, bit_stream);



}
