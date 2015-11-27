
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
#include "ipcClient.H"

int main(int argn, char * argv[])
{
  int delay = 0;

  if (argn > 1)
    delay = atoi(argv[1]);

  cout << "Server with a delay of " << delay << " seconds" << endl;

  char request_buffer[81];
  RawMsg request_message(&request_buffer[0], 81);

  char reply_buffer[81];
  RawMsg reply_message(&reply_buffer[0], 81);

  char stringPort[Port::stringSize];

  cout << "Port::stringSize = " << Port::stringSize << endl;

  MsgId msgid;

  char * received_string;

  IpcServer server;
  printf("SERVER PORT %s\n", 
	 server.getServerPort().getStringPort(stringPort, Port::stringSize)
	 );

  cout << "strlen(stringPort) = " << strlen(stringPort) << endl;;
  char str[80];

  for (int i = 0; true; i++)
    {
      msgid = server.receive_request(request_message); 

      received_string = static_cast<char*>(request_message.getBodyAddr());

      sprintf(reply_buffer, "Reply %i", i);

      reply_message.setBodySize(strlen(reply_buffer) + 1);

      if (delay not_eq 0)
	sleep(delay);

      server.send_reply(reply_message, msgid);

      cout << "Replied message \"" << received_string << "\" (" 
	   << msgid.msgid_to_char(str) << ")" << endl; 

    }
	
  return 0;
}

