// ID1	ID2

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>

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
class Line {
public:
  float a, b;
  Line() : a(0), b(0){};
  Line(float a, float b) : a(a), b(b) {}
  float f(float x) { return a * x + b; }
  Point intersection(Line l1){
	  float x= this->a-l1.a;
	  float w= this->b-l1.b;
	  float xPoint=(w*-1)/x;
	  float yPoint=f(xPoint);
	  return Point(xPoint,yPoint);
  }
};

float distanceBetweenTwoPoints(const Point& p1, const Point& p2){
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

Circle getCircleFrom2(const Point& p1, const Point& p2){
	Point center= {(p1.x+p2.x)/2, (p1.y+p2.y)/2};
	return Circle(center,distanceBetweenTwoPoints(p1,p2)/2);
}

Circle getCircleFrom3(const Point& p1, const Point& p2, const Point& p3){
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
	float m= (p2.y-p1.y)/(p2.x-p1.x);
	// y-y1=m(x-x1) --> y=mx-mx1+y1
	float mx1= p1.x*m;
	float b= mx1*-1+p1.y;
	Line l1(m,b);
	float mVertical= -1/m;
	// y-y3=m(x-x3)--> y=mx-mx3+y3
	float mx3= mVertical*p3.x;
	float b1=mx3*-1 +p3.y;
	Line vertical1(mVertical,b1);
	float m2=(p3.y-p1.y)/(p3.x-p1.x);
	// y-y1=m(x-x1) -->y=mx-mx1+y1
	mx1= p1.x*m2;
	b= mx1*-1+p1.y;
	Line l2(m2,b);
	float mVertical2=-1/m2;
	// y-y2=m(x-x2) --> y=mx-mx2+y2
	float mx2= mVertical2*p2.x;
	float b2= mx2*-1+p2.y;
	Line vertical2(mVertical2,b2);
	Point center= vertical1.intersection(vertical2);
	float R= distanceBetweenTwoPoints(p1,center);
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

			// return getCircleFrom3(vec[0],vec[1],vec[2]);	
					
		}
		for(int i=0;i<3;i++){
			for(int j=i+1;j<3;j++){
				Circle circ=getCircleFrom2(vec[i],vec[j]);
				bool flag=true;
				for(int k=0;k<vec.size();k++){
					if(!pIsInside(circ,vec[k]))
						flag=false;
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
