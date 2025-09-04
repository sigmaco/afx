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
#define _ASX_BUFFER_C
#include "afx/src/sim/impl/asxImplementation.h"
#include "afx/src/sim/impl/asxImpl_Context.h"


_ASX afxError _AsxSmemDtorCb(_asxMemory* smem)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &smem);
#if 0
    asxBuffer sbuf;
    AFX_ITERATE_CHAIN(AFX_OBJECT(asxBuffer), sbuf, mem, &smem->buffers)
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

_ASX afxError _AsxSmemCtorCb(_asxMemory* smem, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SBUF, 1, &smem);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    asxMemoryReq const* req = ((asxMemoryReq const*)args[1]) + invokeNo;
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

_ASX afxClassConfig const _ASX_SMEM_CLASS_CONFIG =
{
    .fcc = afxFcc_SMEM,
    .name = "SynergicMemory",
    .desc = "Synergic Memory",
    .fixedSiz = sizeof(_asxMemory),
    .ctor = (void*)_AsxSmemCtorCb,
    .dtor = (void*)_AsxSmemDtorCb
};

#if 0
_ASX afxError AfxReallocateBuffer(asxBuffer sbuf, afxUnit unit, afxSize offset)
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
        if (AfxAcquireObjects(smemCls, 1, &smem, (void*[]) { AfxGetProvider(sbuf), &req }))
        {
            AfxThrowError();
        }

        AfxPushLink(&sbuf->mem, &smem->buffers);
        sbuf->memBase = 0;
    }
    return err;
}
#endif
