
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
# include <tpl_binTree.H>
# include <tpl_avl.H>
# include <tpl_treap.H>
# include <tpl_splay_tree.H>
# include <tpl_rb_tree.H>
# include <tpl_rand_tree.H>
# include <tpl_dynMapTree.H>
# include <ran_array.h>
# include <time.h>
# include <gsl/gsl_rng.h>
# include <argp.h>
# include <string.h>

using namespace std;

    template <class Node>
static void printNode(Node* node, int, int)
{
  cout << "(" << node->get_key() << "," << node->get_data() << ")";
}


    template <
      template <typename /* key */, class /* Compare */>
    class TreeType>
void test(unsigned long n, gsl_rng * r)
{
  unsigned long max = 100*n;
  int i, value;
  DynMapTree<int, int, TreeType> tree;

  for (i = 0; i < n; i++)
    {
      value = gsl_rng_uniform_int(r, max);
      tree.insert(value, i);
    }

  cout << "Reading test ... " << endl;

  int content;

  for (i = 0; i < n; i++)
    {
      value = gsl_rng_uniform_int(r, max);
      try
	{
	  content = tree.find(value);
	  //	  cout << "(" << value << "," << content << ")";
	}
      catch (...)
	{
	  ;//cout << i << ".";  
	}
    }
  
  cout << endl;

  cout << "Writing test ... " << endl;

  for (i = 0; i < n; i++)
    {
      value = gsl_rng_uniform_int(r, max);
      tree.insert(value, i);
      //      cout << "(" << value << "," << i << ")";
    }
  
  cout << endl;

  cout << "Test test ... " << endl;

  for (i = 0; i < n; i++)
    {
      value = gsl_rng_uniform_int(r, max);
      tree.search(value);
    }
  
  cout << endl;

  cout << "The path length is " << tree.internal_path_length() << endl;

  cout << "The height is " << tree.height() << endl;

  unsigned int insCount = tree.size();
  cout << insCount << " Items inserted" << endl;

  for (i = 0; i < n; i++)
    {
      value = gsl_rng_uniform_int(r, max);
      tree.remove(value);
    }

  cout << insCount - tree.size() << " Items removed" << endl;
}

enum TreeType 
{ 
  INVALID, BIN, AVL, SPLAY, TREAP, RB, RAND 
};

struct Parameters
{
  int      n;
  int      seed;
  TreeType type;
  char     *str;

  Parameters(int _n, int _seed)
    : n(_n), seed(_seed), type(INVALID), str(NULL)
    {
      // Empty 
    }    
};
  

const char *argp_program_version = "testAllTree 0.0";
const char *argp_program_bug_address = "aleph-bugs@aleph.ula.ve";

static char doc[] = "testAllTree -- A tester for all binary trees";
static char argDoc[] = "-n num_nodes -m seed_for_random -<tree type>\n"
;

static struct argp_option options [] = {
  { "bin",  'b', 0, OPTION_ARG_OPTIONAL, "pure binary tree" , 0},
  { "avl",  'a', 0, OPTION_ARG_OPTIONAL, "avl tree" , 0},
  { "splay",  's', 0, OPTION_ARG_OPTIONAL, "splay tree" , 0},
  { "redblack",  'r', 0, OPTION_ARG_OPTIONAL, "red black tree" , 0},
  { "rand",  'd', 0, OPTION_ARG_OPTIONAL, "randomized tree" , 0},
  { "treap",  'p', 0, OPTION_ARG_OPTIONAL, "treap tree" , 0},
  { "nodes", 'n', "num_nodes", OPTION_ARG_OPTIONAL, 
    "Specify the number of nodes to be generated", 0 },
  { "seed",  'm', "seed_for_random", OPTION_ARG_OPTIONAL, 
    "Specify the seed for randon number generator", 0},
  { 0, 0, 0, 0, 0, 0 } 
}; 

static error_t parser_opt(int key, char *, struct argp_state *state)
{
  Parameters *parsPtr = static_cast<Parameters*>(state->input);

  switch (key)
    {
    case ARGP_KEY_END:
      if (parsPtr->type == INVALID)
	argp_usage(state);
      break;
    case 'n': 
      char *end;
      if (state->argv[state->next] == NULL)
	argp_usage(state);
      parsPtr->n = strtol(state->argv[state->next], &end, 10);
      if (*end != '\0' && *end != '\n')
	argp_usage(state);
      state->next++;
      break;
    case 'b':
      parsPtr->type = BIN;
      parsPtr->str = "BinTree";
      break;
    case 'a':
      parsPtr->type = AVL;
      parsPtr->str = "AvlTree";
      break;
    case 'r':
      parsPtr->type = RB;
      parsPtr->str = "RbTree";
      break;
    case 's':
      parsPtr->type = SPLAY;
      parsPtr->str = "SplayTree";
      break;
    case 'p': 
      parsPtr->type = TREAP;
      parsPtr->str = "Treap";
      break;
    case 'd': 
      parsPtr->type = RAND;
      parsPtr->str = "Randomized";
      break;
    case 'm': 
      if (state->argv[state->next] == NULL)
	argp_usage(state);
      parsPtr->seed = strtol(state->argv[state->next], &end, 10);
      if (*end != '\0' && *end != '\n')
	argp_usage(state);
      state->next++;
      break;
    case ARGP_KEY_ARG:
    default: return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static struct argp argDefs = { options, parser_opt, argDoc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{ 
  Parameters pars(1000, time(0));
  
  error_t status = argp_parse(&argDefs, argc, argv, 0, 0, &pars);

  if (status != 0)
    ERROR( ("Internal error") );

  if (pars.type == INVALID)
    ERROR( ("Invalid tree type" ) );

  unsigned long n = pars.n;

  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, pars.seed % gsl_rng_max(r));

  cout << "testAllTree<" << pars.str << "> " << n << " " << pars.seed
       << endl; 

  try
    {
      switch (pars.type)
	{
	case BIN:      
	  test<BinTree>(n, r); 
	  break;
	case AVL:      
	  test<Avl_Tree>(n, r); 
	  break;
	case TREAP:    
	  test<Treap>(n, r); 
	  break;
	case RAND:    
	  test<Rand_Tree>(n, r); 
	  break;
	case SPLAY:    
	  test<Splay_Tree>(n, r); 
	  break;
	case RB:  
	  test<Rb_Tree>(n, r); 
	  break; 
	case INVALID:
	default: ERROR("Invalid tree type %d", pars.type);
	}

      cout << "testAllTree<" << pars.str << "> " << n << " " << pars.seed
	   << endl; 
    }
  catch (exception & e)
    {
      cout << "**** Exception: " << e.what() << endl;
    }
  
  gsl_rng_free(r);
}
