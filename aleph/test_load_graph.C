
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

# include <argp.h>
# include <iostream>
# include "load_digraph.H"

# define TERMINATE(n) (exit(n))

const char *argp_program_version = 
  "cendipei 0.1b\n"
  "CENDITEL PEI Grafo\n"
  "Copyleft (C) 2009 CENDITEL\n"
  "REPÚBLICA BOLIVARIANA DE VENEZUELA\n"
  "This is free software; There is NO warranty; not even for MERCHANTABILITY\n"
  "or FITNESS FOR A PARTICULAR PURPOSE\n"
  "\n";

const char *argp_program_bug_address = "lleon@cenditel.gob.ve";

static char doc[] = "cendipei -- generador de grafos del PEI CENDITEL";

static char argDoc[] = "- input-file\n";

static const char license_text [] = 
  "Generador de grafos del PEI CENDITEL. License & Copyright Note\n"
  "Copyleft (C) 2009\n"
  "CENDITEL\n"
  "\n"
  "This is free software; There is NO warranty; not even for MERCHANTABILITY\n"
  "or FITNESS FOR A PARTICULAR PURPOSE\n"
  "\n"
  "  PERMISSION TO USE, COPY, MODIFY AND DISTRIBUTE THIS SOFTWARE AND ITS \n"
  "  DOCUMENTATION IS HEREBY GRANTED, PROVIDED THAT BOTH THE COPYRIGHT \n"
  "  NOTICE AND THIS PERMISSION NOTICE APPEAR IN ALL COPIES OF THE \n"
  "  SOFTWARE, DERIVATIVE WORKS OR MODIFIED VERSIONS, AND ANY PORTIONS \n"
  "  THEREOF, AND THAT BOTH NOTICES APPEAR IN SUPPORTING DOCUMENTATION. \n"
  "\n"
  "  This program is distributed in the hope that it will be useful,\n"
  "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
  "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. \n"
  "\n";



static struct argp_option options [] = {
  {"acciones", 'a', "archivo-acciones", 0, "archivo de códigos de acciones", 0},
  {"grafo", 'g', "archivo-grafo", 0, "archivo de grafo de acciones", 0},
  {"dot", 'd', "dot-file", OPTION_ARG_OPTIONAL, "archivo de dot de salida", 0},
  {"numeros", 'n', 0, OPTION_ARG_OPTIONAL, "Sólo imprime números", 0},
  {"necesidades", 'N', 0, OPTION_ARG_OPTIONAL, "Imprime % necesidades", 0},
  {"potencia", 'p', 0, OPTION_ARG_OPTIONAL, "Escribe potencia de acción", 0},
  {"fontsize", 'f', "fontsize", 0, "tamaño de letra (aprox 6-20)", 0},
  {"license", 'C', 0, OPTION_ARG_OPTIONAL, "print license", 0},
  {"version", 'V', 0, OPTION_ARG_OPTIONAL, "Print version information", 0},
  {"verbose", 'v', 0, OPTION_ARG_OPTIONAL, "verbose mode", 0},
  { 0, 0, 0, 0, 0, 0 } 
}; 

static const char * hello = 
  "// Generador de grafos del PEI CENDITEL."
  "// Copyright (C) 2009  CENDITEL\n"
  "// Merida - REPÚBLICA BOLIVARIANA DE VENEZUELA\n"
  "// This is free software; There is NO warranty; not even for \n"
  "// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE\n"
  "\n";


string cod_file_name;
string gra_file_name;
string dot_file_name;

static error_t parser_opt(int key, char *arg, struct argp_state *)
{
  switch (key)
    {
    case 'v': /* verbose mode (no implementado) */ 
      break;

    case 'g':
      {
	if (arg == NULL)
	  ERROR("Esperando por archivo grafo de acciones");
	
	gra_file_name  = arg;
	break;
      }

    case 'a':
      if (arg == NULL)
	ERROR("Esperando por archivo de acciones");

      cod_file_name  = arg;

      break;

    case 'n':
      only_num = true;
      break;

    case 'N':
      with_nes = true;
      break;

    case 'p':
      with_power = true;
      break;

    case 'd': /* archivo de salida */
      if (arg == NULL)
	ERROR("Esperando por archivo de salida");

      dot_file_name = arg;

      break;

    case 'f':
      
      if (arg == NULL)
	ERROR("Esperando por tamaño de letra");

      font_size = atoi(arg);

      break;

    case 'C': /* imprime licencia */ 
      cout << license_text;
      TERMINATE(0);

      break;

    case 'V': /* imprimir version */ 
      cout << argp_program_version;
      TERMINATE(0);

      break;
      
    default: return ARGP_ERR_UNKNOWN;
    }

  return NULL;
}


static struct argp arg_defs = { options, parser_opt, argDoc, doc, 0, 0, 0 }; 




int main(int argc, char *argv[]) 
{
  argp_parse(&arg_defs, argc, argv, ARGP_IN_ORDER, 0, NULL);
  
  if (gra_file_name.size() == 0)
    ERROR("Archivo grafo no especificado");

  ifstream grafo_stream(gra_file_name.c_str(),  ios::in);
  if (not grafo_stream)
    ERROR("%s archivo grafo no existe", gra_file_name.c_str());

  if (cod_file_name.size() == 0)
    ERROR("Archivo de acciones no especificado");
  ifstream acciones_stream(cod_file_name.c_str(), ios::in);
  if (not acciones_stream)
    ERROR("%s archivo de acciones no existe", argv[1]);

  cout << hello;

  Digrafo g;

  load_digraph(g, acciones_stream, grafo_stream);

  if (dot_file_name.size() > 0)
    {
      ofstream dot_stream(dot_file_name.c_str(), ios::out);
      generate_dot_file(g, dot_stream);
    }
  else
    generate_dot_file(g, cout);
}

