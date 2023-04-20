#include "sgl.h"

#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxLegoSchema const *socket;
    afxLegoPoint const *points;
} _afxLegoParadigm;


_SGL afxError _SglLegoBindAndSync(afxLego lego, afxNat unit, afxDrawEngine deng)
{
    //AfxEntry("pip=%p", pip);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    sglVmt const* gl = &deng->wglVmt;
    
    AfxAssert(deng->state.pip);
    afxPipelineRig pipr = AfxPipelineGetRig(deng->state.pip);
    AfxAssertObject(pipr, AFX_FCC_PIPR);

    afxLegoSchema const *schema = &(lego->schema);
    afxLegoSchema const *schema2 = &(pipr->sockets[unit]);
    AfxAssert(schema->entryCnt == schema2->entryCnt);

    for (afxNat j = 0; j < schema->entryCnt; j++)
    {
        afxLegoSchemaEntry const *entry = &schema->entries[j];
        afxLegoSchemaEntry const *entry2 = &schema2->entries[j];
        AfxAssert(entry->binding == entry2->binding);
        AfxAssert(entry->visibility == entry2->visibility);
        AfxAssert(entry->type == entry2->type);

        afxLegoPoint const *point = &lego->points[j];

        afxNat setId = (unit * _SGL_MAX_ENTRY_PER_LEGO);
        afxNat binding = setId + entry->binding;

        switch (entry->type)
        {
        case AFX_SUPPLY_TYPE_SAMPLER:
        {
            AfxAssertObject(point->resource.img.smp, AFX_FCC_SMP);
            _SglSmpBindAndSync(point->resource.img.smp, binding, deng);
            break;
        }
        case AFX_SUPPLY_TYPE_SAMPLED_IMAGE:
        {
            AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
            _SglTexBindAndSync(point->resource.img.tex, binding, deng);
            break;
        }
        case AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            AfxAssertObject(point->resource.img.tex, AFX_FCC_TEX);
            _SglTexBindAndSync(point->resource.img.tex, binding, deng);

            AfxAssertObject(point->resource.img.smp, AFX_FCC_SMP);
            _SglSmpBindAndSync(point->resource.img.smp, binding, deng);
#if 0
            afxUri128 uri;
            AfxUri128(&uri, NIL);
            AfxTextureDownload(point->resource.img.tex, AfxUriFormat(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
#endif
            break;
        }
        case AFX_SUPPLY_TYPE_CONSTANT_BUFFER:
        {
            AfxAssertObject(point->resource.data.buf, AFX_FCC_BUF);

            // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

            //loc = gl->GetUniformBlockIndex(deng->state.pip->gpuHandle[deng->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
            //gl->UniformBlockBinding(deng->state.pip->gpuHandle[deng->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));
            _SglBufBindAndSync(point->resource.data.buf, binding, point->resource.data.base, point->resource.data.range, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW, deng); _SglThrowErrorOccuried();
            //gl->BindBufferRange(GL_UNIFORM_BUFFER, binding, point->resource.data.buf->gpuHandle, point->resource.data.base, point->resource.data.range); _SglThrowErrorOccuried();

            break;
        }
        default:
        {
            AfxError("");
        }
        }
    }
    return err;
}

_SGL afxLegoSchema const* _AfxLegoGetSchema(afxLego lego)
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    return &lego->schema;
}

_SGL afxError _AfxLegoCopy(afxLego lego, afxLego in)
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    AfxAssert(lego->schema.entryCnt >= in->schema.entryCnt);

    for (afxNat j = 0; j < lego->schema.entryCnt; j++)
    {
        lego->points[j] = in->points[j];
    }

    return err;
}

_SGL afxError _AfxLegoUpdate(afxLego lego, afxNat cnt, afxLegoPoint const points[])
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    AfxAssert(cnt);
    AfxAssert(points);

    for (afxNat j = 0; j < cnt; j++)
    {
        lego->points[j] = points[j];
    }

    return err;
}

_SGL afxError _AfxLegoDtor(afxLego lego)
{
    AfxEntry("pipa=%p", lego);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);

    return err;
}

_SGL afxLegoImpl const _AfxStdLegoImpl;
afxLegoImpl const _AfxStdLegoImpl =
{
    _AfxLegoUpdate,
    _AfxLegoCopy,
    _AfxLegoGetSchema
};

_SGL afxError _AfxLegoCtor(afxLego lego, _afxLegoParadigm const *paradigm)
{
    AfxEntry("lego=%p", lego);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    AfxAssert(paradigm);
    AfxAssert(paradigm->socket);
    AfxAssert(paradigm->points);

    lego->schema.entryCnt = 0;

    for (afxNat j = 0; j < paradigm->socket->entryCnt; j++)
    {
        AfxString16(&(lego->schema.entries[j].name), &(paradigm->socket->entries[j].name.str));
        lego->schema.entries[j].binding = paradigm->socket->entries[j].binding;
        lego->schema.entries[j].visibility = paradigm->socket->entries[j].visibility;
        lego->schema.entries[j].type = paradigm->socket->entries[j].type;

        lego->points[j].resource = paradigm->points[j].resource;

        lego->schema.entryCnt++;
    }

    if (AfxLegoUpdate(lego, lego->schema.entryCnt, paradigm->points))
        AfxThrowError();

    if (!err)
    {

    }

    return err;
}

_SGL afxLego _AfxDrawContextAcquireLego(afxDrawContext dctx, afxLegoSchema const *socket, afxLegoPoint const points[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxLego lego = NIL;

    _afxLegoParadigm paradigm =
    {
        dctx,
        socket,
        points
    };

    if (!(lego = AfxObjectAcquire(AfxDrawContextGetLegoClass(dctx), &paradigm, AfxSpawnHint())))
        AfxThrowError();

    return lego;
}

_SGL afxClassSpecification const _AfxLegoClassSpec;

afxClassSpecification const _AfxLegoClassSpec =
{
    AFX_FCC_LEGO,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxLego)),
    NIL,
    (void*)_AfxLegoCtor,
    (void*)_AfxLegoDtor,
    "Lego",
    &_AfxStdLegoImpl
};
