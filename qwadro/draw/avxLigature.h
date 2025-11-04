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

  //////////////////////////////////////////////////////////////////////////////
 // QWADRO PIPELINED RESOURCING LIGATURE                                     //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_LIGATURE_H
#define AVX_LIGATURE_H

/*
    avxLigature describes descriptor sets, push constants, and shader resources.
    It's like a contract between your shaders and the pipeline, defining what external resources (textures, buffers, etc.) shaders can access.
    It is created once and passed to the pipeline.
*/

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

#include "qwadro/draw/avxCodebase.h"

#define AVX_MAX_LIGAMENT_SETS (8)
#define AVX_MAX_LIGAMENTS (80) // 80 / 8 = 10 ligaments per set
#define AVX_MAX_PUSH_RANGES (16)

AFX_DEFINE_STRUCT(avxPushRange)
// This structure defines a range within a push constant block.
{
    // Size of the push constant range.
    afxUnit16       size;
    // Offset within the push constant block.
    afxUnit16       offset;
    // Shader stages that can access it.
    afxMask         visibility;
};

AFX_DEFINE_STRUCT(avxLigament)
// This structure represents a single shader resource binding.
// This structure bridges the shader resource declaration with the internal AVX representation.
{
    // Unique binding index (matches shader @binding location).
    afxUnit32       binding;
    // A bitmask specifying the programmable stages which use it.
    afxMask         visibility;
    // The type of shader resource (e.g., uniform buffer, image, sampler.
    avxShaderParam  type;
    // The number of contiguous elements for arrays.
    afxUnit         cnt;
    // The resource name. May be used to resolve shaders and debugging.
    afxString16     name;
    union
    {
        struct
        {
            // Whether it uses dynamic offsets.
            afxBool hasDynOffsets;
            // Minimum buffer size, if applicable.
            afxUnit minSizeBound;
            // Indicates if it's a writable buffe.
            afxBool writeable;
        }           buf;
        struct
        {
            avxSampler smp;
        }           img;
    };
    // Flags for behavior.
    afxFlags        flags;
};

AFX_DEFINE_STRUCT(avxLigamentSet)
// The structure describes a descriptor set or binding group.
{
    // Descriptor set index.
    afxUnit         set;
    // Possibly usage or allocation flags.
    afxFlags        flags;
    // CRC for quick comparison/hashing.
    afxUnit32       crc32;
    // Index into 'avxLigatureConfig.points' for the first resource in this set.
    afxUnit         baseEntryIdx;
    // Number of bindings in this set.
    afxUnit         entryCnt;
    // Possibly implementation-defined data.
    void*           idd;
};

AFX_DEFINE_STRUCT(avxLigatureConfig)
// This structure represents the entire shader binding layout; the resource pipelining layout.
{
    afxFlags        flags;
    // The number of sets.
    afxUnit         setCnt;
    // An array of binding sets.
    avxLigamentSet  sets[AVX_MAX_LIGAMENT_SETS];
    // The number of resource binding poings.
    afxUnit         pointCnt;
    // An array of resource bindings.
    avxLigament     points[AVX_MAX_LIGAMENTS];
    // Number of push constant ranges.
    afxUnit         pushCnt;
    // Push constant definitions.
    avxPushRange    pushes[AVX_MAX_PUSH_RANGES];
    // The debugging string.
    afxString       tag;
    // An user-defined data.
    void*           udd;
};

/*
    The AvxConfigureLigature() function generates a ligature configuration (avxLigatureConfig) based on the provided shaders.
    This is a pipeline reflection operation which introspects each avxCodebase to gather declared resource bindings 
    (uniforms, buffers, textures, samplers, etc), push constant usage, and their visibility per shader stage.
    Then, it fills out points[] with avxLigament entries (each one representing a binding),
    sets[] with avxLigamentSet entries (groupings, like descriptor sets), and
    pushes[] with any push-constant-like uniform data.
*/

AVX afxError AvxConfigureLigature
(
    afxDrawSystem dsys, 
    avxLigatureConfig* cfg, 
    avxCodebase codb,
    afxUnit progCnt,
    afxUnit progs[]
);

/*
    The AvxAcquireLigatures() function instantiates runtime avxLigature objects from the preconfigured layouts.
    This is a pipeline layout construcion operation which uses the avxLigatureConfig as a template to:
     - Sets up uniform buffer bindings.
     - Creates internal mapping between texture units and resource bindings.
     - Allocates or references sampler objects.
     - Tracks how shaders and bindings are to be connected at runtime.
    These avxLigature objects may be stored and reused across frames or passes.
*/

AVX afxError AvxAcquireLigatures
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    avxLigatureConfig const cfg[], 
    avxLigature ligatures[]
);

////////////////////////////////////////////////////////////////////////////////

AVX afxDrawSystem   AvxGetLigatureHost
(
    avxLigature     liga
);

AVX afxFlags        AvxGetLigatureFlags
(
    avxLigature liga, 
    afxFlags mask
);

AVX afxUnit32       AvxGetLigatureHash
(
    avxLigature liga, 
    afxUnit set
);

AVX afxResult       AvxGetLigatureEntry
(
    avxLigature liga, 
    afxUnit set, 
    afxIndex first, 
    afxUnit cnt, 
    avxLigament decl[]
);

#endif//AVX_LIGATURE_H
