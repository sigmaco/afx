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
#include "afx/draw/pipelining/afxLego.h"
#include "afx/draw/afxDrawSystem.h"

#if 0
_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawQueue dque, afxNat unit, afxLego lego, afxLego legt2)
{
    afxError err = AFX_ERR_NONE;
    afxLego legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    AfxAssert(legt->entryCnt >= legt2->entryCnt);

    for (afxNat j = 0; j < legt->entryCnt; j++)
    {
        afxLegoEntry const *entry = &legt->entry[j];
        afxLegoEntry const *entry2 = &legt2->entry[j];
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
            _SglDqueBindAndSyncBuf(dque, binding, data->buf, data->offset, data->range, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(lego, AFX_FCC_LEGO);
    afxLego legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, AFX_FCC_LEGT);
    sglVmt const* gl = &dque->wglVmt;
    
    if (dque->state.pip)
    {
        afxLego legt2;
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
            afxLego legt2 = shd->legt[unit];

            if (_SglDqueBindAndSyncLegoSub(dque, unit, lego, legt2))
                AfxThrowError();
        }
    }
    return err;
}
#endif 

_SGL afxError _SglDqueBindAndResolveLego(afxDrawQueue dque, afxNat unit, afxLego legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    sglVmt const* gl = &dque->wglVmt;

    AfxAssert(dque->state.pip);
    GLuint glHandle = dque->state.pip->glHandle[dque->queueIdx];
    
    afxString32 tmp;
    AfxString32(&tmp);
    afxChar const *rawName = (void const *)AfxStringGetDataConst(&tmp.str, 0);

    for (afxNat j = 0; j < legt->entryCnt; j++)
    {
        afxLegoEntry const *entry = &legt->entries[j];
        AfxAssert(!AfxStringIsEmpty(entry->name));
        AfxStringCopy(&tmp.str, entry->name);
        AfxAssert(entry->type);
        //AfxAssert(entry->visibility);
        //AfxAssert(entry->cnt);

        afxNat setId = (unit * _SGL_MAX_ENTRY_PER_LEGO);
        afxNat binding = setId + entry->binding;
        afxNat loc;

        switch (entry->type)
        {
        case AFX_SHD_RES_TYPE_SAMPLER:
        {
            loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

            if (loc != GL_INVALID_INDEX)
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
            }
            break;
        }
        case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
        {
            loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

            if (loc != GL_INVALID_INDEX)
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
            }
            break;
        }
        case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

            if (loc != GL_INVALID_INDEX)
            {
                gl->Uniform1i(loc, binding); _SglThrowErrorOccuried();
            }
            break;
        }
        case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
        {
            // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

            //loc = gl->GetUniformBlockIndex(dque->state.pip->gpuHandle[dque->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
            //gl->UniformBlockBinding(dque->state.pip->gpuHandle[dque->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));

            GLuint unifBlckIdx = gl->GetUniformBlockIndex(glHandle, rawName);

            if (unifBlckIdx != GL_INVALID_INDEX)
            {
                gl->UniformBlockBinding(glHandle, unifBlckIdx, binding); _SglThrowErrorOccuried();
            }
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

_SGL afxNat32 _AfxLegoGetCrc32(afxLego legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    return legt->crc32;
}

_SGL afxResult _AfxLegoDescribe(afxLego legt, afxNat base, afxNat cnt, afxLegoBindingDecl decl[])
{
    afxError err = AFX_ERR_NONE;
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
            decl[i].binding = legt->entries[i].binding;
            decl[i].visibility = legt->entries[i].visibility;
            decl[i].type = legt->entries[i].type;
            decl[i].cnt = legt->entries[i].cnt;
            decl[i].name = legt->entries[i].name;
            ++rslt;
        }
    }
    return rslt;
}

_SGL afxLegtImpl const _AfxStdLegtImpl;
afxLegtImpl const _AfxStdLegtImpl =
{
    _AfxLegoDescribe,
    _AfxLegoGetCrc32
};

_SGL afxBool _SglLegtEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxLego legt = (void*)obj;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglLegtEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxLego legt = (void*)obj;
    AfxAssertObject(legt, AFX_FCC_LEGT);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxLegtDtor(afxLego legt)
{
    AfxEntry("legt=%p", legt);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGT);

    afxDrawContext dctx = AfxObjectGetProvider(&legt->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    if (legt->entries)
    {
        for (afxNat i = 0; i < legt->entryCnt; i++)
        {
            if (legt->entries[i].name)
                AfxStringDeallocate(legt->entries[i].name);
        }

        AfxDeallocate(mem, legt->entries);
    }

    return err;
}

_SGL afxError _AfxLegtCtor(void *cache, afxNat idx, afxLego legt, afxLegoBlueprint const blueprints[])
{
    AfxEntry("legt=%p", legt);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGT);

    afxDrawContext dctx = AfxObjectGetProvider(&legt->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    AfxAssert(blueprints);
    afxLegoBlueprint const *blueprint = &blueprints[idx];
    
    afxNat bindCnt = AfxArrayGetPop(&blueprint->bindings);
    AfxAssert(bindCnt);

    afxBool incompatible = FALSE;
    legt->entryCnt = 0;

    if (bindCnt && !(legt->entries = AfxAllocate(mem, bindCnt * sizeof(legt->entries[0]), AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < bindCnt; i++)
        {
            afxLegoBlueprintBinding *bindBp = AfxArrayGetElement(&blueprint->bindings, i);
            
            legt->entries[legt->entryCnt].binding = bindBp->binding;
            legt->entries[legt->entryCnt].cnt = bindBp->cnt;
            legt->entries[legt->entryCnt].type = bindBp->type;
            AfxAssert(legt->entries[legt->entryCnt].type != AFX_SHD_RES_TYPE_OUTPUT);
            legt->entries[legt->entryCnt].visibility = bindBp->visibility;
            legt->entries[legt->entryCnt].name = bindBp->name ? AfxStringClone(bindBp->name) : NIL;
            ++legt->entryCnt;
        }
    }

    if (!err)
    {
        struct tmpCrc
        {
            afxNat binding;
            afxFlags visibility;
            afxShaderResourceType type;
            afxNat cnt;
        } tmpCrc[_SGL_MAX_ENTRY_PER_LEGO] = { 0 };

        for (afxNat i = 0; i < legt->entryCnt; i++)
        {
            tmpCrc[i].binding = legt->entries[i].binding;
            tmpCrc[i].type = legt->entries[i].type;
            tmpCrc[i].visibility = legt->entries[i].visibility;
            tmpCrc[i].cnt = legt->entries[i].cnt;
        }

        afxNat32 crc = 0;
        AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * legt->entryCnt);
        legt->crc32 = crc;
    }
    return err;
}

_SGL afxLego _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxLegoBindingDecl const bindings[])
{
    afxError err = AFX_ERR_NONE;
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

    afxLego legt;
    AfxChainForEveryLinkage(&(AfxDrawContextGetLegoClass(dctx)->instances), AFX_OBJECT(afxLego), obj.cls, legt)
    {
        AfxAssertObject(legt, AFX_FCC_LEGT);
        
        if (legt->crc32 == crc)
        {
            return legt;
        }
    }
    return NIL;
}

_SGL afxClassSpecification const _AfxLegtClassSpec;

afxClassSpecification const _AfxLegtClassSpec =
{
    AFX_FCC_LEGT,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxLego)),
    NIL,
    (void*)_AfxLegtCtor,
    (void*)_AfxLegtDtor,
    .event = _SglLegtEventHandler,
    .eventFilter = _SglLegtEventFilter,
    "afxLego",
    &_AfxStdLegtImpl
};
