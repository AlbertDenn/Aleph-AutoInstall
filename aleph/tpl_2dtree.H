
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

# ifndef TPL_2DTREE_H
# define TPL_2DTREE_H

# include <point.H>
# include <htlist.H>
# include <tpl_nodePool.H>

    template <typename T = Empty_Class>
class K2Tree
{
  struct Node 
  {
    Point point; // the point
    Rectangle rect; // the axis-aligned rectangle corresponding to this node
    Node * lb; // the left/bottom subtree
    Node * rt; // the right/top subtree

    Node(const Point & __point) : point(__point), lb(nullptr), rt(nullptr)
    { 
      // empty
    }

    void set_rect(const Geom_Number & xmin, const Geom_Number & ymin, 
		  const Geom_Number & xmax, const Geom_Number & ymax) 
    {
      rect.set_rect(xmin, ymin, xmax, ymax);
    }

    void set_rect(const Point & pmin, const Point & pmax)
    {
      rect.set_rect(pmin.get_x(), pmin.get_y(), pmax.get_x(), pmax.get_y());
    }

    const Geom_Number & xmin() const { return rect.get_xmin(); }
    const Geom_Number & ymin() const { return rect.get_ymin(); }
    const Geom_Number & xmax() const { return rect.get_xmax(); }
    const Geom_Number & ymax() const { return rect.get_ymax(); }
    const Geom_Number & x() const { return point.get_x(); }
    const Geom_Number & y() const { return point.get_y(); }
  };

  Point pmin;  // esquina inferior izquierda del rectángulo de referencia
  Point pmax;  // esquina superior derecha del rectángulo de referencia
  size_t N;    // número de puntos que maneja el árbol
  Node * root; // raíz del árbol

  static const size_t dim = 19;
  Node_Pool<Node> node_pool;

public:

  K2Tree() : pmin(0, 0), pmax(0, 0), node_pool(dim) { /* empty */ }

  K2Tree(const Point & __pmin, const Point & __pmax) 
    : pmin(__pmin), pmax(__pmax), node_pool(dim)
  {
    /* empty */ 
  }

  K2Tree(const Geom_Number & xmin, const Geom_Number & ymin, 
	 const Geom_Number & xmax, const Geom_Number & ymax) 
    : pmin(xmin, ymin), pmax(xmax, ymax), node_pool(dim)
  {
    /* empty */ 
  }

  bool is_empty() const { return N == 0; }

  size_t size() const { return N; }

private:

  static Node * lr_insert(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return new Point (p);

    Node * ret_val = nullptr;
    if (p.get_x() == root->x()) 
      {
	if (p.get_y() == root->y())
	  return nullptr; // duplicated point

	ret_val = bu_insert(root->rt, p);
	if (ret_val != nullptr) 
	  {
	    root->rt = ret_val; // node was inserted
	    root->rt->set_rect(root->x(), root->ymin(), 
			       root->xmax(), root->ymax());

	    return root;
	  }

	return nullptr;
      }

    if (p.get_x() < root->x())
      {
	ret_val = bu_insert(root->lb, p);
	if (ret_val != nullptr) 
	  {
	    root->lb = ret_val;
	    root->lb->set_rect(root->xmin(), root->ymin(), 
			       root->x(), root->ymax());

	    return root;
	  } 
	else
	  return nullptr;
      }

    ret_val = bu_insert(root->rt, p);
    if (ret_val != nullptr) 
      {
	root->rt = ret_val;
	root->rt->set_rect(root->x(), root->ymin(), 
			   root->xmax(), root->ymax());

	return root;
      }

    return nullptr;
  }

  static Node * bu_insert(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return new Point(p);

    Node * ret_val = nullptr;
    if (p.get_y() == root->y()) 
      {
	if (p.get_x() == root->x())
	  return nullptr; // duplicated point

	ret_val = lr_insert(root->rt, p);
	if (ret_val != nullptr) 
	  {
	    root->rt = ret_val; // node was inserted
	    root->rt->set_rect(root->xmin(), root->y(), 
			       root->xmax(), root->ymax());

	    return root;
	  }
	
	return nullptr;
      }

    if (p.get_y() < root->y()) 
      {
	ret_val = lr_insert(root->lb, p);
	if (ret_val != nullptr) 
	  {
	    root->lb = ret_val;
	    root->lb->set_rect(root->xmin(), root->ymin(), 
			       root->xmax(), root->y());
	    return root;
	  } 
	else
	  return nullptr;
      }

    ret_val = lr_insert(root->rt, p);
    if (ret_val != nullptr) 
      {
	root->rt = ret_val;
	root->rt->set_rect(root->xmin(), root->y(), 
			   root->xmax(), root->ymax());
	
	return root;
    }

    return nullptr;
  }

public:

  Point * insert(const Point & p) 
  {
    if (root == nullptr) 
      {
	root = new Node (p);
	root->set_rect(pmin, pmax);
	N++;

	return &root->point;
      }

    Node * ret = lr_insert(root, p);
    if (ret == nullptr)
      return nullptr;

    N++;

    return nullptr;
  }

private:

  static Node * bu_search(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return nullptr;

    if (root->y() == p.get_y()) 
      {
	if (root->x() == p.get_x())
	  return root;

	return lr_search(root->rt, p);
      }

    if (p.get_y() < root->y())
      return lr_search(root->lb, p);
    else
      return lr_search(root->rt, p);
  }

  static Node * lr_search(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return nullptr;

    if (root->x() == p.get_x()) 
      {
	if (root->y() == p.get_y())
	  return root;

	return bu_search(root->rt, p);
      }

    if (p.get_x() < root->x())
      return bu_search(root->lb, p);
    else
      return bu_search(root->rt, p);
  }

public:

  bool contains(const Point & p) 
  {
    return lr_search(root, p) != nullptr;
  }

private:

  static void range(Node * root, const Rectangle & rect, DynList<Point> * q) 
  {
    if (root == nullptr)
      return;
    
    if (not root->rect.intersects(rect))
      return;
    
    const Point & point = root->point;
    if (rect.contains(point))
        q->append(point);
    
    range(root->lb, rect, q);
    range(root->rt, rect, q);
  }

public:

  void range(const Rectangle & rect, DynList<Point> * l)
  { 
    if (N == 0)
      return;
    
    range(root, rect, l);
  }

private:

  Node * min_node;
  Geom_Number min_dist2;

  void lr_nearest(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return;
    
    if (root->rect.distance_squared_to(p) > min_dist2)
      return;
    
    Geom_Number d2 = root->point.distance_squared_to(p); // es con el rectángulo
    if (d2 < min_dist2) 
      {
	min_dist2 = d2;
	min_node = root;
      }
    
    if (p.get_x() < root->x()) // is p to left of this
      { 
	bu_nearest(root->lb, p);
	bu_nearest(root->rt, p);
      } 
    else 
      {
	bu_nearest(root->rt, p);
	bu_nearest(root->lb, p);
      }
  }

  void bu_nearest(Node * root, const Point & p) 
  {
    if (root == nullptr)
      return;
    
    if (root->rect.distance_squared_to(p) > min_dist2)
      return;
    
    Geom_Number d2 = root->point.distance_squared_to(p);
    if (d2 < min_dist2) 
      {   
	min_dist2 = d2;
	min_node = root;
      }
        
    if (p.get_y() < root->y())  // is p to below this?
      {
	lr_nearest(root->lb, p);
	lr_nearest(root->rt, p);
    } 
    else 
      {
	lr_nearest(root->rt, p);
	lr_nearest(root->lb, p);
      }
  }

public:

  Point nearest(const Point & p) 
  { 
    if (N == 0)
      return NullPoint;
    
    min_dist2 = 1.0;
    lr_nearest(root, p);
    
    return min_node->point;
  }
};

# endif // TPL_2DTREE_H
