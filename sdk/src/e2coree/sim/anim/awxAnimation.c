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

#define _AFX_SIM_C
#define _AFX_MOTION_C
#define _AFX_ANIMATION_C
#include "qwadro/sim/anim/awxAnimation.h"
#include "qwadro/math/afxTransform.h"

_AFXINL void AfxGetMotionInitialPlacement(awxMotion mot, afxReal m[4][4])
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssert(m);
    AfxComposeTransformM4d(&mot->init, m);
}

_AFXINL afxBool AfxFindMotionSequence(awxMotion mot, afxString const* seqId, afxNat *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssertType(seqId, afxFcc_STR);
    AfxAssert(seqIdx);
    afxBool found = FALSE;
    afxNat seqCnt = mot->xformSeqCnt;

    if (!(mot->flags & 2)) // if is not sorted
    {
        for (afxNat i = 0; i < seqCnt; i++)
        {
            if (0 == AfxCompareString(seqId, &mot->xformSeqs[i].id.str.str))
            {
                *seqIdx = i;
                found = TRUE;
                break;
            }
        }
    }
    else
    {
        afxNat remainCnt = seqCnt;

        afxNat i = 0;
        while (i < seqCnt)
        {
            afxNat currIdx2 = i + remainCnt / 2;
            afxResult diff;

            if (0 == (diff = AfxCompareString(seqId, &mot->xformSeqs[currIdx2].id.str.str)))
            {
                *seqIdx = currIdx2;
                found = TRUE;
                break;
            }
            else
            {
                if (diff < 0) remainCnt /= 2;
                else
                {
                    remainCnt = remainCnt + remainCnt / -2 - 1;
                    i = currIdx2 + 1;
                }
            }

            if (remainCnt < 1)
                break;
        }        
    }
    
    if (!found)
        *seqIdx = AFX_INVALID_INDEX;

    return found;
}

_AFXINL afxBool AfxFindMotionDataSequence(awxMotion mot, afxString const* seqId, afxNat *seqIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mot, afxFcc_MOT);
    AfxAssertType(seqId, afxFcc_STR);
    AfxAssert(seqIdx);
    afxBool rslt = FALSE;
    afxNat vecSeqCnt = mot->vecSeqCnt;

    for (afxNat i = 0; i < vecSeqCnt; i++)
    {
        if (0 == AfxCompareString(seqId, &mot->vecSeqs[i].id.str.str))
        {
            *seqIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFXINL afxBool AfxFindCorrelatedMotion(awxAnimation ani, afxString const* mdlId, afxNat *motIdx)
{
    afxError err = NIL;
    AfxAssertObjects(1, &ani, afxFcc_ANI);
    AfxAssert(mdlId);
    AfxAssert(motIdx);
    afxBool found = FALSE;
    afxNat motCnt = ani->motionCnt;

    for (afxNat i = 0; i < motCnt; i++)
    {
        if (0 == AfxCompareString(&(ani->motions[*motIdx]->id.str.str), mdlId))
        {
            *motIdx = i;
            found = TRUE;
            break;
        }
    }
    return found;
}

_AFX void AwxTransformAnimations(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxAnimation ani[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(ani);
    AfxThrowError();
    (void)affineTol;
    (void)linearTol;
    AfxThrowError();
}

