
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


# include <gsl/gsl_rng.h>
# include <gsl/gsl_randist.h>
# include <iostream>
# include <limits>
# include <eepicgeom.H>

bool tiny_keys;

using namespace std;

    // construye un poligono aleatorio de n lados cuyos puntos no sobrepasan
    // los extremos actuales del plano
Polygon make_random_polygon(const size_t & n,
			    double min_x, double max_x, 
			    double min_y, double max_y)
{
  gsl_rng * r = gsl_rng_alloc (gsl_rng_mt19937);
  gsl_rng_set(r, time(NULL) % gsl_rng_max(r));

 again:

  {
    Polygon poly;

    for (int i = 0; i < n; ++i)
      {
      
      retry:

	try
	  {
	    cout << "Generating point " << i << endl;

	    const Geom_Number x = gsl_ran_flat(r, min_x, max_x);
	    const Geom_Number y = gsl_ran_flat(r, min_y, max_y);

	    cout << Point(x, y).to_string() << " triying ..." << endl;

	    poly.add_vertex(Point(x, y));

	    cout << "success" << endl;
	  }
	catch (domain_error)
	  {
	    cout << " crosses" << endl;
	    goto retry;
	  }
      }

    cout << "closing ..." << endl;

    try
      {
	poly.close();

	cout << "Finished polygon generation" << endl;
  
	gsl_rng_free(r);

	return poly;
      }
    catch (domain_error)
      {
	cout << " close crosses" << endl;

	goto again;
      }
  }    
}


int main()
{
  Segment sg0(Point(232, 438), Point(1227, 2183));
  Segment sg1(Point(162, 1838), Point(327, 883));
  Segment sg2(Point(16, 38), Point(827, 783));
  Segment sg3(Point(1000, 1500), Point(10, 183));
  Segment sg4(Point(200, 300), Point(-800, 1600));
  Segment sg5(Point(0, 1400), Point(1800, 0));

  Segment s1(Point(0, 0), Point(0, 1400));
  Segment s2(Point(0, 1400), Point(2800, 1400));

  Point p1 = sg4.intersection_with(sg5);

  Triangle tr(Point(-1632, 237), Point(737, 235), Point(272, 1772));

  Triangle tr1(Point(0, 0), Point(600, 600), Point(800, 0));

  Ellipse el(Point(-800, 600), 800, 330);

  Point pt(400 + 700, 400 + 100);

# ifdef nada
    if (el.contains_to(pt))
      cout << "El punto de prueba " << pt.to_string() 
	   << " está dentro de la elipse de centro " 
	   << el.get_center().to_string() << endl
	   << "con a = " << el.get_hradius() << " y b = " << el.get_vradius()
	   << endl;
    else
      cout << "Falló prueba de inclusión del punto " << pt.to_string() 
	   << " en elipse de centro " 
	   << el.get_center().to_string() << endl
	   << "con a = " << el.get_hradius() << " y b = " << el.get_vradius()
	   << endl;

    char c;
    
    cin >> c;
# endif

    Eepic_Plane plane(2000, 2000);

  put_in_plane(plane, pt);

  put_in_plane(plane, Arrow(s1));
  put_in_plane(plane, s2);

  put_in_plane(plane, p1);
  
  put_in_plane(plane, sg4);
  put_in_plane(plane, sg5);

  put_in_plane(plane, sg1);
  put_in_plane(plane, sg2);
  put_in_plane(plane, sg3);
  
  put_in_plane(plane, tr);
  put_in_plane(plane, tr1);
  put_in_plane(plane, el);  

  Segment si = sg4.intersection_with(tr);

  Segment ts1, ts2;

  el.compute_tangents(ts1, ts2, 3.25);
  
  put_in_plane(plane, Arrow(ts1));
  put_in_plane(plane, ts2);

  Segment par(ts1, 100);

  put_in_plane(plane, Arrow(par));

  Segment seg (Point(4,0), Point(-300, 400));
  Segment inter = el.intersection_with(seg);

  put_in_plane(plane, inter);

  put_in_plane(plane, inter.get_src_point());
  put_in_plane(plane, inter.get_tgt_point());

# ifdef nada
  put_in_plane(plane, 
	       Text(sg0.get_tgt_point(), 
		    gnu::autosprintf("m0 = %.2f", sg0.slope())));
  put_in_plane(plane, Arrow(sg0));

  put_in_plane(plane, 
	       Text(sg1.get_tgt_point(), 
		    gnu::autosprintf("m1 = %.2f", sg1.slope())));
  put_in_plane(plane, Arrow(sg1));

  put_in_plane(plane, 
	       Text(sg2.get_tgt_point(), 
		    gnu::autosprintf("m2 = %.2f", sg2.slope())));
  put_in_plane(plane, Arrow(sg2));

  put_in_plane(plane, 
	       Text(sg3.get_tgt_point(), 
		    gnu::autosprintf("m3 = %.2f", sg3.slope())));
  put_in_plane(plane, Arrow(sg3));

  put_in_plane(plane, 
	       Text(sg4.get_tgt_point(), 
		    gnu::autosprintf("m4 = %.2f", sg4.slope())));
  put_in_plane(plane, Arrow(sg4));
# endif
  //  put_in_plane(plane, seg);
  //  put_in_plane(plane, inter);

  //  put_in_plane(plane, Arrow(Point(20, 20), Point(400, 500)));

  //  put_in_plane(plane, Thick_Segment(Point(19,100), Point(582, 267)));

  //  put_in_plane(plane, Thick_Arrow(Point(19,100), Point(82, 267)));

  //  put_in_plane(plane, Dotted_Segment(Point(190, 100), Point(820, 267)));

  //  put_in_plane(plane, Dash_Segment(Point(200, 190), Point(267, 420)));

  //  put_in_plane(plane, Arrow_Dash_Segment(Point(267, 820), Point(10,190)));

  //  put_in_plane(plane, Arrow_Dotted_Segment(Point(67, 20), Point(100,190)));

  //  plane.compute_extreme_points();

  Polygon poly = make_random_polygon(13, 0, 2000, 0, 1400);

  //  put_in_plane(plane, Spline(poly));

  //  put_in_plane(plane, Spline_Arrow(poly));

  //  put_in_plane(plane, Polygon_With_Arrows(poly));

  put_in_plane(plane, poly);

  //  put_in_plane(plane, Shade_Polygon(poly));

  //  put_in_plane(plane, Regular_Polygon(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, Regular_Polygon_With_Points(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, Dotted_Regular_Polygon(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, Dash_Regular_Polygon(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, 
  //	       Dotted_Regular_Polygon_With_Points(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, 
  //	       Dash_Regular_Polygon_With_Points(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, Shade_Regular_Polygon(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, 
  //	       Shade_Regular_Polygon_With_Points(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, 
  //	       Shade_Regular_Polygon_With_Arrows(Point(0, 0), 400, 12, PI/3));

  //  put_in_plane(plane, Eepic_Spline(Point(-100, -200), 400, 7, PI/3));

  Regular_Polygon reg_poly(Point(200, 790), 300, 5);

  //  put_in_plane(plane, reg_poly);

  //  Polygon p(reg_poly);

  //  put_in_plane(plane, Polygon(p));

  Segment linea1(Point(100, 100), Point(400, 200));

  Segment perp1 = linea1.mid_perpendicular(linea1.size()/6);

  put_in_plane(plane, Arrow(linea1));

  put_in_plane(plane, Thick_Arrow(perp1));

  put_in_plane(plane, perp1.get_src_point());

  put_in_plane(plane, perp1.get_tgt_point());

  Segment linea2(Point(100, 100), Point(400, -200));

  Segment perp2 = linea2.mid_perpendicular(linea2.size()/6);

  put_in_plane(plane, Arrow(linea2));

  put_in_plane(plane, Thick_Arrow(perp2));

  put_in_plane(plane, perp2.get_src_point());

  put_in_plane(plane, perp2.get_tgt_point());

  Segment linea3(Point(100, 100), Point(-400, -200));

  Segment perp3 = linea3.mid_perpendicular(linea3.size()/6);

  put_in_plane(plane, Arrow(linea3));

  put_in_plane(plane, Thick_Arrow(perp3));

  put_in_plane(plane, perp3.get_src_point());

  put_in_plane(plane, perp3.get_tgt_point());

  Segment linea4(Point(100, 100), Point(-400, 200));

  Segment perp4 = linea4.mid_perpendicular(linea4.size()/6);

  put_in_plane(plane, Arrow(linea4));

  put_in_plane(plane, Thick_Arrow(perp4));

  put_in_plane(plane, perp4.get_src_point());

  put_in_plane(plane, perp4.get_tgt_point());

  //  put_in_plane(plane, Spline(Regular_Polygon(Point(-300, -300), 300, 4)));

  // put_in_plane(plane, Spline_Arrow(Regular_Polygon(Point(-300, 300), 300, 4)));

  put_in_plane(plane, Polygon(reg_poly));

  put_in_plane(plane, Ellipse(Point(-100, -100), 200, 100));

  //  put_in_plane(plane, Shade_Ellipse(el)); 

  ofstream output1("test-1.eepic", ios::trunc);

  plane.put_cartesian_axis();

  plane.draw(output1);

  ofstream output2("test-2.eepic", ios::trunc);

  plane.zoom(0.25);
  plane.draw(output2);

  
}
