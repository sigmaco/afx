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

#include "sgl.h"

#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/afxDrawSystem.h"

#if 0
_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawQueue dque, afxNat unit, afxBindSchema lego, afxBindSchema legt2)
{
    afxError err = AFX_ERR_NONE;
    afxBindSchema legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, afxFcc_BSCH);
    AfxAssert(lego->base.entryCnt >= legt2->entryCnt);

    for (afxNat j = 0; j < lego->base.entryCnt; j++)
    {
        afxBindSchemaEntry const *entry = &lego->base.entry[j];
        afxBindSchemaEntry const *entry2 = &legt2->entry[j];
        AfxAssert(entry->binding == entry2->binding);
        AfxAssert(entry->visibility == entry2->visibility);
        AfxAssert(entry->type == entry2->type);

        afxPipelineRigData const *data = &lego->base.data[j];

        afxNat setId = (unit * _SGL_MAX_ENTRY_PER_LEGO);
        afxNat binding = setId + entry->binding;

        switch (entry->type)
        {
        case AFX_SHD_RES_TYPE_SAMPLER:
        {
            AfxAssertObject(data->smp, afxFcc_SAMP);
            _SglDqueBindAndSyncSmp(dque, binding, data->smp);
            break;
        }
        case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
        {
            AfxAssertObject(data->tex, afxFcc_RAS);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);
            break;
        }
        case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            AfxAssertObject(data->tex, afxFcc_RAS);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);

            AfxAssertObject(data->smp, afxFcc_SAMP);
            _SglDqueBindAndSyncSmp(dque, binding, data->smp);
#if 0
            afxFixedUri128 uri;
            AfxMakeFixedUri128(&uri, NIL);
            AfxPrintRaster(point->resource.img.tex, AfxFormatUri(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
#endif
            break;
        }
        case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
        {
            AfxAssertObject(data->buf, afxFcc_BUF);

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

_SGL afxError _SglDqueBindAndSyncLego(afxDrawQueue dque, afxNat unit, afxBindSchema lego)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    afxBindSchema legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, afxFcc_BSCH);
    glVmt const* gl = &dque->wglVmt;
    
    if (dque->state.pip)
    {
        afxBindSchema legt2;
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
            afxBindSchema legt2 = shd->legt[unit];

            if (_SglDqueBindAndSyncLegoSub(dque, unit, lego, legt2))
                AfxThrowError();
        }
    }
    return err;
}
#endif 

_SGL afxError _SglDpuBindAndResolveLego(sglDpuIdd* dpu, GLuint glHandle, afxNat unit, afxBindSchema lego, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    AfxAssert(glHandle);
    
    afxFixedString32 tmp;
    AfxMakeFixedString32(&tmp, 0);
    afxChar const *rawName = (void const *)AfxGetStringData(&tmp.str.str, 0);

    for (afxNat j = 0; j < lego->base.entryCnt; j++)
    {
        afxBindSchemaEntry const *entry = &lego->base.entries[j];
        AfxAssert(!AfxStringIsEmpty(&entry->name.str));
        AfxCopyString(&tmp.str, &entry->name.str);
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

            GLuint unifBlckIdx = gl->GetUniformBlockIndex(glHandle, rawName); _SglThrowErrorOccuried();

            if (unifBlckIdx != GL_INVALID_INDEX)
            {
                gl->UniformBlockBinding(glHandle, unifBlckIdx, binding); _SglThrowErrorOccuried();
            }
            break;
        }
        case AFX_SHD_RES_TYPE_STORAGE_BUFFER:
        {
            GLuint storBlckIdx = gl->GetUniformBlockIndex(glHandle, rawName); _SglThrowErrorOccuried();

            if (storBlckIdx != GL_INVALID_INDEX)
            {
                gl->ShaderStorageBlockBinding(glHandle, storBlckIdx, binding); _SglThrowErrorOccuried();
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

_SGL afxBindSchema _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[])
{
    afxError err = AFX_ERR_NONE;

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
    //AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * bindCnt);

    afxNat i = 0;
    afxBindSchema lego;
    while (AfxEnumerateBindSchemas(dctx, i, 1, &lego))
    {
        AfxAssertObjects(1, &lego, afxFcc_BSCH);
        
        if (lego->base.crc32 == crc)
        {
            return lego;
        }
        ++i;
    }
    return NIL;
}

_SGL afxError _SglBschDtor(afxBindSchema lego)
{
    AfxEntry("lego=%p", lego);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    afxDrawContext dctx = AfxGetObjectProvider(lego);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (lego->base.entries)
    {
        for (afxNat i = 0; i < lego->base.entryCnt; i++)
        {
            AfxDeallocateString(&lego->base.entries[i].name);
        }

        AfxDeallocate(mmu, lego->base.entries);
    }

    return err;
}

_SGL afxError _SglBschCtor(afxBindSchema lego, afxCookie const* cookie)
{
    AfxEntry("lego=%p", lego);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    afxPipelineRigBlueprint const *blueprint = ((afxPipelineRigBlueprint const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(lego);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAssert(blueprint);

    afxNat bindCnt = AfxCountArrayElements(&blueprint->bindings);
    AfxAssert(bindCnt);

    lego->base.entryCnt = 0;
    lego->base.entries = NIL;

    if (bindCnt && !(lego->base.entries = AfxAllocate(mmu, bindCnt, sizeof(lego->base.entries[0]), 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < bindCnt; i++)
        {
            afxPipelineRigBlueprintBinding *bindBp = AfxGetArrayUnit(&blueprint->bindings, i);

            lego->base.entries[lego->base.entryCnt].binding = bindBp->binding;
            lego->base.entries[lego->base.entryCnt].cnt = bindBp->cnt;
            lego->base.entries[lego->base.entryCnt].type = bindBp->type;
            AfxAssert(lego->base.entries[lego->base.entryCnt].type != AFX_SHD_RES_TYPE_OUTPUT);
            lego->base.entries[lego->base.entryCnt].visibility = bindBp->visibility;
            AfxCloneString(&lego->base.entries[lego->base.entryCnt].name, &bindBp->name.str);
            ++lego->base.entryCnt;
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
        } tmpCrc[/*_SGL_MAX_ENTRY_PER_LEGO*/10] = { 0 };
        AfxAssert(sizeof(tmpCrc) / sizeof(tmpCrc[0]) >= lego->base.entryCnt);

        for (afxNat i = 0; i < lego->base.entryCnt; i++)
        {
            tmpCrc[i].binding = lego->base.entries[i].binding;
            tmpCrc[i].type = lego->base.entries[i].type;
            tmpCrc[i].visibility = lego->base.entries[i].visibility;
            tmpCrc[i].cnt = lego->base.entries[i].cnt;
        }

        afxNat32 crc = 0;
        //AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * lego->base.entryCnt);
        lego->base.crc32 = crc;
    }
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    return err;
}

_SGL afxClassConfig const _SglBschClsConfig =
{
    .fcc = afxFcc_BSCH,
    .name = "Lego",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBindSchema)),
    .mmu = NIL,
    .ctor = (void*)_SglBschCtor,
    .dtor = (void*)_SglBschDtor
};
