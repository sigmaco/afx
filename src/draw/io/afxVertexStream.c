/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_VERTEX_BUFFER_C
#define _AVX_INDEX_BUFFER_C
#define _AVX_DRAW_INPUT_C
#include "../impl/avxImplementation.h"
#include "qwadro/inc/math/afxReal16.h"

AFX_DEFINE_STRUCT(freeVBlistEntry)
{
    freeVBlistEntry*        next;
    freeVBlistEntry*        prev;
    afxBuffer               vbo;
    afxUnit32               offset;
    afxUnit32               size;
};

AFX_DEFINE_STRUCT(createdVBlistEntry)
{
    createdVBlistEntry*     next;
    afxBuffer               vbo;
};

AFX_DEFINE_STRUCT(StrideEntry)
{
    StrideEntry*            next;
    afxUnit32               stride;
    freeVBlistEntry*        freelist;
    createdVBlistEntry*     vblist;
};

AFX_DEFINE_STRUCT(DynamicVertexBuffer)
{
    DynamicVertexBuffer*    next;
    afxBuffer               vbo;
    afxUnit32               size;
    afxUnit32               used;
    afxBuffer*              variableAddress;
};

#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER       4
#define MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE (256*1024)

AFX_OBJECT(afxBufferizer)
{
    afxDrawInput        din;
    afxUnit32           DefaultVBSize;

    StrideEntry*        StrideList;

    afxSlabAllocator    StrideFreeList;
    afxSlabAllocator    FreeVBFreeList;
    afxSlabAllocator    CreatedVBFreeList;

    DynamicVertexBuffer*DynamicVertexBufferList;
    afxSlabAllocator    DynamicVertexBufferFreeList;

    afxUnit32           CurrentDynamicVertexBufferManager;
    afxUnit32           OffSetDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    afxUnit32           SizeDynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];
    afxBuffer           DynamicVertexBufferManager[MAX_DYNAMIC_VERTEX_BUFFER_MANAGER];

};

#define GET_ADDR(base_, stride_, idx_) (void*)((afxByte*)base_ + stride_ * idx_)
#define GET_CONST_ADDR(base_, stride_, idx_) (void const*)((afxByte const*)base_ + stride_ * idx_)

#if 0
_AVX afxError AfxConvert2
(
    afxUnit cnt,
    void const* src,
    afxVertexFormat srcFmt,
    afxUnit srcStride,
    void* dst,
    afxVertexFormat dstFmt,
    afxUnit dstStride,
    afxBool normalized
)
{
    afxError err = AFX_ERR_NONE;

    afxUnit srcCompCnt = 0;
    afxUnit dstCompCnt = 0;

    switch (srcFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        srcCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        srcCompCnt = 2; break;
    case afxVertexFormat_R32x3:
        //case afxVertexFormat_U32x3:
        //case afxVertexFormat_I32x3:
        srcCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        srcCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        dstCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        dstCompCnt = 2; break;
    case afxVertexFormat_R32x3:
        //case afxVertexFormat_U32x3:
        //case afxVertexFormat_I32x3:
        dstCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        dstCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R32x3:
    case afxVertexFormat_R32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxUnpackReal16(src4[j]);
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_R16:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_R16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16(src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_U32:
    case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
    case afxVertexFormat_U32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat32
        case afxVertexFormat_R32x2: // real32 to nat32
        case afxVertexFormat_R32x3: // real32 to nat32
        case afxVertexFormat_R32x4: // real32 to nat32
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U16:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_U16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 65535.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U8:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_U8x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 255.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
            //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
            //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    default: AfxThrowError();  break;
    }

    return err;
}
#endif

afxBool CreateVertexBuffer(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, afxBuffer* vbo, afxUnit32* offset)
{
    // based on RwD3D9CreateVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo);
    AFX_ASSERT(offset);
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList) // Search for an available stride
    {
        if (stride == strideList->stride)
            break;

        strideList = strideList->next;
    }

    if (!strideList)
    {
        // create one if there isn't any
        strideList = AfxPushSlabUnit(&vbMgr->StrideFreeList);
        AFX_ASSERT(strideList);
        strideList->stride = stride;
        strideList->freelist = NULL;
        strideList->vblist = NULL;

        // Add it to the begining
        strideList->next = vbMgr->StrideList;
        vbMgr->StrideList = strideList;
    }

    freeVBlistEntry* freelist = strideList->freelist;

    while (freelist) // Search for an available free space
    {
        if (freelist->size >= size)
            break;

        freelist = freelist->next;
    }

    if (!freelist)
    {
        // If there is not free space, create a new VB
        freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
        AFX_ASSERT(freelist);
        freelist->offset = 0;
        freelist->size = (((vbMgr->DefaultVBSize + (stride - 1)) / stride) * stride);

        if (size >= freelist->size)
            freelist->size = size;
        
        afxDrawSystem dsys = AfxGetDrawInputContext(vbMgr->din);

        afxBuffer vbo;
        afxBufferInfo spec = { 0 };
        spec.cap = freelist->size;
        spec.flags = afxBufferFlag_W;
        spec.usage = afxBufferUsage_VERTEX;

        if (AfxAcquireBuffers(dsys, 1, &spec, &vbo))
        {
            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);            
            return FALSE;
        }
        
        freelist->vbo = vbo;
        freelist->next = strideList->freelist; // Add it to the begining
        freelist->prev = NULL;

        if (strideList->freelist)
            strideList->freelist->prev = freelist;

        strideList->freelist = freelist;

        // Add an entry to the vb list
        createdVBlistEntry* vblist = AfxPushSlabUnit(&vbMgr->CreatedVBFreeList);
        vblist->vbo = vbo;
        vblist->next = strideList->vblist;
        strideList->vblist = vblist;

#if defined(RWDEBUG)
        VBCreated++;
#endif
    }

    *vbo = freelist->vbo;
    *offset = freelist->offset;
    afxUnit32 nextOffset = (*offset) + size;

    AFX_ASSERT(freelist->size >= (nextOffset - freelist->offset));
    freelist->size -= nextOffset - freelist->offset;
    freelist->offset = nextOffset;

    if (freelist->size <= 0)
    {
        if (freelist->prev)
            freelist->prev->next = freelist->next;

        if (freelist->next)
            freelist->next->prev = freelist->prev;

        if (strideList->freelist == freelist)
            strideList->freelist = freelist->next;

        AfxPopSlabUnit(&vbMgr->FreeVBFreeList, freelist);
    }

#if defined(RWDEBUG)
    BlocksCreated++;
    BytesReserved += size;
#endif

    return TRUE;
}

void DestroyVertexBuffer(afxBufferizer vbMgr, afxUnit32 stride, afxUnit32 size, afxBuffer vbo, afxUnit32 offset)
{
    // based on RwD3D9DestroyVertexBuffer

    afxError err;
    AFX_ASSERT(stride <= size);
    AFX_ASSERT((stride & 0x3) == 0);
    AFX_ASSERT(vbo != NULL);

    // Search for the stride
    StrideEntry* strideList = vbMgr->StrideList;

    while (strideList )
    {
        StrideEntry* next = strideList->next;

        if (stride == strideList->stride)
            break;

        strideList = next;
    }

    if (strideList)
    {
        // Search for an available free space to add
        freeVBlistEntry* freelist = strideList->freelist;

        while (freelist)
        {
            freeVBlistEntry* nextFreelist = freelist->next;

            if (freelist->vbo == vbo)
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == offset) break;
                else
                {
                    // Could be added to the begin of the free space?

                    if (offset + size == freelist->offset)
                        break;
                }
            }
            freelist = nextFreelist;
        }

        if (freelist)
        {
            afxBool canCompactMore = FALSE;
            afxUnit32 newoffset = offset;
            afxUnit32 newSize = size;
            freeVBlistEntry* oldFreelist = freelist;

            do
            {
                // Could be added to the end of the free space?

                if ((freelist->offset + freelist->size) == newoffset) freelist->size += newSize;
                else
                {
                    // Could be added to the begin of the free space?

                    if (newoffset + newSize == freelist->offset)
                    {
                        freelist->offset = newoffset;
                        freelist->size += newSize;
                    }
                }

                // Could be compact more?
                newoffset = freelist->offset;
                newSize = freelist->size;
                oldFreelist = freelist;
                canCompactMore = FALSE;
                freelist = strideList->freelist;
                
                while (freelist)
                {
                    freeVBlistEntry* nextFreelist = freelist->next;

                    if (freelist->vbo == vbo)
                    {
                        // Could be added to the end of the free space?

                        if ((freelist->offset + freelist->size) == newoffset)
                        {
                            canCompactMore = TRUE;
                            break;
                        }
                        else
                        {
                            // Could be added to the begin of the free space?

                            if (newoffset + newSize == freelist->offset)
                            {
                                canCompactMore = TRUE;
                                break;
                            }
                        }
                    }
                    freelist = nextFreelist;
                }

                // Remove old list

                if (canCompactMore)
                {
                    if (oldFreelist->prev)
                        oldFreelist->prev->next = oldFreelist->next;

                    if (oldFreelist->next)
                        oldFreelist->next->prev = oldFreelist->prev;

                    if (strideList->freelist == oldFreelist)
                        strideList->freelist = oldFreelist->next;

                    AfxPopSlabUnit(&vbMgr->FreeVBFreeList, oldFreelist);
                }
            } while (canCompactMore);
        }
        else
        {
            freelist = AfxPushSlabUnit(&vbMgr->FreeVBFreeList);
            freelist->offset = offset;
            freelist->size = size;
            freelist->vbo = vbo;

            // Add it to the begining
            freelist->next = strideList->freelist;
            freelist->prev = NULL;

            if (strideList->freelist)
                strideList->freelist->prev = freelist;

            strideList->freelist = freelist;
        }

#if defined(RWDEBUG)
        BlocksCreated--;
        BytesReserved -= size;
#endif
    }
}

afxBool DynamicVertexBufferCreate(afxBufferizer vbMgr, afxUnit size, afxBuffer *vertexBuffer)
{
    afxError err = NIL;
    DynamicVertexBuffer *freeVertexBuffer = NULL;
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (!currentVertexBuffer->used)
        {
            if (currentVertexBuffer->size == size)
            {
                break;
            }
            else if (freeVertexBuffer == NULL || freeVertexBuffer->size < currentVertexBuffer->size)
            {
                freeVertexBuffer = currentVertexBuffer;
            }
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = TRUE;

        *vertexBuffer = currentVertexBuffer->vbo;

        currentVertexBuffer->variableAddress = vertexBuffer;
    }
    else
    {
        /* Destroy a free vertex buffer to make room for the new one */
        if (freeVertexBuffer)
        {
            AfxDisposeObjects(1, &freeVertexBuffer->vbo);
            freeVertexBuffer->vbo = NULL;

            currentVertexBuffer = freeVertexBuffer;

#if defined(RWDEBUG)
            NumDynamicVertexBuffer--;
#endif /* defined(RWDEBUG) */
        }
        else
        {
            /* Create a new item */
            currentVertexBuffer = AfxPushSlabUnit(&vbMgr->DynamicVertexBufferFreeList);

            /* Add it to the beginnig of the list */
            currentVertexBuffer->next = vbMgr->DynamicVertexBufferList;

            vbMgr->DynamicVertexBufferList = currentVertexBuffer;
        }

        afxDrawSystem dsys = AfxGetDrawInputContext(vbMgr->din);

        afxBufferInfo spec = { 0 };
        spec.cap = size;
        spec.usage = afxBufferUsage_VERTEX;
        spec.flags = afxBufferFlag_W;
        
        if (AfxAcquireBuffers(dsys, 1, &spec, vertexBuffer))
        {
            AfxThrowError();
            return FALSE;
        }

        /* Fill item info */
        currentVertexBuffer->size = size;
        currentVertexBuffer->used = TRUE;
        currentVertexBuffer->vbo = (*vertexBuffer);
        currentVertexBuffer->variableAddress = (vertexBuffer);

#if defined(RWDEBUG)
        NumDynamicVertexBuffer++;
#endif /* defined(RWDEBUG) */
    }

    return TRUE;
}

void DynamicVertexBufferDestroy(afxBufferizer vbMgr, void *vertexBuffer)
{
    DynamicVertexBuffer *currentVertexBuffer = vbMgr->DynamicVertexBufferList;

    while (currentVertexBuffer)
    {
        DynamicVertexBuffer *next = currentVertexBuffer->next;

        if (currentVertexBuffer->vbo == (vertexBuffer))
        {
            break;
        }

        currentVertexBuffer = next;
    }

    if (currentVertexBuffer)
    {
        currentVertexBuffer->used = FALSE;
        currentVertexBuffer->variableAddress = NULL;
    }
}

afxBool _VertexBufferManagerOpen(afxBufferizer vbMgr)
{
    afxError err = NIL;
    //avxVertexBufferizer* vbMgr = AfxAllocate(1, sizeof(vbMgr[0]), 0, AfxHere());
    //*vbMgrPtr = vbMgr;

    vbMgr->DefaultVBSize = 128 * 1024;
    AfxDeploySlabAllocator(&vbMgr->StrideFreeList, sizeof(StrideEntry), 16);
    AfxDeploySlabAllocator(&vbMgr->FreeVBFreeList, sizeof(freeVBlistEntry), 100);
    AfxDeploySlabAllocator(&vbMgr->CreatedVBFreeList, sizeof(createdVBlistEntry), 100);

    AfxDeploySlabAllocator(&vbMgr->DynamicVertexBufferFreeList, sizeof(DynamicVertexBuffer), 4);

    afxBool created;
    afxUnit32 n;

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE;

        created = DynamicVertexBufferCreate(vbMgr, MAX_DYNAMIC_VERTEX_BUFFER_MANAGER_SIZE, &(vbMgr->DynamicVertexBufferManager[n]));

        if (!created)
        {
            AfxThrowError();
        }
    }

    return TRUE;
}

void _VertexBufferManagerClose(afxBufferizer vbMgr)
{
    //avxVertexBufferizer* vbMgr = *vbMgrPtr;
#if defined(RWDEBUG)
    AFX_ASSERT(BlocksCreated == 0);
    AFX_ASSERT(BytesReserved == 0);
#endif /* defined(RWDEBUG) */

    // D3D9DynamicVertexBufferManagerDestroy();

    vbMgr->CurrentDynamicVertexBufferManager = 0;

    for (afxUnit n = 0; n < MAX_DYNAMIC_VERTEX_BUFFER_MANAGER; n++)
    {
        vbMgr->OffSetDynamicVertexBufferManager[n] = 0;

        vbMgr->SizeDynamicVertexBufferManager[n] = 0;

        if (vbMgr->DynamicVertexBufferManager[n])
        {
            DynamicVertexBufferDestroy(vbMgr, vbMgr->DynamicVertexBufferManager[n]);

            vbMgr->DynamicVertexBufferManager[n] = NULL;
        }
    }

    // D3D9DynamicVertexBufferListDestroy();

    while (vbMgr->DynamicVertexBufferList)
    {
        DynamicVertexBuffer *next = vbMgr->DynamicVertexBufferList->next;

        if (vbMgr->DynamicVertexBufferList->vbo)
        {
            AfxDisposeObjects(1, &vbMgr->DynamicVertexBufferList->vbo);

            vbMgr->DynamicVertexBufferList->vbo = NULL;

            if (vbMgr->DynamicVertexBufferList->variableAddress)
            {
                *vbMgr->DynamicVertexBufferList->variableAddress = NULL;
            }
        }

        AfxPopSlabUnit(&vbMgr->DynamicVertexBufferFreeList, vbMgr->DynamicVertexBufferList);

        vbMgr->DynamicVertexBufferList = next;
    }

    AfxDismantleSlabAllocator(&vbMgr->DynamicVertexBufferFreeList);

#if defined(RWDEBUG)
    NumDynamicVertexBuffer = 0;
#endif /* defined(RWDEBUG) */

    // ---

    while (vbMgr->StrideList != NULL)
    {
        StrideEntry  *next = vbMgr->StrideList->next;

        /*  Destroy the free list */
        while (vbMgr->StrideList->freelist != NULL)
        {
            freeVBlistEntry   *nextFreelist = vbMgr->StrideList->freelist->next;

            AfxPopSlabUnit(&vbMgr->FreeVBFreeList, vbMgr->StrideList->freelist);

            vbMgr->StrideList->freelist = nextFreelist;
        }

        /*  Destroy the VB list */
        while (vbMgr->StrideList->vblist != NULL)
        {
            createdVBlistEntry   *nextvblist = vbMgr->StrideList->vblist->next;

            if (vbMgr->StrideList->vblist->vbo != NULL)
            {
                AfxDisposeObjects(1, &(vbMgr->StrideList->vblist->vbo));
            }

            AfxPopSlabUnit(&vbMgr->CreatedVBFreeList, vbMgr->StrideList->vblist);

            vbMgr->StrideList->vblist = nextvblist;
        }

        AfxPopSlabUnit(&vbMgr->StrideFreeList, vbMgr->StrideList);

        vbMgr->StrideList = next;
    }

    AfxDismantleSlabAllocator(&vbMgr->CreatedVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->FreeVBFreeList);
    AfxDismantleSlabAllocator(&vbMgr->StrideFreeList);

#if defined(RWDEBUG)
    BlocksCreated = 0;
    VBCreated = 0;
    BytesReserved = 0;
#endif /* defined(RWDEBUG) */
}

_AVX afxError _AvxBufzDtor(afxBufferizer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VBUF, 1, &vbuf);
    
    _VertexBufferManagerClose(vbuf);
    
    return err;
}

_AVX afxError _AvxBufzCtor(afxBufferizer vbuf, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_VBUF, 1, &vbuf);

    afxDrawInput din = args[0];
    afxBufferizerInfo const* info = ((afxBufferizerInfo const *)args[1]) + invokeNo;

    _VertexBufferManagerOpen(vbuf);

    return err;
}

_AVX afxClassConfig const _AVX_VBUF_CLASS_CONFIG =
{
    .fcc = afxFcc_VBUF,
    .name = "Bufferizer",
    .desc = "Bufferizer",
    .fixedSiz = sizeof(AFX_OBJECT(afxBufferizer)),
    .ctor = (void*)_AvxBufzCtor,
    .dtor = (void*)_AvxBufzDtor
};

_AVX afxError AfxAcquireBufferizer(afxDrawInput din, afxBufferizerInfo const* info, afxBufferizer bufferizer[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(bufferizer);
    AFX_ASSERT(info);

    afxClass* cls = AvxGetVertexBufferClass(din);
    AFX_ASSERT_CLASS(cls, afxFcc_VBUF);

    if (AfxAcquireObjects(cls, 1, (afxObject*)bufferizer, (void const*[]) { din, info }))
        AfxThrowError();

    return err;
}
