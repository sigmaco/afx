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

#ifndef AVX_LIGATURE_H
#define AVX_LIGATURE_H

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO PIPELINED RESOURCING LIGATURE                                     //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/**
    A avxLigature defines the interface between a set of resources bound in a afxBindSet and their accessibility in shader stages.

    A afxBindSet defines a set of resources to be bound together in a group and how the resources are used in shader stages.

    sgl_frame
    sgl_pass
    sgl_mtl
    sgl_obj

    The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
    The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
    The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
    This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.
*/

#include "qwadro/inc/draw/op/avxShader.h"

#define AVX_MAX_LIGATURE_SETS (8)
#define AVX_MAX_LIGAMENTS (80) // 80 / 8 = 10 ligaments per set
#define AVX_MAX_PUSH_RANGES (16)

AFX_DEFINE_STRUCT(avxPushRange)
{
    afxUnit16       size;
    afxUnit16       offset;
    afxMask         visibility;
};

AFX_DEFINE_STRUCT(avxLigament)
// A avxLigament describes a single shader resource binding to be included in a avxLigature.
{
    afxUnit32       binding; // A unique identifier for a resource binding within the avxLigature, corresponding to a avxLigament.binding and a @binding attribute in the avxShader.
    afxMask         visibility; // A bitset of the members of avxShaderType. Each set bit indicates that a avxLigament's resource will be accessible from the associated shader stage.
    avxShaderParam  type;
    afxUnit         cnt;
    afxString16     name;
    union
    {
        struct
        {
            afxBool hasDynOffsets; // if exist it is a push constant block?
            afxUnit minSizeBound;
            afxBool writeable; // if true it is no uniform buffer?
        }           buf;
        struct
        {
            int a;
        }           img;
    };
    afxFlags        flags;
};

AFX_DEFINE_STRUCT(avxLigatureSet)
{
    afxUnit         set;
    afxFlags        flags;
    afxUnit32       crc32;
    afxUnit         baseEntryIdx;
    afxUnit         entryCnt;
    void*           idd;
};

AFX_DEFINE_STRUCT(avxLigatureConfig)
{
    afxFlags        flags;
    afxUnit         pushCnt;
    avxPushRange    pushes[AVX_MAX_PUSH_RANGES];
    afxUnit         pointCnt;
    avxLigament     points[AVX_MAX_LIGAMENTS];
    afxUnit         setCnt;
    avxLigatureSet  sets[AVX_MAX_LIGATURE_SETS];
    afxString       tag;
    void*           udd;
};

AVX afxUnit32       AvxGetLigatureHash(avxLigature liga, afxUnit set);

AVX afxResult       AvxGetLigatureEntry(avxLigature liga, afxUnit set, afxIndex first, afxUnit cnt, avxLigament decl[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AvxAcquireLigatures(afxDrawSystem dsys, afxUnit cnt, avxLigatureConfig const cfg[], avxLigature ligatures[]);

AVX afxError        AvxConfigureLigature(afxDrawSystem dsys, afxUnit shaderCnt, avxShader shaders[], avxLigatureConfig* cfg);

#endif//AVX_LIGATURE_H
