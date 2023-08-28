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

#ifndef AFX_FCC_H
#define AFX_FCC_H

#include "afx/core/afxCoreDefs.h"

#define AfxPackFcc(ch1, ch2, ch3, ch4) (enum afxFcc)(((((((ch4)<<8)|(ch3))<<8)|(ch2))<<8)|(ch1))
#define AfxUnpackFcc(idn_, cA, cB, cC, cD) (cA=(afxChar)(idn_>>0),cB=(afxChar)(idn_>>8),cC =(afxChar)(idn_>>16), cD = (afxChar)(idn_>>24))

typedef enum afxFcc
{
    AFX_FCC_NIL,
    //NIL,

    // base type has one or two letter chars.
    // complex types has three letter chars.
    // subcomplementary types (of complex types) usuallys has four letter chars.

    AFX_FCC_B       = AfxPackFcc('b', NIL, NIL, NIL), // afxBool
    AFX_FCC_B8      = AfxPackFcc('b', '8', NIL, NIL), // afxBool8
    AFX_FCC_B32     = AfxPackFcc('b', '3', '2', NIL), // afxBool32
    AFX_FCC_N       = AfxPackFcc('n', NIL, NIL, NIL), // afxNat
    AFX_FCC_N8      = AfxPackFcc('n', '8', NIL, NIL), // afxNat8
    AFX_FCC_N16     = AfxPackFcc('n', '1', '6', NIL), // afxNat16
    AFX_FCC_N32     = AfxPackFcc('n', '3', '2', NIL), // afxNat32
    AFX_FCC_N64     = AfxPackFcc('n', '6', '4', NIL), // afxNat64
    AFX_FCC_I       = AfxPackFcc('i', NIL, NIL, NIL), // afxInt
    AFX_FCC_I8      = AfxPackFcc('i', '8', NIL, NIL), // afxInt8
    AFX_FCC_I16     = AfxPackFcc('i', '1', '6', NIL), // afxInt16
    AFX_FCC_I32     = AfxPackFcc('i', '3', '2', NIL), // afxInt32
    AFX_FCC_I64     = AfxPackFcc('i', '6', '4', NIL), // afxInt64
    AFX_FCC_R       = AfxPackFcc('r', NIL, NIL, NIL), // afxReal
    AFX_FCC_R16     = AfxPackFcc('r', '1', '6', NIL), // afxReal16
    AFX_FCC_R32     = AfxPackFcc('r', '3', '2', NIL), // afxReal32

    // *** e2coree ***

    AFX_FCC_OBJ     = AfxPackFcc('o', 'b', 'j', '\0'), // afxInstance
    AFX_FCC_OBJC    = AfxPackFcc('o', 'b', 'j', 'c'), // afxInstanceConnection
    AFX_FCC_SLOT    = AfxPackFcc('s', 'l', 'o', 't'), // afxSlot
    AFX_FCC_HNDL    = AfxPackFcc('h', 'n', 'd', 'l'), // 
    AFX_FCC_CLS    = AfxPackFcc('o', 'b', 'j', 't'), // afxClass
    AFX_FCC_OBJK    = AfxPackFcc('o', 'b', 'j', 'k'), // afxObjack

    AFX_FCC_DBG     = AfxPackFcc('d', 'b', 'g', '\0'), // afxDebugger
    AFX_FCC_APP     = AfxPackFcc('a', 'p', 'p', '\0'), // afxApplication
    AFX_FCC_SYS     = AfxPackFcc('s', 'y', 's', '\0'), // afxSystem

    //AFX_FCC_BUF     = AfxPackFcc('b', 'u', 'f'), // afxBuffer
    AFX_FCC_ARR     = AfxPackFcc('a', 'r', 'r', '\0'), // afxArray
    AFX_FCC_QUE     = AfxPackFcc('q', 'u', 'e', '\0'), // afxQueue
    AFX_FCC_STR     = (('q'<<8)|'s'),//AfxPackFcc('s', 't', 'r', '\0'), // afxString
    AFX_FCC_STRB    = (('q' << 8) | 'z'),//AfxPackFcc('s', 't', 'r', '\0'), // afxString
#if 0
    AFX_FCC_STR0    = AfxPackFcc('s', 't', 'r', '0'), // afxString8
    AFX_FCC_STR1    = AfxPackFcc('s', 't', 'r', '1'), // afxString16
    AFX_FCC_STR2    = AfxPackFcc('s', 't', 'r', '2'), // afxString32
    AFX_FCC_STR3    = AfxPackFcc('s', 't', 'r', '3'), // afxString64
    AFX_FCC_STR4    = AfxPackFcc('s', 't', 'r', '4'), // afxString128
    AFX_FCC_STR5    = AfxPackFcc('s', 't', 'r', '5'), // afxString256
    AFX_FCC_STR6    = AfxPackFcc('s', 't', 'r', '6'), // afxString512
    AFX_FCC_STR7    = AfxPackFcc('s', 't', 'r', '7'), // afxString1024
    AFX_FCC_STR8    = AfxPackFcc('s', 't', 'r', '8'), // afxString2048
    AFX_FCC_STR9    = AfxPackFcc('s', 't', 'r', '9'), // afxString4096
    AFX_FCC_STRC    = AfxPackFcc('s', 't', 'r', 'c'), // afxString
    AFX_FCC_STRE    = AfxPackFcc('s', 't', 'r', 'e'), // afxString excerpt; It is just a view of another string; canv't modify data.
    AFX_FCC_STRF    = AfxPackFcc('s', 't', 'r', 'f'), // afxString
#endif
    AFX_FCC_URI     = AfxPackFcc('u', 'r', 'i', '\0'), // afxUri
    AFX_FCC_SIG     = AfxPackFcc('s', 'i', 'g', '\0'), // afxSignal
    AFX_FCC_SIGH    = AfxPackFcc('s', 'i', 'g', 'h'), // afxSignalHandler
    AFX_FCC_SIGC    = AfxPackFcc('s', 'i', 'g', 'c'), // afxSignalConnection
    AFX_FCC_MEM     = AfxPackFcc('m', 'e', 'm', '\0'), // afxMemory
    AFX_FCC_CTX     = AfxPackFcc('c', 't', 'x', '\0'), // afxContext
    AFX_FCC_MPAG    = AfxPackFcc('m', 'p', 'a', 'g'), // afxMemPage
    //AFX_FCC_CHNK     = AfxPackFcc('m', 'e', 'm', '\0'), // afxContext
    AFX_FCC_ALLS    = AfxPackFcc('a', 'l', 'l', 's'), // afxSlabMemory
    AFX_FCC_SLAB    = AfxPackFcc('s', 'l', 'a', 'b'), // afxSlabHeader
    AFX_FCC_ALLR    = AfxPackFcc('a', 'l', 'l', 'r'), // afxRegionMemory
    AFX_FCC_AREN    = AfxPackFcc('a', 'r', 'e', 'n'), // afxArena
    AFX_FCC_CHNK    = AfxPackFcc('c', 'h', 'n', 'k'),
    AFX_FCC_BLCK    = AfxPackFcc('b', 'l', 'c', 'k'),
    AFX_FCC_BLOB    = AfxPackFcc('b', 'l', 'o', 'b'), // afxBlob
    AFX_FCC_IOS     = AfxPackFcc('i', 'o', 's', '\0'), // afxStream
    AFX_FCC_FSSP    = AfxPackFcc('f', 's', 's', 'p'), // afxStoragePoint
    AFX_FCC_ARC     = AfxPackFcc('a', 'r', 'c', '\0'), // afxArchive
    AFX_FCC_RES     = AfxPackFcc('r', 'e', 's', '\0'), // afxResource
    AFX_FCC_XML     = AfxPackFcc('x', 'm', 'l', '\0'), // afxXml
    AFX_FCC_URD     = AfxPackFcc('u', 'r', 'd', '\0'), // afxUrd
    AFX_FCC_STD     = AfxPackFcc('s', 't', 'd', '\0'), // for standard implementations
    AFX_FCC_SLCK    = AfxPackFcc('l', 'c', 'k', '\0'), // afxSlock
    AFX_FCC_BMP     = AfxPackFcc('b', 'm', 'p', '\0'), // afxBitmap
    AFX_FCC_MAP     = AfxPackFcc('m', 'a', 'p', '\0'), // afxMap
    AFX_FCC_DICT    = AfxPackFcc('d', 'i', 'c', 't'), // afxDictionary
    AFX_FCC_POOL    = AfxPackFcc('p', 'o', 'o', 'l'), // afxPool

    AFX_FCC_EMEC    = AfxPackFcc('e', 's', 'y', 's'), // afxEventMechanism
    AFX_FCC_ETRG    = AfxPackFcc('e', 't', 'r', 'g'), // afxEventTrigger
    AFX_FCC_EHND    = AfxPackFcc('e', 'h', 'n', 'd'), // afxEventHandler

    AFX_FCC_FSYS    = AfxPackFcc('f', 's', 'y', 's'), // afxIoSystem
    AFX_FCC_FSMP    = AfxPackFcc('f', 's', 'm', 'p'), // afxIoSystemStoragePoint
    AFX_FCC_FILE    = AfxPackFcc('f', 'i', 'l', 'e'), // afxFile
    
    AFX_FCC_THR     = AfxPackFcc('t', 'h', 'r', '\0'), // afxThread
    AFX_FCC_MTX     = AfxPackFcc('m', 't', 'x', '\0'), // afxMutex

    AFX_FCC_EVNT    = AfxPackFcc('e', 'v', 'n', 't'), // afxEvent
    AFX_FCC_EVLP    = AfxPackFcc('e', 'v', 'l', 'p'), // afxEventLoop

    AFX_FCC_HID     = AfxPackFcc('h', 'i', 'd', '\0'), // afxHid
    AFX_FCC_KBD     = AfxPackFcc('k', 'b', 'd', '\0'), // afxKeyboard
    AFX_FCC_MSE     = AfxPackFcc('m', 's', 'e', '\0'), // afxMouse
    AFX_FCC_DSO     = AfxPackFcc('d', 's', 'o', '\0'), // afxDso
    AFX_FCC_AGTR    = AfxPackFcc('a', 'g', 't', 'r'), // afxAggregator
    AFX_FCC_AGGR    = AfxPackFcc('a', 'g', 'g', 'r'), // afxAggregation

    AFX_FCC_ASI     = AfxPackFcc('a', 's', 'i', '\0'), // afxService
    AFX_FCC_MDLE    = AfxPackFcc('m', 'd', 'l', 'e'), // afxModule
    AFX_FCC_AUTR    = AfxPackFcc('a', 'u', 't', 'r'), // afxAutomator

    // *** e2math ***

    AFX_FCC_V2D     = AfxPackFcc('v', '2', 'd', '\0'), // afxV2d
    AFX_FCC_V3D     = AfxPackFcc('v', '3', 'd', '\0'), // afxV3d
    AFX_FCC_V4D     = AfxPackFcc('v', '4', 'd', '\0'), // afxV4d
    AFX_FCC_QUAT    = AfxPackFcc('q', '\0', '\0', '\0'), // afxQuat
    AFX_FCC_V       = AfxPackFcc('v', '\0', '\0', '\0'), // afxVersor
    AFX_FCC_M2D     = AfxPackFcc('m', '2', 'd', '\0'), // afxM2d
    AFX_FCC_M3D     = AfxPackFcc('m', '3', 'd', '\0'), // afxM3d
    AFX_FCC_M4D     = AfxPackFcc('m', '4', 'd', '\0'), // afxV4d
    AFX_FCC_SPC     = AfxPackFcc('s', 'p', 'c', '\0'), // afxSpace

    // *** e2draw ***

    AFX_FCC_DSYS    = AfxPackFcc('d', 's', 'y', 's'), // afxDrawSystem
    AFX_FCC_DDEV    = AfxPackFcc('d', 'd', 'e', 'v'), // afxDrawDevice
    AFX_FCC_DDRV    = AfxPackFcc('d', 'd', 'r', 'v'), // afxDrawDevice
    AFX_FCC_DIN     = AfxPackFcc('d', 'i', 'n', '\0'), // afxDrawInput
    AFX_FCC_DOUT    = AfxPackFcc('d', 'o', 'u', 't'), // afxDrawOutput
    AFX_FCC_DQUE    = AfxPackFcc('d', 'q', 'u', 'e'), // afxDrawQueue
    AFX_FCC_DTHR    = AfxPackFcc('d', 't', 'h', 'r'), // afxDrawThread
    AFX_FCC_CANV    = AfxPackFcc('c', 'a', 'n', 'v'), // afxCanvas
    AFX_FCC_DCTX    = AfxPackFcc('d', 'c', 't', 'x'), // afxDrawContext
    AFX_FCC_PIP     = AfxPackFcc('p', 'i', 'p', '\0'), // afxPipeline
    AFX_FCC_PIPR    = AfxPackFcc('p', 'i', 'p', 'a'), // afxPipelineRig
    AFX_FCC_LEGT2    = AfxPackFcc('l', 'e', 'g', 't'), // afxLego
    AFX_FCC_LEGO    = AfxPackFcc('l', 'e', 'g', 'o'), // afxLego
    AFX_FCC_SHD     = AfxPackFcc('s', 'h', 'd', '\0'), // afxShader
    AFX_FCC_SHDB    = AfxPackFcc('s', 'h', 'd', 'b'), // afxShaderBlueprint
    AFX_FCC_PIPB    = AfxPackFcc('p', 'i', 'p', 'b'), // afxPipelineBlueprint
    AFX_FCC_DRES    = AfxPackFcc('d', 'r', 'e', 's'), // afxDrawResourceSet
    AFX_FCC_DSCR    = AfxPackFcc('d', 's', 'c', 'r'), // afxDrawScript
    AFX_FCC_DDBG    = AfxPackFcc('d', 'd', 'b', 'g'), // afxDrawDebugger
    AFX_FCC_DOP     = AfxPackFcc('d', 'o', 'p', '\0'), // afxDrawOperation
    AFX_FCC_DOPB    = AfxPackFcc('d', 'o', 'p', 'b'), // afxDrawOperationBlueprint
    AFX_FCC_DTEC    = AfxPackFcc('d', 't', 'e', 'c'), // afxDrawTechnique
    AFX_FCC_DPAS    = AfxPackFcc('d', 'p', 'a', 's'), // afxDrawPass
    AFX_FCC_VPNT    = AfxPackFcc('v', 'p', 'n', 't'), // afxViewpoint
    AFX_FCC_WIMP    = AfxPackFcc('o', 'v', 'l', 'y'), // afxWimp
    AFX_FCC_WID     = AfxPackFcc('w', 'i', 'd', '\0'), // afxWidget
    AFX_FCC_WND     = AfxPackFcc('w', 'n', 'd', '\0'), // afxWindow
    AFX_FCC_WPP     = AfxPackFcc('w', 'p', 'p', '\0'), // afxWallpaper

    AFX_FCC_DUNI    = AfxPackFcc('d', 'u', 'n', 'i'), // afxDrawableUniverse
    AFX_FCC_DMDL    = AfxPackFcc('d', 'm', 'd', 'l'), // sglDrawableModel
    AFX_FCC_DMSH    = AfxPackFcc('d', 'm', 's', 'h'), // sglDrawableMesh
    AFX_FCC_DMSE    = AfxPackFcc('d', 'm', 's', 'e'), // sglDrawableMeshSection
    AFX_FCC_DMTL    = AfxPackFcc('d', 'm', 't', 'l'), // sglDrawableMaterial
    AFX_FCC_DSKY    = AfxPackFcc('d', 's', 'k', 'y'), // sglDrawableSky
    AFX_FCC_DTER    = AfxPackFcc('d', 't', 'e', 'r'), // sglDrawableTerrain
    AFX_FCC_DTEX    = AfxPackFcc('d', 't', 'e', 'x'), // sglDrawableSky

    // *** e2sound ***

    AFX_FCC_SSYS    = AfxPackFcc('s', 's', 'y', 's'), // afxSoundSystem
    AFX_FCC_SDEV    = AfxPackFcc('s', 'o', 'u', 't'), // afxSoundIoDevice
    AFX_FCC_SOUT    = AfxPackFcc('s', 'o', 'u', 't'), // afxSoundOutput
    AFX_FCC_SIN     = AfxPackFcc('s', 'i', 'n', '\0'), // afxSoundInput
    AFX_FCC_SCTX    = AfxPackFcc('s', 'c', 't', 'x'), // afxSoundContext

    // *** e2social ***

    AFX_FCC_SOCS    = AfxPackFcc('s', 'o', 'c', 's'), // afxSocialSystem
    AFX_FCC_SOCP    = AfxPackFcc('s', 'o', 'c', 'p'), // afxSocialPresence

    // *** e2simul ***

    AFX_FCC_SIM     = AfxPackFcc('s', 'i', 'm', '\0'), // afxSimulation
    AFX_FCC_HUB     = AfxPackFcc('h', 'u', 'b', '\0'), // afxHub
    AFX_FCC_DAG     = AfxPackFcc('d', 'a', 'g', '\0'), // afxDag
    AFX_FCC_NOD     = AfxPackFcc('n', 'o', 'd', '\0'), // afxNode
    AFX_FCC_NODA    = AfxPackFcc('n', 'o', 'd', 'a'), // afxNodeAnnex (node annex)
    AFX_FCC_ASIO    = AfxPackFcc('a', 's', 'i', 'o'), // afxAssetIoAssistent
    AFX_FCC_PVT     = AfxPackFcc('p', 'v', 't', '\0'), // afxNode

    AFX_FCC_IMG     = AfxPackFcc('i', 'm', 'g', '\0'), // afxImage
    AFX_FCC_SAMP     = AfxPackFcc('s', 'm', 'p', '\0'), // afxSampler
    AFX_FCC_BUF     = AfxPackFcc('b', 'u', 'f', '\0'), // afxBuffer
    AFX_FCC_VBUB    = AfxPackFcc('v', 'b', 'u', 'b'), // afxVertexBufferBlueprint
    AFX_FCC_VBUF    = AfxPackFcc('v', 'b', 'u', 'f'), // afxVertexBuffer
    AFX_FCC_IBUF    = AfxPackFcc('i', 'b', 'u', 'f'), // afxIndexBuffer
    AFX_FCC_IMGH    = AfxPackFcc('i', 'm', 'g', 'h'), // AfxImageHub
    AFX_FCC_IMIO    = AfxPackFcc('i', 'm', 'i', 'o'), // AfxImageIoAssistent
    AFX_FCC_TEX     = AfxPackFcc('t', 'e', 'x', '\0'), // afxTexture
    AFX_FCC_SURF    = AfxPackFcc('s', 'u', 'r', 'f'), // afxSurface
    AFX_FCC_TEXB    = AfxPackFcc('t', 'e', 'x', 'b'), // afxTextureBlueprint

    AFX_FCC_MDL     = AfxPackFcc('m', 'd', 'l', '\0'), // afxModel
    AFX_FCC_MDLB    = AfxPackFcc('m', 'd', 'l', 'b'), // afxModelBlueprint
    AFX_FCC_SKL     = AfxPackFcc('s', 'k', 'l', '\0'), // afxSkeleton
    AFX_FCC_SKLB    = AfxPackFcc('s', 'k', 'l', 'b'), // afxSkeletonBlueprint
    AFX_FCC_BONB    = AfxPackFcc('b', 'o', 'n', 'b'), // afxBoneBlueprint
    AFX_FCC_MSHA    = AfxPackFcc('m', 's', 'h', 'a'), // afxMeshArticulation
    AFX_FCC_MSHC    = AfxPackFcc('m', 's', 'h', 'c'), // afxMeshConnection
    AFX_FCC_MSH     = AfxPackFcc('m', 's', 'h', '\0'), // afxMesh
    AFX_FCC_MSHB    = AfxPackFcc('m', 's', 'h', 'b'), // afxMeshBlueprint
    AFX_FCC_MSHT    = AfxPackFcc('m', 's', 'h', 't'), // afxMeshTopology
    AFX_FCC_MSHS    = AfxPackFcc('m', 's', 'h', 's'), // afxMeshSection
    AFX_FCC_MSHM    = AfxPackFcc('m', 's', 'h', 'm'), // afxMeshMorph
    AFX_FCC_MVER    = AfxPackFcc('m', 'v', 'e', 'r'), // afxMeshVertebra
    AFX_FCC_MDLM    = AfxPackFcc('m', 'd', 'l', 'm'), // AfxModelingMechanism
    AFX_FCC_MDIO    = AfxPackFcc('m', 'd', 'i', 'o'), // afxModelIoAssistent
    AFX_FCC_MTL     = AfxPackFcc('m', 't', 'l', '\0'), // afxMaterial
    AFX_FCC_MTLB    = AfxPackFcc('m', 't', 'l', 'b'), // afxMaterialBlueprint

    AFX_FCC_UNI     = AfxPackFcc('u', 'n', 'i', '\0'), // afxUniverse
    AFX_FCC_CLST    = AfxPackFcc('c', 'l', 's', 't'), // afxCluster
    AFX_FCC_ENT     = AfxPackFcc('e', 'n', 't', '\0'), // afxEntity
    AFX_FCC_ASP     = AfxPackFcc('a', 's', 'p', '\0'), // afxAspect
    AFX_FCC_LIT     = AfxPackFcc('l', 'i', 't', '\0'), // afxLight
    AFX_FCC_CAM     = AfxPackFcc('c', 'a', 'm', '\0'), // afxCamera
    AFX_FCC_SKY     = AfxPackFcc('s', 'k', 'y', '\0'), // afxSky
    AFX_FCC_TERR    = AfxPackFcc('t', 'e', 'r', 'r'), // afxTerrain
    AFX_FCC_BRN     = AfxPackFcc('b', 'r', 'n', '\0'), // afxBrain
    AFX_FCC_BOD     = AfxPackFcc('b', 'o', 'd', '\0'), // afxBody
    AFX_FCC_BODC     = AfxPackFcc('p', 'u', 'p', '\0'), // afxBodyControl
    AFX_FCC_BODP    = AfxPackFcc('b', 'o', 'd', 'p'), // afxBodyPart

    AFX_FCC_RND     = AfxPackFcc('r', 'n', 'd', '\0'), // afxRenderer

    AFX_FCC_MAX_    = 0x7FFFFFFF
} afxFcc;

#endif//AFX_FCC_H