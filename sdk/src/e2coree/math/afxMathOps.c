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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/math/afxTransform.h"
#include "afx/sim/afxSimulation.h"
#include "afx/math/afxMatrix.h"
#include "afx/sim/afxAsset.h"

_AFXINL void AfxComputeBasisConversion(afxSpaceSpecification const* original, afxSpaceSpecification const* desired, afxReal at[3], afxReal lt[3][3], afxReal ilt[3][3])
{    
    afxError err = NIL;
    AfxAssert(original);
    AfxAssert(desired);

    afxM3d desiredAxisSys, srcAxisSys;
    AfxSetTransposedM3d(desiredAxisSys, desired->right, desired->up, desired->back);
    AfxSetM3d(srcAxisSys, original->right, original->up, original->back);

    AfxMultiplyM3d(lt, desiredAxisSys, srcAxisSys);
        
    afxReal s = desired->unitsPerMeter / original->unitsPerMeter;        
    AfxScaleV3d(lt[0], lt[0], s);
    AfxScaleV3d(lt[1], lt[1], s);
    AfxScaleV3d(lt[2], lt[2], s);
    
    AfxInvertM3d(ilt, lt);

    AfxSubV3d(at, desired->origin, original->origin);
}
