#include "ofMain.h"
#include "Wall.h"

#ifndef THRESHOLD
#define THRESHOLD 0.000001f
#endif

#define TWOPI 6.283185307179586476925287
#define RTOD 57.2957795

int Wall::insertCorner(Common::CVector3 _corner)
{
	return insertCorner(_corner.x, _corner.y, _corner.z);
}


int Wall::insertCorner(float _x, float _y, float _z)
{
	Common::CVector3 tempCorner(_x, _y, _z);

	if (polygon.size() < 3)
	{
		polygon.push_back(tempCorner);
		if (polygon.size() == 3)
		{
			calculate_ABCD();
			return 1;
		}
	}
	else
	{
		double diff = _x * A + _y * B + _z * C + D;
		diff = fabs(diff);
	    if (diff < THRESHOLD) // �DBL_EPSILON? �THRESHOLD?
	    {
			polygon.push_back(tempCorner);
			return 1;
	    }
	    else 
		{
			tempCorner = getPointProjection(_x, _y, _z);
			polygon.push_back(tempCorner);
			return 0;
	    }
	}
}

vector<Common::CVector3> Wall::getCorners()
{
	return polygon;
}

void Wall::setAbsortion(float _absortion)
{
	absortion = _absortion;
}

Common::CVector3 Wall::getNormal()
{
	//Common::CVector3 normal, p1, p2; 
	Common::CVector3 p1, p2, normal;
	float modulus;

	p1 = polygon.at(1) - polygon.at(0);
	p2 = polygon.at(2) - polygon.at(0);

	normal = p1.CrossProduct(p2);

	modulus = normal.GetDistance();

	normal.x = normal.x / modulus;
	normal.y = normal.y / modulus;
	normal.z = normal.z / modulus;
		
	return normal;
}

Common::CVector3 Wall::getCenter()
{
	Common::CVector3 center;

	center = Common::CVector3::ZERO;

	for (auto i = 0; i < polygon.size(); i++) 
	{
		center.x += polygon.at(i).x;
		center.y += polygon.at(i).y;
		center.z += polygon.at(i).z;
	}
	center.x /= polygon.size();
	center.y /= polygon.size();
	center.z /= polygon.size();

	return center;
	 
}

Common::CVector3 Wall::getPointProjection(Common::CVector3 point)
{
	return getPointProjection(point.x, point.y, point.z);
}

Common::CVector3 Wall::getPointProjection(float x0, float y0, float z0)
{
	// Vectorial Ec. of straight line --> (X,Y,Z) = (x0, y0, z0) + lambda (normalV.x, normalV.y, normalV.z)
	// Plane of the wall              --> AX+BY+CZ+D = 0

	Common::CVector3 normalV, point(x0, y0, z0);
	double rX1, rY1, rZ1, lambda;
	double rX2, rY2, rZ2;
	double diff1, diff2;
	float rX, rY, rZ;
	
	calculate_ABCD();
	normalV = getNormal();
	lambda =(double) getDistanceFromPoint(point);

	// lambda could be positive or negative
	// 
	rX1 = x0 + lambda * normalV.x;
	rY1 = y0 + lambda * normalV.y;
	rZ1 = z0 + lambda * normalV.z;
	diff1 = rX1 * A + rY1 * B + rZ1 * C + D;
	diff1 = fabs(diff1);

	rX2 = x0 - lambda * normalV.x;
	rY2 = y0 - lambda * normalV.y;
	rZ2 = z0 - lambda * normalV.z;
	diff2 = rX2 * A + rY2 * B + rZ2 * C + D;
	diff2 = fabs(diff2);
	
	if (diff1 < diff2)
	{
		rX = rX1;
		rY = rY1;
		rZ = rZ1;
	}
	else
	{
		rX = rX2;
		rY = rY2;
		rZ = rZ2;
	}
		   
	return Common::CVector3(rX, rY, rZ);
}

float Wall::getDistanceFromPoint(Common::CVector3 point)
{
	float distance;
	calculate_ABCD();
	distance = fabs(A*point.x + B * point.y + C * point.z + D);
	distance = distance / sqrtf(A * A + B * B + C * C);
	return distance;
}

Common::CVector3 Wall::getImagePoint(Common::CVector3 point)
{
	float distance;
	Common::CVector3 imagePoint, normalRay;
	distance = getDistanceFromPoint(point);

	normalRay = getNormal();
	normalRay.x *= -(2*distance);
	normalRay.y *= -(2*distance);
	normalRay.z *= -(2*distance);

	imagePoint = point + normalRay;

	return imagePoint;
}

Wall Wall::getImageWall(Wall _wall)
{
	Wall tempWall;
	vector<Common::CVector3> corners = _wall.getCorners();
	for(int i = corners.size()-1; i >= 0; i--)
	{
		Common::CVector3 tempImageCorner = getImagePoint(corners.at(i));
		tempWall.insertCorner(tempImageCorner);
	}
	return tempWall;
}

Common::CVector3 Wall::getIntersectionPointWithLine(Common::CVector3 p1, Common::CVector3 p2)
{
	Common::CVector3 cutPoint, vecLine;
	float modulus, lambda;

	vecLine = p2 - p1;
	
	lambda = (- D - (A *p1.x + B * p1.y + C * p1.z));
	lambda = lambda / (A*vecLine.x + B * vecLine.y + C * vecLine.z);

	cutPoint.x = p1.x + lambda * vecLine.x;
	cutPoint.y = p1.y + lambda * vecLine.y;
	cutPoint.z = p1.z + lambda * vecLine.z;

	modulus = getDistanceFromPoint(cutPoint); // must be = ZERO
	
	return cutPoint;
}

bool  Wall::checkPointInsideWall(Common::CVector3 point)
{
	float modulus = getDistanceFromPoint(point);
	if (modulus > THRESHOLD) return FALSE;        // Point is not in the wall

	double m1, m2, anglesum=0, costheta, anglediff;
	Common::CVector3 p1, p2;
	int n = polygon.size();
		
	for (auto i = 0; i<n; i++) 
	{
		p1.x = polygon[i].x - point.x;
		p1.y = polygon[i].y - point.y;
		p1.z = polygon[i].z - point.z;
		p2.x = polygon[(i + 1) % n].x - point.x;
		p2.y = polygon[(i + 1) % n].y - point.y;
		p2.z = polygon[(i + 1) % n].z - point.z;
		m1 = p1.GetDistance();
		m2 = p2.GetDistance();
		if (m1*m2 <= THRESHOLD)
			return TRUE;                     // Point is on a corner of the wall,
		else
			costheta = (p1.x*p2.x + p1.y*p2.y + p1.z*p2.z) / (m1*m2);

		anglesum += acos(costheta);
    }
	anglediff = fabs(TWOPI - anglesum);
    if (anglediff < THRESHOLD)
		return TRUE;
	else
		return FALSE;
}

void Wall::calculate_ABCD()
{
	Common::CVector3 normal;
	normal = getNormal();
	A = normal.x;
	B = normal.y;
	C = normal.z;
	D = -(A * polygon.at(2).x + B * polygon.at(2).y + C * polygon.at(2).z);
}

void Wall::draw()
{
	int numberVertex=polygon.size();
	for(int i=0; i<numberVertex-1; i++)
	{
		ofLine(polygon[i].x, polygon[i].y, polygon[i].z,
			polygon[i + 1].x, polygon[i + 1].y, polygon[i+1].z);
	}
	ofLine(polygon[0].x, polygon[0].y, polygon[0].z,
		polygon[numberVertex-1].x, polygon[numberVertex-1].y, polygon[numberVertex-1].z);
}

void Wall::drawNormal(float length)
{
	Common::CVector3 center;
	Common::CVector3 normalEnd;
	Common::CVector3 normal;
	center = getCenter();
	normal = getNormal();
	normal.x *= length;
	normal.y *= length;
	normal.z *= length;

	normalEnd = center + normal;
	ofLine(center.x, center.y, center.z, 
		normalEnd.x, normalEnd.y, normalEnd.z);
}
