/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_LIGATURE_DDK_H
#define AVX_LIGATURE_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddLiga);
AFX_DECLARE_STRUCT(_avxDdiLiga);

#ifdef _AVX_LIGATURE_C

AFX_DEFINE_STRUCT(_avxPushRange)
{
    afxUnit16       size;
    afxUnit16       offset;
    afxMask         visibility;
};

AFX_DEFINE_STRUCT(_avxLigament)
{
    afxUnit32       binding; // A unique identifier for a resource binding within the avxLigature, corresponding to a avxLigatureEntry.binding and a @binding attribute in the avxCodebase.
    //afxUnit         cachedSet;
    afxMask         visibility; // A bitset of the members of avxShaderType. Each set bit indicates that a avxLigatureEntry's resource will be accessible from the associated shader stage.
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
    void*           idd;
};

AFX_DEFINE_STRUCT(_avxLigamentSet)
{
    afxUnit         set;
    afxUnit         baseEntry;
    afxUnit         entryCnt;
    afxFlags        flags;
    afxUnit32       crc32;
    void*           idd;
};

#ifdef _AVX_BIND_SCHEMA_IMPL
AFX_OBJECT(_avxLigature)
#else
AFX_OBJECT(avxLigature)
#endif
{
    _avxDdiLiga const*  ddi;
    _avxIddLiga*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxFlags            flags;
    afxUnit             pushCnt;
    _avxPushRange*      pushes;
    afxUnit             totalEntryCnt;
    _avxLigament*       totalEntries; // The map of binding indices pointing to the avxLigatureEntry, which this avxLigature describes.
    afxUnit             setCnt;
    _avxLigamentSet*    sets;
};

#endif//_AVX_LIGATURE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_LIGA;

#endif//AVX_LIGATURE_DDK_H
