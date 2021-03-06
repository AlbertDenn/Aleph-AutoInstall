
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

# ifndef SEGMENT_H
# define SEGMENT_H

# include "point.H"
# include "points_utils.H"

class Segment
{

  const Point * a;
  const Point * b;

public:

  Segment() : a(nullptr), b(nullptr)
  { 
    /* empty */  
  }
  
  Segment(const Point * __a, const Point * __b) : a(__a), b(__b) 
  { 
    /* empty */ 
  }

  Segment(const Point & __a, const Point & __b) : a(&__a), b(&__b) 
  { 
    /* empty */ 
  }
  
  ~Segment()
  {
    /* empty */
  }
  
  const bool operator == (const Segment & segment) const 
  {
    return ((*a == *segment.a and *b == *segment.b) or
	    (*a == *segment.b and *b == *segment.a));
  }
  
  /* Verifica si el segmento intersecta otro segmento  */
  const bool intersect(const Segment & segment) const
  {
    return intersectp(*a, *b, *segment.a, *segment.b);
  }

  const Point * get_a() { return a ;} //Retorna el punto 'a' del segmento

  const Point * get_b() { return b; } //Retorna el punto 'b' del segmento


  /* Recibe la coordenada 'y' de una linea horizontal y devuelve */
  /*     el punto de interseccion con el segmento                */
  Point  horiz_line_inter(const Geom_Number & __y)
  {
    
    Geom_Number intersection_x;
    float m;

    m = (float)(a->get_y() - b->get_y()) / (float)(a->get_x() - b->get_x());

    intersection_x = (Geom_Number)((__y - b->get_y())/m + b->get_x());
    
    Point  intersection_point(intersection_x,__y);

    return intersection_point;
  }

  /* Retorna el punto mas alto entre a y b  */  
  const Point * highest_point()
  {

    if (a->get_y() > b->get_y())
      return a;
    else
      return b;
  }

  /* Retorna el punto mas bajo entre a y b  */  
  const Point * lowest_point()
  {
    if (a->get_y() < b->get_y())
      return a;
    else
      return b;
  }
};

# endif // SEGMENT_H
