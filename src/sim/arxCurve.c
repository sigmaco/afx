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

#define _ARX_C
#define _ARX_CURVE_C
#include "../../sim/impl/asxImplementation.h"

_ARXINL afxUnit ArxCountCurveKnots(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit knotCnt = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        knotCnt = c->ctrlCnt / c->dimens;
        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        knotCnt = c->knotCnt;
        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        knotCnt = 0;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
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

_ARXINL afxUnit ArxGetCurveDimensionalityUnchecked(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    afxUnit dims = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    {
        dims = c->dimens;
        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        dims = c->ctrlCnt / c->knotCnt;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        dims = c->ctrlCnt;
        break;
    }
    case arxCurveFormat_D3Constant32f:
    {
        dims = 3;
        break;
    }
    case arxCurveFormat_D4Constant32f:
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

_ARXINL afxUnit ArxGetCurveDimensionality(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    AFX_ASSERT(ArxCountCurveKnots(c) ? ArxGetCurveDimensionalityUnchecked(c) : 0);
#endif
    return c->dimens;
}

_ARX void ArxDescribeCurve(arxCurve cur, arxCurveInfo2* desc)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &cur);
    AFX_ASSERT(desc);

    desc->fmt = cur->fmt;
    desc->degree = cur->degree;
    desc->flags = cur->flags;
    desc->dimens = ArxGetCurveDimensionality(cur);
    desc->knotCnt = ArxCountCurveKnots(cur);
    desc->ctrlCnt = cur->ctrlCnt;
}

_ARXINL afxInt ArxGetCurveDegree(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return c->degree;
}

_ARXINL arxCurveFlags ArxGetCurveFlags(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return c->flags;
}

_ARXINL arxCurveFlags ArxTestCurveFlags(arxCurve c, arxCurveFlags mask)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return (c->flags & mask);
}

_ARXINL afxBool ArxIsCurveKeyframed(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);
    return !!ArxTestCurveFlags(c, arxCurveFlag_KEYFRAMED);
}

_ARXINL afxBool ArxIsCurveIdentity(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return !!ArxTestCurveFlags(c, arxCurveFlag_IDENTITY);
}

_ARXINL afxBool ArxIsCurveConstantOrIdentity(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return !!ArxTestCurveFlags(c, arxCurveFlag_IDENTITY | arxCurveFlag_CONSTANT);
}

_ARXINL afxBool ArxIsCurveConstantNotIdentity(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    return ArxIsCurveConstantOrIdentity(c) && !ArxIsCurveIdentity(c);
}

_ARXINL afxReal* ArxGetCurveKnots(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
#if _AFX_DEBUG
    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        AFX_ASSERT(!c->knots);
        break;
    }
    default:break;
    }
#endif
    return c->knots;
}

_ARXINL afxReal* ArxGetCurveControls(arxCurve c)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    arxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case arxCurveFormat_D3Constant32f: return c->ctrls3;
    case arxCurveFormat_D4Constant32f: return c->ctrls4;
    default: break;
    }
    return c->ctrls;
}

_ARXINL afxUnit _FindKnot(afxReal t, afxInt knotCnt, afxReal const knots[])
{
    afxBool rslt;
    afxInt idx = 0;
    afxReal const* k = knots;
    afxInt kc = knotCnt;

    while (kc > 1)
    {
        rslt = (kc & 1) == 0;
        kc /= 2;

        if (rslt)
        {
            if (t >= k[kc])
            {
                if (idx + kc + 1 >= knotCnt || t < k[kc + 1])
                {
                    idx += kc;
                    k += kc;
                    break;
                }

                idx += kc + 1;
                k += kc-- + 1;
            }
        }
        else
        {
            if (t >= k[kc])
            {
                idx += kc;
                k += kc;
            }
            ++kc;
        }
    }

    rslt = idx == knotCnt;

    if (idx < knotCnt)
    {
        do
        {
            if (*k > t)
                break;

            ++idx;
            ++k;
        } while (idx < knotCnt);

        rslt = idx == knotCnt;
    }

    if (rslt && idx > 0)
        --idx;
    
    afxError err;
    AFX_ASSERT(idx >= 0);
    return idx;
}

_ARXINL afxUnit _FindCloseKnot(afxReal t, afxUnit baseIdx, afxUnit knotCnt, afxReal const knots[])
{
    return _FindKnot(t, knotCnt, knots);
}

_ARX afxUnit ArxFindCurveKnot(arxCurve c, afxReal t)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;
    
    switch (c->fmt)
    {
    case arxCurveFormat_DaK32fC32f:
    {
        knotIdx = _FindKnot(t, c->knotCnt, c->knots);
        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_ARX afxUnit AfxFindCurveCloseKnot(arxCurve c, afxReal t, afxUnit baseIdx)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxUnit knotIdx = 0;

    switch (c->fmt)
    {
    case arxCurveFormat_DaK32fC32f:
    {
        knotIdx = _FindCloseKnot(t, baseIdx, c->knotCnt, c->knots);
        break;
    }
    case arxCurveFormat_DaKeyframes32f:
    case arxCurveFormat_DaIdentity:
    case arxCurveFormat_DaConstant32f:
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        knotIdx = 0;
        break;
    }
    default: AfxThrowError(); break;
    }
    return knotIdx;
}

_ARX void ArxCopyCurve(arxCurve c, arxCurve src)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    AFX_ASSERT(src);
    AFX_ASSERT(c->fmt == src->fmt);
    AfxCatchError(err);
    arxCurveFormat fmt = c->fmt;

    switch (fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        c->dimens = src->dimens;

        for (afxUnit i = c->ctrlCnt; i-- > 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        for (afxUnit i = c->knotCnt; i-- > 0;)
            c->knots[i] = src->knots[i];

        for (afxUnit i = c->ctrlCnt; i-- > 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        c->dimens = src->dimens;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        for (afxUnit i = c->ctrlCnt; i--> 0;)
            c->ctrls[i] = src->ctrls[i];

        break;
    }
    case arxCurveFormat_D3Constant32f:
    {
        AfxV3dCopy(c->ctrls3, src->ctrls3);
        break;
    }
    case arxCurveFormat_D4Constant32f:
    {
        AfxV4dCopy(c->ctrls4, src->ctrls4);
        break;
    }
    default: AfxThrowError(); break;
    }
}

_ARX void ArxUpdateCurveKnots(arxCurve c, afxUnit knotCnt, afxUnit dimens, afxReal const* knotSrc, afxReal const* ctrlSrc)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    AFX_ASSERT_RANGE(c->knotCnt, 0, knotCnt);
    AFX_ASSERT(dimens == c->dimens);
    arxCurveFormat fmt = c->fmt;
    
    switch (fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        c->dimens = dimens;
        afxUnit i = dimens * knotCnt;

        for (; i-- > 0;)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        afxUnit i;

        for (i = 0; i < knotCnt; i++)
            c->knots[i] = knotSrc[i];

        afxUnit ctrlCnt = dimens * knotCnt;

        for (i = 0; i < ctrlCnt; i++)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        c->dimens = dimens;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        afxUnit i = c->ctrlCnt;

        for (; i-- > 0;)
            c->ctrls[i] = ctrlSrc[i];

        break;
    }
    case arxCurveFormat_D3Constant32f:
    {
        AfxV3dCopy(c->ctrls3, ctrlSrc);
        break;
    }
    case arxCurveFormat_D4Constant32f:
    {
        AfxV4dCopy(c->ctrls4, ctrlSrc);
        break;
    }
    default: AfxThrowError(); break;
    }
}

_ARX void ArxExtractCurveKnotValues(arxCurve c, afxUnit knotIdx, afxUnit knotCnt, afxReal* knotResults, afxReal* ctrlResults, afxReal const* identivec)
{
    afxError err = NIL;
    AFX_ASSERT(c);

    if (ArxIsCurveIdentity(c))
    {
        for (afxUnit i = 0; i < c->dimens; i++)
            ctrlResults[i] = identivec[i];
    }
    else
    {
        arxCurveFormat fmt = c->fmt;

        switch (fmt)
        {
        case arxCurveFormat_DaKeyframes32f:
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
        case arxCurveFormat_DaK32fC32f:
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
        case arxCurveFormat_DaIdentity:
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
        case arxCurveFormat_DaConstant32f:
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
        case arxCurveFormat_D3Constant32f:
        case arxCurveFormat_D4Constant32f:
        {
            if (knotResults)
                knotResults[0] = 0.0;

            if (ctrlResults)
            {
                if (fmt == arxCurveFormat_D3Constant32f)
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

_ARX afxReal ArxExtractCurveKnotValue(arxCurve c, afxUnit knotIdx, afxReal* ctrlRslt, afxReal const* identity)
{
    afxError err = NIL;
    AFX_ASSERT(c);
    afxReal f = 0.0;
    ArxExtractCurveKnotValues(c, knotIdx, 1, &f, ctrlRslt, identity);
    return f;
}

_ARX void ArxEvaluateCurveAtKnot(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    afxError err;
    afxReal *tiBuffer;
    afxReal *piBuffer;
    afxReal tiBufferSpace[8];
    afxReal piBufferSpace[64];

    if (ArxIsCurveConstantOrIdentity(c))
    {
        ArxExtractCurveKnotValue(c, 0, rslt, identityVec);
    }
    else
    {
        c->pimpl->evalAtKnot(c, dimens, normalize, bwdsLoop, fwdsLoop, curveDur, knotIdx, t, rslt, identityVec);
    }
}

_ARX void ArxEvaluateCurveAt(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    ArxEvaluateCurveAtKnot(c, dimens, normalize, bwdsLoop, fwdsLoop, curveDur, ArxFindCurveKnot(c, t), t, rslt, identityVec);
}

_ARX void ArxMakeCurveDaKC32f(arxCurve c, afxUnit degree, afxUnit dim, afxUnit knotCnt, afxReal const knots[], afxReal const ctrls[])
{
    afxError err = NIL;
    AFX_ASSERT(c);
    c->fmt = arxCurveFormat_DaK32fC32f;
    c->degree = degree;
    c->dimens = dim; // unused
    c->knotCnt = knotCnt;
    c->knots = (afxReal*)knots;
    c->ctrlCnt = dim * knotCnt;
    c->ctrls = (afxReal*)ctrls;
}

// BLUEPRINT

_ARXINL void ArxResetCurveInfo(arxCurveInfo* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
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

_ARX void ArxBeginCurve(arxCurveBlueprint* cb, arxCurveFormat fmt, afxUnit degree, afxUnit dimens, afxUnit knotCnt)
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

_ARX void ArxBeginCurveCopy(arxCurveBlueprint* cb, arxCurve src)
{
    afxError err = NIL;
    AFX_ASSERT(cb);
    ArxBeginCurve(cb, src->fmt, ArxGetCurveDegree(src), ArxGetCurveDimensionality(src), ArxCountCurveKnots(src));
    cb->srcCurve = src;
}

_ARX _arxCurveDdi const _ARX_CURVE_DDI =
{
    NIL
};

_ARX afxError _ArxCurCtorCb(arxCurve c, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxCurveInfo const* info = ((arxCurveInfo const*)args[1]) + invokeNo;

    c->pimpl = &_ARX_CURVE_DDI;

    arxCurveFormat fmt = info->fmt;
    c->fmt = fmt;
    c->degree = info->degree;
    c->flags = NIL;

    switch (fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        c->dimens = info->dimens;
        c->flags |= arxCurveFlag_KEYFRAMED;
        c->knotCnt = info->knotCnt; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = info->dimens * info->knotCnt;
        c->ctrls = NIL;

        AFX_ASSERT(c->knotCnt == c->ctrlCnt / c->dimens);

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        c->dimens = info->dimens; // unused        
        c->knotCnt = info->knotCnt;
        c->knots = NIL;
        c->ctrlCnt = info->knotCnt * info->dimens;
        c->ctrls = NIL;

        if (c->knotCnt && AfxAllocate(c->knotCnt * sizeof(c->knots[0]), 0, AfxHere(), (void**)&c->knots))
            AfxThrowError();

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        if (!c->knotCnt)
        {
            if (c->ctrlCnt)
                c->flags |= arxCurveFlag_KEYFRAMED;
            else
                c->flags |= arxCurveFlag_IDENTITY;
        }
        else if (c->knotCnt == 1)
        {
            c->flags |= arxCurveFlag_CONSTANT;
        }
        break;
    }
    case arxCurveFormat_DaIdentity:
    {
        c->dimens = info->dimens;
        c->knotCnt = 0; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = 0; // unused
        c->ctrls = NIL; // unused
        c->flags |= arxCurveFlag_IDENTITY | arxCurveFlag_CONSTANT;
        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        c->dimens = info->dimens; // unused
        c->knotCnt = info->knotCnt; // unused
        AFX_ASSERT(c->knotCnt == 1);
        c->knots = NIL; // unused
        c->ctrlCnt = info->dimens;
        c->ctrls = NIL;

        if (c->ctrlCnt && AfxAllocate(c->ctrlCnt * sizeof(c->ctrls[0]), 0, AfxHere(), (void**)&c->ctrls))
            AfxThrowError();

        c->flags |= arxCurveFlag_CONSTANT;
        break;
    }
    case arxCurveFormat_D3Constant32f:
    case arxCurveFormat_D4Constant32f:
    {
        c->dimens = info->dimens; // unused
        c->knotCnt = 1; // unused
        c->knots = NIL; // unused
        c->ctrlCnt = 0; // unused
        c->ctrls = NIL; // unused
        c->flags |= arxCurveFlag_CONSTANT;
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
                //((void(__cdecl *)(arxCurve*, int, int, const float *))*(f))(c, sampleCnt, info->sampleDimension, info->originalSamples);
            }

            if (info->src)
            {
                ArxCopyCurve(c, info->src);
            }
            else
            {
                if (fmt != arxCurveFormat_DaKeyframes32f)
                {
                    ArxUpdateCurveKnots(c, info->knotCnt, info->dimens, info->knots, info->ctrls);
                }
                else
                {
                    ArxUpdateCurveKnots(c, info->knotCnt, info->dimens, info->knots ? info->knots : (afxReal const[]) { 0.f }, info->ctrls);
                }
            }
        }
    }
    return err;
}

_ARX afxError _ArxCurDtorCb(arxCurve c)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CUR, 1, &c);

    switch (c->fmt)
    {
    case arxCurveFormat_DaKeyframes32f:
    {
        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    case arxCurveFormat_DaK32fC32f:
    {
        if (c->knots)
            AfxDeallocate((void**)&c->knots, AfxHere());

        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    case arxCurveFormat_DaConstant32f:
    {
        if (c->ctrls)
            AfxDeallocate((void**)&c->ctrls, AfxHere());

        break;
    }
    default: break;
    }
    return err;
}

_ARX afxClassConfig const _ARX_CUR_CLASS_CONFIG =
{
    .fcc = afxFcc_CUR,
    .name = "Curve",
    .desc = "Geometric Curve",
    .fixedSiz = sizeof(AFX_OBJECT(arxCurve)),
    .ctor = (void*)_ArxCurCtorCb,
    .dtor = (void*)_ArxCurDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireCurves(afxSimulation sim, afxUnit cnt, arxCurveInfo const info[], arxCurve curves[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_ArxGetCurveClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_CUR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)curves, (void const*[]) { sim, info }))
        AfxThrowError();

    return err;
}
