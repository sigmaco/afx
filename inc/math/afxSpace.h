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
// polar coordinates
{
    afxReal32       r; // norm/distance
    afxReal32       theta; // azimuth
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSphericalCoord)
// spherical coordinates
{
    afxPolarCoord   polar;
    afxReal32       phi; // elevation
};

AFX_DEFINE_STRUCT_ALIGNED(AFX_SIMD_ALIGNMENT, afxSpace)
/// A 3D coordinate system, expressed relative to a canonical coordinate system.
{
    afxV3d  right; /// Unit vector pointing to the right (local +x axis).
    afxV3d  up; /// Unit vector pointing upwards (local +y axis).
    afxV3d  ahead; /// Unit vector pointing forwards (local -z axis).
    afxV3d  origin; /// The origin, relative to the canonical coordinate system.
};

#endif//AFX_SPACE_H
