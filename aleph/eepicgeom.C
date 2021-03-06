
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


# include "eepicgeom.H"

const double Eepic_Plane::default_r = 20;

const double Eepic_Plane::normal_thickness = 20;


const double Eepic_Plane::default_shade_thickness = 200;

const double Eepic_Plane::dotgap = 65; 


const double Eepic_Plane::arrow_width_in_mm = 0.5;

const double Eepic_Plane::arrow_lenght_in_mm = 1.5;


IMPL_PUT_IN_PLANE(Point);

IMPL_PUT_IN_PLANE(Segment);

IMPL_PUT_IN_PLANE(Text);

IMPL_PUT_IN_PLANE(Arrow);

IMPL_PUT_IN_PLANE(Thick_Segment);

IMPL_PUT_IN_PLANE(Thick_Dash_Segment);

IMPL_PUT_IN_PLANE(Thick_Arrow);

IMPL_PUT_IN_PLANE(Dotted_Segment);

IMPL_PUT_IN_PLANE(Dash_Segment);

IMPL_PUT_IN_PLANE(Dotted_Arrow);

IMPL_PUT_IN_PLANE(Dash_Arrow);

IMPL_PUT_IN_PLANE(Thick_Dash_Arrow);

IMPL_PUT_IN_PLANE(Triangle);

IMPL_PUT_IN_PLANE(Ellipse);

IMPL_PUT_IN_PLANE(Shade_Ellipse);
IMPL_PUT_IN_PLANE(Thick_Ellipse);

IMPL_PUT_IN_PLANE(Left_Text);
IMPL_PUT_IN_PLANE(Right_Text);
IMPL_PUT_IN_PLANE(Center_Text);

IMPL_PUT_IN_PLANE(Polygon);


    // con c�rculos punto en los v�rtices
IMPL_PUT_IN_PLANE(Polygon_With_Points); 

    // con c�rculos punto en los v�rtices
IMPL_PUT_IN_PLANE(Polygon_With_Arrows); 

    // lados puntillados
IMPL_PUT_IN_PLANE(Dotted_Polygon);

    // lados l�nea partida
IMPL_PUT_IN_PLANE(Dash_Polygon);

    // lados puntillados y c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Dotted_Polygon_With_Points); 

    // lados l�neas partidas con c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Dash_Polygon_With_Points);

    // lados l�neas partidas con punta d eflecha en �ltimo segmento
IMPL_PUT_IN_PLANE(Dash_Polygon_With_Arrow);

    // el interior del pol�gono es sobreado (gris)
IMPL_PUT_IN_PLANE(Shade_Polygon); 

    // el interior del pol�gono es sobreado (gris) con c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Shade_Polygon_With_Points);

    // el interior del pol�gono es sobreado (gris) y lados son flechas
IMPL_PUT_IN_PLANE(Shade_Polygon_With_Arrows);

    // interior sobreado (gris) y con coordenadas de los v�rtices
IMPL_PUT_IN_PLANE(Shade_Polygon_With_Text_Points);

    // interior sobreado (gris) y con n�meros de los v�rtices
IMPL_PUT_IN_PLANE(Shade_Polygon_With_Vertex_Numbers);

    // Curva spline construida seg�n los puntos del pol�gono
IMPL_PUT_IN_PLANE(Spline);

    // Curva spline a l�nea partida construida seg�n los puntos del pol�gono
IMPL_PUT_IN_PLANE(Dash_Spline);

    // Curva spline construida seg�n los puntos del pol�gono con flecha
IMPL_PUT_IN_PLANE(Spline_Arrow);

    // Curva spline a l�nea partida construida seg�n los puntos del
    // pol�gono con flecha
IMPL_PUT_IN_PLANE(Dash_Spline_Arrow);

    // con c�rculos punto en los v�rtices
IMPL_PUT_IN_PLANE(Thick_Polygon_With_Points); 

    // con c�rculos punto en los v�rtices
IMPL_PUT_IN_PLANE(Thick_Polygon_With_Arrows); 

    // lados puntillados
IMPL_PUT_IN_PLANE(Thick_Dotted_Polygon);

    // lados l�nea partida
IMPL_PUT_IN_PLANE(Thick_Dash_Polygon);

    // lados puntillados y c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Thick_Dotted_Polygon_With_Points); 

    // lados l�neas partidas con c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Thick_Dash_Polygon_With_Points);

    // lados l�neas partidas con puntas de flechas
IMPL_PUT_IN_PLANE(Thick_Dash_Polygon_With_Arrow);

    // el interior del pol�gono es sobreado (gris)
IMPL_PUT_IN_PLANE(Thick_Shade_Polygon); 

    // el interior del pol�gono es sobreado (gris) con c�rculos en los v�rtices
IMPL_PUT_IN_PLANE(Thick_Shade_Polygon_With_Points);

    // el interior del pol�gono es sobreado (gris) y lados son flechas
IMPL_PUT_IN_PLANE(Thick_Shade_Polygon_With_Arrows);

    // interior sobreado (gris) y con coordenadas de los v�rtices
IMPL_PUT_IN_PLANE(Thick_Shade_Polygon_With_Text_Points);

    // interior sobreado (gris) y con n�meros de los v�rtices
IMPL_PUT_IN_PLANE(Thick_Shade_Polygon_With_Vertex_Numbers);

    // Curva spline construida seg�n los puntos del pol�gono
IMPL_PUT_IN_PLANE(Thick_Spline);

    // Curva spline a l�nea partida construida seg�n los puntos del pol�gono
IMPL_PUT_IN_PLANE(Thick_Dash_Spline);

    // Curva spline construida seg�n los puntos del pol�gono con flecha
IMPL_PUT_IN_PLANE(Thick_Spline_Arrow);

    // Curva spline a l�nea partida construida seg�n los puntos del
    // pol�gono con flecha
IMPL_PUT_IN_PLANE(Thick_Dash_Spline_Arrow);


IMPL_PUT_IN_PLANE(Regular_Polygon_With_Points);
IMPL_PUT_IN_PLANE(Dotted_Regular_Polygon);
IMPL_PUT_IN_PLANE(Dash_Regular_Polygon);
IMPL_PUT_IN_PLANE(Dotted_Regular_Polygon_With_Points);
IMPL_PUT_IN_PLANE(Dash_Regular_Polygon_With_Points);
IMPL_PUT_IN_PLANE(Shade_Regular_Polygon);
IMPL_PUT_IN_PLANE(Shade_Regular_Polygon_With_Points);
IMPL_PUT_IN_PLANE(Shade_Regular_Polygon_With_Arrows);
IMPL_PUT_IN_PLANE(Shade_Regular_Polygon_With_Text_Points);
IMPL_PUT_IN_PLANE(Shade_Regular_Polygon_With_Vertex_Numbers);

void Eepic_Point::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Point & p = static_cast<const Point&>(*geom_object_ptr);

  plane->draw_point(p, output);
}

void Eepic_Segment::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_segment(sg, output);
}


void Eepic_Thick_Segment::draw(Eepic_Plane * plane, ofstream & output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_segment(sg, output);
}


void Eepic_Thick_Dash_Segment::draw(Eepic_Plane * plane, 
				    ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_dash_segment(sg, output);
}

void Eepic_Arrow::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_arrow(sg, output);
}


void Eepic_Plane::draw_cartesian_axis(ofstream & output)
{
  if (not with_cartesian_axis)
    return;

  Segment x_axis(Point(__leftmost.get_x(), 0), Point(__rightmost.get_x(), 0)); 
  draw_arrow(x_axis, output);

  Segment y_axis(Point(0, __lowest.get_y()), Point(0, __highest.get_y())); 
  draw_arrow(y_axis, output);
}


void Eepic_Thick_Arrow::draw(Eepic_Plane * plane, ofstream & output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_arrow(sg, output);
}


void Eepic_Dotted_Segment::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_dotted_segment(sg, output);
}


void Eepic_Dash_Segment::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_dash_segment(sg, output);
}


void Eepic_Dotted_Arrow::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_arrow_dotted_segment(sg, output);
}


void Eepic_Dash_Arrow::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_arrow_dash_segment(sg, output);
}


void Eepic_Thick_Dash_Arrow::draw(Eepic_Plane * plane, 
				  ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  const Segment & sg = static_cast<const Segment&>(*geom_object_ptr);

  plane->draw_arrow_dash_segment(sg, output);
}

void Eepic_Triangle::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Triangle & t = static_cast<const Triangle&>(*geom_object_ptr);

  const double p1_x = plane->x_geom_number_to_eepic(t.get_p1().get_x());

  const double p1_y = plane->y_geom_number_to_eepic(t.get_p1().get_y());

  const double p2_x = plane->x_geom_number_to_eepic(t.get_p2().get_x());

  const double p2_y = plane->y_geom_number_to_eepic(t.get_p2().get_y());

  const double p3_x = plane->x_geom_number_to_eepic(t.get_p3().get_x());

  const double p3_y = plane->y_geom_number_to_eepic(t.get_p3().get_y());

  output << "    %% Triange from " << t.get_p1().to_string() 
	 << " to " << t.get_p2().to_string() 
	 << " to " << t.get_p2().to_string() << std::endl
	 << "    %% mapped in this plane to (" << p1_x << "," << p1_y 
	 << ") to (" << p2_x << "," << p2_y << ") to (" 
	 << p3_x << "," << p2_y << ")" << std::endl
	 << "\\path(" << p1_x << "," << p1_y 
	 << ")(" << p2_x << "," << p2_y << ")("
	 << p3_x << "," << p3_y << ")(" 
	 << p1_x << "," << p1_y << ")" << std::endl
	 << std::endl;
}

void Eepic_Ellipse::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Ellipse & e = static_cast<const Ellipse&>(*geom_object_ptr);
    
  plane->draw_ellipse(e, output);
}


void Eepic_Shade_Ellipse::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Ellipse & e = static_cast<const Ellipse&>(*geom_object_ptr);
    
  output << "\\shade" << endl;

  plane->draw_ellipse(e, output);
}


void Eepic_Thick_Ellipse::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Ellipse & e = static_cast<const Ellipse&>(*geom_object_ptr);
    
  output << "\\Thicklines" << std::endl;

  plane->draw_ellipse(e, output);
}


void Eepic_Text::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Text & t = static_cast<const Text &>(*geom_object_ptr);	

  plane->draw_text(t, output, 0, 0.5);
}


void Eepic_Left_Text::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Text & t = static_cast<const Text &>(*geom_object_ptr);	

  plane->draw_text(t, output, 0, 0.5);
}


void Eepic_Right_Text::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Text & t = static_cast<const Text &>(*geom_object_ptr);	

  plane->draw_text(t, output, 1.2, 0.5);
}


void Eepic_Center_Text::draw(Eepic_Plane * plane, ofstream & output) const
{
  const Text & t = static_cast<const Text &>(*geom_object_ptr);	

  plane->draw_text(t, output, 0.5, 0.5);
}

void Eepic_Polygon::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_segment);
}

                // ahora vienen las implantaciones de c�mo se dibuja
                // cada uno de los polinomios 

void Eepic_Polygon_With_Points::draw(Eepic_Plane * plane, 
				     ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Polygon_With_Arrows::draw(Eepic_Plane * plane, 
				     ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon_with_arrows(poly, output);
}


void Eepic_Dotted_Polygon::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);
}


void Eepic_Dash_Polygon::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);
}

void Eepic_Dotted_Polygon_With_Points::draw(Eepic_Plane * plane, 
					    ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Dash_Polygon_With_Points::draw(Eepic_Plane * plane, 
					  ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);

  plane->draw_dots_from_polygon(poly, output);
}

void Eepic_Dash_Polygon_With_Arrow::draw(Eepic_Plane * plane, 
					 ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);

  Segment sg = poly.get_last_segment();

  plane->draw_arrow_extreme(sg, output);
}



void Eepic_Shade_Polygon::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);
}


void Eepic_Shade_Polygon_With_Points::draw(Eepic_Plane * plane, 
					   ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Shade_Polygon_With_Arrows::draw(Eepic_Plane * plane, 
					   ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_arrows_in_polygon(poly, output);
}


void Eepic_Shade_Polygon_With_Text_Points::draw(Eepic_Plane * plane, 
						ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_points_text_in_polygon(poly, output);
}


void Eepic_Shade_Polygon_With_Vertex_Numbers::draw(Eepic_Plane * plane, 
						   ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_vertex_numbers_in_polygon(poly, output);
}


void Eepic_Spline::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_spline(poly, output);
}


void Eepic_Dash_Spline::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_dash_spline(poly, output);
}


void Eepic_Spline_Arrow::draw(Eepic_Plane * plane, ofstream & output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_spline_arrow(poly, output);
}


void Eepic_Dash_Spline_Arrow::draw(Eepic_Plane * plane, 
				   ofstream &    output) const
{
  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_dash_spline_arrow(poly, output);
}


// Exactamente los mismo m�todos pero con la adici�n de Thicklines

void Eepic_Thick_Polygon_With_Points::draw(Eepic_Plane * plane, 
					   ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Thick_Polygon_With_Arrows::draw(Eepic_Plane * plane, 
					   ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon_with_arrows(poly, output);
}


void Eepic_Thick_Dotted_Polygon::draw(Eepic_Plane * plane, 
				      ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);
}


void Eepic_Thick_Dash_Polygon::draw(Eepic_Plane * plane, 
				    ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);
}

void Eepic_Thick_Dotted_Polygon_With_Points::draw(Eepic_Plane * plane, 
						  ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Thick_Dash_Polygon_With_Arrow::draw(Eepic_Plane * plane, 
					       ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);

  Segment sg = poly.get_last_segment();

  plane->draw_arrow_extreme(sg, output);
}


void Eepic_Thick_Dash_Polygon_With_Points::draw(Eepic_Plane * plane, 
						ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Thick_Shade_Polygon::draw(Eepic_Plane * plane, 
				     ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);
}


void Eepic_Thick_Shade_Polygon_With_Points::draw(Eepic_Plane * plane, 
						 ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Thick_Shade_Polygon_With_Arrows::draw(Eepic_Plane * plane, 
						 ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_arrows_in_polygon(poly, output);
}


    void 
Eepic_Thick_Shade_Polygon_With_Text_Points::draw(Eepic_Plane * plane, 
						 ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_points_text_in_polygon(poly, output);
}


    void 
Eepic_Thick_Shade_Polygon_With_Vertex_Numbers::draw(Eepic_Plane * plane, 
						    ofstream & output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_vertex_numbers_in_polygon(poly, output);
}


void Eepic_Thick_Spline::draw(Eepic_Plane * plane, ofstream & output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_spline(poly, output);
}


void Eepic_Thick_Dash_Spline::draw(Eepic_Plane * plane, 
				   ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_dash_spline(poly, output);
}


void Eepic_Thick_Spline_Arrow::draw(Eepic_Plane * plane, 
				    ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_spline_arrow(poly, output);
}


void Eepic_Thick_Dash_Spline_Arrow::draw(Eepic_Plane * plane, 
					 ofstream &    output) const
{
  output << "\\Thicklines" << std::endl
	 << std::endl;

  Polygon & poly = (Polygon&)(*geom_object_ptr);

  plane->draw_dash_spline_arrow(poly, output);
}


void Eepic_Regular_Polygon::draw(Eepic_Plane * plane, ofstream & output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_segment);
}


void Eepic_Regular_Polygon_With_Points::draw(Eepic_Plane * plane, 
					     ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Dotted_Regular_Polygon::draw(Eepic_Plane * plane, 
					ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);
}


void Eepic_Dash_Regular_Polygon::draw(Eepic_Plane * plane, 
				      ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);
}

void Eepic_Dotted_Regular_Polygon_With_Points::draw(Eepic_Plane * plane, 
						    ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dotted_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Dash_Regular_Polygon_With_Points::draw(Eepic_Plane * plane, 
						  ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_polygon(poly, output, &Eepic_Plane::draw_dash_segment);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Shade_Regular_Polygon::draw(Eepic_Plane * plane, 
				       ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);
}


void Eepic_Shade_Regular_Polygon_With_Points::draw(Eepic_Plane * plane, 
						   ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_dots_from_polygon(poly, output);
}


void Eepic_Shade_Regular_Polygon_With_Arrows::draw(Eepic_Plane * plane, 
						   ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_arrows_in_polygon(poly, output);
}


    void
Eepic_Shade_Regular_Polygon_With_Text_Points::draw(Eepic_Plane * plane, 
						   ofstream &    output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_points_text_in_polygon(poly, output);
}


    void 
Eepic_Shade_Regular_Polygon_With_Vertex_Numbers::draw
    (Eepic_Plane * plane, ofstream & output) const
{
  Regular_Polygon & poly = (Regular_Polygon&)(*geom_object_ptr);

  plane->draw_closed_polygon(poly, output);

  plane->draw_vertex_numbers_in_polygon(poly, output);
}

