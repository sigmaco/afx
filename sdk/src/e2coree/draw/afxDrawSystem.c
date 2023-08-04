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
 *                               (c) 2017 Federação SIGMA
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
#include "_classified/afxDrawClassified.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/mem/afxMemory.h"
#include "afx/draw/io/afxTarga.h"
#include "afxDrawParadigms.h"
#include "../src/e2coree/core/afxCoreParadigms.h"

_AFX afxDrawSystem theDsys;
afxDrawSystem theDsys = NIL;

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

// DRAW SYSTEM API /////////////////////////////////////////////////////////////

_AFX afxClass* AfxGetDrawDriverClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxClass *cls = &dsys->ddrvClass;
    AfxAssertClass(cls, AFX_FCC_DDRV);
    return cls;
}

_AFX afxClass* AfxGetDrawThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxClass *cls = &dsys->dthrClass;
    AfxAssertClass(cls, AFX_FCC_DTHR);
    return cls;
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

_AFX afxNat AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, AFX_FCC_DTHR);
    AfxAssert(dthr);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)dthr);
}

_AFX afxNat AfxEnumerateDrawDrivers(afxNat first, afxNat cnt, afxDrawDriver ddrv[])
{
    afxError err = AFX_ERR_NONE;
    afxClass *cls = AfxGetDrawDriverClass();
    AfxAssertClass(cls, AFX_FCC_DDRV);
    AfxAssert(ddrv);
    return AfxEnumerateFirstClassInstances(cls, first, cnt, (afxObject**)ddrv);
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
    afxClass *cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, AFX_FCC_DTHR);
    return AfxCurateFirstClassInstances(cls, first, cnt, (void*)f, udd);
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

    AfxAssertObject(dsys, AFX_FCC_DSYS);
    AfxAssertRange(dsys->texIoCodecs.cnt, first, cnt);
    AfxAssert(spec);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsys->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
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

    AfxAssertObject(dsys, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssertRange(dsys->texIoCodecs.cnt, first, cnt);
    AfxAssert(storage);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsys->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
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
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssert(specs);
    afxResult hitCnt = 0;
    afxTextureStorageRegistry* codec;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(codec = AfxAllocate(dsys->mem, sizeof(*codec), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            codec->fcc = AFX_FCC_IMIO;
            AfxPushLinkage(&codec->dsys, &dsys->texIoCodecs);
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

_AFX afxMemory AfxGetDrawMemory(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxMemory mem = dsys->mem;
    AfxAssertObject(mem, AFX_FCC_MEM);
    return mem;
}

_AFX afxBool AfxGetDrawSystem(afxDrawSystem *dsys)
{
    if (dsys)
        *dsys = theDsys;

    return !!theDsys;
}

_AFX afxError AfxBootUpDrawSystem(afxDrawSystemSpecification const *spec, afxDrawSystem *dsys)
{
    afxError err = AFX_ERR_NONE;

    if (AfxGetDrawSystem(dsys)) AfxThrowError();
    else
    {
        if (AfxClassAcquireObjects(AfxGetDrawSystemClass(), NIL, 1, spec, (afxObject**)dsys, AfxSpawnHint()))
            AfxThrowError();
    }

    return err;
}

_AFX void AfxShutdownDrawSystem(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;

    if (AfxGetDrawSystem(&dsys))
    {
        while (0 < AfxReleaseObject(&dsys->obj));
    };
}

_AFX afxBool _AfxDsysEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = (void*)obj;
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDsysEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = (void*)obj;
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dsys, AFX_FCC_DSYS);

    AfxReleaseObject(&dsys->e2draw->obj);

    _AfxDropClassChain(&dsys->provisions);

    while (1)
    {
        afxChain *codecs = &dsys->texIoCodecs;
        afxLinkage *first = AfxGetLastLinkage(codecs);

        if (!first) break;
        else
        {
            afxTextureStorageRegistry *codec = AfxContainerOf(first, afxTextureStorageRegistry, dsys);
            AfxAssertType(codec, AFX_FCC_IMIO);
            AfxPopLinkage(&codec->dsys);
            AfxDeallocate(dsys->mem, codec);
        }
    }

    AfxReleaseObject(&dsys->mem->obj);

    theDsys = NIL;
    return err;
}

_AFX afxError _AfxDsysLoadIcds(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dsys, AFX_FCC_DSYS);

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

            if (!AfxAcquireModules(1, &file, &mdle))
            {
                if (mdle == dsys->e2draw)
                {
                    AfxReleaseModules(1, &mdle);
                    continue;
                }

                afxError(*registerDrawDrivers)(afxModule mdle, afxDrawSystem dsys) = AfxFindModuleSymbol(mdle, "AfxRegisterDrawDrivers");
                
                if (registerDrawDrivers && registerDrawDrivers(mdle, dsys)) AfxReleaseObject(&mdle->obj);
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

_AFX afxError _AfxDsysCtor(void *cache, afxNat idx, afxDrawSystem dsys, afxDrawSystemSpecification const *specs)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    (void)cache;
    afxDrawSystemSpecification const *spec = &specs[idx];
    AfxAssert(spec);

    theDsys = dsys; // evil trick to get this working at bootstrap

    if (!(dsys->mem = spec->genrlMem))
    {
        dsys->mem = AfxGetMainMemory();
    }

    AfxAssertObject(dsys->mem, AFX_FCC_MEM);

    if (!(AfxObjectReacquire(&dsys->mem->obj, &dsys->obj, NIL, NIL, NIL))) AfxThrowError();
    else
    {
        {
            afxChain *provisions = &dsys->provisions;
            AfxAcquireChain(provisions, dsys);

            AfxMountClass(&dsys->ddrvClass, provisions, &_AfxDdrvClassSpec);
            AfxMountClassInherited(&dsys->dthrClass, AfxGetThreadClass(), provisions, &_AfxDthrClassSpec);
            AfxAcquireChain(&dsys->texIoCodecs, dsys);

            //AfxObject_Reparent(&dsys->obj, &(drv->obj));

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

                if (AfxAcquireModules(1, &uri, &dsys->e2draw)) AfxThrowError();
                else
                {
                    afxError(*registerDrawDrivers)(afxModule mdle, afxDrawSystem dsys) = AfxFindModuleSymbol(dsys->e2draw, "AfxRegisterDrawDrivers");

                    if (registerDrawDrivers(dsys->e2draw, dsys)) AfxThrowError();
                    else
                    {
                        // find all ICDs here
                        AfxComment("Loading draw ICDs...");
                        _AfxDsysLoadIcds(dsys);

                        afxDrawThread dthr[16];
                        afxNat threadCnt = AfxMin(AfxGetThreadingCapacity(), 16);

                        if (AfxAcquireDrawThreads(NIL, threadCnt, dthr)) AfxThrowError();
                        else
                        {
                            for (afxNat i = 0; i < threadCnt; i++)
                            {
                                AfxSetThreadAffinity(&dthr[i]->thr, i);
                                AfxRunThread(&dthr[i]->thr);
                            }
                        }
                    }

                    if (err)
                        AfxReleaseModules(1, &dsys->e2draw);
                }
            }

            if (err)
                _AfxDropClassChain(&dsys->provisions);
        }

        if (err)
            AfxReleaseObject(&dsys->mem->obj);
    }

    if (err)
        theDsys = NIL;

    return err;
}

AFX afxClassSpecification const _AfxDsysClassSpec;

afxClassSpecification const _AfxDsysClassSpec =
{
    AFX_FCC_DSYS,
    NIL,
    1,
    sizeof(AFX_OBJECT(afxDrawSystem)),
    NIL,
    (void*)_AfxDsysCtor,
    (void*)_AfxDsysDtor,
    .event = _AfxDsysEventHandler,
    .eventFilter = _AfxDsysEventFilter,
    "afxDrawSystem",
    NIL
};
