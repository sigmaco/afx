/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"

#include "qwadro/draw/pipe/afxBindSchema.h"
#include "qwadro/draw/afxDrawSystem.h"

#if 0
_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawBridge ddge, afxNat unit, afxBindSchema lego, afxBindSchema legt2)
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
            _SglDqueBindAndSyncSmp(ddge, binding, data->smp);
            break;
        }
        case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
        {
            AfxAssertObject(data->tex, afxFcc_RAS);
            _SglDqueBindAndSyncTex(ddge, binding, data->tex);
            break;
        }
        case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
        {
            AfxAssertObject(data->tex, afxFcc_RAS);
            _SglDqueBindAndSyncTex(ddge, binding, data->tex);

            AfxAssertObject(data->smp, afxFcc_SAMP);
            _SglDqueBindAndSyncSmp(ddge, binding, data->smp);
#if 0
            afxUri128 uri;
            AfxMakeUri128(&uri, NIL);
            AfxPrintRaster(point->resource.img.tex, AfxFormatUri(&uri.uri, "system/tex-%u-%u.tga", i, entry->binding));
#endif
            break;
        }
        case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
        {
            AfxAssertObject(data->buf, afxFcc_BUF);

            // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

            //loc = gl->GetUniformBlockIndex(ddge->state.pip->gpuHandle[ddge->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
            //gl->UniformBlockBinding(ddge->state.pip->gpuHandle[ddge->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));
            _SglDqueBindAndSyncBuf(ddge, binding, data->buf, data->offset, data->range, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
            //gl->BindBufferRange(GL_UNIFORM_BUFFER, binding, point->resource.data.buf->gpuHandle, point->resource.data.base, point->resource.data.range); _SglThrowErrorOccuried();

            break;
        }
        default:
        {
            AfxLogError("");
        }
        }
    }
    return err;
}

_SGL afxError _SglDqueBindAndSyncLego(afxDrawBridge ddge, afxNat unit, afxBindSchema lego)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    afxBindSchema legt = AfxLegoGetTemplate(lego);
    AfxAssertObject(legt, afxFcc_BSCH);
    glVmt const* gl = &ddge->wglVmt;
    
    if (ddge->state.pip)
    {
        afxBindSchema legt2;
        AfxPipelineRigEnumerateTemplates(AfxPipelineGetRig(ddge->state.pip), unit, 1, &legt2);

        if (_SglDqueBindAndSyncLegoSub(ddge, unit, lego, legt2))
            AfxThrowError();
    }
    else
    {
        afxNat shdCnt;
        afxShader shd;
        shdCnt = ddge->state.shdCnt;

        for (afxNat i = 0; i < shdCnt; i++)
        {
            shd = ddge->state.shd[i];
            afxBindSchema legt2 = shd->legt[unit];

            if (_SglDqueBindAndSyncLegoSub(ddge, unit, lego, legt2))
                AfxThrowError();
        }
    }
    return err;
}
#endif 

_SGL afxError _DpuBindAndResolveLego(sglDpu* dpu, GLuint glHandle, afxNat unit, afxBindSchema lego, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    AfxAssert(glHandle);
    
    afxString32 tmp;
    AfxMakeString32(&tmp, 0);
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

            //loc = gl->GetUniformBlockIndex(ddge->state.pip->gpuHandle[ddge->queueIdx], entry->name.buf); _SglThrowErrorOccuried();
            //gl->UniformBlockBinding(ddge->state.pip->gpuHandle[ddge->queueIdx], loc, ((i * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding));

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
            AfxLogError("");
        }
        }
    }

    return err;
}

_SGL afxError _SglBschDtor(afxBindSchema lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    if (_AvxBschStdImplementation.dtor(lego))
        AfxThrowError();

    return err;
}

_SGL afxError _SglBschCtor(afxBindSchema lego, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    if (_AvxBschStdImplementation.ctor(lego, cookie)) AfxThrowError();
    else
    {
        if (err && _AvxBschStdImplementation.dtor(lego))
            AfxThrowError();
    }
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    return err;
}

_SGL afxClassConfig const _SglBschMgrCfg =
{
    .fcc = afxFcc_BSCH,
    .name = "Lego",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBindSchema)),
    .mmu = NIL,
    .ctor = (void*)_SglBschCtor,
    .dtor = (void*)_SglBschDtor
};
