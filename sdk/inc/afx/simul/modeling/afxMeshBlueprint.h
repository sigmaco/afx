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

#ifndef AFX_MESH_BLUEPRINT_H
#define AFX_MESH_BLUEPRINT_H

#include "afxMesh.h"

AFX_DEFINE_STRUCT(afxMeshArticulationBlueprint)
{
    afxString32                     boneName;
    afxNat                          triCnt;
    void const                      *tris;
};

AFX_DEFINE_STRUCT(afxMeshSectionBlueprint) // aka tri material group
{
    afxString32                     name;
    afxNat                          mtlIdx;
    afxNat                          baseTriIdx;
    afxNat                          triCnt;

    void const                      *idxSrc;
    afxNat                          srcIdxSiz;
};

AFX_DEFINE_STRUCT(afxMeshVertexArrangeBlueprint)
{
    afxString8                      semantic;
    afxVertexFormat                 fmt;
    afxVertexUsage                  usage;

    void const                      *src;
    afxVertexFormat                 srcFmt;
};

// Um blueprint foi idealizado para ser um meta-objeto quase-completo para geração de uma malha. Dependendo apenas das referências das fontes de vértices e índices, para evitar alocações desnecessárias.

AFX_DEFINE_STRUCT(afxMeshBlueprint)
{
    afxFcc                          fcc;
    afxString32                     name;

    // vertex data
    afxNat                          vtxCnt; // must be greater than zero.
    afxNat                          arrCnt;
    afxMeshVertexArrangeBlueprint   *arranges; // array of vertex arrange specifications.

    // topology data
    afxNat                          idxCnt;
    void const                      *idxSrc;
    afxNat                          srcIdxSiz;
    afxPrimTopology                 prim;
    afxNat                          secCnt;
    afxMeshSectionBlueprint         *sections;

    // rigging data
    afxNat                          artCnt;
    afxMeshArticulationBlueprint    *articulations;

    // material data
    afxNat                          mtlCnt;
    afxUri128                       *materials;
};

// Inicializa uma blueprint, e reserva recursos.
AFX afxError                        AfxMeshBlueprintDeploy(afxMeshBlueprint *mshb, afxString const *name, afxNat vtxCnt, afxNat vtxArrCnt, afxNat idxCnt, afxNat secCnt, afxNat artCnt, afxNat mtlCnt);

// Retira de uso uma blueprint, e libera recursos.
AFX afxError                        AfxMeshBlueprintDrop(afxMeshBlueprint *mshb);

// Buscar por um nomeado material se já inserido.
AFX afxNat                          AfxMeshBlueprintFindMaterial(afxMeshBlueprint *mshb, afxUri const *uri);

// Adiciona numa nova articulação a malha. Ao menos uma deve existir.
AFX afxError                        AfxMeshBlueprintDefineArticulation(afxMeshBlueprint *mshb, afxNat artIdx, afxString const *name, afxNat triCnt, void const *tris);

// Adiciona um novo material a malha.
AFX afxError                        AfxMeshBlueprintDefineMaterial(afxMeshBlueprint *mshb, afxNat mtlIdx, afxUri const *uri);

// Adiciona numa nova seção a malha. Ao menos uma deve existir.
AFX afxError                        AfxMeshBlueprintDefineSection(afxMeshBlueprint *mshb, afxNat secIdx, afxString const *name, afxNat mtlIdx, afxNat baseTriIdx, afxNat triCnt, void const *idxSrc, afxNat idxSrcSiz);

// Define dados de topologia da malha. Se um 'ibuf' não for oferecido, um novo será gerado com a capacidade de 'idxCnt'. Noutro caso, 'baseIdx' deve ser usado para delimitar o início do espaço utilizável num 'ibuf' compartilhado.
AFX afxError                        AfxMeshBlueprintDefineTopology(afxMeshBlueprint *mshb, afxPrimTopology prim, afxNat idxCnt, void const *idxSrc, afxNat idxSrcSiz);

// Adiciona um novo arranjo de vértice a malha. Ao menos uma, especificando posição, é necessária.
AFX afxError                        AfxMeshBlueprintDefineVertexArrange(afxMeshBlueprint *mshb, afxNat rowIdx, afxVertexRowSpecification const *spec);

// Tenta completar uma blueprint com os dados já fornecidos, se suficentes para serem usados para gerar um novo afxMesh.
AFX afxError                        AfxMeshBlueprintValidate(afxMeshBlueprint *mshb);

#endif//AFX_MESH_BLUEPRINT_H