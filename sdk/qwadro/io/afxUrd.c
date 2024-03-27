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

#define _CRT_SECURE_NO_WARNINGS 1
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _AFX_URD_C
#include "qwadro/core/afxManager.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/io/afxData.h"

#define LOWORD(l) ((afxNat16)(l))
#define HIWORD(l) ((afxNat16)(((afxNat32)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((afxByte)(w))
#define HIBYTE(w) ((afxByte)(((afxNat16)(w) >> 8) & 0xFF))

////////////////////////////////////////////////////////////////////////////////
// Writer
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Writer
////////////////////////////////////////////////////////////////////////////////


_AFX void Reverse32_(int Count, void *BufferInit)
{
    afxNat32 *v2; // ecx
    int v3; // edx

    v2 = BufferInit;
    if (Count / 4)
    {
        v3 = Count / 4;
        do
        {
            *v2 = (((*v2 << 16) | *v2 & 0xFF00) << 8) | (((*v2 >> 16) | *v2 & 0xFF0000u) >> 8);
            ++v2;
            --v3;
        } while (v3);
    }
}

_AFX void Reverse16(int Count, void *BufferInit)
{
    int v2; // eax
    afxNat16 *v3; // ecx
    int v4; // eax
    int v5; // edx
    int v6; // esi
    __int16 v7; // dx

    v2 = Count / 2;
    v3 = BufferInit;
    if (Count / 2 % 2)
    {
        if (v2)
        {
            v6 = Count / 2;
            do
            {
                //LOBYTE(v7) = *v3 >> 8;
                //HIBYTE(v7) = *v3;
                ++v3;
                --v6;
                *(v3 - 1) = v7;
            } while (v6);
        }
    }
    else
    {
        v4 = v2 / 2;
        if (v4)
        {
            v5 = v4;
            do
            {
                *(afxNat32*)v3 = (*(afxNat32*)v3 << 8) ^ ((*(afxNat32*)v3 << 8) ^ (*(afxNat32*)v3 >> 8)) & 0xFF00FF;
                v3 += 2;
                --v5;
            } while (v5);
        }
    }
}

_AFX void ReverseSection(int First16Bit, int First8Bit, int End, void *BufferInit)
{
    Reverse32_(First16Bit, BufferInit);
    Reverse16(First8Bit - First16Bit, (char *)BufferInit + First16Bit);
}

_AFX void* LoadFileSection2(afxUrdSection const *Section, void *DestinationMemory, void *Reader, afxBool fileIsByteReversed)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(Section);
    AfxAssert(DestinationMemory);
    AfxAssert(Reader);

    void *result = 0;
    void *Result = 0;

    if (Section->expandedDataSiz)
    {
        afxNat alignedSiz = (Section->expandedDataSiz + 3) & 0xFFFFFFFC;

        if (DestinationMemory)
        {
            result = DestinationMemory;
            Result = DestinationMemory;
        }
        else
        {
            //use Section->InternalAlignment
            Result = AfxAllocate(1, alignedSiz, 0, AfxHint());
            result = Result;
        }

        if (result)
        {
            if (Section->fmt)
            {
                afxNat v8 = AfxGetCompressionPaddingSize(Section->fmt);
                void *v9 = AfxAllocate(1, Section->dataSiz + v8, 0, AfxHint());

                if (v9)
                {
                    AfxReadStreamAt(Reader, Section->dataOffset, Section->dataSiz, 0, v9);

                    if (!AfxDecompressData(Section->fmt, fileIsByteReversed, Section->dataSiz, v9, /*Section->first16Bit*/0, /*Section->first8Bit*/0, Section->expandedDataSiz, Result))
                    {
                        AfxDeallocate(Result);
                        Result = 0;
                    }
                }

                AfxDeallocate(v9);
                result = Result;
            }
            else
            {
                AfxReadStreamAt(Reader, Section->dataOffset, Section->dataSiz, 0, result);
                result = Result;
            }
        }
    }
    return result;
}

_AFX void* AfxDecodeUrdReference(void const **sections, afxUrdReference const* ref)
{
    return (char *)sections[ref->secIdx] + ref->offset;
}

_AFX afxNat AfxFindUrdSectionOfLoadedObject(afxUrd urd, void const* obj)
{
    afxNat secCnt = urd->hdr->secCnt;    
    afxNat result = 0;

    if (!secCnt)
        return -1;

    afxUrdSection** sections = urd->sections;
    afxNat *expandedDataSiz = &((afxUrdSection*)&(((afxByte*)urd->hdr)[urd->hdr->secOffset]))->expandedDataSiz;

    while (obj < *sections || obj >= (char *)*sections + *expandedDataSiz)
    {
        ++result;
        ++sections;
        expandedDataSiz += 11;

        if (result >= secCnt)
            return -1;
    }
    return result;
}

_AFX afxBool AfxOpenUrdSections(afxUrd urd, afxNat baseIdx, afxNat secCnt, void *buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);

    AfxAssert(urd->hdr);
    AfxAssertRange(urd->secCnt, baseIdx, secCnt);

    afxBool ret = TRUE;

    afxUrdSection *sections = ((afxUrdSection*)((afxByte*)urd->hdr + urd->hdr->secOffset));

    for (afxNat i = 0; i < secCnt; i++)
    {
        afxUrdSection *sec = &sections[baseIdx + i];

        urd->isUserMem[baseIdx + i] = !!buf;

        if (!(!sec->expandedDataSiz || urd->sections[baseIdx + i]))
            ret = FALSE;
    }
    return ret;
}

_AFX afxBool AfxOpenUrdSection(afxUrd urd, afxNat secIdx, void *buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    return AfxOpenUrdSections(urd, secIdx, 1, buf ? &buf : NIL);
}

_AFX void AfxCloseUrdSections(afxUrd urd, afxNat baseIdx, afxNat secCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    afxMmu mmu = AfxGetIoContext();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    
    AfxAssertRange(urd->secCnt, baseIdx, secCnt);

    for (afxNat i = 0; i < secCnt; i++)
    {
        if (urd->sections[baseIdx + i])
        {
            if (!urd->isUserMem[baseIdx + i])
                AfxDeallocate(urd->sections[baseIdx + i]);

            urd->isUserMem[baseIdx + i] = FALSE;
            urd->marshalled[baseIdx + i] = FALSE;
            urd->sections[baseIdx + i] = NIL;
        }
    }
}

_AFX void AfxCloseUrdSection(afxUrd urd, afxNat secIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AfxCloseUrdSections(urd, secIdx, 1);
}

_AFX void AfxCloseAllUrdSections(afxUrd urd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AfxCloseUrdSections(urd, 0, urd->secCnt);
}

_AFX afxUrd AfxBuildUrd(afxUri const *path)
{
    AfxEntry("uri=%.*s", AfxPushString(AfxGetUriString(path)));
    afxError err = AFX_ERR_NONE;
    afxUrd urd = NIL;

    AfxEntry("uri:%.*s", AfxPushString(path ? AfxGetUriString(path) : &AFX_STR_EMPTY));
    afxStream file;

    if (!(file = AfxOpenFile(path, afxIoFlag_R))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHint()))
            AfxThrowError();

        AfxReleaseObjects(1, (void*) { file });
    }

    return urd;
}

_AFX afxUrd AfxAcquireUrd(afxUri const *path)
{
    AfxEntry("uri=%.*s", AfxPushString(AfxGetUriString(path)));
    afxError err = AFX_ERR_NONE;
    afxUrd urd = NIL;

    AfxEntry("uri:%.*s", AfxPushString(path ? AfxGetUriString(path) : &AFX_STR_EMPTY));
    afxStream file;

    if (!(file = AfxOpenFile(path, afxIoFlag_R))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHint()))
            AfxThrowError();

        AfxReleaseObjects(1, (void*) { file });
    }

    return urd;
}

_AFX afxError _AfxUrdDtor(afxUrd urd)
{
    AfxEntry("urd=%p", urd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    afxMmu mmu = AfxGetIoContext();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxCloseAllUrdSections(urd);

    if (urd->sections)
        AfxDeallocate(urd->sections);
    
    if (urd->conversionBuf)
        AfxDeallocate(urd->conversionBuf);

    if (urd->hdr)
        AfxDeallocate(urd->hdr);

    if (urd->srcMagicVal)
        AfxDeallocate(urd->srcMagicVal);

    return err;
}

_AFX afxError _AfxUrdCtor(void *cache, afxNat idx, afxUrd urd, void const *paradigms)
{
    afxError err = AFX_ERR_NONE;

    return err;
}

_AFX afxClassConfig _AfxUrdClsConfig =
{
    .fcc = afxFcc_URD,
    .name = "Uniform Resource Dictionary",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxUrd)),
    .mmu = NIL,
    .ctor = (void*)_AfxUrdCtor,
    .dtor = (void*)_AfxUrdDtor
};
