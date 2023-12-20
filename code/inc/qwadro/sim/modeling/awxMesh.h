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

#include "qwadro/sim/awxSimDefs.h"
#include "qwadro/core/afxArray.h"
#include "qwadro/math/afxAabb.h"
#include "qwadro/core/afxInstance.h"
#include "qwadro/sim/awxMaterial.h"
#include "qwadro/sim/awxSkeleton.h"
#include "qwadro/core/afxUrd.h"
#include "qwadro/math/afxVertex.h"
#include "qwadro/sim/modeling/awxMeshTopology.h"

/// O objeto awxMesh é a estrutura primária para dado geométrico no Qwadro.
/// Este referencia dados de vértice, dados de triângulo, awxMaterial's, awxMeshMorph'es e awxMeshVertebra's.
/// Assets padrões do Qwadro contêm um arranjo de awxMesh na estrutura de informação do arquivo, e awxMesh'es são também referenciadas pelos objetos awxModel que as usam.
/// Cada awxMesh é separada em awxMeshSurface's delitmitadas por awxMaterial's.
/// A estrutura awxMeshSurface é a estrutura primária que você interage para obter estas porções.
/// A estrutura awxMeshSurface referencia o arranjo de índices para a awxMesh, a qual em sua vez referencia o arranjo de vértice.
/// Se a awxMesh é deformável, as vértices no arranjo de vértice referenciam as articulações no arranjo de awxMeshVertebra's.
/// Os dados de vértice para uma awxMesh, dados por um afxVertexBuffer referenciado pelo awxMesh, contêm todos os vértices únicos na awxMesh.

/// As vértices no Qwadro são rígidos ou deformáveis.
/// awxMesh'es deformáveis são aquelas que estão ligadas a múltiplas articulações, onde awxMesh'es rígidas são aquelas que estão ligadas a uma singela articulação (e portanto movem-se "rigidamente" com aquela articulação).

/// Para determinar quais articulações uma awxMesh está ligada a (uma para awxMesh rígida, muitas para awxMesh deformável), você pode acessar o arranjo de awxMeshVertebra's. 
/// Este arranjo contém nomes das articulações as quais a awxMesh está ligada, bem como parâmetros de "oriented bounding box" para as partes da awxMesh que estão ligadas àquela articulação e outra informação pertinente a ligação malha-a-articulação.
/// Note que na maioria dos casos você não necessitará de usar os nomes das articulações no awxMeshVertebra diretamente, porque você pode usar um objeto awxMeshRig para fazer este trabalho (e outro trabalho necessário de ligação) para você.

/// Os dados de índice para uma awxMesh, dado por um awxMeshTopology referenciado pelo awxMesh, contém todos os índices para os triângulos na awxMesh.
/// Estes índices sempre descrevem uma lista de triângulo - isso é, cada grupo de três índices descrevem um singelo triângulo - os dados não são organizados em "strips" ou "fans".

/// Uma vez que você tem vértices e índices, você basicamente tem todos os dados da awxMesh que você necessita para quais porções de índice vão com quais awxMaterial's.
/// A estrutura awxMeshSurface provém esta informação.

AFX_DEFINE_STRUCT(awxMeshMorph) // aka morph target, blend shape
{
    _AFX_DBG_FCC;
    afxString           id; // 16
    awxVertexData       vtd;
    afxNat              baseVtxIdx;
    afxNat              vtxCnt;
    afxBool             delta;
};

AFX_DEFINE_STRUCT(awxMeshVertebra)
{
    _AFX_DBG_FCC;
    afxFixedString32         id;
    afxAabb             aabb; // originally oobb;
    afxNat              triCnt;
    afxNat*             tris; // indices to vertices
};

#ifdef _AFX_MESH_C
AFX_OBJECT(awxMesh)
{
    afxBufferedString   id; // 128

    awxVertexData       vtd;

    afxNat              morphCnt;
    awxMeshMorph*       morphs;

    awxMeshTopology     topology;
    
    afxNat              vertebraCnt;
    awxMeshVertebra*    vertebras;
    void*               extData;
};
#endif

AFX afxString const*    AwxGetMeshId(awxMesh msh);
AFX awxVertexData       AwxGetMeshVertices(awxMesh msh);
AFX afxNat              AwxCountMeshVertices(awxMesh msh);

AFX afxNat              AwxCountMeshMorphes(awxMesh msh);
AFX awxMeshMorph*       AwxGetMeshMorph(awxMesh msh, afxNat morphIdx);

AFX void                AwxRelinkMeshTopology(awxMesh msh, awxMeshTopology msht);
AFX afxBool             AwxGetMeshTopology(awxMesh msh, awxMeshTopology* msht);

AFX afxBool             AwxMeshIsDeformable(awxMesh msh);
AFX afxNat              AwxCountMeshVertebras(awxMesh msh);
AFX awxMeshVertebra*    AwxGetMeshVertebra(awxMesh msh, afxNat artIdx);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX awxMesh             AwxAssembleMesh(awxSimulation sim, awxVertexData vtd, afxNat baseVtxIdx, afxNat vtxCnt, awxMeshTopology msht, afxNat baseSurfIdx, afxNat surfCnt, afxString const pivots[]);

AFX afxError            AwxBuildMeshes(awxSimulation sim, afxNat cnt, awxMeshBuilder const mshb[], awxMesh msh[]);

AFX void                AwxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxMesh msh[]);

AFX void                AwxRenormalizeMeshes(afxNat cnt, awxMesh msh[]);

#endif//AFX_MESH_H
