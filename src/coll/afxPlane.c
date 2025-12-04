/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxVector.h"
#include "qwadro/coll/afxPlane.h"
#include "qwadro/coll/afxSphere.h"

_AFXINL afxBool AfxPlaneIsEqual(afxPlane a, afxPlane b)
{
    // Should be compatible with XMPlaneEqual

    return (0 == AfxV4dDot(a.uvwd, b.uvwd));
}

_AFXINL afxBool AfxPlaneIsNearEqual(afxPlane a, afxPlane b, afxV4d epsilon)
{
    // Should be compatible with XMPlaneNearEqual

    afxPlane na = AfxPlaneNormalize(a);
    afxPlane nb = AfxPlaneNormalize(b);
    return AfxV4dDot(na.uvwd, nb.uvwd) > (epsilon[0] * epsilon[0]);
}

_AFXINL afxBool AfxPlaneIsNotEqual(afxPlane a, afxPlane b)
{
    // Should be compatible with XMPlaneNotEqual

    return AfxV4dDot(a.uvwd, b.uvwd) != 0;
}

_AFXINL afxBool AfxPlaneIsNaN(afxPlane p)
{
    // Should be compatible with XMPlaneIsNaN

    for (afxUnit i = 0; i < 4; ++i) if (isnan(p.uvwd[i])) return TRUE;
    return FALSE;
}

_AFXINL afxBool AfxPlaneIsInfinite(afxPlane p)
{
    // Should be compatible with XMPlaneIsInfinite

    for (afxUnit i = 0; i < 4; ++i) if (isinf(p.uvwd[i])) return TRUE;
    return FALSE;
}

_AFXINL afxReal AfxPlaneDot(afxPlane a, afxPlane b)
{
    // Should be compatible with XMPlaneDot

    return AfxV4dDot(a.uvwd, b.uvwd);
}

_AFXINL afxReal AfxPlaneDotCoord(afxPlane p, afxV3d v)
{
    // Should be compatible with XMPlaneDotCoord

    // Mask the W component to 1.
    return AfxPlaneDot(p, AFX_PLANE(v[0], v[1], v[2], 1));
}

_AFXINL afxReal AfxPlaneDotNormal(afxPlane p, afxV3d v)
{
    // Should be compatible with XMPlaneDotNormal

    return AfxV3dDot(p.uvwd, v);
}

_AFXINL afxPlane AfxPlaneNormalizeEst(afxPlane p)
{
    // Should be compatible with XMPlaneNormalizeEst

    afxPlane n;
    afxReal rlen = AfxV3dMagRecip(p.uvwd);
    AfxV4dScale(n.uvwd, p.uvwd, rlen);
    return n;
}

_AFXINL afxPlane AfxPlaneNormalize(afxPlane p)
{
    // Should be compatible with XMPlaneNormalize

    afxPlane n;
    afxReal len = AfxV3dMag(p.uvwd);
    if (len > 0) len = 1.f / len;
    AfxV4dScale(n.uvwd, p.uvwd, len);
    return n;
}

_AFXINL afxPlane AfxPlaneIntersectLine(afxPlane p, afxV4d start, afxV4d end)
{
    // Should be compatible with XMPlaneIntersectLine

    afxReal dot1 = AfxV3dDot(p.uvwd, start);
    afxReal dot2 = AfxV3dDot(p.uvwd, end);
    afxReal d = dot1 - dot2;
    afxReal vt = AfxPlaneDotCoord(p, start);
    vt /= d;

    afxV4d pnt;
    AfxV4dSub(pnt, end, start);
    AfxV4dMad(pnt, start, pnt, AFX_V4D(vt, vt, vt, vt));
}

_AFXINL void AfxPlaneIntersectPlane(afxPlane a, afxPlane b, afxV4d start, afxV4d end)
{
    // Plane-plane intersection function
    // Should be compatible with XMPlaneIntersectPlane

    // Compute the cross product of P2 and P1
    afxV4d V1;
    AfxV3dCross(V1, b.uvwd, a.uvwd);

    // Compute the squared length of V1
    afxReal lenSq = AfxV3dSq(V1);

    // Compute the cross product of P2 and V1
    afxV4d V2 = { 0 };
    AfxV3dCross(V2, b.uvwd, V1);

    afxV4d Point;
    // Multiply V2 by the W component of P1
    AfxV4dMultiply(Point, V2, AFX_V4D(a.uvwd[3], a.uvwd[3], a.uvwd[3], a.uvwd[3]));

    afxV4d V3;
    // Compute the cross product of V1 and P1
    AfxV3dCross(V3, V1, a.uvwd);

    // Multiply V3 by P2W and add to Point
    AfxV4dMad(Point, Point, V3, AFX_V4D(b.uvwd[3], b.uvwd[3], b.uvwd[3], b.uvwd[3]));

    afxV4d LinePoint1;
    // Divide Point by LengthSq to get LinePoint1
    AfxV4dDivs(LinePoint1, Point, lenSq);
    afxV4d LinePoint2;
    // Add V1 to LinePoint1 to get LinePoint2
    AfxV4dAdd(LinePoint2, LinePoint1, V1);

    AfxV4dClamp(start, LinePoint1, AFX_V4D(AFX_EPSILON, AFX_EPSILON, AFX_EPSILON, AFX_EPSILON), LinePoint1);
    AfxV4dClamp(end, LinePoint2, AFX_V4D(AFX_EPSILON, AFX_EPSILON, AFX_EPSILON, AFX_EPSILON), LinePoint2);    
}

_AFXINL afxPlane AfxPlaneTransform(afxPlane p, afxM4d const m)
{
    // Should be compatible with XMPlaneTransform

    afxV4d a, b;
    afxPlane t;
    AfxV4dMultiply(a,   AFX_V4D(p.uvwd[3], p.uvwd[3], p.uvwd[3], p.uvwd[3]), m[3]);
    AfxV4dMad(b, a,        AFX_V4D(p.uvwd[2], p.uvwd[2], p.uvwd[2], p.uvwd[2]), m[2]);
    AfxV4dMad(a, b,        AFX_V4D(p.uvwd[1], p.uvwd[1], p.uvwd[1], p.uvwd[1]), m[1]);
    AfxV4dMad(t.uvwd, a,      AFX_V4D(p.uvwd[0], p.uvwd[0], p.uvwd[0], p.uvwd[0]), m[0]);
    return t;
}

_AFXINL afxPlane AfxPlaneFromPointNormal(afxV4d point, afxV4d normal)
{
    // Should be compatible with XMPlaneFromPointNormal

    afxReal dot = AfxV4dDot(point, normal);
    dot = -dot;
    return AFX_PLANE(dot, dot, dot, dot);
}

_AFXINL afxPlane AfxPlaneFromPoints(afxV4d a, afxV4d b, afxV4d c)
{
    // Should be compatible with XMPlaneFromPoints

    afxV4d ba, ca, n, D;
    AfxV4dSub(ba, a, b);
    AfxV4dSub(ca, a, c);
    AfxV3dCross(n, ba, ca);
    AfxV3dNormalize(n, n);
    
    afxPlane p = AFX_PLANE(n[0], n[1], n[2], 0);
    afxReal d = AfxPlaneDotNormal(p, a);
    d *= -1;
    p.uvwd[AFX_PLANE_DIST] = d;
}






////////////////////////////////////////////////////////////////

/*
    The AfxMakePlane() sets a normalized plane from a given normal vector (normal) and a distance value (dist).
*/

_AFXINL void AfxMakePlane(afxPlane* p, afxV3d const normal, afxReal dist)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    AFX_ASSERT(normal);
    *p = AfxPlaneNormalize(AFX_PLANE(normal[0], normal[1], normal[2], dist));
}

/*
    The AfxMakePlaneFromPointNormal() function constructs a plane from a point on the plane and a normal vector.
    The plane equation is: n * x + d = 0;
    Where: d = - n * p;
    
    The caller must ensure n is normalized.

    p->uvwd[0..2] = normalized normal
    p->uvwd[3]    = distance term
*/

_AFXINL void AfxMakePlaneFromPointNormal(afxPlane *p, const afxV3d point, const afxV3d normal)
{
    afxError err = { 0 };
    AFX_ASSERT(p);

#ifdef _AFX_PLANE_CHECK_DEGENERACY
    if (AfxV3dSq(normal) < 1e-12f)
    {
        p->uvwd[0] = p->uvwd[1] = p->uvwd[2] = 0.0f;
        p->uvwd[3] = 0.0f;
        return;
    }
#else
    // Normalize the input normal
    AfxV3dNormalize(p->uvwd, normal);
#endif

    // d = -dot(n, point)
    p->uvwd[AFX_PLANE_DIST] = -AfxV3dDot(p->uvwd, point);
}

/*
    The AfxMakePlaneFromTriangle() function constructs a plane from three points in the form:
        n * x + d = 0;

    Where:
        n is the normalized plane normal
        d is the distance term

    Given points a, b, c, the normal is computed as:
        n = normalize( (b - a) X (c - a) )
    And the plane distance is:
        d = - dot(n, a)
    This is a standard plane-from-triangle computation.
*/

_AFXINL void AfxMakePlaneFromTriangle(afxPlane* p, afxV3d const a, afxV3d const b, afxV3d const c)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    AFX_ASSERT(c);

    afxV3d ba, ca;
    AfxV3dSub(ba, b, a);
    AfxV3dSub(ca, c, a);

    // Compute normal
    AfxV3dCross(p->uvwd, ba, ca);

#ifndef _AFX_PLANE_DONT_CHECK_DEGENERACY
    // Check degeneracy (optional)
    if (AfxV3dSq(p->uvwd) < 1e-12f)
    {
        // Default normal (0,0,0) and distance = 0
        p->uvwd[0] = p->uvwd[1] = p->uvwd[2] = 0.0f;
        p->uvwd[3] = 0.0f;
        return;
    }
#endif

    // Normalize
    AfxV3dNormalize(p->uvwd, p->uvwd);

    // Distance term
    p->uvwd[AFX_PLANE_DIST] = -AfxV3dDot(p->uvwd, a);
}

_AFXINL void AfxGetPlaneNormal(afxPlane* p, afxV3d normal)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    AFX_ASSERT(normal);
    AfxV3dCopy(normal, p->uvwd);
}

_AFXINL afxReal AfxGetPlaneDistance(afxPlane const* p)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    return p->uvwd[AFX_PLANE_DIST];
}

_AFXINL afxReal AfxFindPlaneDistance(afxPlane const* p, afxV3d const point)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    AFX_ASSERT(point);
    // Returns whether a point is in front of the plane.
    return AfxV3dDot(p->uvwd, point) + p->uvwd[AFX_PLANE_DIST];
}

_AFXINL afxReal AfxFindPlaneHitInterpolationConstant(afxPlane const* p, afxV3d const a, afxV3d const b)
{
    afxError err = { 0 };
    AFX_ASSERT(p);
    AFX_ASSERT(a);
    AFX_ASSERT(b);
    afxV3d t;
    AfxV3dSub(t, a, b);
    return (AfxFindPlaneDistance(p, a)) / AfxV3dDot(p->uvwd, t);
}

_AFXINL afxBool AfxPlaneTestSpheres(afxPlane const* p, afxUnit cnt, afxSphere const spheres[])
{
    afxError err = { 0 };
    AFX_ASSERT2(p, spheres);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxSphere const* s = &spheres[i];

        if (AfxV3dDot(s->xyzr, p->uvwd) + p->uvwd[AFX_PLANE_DIST] <= -s->xyzr[AFX_SPHERE_RADIUS])
            return FALSE;
    }
    return TRUE;
}

_AFXINL afxResult AfxPlaneTestAabbs(afxPlane const* p, afxUnit cnt, afxBox const aabb[])
{
    afxError err = { 0 };
    AFX_ASSERT2(p, aabb);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxReal minD;
        afxReal maxD;

        if (p->uvwd[0] > 0.0f)
        {
            minD = p->uvwd[0] * aabb[i].min[0];
            maxD = p->uvwd[0] * aabb[i].max[0];
        }
        else
        {
            minD = p->uvwd[0] * aabb[i].max[0];
            maxD = p->uvwd[0] * aabb[i].min[0];
        }

        if (p->uvwd[1] > 0.0f)
        {
            minD += p->uvwd[1] * aabb[i].min[1];
            maxD += p->uvwd[1] * aabb[i].max[1];
        }
        else
        {
            minD += p->uvwd[1] * aabb[i].max[1];;
            maxD += p->uvwd[1] * aabb[i].min[1];
        }

        if (p->uvwd[2] > 0.0f)
        {
            minD += p->uvwd[2] * aabb[i].min[2];
            maxD += p->uvwd[2] * aabb[i].max[2];
        }
        else
        {
            minD += p->uvwd[2] * aabb[i].max[2];
            maxD += p->uvwd[2] * aabb[i].min[2];
        }

        // What side of the plane

        if (minD >= -p->uvwd[AFX_PLANE_DIST])
            return +1;

        if (maxD <= -p->uvwd[AFX_PLANE_DIST])
            return -1;
    }
    //Intersection
    return 0;
}

_AFXINL afxBool AfxIntersectPlanes(afxPlane const * p1, afxPlane const* p2, afxPlane const* p3, afxV3d v)
// Function to solve the system of three plane equations using Cramer's rule
{
    float det, detX, detY, detZ;

    // Matrix of the coefficients of the planes
    afxM3d mat =
    {
        { p1->uvwd[0], p1->uvwd[1], p1->uvwd[2] },
        { p2->uvwd[0], p2->uvwd[1], p2->uvwd[2] },
        { p3->uvwd[0], p3->uvwd[1], p3->uvwd[2] }
    };

    // Determinant of the coefficient matrix
    det =   mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
            mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
            mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    if (fabs(det) < 1e-6)
    {
        // If the determinant is zero, the planes are parallel (no intersection)
        AfxV3dSet(v, 0.0f, 0.0f, 0.0f);
        return FALSE;
    }

    // Determinant for x, y, and z
    detX =-(p1->uvwd[AFX_PLANE_DIST] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) -
            p2->uvwd[AFX_PLANE_DIST] * (mat[0][1] * mat[2][2] - mat[0][2] * mat[2][1]) +
            p3->uvwd[AFX_PLANE_DIST] * (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]));

    detY =-(p1->uvwd[AFX_PLANE_DIST] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) -
            p2->uvwd[AFX_PLANE_DIST] * (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) +
            p3->uvwd[AFX_PLANE_DIST] * (mat[0][0] * mat[1][2] - mat[0][2] * mat[1][0]));

    detZ =-(p1->uvwd[AFX_PLANE_DIST] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) -
            p2->uvwd[AFX_PLANE_DIST] * (mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0]) +
            p3->uvwd[AFX_PLANE_DIST] * (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]));

    // Return the intersection point (x, y, z)
    AfxV3dSet(v, detX / det, detY / det, detZ / det);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxCopyPlanes(afxUnit cnt, afxPlane const src[], afxPlane dst[])
{
    afxError err = { 0 };
    AFX_ASSERT(src);
    AFX_ASSERT(dst);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        AfxV3dCopy(dst[i].uvwd, src[i].uvwd);
        dst[i].uvwd[AFX_PLANE_DIST] = src[i].uvwd[AFX_PLANE_DIST];
    }
}
