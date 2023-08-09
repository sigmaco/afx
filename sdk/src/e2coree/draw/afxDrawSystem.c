/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#include "_classified/afxDrawClassified.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/mem/afxContext.h"
#include "afx/draw/io/afxTarga.h"
#include "afxDrawParadigms.h"
#include "../src/e2coree/core/afxCoreParadigms.h"

_AFX afxDrawSystem theDsys;
afxDrawSystem theDsys = NIL;
struct _afxDsysD TheDrawSystem = { 0 };

AFX_DEFINE_STRUCT(afxTextureStorageRegistry)
{
    afxFcc                  fcc;
    afxLinkage              dsys;

    afxString const*    name; // driver name: SIGGL
    afxString const*    author; // author: SIGMA Technology Group
    afxString const*    website; // website: sigmaco.org
    afxString const*    note; // The standard QWADRO draw system implementation.
    afxNat                  verMajor; // 1
    afxNat                  verMinor; // 0
    afxNat                  verPatch; // 0

    afxNat                  minScale;
    afxNat                  maxScale;
    afxNat                  minDepth;
    afxNat                  maxDepth;
    afxNat                  minBpp;
    afxNat                  maxBpp;
    afxBool                 hasAlpha;

    afxNat                  supportedFmtCnt;
    afxString const*    exts;
    afxError                (*acq)(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);
    // coacquire for batch layered textures such as DDS?
    afxError                (*upload)(afxTexture tex, afxUri const *uri);
    afxError                (*download)(afxTexture tex, afxUri const *uri);
    afxError                (*uploadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
    afxError                (*downloadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
};

AFX afxClassSpecification const _AfxDdrvClassSpec;
AFX afxClassSpecification const _AfxDthrClassSpec;

AFX afxBool _AfxGetDsysD(afxDrawSystem dsys, struct _afxDsysD**dsysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxBool rslt = FALSE;

    if (TheDrawSystem.dsysObj == dsys)
    {
        AfxAssert(dsysD);
        *dsysD = &TheDrawSystem;
        rslt = TRUE;
    }
    return rslt;
}

_AFX afxClass* AfxGetDrawDriverClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    afxClass *cls = &dsysD->ddrvClass;
    AfxAssertClass(cls, AFX_FCC_DDRV);
    return cls;
}

_AFX afxNat AfxGetDrawContextCount(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetResidencyCount(&dsysD->contexts);
}

_AFX afxNat AfxGetDrawOutputCount(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetResidencyCount(&dsysD->outputs);
}

_AFX afxNat AfxGetDrawInputCount(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxGetResidencyCount(&dsysD->inputs);
}

_AFX afxNat AfxGetDrawThreadCount(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawDriverClass();
    AfxAssertClass(cls, AFX_FCC_DTHR);
    return AfxGetClassInstanceCount(cls);
}

_AFX afxNat AfxGetDrawDriverCount(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawDriverClass();
    AfxAssertClass(cls, AFX_FCC_DDRV);
    return AfxGetClassInstanceCount(cls);
}

_AFX afxNat AfxEnumerateDrawInputs(afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxEnumerateResidentObjects(&dsysD->inputs, first, cnt, (afxObject*)din);
}

_AFX afxNat AfxEnumerateDrawOutputs(afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxEnumerateResidentObjects(&dsysD->outputs, first, cnt, (afxObject*)dout);
}

_AFX afxNat AfxEnumerateDrawContexts(afxNat first, afxNat cnt, afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxEnumerateResidentObjects(&dsysD->contexts, first, cnt, (afxObject*)dctx);
}

_AFX afxNat AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxEnumerateResidentObjects(&dsysD->threads, first, cnt, (afxObject*)dthr);
}

_AFX afxNat AfxEnumerateDrawDrivers(afxNat first, afxNat cnt, afxDrawDriver ddrv[])
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawDriverClass();
    AfxAssertClass(cls, AFX_FCC_DDRV);
    AfxAssert(ddrv);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxInstance**)ddrv);
}

_AFX afxNat AfxCurateDrawContexts(afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxCurateResidentObjects(&dsysD->contexts, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawOutputs(afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxCurateResidentObjects(&dsysD->outputs, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawInputs(afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxCurateResidentObjects(&dsysD->inputs, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawDrivers(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDriver, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawDriverClass();
    AfxAssertClass(cls, AFX_FCC_DDRV);
    return AfxCurateFirstClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    return AfxCurateResidentObjects(&dsysD->threads, first, cnt, (void*)f, udd);
}

_AFX afxBool AfxGetDrawDriver(afxNat drvIdx, afxDrawDriver *ddrv)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    if ((rslt = !!(AfxEnumerateDrawDrivers(drvIdx, 1, ddrv))))
    {
        AfxAssertObject(*ddrv, AFX_FCC_DDRV);
    }
    return rslt;
}

#if 0
_AFX afxError AfxProcessDrawSystem(afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mainThreadId == AfxGetThreadingId());

    for (afxNat i = 0; i < AfxGetDrawDriverCount(); i++)
    {
        afxDrawDriver ddrv;
        
        if (AfxEnumerateDrawDrivers(i, 1, &ddrv))
        {
            AfxAssertObject(ddrv, AFX_FCC_DDRV);

            if (ddrv->vmt->proc(ddrv, timeout))
                AfxThrowError();
        }
    }
    return err;
}
#endif

_AFX afxNat AfxChooseTextureStorage(afxTexture tex, afxTextureRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    (void)tex;
    (void)rgn;
    AfxThrowError();
    return 0;
}

_AFX afxResult AfxDescribeTextureStorages(afxNat first, afxNat cnt, afxTextureStorageSpecification spec[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertRange(dsysD->texIoCodecs.cnt, first, cnt);
    AfxAssert(spec);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsysD->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
    {
        AfxAssert(codec->fcc == AFX_FCC_IMIO);

        if (posn >= first)
        {
            spec[rslt].name = codec->name;
            spec[rslt].author = codec->author;
            spec[rslt].website = codec->website;
            spec[rslt].note = codec->note;
            spec[rslt].verMajor = codec->verMajor;
            spec[rslt].verMinor = codec->verMinor;
            spec[rslt].verPatch = codec->verPatch;
            spec[rslt].extCnt = codec->supportedFmtCnt;
            spec[rslt].exts = codec->exts;
            spec[rslt].minScale = codec->minScale;
            spec[rslt].maxScale = codec->maxScale;
            spec[rslt].minDepth = codec->minDepth;
            spec[rslt].maxDepth = codec->maxDepth;
            spec[rslt].minBpp = codec->minBpp;
            spec[rslt].maxBpp = codec->maxBpp;
            spec[rslt].hasAlpha = codec->hasAlpha;
            spec[rslt].acq = codec->acq;
            spec[rslt].upload = codec->upload;
            spec[rslt].uploadRgn = codec->uploadRgn;
            spec[rslt].download = codec->download;
            spec[rslt].downloadRgn = codec->downloadRgn;
            ++rslt;
        }

        ++posn;

        if (posn >= first + cnt)
            break;
    }
    return rslt;
}

_AFX afxResult AfxEnumerateTextureStorages(afxNat first, afxNat cnt, afxTextureStorageRegistry *storage[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssertRange(dsysD->texIoCodecs.cnt, first, cnt);
    AfxAssert(storage);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsysD->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
    {
        AfxAssert(codec->fcc == AFX_FCC_IMIO);

        if (posn >= first)
        {
            storage[rslt] = codec;
            ++rslt;
        }

        ++posn;

        if (posn >= first + cnt)
            break;
    }
    return rslt;
}

_AFX afxResult AfxRegisterTextureStorages(afxNat cnt, afxTextureStorageSpecification const specs[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssert(specs);
    afxResult hitCnt = 0;
    afxTextureStorageRegistry* codec;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(codec = AfxAllocate(dsysD->mem, sizeof(*codec), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            codec->fcc = AFX_FCC_IMIO;
            AfxPushLinkage(&codec->dsys, &dsysD->texIoCodecs);
            codec->supportedFmtCnt = 0;

            codec->name = specs[i].name;
            codec->author = specs[i].author;
            codec->website = specs[i].website;
            codec->note = specs[i].note;

            codec->verMajor = specs[i].verMajor;
            codec->verMinor = specs[i].verMinor;
            codec->verPatch = specs[i].verPatch;

            codec->acq = specs[i].acq;
            codec->upload = specs[i].upload;
            codec->uploadRgn = specs[i].uploadRgn;
            codec->download = specs[i].download;
            codec->downloadRgn = specs[i].downloadRgn;
            
            codec->exts = specs[i].exts;
            codec->supportedFmtCnt = specs[i].extCnt;

            AfxLogMessageFormatted(0xFFFF0000, "\nNew texture codec registered.");
            AfxLogMessageFormatted(0xFFFF0000, "\nName: %.*s", AfxPushString(codec->name));
            AfxLogMessageFormatted(0xFFFF0000, "\nAuthor: %.*s", AfxPushString(codec->author));
            AfxLogMessageFormatted(0xFFFF0000, "\nVersion: %u.%u.%u", codec->verMajor, codec->verMinor, codec->verPatch);
            AfxLogMessageFormatted(0xFFFF0000, "\nNote: %.*s", AfxPushString(codec->note));

            hitCnt++;
        }
    }
    return hitCnt;
}

_AFX afxContext AfxGetDrawMemory(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    afxContext mem = dsysD->mem;
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);
    return mem;
}

_AFX afxBool AfxGetDrawSystem(afxDrawSystem *dsys)
{
    if (dsys)
        *dsys = theDsys;

    return !!theDsys;
}

_AFX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    AfxReleaseModules(1, &dsysD->e2draw);

    _AfxDropClassChain(&dsysD->provisions);

    while (1)
    {
        afxChain *codecs = &dsysD->texIoCodecs;
        afxLinkage *first = AfxGetLastLinkage(codecs);

        if (!first) break;
        else
        {
            afxTextureStorageRegistry *codec = AFX_REBASE(first, afxTextureStorageRegistry, dsys);
            AfxAssertType(codec, AFX_FCC_IMIO);
            AfxPopLinkage(&codec->dsys);
            AfxDeallocate(dsysD->mem, codec);
        }
    }

    AfxReleaseContexts(1, &dsysD->mem);

    theDsys = NIL;
    return err;
}

_AFX void AfxAbolishDrawSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;

    if (AfxGetDrawSystem(&dsys))
    {
        if (_AfxDsysDtor(dsys))
            AfxThrowError();

        while(!AfxReleaseObjects(1, &dsys, NIL));

        theDsys = NIL;
        AFX_ZERO(&TheDrawSystem);
    };
}

_AFX afxError _AfxDsysLoadIcds(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    WIN32_FIND_DATAA wfd;
    HANDLE fh;

    if ((fh = FindFirstFileA("*.icd", &(wfd))))
    {
        do
        {
            afxUri uri, file;
            AfxUriWrapLiteral(&uri, wfd.cFileName, 0);
            AfxExcerptUriObject(&file, &uri);

            afxModule mdle;

            if (!AfxAcquireModules(1, &mdle, &file))
            {
                if (mdle == dsysD->e2draw)
                {
                    AfxReleaseModules(1, &mdle);
                    continue;
                }

                afxError(*registerDrawDrivers)(afxModule mdle, afxDrawSystem dsys) = AfxFindModuleSymbol(mdle, "AfxRegisterDrawDrivers");
                
                if (registerDrawDrivers && registerDrawDrivers(mdle, dsys)) AfxReleaseModules(1, &mdle);
                else
                {
                    afxError(*registerTextureStorages)(afxModule mdle, afxDrawSystem dsys) = AfxFindModuleSymbol(mdle, "AfxRegisterTextureStorages");

                    if (registerTextureStorages && registerTextureStorages(mdle, dsys))
                            AfxThrowError();
                }
            }
        } while (FindNextFileA(fh, &wfd));
        FindClose(fh);
    }
    return err;
}

_AFX afxError _AfxDsysCtor(afxDrawSystem dsys, afxDrawSystemConfig const *config)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    
    
    struct _afxDsysD* dsysD = &TheDrawSystem;
    //_AfxGetDsysD(dsys, &dsysD);
    //AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssignTypeFcc(dsysD, AFX_FCC_DSYS);
    AfxAssert(config);

    theDsys = dsys; // evil trick to get this working at bootstrap

    dsysD->dsysObj = dsys;

    if (!(dsysD->mem = config->genrlMem))
    {
        dsysD->mem = AfxGetMainMemory();
    }

    AfxAssertObjects(1, &dsysD->mem, AFX_FCC_MEM);

    if (AfxReacquireObjects(1, &dsysD->mem)) AfxThrowError();
    else
    {
        {
            afxChain *provisions = &dsysD->provisions;
            AfxAcquireChain(provisions, dsys);

            AfxAcquireResidency(&dsysD->outputs, sizeof(struct _afxDoutD), 1);
            AfxAcquireResidency(&dsysD->inputs, sizeof(struct _afxDinD), 1);
            AfxAcquireResidency(&dsysD->contexts, sizeof(struct _afxDctxD), 1);
            AfxAcquireResidency(&dsysD->threads, sizeof(struct _afxDthrD), 1);


            AfxMountClass(&dsysD->ddrvClass, provisions, &_AfxDdrvClassSpec);
            AfxAcquireChain(&dsysD->texIoCodecs, dsys);

            static afxString codecName, codecAuthor, codecWebsite, codecNote, ext;
            afxTextureStorageSpecification stdTexCodec = { 0 };
            AfxWrapStringLiteral(&codecName, "SIGGL", 0);
            AfxWrapStringLiteral(&codecAuthor, "SIGMA Technology Group", 0);
            AfxWrapStringLiteral(&codecWebsite, "www.sigmaco.org", 0);
            AfxWrapStringLiteral(&codecNote, "", 0);
            AfxWrapStringLiteral(&ext, ".tga", 0);

            stdTexCodec.name = &codecName;
            stdTexCodec.author = &codecAuthor;
            stdTexCodec.website = &codecWebsite;
            stdTexCodec.note = &codecNote;
            stdTexCodec.verMajor = 0;
            stdTexCodec.verMinor = 5;
            stdTexCodec.verPatch = 1;
            stdTexCodec.extCnt = 1;
            stdTexCodec.exts = &ext;
            stdTexCodec.acq = AfxLoadTexturesTarga;
            stdTexCodec.download = AfxPrintTextureToTarga;
            stdTexCodec.downloadRgn = AfxPrintTextureRegionsToTarga;
            stdTexCodec.upload = AfxFetchTextureFromTarga;
            stdTexCodec.uploadRgn = AfxFetchTextureRegionsFromTarga;

            if (1 != AfxRegisterTextureStorages(1, &stdTexCodec)) AfxThrowError();
            else
            {
                AfxEcho("Truecolor Targa texture serializer installed!");

                afxUri uri;
                AfxUriWrapLiteral(&uri, "e2draw.icd", 0);

                if (AfxAcquireModules(1, &dsysD->e2draw, &uri)) AfxThrowError();
                else
                {
                    afxError(*registerDrawDrivers)(afxModule mdle, afxDrawSystem dsys) = AfxFindModuleSymbol(dsysD->e2draw, "AfxRegisterDrawDrivers");

                    if (registerDrawDrivers(dsysD->e2draw, dsys)) AfxThrowError();
                    else
                    {
                        // find all ICDs here
                        AfxComment("Loading draw ICDs...");
                        _AfxDsysLoadIcds(dsys);

                        afxDrawThread dthr[16];
                        afxNat threadCnt = AfxMini(AfxGetThreadingCapacity(), 16);

                        if (AfxAcquireDrawThreads(threadCnt, dthr, NIL)) AfxThrowError();
                        else
                        {
                            for (afxNat i = 0; i < threadCnt; i++)
                            {
                                AfxAssertObjects(1, &dthr[i], AFX_FCC_DTHR);
                                struct _afxDthrD *dthrD;
                                _AfxGetDthrD(dthr[i], &dthrD, dsysD);
                                AfxAssertType(dthrD, AFX_FCC_DTHR);
                                AfxSetThreadAffinity(dthrD->thr, i);
                                AfxRunThread(dthrD->thr);
                            }
                        }
                    }

                    if (err)
                        AfxReleaseModules(1, &dsysD->e2draw);
                }
            }

            if (err)
                _AfxDropClassChain(&dsysD->provisions);
        }

        if (err)
            AfxReleaseObjects(1, &dsysD->mem, NIL);
    }

    if (err)
        theDsys = NIL;

    return err;
}

_AFX afxError AfxEstablishDrawSystem(afxDrawSystemConfig const *config, afxDrawSystem *dsys)
{
    afxError err = AFX_ERR_NONE;

    if (AfxGetDrawSystem(dsys)) AfxThrowError();
    else
    {
        afxDrawSystem dsysObj;

        if (AfxAcquireNamedObjects(1, &dsysObj, AFX_FCC_DSYS, "The Qwadro Draw System")) AfxThrowError();
        else
        {
            AFX_ZERO(&TheDrawSystem);
            TheDrawSystem.fcc = AFX_FCC_DSYS;

            if (_AfxDsysCtor(dsysObj, config))
                AfxThrowError();

            if (err)
                *dsys = (theDsys = NIL);
            else
                *dsys = (theDsys = dsysObj);

            if (err)
                while(!AfxReleaseObjects(1, &dsysObj, NIL));
        }
    }
    return err;
}

_AFX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxZero(config, AfxMaxi(extendedSiz, sizeof(*config)));
}
