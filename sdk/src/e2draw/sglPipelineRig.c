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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "sgl.h"

#include "afx/draw/afxPipelineRig.h"
#include "afx/draw/afxPipelineRig.h"
#include "afx/draw/afxDrawSystem.h"

#if 0
_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawQueue dque, afxNat unit, afxPipelineRig lego, afxPipelineRig legt2)
{
    afxError err = AFX_ERR_NONE;
    afxPipelineRig legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, afxFcc_LEGO);
    AfxAssert(lego->base.entryCnt >= legt2->entryCnt);

    for (afxNat j = 0; j < lego->base.entryCnt; j++)
    {
        afxPipelineRigEntry const *entry = &lego->base.entry[j];
        afxPipelineRigEntry const *entry2 = &legt2->entry[j];
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
            AfxAssertObject(data->tex, afxFcc_TEX);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);
            break;
        }
        case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            AfxAssertObject(data->tex, afxFcc_TEX);
            _SglDqueBindAndSyncTex(dque, binding, data->tex);

            AfxAssertObject(data->smp, afxFcc_SAMP);
            _SglDqueBindAndSyncSmp(dque, binding, data->smp);
#if 0
            afxUri128 uri;
            AfxUri128(&uri, NIL);
            AfxPrintTexture(point->resource.img.tex, AfxFormatUri(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
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

_SGL afxError _SglDqueBindAndSyncLego(afxDrawQueue dque, afxNat unit, afxPipelineRig lego)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);
    afxPipelineRig legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, afxFcc_LEGO);
    glVmt const* gl = &dque->wglVmt;
    
    if (dque->state.pip)
    {
        afxPipelineRig legt2;
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
            afxPipelineRig legt2 = shd->legt[unit];

            if (_SglDqueBindAndSyncLegoSub(dque, unit, lego, legt2))
                AfxThrowError();
        }
    }
    return err;
}
#endif 

_SGL afxError _SglDpuBindAndResolveLego(sglDpuIdd* dpu, afxPipeline pip, afxNat unit, afxPipelineRig lego, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);

    AfxAssert(pip);
    GLuint glHandle = pip->glHandle;
    
    afxString32 tmp;
    AfxString32(&tmp);
    afxChar const *rawName = (void const *)AfxGetStringData(&tmp.str, 0);

    for (afxNat j = 0; j < lego->base.entryCnt; j++)
    {
        afxPipelineRigEntry const *entry = &lego->base.entries[j];
        AfxAssert(!AfxStringIsEmpty(&entry->name));
        AfxCopyString(&tmp.str, &entry->name);
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
        default:
        {
            AfxError("");
        }
        }
    }

    return err;
}

_SGL afxPipelineRig _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[])
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
    afxPipelineRig lego;
    while (AfxEnumeratePipelineRigs(dctx, i, 1, &lego))
    {
        AfxAssertObjects(1, &lego, afxFcc_LEGO);
        
        if (lego->base.crc32 == crc)
        {
            return lego;
        }
        ++i;
    }
    return NIL;
}

_SGL afxError _SglLegoDtor(afxPipelineRig lego)
{
    AfxEntry("lego=%p", lego);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);

    afxDrawContext dctx = AfxGetObjectProvider(lego);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (lego->base.entries)
    {
        for (afxNat i = 0; i < lego->base.entryCnt; i++)
        {
            AfxDeallocateString(&lego->base.entries[i].name);
        }

        AfxDeallocate(mem, lego->base.entries);
    }

    return err;
}

_SGL afxError _SglLegoCtor(afxPipelineRig lego, afxCookie const* cookie)
{
    AfxEntry("lego=%p", lego);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_LEGO);

    afxPipelineRigBlueprint const *blueprint = ((afxPipelineRigBlueprint const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(lego);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    AfxAssert(blueprint);

    afxNat bindCnt = AfxCountArrayElements(&blueprint->bindings);
    AfxAssert(bindCnt);

    lego->base.entryCnt = 0;
    lego->base.entries = NIL;

    if (bindCnt && !(lego->base.entries = AfxAllocate(mem, bindCnt * sizeof(lego->base.entries[0]), 0, AfxSpawnHint()))) AfxThrowError();
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
            AfxCloneString(&lego->base.entries[lego->base.entryCnt].name, &bindBp->name);
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
    AfxAssertObjects(1, &lego, afxFcc_LEGO);
    return err;
}

_SGL afxClassConfig _SglLegoClsConfig =
{
    .fcc = afxFcc_LEGO,
    .name = "Lego",
    .unitsPerPage = 7,
    .size = sizeof(AFX_OBJECT(afxPipelineRig)),
    .ctx = NIL,
    .ctor = (void*)_SglLegoCtor,
    .dtor = (void*)_SglLegoDtor
};
