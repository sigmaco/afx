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

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/cad/afxMaterial.h"
#include "qwadro/space/afxSkeleton.h"
#include "qwadro/io/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/cad/afxMeshTopology.h"
#include "qwadro/core/afxFixedString.h"

/// O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, afxMaterial's, afxMeshMorph'es e afxMeshBias's.
/// Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh é separada em afxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSurface é a estrutura primária que você interage para obter estas porções.
/// A estrutura afxMeshSurface referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshBias's.
/// Os dados de vértice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshBias's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshBias diretamente, porque você pode usar um objeto afxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura afxMeshSurface provém esta informação.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    afxString           id; // 16
    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(afxMeshBias)
{
    afxAabb             aabb; // originally oobb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

#ifdef _AFX_MESH_C
AFX_OBJECT(afxMesh)
{
    afxMeshTopology     topology;

    akxVertexData       vtd;
    afxNat              baseVtx;
    afxNat              vtxCnt;
    afxNat              morphCnt;
    afxMeshMorph*       morphs;
    
    afxNat              biasCnt;
    afxMeshBias*        biasData;
    afxString*          biasName;

    afxString           id; // 32
    afxStringBase       strb;
    void*               extData;

    // EXP ---
    struct
    {
        akxVertexData   vtd;
        afxNat          baseVtx;
        afxNat          vtxCnt;
    }*                  lods;
    afxNat              lodCnt;
    // --- EXP
};
#endif

AFX_DEFINE_STRUCT(afxMeshBlueprint)
/// Data needed for mesh assembly
{
    afxString32         id;
    akxVertexData       vertices;
    afxMeshTopology     topology;
    afxNat              mtlCnt;
    afxNat              biasCnt;
    afxString const*    pivots;
    afxStringBase       strb;
};

AKX afxBool             AfxGetMeshId(afxMesh msh, afxString* id);

AKX afxNat              AfxCountMeshVertices(afxMesh msh);
AKX akxVertexData       AfxGetMeshVertices(afxMesh msh);

AKX afxNat              AfxCountMeshMorphes(afxMesh msh);
AKX afxMeshMorph*       AfxGetMeshMorphes(afxMesh msh, afxNat baseMorphIdx);

AKX void                AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht);
AKX afxMeshTopology     AfxGetMeshTopology(afxMesh msh);

AKX afxBool             AfxMeshIsDeformable(afxMesh msh);

AKX afxNat              AfxCountMeshBiases(afxMesh msh);
AKX afxMeshBias*        AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx);
AKX afxBool             AfxGetMeshBiasName(afxMesh msh, afxNat biasIdx, afxString* id);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAssembleMeshes(afxSimulation sim, afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[]);

AKX afxError            AfxBuildMeshes(afxSimulation sim, afxStringBase strb, afxNat cnt, afxMeshBuilder const mshb[], afxMesh meshes[]);

AKX void                AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[]);

AKX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[]);

#endif//AFX_MESH_H
