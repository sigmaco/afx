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

#include "afx/draw/afxShader.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxUri.h"
#include "afx/draw/afxShaderBlueprint.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDpuSyncShd(sglDpuIdd* dpu, afxShader shd, afxShaderStage stage, glVmt const* gl)
{
    //AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    (void)dpu;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    if ((shd->updFlags & SGL_UPD_FLAG_DEVICE))
    {
        if ((shd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (shd->glHandle)
            {
                gl->DeleteShader(shd->glHandle); _SglThrowErrorOccuried();
                shd->glHandle = NIL;
            }

            AfxAssert(NIL == shd->glHandle);

            if (!(shd->glHandle = gl->CreateShader(AfxToGlShaderStage(stage))))
            {
                AfxThrowError();
                _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(shd);
                AfxAssert(gl->IsShader(shd->glHandle));
                const GLint codeLens[] = { shd->base.codeLen };
                GLchar const* const codes[] = { (GLchar const*)shd->base.code };
                gl->ShaderSource(shd->glHandle, 1, codes, codeLens); _SglThrowErrorOccuried();
                gl->CompileShader(shd->glHandle); _SglThrowErrorOccuried();
                GLint status = 0;
                gl->GetShaderiv(shd->glHandle, GL_COMPILE_STATUS, &status); _SglThrowErrorOccuried();

                if (status == GL_FALSE)
                {
                    AfxThrowError();
                    afxChar str[1024];
                    gl->GetShaderInfoLog(shd->glHandle, sizeof(str), NIL, (GLchar*)str); _SglThrowErrorOccuried();
                    AfxError(str);
                    gl->DeleteShader(shd->glHandle); _SglThrowErrorOccuried();
                    shd->glHandle = NIL;
                }
                else
                {
                    shd->glProgHandle = 0;
                    shd->compiled = TRUE;
                    shd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    AfxEcho("afxShader %p hardware-side data instanced.", shd);
                }
            }
        }
        else if ((shd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(shd->glHandle);
            
            AfxThrowError(); // can't be modified
        }
    }
    return err;
}

_SGL afxError _SglShdDtor(afxShader shd)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetObjectProvider(shd);
    afxContext ctx = AfxGetDrawContextMemory(dctx);
    
    if (shd->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 6, shd->glHandle);
        shd->glHandle = 0;
    }

    if (shd->glProgHandle)
    {
        _SglDctxDeleteGlRes(dctx, 7, shd->glProgHandle);
        shd->glProgHandle = 0;
    }

    //if (shd->vmt->dtor && shd->vmt->dtor(shd))
        //AfxThrowError();

    //AfxAssert(!shd->idd);

    if (shd->base.resDecls)
    {
        for (afxNat j = 0; j < shd->base.resDeclCnt; j++)
        {
            AfxDeallocateString(&shd->base.resDecls[j].name);
        }

        AfxDeallocate(ctx, shd->base.resDecls);
    }

    if (shd->base.ioDecls)
    {
        for (afxNat j = 0; j < shd->base.ioDeclCnt; j++)
        {
            AfxDeallocateString(&shd->base.ioDecls[j].semantic);
        }

        AfxDeallocate(ctx, shd->base.ioDecls);
    }

    if (shd->base.code)
    {
        AfxDeallocate(ctx, shd->base.code);
    }

    AfxUriDeallocate(&shd->base.uri);

    return err;
}

_SGL afxError _SglShdCtor(afxShader shd, afxCookie const* cookie)
{
    AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxShaderBlueprint const *blueprint = ((afxShaderBlueprint const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(shd);
    afxContext ctx = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &ctx, afxFcc_CTX);

     AfxCloneUri(&shd->base.uri, &blueprint->uri.uri);

    shd->base.stage = blueprint->stage;

    AfxCloneString(&shd->base.entry, &blueprint->entry.str);

    shd->base.code = NIL;
    afxNat codeLen = AfxCountArrayElements(&blueprint->codes);

    if (codeLen && !(shd->base.code = AfxAllocate(ctx, codeLen, 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        AfxAssertType(&blueprint->codes, afxFcc_ARR);
        AfxDumpArray(&blueprint->codes, shd->base.code);
    }

    shd->base.codeLen = codeLen;
    //shd->code[shd->codeLen] = '\0';

    AfxComment("%.*s", shd->base.codeLen, shd->base.code);

    if (!err)
    {
        shd->base.resDeclCnt = 0;
        shd->base.resDecls = NIL;

        afxNat resDeclCnt = AfxCountArrayElements(&blueprint->resources);

        if (resDeclCnt && !(shd->base.resDecls = AfxAllocate(ctx, resDeclCnt * sizeof(shd->base.resDecls[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < resDeclCnt; j++)
            {
                afxShaderBlueprintResource const *decl = AfxGetArrayUnit(&blueprint->resources, j);
                shd->base.resDecls[j].set = decl->set;
                AfxAssert(4/*_SGL_MAX_LEGO_PER_BIND*/ > shd->base.resDecls[j].set);
                shd->base.resDecls[j].binding = decl->binding;
                AfxAssert(8/*_SGL_MAX_ENTRY_PER_LEGO*/ > shd->base.resDecls[j].binding);
                //shd->resDecls[j].visibility = decl->visibility;
                shd->base.resDecls[j].type = decl->type;
                AfxAssert(shd->base.resDecls[j].type != AFX_SHD_RES_TYPE_OUTPUT);
                shd->base.resDecls[j].cnt = decl->cnt;
                 AfxCloneString(&shd->base.resDecls[j].name, &decl->name);
                ++shd->base.resDeclCnt;
            }
        }

        if (!err)
        {
            afxNat ioCnt = AfxCountArrayElements(&blueprint->inOuts);
            shd->base.ioDeclCnt = 0;
            shd->base.ioDecls = NIL;

            if (ioCnt && !(shd->base.ioDecls = AfxAllocate(ctx, ioCnt * sizeof(shd->base.ioDecls[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < ioCnt; i++)
                {
                    afxShaderBlueprintInOut* ioBp = AfxGetArrayUnit(&blueprint->inOuts, i);
                    shd->base.ioDecls[i] = *ioBp;
                    AfxCloneString(&shd->base.ioDecls[i].semantic, &ioBp->semantic);
                    ++shd->base.ioDeclCnt;
                }
            }

            shd->base.topology = blueprint->topology;

            shd->glHandle = NIL;
            shd->glProgHandle = NIL;
            shd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
            shd->compiled = FALSE;

            if (err && shd->base.ioDecls)
                AfxDeallocate(ctx, shd->base.ioDecls);
        }

        if (err && shd->base.resDecls)
            AfxDeallocate(ctx, shd->base.resDecls);
    }

    if (err && shd->base.code)
        AfxDeallocate(ctx, shd->base.code);

    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_SGL afxClassConfig _SglShdClsConfig =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .unitsPerPage = 10,
    .size = sizeof(AFX_OBJECT(afxShader)),
    .ctx = NIL,
    .ctor = (void*)_SglShdCtor,
    .dtor = (void*)_SglShdDtor
};
