
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

# include <X11/Xlib.h>
# include <unistd.h>
# include <iostream>
# include <aleph.H>


using namespace std;


int window_area(const XWindowAttributes & attr)
{
  return attr.width * attr.height;
}

XWindowAttributes window_attributes(Display * disp, const Window & window)
{
  XWindowAttributes attr;

  int status =  XGetWindowAttributes(disp, window, &attr);

  if (status == 0)
    throw std::domain_error("cannot get window attributes");

  return attr;
}

int window_area(Display * disp, const Window & window)
{
  return window_area(window_attributes(disp, window));
}

void print_window_attributes(const XWindowAttributes & attr)
{
  cout << "(x,y)  = (" << attr.x << "," << attr.y << ")" << endl
       << "width  = " << attr.width << endl
       << "height = " << attr.height << endl;
}


void print_window_attributes(Display * disp, const Window & window)
{
  print_window_attributes(window_attributes(disp, window));
}

Window find_window_with_bigest_area(Display * disp, 
				    Window * ww_array,
				    unsigned int & nwindows)
{
  if (nwindows == 0)
    throw std::domain_error("There are not windows");

  Window max_window = ww_array[0];
  int max_area = window_area(disp, ww_array[0]);

  for (int i = 1; i < nwindows; ++i)
    {
      int current_area = window_area(disp, ww_array[i]);

      if (current_area > max_area)
	{
	  max_area = current_area;
	  max_window = ww_array[i];
	}
    }

  return max_window;
}

int main()
{
// Open a display.
  Display * d = XOpenDisplay(0);

  if (d == NULL)
    EXIT("cannot open dispaly");

  Window default_root_window = DefaultRootWindow(d);

  Window root;
  Window parent;
  Window * children;
  unsigned int nchildren;

  int status = XQueryTree(d, default_root_window, &root, &parent,
			  &children, &nchildren);

  if (status == 0)
    EXIT("error obtaining root window (status = %d)", status);

  cout << "Ventana raíz por omisión es " << default_root_window << endl
       << "que debe ser la misma que " << root << endl
       << "el padre tiene valor " << parent << endl
       << "hay " << nchildren << " ventanas hijas" << endl;

  XWindowAttributes attr;

  status = XGetWindowAttributes(d, root, &attr);

  if (status == 0)
    EXIT("error obtaining root window attributes (status = %d)", status);  

  print_window_attributes(attr);
  
  Window max_window = find_window_with_bigest_area(d, children, nchildren);

  cout << "La ventana con mayor area es " << max_window
       << " con valor de area = " << window_area(d, max_window) << endl;

  print_window_attributes(d, max_window);

  return 0;

}


