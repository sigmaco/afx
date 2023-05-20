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

The components of this address can also be seen as the binding space of a pipeline. A afxLego (with the corresponding afxLegoTemplate) covers that space for a fixed bind group index. 
The contained bindings need to be a superset of the resources used by the shader at this bind group index.
*/

#include "afx/draw/pipelining/afxSampler.h"

typedef enum afxShaderResourceType
{
    AFX_SHD_RES_TYPE_INPUT = 0,
    AFX_SHD_RES_TYPE_OUTPUT = 1,
    AFX_SHD_RES_TYPE_SAMPLER = 2,
    AFX_SHD_RES_TYPE_COMBINED_IMAGE_SAMPLER = 3, // when an image is bound without a sampler
    AFX_SHD_RES_TYPE_SAMPLED_IMAGE = 4,
    AFX_SHD_RES_TYPE_STORAGE_IMAGE = 5,
    AFX_SHD_RES_TYPE_UNIFORM_TEXEL_BUFFER = 6,
    AFX_SHD_RES_TYPE_STORAGE_TEXEL_BUFFER = 7,
    AFX_SHD_RES_TYPE_CONSTANT_BUFFER = 8,
    AFX_SHD_RES_TYPE_STORAGE_BUFFER = 9,
    AFX_SHD_RES_TYPE_INPUT_ATTACHMENT = 10,
    AFX_SHD_RES_TYPE_PUSH_CONSTANT_BUFFER = 11,

    AFX_SHD_RES_TYPE_TOTAL
} afxShaderResourceType;

// Access to descriptor sets from a pipeline is accomplished through a pipeline layout. 
// Zero or more descriptor set layouts and zero or more push constant ranges are combined to form a pipeline layout object describing the complete set of resources that can be accessed by a pipeline. 
// The pipeline layout represents a sequence of descriptor sets with each having a specific layout. 
// This sequence of layouts is used to determine the interface between shader stages and shader resources. 
// Each pipeline is created using a pipeline layout.

// ao invés de criar um layout para sets, por que não criar a capacidade de clonar um set? Já que o layout já é para 

AFX_DEFINE_STRUCT(afxLegoBindingDecl)
{
    afxNat                  binding; // A unique identifier for a resource binding within the afxLegoScheme, corresponding to a afxLegoDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLegoDataScheme's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString const*        name;
};

AFX_DEFINE_HANDLE(afxLegoTemplate);
AFX_DEFINE_HANDLE(afxPipelineRig);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxLegoTemplate)
{
    afxObject           obj;
};

AFX_OBJECT(afxPipelineRig)
{
    afxObject           obj;
};

#endif

AFX afxResult           AfxPipelineRigEnumerateTemplates(afxPipelineRig pipr, afxNat first, afxNat cnt, afxLegoTemplate sockets[]);

#endif//AFX_PIPELINE_RIG_H