/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ASX_POSE_C
#define _ARX_PLACEMENT_C
#define _ARX_BODY_C
#define _ARX_MODEL_C
#define _ARX_ANIMATION_C
#define _ARX_MOTION_C
#define _ARX_MOTOR_C
#define _ASX_SIM_BRIDGE_C
#define _ASX_SIM_QUEUE_C
#define _ASX_CONTEXT_C
#define _ARX_BUFFER_C
#include "afx/src/render/ddi/arxImpl_Input.h"

_ARX afxError _ArxSmemDtorCb(_arxMemory* smem)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &smem);
#if 0
    arxBuffer sbuf;
    AFX_ITERATE_CHAIN(AFX_OBJECT(arxBuffer), sbuf, mem, &smem->buffers)
    {
        AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

        while (smem->mapPtr)
        {
            AsxUnmapBuffer(sbuf, TRUE);
            AfxYield();
            //AFX_ASSERT(!buf->bytemap);
        }

        AfxPopLink(&sbuf->mem);
        sbuf->memBase = 0;
    }
#endif
    void* p = smem->hostedAlloc.bytemap;

    AfxDeallocate(&p, AfxHere());

    return err;
}

_ARX afxError _ArxSmemCtorCb(_arxMemory* smem, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &smem);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxMemoryReq const* req = ((arxMemoryReq const*)args[1]) + invokeNo;
    AFX_ASSERT(req);

    AfxResetLink(&smem->iommu);
    //AfxDeployChain(&smem->buffers, smem);

    smem->size = req->size;
    smem->alignment = AFX_SIMD_ALIGNMENT;

    smem->permanentlyMapped = FALSE;
    smem->pendingRemap = 0;
    smem->mapOffset = 0;
    smem->mapRange = 0;
    smem->mapFlags = 0;
    smem->mapPtr = NIL;

    smem->hostedAlloc.bytemap = NIL;

    AfxAllocate(smem->size, smem->alignment, AfxHere(), (void**)&smem->hostedAlloc.bytemap);

    return err;
}

_ASX afxClassConfig const _ARX_SMEM_CLASS_CONFIG =
{
    .fcc = afxFcc_SMEM,
    .name = "SynergicMemory",
    .desc = "Synergic Memory",
    .fixedSiz = sizeof(_arxMemory),
    .ctor = (void*)_ArxSmemCtorCb,
    .dtor = (void*)_ArxSmemDtorCb
};

#if 0
_ASX afxError AfxReallocateBuffer(arxBuffer sbuf, afxUnit unit, afxSize offset)
{
    afxError err;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &sbuf);

    _asxMemory* sbs = AfxGetLinker(&sbuf->mem);

    if (sbs)
    {
        afxUnit sbsIdx;
        AfxFindPoolUnit(&pool, sbs, &sbsIdx, NIL);

        if (sbsIdx != unit)
        {

        }
    }
    else
    {
        afxClass* smemCls = NIL;

        asxMemoryReq req = { 0 };
        req.size = sbuf->reqSiz;
        req.propFlags = sbuf->reqMemType;

        _asxMemory* smem;
        if (AfxAcquireObjects(smemCls, 1, &smem, (void*[]) { AfxGetHost(sbuf), &req }))
        {
            AfxThrowError();
        }

        AfxPushLink(&sbuf->mem, &smem->buffers);
        sbuf->memBase = 0;
    }
    return err;
}
#endif
