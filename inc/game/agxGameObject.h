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

#ifndef AFX_GAME_OBJECT_H
#define AFX_GAME_OBJECT_H

#include "agxGameDefs.h"
#include "qwadro/inc/afxQwadro.h"

typedef enum agxMovementType
{
    agxMovementType_LAND,
    agxMovementType_WATER,
    agxMovementType_AIR
} agxMovementType;

typedef enum agxFormationCategory
{
    agxFormationCategory_PROTECTED
} agxFormationCategory;

typedef enum agxImpactType
{
    agxImpactType_WOOD,
    agxImpactType_STONE
} agxImpactType;

typedef enum agxPhysicsInfo
{
    agxPhysicsInfo_DUDE
} agxPhysicsInfo;

typedef enum agxUnitAiType
{
    agxUnitAiType_RangedCombative,

} agxUnitAiType;

typedef enum agxResourceType
{
    agxResourceType_FOOD,
    agxResourceType_WOOD,
    agxResourceType_GOLD
} agxResourceType;

typedef enum agxAttackType
{
    agxAttackType_HAND,
} agxAttackType;

typedef enum agxUnitType
{
    agxUnitType_LogicalTypeHealed,
    agxUnitType_LogicalTypeValidSharpshoot,
    agxUnitType_LogicalTypeNeededForVictory,
    agxUnitType_LogicalTypeHandUnitsAutoAttack,
    agxUnitType_LogicalTypeLandMilitary,
    agxUnitType_LogicalTypeScout,
    agxUnitType_LogicalTypeValidSPCUnitsDeadCondition,
    agxUnitType_LogicalTypeGarrisonInShips,
    agxUnitType_LogicalTypeRangedUnitsAutoAttack,
    agxUnitType_LogicalTypeVillagersAttack,
    agxUnitType_LogicalTypeHandUnitsAttack,
    agxUnitType_LogicalTypeRangedUnitsAttack,
    agxUnitType_LogicalTypeMinimapFilterMilitary,
    agxUnitType_ConvertsHerds,
    agxUnitType_AbstractHandSiege,
    agxUnitType_CountsTowardMilitaryScore,
    agxUnitType_AbstractSiegeTrooper,
    agxUnitType_Ranged,
    agxUnitType_HasBountyValue,
    agxUnitType_AbstractInfantry,
    agxUnitType_Unit,
    agxUnitType_Military,
    agxUnitType_UnitClass,
    agxUnitType_,
    agxUnitType_,
    agxUnitType_,
    agxUnitType_,
} agxUnitType;

typedef enum agxFlag
{
    agxFlag_CollidesWithProjectiles,
    agxFlag_ApplyHandicapTraining,
    agxFlag_CorpseDecays,
    agxFlag_ShowGarrisonButton,
    agxFlag_DontRotateObstruction,
    agxFlag_ObscuredByUnits,
    agxFlag_RotateInPlace,
    agxFlag_Tracked,
    agxFlag_,

} agxFlag;

AFX_DEFINE_STRUCT(agxEntityDecl)
{
    afxUri                  icon;
    afxUri                  portrait;
    afxUri                  animFile;
    afxUnit                  displayTxtId;
    afxUnit                  editorTxtId;
    afxUnit                  rolloverTxtId;
    afxUnit                  shortRolloverTxtId;
    agxMovementType         movType; // movement type
    afxReal                 obstrucRadiusX; // obstruction radius X
    afxReal                 obstrucRadiusZ; // obstruction radius Z
    agxImpactType           impactType;
    agxPhysicsInfo          physInfo;
    afxReal                 initialHp;
    afxReal                 maxHp;
    afxReal                 los; // line of sight
    agxUnitAiType           unitAiType;
    afxReal                 trainPoints;
    afxReal                 bounty;
    afxReal                 buildBounty;
    struct
    {
        agxResourceType     resType;
        afxReal             value;
    }*                      costs;
    afxUnit                  costCnt;
    afxUnit                  allowedAge;
    struct
    {
        agxAttackType       attackType;
        afxReal             value;
    }*                      armors;
    afxUnit                  armorCnt;

    // movible entities
    afxReal                 turnRate;
    afxUnit                  popCnt; // population count
    agxFormationCategory    formation; // formation category
    afxReal                 maxVelocity;
    afxReal                 maxRunVelocity;

    afxUri                  tactics;

    struct
    {
        afxUnit              page;
        afxUnit              column;
    }*                      cmds;
    afxUnit                  cmdCnt;

};

#endif//AFX_GAME_OBJECT_H
