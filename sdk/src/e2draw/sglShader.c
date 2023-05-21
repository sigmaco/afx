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

typedef struct
{
    afxDrawContext dctx;
    afxShaderBlueprint const* blueprint;
} _afxShdCtorArgs;

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDqueSyncShd(afxDrawQueue dque, afxShader shd, afxShaderStage stage)
{
    //AfxEntry("shd=%p", shd);
    afxError err = NIL;
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
    afxError err = NIL;
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
    afxError err = NIL;
    AfxAssertObject(shd, AFX_FCC_SHD);
    AfxAssertObject(ios, AFX_FCC_IOS);

    AfxStreamWrite(ios, shd->code, shd->codeLen);

    return err;
}

_SGL afxNat _AfxShaderGetLegoTemplateCount(afxShader shd)
{
    afxError err = NIL;
    AfxAssertObject(shd, AFX_FCC_SHD);
    return shd->legtCnt;
}

_SGL afxLegoTemplate _AfxShaderGetLegoTemplate(afxShader shd, afxNat idx)
{
    afxError err = NIL;
    AfxAssertObject(shd, AFX_FCC_SHD);
    AfxAssert(shd->legtCnt > idx);
    afxLegoTemplate legt = shd->legt[idx];
    AfxAssertObject(legt, AFX_FCC_LEGT);
    return legt;
}

_SGL afxShaderStage _AfxShaderGetStage(afxShader shd)
{
    afxError err = NIL;
    AfxAssertObject(shd, AFX_FCC_SHD);
    return shd->stage;
}

_SGL afxBool _SglShdEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = NIL;
    afxShader shd = (void*)obj;
    AfxAssertObject(shd, AFX_FCC_SHD);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglShdEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = NIL;
    afxShader shd = (void*)obj;
    AfxAssertObject(shd, AFX_FCC_SHD);
    (void)watched;
    (void)ev;

    if (ev->type == AFX_EVENT_OBJ_DESTROYED)
    {
        afxFcc fcc = AfxObjectGetFcc(watched);

        if (fcc == AFX_FCC_LEGT)
        {
            for (afxNat i = 0; i < shd->legtCnt; i++)
            {
                afxLegoTemplate legt = shd->legt[i];
                AfxAssertObject(legt, AFX_FCC_LEGT);

                if (legt == watched)
                {
                    shd->legt[i] = NIL;
                }
            }
        }
    }

    return FALSE;
}

_SGL afxError _AfxShdDtor(afxShader shd)
{
    afxError err = NIL;
    AfxAssertObject(shd, AFX_FCC_SHD);

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

    for (afxNat i = 0; i < shd->legtCnt; i++)
    {
        afxLegoTemplate legt = shd->legt[i];

        if (legt)
        {
            AfxAssertObject(legt, AFX_FCC_LEGT);
            AfxObjectRelease(&legt->obj);
        }
    }

    if (shd->code)
    {
        afxDrawContext dctx = AfxShaderGetContext(shd);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxDrawContextGetMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        AfxDeallocate(mem, shd->code);
    }
    return err;
}

_SGL afxShdImpl const _AfxStdShdImpl;

afxShdImpl const _AfxStdShdImpl =
{
    _AfxShaderGetStage,
    _AfxShaderGetLegoTemplate,
    _AfxShaderGetLegoTemplateCount,
    _AfxShaderDownload,
    _AfxShaderSerialize
};

_SGL afxError _AfxShdCtor(afxShader shd, _afxShdCtorArgs *args)
{
    AfxEntry("shd=%p", shd);
    afxError err = NIL;
    AfxAssertObject(args->dctx, AFX_FCC_DCTX);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(args->dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    afxMemory mem = AfxDrawContextGetMemory(args->dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    afxShaderBlueprint const *blueprint = args->blueprint;
    AfxString32(&shd->name, &blueprint->name.str);

    shd->stage = blueprint->stage;

    static afxString l_str_main = AFX_STRING("main");
    AfxString8(&shd->entry, AfxStringIsEmpty(&blueprint->entry.str) ? &l_str_main : &blueprint->entry.str);

    shd->code = NIL;
    shd->codeLen = 0;
    afxByte *bytecode;

    AfxAssert(blueprint->srcCnt <= sizeof(blueprint->srcs) / sizeof(blueprint->srcs[0]));

    for (afxNat i = 0; i < blueprint->srcCnt; i++)
    {
        AfxAssert(i <= sizeof(blueprint->srcs) / sizeof(blueprint->srcs[0]));

        switch (blueprint->srcs[i].type)
        {
        case AFX_FCC_NIL:
        {
            afxNat siz = blueprint->srcs[i].data.range;

            if (!(bytecode = AfxReallocate(mem, shd->code, shd->codeLen + siz, AfxSpawnHint()))) AfxThrowError();
            else
            {
                AfxAssert(blueprint->srcs[i].data.start);
                AfxCopy(&bytecode[shd->codeLen], blueprint->srcs[i].data.start, siz);
                shd->codeLen += siz;
                bytecode[shd->codeLen] = '\0';
                shd->code = bytecode;
            }
            break;
        }
        case AFX_FCC_URI:
        {
            afxFile file;
            AfxAssertType(&blueprint->srcs[i].uri, AFX_FCC_URI);

            if (!(file = AfxFileSystemOpenReadableFile(fsys, &blueprint->srcs[i].uri))) AfxThrowError();
            else
            {
                AfxAssertObject(file, AFX_FCC_FILE);
                afxNat size = AfxStreamMeasure(&file->ios);
                AfxAssert(size);

                if (!(bytecode = AfxReallocate(mem, shd->code, shd->codeLen + size, AfxSpawnHint()))) AfxThrowError();
                else
                {
                    if (AfxStreamRead(&file->ios, &bytecode[shd->codeLen], size)) AfxThrowError();
                    else
                    {
                        shd->codeLen += size;
                        bytecode[shd->codeLen] = '\0';
                    }
                    shd->code = bytecode;
                }
                AfxObjectRelease(&file->ios.obj);
            }
            break;
        }
        case AFX_FCC_IOS:
        {
            afxNat range = blueprint->srcs[i].stream.range;
            AfxAssert(range);

            if (!(bytecode = AfxReallocate(mem, shd->code, shd->codeLen + range, AfxSpawnHint()))) AfxThrowError();
            else
            {
                afxStream ios = blueprint->srcs[i].stream.ios;
                AfxAssertObject(ios, AFX_FCC_IOS);
                afxSize offset = blueprint->srcs[i].stream.offset;
                afxNat posn = AfxStreamAsk(ios);
                
                if (AfxStreamGoToBegin(ios, offset)) AfxThrowError();
                else
                {
                    if (AfxStreamRead(ios, &bytecode[shd->codeLen], range)) AfxThrowError();
                    else
                    {
                        shd->codeLen += range;
                        bytecode[shd->codeLen] = '\0';
                    }
                }
                AfxStreamGoToBegin(ios, (afxInt)posn);
                shd->code = bytecode;
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }

    AfxComment("%.*s", shd->codeLen, shd->code);

    if (!err)
    {
        shd->legtCnt = 0;

        for (afxNat i = 0; i < _SGL_MAX_LEGO_PER_BIND; i++)
        {
            afxNat bindDeclCnt = 0;
            afxLegoBindingDecl bindDecl[_SGL_MAX_ENTRY_PER_LEGO];

            for (afxNat j = 0; j < blueprint->resDeclCnt; j++)
            {
                if (blueprint->resDecls[j].set == i)
                {
                    bindDecl[bindDeclCnt].binding = blueprint->resDecls[j].binding;
                    bindDecl[bindDeclCnt].visibility = blueprint->resDecls[j].visibility;
                    bindDecl[bindDeclCnt].type = blueprint->resDecls[j].type;
                    bindDecl[bindDeclCnt].cnt = blueprint->resDecls[j].cnt;
                    bindDecl[bindDeclCnt].name = &blueprint->resDecls[j].name.str;
                    ++bindDeclCnt;
                }
            }

            if (!bindDeclCnt) shd->legt[shd->legtCnt] = NIL;
            else
            {
                if (!(shd->legt[shd->legtCnt] = AfxDrawContextBuildLegoTemplate(args->dctx, bindDeclCnt, bindDecl)))
                {
                    AfxThrowError();
                }
                else
                {
                    if (AfxObjectInstallEventFilter(&(shd->legt[shd->legtCnt]->obj), &shd->obj))
                    {
                        AfxThrowError();
                    }
                    else
                    {
                        ++shd->legtCnt;
                    }
                }
            }

            if (err)
                break;
        }
        
        if (!err)
        {
            shd->inDeclCnt = 0;

            for (afxNat i = 0; i < blueprint->inDeclCnt; i++)
            {
                shd->inDecls[i] = blueprint->inDecls[i];
                ++shd->inDeclCnt;
            }

            shd->outDeclCnt = 0;

            for (afxNat i = 0; i < blueprint->outDeclCnt; i++)
            {
                shd->outDecls[i] = blueprint->outDecls[i];
                ++shd->outDeclCnt;
            }

            shd->glHandle = NIL;
            shd->glProgHandle[0] = NIL;
            shd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
            shd->compiled = FALSE;
        }

        if (err)
        {
            for (afxNat i = 0; i < shd->legtCnt; i++)
            {
                afxLegoTemplate legt = shd->legt[i];
                AfxAssertObject(legt, AFX_FCC_LEGT);
                AfxObjectRelease(&legt->obj);
            }
        }
    }

    if (err && shd->code)
        AfxDeallocate(mem, shd->code);

    return err;
}

_SGL afxResult _AfxDrawContextBuildShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shd[])
{
    afxError err = NIL;
    AfxEntry("dctx=%p,cnt=%u,blueprint=%p,shd=%p", dctx, cnt, blueprint, shd);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(shd);
    afxResult rslt = NIL;

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxShdCtorArgs args =
        {
            dctx,
            &blueprint[i]
        };

        AfxEcho("Building draw operation '%.*s'", AfxPushString(&blueprint[i].name.str));


        if (!(shd[i] = AfxObjectAcquire(AfxDrawContextGetShaderClass(dctx), &args, AfxSpawnHint()))) AfxThrowError();
        else
        {
            ++rslt;
        }

    }
    return rslt;
}

_SGL void _SglIteratorCompareNameCiShd(afxIterator *iter)
{
    struct { afxUri const *name; afxObject *obj; } *data = iter->udd;
    afxShader shd = iter->voidItem;

    afxUri name;
    AfxUriExcerptName(&name, data->name);

    if (!AfxUriIsBlank(&name))
    {
        if (0 == AfxStringCompareCi(&shd->name.str, AfxUriGetStringConst(&name)))
        {
            data->obj = &shd->obj;
            iter->abort = TRUE;
        }
    }
}

_SGL afxResult _AfxDrawContextFindShaders(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxShader shd[])
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(cnt);
    AfxAssert(name);
    AfxAssert(shd);
    afxResult foundCnt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(&name[i]));

        struct { afxUri const *name; afxObject *obj; } data = { &name[i], NIL };
        AfxClassForEveryInstance(&dctx->shdClass, FALSE, _SglIteratorCompareNameCiShd, &data);

        if ((shd[i] = (afxShader)data.obj))
            ++foundCnt;
    }
    return foundCnt;
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
