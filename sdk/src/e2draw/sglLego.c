#include "sgl.h"

#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxLegoSchema const *socket;
    afxLegoPoint const *points;
} _afxLegoParadigm;

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
