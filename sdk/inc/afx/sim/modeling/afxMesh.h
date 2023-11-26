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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MESH_H
#define AFX_MESH_H

#include "afx/sim/afxSimDefs.h"
#include "afx/core/afxArray.h"
#include "afx/math/afxAabb.h"
#include "afx/core/afxInstance.h"
#include "afx/sim/afxMaterial.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/core/afxUrd.h"
#include "afx/math/afxVertex.h"
#include "afx/sim/modeling/afxMeshTopology.h"

/// O objeto afxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, afxMaterial's, afxMeshMorph'es e afxMeshVertebra's.
/// Assets padrões do Qwadro contêm um arranjo de afxMesh na estrutura de informação do arquivo, e afxMesh'es são também referenciadas pelos objetos afxModel que as usam.
/// Cada afxMesh é separada em afxMeshSurface's delitmitadas por afxMaterial's.
/// A estrutura afxMeshSurface é a estrutura primária que você interage para obter estas porções.
/// A estrutura afxMeshSurface referencia o arranjo de índices para a afxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a afxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de afxMeshVertebra's.
/// Os dados de vértice para uma afxMesh, dados por um afxVertexBuffer referenciado pelo afxMesh, contêm todos os vértices únicos na afxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// afxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde afxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma afxMesh está ligada a (uma para afxMesh rígida, muitas para afxMesh deformável), você pode acessar o arranjo de afxMeshVertebra's. 
/// Este arranjo contém nomes das articulações as quais a afxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da afxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no afxMeshVertebra diretamente, porque você pode usar um objeto afxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma afxMesh, dado por um afxMeshTopology referenciado pelo afxMesh, contém todos os índices para os triângulos na afxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da afxMesh que você necessita para quais porções de índice vão com quais afxMaterial's.
/// A estrutura afxMeshSurface provém esta informação.

AFX_DEFINE_STRUCT(afxMeshMorph) // aka morph target, blend shape
{
    _AFX_DBG_FCC;
    afxString           id; // 16
    afxVertexData       vtd;
    afxNat              baseVtxIdx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(afxMeshVertebra)
{
    _AFX_DBG_FCC;
    afxString16         id; // 16
    afxAabb             aabb; // originally oobb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

AFX_DEFINE_STRUCT(afxMaterialSlot)
{
    afxMaterial         mtl;
};

#ifdef _AFX_MESH_C
AFX_OBJECT(afxMesh)
{
    afxString           id; // 128

    afxVertexData       vtd;

    afxNat              morphCnt;
    afxMeshMorph*       morphs;

    afxMeshTopology     topology;
    
    afxNat              mtlSlotCnt;
    afxMaterialSlot*    mtlSlots;

    afxNat              vertebraCnt;
    afxMeshVertebra*    vertebras;
    void*               extData;
};
#endif

AFX afxString const*    AfxGetMeshId(afxMesh msh);
AFX afxVertexData       AfxGetMeshVertices(afxMesh msh);
AFX afxNat              AfxCountMeshVertices(afxMesh msh);

AFX afxNat              AfxCountMeshMorphes(afxMesh msh);
AFX afxMeshMorph*       AfxGetMeshMorph(afxMesh msh, afxNat morphIdx);

AFX void                AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht);
AFX afxBool             AfxGetMeshTopology(afxMesh msh, afxMeshTopology* msht);

AFX afxNat              AfxCountMeshMaterials(afxMesh msh);
AFX void                AfxSetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl);
AFX afxBool             AfxGetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial* mtl);
AFX afxBool             AfxFindMeshMaterial(afxMesh msh, afxUri const *id, afxMaterial* mtl);

AFX afxBool             AfxMeshIsDeformable(afxMesh msh);
AFX afxNat              AfxCountMeshVertebras(afxMesh msh);
AFX afxMeshVertebra*    AfxGetMeshVertebra(afxMesh msh, afxNat artIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxMesh             AfxAssembleMesh(afxSimulation sim, afxVertexData vtd, afxNat baseVtxIdx, afxNat vtxCnt, afxMeshTopology msht, afxNat baseSurfIdx, afxNat surfCnt, afxString const pivots[]);

AFX afxError            AfxBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBuilder const mshb[], afxMesh msh[]);

AFX void                AfxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxMesh msh[]);

AFX void                AfxRenormalizeMeshes(afxNat cnt, afxMesh msh[]);

#endif//AFX_MESH_H
