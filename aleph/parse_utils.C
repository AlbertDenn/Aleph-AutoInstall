
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

# include "parse_utils.H"

const size_t Buffer_Size = 512;

void put_char_in_buffer(char *& start_addr, char * end_addr, int c)
{
  if (start_addr >= end_addr)
    ERROR("String too long");

  *start_addr++ = c;  
}


string command_line_to_string(int argc, char *argv[])
{
  string command_line;

  for (int i = 0; i < argc; i++)
    {
      command_line += " ";
      command_line += argv[i];
    }

  return command_line;
}


int current_line_number = 1;
int current_col_number  = 1;

int previous_line_number = 1;
int previous_col_number  = 1;



string token_instance;

void init_token_scanning()
{
  previous_line_number = current_line_number;
  previous_col_number = current_col_number;
}


void close_token_scanning(char * buffer, char *& start_addr, char * end_addr)
{
  put_char_in_buffer(start_addr, end_addr, '\0');
  token_instance = buffer;
}



int read_char_from_stream(ifstream& input_stream) throw(out_of_range)
{
  if (input_stream.eof())
    throw out_of_range("end of file has been reached");

  int c = input_stream.get();

  if (c == '\n')
    {
      current_line_number++;
      current_col_number = 0;
    }
  else
    current_col_number++;	

  return c;
}


// string read_line(ifstream& input_stream) throw(out_of_range)
// {
//   if (input_stream.eof())
//     throw out_of_range("end of file has been reached");

//   DynArray<char> line;

//   for (int c = input_stream.get(); c != '\n'; c = input_stream.get())
//     line[i] = c;
  
//   string ret_val
// }


void skip_white_spaces(ifstream& input_stream)
{
  while (isspace(read_char_from_stream(input_stream)));
  input_stream.unget();
}



long load_number(ifstream& input_stream)
{
  char buffer[Buffer_Size];
  char * start_addr = buffer;
  char * end_addr   = buffer + Buffer_Size;

  init_token_scanning();

  try 
    { 
      skip_white_spaces(input_stream);
      int c = read_char_from_stream(input_stream);

      if (c == '-')
	{
	  put_char_in_buffer(start_addr, end_addr, c);
	  c = read_char_from_stream(input_stream);

	  if (not isdigit(c))
	    return atoi(buffer);
	}
      
      while (true)
	{
	  while (isdigit(c))
	    {
	      put_char_in_buffer(start_addr, end_addr, c);
	      c = read_char_from_stream(input_stream); 
	    }

	  if (isspace(c))
	    { 
	      close_token_scanning(buffer, start_addr, end_addr);
	      return atoi(buffer); 
	    }

	  throw domain_error("Invalid number");
	}
    }
  catch (out_of_range) // Se alcanza fin de archivo
    { 
      close_token_scanning(buffer, start_addr, end_addr);
      return atoi(buffer); 
    }
}


string load_string(ifstream& input_stream)
{
  char buffer[Buffer_Size];

  char * start_addr = buffer;
  char * end_addr   = buffer + Buffer_Size;

  bool quotations_mark = false;

  init_token_scanning();
  skip_white_spaces(input_stream);

  int c = read_char_from_stream(input_stream);
  if (c == '\"')
    quotations_mark = true;
  else
    put_char_in_buffer(start_addr, end_addr, c);

  while (true)
    {
      c = read_char_from_stream(input_stream);

      if (c == '\"' and quotations_mark)
	break;

      if (not quotations_mark and 
	  (isblank(c) or c == '\n' or c == EOF))
	break;

      put_char_in_buffer(start_addr, end_addr, c);
    }
  close_token_scanning(buffer,start_addr, end_addr);

  return string(buffer);
}



void print_parse_error_and_exit(const string& str)
{
  cout << "Parse error " 
       << previous_line_number << ":" << previous_col_number <<" " 
       << str << endl
       << "Last token seen is: " << token_instance << endl;
  exit(1);
}


void print_parse_warning(const string& str)
{
  cout << "Parse warning " 
       << previous_line_number << ":" << previous_col_number <<" " 
       << str << endl
       << "Last token seen is: " << token_instance << endl;
}
