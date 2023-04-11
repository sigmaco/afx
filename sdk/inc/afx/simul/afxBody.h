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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "dag/afxSkeleton.h"
#include "modeling/afxModel.h"

// The granny_model_instance object helps you manage the run - time state of a granny_model.
// The granny_model is just the structured data for a particular model - it does not have any concept of where it is, what state it's in, or what animations are playing on it.
// The granny_model_instance keeps track of this and provides a number of handy utility functions to ease working with model states. 

// While all your identical entities should share the same granny_model, since their mesh and skeletal data is identical, each should still have its own granny_model_instance.
// This is because the granny_model_instance data is all dynamic, and is not shared across multiple models even if they are made from the same granny_model data.
// So while it is common to cache the granny_model hierarchy of structures and instantiate them multiple times, you never want to cache or reuse granny_model_instance objects.
// They should always be instantiated and freed along with the entities they represent in your application.

// Note, however, that these are simply pointers into the original data you passed during instantiation. 
// The granny_model_instance does not keep copies of the data (to minimize memory use), so you cannot free the original model data and expect to get valid pointers back here. 
// For that matter, if you free the original model data, none of the granny_model_instance calls will work either, since they all rely on that data, as you would expect.

// afxBody is a instanced afxModel, following the Granny concept.
// afxBody = granny_model_instance
// afxModel = granny_model

AFX_DEFINE_HANDLE(afxBody);

AFX_OBJECT(afxBody)
{
    AFX_OBJECT(afxNode) nod;

    afxConnection       mdl; // afxModel
    void                *_cachedSkel;
    afxNat32            _cachedBones,
                        _cachedBonesCnt;
    void                *_bindingSentinel; // granny_model_control_binding
    afxNat32            _next,
                        _prev;
    afxNat32            _usrData[4];
    afxNat32            _reserved0,
                        _reserved1;

    afxReal             mass; // totally of masses of parts. Additive to initial "fake" value. May be used to create physical behavoir on empty bodies.
    afxV4d              centre;
    afxV4d              velocity;
    afxV4d              force;
    afxV4d              torque;

    afxReal             linearDamping,
                        angularDamping;
};

AFX afxNode             AfxBodyGetNode(afxBody bod);

AFX afxModel            AfxBodyGetModel(afxBody bod);
AFX afxSkeleton         AfxBodyGetSkeleton(afxBody bod);
    
AFX void                AfxBodyResetClock(afxBody bod);
AFX void                AfxBodyUpdateModelMatrix(afxBody bod, afxReal secondsElapsed, afxM4d const mm, afxM4d dest, afxBool inverse);

#endif//AFX_BODY_H