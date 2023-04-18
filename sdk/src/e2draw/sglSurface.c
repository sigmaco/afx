
#include "sgl.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/res/afxSurface.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"

AFX_DEFINE_STRUCT(_afxSurfCtorArgs)
{
    afxTextureParadigm tex;
};

_SGL afxError _AfxStdSurfUpdate(afxSurface surf, afxDrawEngine deng)
{
    afxError err = NIL;

    sglVmt const* gl = &deng->wglVmt;

    if (surf->tex.layoutAltered)
    {
        gl->DeleteRenderbuffers(1, &(surf->gpuRboHandle)); _SglThrowErrorOccuried();
        surf->gpuRboHandle = NIL;
    }

    if (!surf->gpuRboHandle)
    {
        gl->GenRenderbuffers(1, &(surf->gpuRboHandle)); _SglThrowErrorOccuried();
        gl->BindRenderbuffer(GL_RENDERBUFFER, surf->gpuRboHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsRenderBuffer(surf->gpuRboHandle));
        AfxAssert(1 < surf->tex.whd[1]);
        gl->RenderbufferStorage(GL_RENDERBUFFER, surf->tex.glIntFmt, surf->tex.whd[0], surf->tex.whd[1]); _SglThrowErrorOccuried();
        gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();
    }
    return err;
}


_SGL afxSurface _AfxDrawContextAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(extent);
    AfxAssert(extent[0]);
    AfxAssert(extent[1]);
    AfxAssert(extent[2]);
    AfxAssert(usage & AFX_TEX_USAGE_SURFACE_RASTER | AFX_TEX_USAGE_SURFACE_DEPTH);
    AfxEntry("dctx=%p,fmt=%u,whd=[%u,%u,%u],usage=%u", dctx, fmt, extent[0], extent[1], extent[2], usage);
    afxSurface surf = NIL;

    afxTextureBlueprint texb;
    AfxTextureBlueprintDeploy(&texb, NIL, fmt, extent, 1, 1, usage);
    AfxTextureBlueprintDefineLayer(&texb, 0, NIL, fmt, NIL);

    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);

    _afxSurfCtorArgs args =
    {
        {.res = { AfxObjectGetProvider(&dsys->obj), NIL }, dctx, &texb }
    };

    if (!(surf = AfxObjectAcquire(AfxDrawContextGetSurfaceClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return surf;
};

_SGL afxError _AfxSurfDtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = NIL;
    AfxAssertObject(surf, AFX_FCC_SURF);

    while (1)
    {
        if (!surf->swapchain.chain) break;
        else
        {
            AfxAssert(surf->state == AFX_SURF_STATE_PENDING);

            // AfxLinkageDrop(&canv->queue); // we can't do it here. We need wait for draw context to liberate it.

            afxDrawOutput dout = AfxLinkageGetOwner(&surf->swapchain);
            AfxAssertObject(dout, AFX_FCC_DOUT);
            //_AfxDrawOutputProcess(dout); // process until draw output ends its works and unlock this canvas.
            AfxYield();
        }
    }

    if (surf->gpuRboHandle)
    {
        afxDrawContext dctx = AfxTextureGetContext(&surf->tex);
        _SglEnqueueGlResourceDeletion(dctx, 0, 2, surf->gpuRboHandle);
        surf->gpuRboHandle = 0;
    }

    return err;
}

_SGL afxError _AfxSurfCtor(afxSurface surf, void *args)
{
    (void)args;
    AfxEntry("surf=%p", surf);
    afxError err = NIL;
    AfxAssertObject(surf, AFX_FCC_SURF);

    surf->state = AFX_SURF_STATE_IDLE;

    AfxLinkageDeploy(&surf->swapchain, NIL);

    surf->gpuRboHandle = 0;

    return err;
}

_SGL afxSurfImpl const _AfxStdSurfImpl;
afxSurfImpl const _AfxStdSurfImpl =
{
    {
        _AfxTextureGetColorSwizzling,
        _AfxTextureGetData,
        //_AfxTextureMeasure,
        _AfxTextureGetExtent,
        _AfxTextureGenerateLods,
        _AfxTextureGetFormat,
        _AfxTextureGetLodCount,
        _AfxTextureGetLayerCount,
        _AfxTextureGetSampleCount,
        _AfxTextureMap,
        _AfxTextureSetExtent,
        _AfxTextureTestFlags,
        _AfxTextureTestUsageFlags,
        _AfxTextureUnmap,
        _AfxTextureUpdateRegions
    }
};

_SGL afxClassSpecification const _AfxSurfClassSpec;

afxClassSpecification const _AfxSurfClassSpec =
{
    AFX_FCC_SURF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxSurface)),
    NIL,
    (void*)_AfxSurfCtor,
    (void*)_AfxSurfDtor,
    "Surface",
    &_AfxStdSurfImpl
};
