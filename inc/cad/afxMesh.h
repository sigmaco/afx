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

/// O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, afxMaterial's, akxMeshMorph'es e akxMeshBias's.
/// Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh é separada em akxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura akxMeshSurface é a estrutura primária que você interage para obter estas porções.
/// A estrutura akxMeshSurface referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de akxMeshBias's.
/// Os dados de vértice para uma afxMesh, dados por um akxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de akxMeshBias's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no akxMeshBias diretamente, porque você pode usar um objeto akxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura akxMeshSurface provém esta informação.

#ifndef AVX_MESH_H
#define AVX_MESH_H

#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/math/afxBox.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/math/afxVertex.h"
#include "qwadro/inc/cad/afxMeshTopology.h"
#include "qwadro/inc/cad/akxVertexData.h"
#include "qwadro/inc/base/afxFixedString.h"

AFX_DEFINE_STRUCT(akxMeshMorph) // aka morph target, blend shape
{
    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(akxMeshBias)
{
    afxBox              obb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

AFX_DEFINE_STRUCT(akxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxString32         id;
    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxNat              extraMorphCnt;
    akxMeshMorph const* extraMorphs;
    afxString const*    extraMorphTagMap;
    afxMeshTopology     topology;
    afxNat              mtlCnt;
    afxNat              biasCnt;
    afxString const*    biases;
};

AVX afxBool             AfxGetMeshUrn(afxMesh msh, afxString* id);

AVX afxNat              AfxCountMeshVertices(afxMesh msh, afxNat morphIdx, afxNat baseVtxIdx);
AVX afxBool             AfxGetMeshVertices(afxMesh msh, afxNat morphIdx, akxVertexData* data, afxNat* baseVtx, afxNat* vtxCnt);

AVX afxNat              AfxCountMeshMorphs(afxMesh msh, afxNat baseMorphIdx);
AVX akxMeshMorph*       AfxGetMeshMorphs(afxMesh msh, afxNat baseMorphIdx);

AVX afxMeshTopology     AfxGetMeshTopology(afxMesh msh);

AVX afxBool             AfxMeshIsDeformable(afxMesh msh);

AVX afxNat              AfxCountMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AVX akxMeshBias*        AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AVX afxString*          AfxGetMeshBiasTags(afxMesh msh, afxNat baseBiasIdx);

////////////////////////////////////////////////////////////////////////////////

AVX afxError            AfxAssembleMeshes(afxDrawInput din, afxNat cnt, akxMeshBlueprint const blueprints[], afxMesh meshes[]);

AVX afxNat              AfxEnumerateMeshes(afxDrawInput din, afxNat base, afxNat cnt, afxMesh msh[]);

AVX afxError            AfxBuildMeshes(afxDrawInput din, afxNat cnt, afxMeshBuilder const mshb[], afxMesh meshes[]);

AVX void                AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[]);

AVX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[]);

#endif//AVX_MESH_H
