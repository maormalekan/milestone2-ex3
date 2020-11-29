// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

// ------------ DO NOT CHANGE -----------
class Point{
public:
	float x,y;
	Point(float x,float y):x(x),y(y){}
};

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// --------------------------------------

// created a class of line that will help in cases of finding the circle by given 3 points
class Line {
public:
  float a, b;
  Line() : a(0), b(0){};
  Line(float a, float b) : a(a), b(b) {}
  float f(float x) { return a * x + b; }
  // finding the intersection point between 2 lines
  Point intersection(Line l1){
	  float x= this->a-l1.a;
	  float w= this->b-l1.b;
	  float xPoint=(w*-1)/x;
	  float yPoint=f(xPoint);
	  return Point(xPoint,yPoint);
  }
};
/**
 * @brief return the distance of 2 points
 * 
 * @param p1 point
 * @param p2 point
 * @return float distance
 */
float distanceBetweenTwoPoints(const Point& p1, const Point& p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

Circle getCircleFrom2(const Point& p1, const Point& p2){
	Point center= {(p1.x+p2.x)/2, (p1.y+p2.y)/2};
	return Circle(center,distanceBetweenTwoPoints(p1,p2)/2);
}

Circle getCircleFrom3(const Point& p1, const Point& p2, const Point& p3){
	// cases of slopes of 0 and indefinite - the center of the circle must lie in the 
	// middle of the diameter
	if(p2.x-p1.x==0){
		if(p3.y-p1.y==0)
			return Circle(Point((p2.x+p3.x)/2,(p2.y+p3.y)/2),distanceBetweenTwoPoints(p2,p3)/2);
		if(p3.y-p2.y==0)
			return Circle(Point((p1.x+p3.x)/2,(p1.y+p3.y)/2),distanceBetweenTwoPoints(p1,p3)/2);
	}
	if(p3.x-p1.x==0){
		if(p2.y-p1.y==0)
			return Circle(Point((p2.x+p3.x)/2,(p2.y+p3.y)/2),distanceBetweenTwoPoints(p2,p3)/2);
		if(p3.y-p2.y==0)
			return Circle(Point((p1.x+p2.x)/2,(p1.y+p2.y)/2),distanceBetweenTwoPoints(p1,p2)/2);
	}
	if(p3.x-p2.x==0){
		if(p2.y-p1.y==0)
			return Circle(Point((p1.x+p3.x)/2,(p1.y+p3.y)/2),distanceBetweenTwoPoints(p1,p3)/2);
		if(p3.y-p1.y==0)
			return Circle(Point((p1.x+p2.x)/2,(p1.y+p2.y)/2),distanceBetweenTwoPoints(p1,p2)/2);
	}
	// building two lines and building two vertical lines to the middle of the lines.
	// the center of the circle must lie in the intersection point between the vertical lines
	// according to a theorem - in a circle that blocks triangle, the center of the circle is the 
	// meeting point of the vertical lines to each edge.

	float m= (p2.y-p1.y)/(p2.x-p1.x);
	// y-y1=m(x-x1) --> y=mx-mx1+y1
	float mx1= p1.x*m;
	float b= mx1*-1+p1.y;
	Line l1(m,b);
	float mVertical1= -1/m;
	Point middle1={(p1.x+p2.x)/2,(p1.y+p2.y)/2};
	// y-middle1.y=m(x-middle1.x)--> y=mx-m*middle1.x+middle1.y
	float b1=mVertical1*-1*middle1.x +middle1.y;
	// vertical line to the line of p1 and p2
	Line vertical1(mVertical1,b1);
	float m2=(p3.y-p1.y)/(p3.x-p1.x);
	// y-y1=m(x-x1) -->y=mx-mx1+y1
	mx1= p1.x*m2;
	b= mx1*-1+p1.y;
	Line l2(m2,b);
	float mVertical2=-1/m2;
	Point middle2={(p1.x+p3.x)/2,(p1.y+p3.y)/2};
	// y-middle2.y=m(x-middle2.x)--> y=mx-m*middle2.x+middle2.y
	float b2= mVertical2*-1*middle2.x+middle2.y;
	// vertical line to the line of p1 and p3
	Line vertical2(mVertical2,b2);
	// finding the center of the circle
	Point center=vertical1.intersection(vertical2);
	// finding the radius
	float R=distanceBetweenTwoPoints(p1,center);
	return Circle(center,R);

}
bool pIsInside(const Circle& c, const Point& p){
	return (distanceBetweenTwoPoints(c.center,p)<=c.radius);
}

Circle minCircle(vector<Point> vec){
		if(vec.size()<=3){
			if(vec.empty())
				return Circle(Point(0,0),0);
			if(vec.size()==1)
				return Circle(vec[0] ,0);
			if(vec.size()==2)
				return getCircleFrom2(vec[0],vec[1]);
					
		}

		// check if we can create a circle by 2 points only
		for(int i=0;i<3;i++){
			for(int j=i+1;j<3;j++){
				Circle circ=getCircleFrom2(vec[i],vec[j]);
				bool flag=true;
				for(int k=0;k<vec.size();k++){
					if(!pIsInside(circ,vec[k])){
						flag=false;
						break;
					}
				}
				if(flag)
					return circ;
			}
		}
      	return getCircleFrom3(vec[0],vec[1],vec[2]);	

}

Circle helper(Point** points,size_t size, vector<Point> vec){
		if(size==0||vec.size()==3){
			return minCircle(vec);	
		}

		Point p= {points[size-1]->x,points[size-1]->y};
		Circle c= helper(points,size-1,vec);
		if(pIsInside(c,p))
			return c;

		// else, the point must be on the edge of the circle
		vec.push_back(p);

		return helper(points,size-1,vec);



}


Circle findMinCircle(Point** points,size_t size){
	return helper(points,size,{});
	
}





#endif /* MINCIRCLE_H_ */
