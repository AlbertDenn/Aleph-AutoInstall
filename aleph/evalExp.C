
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

# include <ctype.h>
# include <string.h>
# include <tpl_arrayStack.H>

enum Token_Type { Value, Operator, Lpar, Rpar, End, Error };

Token_Type lexer(char *& str, size_t & len)
{
  str += len;
  len = 1;
  while (isblank(*str))
    str++;
  switch (*str)
  {
   case '(':  return Lpar;
   case ')':  return Rpar;
   case '+':
   case '-':
   case '*':
   case '/':  return Operator;
   case '\0': return End;
  }
  if (not isdigit(*str)) 
    return Error;
  char* base = str + 1;
  while (isdigit(*base++))
    len++;
  return Value;
}

char * str_to_token(char * token_str, const size_t & len)
{
  static char buffer[256];
  strncpy(buffer, token_str, len); 
  buffer[len] = '\0';
  return buffer;
}

unsigned precedence(const char & op) // $ < ( < +- < */
{
  switch (op)
    {
    case '$': return 0;
    case '(': return 1;
    case '+':
    case '-': return 2;
    case '/':
    case '*': return 3;

    default: ERROR("Invalid operator %c\n", op);
      return 0;
    }
}

void apply(ArrayStack<int>&  val_stack, ArrayStack<char>& op_stack)
{

  assert(op_stack.size() > 0);
  assert(val_stack.size() >= 2);

  const char the_operator = op_stack.pop(); 
  const int right_operand = val_stack.pop();
  const int left_operand  = val_stack.pop();
  int result;
  switch (the_operator)
    {
    case '+': result = left_operand + right_operand; break;
    case '-': result = left_operand - right_operand; break;
    case '*': result = left_operand * right_operand; break;
    case '/': result = left_operand / right_operand; break;

    default: ERROR("Operator %c invalid", the_operator);

    }
  val_stack.push(result);
}

int eval(char* input)
{
  ArrayStack<int>  val_stack;
  ArrayStack<char> op_stack;
  Token_Type current_token;
  size_t token_len = 0;
  op_stack.push('$');
  while (true)
    {
      current_token = lexer(input, token_len);
      switch (current_token)
        {
        case Value:
          {
            const int operand = atoi(str_to_token(input, token_len));
            val_stack.push(operand);
            break;
          }
        case Lpar: 
          {
            op_stack.push(*input); // introducir parentesis en op_stack
            break;
          }
        case Operator: 
          {
            while (precedence(op_stack.top()) >= precedence(*input))
              apply(val_stack, op_stack);
            op_stack.push(*input); // introducir operador en op_stack
            break;
          }
        case Rpar: 
          {
            while (op_stack.top() != '(')
              apply(val_stack, op_stack);
            op_stack.pop(); /* saca el parentesis izquierdo */
            break;
          }
        case End:
          {
            while (op_stack.top() != '$')
              apply(val_stack, op_stack);
            op_stack.pop(); // debe ser '$'
            const int ret_val = val_stack.pop();

            if ((val_stack.size() != 0) or (op_stack.size() != 0))
              ERROR("Bad expression");

            return ret_val;
          }

        case  Error:
        default: ERROR("Bad token detected");

        } 
    }
}

int main(int, char** argc)
{
  printf("\t%d\n\n", eval(argc[1]));
}
