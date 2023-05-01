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

#ifndef AFX_PIPELINE_RIG_H
#define AFX_PIPELINE_RIG_H

/*

A afxPipelineRig defines the mapping between resources of all afxLego objects set up during command encoding in setBindGroup(), 
and the shaders of the pipeline set by GPURenderCommandsMixin.setPipeline or GPUComputePassEncoder.setPipeline.

The full binding address of a resource can be defined as a trio of:
 - shader stage mask, to which the resource is visible
 - bind group index
 - binding number

The components of this address can also be seen as the binding space of a pipeline. A afxLego (with the corresponding afxLegoSchema) covers that space for a fixed bind group index. 
The contained bindings need to be a superset of the resources used by the shader at this bind group index.

*/

#include "afx/draw/pipelining/afxSampler.h"

typedef enum afxPipelineAccessType
{
    AFX_SUPPLY_TYPE_INPUT = 0,
    AFX_SUPPLY_TYPE_OUTPUT = 1,
    AFX_SUPPLY_TYPE_SAMPLER = 2,
    AFX_SUPPLY_TYPE_COMBINED_IMAGE_SAMPLER = 3, // when an image is bound without a sampler
    AFX_SUPPLY_TYPE_SAMPLED_IMAGE = 4,
    AFX_SUPPLY_TYPE_STORAGE_IMAGE = 5,
    AFX_SUPPLY_TYPE_UNIFORM_TEXEL_BUFFER = 6,
    AFX_SUPPLY_TYPE_STORAGE_TEXEL_BUFFER = 7,
    AFX_SUPPLY_TYPE_CONSTANT_BUFFER = 8,
    AFX_SUPPLY_TYPE_STORAGE_BUFFER = 9,
    AFX_SUPPLY_TYPE_INPUT_ATTACHMENT = 10,
    AFX_SUPPLY_TYPE_PUSH_CONSTANT_BUFFER = 11,

    AFX_SUPPLY_TYPE_TOTAL
} afxPipelineAccessType;

// Access to descriptor sets from a pipeline is accomplished through a pipeline layout. 
// Zero or more descriptor set layouts and zero or more push constant ranges are combined to form a pipeline layout object describing the complete set of resources that can be accessed by a pipeline. 
// The pipeline layout represents a sequence of descriptor sets with each having a specific layout. 
// This sequence of layouts is used to determine the interface between shader stages and shader resources. 
// Each pipeline is created using a pipeline layout.

// ao invés de criar um layout para sets, por que não criar a capacidade de clonar um set? Já que o layout já é para 

AFX_DEFINE_STRUCT(afxLegoSchemaEntry) // A GPUBindGroupLayoutEntry describes a single shader resource binding to be included in a GPUBindGroupLayout.
{
    // A afxLegoPointScheme describes a single shader resource binding to be included in a afxLegoScheme.
    afxString16             name; // by QWADRO
    afxNat32                binding; // A unique identifier for a resource binding within the afxLegoScheme, corresponding to a afxLegoPointScheme.binding and a @binding attribute in the afxPipelineModule.
    afxShaderStage          visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLegoPointScheme's resource will be accessible from the associated shader stage.
    afxPipelineAccessType   type;
    afxNat                  cnt;
};

AFX_DEFINE_STRUCT(afxLegoSchema) // A GPUBindGroupLayout defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.
{
    // A afxLegoScheme defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.
    afxNat32                entryCnt;
    afxLegoSchemaEntry  entries[8]; // The map of binding indices pointing to the GPUBindGroupLayoutEntrys, which this GPUBindGroupLayout describes.
};

AFX_DEFINE_HANDLE(afxPipelineRig);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxPipelineRig) // A GPUPipelineLayout defines the mapping between resources of all GPUBindGroup objects set up during command encoding in setBindGroup(), and the shaders of the pipeline set by GPURenderCommandsMixin.setPipeline or GPUComputePassEncoder.setPipeline.
{
    afxObject           obj;
};

#endif

AFX afxResult           AfxPipelineRigFindEntries(afxPipelineRig pipr, afxNat socket, afxNat cnt, afxString const names[], afxLegoSchemaEntry entries[]);
AFX afxResult           AfxPipelineRigForEachSocket(afxPipelineRig pipr, afxNat first, afxNat cnt, afxResult(*f)(afxLegoSchema const*, void*), void *data);
AFX afxNat              AfxPipelineRigGetSocketCount(afxPipelineRig pipr);
AFX afxResult           AfxPipelineRigGetSockets(afxPipelineRig pipr, afxNat first, afxNat cnt, afxLegoSchema sockets[]);

#endif//AFX_PIPELINE_RIG_H