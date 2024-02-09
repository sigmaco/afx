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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/sim/anim/afxCurve2.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxQuaternion.h"

AFX_DEFINE_STRUCT(afxCurveDataHdr)
{
    afxInt8 fmt;
    afxInt8 degree;
};

AFX_DEFINE_STRUCT(afxCurveDataDaIdentity) /// Da, Identity
{
    afxCurveDataHdr hdr;
    afxInt16        dimension;
};

AFX_DEFINE_STRUCT(afxCurveDataDaConstant32f) /// Da, Constant32f
{
    afxCurveDataHdr hdr;
    afxNat          ctrlCnt;
    afxReal*        ctrls; // Constant32f
};

AFX_DEFINE_STRUCT(afxCurveDataDaKeyframes32f) /// Da, Keyframes32f
{
    afxCurveDataHdr hdr;
    afxInt16        dimension;
    afxNat          ctrlCnt;
    afxReal*        ctrls; // 32f
};

AFX_DEFINE_STRUCT(afxCurveDataDaK8uC8u) /// Da, K8u, C8u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxNat          ctrlScaleOffsetCount;
    afxReal*        ctrlScaleOffs;
    afxNat          knotCtrlCnt;
    afxChar*        knotsCtrls; // K8u, C8u
};

AFX_DEFINE_STRUCT(afxCurveDataDaK16uC16u) /// Da, K16u, C16u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxNat          ctrlScaleOffCnt;
    afxReal*        ctrlScaleOffs;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C16u
};

AFX_DEFINE_STRUCT(afxCurveDataDaK32fC32f) /// Da, K32f, C32f
{
    afxCurveDataHdr hdr;
    afxNat          knotCnt;
    afxReal*        knots; // K32f
    afxNat          ctrlCnt;
    afxReal*        ctrls; // C32f
};

AFX_DEFINE_STRUCT(afxCurveDataD3Constant32f) /// D3, Constant32f
{
    afxCurveDataHdr hdr;
    afxV3d         ctrls;
};

AFX_DEFINE_STRUCT(afxCurveDataD3K8uC8u) /// D3, K8u, C8u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffs;
    afxNat          knotCtrlCnt;
    afxChar*        knotsCtrls; // K8u, C8u
};

AFX_DEFINE_STRUCT(afxCurveDataD3K16uC16u) /// D3, K16u, C16u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffs;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C16u
};

AFX_DEFINE_STRUCT(afxCurveDataD3i1K8uC8u) /// D3i1, K8u, C8u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffs;
    afxNat          knotControlCnt;
    afxChar*        knotsCtrls; // K8u, C8u
};

AFX_DEFINE_STRUCT(afxCurveDataD3i1K16uC16u) /// D3i1, K16u, C16u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffs;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C16u
};

AFX_DEFINE_STRUCT(afxCurveDataD3i1K32fC32f) /// D3i1, K32f, C32f
{
    afxCurveDataHdr hdr;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffsets;
    afxNat          knotCtrlCnt;
    afxReal*        knotsCtrls; // K32f, C32f
};

AFX_DEFINE_STRUCT(afxCurveDataD4Constant32f) /// D4, Constant32f
{
    afxCurveDataHdr hdr;
    afxV4d          ctrls; // D4, Constant32f
};

AFX_DEFINE_STRUCT(afxCurveDataD4nK8uC7u) /// D4n, K8u, C7u
{
    afxCurveDataHdr hdr;
    afxNat16        scaleOffTabEntries;
    afxReal         oneOverKnotScale;
    afxNat          knotCtrlCnt;
    afxChar*        knotsCtrls; // K8u, C7u
};

AFX_DEFINE_STRUCT(afxCurveDataD4nK16uC15u) /// D4n, K16u, C15u
{
    afxCurveDataHdr hdr;
    afxNat16        scaleOffTabEntries;
    afxReal         oneOverKnotScale;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C15u
};

AFX_DEFINE_STRUCT(afxCurveDataD9i1K8uC8u) /// D9i1, K8u, C8u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxReal         ctrlScale;
    afxReal         ctrlOff;
    afxNat          knotControlCnt;
    afxChar*        knotsCtrls; // K8u, C8u
};

AFX_DEFINE_STRUCT(afxCurveDataD9i1K16uC16u) /// D9i1, K16u, C16u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxReal         ctrlScale;
    afxReal         ctrlOff;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C16u
};

AFX_DEFINE_STRUCT(afxCurveDataD9i3K8uC8u) /// D9i3, K8u, C8u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffsets;
    afxNat          knotCtrlCnt;
    afxChar*        knotsCtrls; // K8u, C8u
};

AFX_DEFINE_STRUCT(afxCurveDataD9i3K16uC16u) /// D9i3, K16u, C16u
{
    afxCurveDataHdr hdr;
    afxNat16        oneOverKnotScaleTrunc;
    afxV3d          ctrlScales;
    afxV3d          ctrlOffs;
    afxNat          knotCtrlCnt;
    afxNat16*       knotsCtrls; // K16u, C16u
};

typedef enum afxCurveFormat
{
    afxCurveFormat_DaIdentity, /// Da, Identity
    afxCurveFormat_DaConstant32f, /// Da, Constant32f
    afxCurveFormat_DaKeyframes32f, /// Da, Keyframes32f
    afxCurveFormat_DaK8uC8u, /// Da, K8u, C8u
    afxCurveFormat_DaK16uC16u, /// Da, K16u, C16u
    afxCurveFormat_DaK32fC32f, /// Da, K32f, C32f
    afxCurveFormat_D3Constant32f, /// D3, Constant32f
    afxCurveFormat_D3K8uC8u, /// D3, K8u, C8u
    afxCurveFormat_D3K16uC16u, /// D3, K16u, C16u
    afxCurveFormat_D3i1K8uC8u, /// D3i1, K8u, C8u
    afxCurveFormat_D3i1K16uC16u, /// D3i1, K16u, C16u
    afxCurveFormat_D3i1K32fC32f, /// D3i1, K32f, C32f
    afxCurveFormat_D4Constant32f, /// D4, Constant32f
    afxCurveFormat_D4nK8uC7u, /// D4n, K8u, C7u
    afxCurveFormat_D4nK16uC15u, /// D4n, K16u, C15u
    afxCurveFormat_D9i1K8uC8u, /// D9i1, K8u, C8u
    afxCurveFormat_D9i1K16uC16u, /// D9i1, K16u, C16u    
    afxCurveFormat_D9i3K8uC8u, /// D9i3, K8u, C8u
    afxCurveFormat_D9i3K16uC16u /// D9i3, K16u, C16u
} afxCurveFormat;

AFX_DEFINE_UNION(afxCurveData)
{
    afxCurveDataDaIdentity  daIdentity; /// Da, Identity
    afxCurveDataDaConstant32f daConst; /// Da, Constant32f
    afxCurveDataDaKeyframes32f daKeys;/// Da, Keyframes32f
    afxCurveDataDaK8uC8u daKC8u; /// Da, K8u, C8u
    afxCurveDataDaK16uC16u daKC16u;/// Da, K16u, C16u
    afxCurveDataDaK32fC32f daKC32f;/// Da, K32f, C32f
    afxCurveDataD3Constant32f d3Const; /// D3, Constant32f
    afxCurveDataD3K8uC8u d3KC8u; /// D3, K8u, C8u
    afxCurveDataD3K16uC16u d3KC16u; /// D3, K16u, C16u
    afxCurveDataD3i1K8uC8u d3i1KC8u; /// D3i1, K8u, C8u
    afxCurveDataD3i1K16uC16u d3i1KC16u; /// D3i1, K16u, C16u
    afxCurveDataD3i1K32fC32f d3i1KC32f; /// D3i1, K32f, C32f
    afxCurveDataD4Constant32f d4Const; /// D4, Constant32f
    afxCurveDataD4nK8uC7u d4nK8C7u; /// D4n, K8u, C7u
    afxCurveDataD4nK16uC15u d4nK16C15u; /// D4n, K16u, C15u
    afxCurveDataD9i1K8uC8u d9i1KC8u; /// D9i1, K8u, C8u
    afxCurveDataD9i1K16uC16u d9i1KC16u; /// D9i1, K16u, C16u    
    afxCurveDataD9i3K8uC8u d9i3KC8u; /// D9i3, K8u, C8u
    afxCurveDataD9i3K16uC16u d9i3KC16u; /// D9i3, K16u, C16u
};

AFX_DEFINE_STRUCT(afxCurveBlueprint)
{
    afxBool         curveBuilderNeedsFreeing;
    void*           typeDefinition;
    afxNat          fmtEnum;
    afxNat          degree;
    afxNat          dimension;
    afxNat          knotCnt;
    afxReal const*  knotArray;
    afxReal const*  ctrlArray;
    afxCurve const*srcCurve;
    afxNat          sampleCnt;
    afxNat          sampleDimension;
    afxReal const*  transformedSamples;
    afxReal const*  originalSamples;
};

AFX_DEFINE_STRUCT(afxCurveTypeTable)
{
    void* TypeDefinition;
    char *ReadableName;
    int(*GetDimension)(afxCurve const*);
    int(*GetKnotCount)(afxCurve const*);
    int(*FindKnot)(afxCurve const*, afxReal);
    int(*FindCloseKnot)(afxCurve const*, afxReal, afxInt);
    void(*ExtractKnotValues)(afxCurve const*, afxInt, afxInt, afxReal*, afxReal*, afxReal const*);
    void(*TransformSamples)(afxInt, afxInt, afxReal*);
    void(*NoteSampleTransform)(afxCurve*, afxInt, afxInt, afxReal const*);
    void(*SetAllKnotValues)(afxCurve*, afxInt, afxInt, afxReal const*, afxReal const*);
    void(*AggrCurveData)(void*/*allocator*/, afxCurveBlueprint const*, void **);
    void(*CopyCurve)(afxCurve*, afxCurve const*);
    void(*ScaleOffsetSwizzle)(afxCurve*, afxInt, afxReal const*, afxReal const*, afxInt const*);
};

_AFX void AfxCurveScaleOffsetSwizzle(afxCurve *cur, afxInt dimension, afxReal const* scales, afxReal const* offsets, afxInt const* swizzles)
{
    afxCurveTypeTable *vmt;
    vmt->ScaleOffsetSwizzle(cur, dimension, scales, offsets, swizzles);
}

_AFX afxInt AfxCurveFindKnot(afxCurve const *cur, afxReal t)
{
    afxCurveTypeTable *vmt;
    vmt->FindKnot(cur, t);
}

_AFX afxInt AfxCurveFindCloseKnot(afxCurve const *cur, afxReal t, afxInt startingKnotIdx)
{
    afxCurveTypeTable *vmt;
    vmt->FindCloseKnot(cur, t, startingKnotIdx);
}

_AFX afxInt AfxCurveGetKnotCount(afxCurve const *cur)
{
    afxCurveTypeTable *vmt;
    vmt->GetKnotCount(cur);
}

_AFX void AfxCurveGetDimensionUnchecked(afxCurve const *cur)
{
    afxCurveTypeTable *vmt;
    vmt->GetDimension(cur);
}

_AFX afxInt AfxCurveGetDimension(afxCurve const *cur)
{
    if (AfxCurveGetKnotCount(cur))
        AfxCurveGetDimensionUnchecked(cur);
}

_AFX afxInt AfxCurveGetDegree(afxCurve const *cur)
{
    afxCurveDataHdr const *hdr = cur;
    return hdr->degree;
}

_AFX afxBool AfxCurveIsTypeDaK32fC32f(afxCurve const* cur)
{
    afxCurveDataHdr const *hdr = cur;
    return hdr->fmt == 1;
}

_AFX afxBool AfxCurveIsKeyframed(afxCurve const* cur)
{
    afxCurveDataHdr const *hdr = cur;
    return !hdr->fmt || hdr->fmt == 1 && !hdr->degree && hdr->degree > 0;
}

_AFX afxCurveDataDaK32fC32f* AfxCurveGetContentsOfDaK32fC32f(afxCurve const* src)
{
    return (void*)(AfxCurveIsTypeDaK32fC32f(src) ? src : NIL);
}

_AFX void AfxCurveMakeStaticDaK32fC32f(afxCurve *cur, afxCurveDataDaK32fC32f* data, afxInt knotCnt, afxInt degree, afxInt dimension, afxReal const* knots, afxReal const* ctrls)
{
    data->hdr.degree = degree;
    data->hdr.fmt = 1;
    data->knotCnt = knotCnt;
    data->ctrlCnt = dimension * knotCnt;
    data->knots = (float *)knots;
    data->ctrls = (float *)ctrls;
}

_AFX afxInt AfxGetResultingDaK32fC32fCurveSize(afxNat knotCnt, afxInt dimension)
{
    return 4 * knotCnt * (dimension + 1) + 20;
}

_AFX afxReal AfxCurveExtractKnotValue(afxCurve const* cur, afxInt knotIdx)
{
    afxCurveTypeTable *vmt;
    afxReal rslt = 0.0;
    vmt->ExtractKnotValues(cur, knotIdx, 1, &rslt, NIL, NIL);
    return rslt;
}

_AFX void AfxCurveExtractKnotValues(afxCurve const* cur, afxInt knotIdxStart, afxInt knotCnt, afxReal* knotRslts, afxReal* ctrlRslts, afxReal const* identityVec)
{
    afxCurveTypeTable *vmt;
    vmt->ExtractKnotValues(cur, knotIdxStart, knotCnt, knotRslts, ctrlRslts, identityVec);
}

#if 0
_AFX afxBool AfxCurveIsIdentity(afxCurve const *cur)
{
    afxCurveDataHdr const *hdr = cur;
    unsigned __int8 *v1; // eax
    int v2; // ecx

    v1 = (unsigned __int8 *)curv->CurveData.Object;
    v2 = *v1;
    return hdr->fmt == 2 || hdr->fmt == 1 && !*((_DWORD *)v1 + 1) && !*((_DWORD *)v1 + 3);
}

_AFX afxBool AfxCurveIsConstantOrIdentity(afxCurve const* cur)
{
    afxCurveDataHdr const *hdr = cur;

    unsigned __int8 *v1;
    bool result; // al
    int v4; // eax

    if (hdr->fmt == 2 || hdr->fmt >= 3 && hdr->fmt <= 5)
        return 1;

    if (hdr->fmt != 1)
        return 0;

    v4 = *((_DWORD *)v1 + 1);
    if (v4)
        result = v4 == 1;
    else
        result = *((_DWORD *)v1 + 3) == 0;
    return result;
}

_AFX afxBool AfxCurveIsConstantNotIdentity(afxCurve const* cur)
{
    return AfxCurveIsConstantOrIdentity(curv) && !AfxCurveIsIdentity(curv);
}
#endif

_AFX void AfxFreeCurve(afxCurve *curv)
{
    AfxDeallocate(NIL, curv);
}

_AFX afxCurve* AfxCurveConvertToDaK32fC32f(afxCurve const*SrcCurve, afxReal const * IdentityVector)
{

}

_AFX afxCurve* AfxCurveConvertToDaK32fC32fInPlace(afxCurve const*SrcCurve, void * Memory, afxReal const * IdentityVector)
{

}

#if 0
_AFX void AfxCurveSetAllKnotValues(afxCurve *curv, afxInt KnotCount, afxInt Dimension, afxReal const * KnotSrc, afxReal const * ControlSrc)
{
    int v5; // esi
    int v6; // eax
    int v7; // eax
    const char *Dimensiona; // [esp+18h] [ebp+Ch]
    const float *ControlSrca; // [esp+20h] [ebp+14h]

    v5 = *(unsigned __int8 *)curv->CurveData.Object;
    granny::CurveGetDimension(Curve);
    if (Dimension != v6)
    {
        ControlSrca = (const float *)1466;
        Dimensiona = "%s(%d): Curve and argument dimensions do not match";
    LABEL_5:
        granny::_Log(ErrorLogMessage, CurveLogMessage, Dimensiona, "d:/dev/rad/granny/rt/granny_curve.cpp", ControlSrca);
        return;
    }
    granny::CurveGetKnotCount(Curve);
    if (KnotCount != v7)
    {
        ControlSrca = (const float *)1471;
        Dimensiona = "%s(%d): Curve and argument knot counts do not match";
        goto LABEL_5;
    }
    ((void(__cdecl *)(granny::curve2 *, int, int, const float *, const float *))*(&off_500792EC + 13 * v5))(
        Curve,
        KnotCount,
        Dimension,
        KnotSrc,
        ControlSrc);

    // 500792EC: using guessed type void (__cdecl *off_500792EC)(granny::curve2 *Curve, struct granny::curve2 *KnotCount, int Dimension, const float *KnotSrc, const float *ControlSrc, const float *);
}

_AFX afxInt AfxCurveGetSize(afxCurve2 const *curv)
{
    int v2; // esi
    granny::curve_builder Builder; // [esp+4h] [ebp-34h]

    granny::BeginCurveCopyInPlace(&Builder, Curve);
    v2 = granny::GetResultingCurveSize(a1, &Builder);
    granny::AbortCurveBuilder(&Builder);
    return v2;
}
#endif

char ConstructBSplineBuffers(afxInt dim, afxCurve const* prevCur, afxCurve const* cur, afxCurve const* nextCur, afxReal prevCurDur, afxReal curDur, afxReal nextCurDur, afxInt knotIdx, afxReal* ti, afxReal* pi, afxReal** tiPtr, afxReal** piPtr, afxReal const* identityVec)
{
    int v13; // edi
    int v14; // eax
    int v15; // esi
    int v25; // ecx
    int v26; // ebp
    int v31; // edx
    int v37; // eax
    int v46; // eax
    
    afxCurveDataHdr* hdr = cur;

    afxInt degree = hdr->degree;
    afxNat knotCnt = AfxCurveGetKnotCount(cur);
    
    if (knotCnt > 0 && nextCur)
        --knotCnt;
    
    afxInt doubledDegree = 2 * degree;
    afxInt knotIdxStart = knotIdx - degree;
    afxBool underflow = knotIdxStart < 0;
    afxBool overflow = 2 * degree + knotIdx - degree > knotCnt;

    if (!underflow && !overflow)
    {
        afxCurveDataDaK32fC32f *v19 = AfxCurveGetContentsOfDaK32fC32f(cur);

        if (v19)
        {
            *tiPtr = &v19->knots[knotIdx];
            *piPtr = &v19->ctrls[knotIdx * dim];
        }
        else
        {
            *tiPtr = &ti[degree];
            *piPtr = &pi[dim * degree];
            AfxCurveExtractKnotValues(cur, knotIdxStart, doubledDegree, ti, pi, identityVec);
        }
        return 0;
    }

    *tiPtr = &ti[degree];
    *piPtr = &pi[dim * degree];
    afxInt iter = 0;
    afxInt involvedKnotCnt = 2 * degree;
    afxInt timeOffset = 0;

    if (underflow)
    {
        afxInt v22 = -knotIdxStart;
        iter = v22;
        involvedKnotCnt = 2 * degree - v22;
        timeOffset = v22;
        knotIdxStart = 0;
    }

    if (overflow)
        involvedKnotCnt = knotCnt - knotIdxStart;

    if (involvedKnotCnt > 0)
        AfxCurveExtractKnotValues(cur, knotIdxStart, involvedKnotCnt, &ti[iter], &pi[dim * iter], identityVec);

    if (underflow)
    {
        if (prevCur)
        {
            if (AfxCurveIsIdentity(prevCur))
            {
                afxReal* tiIter = ti;
                afxReal* piIter = pi;
                afxReal64 v29 = ti[iter] - (afxReal64)timeOffset * prevCurDur;

                if (iter > 0)
                {
                    afxInt iterRev = iter;
                    afxBool endReached;

                    do
                    {
                        *tiIter = v29;
                        ++tiIter;

                        afxReal const* v30 = identityVec;

                        if (dim)
                        {
                            v31 = dim;

                            do
                            {
                                *(afxReal const*)((char *)v30 + (char *)piIter - (char *)identityVec) = *v30;
                                ++v30;
                                --v31;
                            } while (v31);
                        }
                        v29 = v29 + prevCurDur;
                        endReached = iterRev == 1;
                        piIter += dim;
                        --iterRev;
                    } while (!endReached);
                }
            }
            else
            {
                afxNat prevKnotCnt = AfxCurveGetKnotCount(prevCur);

                if (prevKnotCnt > 1)
                    --prevKnotCnt;

                afxReal timeOffsetc = -prevCurDur;
                afxReal* v39 = &ti[iter - 1];
                afxInt prevInvolvedKnotCnt = prevKnotCnt - 1;
                afxInt prevInvolvedKnotCntRemain = prevInvolvedKnotCnt;
                afxReal* Curved = &pi[dim * (iter - 1)];

                if (iter - 1 >= 0)
                {
                    afxReal *tiDst = (float *)iter;

                    do
                    {
                        AfxCurveExtractKnotValues(prevCur, prevInvolvedKnotCntRemain, 1, v39, Curved, identityVec);
                        *v39 = timeOffsetc + *v39;
                        --v39;
                        --prevInvolvedKnotCntRemain;
                        Curved -= dim;

                        if (prevInvolvedKnotCntRemain < 0)
                        {
                            prevInvolvedKnotCntRemain = prevInvolvedKnotCnt;
                            timeOffsetc = timeOffsetc - prevCurDur;
                        }
                        tiDst = (float *)((char *)tiDst - 1);
                    } while (tiDst);
                }
            }
        }
        else
        {
            afxReal* v23 = ti;
            afxReal* Curvea = pi;
            afxReal* v24 = &pi[dim * iter];

            if (iter > 0)
            {
                afxInt TimeOffseta = iter;
                afxBool endReached;

                do
                {
                    *v23 = ti[iter];
                    ++v23;
                    if (dim)
                    {
                        v25 = (char *)Curvea - (char *)v24;
                        v26 = dim;

                        do
                        {
                            *(float *)((char *)v24 + v25) = *v24;
                            ++v24;
                            --v26;
                        } while (v26);
                        v24 = &pi[dim * iter];
                    }

                    endReached = TimeOffseta == 1;
                    Curvea += dim;
                    --TimeOffseta;
                } while (!endReached);
            }
        }
    }
    
    if (overflow)
    {
        afxInt v32 = iter + involvedKnotCnt - 1;

        if (!nextCur)
        {
            afxReal* v33 = &pi[dim * v32];
            afxReal* v34 = &ti[v32 + 1];
            afxCurve const* Curvec = (const afxCurve*)&v33[dim];

            if (iter + involvedKnotCnt < doubledDegree)
            {
                afxInt KnotIndexb = doubledDegree - (iter + involvedKnotCnt);
                afxBool endReached;

                do
                {
                    *v34 = ti[v32];
                    ++v34;
                    afxReal* v35 = &pi[dim * v32];

                    if (dim)
                    {
                        afxInt v36 = dim;

                        do
                        {
                            *(float *)((char *)v35 + (char *)Curvec - (char *)v33) = *v35;
                            ++v35;
                            --v36;
                        } while (v36);
                    }

                    endReached = KnotIndexb == 1;
                    Curvec = (const afxCurve*)((char *)Curvec + 4 * dim);
                    --KnotIndexb;
                } while (!endReached);
                return 1;
            }
            return 1;
        }

        if (AfxCurveIsIdentity(nextCur))
        {
            afxReal64 v40 = ti[v32];
            afxReal* v41 = &pi[dim * (iter + involvedKnotCnt)];
            afxReal* v42 = &ti[v32 + 1];
            afxCurve const* Curvee = &pi[dim * (iter + involvedKnotCnt)];

            if (iter > 0)
            {
                do
                {
                    v40 = v40 + nextCurDur;
                    afxReal const* v43 = identityVec;
                    ++v42;
                    *(v42 - 1) = v40;

                    if (dim)
                    {
                        afxInt v44 = (char *)v41 - (char *)identityVec;
                        afxInt v45 = dim;

                        do
                        {
                            *(const float *)((char *)v43 + v44) = *v43;
                            ++v43;
                            --v45;
                        } while (v45);
                    }

                    v41 = (float *)(&Curvee->CurveData.Type + dim);
                    --iter;
                    Curvee = (const afxCurve*)((char *)Curvee + 4 * dim);
                } while (iter);
            }
            return 1;
        }

        afxNat nextKnotCnt = AfxCurveGetKnotCount(nextCur);
        afxInt KnotIndexc = nextKnotCnt;

        if (nextKnotCnt > 1)
            KnotIndexc = nextKnotCnt - 1;

        afxInt v47 = iter + involvedKnotCnt;
        afxReal* v48 = &ti[v32 + 1];
        afxReal* v49 = &pi[dim * (iter + involvedKnotCnt)];
        afxInt v50 = 0;
        afxReal Curvef = curDur;

        if (v47 >= doubledDegree)
            return 1;

        afxReal* tiDsta = (afxReal*)(doubledDegree - v47);

        do
        {
            AfxCurveExtractKnotValues(nextCur, v50, 1, v48, v49, identityVec);
            afxReal64 v51 = Curvef + *v48;
            ++v48;
            *(v48 - 1) = v51;
            v49 += dim;

            if (++v50 >= KnotIndexc)
            {
                v50 = 0;
                Curvef = Curvef + nextCurDur;
            }
            tiDsta = (float *)((char *)tiDsta - 1);
        } while (tiDsta);
    }
    return 1;
}

_AFXINL void SampleBSpline(afxInt Degree, afxInt Dimension, afxBool Normalize, afxReal const* ti, afxReal const* pi, afxReal t, afxReal* Result)
{
    afxError err = NIL;
    AfxAssertRangef(10.f, 0, Dimension);
    AfxAssertRangef(4,);

    int v7; // edx
    void(__cdecl *v8)(const float *, const float *, float, float *); // edx
    const char *Normalizea; // [esp+Ch] [ebp+Ch]
    const float *pia; // [esp+14h] [ebp+14h]
    int ta; // [esp+18h] [ebp+18h]
    float *Resulta; // [esp+1Ch] [ebp+1Ch]

    if (Degree < 0 || Degree >= 4)
    {
        Resulta = (float *)4;
        pia = (const float *)555;
        Normalizea = "%s(%d): Degree %d is out of range [0, %d)";
    LABEL_13:
        ta = Degree;
        goto LABEL_14;
    }
    if (Dimension >= 0 && Dimension < 10)
    {
        v7 = Dimension + 10 * Degree;
        if (Normalize)
            v8 = NormalizedEvaluators[0][v7];
        else
            v8 = Evaluators[0][v7];
        if (v8)
        {
            ((void(__cdecl *)(const float *, const float *, _DWORD, float *))v8)(ti, pi, LODWORD(t), Result);
            return;
        }
        Resulta = (float *)Dimension;
        pia = (const float *)571;
        Normalizea = "%s(%d): Unrecognized degree/dimension combination (%d/%d)";
        goto LABEL_13;
    }
    Resulta = (float *)10;
    ta = Dimension;
    pia = (const float *)556;
    Normalizea = "%s(%d): Dimension %d is out of range [0, %d)";
LABEL_14:
    granny::_Log(
        ErrorLogMessage,
        BSplineLogMessage,
        Normalizea,
        "d:/dev/rad/granny/rt/granny_bspline.cpp",
        pia,
        ta,
        Resulta);
}

_AFXINL void AfxEvaluateCurveAtKnotIndex(afxInt dimension, afxBool normalize, afxBool backwardsLoop, afxCurve const* cur, afxBool forwardsLoop, afxReal curDur, afxInt knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    afxCurveDataHdr const* hdr = (void const*)cur;

    int v12; // edi
    afxReal* tiBuffer;
    afxReal* piBuffer;
    afxReal tiBufferSpace[8];
    afxReal piBufferSpace[64];

    if (AfxCurveIsIdentity(cur))
    {
        afxReal const* i;
        afxInt v10 = dimension;

        for (i = identityVec; v10; --v10)
        {
            *(afxReal*)((char *)i + (char *)rslt - (char *)identityVec) = *i;
            ++i;
        }
    }
    else if (AfxCurveIsConstantOrIdentity(cur))
    {
        AfxCurveExtractKnotValue(cur, 0, cade o restante);
    }
    else
    {
        afxNat degree = hdr->degree;

        afxNat v14 = AfxCurveGetKnotCount(cur);
        afxNat v13 = knotIdx;
        
        if (knotIdx == v14)
            v13 = knotIdx - 1;

        if (ConstructBSplineBuffers(dimension, backwardsLoop ? cur : NIL, cur, forwardsLoop?cur :NIL, curDur, curDur, curDur, v13, ti
            dimension,
            (const granny::curve2 *)(backwardsLoop ? (unsigned int)cur : 0),
            cur,
            (const granny::curve2 *)(forwardsLoop ? (unsigned int)cur : 0),
            curDur, curDur, curDur, v13, tiBufferSpace, piBufferSpace, &tiBuffer, &piBuffer, identityVec) && normalize && dimension == 4)
        {
            AfxEnsureQuaternionContinuity(degree + 1, piBufferSpace);
        }
        SampleBSpline(degree, dimension, normalize, tiBuffer, piBuffer, t, rslt);
    }
}

_AFXINL void AfxEvaluateCurveAtT(afxInt dimension, afxBool normalize, afxBool backwardsLoop, afxCurve const* cur, afxBool forwardsLoop, afxReal curveDur, afxReal t, afxReal* rslt, afxReal const* identityVec)
{
    afxNat v9 = AfxCurveFindKnot(cur, t);
    AfxEvaluateCurveAtKnotIndex(dimension, normalize, backwardsLoop, cur, forwardsLoop, curveDur, v9, t, rslt, identityVec);
}
