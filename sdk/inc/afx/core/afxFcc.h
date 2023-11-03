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

#ifndef afxFcc_H
#define afxFcc_H

#include "afx/core/afxCoreDefs.h"

#define AfxPackFcc(ch1, ch2, ch3, ch4) (enum afxFcc)(((((((ch4)<<8)|(ch3))<<8)|(ch2))<<8)|(ch1))
#define AfxUnpackFcc(idn_, cA, cB, cC, cD) (cA=(afxChar)(idn_>>0),cB=(afxChar)(idn_>>8),cC =(afxChar)(idn_>>16), cD = (afxChar)(idn_>>24))

typedef enum afxFcc
{
    afxFcc_NIL,
    //NIL,

    // base type has one or two letter chars.
    // complex types has three letter chars.
    // subcomplementary types (of complex types) usuallys has four letter chars.

    afxFcc_B       = AfxPackFcc('b', NIL, NIL, NIL), // afxBool
    afxFcc_B8      = AfxPackFcc('b', '8', NIL, NIL), // afxBool8
    afxFcc_B32     = AfxPackFcc('b', '3', '2', NIL), // afxBool32
    afxFcc_N       = AfxPackFcc('n', NIL, NIL, NIL), // afxNat
    afxFcc_N8      = AfxPackFcc('n', '8', NIL, NIL), // afxNat8
    afxFcc_N16     = AfxPackFcc('n', '1', '6', NIL), // afxNat16
    afxFcc_N32     = AfxPackFcc('n', '3', '2', NIL), // afxNat32
    afxFcc_N64     = AfxPackFcc('n', '6', '4', NIL), // afxNat64
    afxFcc_I       = AfxPackFcc('i', NIL, NIL, NIL), // afxInt
    afxFcc_I8      = AfxPackFcc('i', '8', NIL, NIL), // afxInt8
    afxFcc_I16     = AfxPackFcc('i', '1', '6', NIL), // afxInt16
    afxFcc_I32     = AfxPackFcc('i', '3', '2', NIL), // afxInt32
    afxFcc_I64     = AfxPackFcc('i', '6', '4', NIL), // afxInt64
    afxFcc_R       = AfxPackFcc('r', NIL, NIL, NIL), // afxReal
    afxFcc_R16     = AfxPackFcc('r', '1', '6', NIL), // afxReal16
    afxFcc_R32     = AfxPackFcc('r', '3', '2', NIL), // afxReal32

    // *** e2coree ***

    afxFcc_OBJ     = AfxPackFcc('o', 'b', 'j', '\0'), // afxInstance
    afxFcc_OBJC    = AfxPackFcc('o', 'b', 'j', 'c'), // afxInstanceConnection
    afxFcc_SLOT    = AfxPackFcc('s', 'l', 'o', 't'), // afxSlot
    afxFcc_HNDL    = AfxPackFcc('h', 'n', 'd', 'l'), // 
    afxFcc_CLS    = AfxPackFcc('o', 'b', 'j', 't'), // afxClass
    afxFcc_OBJK    = AfxPackFcc('o', 'b', 'j', 'k'), // afxObjack

    afxFcc_DBG     = AfxPackFcc('d', 'b', 'g', '\0'), // afxDebugger
    afxFcc_APP     = AfxPackFcc('a', 'p', 'p', '\0'), // afxApplication
    afxFcc_SYS     = AfxPackFcc('s', 'y', 's', '\0'), // afxSystem

    //afxFcc_BUF     = AfxPackFcc('b', 'u', 'f'), // afxBuffer
    afxFcc_ARR     = AfxPackFcc('a', 'r', 'r', '\0'), // afxArray
    afxFcc_QUE     = AfxPackFcc('q', 'u', 'e', '\0'), // afxQueue
    afxFcc_STR     = (('q'<<8)|'s'),//AfxPackFcc('s', 't', 'r', '\0'), // afxString
    afxFcc_STRB    = (('q' << 8) | 'z'),//AfxPackFcc('s', 't', 'r', '\0'), // afxString
#if 0
    afxFcc_STR0    = AfxPackFcc('s', 't', 'r', '0'), // afxString8
    afxFcc_STR1    = AfxPackFcc('s', 't', 'r', '1'), // afxString16
    afxFcc_STR2    = AfxPackFcc('s', 't', 'r', '2'), // afxString32
    afxFcc_STR3    = AfxPackFcc('s', 't', 'r', '3'), // afxString64
    afxFcc_STR4    = AfxPackFcc('s', 't', 'r', '4'), // afxString128
    afxFcc_STR5    = AfxPackFcc('s', 't', 'r', '5'), // afxString256
    afxFcc_STR6    = AfxPackFcc('s', 't', 'r', '6'), // afxString512
    afxFcc_STR7    = AfxPackFcc('s', 't', 'r', '7'), // afxString1024
    afxFcc_STR8    = AfxPackFcc('s', 't', 'r', '8'), // afxString2048
    afxFcc_STR9    = AfxPackFcc('s', 't', 'r', '9'), // afxString4096
    afxFcc_STRC    = AfxPackFcc('s', 't', 'r', 'c'), // afxString
    afxFcc_STRE    = AfxPackFcc('s', 't', 'r', 'e'), // afxString excerpt; It is just a view of another string; canv't modify data.
    afxFcc_STRF    = AfxPackFcc('s', 't', 'r', 'f'), // afxString
#endif
    afxFcc_URI     = AfxPackFcc('u', 'r', 'i', '\0'), // afxUri
    afxFcc_SRC     = AfxPackFcc('s', 'r', 'c', '\0'), // afxSource
    afxFcc_SIG     = AfxPackFcc('s', 'i', 'g', '\0'), // afxSignal
    afxFcc_SIGH    = AfxPackFcc('s', 'i', 'g', 'h'), // afxSignalHandler
    afxFcc_SIGC    = AfxPackFcc('s', 'i', 'g', 'c'), // afxSignalConnection
    afxFcc_MEM     = AfxPackFcc('m', 'e', 'm', '\0'), // afxMemory
    afxFcc_CTX     = AfxPackFcc('c', 't', 'x', '\0'), // afxContext
    afxFcc_MPAG    = AfxPackFcc('m', 'p', 'a', 'g'), // afxMemPage
    //afxFcc_CHNK     = AfxPackFcc('m', 'e', 'm', '\0'), // afxContext
    afxFcc_ALLS    = AfxPackFcc('a', 'l', 'l', 's'), // afxSlabMemory
    afxFcc_SLAB    = AfxPackFcc('s', 'l', 'a', 'b'), // afxSlabHeader
    afxFcc_ALLR    = AfxPackFcc('a', 'l', 'l', 'r'), // afxRegionMemory
    afxFcc_AREN    = AfxPackFcc('a', 'r', 'e', 'n'), // afxArena
    afxFcc_CHNK    = AfxPackFcc('c', 'h', 'n', 'k'),
    afxFcc_BLCK    = AfxPackFcc('b', 'l', 'c', 'k'),
    afxFcc_BLOB    = AfxPackFcc('b', 'l', 'o', 'b'), // afxBlob
    afxFcc_IOS     = AfxPackFcc('i', 'o', 's', '\0'), // afxStream
    afxFcc_FSSP    = AfxPackFcc('f', 's', 's', 'p'), // afxStoragePoint
    afxFcc_ARC     = AfxPackFcc('a', 'r', 'c', '\0'), // afxArchive
    afxFcc_RES     = AfxPackFcc('r', 'e', 's', '\0'), // afxResource
    afxFcc_XML     = AfxPackFcc('x', 'm', 'l', '\0'), // afxXml
    afxFcc_URD     = AfxPackFcc('u', 'r', 'd', '\0'), // afxUrd
    afxFcc_STD     = AfxPackFcc('s', 't', 'd', '\0'), // for standard implementations
    afxFcc_SLCK    = AfxPackFcc('l', 'c', 'k', '\0'), // afxSlock
    afxFcc_BMP     = AfxPackFcc('b', 'm', 'p', '\0'), // afxBitmap
    afxFcc_MAP     = AfxPackFcc('m', 'a', 'p', '\0'), // afxMap
    afxFcc_DICT    = AfxPackFcc('d', 'i', 'c', 't'), // afxDictionary
    afxFcc_POOL    = AfxPackFcc('p', 'o', 'o', 'l'), // afxPool

    afxFcc_EMEC    = AfxPackFcc('e', 's', 'y', 's'), // afxEventMechanism
    afxFcc_ETRG    = AfxPackFcc('e', 't', 'r', 'g'), // afxEventTrigger
    afxFcc_EHND    = AfxPackFcc('e', 'h', 'n', 'd'), // afxEventHandler

    afxFcc_FSYS    = AfxPackFcc('f', 's', 'y', 's'), // afxIoSystem
    afxFcc_FSMP    = AfxPackFcc('f', 's', 'm', 'p'), // afxIoSystemStoragePoint
    afxFcc_FILE    = AfxPackFcc('f', 'i', 'l', 'e'), // afxFile
    
    afxFcc_THR     = AfxPackFcc('t', 'h', 'r', '\0'), // afxThread
    afxFcc_MTX     = AfxPackFcc('m', 't', 'x', '\0'), // afxMutex

    afxFcc_EVNT    = AfxPackFcc('e', 'v', 'n', 't'), // afxEvent
    afxFcc_EVLP    = AfxPackFcc('e', 'v', 'l', 'p'), // afxEventLoop

    afxFcc_HID     = AfxPackFcc('h', 'i', 'd', '\0'), // afxHid
    afxFcc_KBD     = AfxPackFcc('k', 'b', 'd', '\0'), // afxKeyboard
    afxFcc_MSE     = AfxPackFcc('m', 's', 'e', '\0'), // afxMouse
    afxFcc_DSO     = AfxPackFcc('d', 's', 'o', '\0'), // afxDso
    afxFcc_AGTR    = AfxPackFcc('a', 'g', 't', 'r'), // afxAggregator
    afxFcc_AGGR    = AfxPackFcc('a', 'g', 'g', 'r'), // afxAggregation

    afxFcc_ASI     = AfxPackFcc('a', 's', 'i', '\0'), // afxService
    afxFcc_MDLE    = AfxPackFcc('m', 'd', 'l', 'e'), // afxModule
    afxFcc_AUTR    = AfxPackFcc('a', 'u', 't', 'r'), // afxAutomator

    // *** e2math ***

    afxFcc_V2D     = AfxPackFcc('v', '2', 'd', '\0'), // afxV2d
    afxFcc_V3D     = AfxPackFcc('v', '3', 'd', '\0'), // afxV3d
    afxFcc_V4D     = AfxPackFcc('v', '4', 'd', '\0'), // afxV4d
    afxFcc_QUAT    = AfxPackFcc('q', '\0', '\0', '\0'), // afxQuat
    afxFcc_V       = AfxPackFcc('v', '\0', '\0', '\0'), // afxVersor
    afxFcc_M2D     = AfxPackFcc('m', '2', 'd', '\0'), // afxM2d
    afxFcc_M3D     = AfxPackFcc('m', '3', 'd', '\0'), // afxM3d
    afxFcc_M4D     = AfxPackFcc('m', '4', 'd', '\0'), // afxV4d
    afxFcc_SPC     = AfxPackFcc('s', 'p', 'c', '\0'), // afxSpace

    // *** e2draw ***

    afxFcc_DSYS    = AfxPackFcc('d', 's', 'y', 's'), // afxDrawSystem
    afxFcc_DDEV    = AfxPackFcc('d', 'd', 'e', 'v'), // afxDrawDevice
    afxFcc_DDRV    = AfxPackFcc('d', 'd', 'r', 'v'), // afxDrawDevice
    afxFcc_DIN     = AfxPackFcc('d', 'i', 'n', '\0'), // afxDrawInput
    afxFcc_DOUT    = AfxPackFcc('d', 'o', 'u', 't'), // afxDrawOutput
    afxFcc_DQUE    = AfxPackFcc('d', 'q', 'u', 'e'), // afxDrawQueue
    afxFcc_DTHR    = AfxPackFcc('d', 't', 'h', 'r'), // afxDrawThread
    afxFcc_CANV    = AfxPackFcc('c', 'a', 'n', 'v'), // afxCanvas
    afxFcc_DCTX    = AfxPackFcc('d', 'c', 't', 'x'), // afxDrawContext
    afxFcc_PIP     = AfxPackFcc('p', 'i', 'p', '\0'), // afxPipeline
    afxFcc_PIPR    = AfxPackFcc('p', 'i', 'p', 'a'), // afxPipelineRig
    afxFcc_LEGT2    = AfxPackFcc('l', 'e', 'g', 't'), // afxPipelineRig
    afxFcc_LEGO    = AfxPackFcc('l', 'e', 'g', 'o'), // afxPipelineRig
    afxFcc_SHD     = AfxPackFcc('s', 'h', 'd', '\0'), // afxShader
    afxFcc_SHDB    = AfxPackFcc('s', 'h', 'd', 'b'), // afxShaderBlueprint
    afxFcc_PIPB    = AfxPackFcc('p', 'i', 'p', 'b'), // afxPipelineBlueprint
    afxFcc_DRES    = AfxPackFcc('d', 'r', 'e', 's'), // afxDrawResourceSet
    afxFcc_DSCR    = AfxPackFcc('d', 's', 'c', 'r'), // afxDrawScript
    afxFcc_DDBG    = AfxPackFcc('d', 'd', 'b', 'g'), // afxDrawDebugger
    afxFcc_DOP     = AfxPackFcc('d', 'o', 'p', '\0'), // afxDrawOperation
    afxFcc_DOPB    = AfxPackFcc('d', 'o', 'p', 'b'), // afxDrawOperationBlueprint
    afxFcc_DTEC    = AfxPackFcc('d', 't', 'e', 'c'), // afxDrawTechnique
    afxFcc_DPAS    = AfxPackFcc('d', 'p', 'a', 's'), // afxDrawPass
    afxFcc_VPNT    = AfxPackFcc('v', 'p', 'n', 't'), // afxViewpoint
    afxFcc_WIMP    = AfxPackFcc('o', 'v', 'l', 'y'), // afxWimp
    afxFcc_WID     = AfxPackFcc('w', 'i', 'd', '\0'), // afxWidget
    afxFcc_WND     = AfxPackFcc('w', 'n', 'd', '\0'), // afxWindow
    afxFcc_WPP     = AfxPackFcc('w', 'p', 'p', '\0'), // afxWallpaper

    afxFcc_DUNI    = AfxPackFcc('d', 'u', 'n', 'i'), // afxDrawableUniverse
    afxFcc_DMDL    = AfxPackFcc('d', 'm', 'd', 'l'), // sglDrawableModel
    afxFcc_DMSH    = AfxPackFcc('d', 'm', 's', 'h'), // sglDrawableMesh
    afxFcc_DMSE    = AfxPackFcc('d', 'm', 's', 'e'), // sglDrawableMeshSection
    afxFcc_DMTL    = AfxPackFcc('d', 'm', 't', 'l'), // sglDrawableMaterial
    afxFcc_DSKY    = AfxPackFcc('d', 's', 'k', 'y'), // sglDrawableSky
    afxFcc_DTER    = AfxPackFcc('d', 't', 'e', 'r'), // sglDrawableTerrain
    afxFcc_DTEX    = AfxPackFcc('d', 't', 'e', 'x'), // sglDrawableSky

    // *** e2sound ***

    afxFcc_SSYS    = AfxPackFcc('s', 's', 'y', 's'), // afxSoundSystem
    afxFcc_SDEV    = AfxPackFcc('s', 'o', 'u', 't'), // afxSoundIoDevice
    afxFcc_SOUT    = AfxPackFcc('s', 'o', 'u', 't'), // afxSoundOutput
    afxFcc_SIN     = AfxPackFcc('s', 'i', 'n', '\0'), // afxSoundInput
    afxFcc_SCTX    = AfxPackFcc('s', 'c', 't', 'x'), // afxSoundContext

    // *** e2social ***

    afxFcc_SOCS    = AfxPackFcc('s', 'o', 'c', 's'), // afxSocialSystem
    afxFcc_SOCP    = AfxPackFcc('s', 'o', 'c', 'p'), // afxSocialPresence

    // *** e2simul ***

    afxFcc_SIM     = AfxPackFcc('s', 'i', 'm', '\0'), // afxSimulation
    afxFcc_HUB     = AfxPackFcc('h', 'u', 'b', '\0'), // afxHub
    afxFcc_DAG     = AfxPackFcc('d', 'a', 'g', '\0'), // afxDag
    afxFcc_NOD     = AfxPackFcc('n', 'o', 'd', '\0'), // afxNode
    afxFcc_NODA    = AfxPackFcc('n', 'o', 'd', 'a'), // afxNodeAnnex (node annex)
    afxFcc_ASIO    = AfxPackFcc('a', 's', 'i', 'o'), // afxAssetIoAssistent
    afxFcc_PVT     = AfxPackFcc('p', 'v', 't', '\0'), // afxNode

    afxFcc_IMG     = AfxPackFcc('i', 'm', 'g', '\0'), // afxImage
    afxFcc_SAMP     = AfxPackFcc('s', 'm', 'p', '\0'), // afxSampler
    afxFcc_BUF     = AfxPackFcc('b', 'u', 'f', '\0'), // afxBuffer
    afxFcc_VBUB    = AfxPackFcc('v', 'b', 'u', 'b'), // afxVertexBufferBlueprint
    afxFcc_VBUF    = AfxPackFcc('v', 'b', 'u', 'f'), // afxVertexBuffer
    afxFcc_IBUF    = AfxPackFcc('i', 'b', 'u', 'f'), // afxIndexBuffer
    afxFcc_IMGH    = AfxPackFcc('i', 'm', 'g', 'h'), // AfxImageHub
    afxFcc_IMIO    = AfxPackFcc('i', 'm', 'i', 'o'), // AfxImageIoAssistent
    afxFcc_TEX     = AfxPackFcc('t', 'e', 'x', '\0'), // afxTexture
    afxFcc_SURF    = AfxPackFcc('s', 'u', 'r', 'f'), // afxSurface
    afxFcc_TEXB    = AfxPackFcc('t', 'e', 'x', 'b'), // afxTextureBlueprint

    afxFcc_MDL     = AfxPackFcc('m', 'd', 'l', '\0'), // afxModel
    afxFcc_MDLB    = AfxPackFcc('m', 'd', 'l', 'b'), // afxModelBlueprint
    afxFcc_SKL     = AfxPackFcc('s', 'k', 'l', '\0'), // afxSkeleton
    afxFcc_SKLB    = AfxPackFcc('s', 'k', 'l', 'b'), // afxSkeletonBlueprint
    afxFcc_BONB    = AfxPackFcc('b', 'o', 'n', 'b'), // afxBoneBlueprint
    afxFcc_MSHV    = AfxPackFcc('m', 's', 'h', 'v'), // afxMeshVertebra
    afxFcc_MSHC    = AfxPackFcc('m', 's', 'h', 'c'), // afxMeshConnection
    afxFcc_MSH     = AfxPackFcc('m', 's', 'h', '\0'), // afxMesh
    afxFcc_MSHB    = AfxPackFcc('m', 's', 'h', 'b'), // afxMeshBlueprint
    afxFcc_VTD      = AfxPackFcc('v', 't', 'd', '\0'), // afxVertexData
    afxFcc_VTDS     = AfxPackFcc('v', 't', 'd', 's'), // afxVertexDataStream
    afxFcc_MSHT    = AfxPackFcc('m', 's', 'h', 't'), // afxMeshTopology
    afxFcc_MSHS    = AfxPackFcc('m', 's', 'h', 's'), // afxMeshSurface
    afxFcc_MSHM    = AfxPackFcc('m', 's', 'h', 'm'), // afxMeshMorph
    afxFcc_MSHR    = AfxPackFcc('m', 's', 'h', 'r'), // afxMeshRig
    afxFcc_MDLM    = AfxPackFcc('m', 'd', 'l', 'm'), // AfxModelingMechanism
    afxFcc_MDIO    = AfxPackFcc('m', 'd', 'i', 'o'), // afxModelIoAssistent
    afxFcc_MTL     = AfxPackFcc('m', 't', 'l', '\0'), // afxMaterial
    afxFcc_MTLB    = AfxPackFcc('m', 't', 'l', 'b'), // afxMaterialBlueprint

    afxFcc_CAD     = AfxPackFcc('c', 'a', 'd', '\0'), // afxAsset

    afxFcc_UNI     = AfxPackFcc('u', 'n', 'i', '\0'), // afxUniverse
    afxFcc_CLST    = AfxPackFcc('c', 'l', 's', 't'), // afxCluster
    afxFcc_ENT     = AfxPackFcc('e', 'n', 't', '\0'), // afxEntity
    afxFcc_ASP     = AfxPackFcc('a', 's', 'p', '\0'), // afxAspect
    afxFcc_LIT     = AfxPackFcc('l', 'i', 't', '\0'), // afxLight
    afxFcc_CAM     = AfxPackFcc('c', 'a', 'm', '\0'), // afxCamera
    afxFcc_SKY     = AfxPackFcc('s', 'k', 'y', '\0'), // afxSky
    afxFcc_TERR    = AfxPackFcc('t', 'e', 'r', 'r'), // afxTerrain
    afxFcc_BRN     = AfxPackFcc('b', 'r', 'n', '\0'), // afxBrain
    afxFcc_BOD     = AfxPackFcc('b', 'o', 'd', '\0'), // afxBody
    afxFcc_BODC     = AfxPackFcc('p', 'u', 'p', '\0'), // afxAnimator
    afxFcc_BODP    = AfxPackFcc('b', 'o', 'd', 'p'), // afxBodyPart

    afxFcc_RND     = AfxPackFcc('r', 'n', 'd', '\0'), // afxRenderer

    afxFcc_MAX_    = 0x7FFFFFFF
} afxFcc;

#endif//afxFcc_H
