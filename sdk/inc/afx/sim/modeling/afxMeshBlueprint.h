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

#ifndef AFX_MESH_BLUEPRINT_H
#define AFX_MESH_BLUEPRINT_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/mem/afxArray.h"
#include "../../math/volume/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afxMaterial.h"
#include "afx/sim/modeling/afxSkeleton.h"
#include "afx/core/mem/afxStack.h"
#include "afx/sim/modeling/afxVertex.h"

/// A serialization wrapper for the relationship between a Vertex buffer and a set of primitives.

AFX_DEFINE_STRUCT(afxMeshBlueprintArticulation)
{
    afxString*              boneName; // 32
    afxNat                  triCnt;
    void const              *tris;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintSection) // aka tri material group
{
    afxString*              name; // 32
    afxNat                  mtlIdx;
    afxNat                  firstTriIdx;
    afxNat                  triCnt;

    void const              *idxSrc;
    afxNat                  srcIdxSiz;
};

AFX_DEFINE_STRUCT(afxMeshBlueprintVertexArrange)
{
    afxString*              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;

    void const              *src;
    afxVertexFormat         srcFmt;
};

/// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de uma malha. Dependendo apenas das referências das fontes de vértices e índices, para evitar alocações desnecessárias.

AFX_DEFINE_STRUCT(afxMeshBlueprint)
{
    afxFcc                  fcc;
    void*                   sim;
    afxUri128               uri;

    // vertex data
    afxNat                  vtxCnt; // must be greater than zero.
    afxArray                arranges; // afxMeshBlueprintVertexArrange

    // topology data
    afxNat                  idxCnt;
    void const              *idxSrc;
    afxNat                  srcIdxSiz;
    afxPrimTopology         prim;
    afxArray                sections; // afxMeshBlueprintSection

    // rigging data
    afxArray                articulations; // afxMeshBlueprintArticulation

    // material data
    afxArray                materials; // afxUri
};

struct buffered_vertex_prefix
{
    afxV3d normal;
    afxV3d tan;
    afxV3d binormal;
    afxV3d tanBinormalCross;
};

struct vertex_weight_arrays
{
    afxReal boneWeights[256];
    afxNat boneIndices[256]; // a matrix da articulação que exerce tal influência
};

AFX_DEFINE_STRUCT(afxMeshBlueprint2)
{
    afxReal tbnTol[3]; // tangent, binormal, normal
    afxReal tanBinormalCrossTol;
    afxReal tanMergingMinCos;
    afxReal chanTol[256];
    /*const data_type_definition* */afxVertexLayout VertexType;
    afxNat vtxSiz;
    afxNat chanCnt;
    afxNat maxVtxCnt;
    afxNat vtxCnt;
    afxStack vtxStack;
    afxStack nextCoincidentVtxStack;
    afxStack fromTriStack;
    afxReal point[3];
    afxNat boneCnt;
    afxReal *boneWeights;
    afxNat weightCnt;
    struct vertex_weight_arrays weightArrays;
    afxStack vtxIdxStack;
    afxStack nextInMtlStack;
    afxNat mtlCnt;
    afxNat usedMtlCnt;
    afxNat *firstMtlTri;
    afxStack edgeStack;
    afxNat *edgeHashHeads;
    afxNat nextUnusedTri;
    afxNat mtlIdx;
    afxNat vtxIdx[3];
    /*data_type_definition* */afxVertexLayout *bufferedVertexType;
    /*data_type_definition*/void *channelTypes;
    afxNat bufferedPrefixSiz;
    afxNat bufferedChanSiz;
    afxNat bufferedVtxSiz;
    struct buffered_vertex_prefix *bufferedVtx[3];
    struct buffered_vertex_prefix *comparisonVtx;
    /*void * */afxVertexLayout truncatedVtx;
    afxStack triTanStack;
    afxStack vtxTanStack;
    afxNat vtxAttrNameCnt;
    afxString const **vtxAttrNames;

    afxNat pwiSiz;
    afxNat pwiOffset[3];
    afxVertexFormat pwiFmt[3];
    afxNat tbnSiz;
    afxNat tbnOffset[3];
    afxVertexFormat tbnFmt[3];
    afxNat ttcSiz;
    afxNat ttcOffset[3];
    afxVertexFormat ttcFmt[3];

    struct
    {
        
    } verMd5;

    void *sim;
    void *cachedMem;
    _AFX_DBG_FCC;
};

// Inicializa uma blueprint, e reserva recursos.
AFX void                    AfxMeshBlueprintDeploy(afxMeshBlueprint *mshb, void *sim, afxUri const *name, afxNat vtxCnt, afxNat estVtxArrCnt, afxNat estSecCnt, afxNat estArtCnt, afxNat estMtlCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX void                    AfxMeshBlueprintDiscard(afxMeshBlueprint *mshb);

AFX void                    AfxMeshBlueprintErase(afxMeshBlueprint *mshb);

// Buscar por um nomeado material se já inserido.
AFX afxBool                 AfxMeshBlueprintFindMaterial(afxMeshBlueprint *mshb, afxUri const *uri, afxNat *idx);

// Adiciona numa nova articulação a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddArticulation(afxMeshBlueprint *mshb, afxString const *name, afxNat triCnt, void const *tris);

// Adiciona um novo material a malha.
AFX afxError                AfxMeshBlueprintAddMaterial(afxMeshBlueprint *mshb, afxMaterial mtl);

// Adiciona numa nova seção a malha. Ao menos uma deve existir.
AFX afxError                AfxMeshBlueprintAddSection(afxMeshBlueprint *mshb, afxString const *name, afxNat mtlIdx, afxNat firstTriIdx, afxNat triCnt, void const *idxSrc, afxNat idxSrcSiz);

// Define dados de topologia da malha. Se um 'ibuf' não for oferecido, um novo será gerado com a capacidade de 'idxCnt'. Noutro caso, 'baseIdx' deve ser usado para delimitar o início do espaço utilizável num 'ibuf' compartilhado.
AFX void                    AfxMeshBlueprintSetTopology(afxMeshBlueprint *mshb, afxPrimTopology prim, afxNat idxCnt, void const *idxSrc, afxNat idxSrcSiz);

// Adiciona um novo arranjo de vértice a malha. Ao menos uma, especificando posição, é necessária.
AFX afxError                AfxMeshBlueprintAddVertexArrange(afxMeshBlueprint *mshb, afxVertexRowSpecification const *spec);

#endif//AFX_MESH_BLUEPRINT_H