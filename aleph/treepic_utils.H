
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

# ifndef TREEPIC_UTILS_H
# define TREEPIC_UTILS_H

# include <argp.h>
# include <string.h>
# include <stdio.h>
# include <math.h>
# include <stdlib.h>

# include <iostream>
# include <fstream>
# include <aleph.H>

using namespace std;

extern const long double sin_45;
extern const long double cos_45;

extern string input_file_name;
extern string output_file_name;


extern const char * const font_wrapper; 
extern const char * fill_type;

extern long double resolution;
extern long double font_width_in_mm; 
extern long double font_height_in_mm; 
extern long double font_width_in_points;

extern long double resolution; /* resolution en milimetros */
extern double v_size;

//extern bool tiny_keys;

long double font_width();
long double font_height();


/* 
   calcula los puntos de interseccion entre una recta y una elipse. La
   recta pasa por el punto central de la elipse.

   lx0, ly0 : punto centro de la elipse y primer punto de la recta
   lx1, ly1 : segundo punto de la recta
   a,b parametros de elipse (x - lx0)^2/a^2 + y - ly0)^2/b^2

   dx, dy son las diferencias en x e y de los puntos de interseccion 
*/
    void 
intersection_ellipse_line(long double lx0, // puntos recta
			  long double ly0, 
			  long double lx1, 
			  long double ly1, 
			  long double a, // parametros elipse (centro lx0,ly0)
			  long double b, 
			  long double & dx, 
			  long double & dy); /* Diferencias respecto al
						centro de la elipse */ 


    void 
intersection_line_line(long double lx1, // puntos recta 1
		       long double ly1, 
		       long double lx2, 
		       long double ly2, 
		       long double rx1, // puntos recta 2
		       long double ry1, 
		       long double rx2, 
		       long double ry2,
		       long double & x,
		       long double & y);


long double distance_between_points(long double x1, 
				    long double y1, 
				    long double x2,
				    long double y2);

/* 
   calcula los puntos de interseccion entre una recta y un
   rectangulo. La recta pasa por el punto central del rectangulo.

   lx0, ly0 : punto centro del rectangulo y primer punto de la recta
   lx1, ly1 : segundo punto de la recta
   a,b radios horizontal y vertical del rectangulo

   dx, dy son las diferencias en x e y de los puntos de interseccion 
*/
    void 
intersection_rectangle_line(long double lx0, // puntos recta
			    long double ly0, 
			    long double lx1, 
			    long double ly1, 
			    long double a, // radio hor (centro lx0,ly0)
			    long double b, // radio ver
			    long double & dx, 
			    long double & dy); /* Diferencias respecto al
						 centro rectangulo */ 

extern bool flip_y;

inline long double YPIC(long double y) 
{ 
  return flip_y ? y : v_size - y; 
}


size_t compute_true_len(const string & str);


void put_string(ofstream& output,
		const long double& x, const long double& y,
		const string & comment, const string & str);

void put_string_tkiz(ofstream& output,
		     const long double& x, const long double& y,
		     const string & comment, const string & str);

extern long double arrow_width;
extern long double arrow_lenght;
extern bool with_arrow;
extern long double dash_len;

void draw_arc(ofstream& output,
	      long double src_x, long double src_y, 
	      long double tgt_x, long double tgt_y,
	      bool is_dashed, bool with_arrow, bool thick = true);

void draw_arc_tikz(ofstream& output,
		   long double src_x, long double src_y, 
		   long double tgt_x, long double tgt_y,
		   bool is_dashed, bool with_arrow, bool thick = true);

/*
  Esta rutina estima el offset x donde se debe colocar la cadena str
  en una ventana de ancho window_size */
long double center_string(const string& str, long double window_size);


/* calcula la longitud de la cadena en puntos segun la resolucion */
long double string_width(const string& str);


int compute_section(long double x1, long double y1, 
		    long double x2, long double y2);


/* Dados dos puntos (sx,sy) y (tx,ty) calcula el punto (mx,my) situado
   perpendicularmente sobre la recta */
void compute_mid_point_line(const long double & sx, const long double & sy, 
			    const long double & tx, const long double & ty,
			    const long double & d, 
			    const bool &        left, 
			    long double & mx, long double & my); 


# endif // TREEPIC_UTILS_H
