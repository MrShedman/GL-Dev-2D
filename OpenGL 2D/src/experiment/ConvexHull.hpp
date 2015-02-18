#pragma once

#include "..\math\Vector.h"

#include <vector>

typedef Vector3f Point;

class HullFinder
{
public:

	HullFinder();

	static float IsLeft(Point p0, Point p1, Point p2);
	static bool IsPointInsidePolygon(Point v, const std::vector<Point> & polygon);
	static bool CheckEdgeIntersection(const Point & p1, const Point & p2, const Point & p3, const Point & p4);
	static bool CheckEdgeIntersection(const std::vector<Point> & hull, Point curEdgeStart, Point curEdgeEnd, Point checkEdgeStart, Point checkEdgeEnd);
	static Point NearestInnerPoint(Point edgeStart, Point edgeEnd, const std::vector<Point> & points, const std::vector<Point> & hull, bool * found);
	static std::vector<Point> FindConvexHull(const std::vector<Point> & points);
	static std::vector<Point> FindConcaveHull(const std::vector<Point> & points, float N);
};