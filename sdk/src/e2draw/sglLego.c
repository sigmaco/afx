/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#include "sgl.h"

#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/pipelining/afxPipelineRig.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
    afxDrawContext dctx;
    afxNat declCnt;
    afxLegoBindingDecl const *decls;
} _afxLegtParadigm;

typedef struct
{
    afxDrawContext dctx;
    afxLegoTemplate legt;
    afxLegoData const *data;
} _afxLegoParadigm;

_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawQueue dque, afxNat unit, afxLego lego, afxLegoTemplate legt2)
{
    afxError err = NIL;
    afxLegoTemplate legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    AfxAssert(legt->entryCnt >= legt2->entryCnt);

    for (afxNat j = 0; j < legt->entryCnt; j++)
    {
        afxLegoTemplateEntry const *entry = &legt->entry[j];
        afxLegoTemplateEntry const *entry2 = &legt2->entry[j];
        AfxAssert(entry->binding == entry2->binding);
        AfxAssert(entry->visibility == entry2->visibility);
        AfxAssert(entry->type == entry2->type);

        afxLegoData const *data = &lego->data[j];

        afxNat setId = (unit * _SGL_MAX_ENTRY_PER_LEGO);
        afxNat binding = setId + entry->binding;

        switch (entry->type)
        {
        case AFX_SHD_RES_TYPE_SAMPLER:
        {
            AfxAssertObject(data->smp, AFX_FCC_SMP);
            _SglDqueBindAndSyncSmp(dque, binding, data->smp);
            break;
        }
        case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
        {
            AfxAssertObject(data->tex, AFX_FCC_TEX);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);
            break;
        }
        case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            AfxAssertObject(data->tex, AFX_FCC_TEX);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);

            AfxAssertObject(data->smp, AFX_FCC_SMP);
            _SglDqueBindAndSyncSmp(dque, binding, data->smp);
#if 0
            afxUri128 uri;
            AfxUri128(&uri, NIL);
            AfxTextureDownload(point->resource.img.tex, AfxUriFormat(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
#endif
            break;
        }
        case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
        {
            AfxAssertObject(data->buf, AFX_FCC_BUF);

            // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

            //loc = gl->GetUniformBlockIndex(dque->state.pip->gpuHandle[dque->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
            //gl->UniformBlockBinding(dque->state.pip->gpuHandle[dque->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));
            _SglDqueBindAndSyncBuf(dque, binding, data->buf, data->base, data->range, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
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

_SGL afxError _SglDqueBindAndSyncLego(afxDrawQueue dque, afxNat unit, afxLego lego)
{
    //AfxEntry("pip=%p", pip);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    afxLegoTemplate legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    sglVmt const* gl = &dque->wglVmt;
    
    if (dque->state.pip)
    {
        afxLegoTemplate legt2;
        AfxPipelineRigEnumerateTemplates(AfxPipelineGetRig(dque->state.pip), unit, 1, &legt2);

        if (_SglDqueBindAndSyncLegoSub(dque, unit, lego, legt2))
            AfxThrowError();
    }
    else
    {
        afxNat shdCnt;
        afxShader shd;
        shdCnt = dque->state.shdCnt;

        for (afxNat i = 0; i < shdCnt; i++)
        {
            shd = dque->state.shd[i];
            afxLegoTemplate legt2 = shd->legt[unit];

            if (_SglDqueBindAndSyncLegoSub(dque, unit, lego, legt2))
                AfxThrowError();
        }
    }
    return err;
}

_SGL afxLegoTemplate _AfxLegoGetTemplate(afxLego lego)
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    return AfxLinkageGetOwner(&lego->legt);
}

_SGL afxResult _AfxLegoTemplateDescribe(afxLegoTemplate legt, afxNat base, afxNat cnt, afxLegoBindingDecl decl[])
{
    afxError err = NIL;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    AfxAssert(cnt);
    AfxAssert(legt->entryCnt >= cnt);
    AfxAssert(legt->entryCnt > base);
    AfxAssert(decl);
    afxResult rslt = 0;

    for (afxNat i = 0; i < legt->entryCnt; i++)
    {
        if (i >= cnt) break;
        else
        {
            decl[i].binding = legt->entry[i].binding;
            decl[i].visibility = legt->entry[i].visibility;
            decl[i].type = legt->entry[i].type;
            decl[i].cnt = legt->entry[i].cnt;
            decl[i].name = &(legt->entry[i].name.str);
            ++rslt;
        }
    }
    return rslt;
}

_SGL afxError _AfxLegoCopy(afxLego lego, afxLego in)
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);

    afxLegoTemplate legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    afxLegoTemplate legt2 = AfxLegoGetTemplate(in);
    AfxAssertObject(legt2, AFX_FCC_LEGT);

    AfxAssert(legt->entryCnt >= legt2->entryCnt);

    for (afxNat i = 0; i < legt->entryCnt; i++)
    {
        AfxAssert(legt->entry[i].binding == legt2->entry[i].binding);
        AfxAssert(legt->entry[i].visibility == legt2->entry[i].visibility);
        AfxAssert(legt->entry[i].type == legt2->entry[i].type);
        AfxAssert(legt->entry[i].cnt == legt2->entry[i].cnt);
        AfxAssert(legt->crc32 == legt2->crc32);
        lego->data[i] = in->data[i];
    }

    return err;
}

_SGL afxError _AfxLegoUpdate(afxLego lego, afxNat cnt, afxLegoData const data[])
{
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    AfxAssert(cnt);
    AfxAssert(data);

    afxLegoTemplate legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);

    for (afxNat j = 0; j < cnt; j++)
    {
        lego->data[j] = data[j];
    }

    return err;
}

_SGL afxError _AfxLegoDtor(afxLego lego)
{
    AfxEntry("lego=%p", lego);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);

    afxLegoTemplate legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    AfxLinkageDrop(&lego->legt);
    AfxObjectRelease(&legt->obj);

    return err;
}

_SGL afxError _AfxLegtDtor(afxLegoTemplate legt)
{
    AfxEntry("legt=%p", legt);
    afxError err = NIL;
    AfxAssertObject(legt, AFX_FCC_LEGT);

    afxLego lego;
    AfxChainForEveryLinkage(&legt->instances, AFX_OBJECT(afxLego), legt, lego)
    {
        AfxAssertObject(lego, AFX_FCC_LEGO);
        while (0 < AfxObjectRelease(&lego->obj));
    };
   
    AfxAssert(legt->instances.cnt == 0);

    return err;
}

_SGL afxLegoImpl const _AfxStdLegoImpl;
afxLegoImpl const _AfxStdLegoImpl =
{
    _AfxLegoUpdate,
    _AfxLegoCopy,
    _AfxLegoGetTemplate
};

_SGL afxError _AfxLegtCtor(afxLegoTemplate legt, _afxLegtParadigm const *paradigm)
{
    AfxEntry("legt=%p", legt);
    afxError err = NIL;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    AfxAssert(paradigm);
    AfxAssert(paradigm->declCnt);
    AfxAssert(paradigm->decls);

    legt->entryCnt = 0;

    for (afxNat i = 0; i < paradigm->declCnt; i++)
    {
        afxBool existing = FALSE;
        afxBool incompatible = FALSE;

        for (afxNat j = 0; j < legt->entryCnt; j++)
        {
            if ((existing = (legt->entry[j].binding == paradigm->decls[i].binding)))
            {
                if ((incompatible = (legt->entry[j].type != paradigm->decls[i].type || legt->entry[j].cnt != paradigm->decls[i].cnt)))
                {
                    AfxThrowError();
                }
                break;
            }
        }
        
        if (!incompatible)
        {
            if (existing)
            {
                legt->entry[legt->entryCnt].visibility |= paradigm->decls[i].visibility;
            }
            else
            {
                legt->entry[legt->entryCnt].binding = paradigm->decls[i].binding;
                legt->entry[legt->entryCnt].visibility = paradigm->decls[i].visibility;
                legt->entry[legt->entryCnt].type = paradigm->decls[i].type;
                legt->entry[legt->entryCnt].cnt = paradigm->decls[i].cnt;
                AfxString16(&(legt->entry[legt->entryCnt].name), paradigm->decls[i].name);
                legt->entryCnt++;
            }
        }
    }

    if (!err)
    {
        AfxChainDeploy(&legt->instances, legt);

        struct tmpCrc
        {
            afxNat binding;
            afxFlags visibility;
            afxShaderResourceType type;
            afxNat cnt;
        } tmpCrc[_SGL_MAX_ENTRY_PER_LEGO] = { 0 };

        for (afxNat i = 0; i < legt->entryCnt; i++)
        {
            tmpCrc[i].binding = legt->entry[i].binding;
            tmpCrc[i].type = legt->entry[i].type;
            tmpCrc[i].visibility = legt->entry[i].visibility;
            tmpCrc[i].cnt = legt->entry[i].cnt;
        }

        afxNat32 crc = 0;
        AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * legt->entryCnt);
        legt->crc32 = crc;
    }
    return err;
}

_SGL afxError _AfxLegoCtor(afxLego lego, _afxLegoParadigm const *paradigm)
{
    AfxEntry("lego=%p", lego);
    afxError err = NIL;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    AfxAssert(paradigm);

    AfxAssert(paradigm->legt);
    afxLegoTemplate legt = paradigm->legt;
    AfxAssertObject(legt, AFX_FCC_LEGT);

    AfxAssert(paradigm->data);

    AfxLinkageDeploy(&lego->legt, &(legt->instances));
    AfxObjectReacquire(&legt->obj, NIL, NIL, NIL, NIL);

    if (paradigm->data && AfxLegoUpdate(lego, legt->entryCnt, paradigm->data))
        AfxThrowError();

    return err;
}

_SGL afxLegoTemplate _SglDrawContextFindLegoTemplate(afxDrawContext dctx, afxNat bindCnt, afxLegoBindingDecl const bindings[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(bindCnt);
    AfxAssert(bindings);

    struct tmpCrc
    {
        afxNat binding;
        afxFlags visibility;
        afxShaderResourceType type;
        afxNat cnt;
    } tmpCrc[_SGL_MAX_ENTRY_PER_LEGO] = { 0 };

    for (afxNat i = 0; i < bindCnt; i++)
    {
        tmpCrc[i].binding = bindings[i].binding;
        tmpCrc[i].type = bindings[i].type;
        tmpCrc[i].visibility = bindings[i].visibility;
        tmpCrc[i].cnt = bindings[i].cnt;
    }

    afxNat32 crc = 0;
    AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * bindCnt);

    afxLegoTemplate legt;
    AfxChainForEveryLinkage(&(AfxDrawContextGetLegoTemplateClass(dctx)->instances), AFX_OBJECT(afxLegoTemplate), obj.cls, legt)
    {
        AfxAssertObject(legt, AFX_FCC_LEGT);
        
        if (legt->crc32 == crc)
        {
            return legt;
        }
    }
    return NIL;
}

_SGL afxLegoTemplate _AfxDrawContextAcquireLegoTemplate(afxDrawContext dctx, afxNat bindCnt, afxLegoBindingDecl const bindings[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(bindCnt);
    AfxAssert(bindings);
    afxLegoTemplate legt = _SglDrawContextFindLegoTemplate(dctx, bindCnt, bindings);

    if (legt)
    {
        AfxAssertObject(legt, AFX_FCC_LEGT);
        AfxObjectReacquire(&legt->obj, NIL, NIL, NIL, NIL);
    }
    else
    {
        _afxLegtParadigm paradigm =
        {
            dctx,
            bindCnt,
            bindings
        };

        if (!(legt = AfxObjectAcquire(AfxDrawContextGetLegoTemplateClass(dctx), &paradigm, AfxSpawnHint()))) AfxThrowError();
        else
        {
            AfxAssertObject(legt, AFX_FCC_LEGT);
        }
    }
    return legt;
}

_SGL afxResult _AfxDrawContextAcquireLegos(afxDrawContext dctx, afxNat cnt, afxLegoTemplate legt[], afxLegoData const data[], afxLego lego[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(legt);
    AfxAssert(lego);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxLegoParadigm paradigm =
        {
            dctx,
            legt[i],
            data ? &data[i] : NIL
        };

        if (!(lego[i] = AfxObjectAcquire(AfxDrawContextGetLegoClass(dctx), &paradigm, AfxSpawnHint()))) AfxThrowError();
        else
        {
            AfxAssertObject(lego[i], AFX_FCC_LEGO);

            ++rslt;
        }
    }
    return rslt;
}

_SGL afxClassSpecification const _AfxLegoClassSpec;
_SGL afxClassSpecification const _AfxLegtClassSpec;

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

afxClassSpecification const _AfxLegtClassSpec =
{
    AFX_FCC_LEGT,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxLegoTemplate)),
    NIL,
    (void*)_AfxLegtCtor,
    (void*)_AfxLegtDtor,
    "Lego Template",
    NIL
};
