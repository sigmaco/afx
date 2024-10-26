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

#define _AMX_C
#define _AMX_CURVE_C
#include "../dev/AmxCadImplKit.h"

_AMXINL afxInt AfxGetCurveDegree(afxCurve c)
{
    afxError err = NIL;
    AfxAssertObjects(1, &c, afxFcc_CUR);
    return c->degree;
}

_AMXINL afxCurveFlags AfxGetCurveFlags(afxCurve c)
{
    afxError err = NIL;
    AfxAssertObjects(1, &c, afxFcc_CUR);
    return c->flags;
}

_AMXINL afxCurveFlags AfxTestCurveFlags(afxCurve c, afxCurveFlags mask)
{
    afxError err = NIL;
    AfxAssertObjects(1, &c, afxFcc_CUR);
    return (c->flags & mask);
}

_AMXINL afxBool AfxIsCurveKeyframed(afxCurve c)
{
    afxError err = NIL;
    AfxAssertObjects(1, &c, afxFcc_CUR);
    return !!AfxTestCurveFlags(c, afxCurveFlag_KEYFRAMED);
}

_AMXINL afxBool AfxIsCurveIdentity(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return !!AfxTestCurveFlags(c, afxCurveFlag_IDENTITY);
}

_AMXINL afxBool AfxIsCurveConstantOrIdentity(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return !!AfxTestCurveFlags(c, afxCurveFlag_IDENTITY | afxCurveFlag_CONSTANT);
}

_AMXINL afxBool AfxIsCurveConstantNotIdentity(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return AfxIsCurveConstantOrIdentity(c) && !AfxIsCurveIdentity(c);
}

_AMXINL afxUnit AfxCountCurveKnots(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit knotCnt = 0;

    switch (c->fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    {
        knotCnt = c->ctrlCnt / c->dimens;
        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        knotCnt = c->knotCnt;
        break;
    }
    case afxCurveFormat_DaIdentity:
    {
        knotCnt = 0;
        break;
    }
    case afxCurveFormat_DaConstant32f:
    case afxCurveFormat_D3Constant32f:
    case afxCurveFormat_D4Constant32f:
    {
        knotCnt = 1;
        break;
    }
    default: AfxThrowError(); break;
    }
    AFX_ASSERT(knotCnt == c->knotCnt);
#endif
    return c->knotCnt;
}

_AMXINL afxUnit AfxGetCurveDimensionalityUnchecked(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit dims = 0;

    switch (c->fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    case afxCurveFormat_DaIdentity:
    {
        dims = c->dimens;
        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        dims = c->ctrlCnt / c->knotCnt;
        break;
    }
    case afxCurveFormat_DaConstant32f:
    {
        dims = c->ctrlCnt;
        break;
    }
    case afxCurveFormat_D3Constant32f:
    {
        dims = 3;
        break;
    }
    case afxCurveFormat_D4Constant32f:
    {
        dims = 4;
        break;
    }
    default: AfxThrowError(); break;
    };

    AFX_ASSERT(dims == c->dimens);
#endif
    return c->dimens;
}

_AMXINL afxUnit AfxGetCurveDimensionality(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    AFX_ASSERT(AfxCountCurveKnots(c) ? AfxGetCurveDimensionalityUnchecked(c) : 0);
#endif
    return c->dimens;
}

_AMXINL afxReal* AfxGetCurveKnots(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    switch (c->fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    case afxCurveFormat_DaIdentity:
    case afxCurveFormat_DaConstant32f:
    case afxCurveFormat_D3Constant32f:
    case afxCurveFormat_D4Constant32f:
    {
        AFX_ASSERT(!c->knots);
        break;
    }
    default:break;
    }
#endif
    return c->knots;
}

_AMXINL afxReal* AfxGetCurveControls(afxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case afxCurveFormat_D3Constant32f: return c->ctrls3;
    case afxCurveFormat_D4Constant32f: return c->ctrls4;
    default: break;
    }
    return c->ctrls;
}

_AMXINL afxUnit _FindKnot(afxReal t, afxInt knotCnt, afxReal const knots[])
{
    afxBool v6;
    afxInt v3 = 0;
    afxReal const* v4 = knots;
    afxInt v5 = knotCnt;

    while (v5 > 1)
    {
        v6 = (v5 & 1) == 0;
        v5 /= 2;

        if (v6)
        {
            if (t >= v4[v5])
            {
                if (v3 + v5 + 1 >= knotCnt || t < v4[v5 + 1])
                {
                    v3 += v5;
                    v4 += v5;
                    break;
                }

                v3 += v5 + 1;
                v4 += v5-- + 1;
            }
        }
        else
        {
            if (t >= v4[v5])
            {
                v3 += v5;
                v4 += v5;
            }
            ++v5;
        }
    }

    v6 = v3 == knotCnt;

    if (v3 < knotCnt)
    {
        do
        {
            if (*v4 > t)
                break;

            ++v3;
            ++v4;
        } while (v3 < knotCnt);

        v6 = v3 == knotCnt;
    }

    if (v6 && v3 > 0)
        --v3;
    
    afxError err;
    AFX_ASSERT(v3 >= 0);
    return v3;
}

_AMXINL afxUnit _FindCloseKnot(afxReal t, afxUnit baseIdx, afxUnit knotCnt, afxReal const knots[])
{
    return _FindKnot(t, knotCnt, knots);
}

_AMX afxUnit AfxFindCurveKnot(afxCurve c, afxReal t)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;
    
    switch (c->fmt)
    {
    case afxCurveFormat_DaK32fC32f:
    {
        knotIdx = _FindKnot(t, c->knotCnt, c->knots);
        break;
    }
    case afxCurveFormat_DaKeyframes32f:
    case afxCurveFormat_DaIdentity:
    case afxCurveFormat_DaConstant32f:
    case afxCurveFormat_D3Constant32f:
    case afxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_AMX afxUnit AfxFindCurveCloseKnot(afxCurve c, afxReal t, afxUnit baseIdx)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;

    switch (c->fmt)
    {
    case afxCurveFormat_DaK32fC32f:
    {
        knotIdx = _FindCloseKnot(t, baseIdx, c->knotCnt, c->knots);
        break;
    }
    case afxCurveFormat_DaKeyframes32f:
    case afxCurveFormat_DaIdentity:
    case afxCurveFormat_DaConstant32f:
    case afxCurveFormat_D3Constant32f:
    case afxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_AMX void AfxCopyCurve(afxCurve c, afxCurve src)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    AFX_ASSERT(src);
    AFX_ASSERT(c->fmt == src->fmt);
    AfxCatchError(err);
    afxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    {
        c->dimens = src->dimens;

        for (afxUnit i = c->ctrlCnt; i-- > 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        for (afxUnit i = c->knotCnt; i-- > 0;)
            c->knots[i] = src->knots[i];

        for (afxUnit i = c->ctrlCnt; i-- > 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case afxCurveFormat_DaIdentity:
    {
        c->dimens = src->dimens;
        break;
    }
    case afxCurveFormat_DaConstant32f:
    {
        for (afxUnit i = c->ctrlCnt; i--> 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case afxCurveFormat_D3Constant32f:
    {
        AfxV3dCopy(c->ctrls3, src->ctrls3);
        break;
    }
    case afxCurveFormat_D4Constant32f:
    {
        AfxV4dCopy(c->ctrls4, src->ctrls4);
        break;
    }
    default: AfxThrowError(); break;
    }
}

_AMX void AfxSetCurveAllKnotValues(afxCurve c, afxUnit knotCnt, afxUnit dimens, afxReal const* knotSrc, afxReal const* ctrlSrc)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    AFX_ASSERT_RANGE(c->knotCnt, 0, knotCnt);
    AFX_ASSERT(dimens == c->dimens);
    afxCurveFormat fmt = c->fmt;
    
    switch (fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    {
        c->dimens = dimens;
        afxUnit i = dimens * knotCnt;

        for (; i-- > 0;)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        afxUnit i;

        for (i = 0; i < knotCnt; i++)
            c->knots[i] = knotSrc[i];

        afxUnit ctrlCnt = dimens * knotCnt;

        for (i = 0; i < ctrlCnt; i++)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case afxCurveFormat_DaIdentity:
    {
        c->dimens = dimens;
        break;
    }
    case afxCurveFormat_DaConstant32f:
    {
        afxUnit i = c->ctrlCnt;

        for (; i-- > 0;)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case afxCurveFormat_D3Constant32f:
    {
        AfxV3dCopy(c->ctrls3, ctrlSrc);
        break;
    }
    case afxCurveFormat_D4Constant32f:
    {
        AfxV4dCopy(c->ctrls4, ctrlSrc);
        break;
    }
    default: AfxThrowError(); break;
    }
}

_AMX void AfxExtractCurveKnotValues(afxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec)
{
    afxError err = NIL;
    AFX_ASSERT(c);

    if (AfxIsCurveIdentity(c))
    {
        for (afxUnit i = 0; i < c->dimens; i++)
            ctrlResults[i] = identivec[i];
    }
    else
    {
        afxCurveFormat fmt = c->fmt;

        switch (fmt)
        {
        case afxCurveFormat_DaKeyframes32f:
        {
            if (ctrlResults)
            {
                afxUnit dims = c->dimens;
                afxReal const* s = &c->ctrls[knotIdx * dims];

                for (afxUnit i = dims * knotCnt; i-- > 0;)
                    ctrlResults[i] = s[i];
            }
            break;
        }
        case afxCurveFormat_DaK32fC32f:
        {
            afxUnit dim = c->ctrlCnt / c->knotCnt;
            AFX_ASSERT(c->dimens == dim);

            if (knotResults)
            {
                AFX_ASSERT_RANGE(c->knotCnt, knotIdx, knotCnt);
                afxReal const* s = &c->knots[knotIdx];

                for (afxUnit i = 0; i < knotCnt; i++)
                    knotResults[i] = s[i];
            }

            if (ctrlResults)
            {
                afxUnit baseCtrlIdx = knotIdx * dim;
                afxUnit ctrlCnt = knotCnt * dim;
                AFX_ASSERT(c->ctrlCnt >= ctrlCnt);
                AFX_ASSERT_RANGE(c->ctrlCnt, baseCtrlIdx, ctrlCnt);
                afxReal const* s = &c->ctrls[baseCtrlIdx];

                for (afxUnit i = 0; i < ctrlCnt; i++)
                    ctrlResults[i] = s[i];
            }
            break;
        }
        case afxCurveFormat_DaIdentity:
        {
            if (knotCnt == 1 && knotResults)
                knotResults[0] = 0.0;

            if (ctrlResults)
            {
                for (afxUnit i = c->dimens; i-- > 0;)
                    ctrlResults[i] = identivec[i];
            }
            break;
        }
        case afxCurveFormat_DaConstant32f:
        {
            afxUnit ctrlCnt = c->ctrlCnt;

            if (knotResults)
                knotResults[0] = 0.0;

            if (ctrlResults)
            {
                for (afxUnit i = ctrlCnt; i-- > 0;)
                    ctrlResults[i] = c->ctrls[i];
            }
            break;
        }
        case afxCurveFormat_D3Constant32f:
        case afxCurveFormat_D4Constant32f:
        {
            if (knotResults)
                knotResults[0] = 0.0;

            if (ctrlResults)
            {
                if (fmt == afxCurveFormat_D3Constant32f)
                    AfxV3dCopy(ctrlResults, c->ctrls3);
                else
                    AfxV4dCopy(ctrlResults, c->ctrls4);
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
}

_AMX afxReal AfxExtractCurveKnotValue(afxCurve c, afxUnit knotIdx, afxReal* ctrlRslt, afxReal const* identity)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxReal f = 0.0;
    AfxExtractCurveKnotValues(c, knotIdx, 1, &f, ctrlRslt, identity);
    return f;
}

_AMX void AfxEvaluateCurveAtKnot(afxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    afxError err;
    afxReal *tiBuffer;
    afxReal *piBuffer;
    afxReal tiBufferSpace[8];
    afxReal piBufferSpace[64];

    if (AfxIsCurveConstantOrIdentity(c))
    {
        AfxExtractCurveKnotValue(c, 0, rslt, identityVec);
    }
    else
    {
        afxInt degree = AfxGetCurveDegree(c);
        afxUnit knotCnt = AfxCountCurveKnots(c);
        
        if (knotIdx == knotCnt)
            knotIdx -= 1;

        AFX_ASSERT(knotIdx < AFX_INVALID_INDEX - 10);

        if (Smt2.ConstructBSplineBuffers(dimens,
            bwdsLoop ? c : NIL, c, fwdsLoop ? c : NIL,
            curveDur, curveDur, curveDur,
            knotIdx, tiBufferSpace, piBufferSpace, &tiBuffer, &piBuffer, identityVec) && normalize && dimens == 4)
        {
            AfxEnsureQuaternionContinuity(degree + 1, &piBufferSpace);
        }

        Smt2.SampleBSpline(degree, dimens, normalize, tiBuffer, piBuffer, t, rslt);
    }
}

_AMX void AfxEvaluateCurveAt(afxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    AfxEvaluateCurveAtKnot(c, dimens, normalize, bwdsLoop, fwdsLoop, curveDur, AfxFindCurveKnot(c, t), t, rslt, identityVec);
}

_AMX void AfxMakeCurveDaKC32f(afxCurve c, afxUnit degree, afxUnit dim, afxUnit knotCnt, afxReal const knots[], afxReal const ctrls[])
{
    afxError err = NIL;
    AFX_ASSERT(c);
    c->fmt = afxCurveFormat_DaK32fC32f;
    c->degree = degree;
    c->dimens = dim; // unused
    c->knotCnt = knotCnt;
    c->knots = (afxReal*)knots;
    c->ctrlCnt = dim * knotCnt;
    c->ctrls = (afxReal*)ctrls;
}

// BLUEPRINT

_AMXINL void AfxResetCurveInfo(afxCurveInfo* cb, afxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
{
    //cb->needsFreeing = FALSE;
    cb->fmt = fmt;
    cb->degree = degree;
    cb->dimens = dimens;
    cb->knotCnt = knotCnt;
    cb->knots = NIL;
    cb->ctrls = NIL;
    cb->src = NIL;
    cb->sampleCnt = -1;
    cb->sampleDim = -1;
    cb->xformedSamples = NIL;
    cb->origSamples = NIL;
}

_AMX void AfxBeginCurve(afxCurveBlueprint* cb, afxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
{
    afxError err = NIL;
    AFX_ASSERT(cb);
    cb->curveBuilderNeedsFreeing = FALSE;
    cb->fmt = fmt;
    cb->degree = degree;
    cb->dimens = dimens;
    cb->knotCnt = knotCnt;
    cb->knotArray = 0;
    cb->ctrlArray = 0;
    cb->srcCurve = 0;
    cb->sampleCnt = -1;
    cb->sampleDimension = -1;
    cb->transformedSamples = 0;
    cb->originalSamples = 0;
}

_AMX void AfxBeginCurveCopy(afxCurveBlueprint* cb, afxCurve src)
{
    afxError err = NIL;
    AFX_ASSERT(cb);
    AfxBeginCurve(cb, src->fmt, AfxGetCurveDegree(src), AfxGetCurveDimensionality(src), AfxCountCurveKnots(src));
    cb->srcCurve = src;
}

_AMX afxError _AmxCurCtorCb(afxCurve cur, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cur, afxFcc_CUR);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxCurveInfo const* info = ((afxCurveInfo const*)args[1]) + invokeNo;

    afxCurveFormat fmt = info->fmt;
    cur->fmt = fmt;
    cur->degree = info->degree;
    cur->flags = NIL;

    switch (fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    {
        cur->dimens = info->dimens;
        cur->flags |= afxCurveFlag_KEYFRAMED;
        cur->knotCnt = info->knotCnt; // unused
        cur->knots = NIL; // unused
        cur->ctrlCnt = info->dimens * info->knotCnt;
        cur->ctrls = NIL;

        AFX_ASSERT(cur->knotCnt == cur->ctrlCnt / cur->dimens);

        if (cur->ctrlCnt && !(cur->ctrls = AfxAllocate(cur->ctrlCnt, sizeof(cur->ctrls[0]), 0, AfxHere())))
            AfxThrowError();

        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        cur->dimens = info->dimens; // unused        
        cur->knotCnt = info->knotCnt;
        cur->knots = NIL;
        cur->ctrlCnt = info->knotCnt * info->dimens;
        cur->ctrls = NIL;

        if (cur->knotCnt && !(cur->knots = AfxAllocate(cur->knotCnt, sizeof(cur->knots[0]), 0, AfxHere())))
            AfxThrowError();

        if (cur->ctrlCnt && !(cur->ctrls = AfxAllocate(cur->ctrlCnt, sizeof(cur->ctrls[0]), 0, AfxHere())))
            AfxThrowError();

        if (!cur->knotCnt)
        {
            if (cur->ctrlCnt)
                cur->flags |= afxCurveFlag_KEYFRAMED;
            else
                cur->flags |= afxCurveFlag_IDENTITY;
        }
        else if (cur->knotCnt == 1)
        {
            cur->flags |= afxCurveFlag_CONSTANT;
        }
        break;
    }
    case afxCurveFormat_DaIdentity:
    {
        cur->dimens = info->dimens;
        cur->knotCnt = 0; // unused
        cur->knots = NIL; // unused
        cur->ctrlCnt = 0; // unused
        cur->ctrls = NIL; // unused
        cur->flags |= afxCurveFlag_IDENTITY | afxCurveFlag_CONSTANT;
        break;
    }
    case afxCurveFormat_DaConstant32f:
    {
        cur->dimens = info->dimens; // unused
        cur->knotCnt = info->knotCnt; // unused
        AFX_ASSERT(cur->knotCnt == 1);
        cur->knots = NIL; // unused
        cur->ctrlCnt = info->dimens;
        cur->ctrls = NIL;

        if (cur->ctrlCnt && !(cur->ctrls = AfxAllocate(cur->ctrlCnt, sizeof(cur->ctrls[0]), 0, AfxHere())))
            AfxThrowError();

        cur->flags |= afxCurveFlag_CONSTANT;
        break;
    }
    case afxCurveFormat_D3Constant32f:
    case afxCurveFormat_D4Constant32f:
    {
        cur->dimens = info->dimens; // unused
        cur->knotCnt = 1; // unused
        cur->knots = NIL; // unused
        cur->ctrlCnt = 0; // unused
        cur->ctrls = NIL; // unused
        cur->flags |= afxCurveFlag_CONSTANT;
        break;
    }
    default: AfxThrowError(); break;
    }

    if (!err)
    {
        afxReal const* ctrlArray = info->ctrls;

        if (!info->src)
        {
            if (!ctrlArray)
            {
                if (info->knotCnt)
                    AfxThrowError();
            }
            else
            {
                if (!info->knots)
                {
                    if (info->knotCnt != 1 && (info->degree || fmt))
                        AfxThrowError();
                }
            }
        }

        if (!err)
        {
            if (info->sampleCnt != -1 && info->origSamples)
            {
                AfxThrowError();
                //((void(__cdecl *)(afxCurve*, int, int, const float *))*(f))(cur, sampleCnt, info->sampleDimension, info->originalSamples);
            }

            if (info->src)
            {
                AfxCopyCurve(cur, info->src);
            }
            else
            {
                if (fmt != afxCurveFormat_DaKeyframes32f)
                {
                    AfxSetCurveAllKnotValues(cur, info->knotCnt, info->dimens, info->knots, info->ctrls);
                }
                else
                {
                    AfxSetCurveAllKnotValues(cur, info->knotCnt, info->dimens, info->knots ? info->knots : (afxReal const[]) { 0.f }, info->ctrls);
                }
            }
        }
    }
    return err;
}

_AMX afxError _AmxCurDtorCb(afxCurve cur)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cur, afxFcc_CUR);

    switch (cur->fmt)
    {
    case afxCurveFormat_DaKeyframes32f:
    {
        if (cur->ctrls)
            AfxDeallocate(cur->ctrls);

        break;
    }
    case afxCurveFormat_DaK32fC32f:
    {
        if (cur->knots)
            AfxDeallocate(cur->knots);

        if (cur->ctrls)
            AfxDeallocate(cur->ctrls);

        break;
    }
    case afxCurveFormat_DaConstant32f:
    {
        if (cur->ctrls)
            AfxDeallocate(cur->ctrls);

        break;
    }
    default: break;
    }
    return err;
}

_AMX afxClassConfig const _AmxCurClsCfg =
{
    .fcc = afxFcc_CUR,
    .name = "Curve",
    .desc = "Geometric Curve",
    .fixedSiz = sizeof(AFX_OBJECT(afxCurve)),
    .ctor = (void*)_AmxCurCtorCb,
    .dtor = (void*)_AmxCurDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireCurves(afxSimulation sim, afxUnit cnt, afxCurveInfo const info[], afxCurve curves[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxClass* cls = (afxClass*)AfxGetCurveClass(sim);
    AfxAssertClass(cls, afxFcc_CUR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)curves, (void const*[]) { sim, info }))
        AfxThrowError();

    return err;
}
