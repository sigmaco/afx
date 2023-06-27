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
 *                      S I G M A   T E C H N O L O G Y   G R O U P
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

#include "../afxBody.h"

AFX_DEFINE_HANDLE(afxAnimation);

AFX_DEFINE_STRUCT(granny_transform)
{
    afxNat Flags;
    afxV3d Position;
    afxQuat Orientation;
    afxM3d ScaleShear;
};

AFX_DEFINE_STRUCT(granny_vector_track)
{
    afxString32 Name;
    afxNat TrackKey;
    afxInt Dimension;
    afxCurve2 ValueCurve;
};

AFX_DEFINE_STRUCT(granny_transform_track)
{
    afxString32 Name;
    afxInt Flags;
    afxCurve2 OrientationCurve;
    afxCurve2 PositionCurve;
    afxCurve2 ScaleShearCurve;
};

AFX_DEFINE_STRUCT(granny_text_track_entry)
{
    afxReal TimeStamp;
    afxString128 Text;
};

AFX_DEFINE_STRUCT(granny_text_track)
{
    char const * Name;
    afxInt EntryCount;
    granny_text_track_entry * Entries;
};

AFX_DEFINE_STRUCT(granny_periodic_loop)
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
    granny_vector_track * VectorTracks;
    afxInt TransformTrackCount;
    granny_transform_track * TransformTracks;
    afxInt TransformLODErrorCount;
    afxReal * TransformLODErrors;
    afxInt TextTrackCount;
    granny_text_track * TextTracks;
    granny_transform InitialPlacement;
    afxInt Flags;
    afxV3d LoopTranslation;
    granny_periodic_loop * PeriodicLoop;
    granny_transform_track * RootMotion;
    struct ExtendedData { afxNat32 type; void*obj; };
};

AFX_OBJECT(afxAnimation)
{
    afxObject           obj;
    afxString32         name;
    afxReal             dur;
    afxReal             timeStep;
    afxReal             oversampling;
    afxInt              trackGrpCount;
    afxAnimTrackGroup** TrackGrps;
};



#endif//AFX_ANIMATION_H