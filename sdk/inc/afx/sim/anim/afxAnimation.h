#pragma once
/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_ANIMATION_H
#define AFX_ANIMATION_H

 // Terminology: BodyControl is a movable model of a person or animal that is used in entertainment and is typically moved either by strings controlled from above or by a hand inside it.

 // There are two primary objects involved in the run-time animation layer : the afxBody and the afxBodyControl.
 // Together, they represent the two ends of the animation process.
 // The afxBody represents the state of a single model that may have some number of animations affecting it, 
 // whereas the afxBodyControl represents the state of an animation that may be affecting some number of models.
 // When your engine creates a new animating entity in the game world, you create a afxBody to accompany it.
 // When you want to play an animation on such an instance, you create a afxBodyControl.

#include "afxCurve2.h"

AFX_DEFINE_HANDLE(afxAnimation);

AFX_DEFINE_STRUCT(afxVectorTrack)
{
    afxString32 Name;
    afxNat TrackKey;
    afxInt Dimension;
    afxCurve2 ValueCurve;
};

AFX_DEFINE_STRUCT(afxTransformTrack)
{
    afxString32 Name;
    afxInt Flags;
    afxCurve2 OrientationCurve;
    afxCurve2 PositionCurve;
    afxCurve2 ScaleShearCurve;
};

AFX_DEFINE_STRUCT(afxTextTrackEntry)
{
    afxReal TimeStamp;
    afxString128 Text;
};

AFX_DEFINE_STRUCT(afxTextTrack)
{
    char const * Name;
    afxInt EntryCount;
    afxTextTrackEntry * Entries;
};

AFX_DEFINE_STRUCT(afxPeriodicLoop)
{
    afxReal Radius;
    afxReal dAngle;
    afxReal dZ;
    afxV3d BasisX;
    afxV3d BasisY;
    afxV3d Axis;
};

AFX_DEFINE_STRUCT(afxAnimTrackGroup)
{
    char const * Name;
    afxInt VectorTrackCount;
    afxVectorTrack * VectorTracks;
    afxInt TransformTrackCount;
    afxTransformTrack * TransformTracks;
    afxInt TransformLODErrorCount;
    afxReal * TransformLODErrors;
    afxInt TextTrackCount;
    afxTextTrack * TextTracks;
    afxTransform InitialPlacement;
    afxInt Flags;
    afxV3d LoopTranslation;
    afxPeriodicLoop * PeriodicLoop;
    afxTransformTrack * RootMotion;
    struct ExtendedData { afxNat32 type; void*obj; };
};

AFX_OBJECT(afxAnimation)
{
    afxInstance           obj;
    afxString32         name;
    afxReal             dur;
    afxReal             timeStep;
    afxReal             oversampling;
    afxInt              trackGrpCount;
    afxAnimTrackGroup** TrackGrps;
};



#endif//AFX_ANIMATION_H