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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

// A avxLigature defines the interface between a set of resources bound in a afxBindSet and their accessibility in shader stages.

// A afxBindSet defines a set of resources to be bound together in a group and how the resources are used in shader stages.

// sgl_frame
// sgl_pass
// sgl_mtl
// sgl_obj

// The descriptor set number 0 will be used for engine-global resources, and bound once per frame.
// The descriptor set number 1 will be used for per-pass resources, and bound once per pass.
// The descriptor set number 2 will be used for material resources, and the number 3 will be used for per-object resources.
// This way, the inner render loops will only be binding descriptor sets 2 and 3, and performance will be high.

#ifndef AVX_LIGATURE_H
#define AVX_LIGATURE_H

#include "qwadro/inc/draw/pipe/avxShader.h"

AFX_DEFINE_STRUCT(avxLigatureEntry) // A avxLigatureEntry describes a single shader resource binding to be included in a avxLigature.
{
    // A avxLigatureEntry describes a single shader resource binding to be included in a avxLigature.
    afxNat              set;
    afxNat32            binding; // A unique identifier for a resource binding within the avxLigature, corresponding to a avxLigatureEntry.binding and a @binding attribute in the avxShader.
    afxMask             visibility; // A bitset of the members of avxShaderStage. Each set bit indicates that a avxLigatureEntry's resource will be accessible from the associated shader stage.
    avxShaderResType    type;
    afxNat              cnt;
    afxString16         name;

    union
    {
        struct
        {
            afxBool     hasDynOffsets; // if exist it is a push constant block?
            afxNat      minSizeBound;
            afxBool     writeable; // if true it is no uniform buffer?
        }               buf;
        struct
        {
            int a;
        }               img;
    };
};

AFX_DEFINE_STRUCT(avxLigatureConfig)
{
    afxNat          shaderCnt;
    avxShader*      shaders;
};

AVX afxNat32        AfxGetLigatureHash(avxLigature liga, afxNat set);

AVX afxResult       AfxGetLigatureEntry(avxLigature liga, afxNat set, afxIndex first, afxNat cnt, avxLigatureEntry decl[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxError        AfxBuildLigatures(afxDrawContext dctx, afxNat cnt, avxLigatureConfig const cfg[], avxLigature ligatures[]);

#endif//AVX_LIGATURE_H
