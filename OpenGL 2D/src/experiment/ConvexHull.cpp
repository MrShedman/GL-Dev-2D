#include "ConvexHull.hpp"


namespace
{
	float DecisionDistance(Point p, const std::vector<Point> & points)
	{
		Point result = points[0];
		double dst = p.Distance(points[0]);
		for (int i = 1; i < points.size(); i++) {
			Point cur = points[i];
			double curDistance = p.Distance(cur);
			if (curDistance < dst) {
				result = cur;
				dst = curDistance;
			}
		}
		return dst;
	}
}

HullFinder::HullFinder()
{
}

float HullFinder::IsLeft(Point p0, Point p1, Point p2)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool HullFinder::IsPointInsidePolygon(Point v, const std::vector<Point> & polygon)
{
	bool result = false;
	int j = polygon.size() - 1;
	for (int i = 0; i < polygon.size(); i++)
	{
		if ((polygon[i].y < v.y && polygon[j].y > v.y) || (polygon[j].y < v.y && polygon[i].y > v.y))
		{
			if (polygon[i].x + (v.y - polygon[i].y) / (polygon[j].y - polygon[i].y) * (polygon[j].x - polygon[i].x) < v.x)
			{
				result = !result;
			}
		}
		j = i;
	}
	return result;
}

bool HullFinder::CheckEdgeIntersection(const Point & p0, const Point & p1, const Point & p2, const Point & p3)
{
	float s1_x = p1.x - p0.x;
	float s1_y = p1.y - p0.y;
	float s2_x = p3.x - p2.x;
	float s2_y = p3.y - p2.y;
	float s = (-s1_y * (p0.x - p2.x) + s1_x * (p0.y - p2.y)) / (-s2_x * s1_y + s1_x * s2_y);
	float t = (s2_x * (p0.y - p2.y) - s2_y * (p0.x - p2.x)) / (-s2_x * s1_y + s1_x * s2_y);
	return (s > 0 && s < 1 && t > 0 && t < 1);
}

bool HullFinder::CheckEdgeIntersection(const std::vector<Point> & hull, Point curEdgeStart, Point curEdgeEnd, Point checkEdgeStart, Point checkEdgeEnd)
{
	for (int i = 0; i < hull.size() - 2; i++) {
		int e1 = i;
		int e2 = i + 1;
		Point p1 = hull[e1];
		Point p2 = hull[e2];

		if (curEdgeStart == p1 && curEdgeEnd == p2) {
			continue;
		}

		if (CheckEdgeIntersection(checkEdgeStart, checkEdgeEnd, p1, p2)) {
			return true;
		}
	}
	return false;
}

Point HullFinder::NearestInnerPoint(Point edgeStart, Point edgeEnd, const std::vector<Point> &points, const std::vector<Point> &hull, bool * found)
{
	Point result;
	float distance = 0;
	*found = false;

	for(auto &p : points) {
		// Skip points that are already in he hull
		if (std::find(hull.begin(), hull.end(), p) != hull.end()) {
			continue;
		}
		/*if (!IsPointInsidePolygon(p, hull)) {
		continue;
		}*/

		float d = p.Distance(edgeStart, edgeEnd);
		bool skip = false;
		for (int i = 0; !skip && i < hull.size() - 1; i++) 
		{
			double dTmp = p.Distance(hull[i], hull[i + 1]);
			skip |= dTmp < d;
		}
		if (skip)
		{
			continue;
		}

		if (!(*found) || distance > d)
		{
			result = p;
			distance = d;
			*found = true;
		}
	}
	return result;
}

std::vector<Point> HullFinder::FindConvexHull(const std::vector<Point> & points)
{
	std::vector<Point> P = points;
	std::vector<Point> H;

	// Sort P by x and y
	for (int i = 0; i < P.size(); i++) {
		for (int j = i + 1; j < P.size(); j++) {
			if (P[j].x < P[i].x || (P[j].x == P[i].x && P[j].y < P[i].y))
			{
				Point tmp = P[i];
				P[i] = P[j];
				P[j] = tmp;
			}
		}
	}

	// the output array H[] will be used as the stack
	int i;                 // array scan index

	// Get the indices of points with min x-coord and min|max y-coord
	int minmin = 0, minmax;
	float xmin = P[0].x;
	for (i = 1; i < P.size(); i++)
		if (P[i].x != xmin) break;
	minmax = i - 1;
	if (minmax == P.size() - 1) 
	{       // degenerate case: all x-coords == xmin
		H.push_back(P[minmin]);
		if (P[minmax].y != P[minmin].y) // a  nontrivial segment
			H.push_back(P[minmax]);
		H.push_back(P[minmin]);            // add polygon endpoint
		return H;
	}

	// Get the indices of points with max x-coord and min|max y-coord
	int maxmin, maxmax = P.size() - 1;
	float xmax = P.back().x;
	for (i = P.size() - 2; i >= 0; i--)
		if (P[i].x != xmax) break;
	maxmin = i + 1;

	// Compute the lower hull on the stack H
	H.push_back(P[minmin]);      // push  minmin point onto stack
	i = minmax;
	while (++i <= maxmin)
	{
		// the lower line joins P[minmin]  with P[maxmin]
		if (IsLeft(P[minmin], P[maxmin], P[i]) >= 0 && i < maxmin)
			continue;           // ignore P[i] above or on the lower line

		while (H.size() > 1)         // there are at least 2 points on the stack
		{
			// test if  P[i] is left of the line at the stack top
			if (IsLeft(H[H.size() - 2], H.back(), P[i]) > 0)
				break;         // P[i] is a new hull  vertex
			H.pop_back();         // pop top point off  stack
		}
		H.push_back(P[i]);        // push P[i] onto stack
	}

	// Next, compute the upper hull on the stack H above  the bottom hull
	if (maxmax != maxmin)      // if  distinct xmax points
		H.push_back(P[maxmax]);  // push maxmax point onto stack
	int bot = H.size();                  // the bottom point of the upper hull stack
	i = maxmin;
	while (--i >= minmax)
	{
		// the upper line joins P[maxmax]  with P[minmax]
		if (IsLeft(P[maxmax], P[minmax], P[i]) >= 0 && i > minmax)
			continue;           // ignore P[i] below or on the upper line

		while (H.size() > bot)     // at least 2 points on the upper stack
		{
			// test if  P[i] is left of the line at the stack top
			if (IsLeft(H[H.size() - 2], H.back(), P[i]) > 0)
				break;         // P[i] is a new hull  vertex
			H.pop_back();         // pop top point off stack
		}
		H.push_back(P[i]);        // push P[i] onto stack
	}
	if (minmax != minmin)
		H.push_back(P[minmin]);  // push  joining endpoint onto stack

	return H;
}

std::vector<Point> HullFinder::FindConcaveHull(const std::vector<Point> & points, float N)
{
	std::vector<Point> concaveList = FindConvexHull(points);

	for (int i = 0; i < concaveList.size() - 1; i++) {
		// Find the nearest inner point pk ? G from the edge (ci1, ci2);
		Point ci1 = concaveList[i];
		Point ci2 = concaveList[i + 1];

		bool found;
		Point pk = NearestInnerPoint(ci1, ci2, points, concaveList, &found);
		if (!found || std::find(concaveList.begin(), concaveList.end(), pk) != concaveList.end()) 
		{
			continue;
		}

		double eh = ci1.Distance(ci2);  // the lenght of the edge
		std::vector<Point> tmp;
		tmp.push_back(ci1);
		tmp.push_back(ci2);
		double dd = DecisionDistance(pk, tmp);

		if (eh / dd > N) {
			// Check that new candidate edge will not intersect existing edges.
			bool intersects = CheckEdgeIntersection(concaveList, ci1, ci2, ci1, pk);
			intersects |= CheckEdgeIntersection(concaveList, ci1, ci2, pk, ci2);
			if (!intersects) {
				concaveList.insert(concaveList.begin() + i + 1, pk);
				i--;
			}
		}
	}
	return concaveList;
}
