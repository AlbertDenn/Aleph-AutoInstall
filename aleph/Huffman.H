
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

# ifndef HUFFMAN_H
# define HUFFMAN_H


# include <memory>
# include <tpl_binNodeUtils.H>
# include <tpl_treap.H>
# include <tpl_binHeap.H>
# include <tpl_dynMapTree.H>
# include <bitArray.H>


namespace Aleph {

struct Huffman_Node;

using Symbol_Map = DynMapTree<string, Huffman_Node *, Treap_Vtl>;

using Freq_Node = BinNode<std::pair<string, size_t>>;

struct  Huffman_Node : public BinHeap<size_t>::Node
{
  BinNode<string> * bin_node; 

  Freq_Node * freq_node;

public:

  Huffman_Node() 
    : BinHeap<size_t>::Node(0), bin_node(nullptr), freq_node(nullptr) 
  {
    /* empty */ 
  }

  Huffman_Node(BinNode<string> * node) 
    : BinHeap<size_t>::Node(0), bin_node(node), freq_node(nullptr)
  {
    /* empty */
  }

  ~Huffman_Node() { /* No debe liberarse memoria de bin_node */ }

};

typedef BinHeap<size_t> Huffman_Heap;

static inline const size_t & get_freq(Huffman_Node * huffman_node) noexcept
{
  return huffman_node->get_key();
}


static inline void increase_freq(Huffman_Node * huffman_node) noexcept
{
  huffman_node->get_key()++;
}


static inline void set_freq(Huffman_Node * huffman_node, const size_t & freq)
  noexcept
{
  huffman_node->get_key() = freq;
}

  typedef DynMapTree<string, BitArray, Treap_Vtl> Code_Map;
static inline bool is_leaf(BinNode<string> * p) noexcept
{
  return LLINK(p) == nullptr and RLINK(p) == nullptr;
}

class Huffman_Encoder_Engine
{
  BinNode<string> * root; 
  Huffman_Heap heap;
  Symbol_Map   symbol_map; 
  Code_Map code_map;

  Freq_Node * freq_root;

  string end_symbol;
  size_t text_len;

  void build_prefix_encoding(BinNode<string> * p, BitArray & array)
  {
    if (is_leaf(p))
      {
        const string & str = p->get_key();
        code_map.insert(str, BitArray(array));
        return;
      }
    array.push(0); build_prefix_encoding(LLINK(p), array); array.pop();
    array.push(1); build_prefix_encoding(RLINK(p), array); array.pop();
  }

  void build_encoding_map()
  {
    assert(symbol_map.size() > 0);

    if (root == nullptr)
      throw domain_error("Huffman encoding tree has not been generated");

    BitArray array(0);
    symbol_map.empty();
    build_prefix_encoding(root, array);
  }

  bool test_end(const string & str) const
  {
    if (end_symbol == "NO-END") 
      return false;
    return end_symbol == str;
  }

  void update_freq(const string & str)
  {
    if (root != nullptr)
      throw domain_error("Huffman encoding tree has already been generated");

    if (test_end(str))
      throw domain_error("End symbol has already been inserted");

    Huffman_Node * huffman_node      = nullptr;
    auto huffman_node_ptr = symbol_map.search(str);
    if (huffman_node_ptr == nullptr) // ¿símbolo definido previamente?
      { // No ==> crear una entrada en symbol_map e insertarlo en heap
        unique_ptr<BinNode<string> > bin_node_auto(new BinNode<string>(str));
        huffman_node = static_cast<Huffman_Node*>
          (heap.insert(new Huffman_Node(bin_node_auto.get())));
        symbol_map.insert(str, huffman_node);
        bin_node_auto.release();
      }
    else
      huffman_node = huffman_node_ptr->second; // ya definido, recuperarlo

    increase_freq(huffman_node); 
    heap.update(huffman_node);
  }

  static void append_code(BitArray & bit_stream, const BitArray & symbol_code)
  {
    const size_t symbol_code_size = symbol_code.size();
    for (int i = 0; i < symbol_code_size; i++) 
      bit_stream.push(symbol_code[i]); 
  }

  public:

  Huffman_Encoder_Engine() 
    : root(nullptr), freq_root(nullptr), end_symbol("NO-END"), text_len(0)
  {
    // empty
  }

  private:

  struct Get_Key
  {
    string operator () (BinNode<string> * p) const noexcept
    {
      return is_leaf(p) ? p->get_key() : "";
    }
  };

  struct Load_Key
  {
    void operator () (BinNode<string> * p, istream & input) const noexcept
    {
      if (is_leaf(p))
        input >> p->get_key();
    }
  };

  public:

  void save_tree(ostream & output) 
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    Aleph::save_tree<BinNode<string>> (root, output);
  }

  void save_tree_in_array_of_chars(const string & array_name, ostream & output)
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    Aleph::save_tree_in_array_of_chars<BinNode<string>, Get_Key> 
      (root, array_name, output);
  }

  BinNode<string> *& get_root()  
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    return root; 
  }

  BinNode<string> * generate_huffman_tree(const bool & with_freqs = false) 
  {
    while (heap.size() > 1) // hasta que quede solo un nodo
      {
        Huffman_Node * l_huffman_node = (Huffman_Node*) heap.getMin(); // izq
        Huffman_Node * r_huffman_node = (Huffman_Node*) heap.getMin(); // der
        BinNode <string> * bin_node = new BinNode <string>;
        Huffman_Node * huffman_node = new Huffman_Node (bin_node);
        LLINK(bin_node) = l_huffman_node->bin_node;
        RLINK(bin_node) = r_huffman_node->bin_node;
        const size_t new_freq = get_freq(l_huffman_node) + 
	  get_freq(r_huffman_node);
        Aleph::set_freq(huffman_node, new_freq);

        if (with_freqs)
          {
            Freq_Node *& l_freq_node = l_huffman_node->freq_node;
            if (l_freq_node == nullptr)
              {
                l_freq_node                    = new Freq_Node;
                l_freq_node->get_key().first   = 
                  l_huffman_node->bin_node->get_key();
                l_freq_node->get_key().second = l_huffman_node->get_key();
              }

            Freq_Node *& r_freq_node = r_huffman_node->freq_node;
            if (r_freq_node == nullptr)
              {
                r_freq_node                    = new Freq_Node;
                r_freq_node->get_key().first   = 
                  r_huffman_node->bin_node->get_key();
                r_freq_node->get_key().second = r_huffman_node->get_key();
              }

            const string str = to_string(new_freq);
            Freq_Node *& freq_node      = huffman_node->freq_node;
            freq_node                   = new Freq_Node;
            freq_node->get_key().first  = str;
            freq_node->get_key().second = huffman_node->get_key();
            LLINK(freq_node)            = l_freq_node;
            RLINK(freq_node)            = r_freq_node;
          }

        delete l_huffman_node;
        delete r_huffman_node;
        heap.insert(huffman_node);
      } // nodo que queda en heap es la raíz del árbol de prefijos  

    Huffman_Node * huffman_root = (Huffman_Node *) heap.getMin();
    root = huffman_root->bin_node;

    if (with_freqs)
      freq_root = huffman_root->freq_node;

    delete huffman_root;
    build_encoding_map(); // construir mapeo de códigos

    return root;
  }

  void load_tree(ifstream & input)
  {
    if (root != BinNode<string>::NullPtr)
      destroyRec(root);
    
    root = Aleph::load_tree<BinNode<string>>(input);
  }

  Freq_Node *& get_freq_root() 
  { 
    if (freq_root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    return freq_root; 
  }

  void set_freq(const string & str, const size_t & freq)
  {
    if (root != nullptr)
      throw domain_error("Huffman encoding tree has already been generated");
    if (test_end(str))
      throw domain_error("End symbol has already been inserted");
       
        // Buscar símbolo str
    auto huffman_node_ptr = symbol_map.search(str);
    if (huffman_node_ptr != nullptr) // ¿ya fue definido?
      {
	std::string msg = "Frequency for symbol " + str + " has already set";
	throw std::domain_error(msg); // Sí ==> esto es un error!
      }
   
    unique_ptr<BinNode<string> > bin_node_auto(new BinNode<string>(str));
    Huffman_Node * huffman_node = new Huffman_Node(bin_node_auto.get());
    Aleph::set_freq(huffman_node, freq); 
    heap.insert(huffman_node);
    symbol_map.insert(str, huffman_node);
    bin_node_auto.release();
  }

  private:

    static const size_t Max_Token_Size = 256;

  public:

  void read_input(char * input, const bool & with_freqs = false)
  {
    if (root != nullptr)
      throw domain_error("Huffman encoding tree has already been generated");

    char * curr_stream = input;
    char curr_token[Max_Token_Size];
    curr_token[1] = '\0';

    while (*curr_stream != '\0')
      {
        curr_token[0] = *curr_stream++; 
        update_freq(curr_token);
        text_len++;
      }

    set_end_of_stream("");
    generate_huffman_tree(with_freqs);
  }

  void read_input(ifstream & input, const bool & with_freqs = false)
  {
    if (root != nullptr)
      throw domain_error("Huffman encoding tree has already been generated");

    char curr_token[2];
    curr_token[0] = curr_token[1] = '\0';

    while (not input.eof())
      {
        input.read(curr_token, 1); 
        update_freq(curr_token);
        text_len++;
      }

    set_end_of_stream("");
    generate_huffman_tree(with_freqs);
  }

  void set_end_of_stream(const string & str)
  {
    if (test_end(str))
      throw domain_error("End symbol has already been inserted");

    if (root != nullptr)
      throw domain_error("Huffman encoding tree has already been generated");

    unique_ptr<BinNode<string> > bin_node_auto ( new BinNode<string> (str) );

    Huffman_Node * huffman_node =  static_cast<Huffman_Node*>
      (heap.insert(new Huffman_Node(bin_node_auto.get())));
    symbol_map.insert(str, huffman_node); 
    bin_node_auto.release();
    end_symbol = str;
  }

  size_t encode(char * input, BitArray & bit_stream)
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    char * curr_stream = input;
    char curr_token[Max_Token_Size];
    curr_token[1] = '\0';
    while (*curr_stream != '\0')
      {
        curr_token[0] = *curr_stream++; 
        append_code(bit_stream, code_map.find(curr_token));
      }
    append_code(bit_stream, code_map.find(""));

    return bit_stream.size();
  }

  size_t encode(ifstream & input, BitArray & bit_stream)
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");
    char curr_token[2];
    curr_token[0] = curr_token[1] = '\0'; 
    while (not input.eof())
      {
        input.read(curr_token, 1); 
        append_code(bit_stream, code_map.find(curr_token));
      }
    append_code(bit_stream, code_map.find(""));
    return bit_stream.size();
  }
};

class Huffman_Decoder_Engine
{
  BinNode<string> * root; 
  string end_symbol;
  public:

  Huffman_Decoder_Engine(BinNode<string> * p, const string & end)
    : root(p), end_symbol(end)
  {
    // empty
  }

  BinNode<string> *& get_root()  
  {
    if (root == nullptr)
      throw std::domain_error("Huffman tree has not been generated");

    return root; 
  }

  void decode(BitArray & bit_stream, ostream & output)
  {
    const size_t & bit_stream_len = bit_stream.size();
    BinNode<string> * p = root;
    for (int i = 0; i < bit_stream_len; ++i)
      {
        if (bit_stream.read_bit(i) == 0)
          p = LLINK(p);
        else
          p = RLINK(p);


       if (p == nullptr)
         throw std::domain_error("Invalid bits sequence");

       if (is_leaf(p)) // ¿es hoja?
         {     // sí ==> escribir símbolo y reiniciar a raíz
           const string & symbol = p->get_key();
           if (symbol == end_symbol) // ¿se alcanza final?
             break;

           output << symbol;
           p = root; // reiniciar a raíz, pues se leerá un nuevo código
         }
      }
  }
};

} // end namespace Aleph

# endif // HUFFMAN_H
