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

#include "../e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/pipelining/afxShader.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/io/afxUri.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDqueSyncShd(afxDrawQueue dque, afxShader shd, afxShaderStage stage)
{
    //AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);

    sglVmt const* gl = &dque->wglVmt;

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
                const GLint codeLens[] = { shd->codeLen };
                GLchar const* const codes[] = { (GLchar const*)shd->code };
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
                    AFX_ZERO(shd->glProgHandle);
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

_SGL afxError _AfxShaderDownload(afxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!(AfxUriIsBlank(uri)));

    afxDrawContext dctx = AfxShaderGetContext(shd);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    afxFile file;

    if (!(file = AfxFileSystemOpenFile(fsys, AFX_IO_FLAG_W, uri))) AfxThrowError();
    else
    {
        if (AfxShaderSerialize(shd, &file->ios)) AfxThrowError();
        else
        {
            // success
        }
        AfxObjectRelease(&file->ios.obj);
    }
    return err;
}

_SGL afxError _AfxShaderSerialize(afxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    AfxAssertObject(ios, AFX_FCC_IOS);

    AfxStreamWrite(ios, shd->code, shd->codeLen);

    return err;
}

_SGL afxNat _AfxShaderGetInterfaceCount(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    return shd->resDeclCnt;
}

_SGL afxResult _AfxShaderDescribeInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    AfxAssert(rsrc);
    AfxAssert(cnt);
    AfxAssert(first < shd->resDeclCnt);
    AfxAssert(first + cnt <= shd->resDeclCnt);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        rsrc[i].set = shd->resDecls[first + i].set;
        rsrc[i].binding = shd->resDecls[first + i].binding;
        rsrc[i].type = shd->resDecls[first + i].type;
        rsrc[i].cnt = shd->resDecls[first + i].cnt;
        rsrc[i].name = shd->resDecls[first + i].name;
        ++rslt;
    }
    return rslt;
}

_SGL afxShaderStage _AfxShaderGetStage(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    return shd->stage;
}

_SGL afxUri const* _AfxShaderGetUri(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);
    return shd->uri;
}

_SGL afxBool _SglShdEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxShader shd = (void*)obj;
    AfxAssertObject(shd, AFX_FCC_SHD);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglShdEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxShader shd = (void*)obj;
    AfxAssertObject(shd, AFX_FCC_SHD);
    (void)watched;
    (void)ev;

    return FALSE;
}

_SGL afxError _AfxShdDtor(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);

    afxDrawContext dctx = AfxShaderGetContext(shd);
    afxMemory mem = AfxDrawContextGetMemory(dctx);

    if (shd->glHandle)
    {
        afxDrawContext dctx = AfxShaderGetContext(shd);
        _SglEnqueueGlResourceDeletion(dctx, 0, 6, shd->glHandle);
        shd->glHandle = 0;
    }

    if (shd->glProgHandle[0])
    {
        afxDrawContext dctx = AfxShaderGetContext(shd);
        _SglEnqueueGlResourceDeletion(dctx, 0, 7, shd->glProgHandle[0]);
        shd->glProgHandle[0] = 0;
    }

    if (shd->resDecls)
    {
        for (afxNat j = 0; j < shd->resDeclCnt; j++)
        {
            if (shd->resDecls[j].name)
                AfxStringDeallocate(shd->resDecls[j].name);
        }

        AfxDeallocate(mem, shd->resDecls);
    }

    if (shd->ioDecls)
    {
        for (afxNat j = 0; j < shd->ioDeclCnt; j++)
        {
            if (shd->ioDecls[j].semantic)
                AfxStringDeallocate(shd->ioDecls[j].semantic);
        }

        AfxDeallocate(mem, shd->ioDecls);
    }

    if (shd->code)
    {
        afxDrawContext dctx = AfxShaderGetContext(shd);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxDrawContextGetMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        AfxDeallocate(mem, shd->code);
    }

    if (shd->uri)
        AfxUriDeallocate(shd->uri);

    return err;
}

_SGL afxShdImpl const _AfxStdShdImpl;

afxShdImpl const _AfxStdShdImpl =
{
    _AfxShaderGetUri,
    _AfxShaderGetStage,
    _AfxShaderDescribeInterfaces,
    _AfxShaderGetInterfaceCount,
    _AfxShaderDownload,
    _AfxShaderSerialize
};

_SGL afxError _AfxShdCtor(void *cache, afxNat idx, afxShader shd, afxShaderBlueprint const *blueprints)
{
    AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = AfxObjectGetProvider(&shd->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    afxShaderBlueprint const *blueprint = &blueprints[idx];

    shd->uri = AfxUriIsBlank(&blueprint->uri.uri) ? NIL : AfxUriClone(&blueprint->uri.uri);

    shd->stage = blueprint->stage;

    shd->entry = AfxStringIsEmpty(&blueprint->entry.str) ? NIL : AfxStringClone(&blueprint->entry.str);

    shd->code = NIL;
    afxNat codeLen = AfxArrayGetPop(&blueprint->codes);

    if (codeLen && !(shd->code = AfxAllocate(mem, codeLen, AfxSpawnHint()))) AfxThrowError();
    else
    {
        AfxAssertType(&blueprint->codes, AFX_FCC_ARR);
        AfxCopy(shd->code, AfxArrayGetPool(&blueprint->codes), codeLen);
    }

    shd->codeLen = codeLen;
    //shd->code[shd->codeLen] = '\0';

    AfxComment("%.*s", shd->codeLen, shd->code);

    if (!err)
    {
        shd->resDeclCnt = 0;
        shd->resDecls = NIL;

        afxNat resDeclCnt = AfxArrayGetPop(&blueprint->resources);

        if (resDeclCnt && !(shd->resDecls = AfxAllocate(mem, resDeclCnt * sizeof(shd->resDecls[0]), AfxSpawnHint()))) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < resDeclCnt; j++)
            {
                afxShaderBlueprintResource const *decl = AfxArrayGetElement(&blueprint->resources, j);
                shd->resDecls[j].set = decl->set;
                AfxAssert(_SGL_MAX_LEGO_PER_BIND > shd->resDecls[j].set);
                shd->resDecls[j].binding = decl->binding;
                AfxAssert(_SGL_MAX_ENTRY_PER_LEGO > shd->resDecls[j].binding);
                //shd->resDecls[j].visibility = decl->visibility;
                shd->resDecls[j].type = decl->type;
                AfxAssert(shd->resDecls[j].type != AFX_SHD_RES_TYPE_OUTPUT);
                shd->resDecls[j].cnt = decl->cnt;
                shd->resDecls[j].name = decl->name && !AfxStringIsEmpty(decl->name) ? AfxStringClone(decl->name) : NIL;
                ++shd->resDeclCnt;
            }
        }
        
        if (!err)
        {
            afxNat ioCnt = AfxArrayGetPop(&blueprint->inOuts);
            shd->ioDeclCnt = 0;
            shd->ioDecls = NIL;
            
            if (ioCnt && !(shd->ioDecls = AfxAllocate(mem, ioCnt * sizeof(shd->ioDecls[0]), AfxSpawnHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < ioCnt; i++)
                {
                    afxShaderBlueprintInOut* ioBp = AfxArrayGetElement(&blueprint->inOuts, i);
                    shd->ioDecls[i] = *ioBp;
                    shd->ioDecls[i].semantic = ioBp->semantic && !AfxStringIsEmpty(ioBp->semantic) ? AfxStringClone(ioBp->semantic) : NIL;
                    ++shd->ioDeclCnt;
                }
            }

            shd->topology = blueprint->topology;

            shd->glHandle = NIL;
            shd->glProgHandle[0] = NIL;
            shd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
            shd->compiled = FALSE;

            if (err && shd->ioDecls)
                AfxDeallocate(mem, shd->ioDecls);
        }

        if (err && shd->resDecls)
            AfxDeallocate(mem, shd->resDecls);
    }

    if (err && shd->code)
        AfxDeallocate(mem, shd->code);

    return err;
}

_SGL afxClassSpecification const _AfxShdClassSpec;

afxClassSpecification const _AfxShdClassSpec =
{
    AFX_FCC_SHD,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxShader)),
    NIL,
    (void*)_AfxShdCtor,
    (void*)_AfxShdDtor,
    .event = _SglShdEventHandler,
    .eventFilter = _SglShdEventFilter,
    "afxShader",
    &_AfxStdShdImpl
};
