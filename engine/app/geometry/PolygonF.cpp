/** /////////////////////////////////////////////////////////////////////////////////
//
// @description Eyedrop
// @about       C++ game engine built on Sokol
// @author      Stephens Nunnally <@stevinz>
// @license     MIT - Copyright (c) 2021 Stephens Nunnally and Scidian Studios
// @source      https://github.com/scidian/eyedrop
//
///////////////////////////////////////////////////////////////////////////////////*/

//
// DrPolygonF::isInside() adapted from:
//      https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
//
//
#include <math.h>

#include "../core/Math.h"
#include "PointF.h"
#include "PolygonF.h"


//####################################################################################
//##    Constructors
//####################################################################################
DrPolygonF::DrPolygonF() {
    m_points.clear();
}

DrPolygonF::DrPolygonF(const DrPolygonF& polygon) {
    m_points.clear();
    for (auto point : polygon.m_points) {
        m_points.push_back(point);
    }
}


//####################################################################################
//##    Manangement
//####################################################################################
void DrPolygonF::addPoint(DrPointF point) {
    m_points.push_back(point);
}


//####################################################################################
//##    Polygon Functions
//####################################################################################
// Checks if point lies on line segment made from line_a and line_b
bool DrPolygonF::onSegment(DrPointF line_a, DrPointF point, DrPointF line_b) {
    return (point.x <= Max(line_a.x, line_b.x) && point.x >= Min(line_a.x, line_b.x) &&
            point.y <= Max(line_a.y, line_b.y) && point.y >= Min(line_a.y, line_b.y));
}

// Finds orientation of three points (p, q, r).
// RETURNS:
//      0 - Fall along same segment
//      1 - Clock-wise
//      2 - Counterclock-wise
Winding_Orientation DrPolygonF::orientation(DrPointF p, DrPointF q, DrPointF r) {
    double value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (FuzzyCompare(value, 0.0)) return DROP_WINDING_LINE_SEGMENT;                         // On a line
    return ((value > 0) ? DROP_WINDING_CLOCKWISE : DROP_WINDING_COUNTERCLOCKWISE);          // Clock-wise or Counterclock-wise
}

// Makes sure points are in the desired Winding Orientation
void DrPolygonF::ensureWindingOrientation(std::vector<DrPointF>& points, Winding_Orientation direction_desired) {
    Winding_Orientation winding = findWindingOrientation(points);
    if ((winding == DROP_WINDING_CLOCKWISE        && direction_desired == DROP_WINDING_COUNTERCLOCKWISE) ||
        (winding == DROP_WINDING_COUNTERCLOCKWISE && direction_desired == DROP_WINDING_CLOCKWISE))
    {
        std::reverse(points.begin(), points.end());
    }
}

// Returns winding direction of points
Winding_Orientation DrPolygonF::findWindingOrientation(const std::vector<DrPointF>& points) {
    size_t i1, i2;
    double area = 0;
    for (i1 = 0; i1 < points.size(); i1++) {
        i2 = i1 + 1;
        if (i2 == points.size()) i2 = 0;
        area += points[i1].x * points[i2].y - points[i1].y * points[i2].x;
    }
    if (area > 0) return DROP_WINDING_COUNTERCLOCKWISE;
    if (area < 0) return DROP_WINDING_CLOCKWISE;
    return DROP_WINDING_LINE_SEGMENT;
}



// RETURNS true if line segment 'p1q1' and 'p2q2' intersect
bool DrPolygonF::doIntersect(DrPointF p1, DrPointF q1, DrPointF p2, DrPointF q2) {
    // Find the four orientations needed for general and special cases
    Winding_Orientation o1 = orientation(p1, q1, p2);
    Winding_Orientation o2 = orientation(p1, q1, q2);
    Winding_Orientation o3 = orientation(p2, q2, p1);
    Winding_Orientation o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) return true;

    // Special Cases
    if (o1 == DROP_WINDING_LINE_SEGMENT && onSegment(p1, p2, q1)) return true;               // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o2 == DROP_WINDING_LINE_SEGMENT && onSegment(p1, q2, q1)) return true;               // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o3 == DROP_WINDING_LINE_SEGMENT && onSegment(p2, p1, q2)) return true;               // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o4 == DROP_WINDING_LINE_SEGMENT && onSegment(p2, q1, q2)) return true;               // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    return false;                                                                                   // Doesn't fall in any of the above cases
}

// RETURNS true if the point p lies inside the polygon
bool DrPolygonF::isInside(DrPointF point) {
    if (numberOfPoints() < 3) return false;                         // There must be at least 3 vertices

    DrPointF extreme { DR_INFINITY, point.y };                      // Create a point for line segment from p to infinite
    point.x += 0.00005;                                             // Adjust for edge cases
    point.y += 0.00005;                                             // Adjust for edge cases

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do {
        int next = (i+1)%numberOfPoints();

        // Check if the line segment from 'p' to 'extreme' intersects with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(points()[i], points()[next], point, extreme)) {
            // If the point 'p' is colinear with line segment 'i-next', then check if it lies on segment, if it does, return true
            if (orientation(points()[i], point, points()[next]) == DROP_WINDING_LINE_SEGMENT)
                return onSegment(points()[i], point, points()[next]);
            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count&1;                                                 // Same as (count%2 == 1)
}







