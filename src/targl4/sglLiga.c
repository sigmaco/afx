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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"

#if 0
_SGL afxError _SglDqueBindAndSyncLegoSub(afxDrawBridge ddge, afxNat unit, afxLigature liga, afxLigature legt2)
{
    afxError err = AFX_ERR_NONE;
    afxLigature liga = AfxLegoGetTemplate(liga);
    AfxAssertObject(liga, afxFcc_BSCH);
    AfxAssert(liga->base.entryCnt >= legt2->entryCnt);

    for (afxNat j = 0; j < liga->base.entryCnt; j++)
    {
        afxLigatureEntry const *entry = &liga->base.entry[j];
        afxLigatureEntry const *entry2 = &legt2->entry[j];
        AfxAssert(entry->binding == entry2->binding);
        AfxAssert(entry->visibility == entry2->visibility);
        AfxAssert(entry->type == entry2->type);

        afxPipelineRigData const *data = &liga->base.data[j];

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

_SGL afxError _SglDqueBindAndSyncLego(afxDrawBridge ddge, afxNat unit, afxLigature liga)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    afxLigature liga = AfxLegoGetTemplate(liga);
    AfxAssertObject(liga, afxFcc_BSCH);
    glVmt const* gl = &ddge->wglVmt;
    
    if (ddge->state.pip)
    {
        afxLigature legt2;
        AfxPipelineRigEnumerateTemplates(AfxPipelineGetRig(ddge->state.pip), unit, 1, &legt2);

        if (_SglDqueBindAndSyncLegoSub(ddge, unit, liga, legt2))
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
            afxLigature legt2 = shd->liga[unit];

            if (_SglDqueBindAndSyncLegoSub(ddge, unit, liga, legt2))
                AfxThrowError();
        }
    }
    return err;
}
#endif 

_SGL afxError _DpuBindAndResolveLego(sglDpu* dpu, afxLigature liga, GLuint glHandle)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    glVmt const* gl = &dpu->gl;
    AfxAssert(glHandle);
    
    afxString32 tmp;
    AfxMakeString32(&tmp, 0);
    afxChar const *rawName = (void const *)AfxGetStringData(&tmp.str.str, 0);

    afxNat setCnt = liga->base.setCnt;

    for (afxNat i = 0; i < setCnt; i++)
    {
        afxNat set = liga->base.sets[i].set;
        afxNat entryCnt = liga->base.sets[i].entryCnt;
        afxNat baseEntry = liga->base.sets[i].baseEntry;

        for (afxNat j = 0; j < entryCnt; j++)
        {
            afxLigatureEntry const *entry = &liga->base.totalEntries[baseEntry + j];
            AfxAssert(!AfxStringIsEmpty(&entry->name.str.str));
            AfxCopyString(&tmp.str, &entry->name.str.str);
            //AfxAssert(entry->visibility);
            //AfxAssert(entry->cnt);

            afxNat glBinding = (set * _SGL_MAX_ENTRY_PER_LEGO) + entry->binding;
            afxNat loc;

            AfxAssert(entry->type);
            switch (entry->type)
            {
            case AFX_SHD_RES_TYPE_SAMPLER:
            {
                loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

                if (loc != GL_INVALID_INDEX)
                {
                    gl->Uniform1i(loc, glBinding); _SglThrowErrorOccuried();
                }
                else AfxThrowError();
                break;
            }
            case AFX_SHD_RES_TYPE_SAMPLED_IMAGE:
            {
                loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

                if (loc != GL_INVALID_INDEX)
                {
                    gl->Uniform1i(loc, glBinding); _SglThrowErrorOccuried();
                }
                else AfxThrowError();
                break;
            }
            case AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER:
            {
                loc = gl->GetUniformLocation(glHandle, rawName); _SglThrowErrorOccuried();

                if (loc != GL_INVALID_INDEX)
                {
                    gl->Uniform1i(loc, glBinding); _SglThrowErrorOccuried();
                }
                else AfxThrowError();
                break;
            }
            case AFX_SHD_RES_TYPE_CONSTANT_BUFFER:
            {
                // https://stackoverflow.com/questions/44629165/bind-multiple-uniform-buffer-objects

                GLuint unifBlckIdx = gl->GetUniformBlockIndex(glHandle, rawName); _SglThrowErrorOccuried();

                if (unifBlckIdx != GL_INVALID_INDEX)
                {
                    gl->UniformBlockBinding(glHandle, unifBlckIdx, glBinding); _SglThrowErrorOccuried();
                }
                else AfxThrowError();
                break;
            }
            case AFX_SHD_RES_TYPE_STORAGE_BUFFER:
            {
                GLuint storBlckIdx = gl->GetUniformBlockIndex(glHandle, rawName); _SglThrowErrorOccuried();

                if (storBlckIdx != GL_INVALID_INDEX)
                {
                    gl->ShaderStorageBlockBinding(glHandle, storBlckIdx, glBinding); _SglThrowErrorOccuried();
                }
                else AfxThrowError();
                break;
            }
            default:
            {
                AfxLogError("");
            }
            }
        }
    }
    err = 0;
    return err;
}

_SGL afxError _SglBschDtor(afxLigature liga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    if (_AvxBschStdImplementation.dtor(liga))
        AfxThrowError();

    return err;
}

_SGL afxError _SglBschCtor(afxLigature liga, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    if (_AvxBschStdImplementation.ctor(liga, cookie)) AfxThrowError();
    else
    {
        if (err && _AvxBschStdImplementation.dtor(liga))
            AfxThrowError();
    }
    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    return err;
}

_SGL afxClassConfig const _SglBschMgrCfg =
{
    .fcc = afxFcc_BSCH,
    .name = "Ligature",
    .desc = "Pipelined Resourcing Ligature",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxLigature)),
    .ctor = (void*)_SglBschCtor,
    .dtor = (void*)_SglBschDtor
};
