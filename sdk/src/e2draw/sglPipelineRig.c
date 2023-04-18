#include "sgl.h"

#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxNat socketCnt;
    afxLegoSchema const *socket;
} _afxPipaParadigm;

_SGL afxResult _AfxPipelineRigForEachSocket(afxPipelineRig pipr, afxNat first, afxNat cnt, afxResult(*f)(afxLegoSchema const*, void*), void *data)
{
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    AfxAssert(cnt);
    AfxAssert(cnt <= pipr->socketCnt - first);
    AfxAssert(f);
    afxResult cnt2 = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxLegoSchema const *sch = &pipr->sockets[i];
        cnt2++;

        if (!f(sch, data))
            break;
    }
    return cnt2;
}

_SGL afxResult _AfxPipelineRigFindEntries(afxPipelineRig pipr, afxNat socket, afxNat cnt, afxString const names[], afxLegoSchemaEntry entries[])
{
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    AfxAssert(cnt);
    AfxAssert(socket <= pipr->socketCnt);
    AfxAssert(names);
    AfxAssert(entries);
    afxResult hitCnt = 0;

    for (afxNat h = 0; h < cnt; h++)
    {
        for (afxNat j = 0; j < pipr->sockets[socket].entryCnt; j++)
        {
            if (0 == AfxStringCompareCi(&names[h], &(pipr->sockets[socket].entries[j].name.str)))
            {
                entries[j] = pipr->sockets[socket].entries[j];
                hitCnt++;
            }
        }
    }
    return hitCnt;
}

_SGL afxResult _AfxPipelineRigGetSockets(afxPipelineRig pipr, afxNat first, afxNat cnt, afxLegoSchema sockets[])
{
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    AfxAssert(cnt);
    AfxAssert(sockets);
    AfxAssert(cnt <= pipr->socketCnt - first);
    afxResult hitCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        sockets[i] = pipr->sockets[first + i];
    }
    return hitCnt;
}

_SGL afxNat _AfxPipelineRigGetSocketCount(afxPipelineRig pipr)
{
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);
    return pipr->socketCnt;
}

_SGL afxError _AfxPiprDtor(afxPipelineRig pipr)
{
    AfxEntry("pipr=%p", pipr);
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);

    return err;
}

_SGL afxPipelineRigImpl const _AfxStdPiprImpl;
afxPipelineRigImpl const _AfxStdPiprImpl =
{
    _AfxPipelineRigFindEntries,
    _AfxPipelineRigForEachSocket,
    _AfxPipelineRigGetSocketCount,
    _AfxPipelineRigGetSockets
};

_SGL afxError _AfxPiprCtor(afxPipelineRig pipr, _afxPipaParadigm const *paradigm)
{
    AfxEntry("pipr=%p", pipr);
    afxError err = NIL;
    AfxAssertObject(pipr, AFX_FCC_PIPR);

    AfxChainDeploy(&pipr->pipelines, pipr);

    pipr->socketCnt = 0;

    for (afxNat i = 0; i < paradigm->socketCnt; i++)
    {
        pipr->sockets[i].entryCnt = 0;

        for (afxNat j = 0; j < paradigm->socket[i].entryCnt; j++)
        {
            AfxString16(&(pipr->sockets[i].entries[j].name), &(paradigm->socket[i].entries[j].name.str));
            pipr->sockets[i].entries[j].binding = paradigm->socket[i].entries[j].binding;
            pipr->sockets[i].entries[j].visibility = paradigm->socket[i].entries[j].visibility;
            pipr->sockets[i].entries[j].type = paradigm->socket[i].entries[j].type;

            pipr->sockets[i].entryCnt++;
        }
        pipr->socketCnt++;
    }

    if (!err)
    {
        
    }

    return err;
}

_SGL afxPipelineRig _AfxDrawContextBuildPipelineRig(afxDrawContext dctx, afxNat socketCnt, afxLegoSchema const socket[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxPipelineRig pipr = NIL;

    _afxPipaParadigm paradigm =
    {
        dctx,
        socketCnt,
        socket
    };

    if (!(pipr = AfxObjectAcquire(AfxDrawContextGetPipelineRigClass(dctx), &paradigm, AfxSpawnHint())))
        AfxThrowError();

    return pipr;
}

_SGL afxClassSpecification const _AfxPiprClassSpec;

afxClassSpecification const _AfxPiprClassSpec =
{
    AFX_FCC_PIPR,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxPipelineRig)),
    NIL,
    (void*)_AfxPiprCtor,
    (void*)_AfxPiprDtor,
    "Pipeline Rig",
    &_AfxStdPiprImpl
};
