/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#ifndef AFX_SPACE_H
#define AFX_SPACE_H

#include "qwadro/inc/math/afxScalar.h"

/**
    A spherical coordinate system is a coordinate system for three-dimensional space where the position of a point is specified by three numbers: 
        the radial distance of that point from a fixed origin; 
        its polar angle measured from a fixed polar axis or zenith direction; 
        and the azimuthal angle of its orthogonal projection on a reference plane that passes through the origin and is orthogonal to the fixed axis, measured from another fixed reference direction on that plane. 
    When radius is fixed, the two angular coordinates make a coordinate system on the sphere sometimes called spherical polar coordinates.

    The radial distance is also called the radius or radial coordinate. The polar angle may be called colatitude, zenith angle, normal angle, or inclination angle. 
    The polar angle is often replaced by the elevation angle measured from the reference plane towards the positive Z axis; the depression angle is the negative of the elevation angle.

    According to the conventions of geographical coordinate systems, positions are measured by latitude, longitude, and height (altitude). 
    There are a number of celestial coordinate systems based on different fundamental planes and with different terms for the various coordinates. 
    The spherical coordinate systems used in mathematics normally use radians rather than degrees and measure the azimuthal angle counterclockwise from the x-axis to the y-axis rather than clockwise from north (0°) to east (+90°) like the horizontal coordinate system

    The spherical coordinate system can be seen as one possible generalization of the polar coordinate system in three-dimensional space. 
    It can also be further extended to higher-dimensional spaces and is then referred to as a hyperspherical coordinate system.

    The polar coordinate system is a two-dimensional coordinate system in which each point on a plane is determined by a distance from a reference point and an angle from a reference direction. 
    The reference point (analogous to the origin of a Cartesian coordinate system) is called the pole, and the ray from the pole in the reference direction is the polar axis. 
    The distance from the pole is called the radial coordinate, radial distance or simply radius, and the angle is called the angular coordinate, polar angle, or azimuth. 
    Angles in polar notation are generally expressed in either degrees or radians (2PI radians being equal to 360 degrees).
*/

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxPolarCoord)
// This structure holds a polar coordinate, which is typically a 2D coordinate system where the 
// position of a point is determined by its distance from the origin and an angle.
{
    // Radial distance from the origin (magnitude).
    // This is the radial distance from the origin (in 2D polar coordinates).
    // It is typically positive and represents how far away the point is from the origin.
    afxReal32       r; // norm/distance

    // Azimuthal angle (longitude).
    // This is the angle of the point relative to a reference axis, typically the x-axis (in a 2D plane).
    // The angle theta can range from 0 to 2Pi or -Pi to Pi, depending on the convention.
    // It describes the direction of the point from the origin in the 2D plane.
    afxReal32       theta; // azimuth
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSphericalCoord)
// The spherical coordinates system is an extension of the polar coordinate system to 3D space, 
// and the position of a point is described by three values: radial distance, azimuthal angle, and elevation angle.
{
    // The polar coordinates (r, Theta) describe the radial distance (r) and azimuthal angle (Theta), 
    // which are the 2D components of the spherical coordinate system.
    afxPolarCoord   polar;

    // Elevation or polar angle (latitude).
    // This represents the elevation angle (sometimes called the colatitude), which is the angle between the point and the z-axis in 3D space.
    // Typically, phi ranges from 0 to Pi (from the north pole to the south pole in spherical coordinates).
    // It describes how "high" or "low" the point is relative to the horizontal plane, usually the xy-plane.
    afxReal32       phi; // elevation
};

AFXINL void AfxGetCartesionFromSphericalCoord(afxSphericalCoord const* sc, afxV3d cartesian)
{
    afxReal phiS = AfxSin(sc->phi);
    afxReal x = sc->polar.r * phiS * AfxCos(sc->polar.theta);
    afxReal y = sc->polar.r * phiS * AfxSin(sc->polar.theta);
    afxReal z = sc->polar.r * AfxCos(sc->phi);
    // Now, (x, y, z) would be the Cartesian coordinates of the point in 3D space.
    cartesian[0] = x;
    cartesian[1] = y;
    cartesian[2] = z;
}

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSpace)
/// A 3D coordinate system, expressed relative to a canonical coordinate system.
{
    afxV3d  right; // Unit vector pointing to the right (local +x axis).
    afxV3d  up; // Unit vector pointing upwards (local +y axis).
    afxV3d  ahead; // Unit vector pointing forwards (local -z axis).
    afxV3d  origin; // The origin, relative to the canonical coordinate system.
};

#endif//AFX_SPACE_H
