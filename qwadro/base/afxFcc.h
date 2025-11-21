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

#ifndef AFX_FCC_H
#define AFX_FCC_H

#include "qwadro/base/afxCoreDefs.h"

#define AFX_MAKE_FCC(ch1, ch2, ch3, ch4) (enum afxFcc)(((((((ch4)<<8)|(ch3))<<8)|(ch2))<<8)|(ch1))
#define AFX_SPLIT_FCC(idn_, cA, cB, cC, cD) (cA=(afxChar)(idn_>>0),cB=(afxChar)(idn_>>8),cC =(afxChar)(idn_>>16), cD = (afxChar)(idn_>>24))

typedef enum afxFcc
{
    afxFcc_NIL,
    //NIL,

    // base type has one or two letter chars.
    // complex types has three letter chars.
    // subcomplementary types (of complex types) usuallys has four letter chars.

    afxFcc_B        = AFX_MAKE_FCC('b', NIL, NIL, NIL), // afxBool
    afxFcc_B8       = AFX_MAKE_FCC('b', '8', NIL, NIL), // afxBool8
    afxFcc_B32      = AFX_MAKE_FCC('b', '3', '2', NIL), // afxBool32
    afxFcc_N        = AFX_MAKE_FCC('n', NIL, NIL, NIL), // afxUnit
    afxFcc_N8       = AFX_MAKE_FCC('n', '8', NIL, NIL), // afxUnit8
    afxFcc_N16      = AFX_MAKE_FCC('n', '1', '6', NIL), // afxUnit16
    afxFcc_N32      = AFX_MAKE_FCC('n', '3', '2', NIL), // afxUnit32
    afxFcc_N64      = AFX_MAKE_FCC('n', '6', '4', NIL), // afxUnit64
    afxFcc_I        = AFX_MAKE_FCC('i', NIL, NIL, NIL), // afxInt
    afxFcc_I8       = AFX_MAKE_FCC('i', '8', NIL, NIL), // afxInt8
    afxFcc_I16      = AFX_MAKE_FCC('i', '1', '6', NIL), // afxInt16
    afxFcc_I32      = AFX_MAKE_FCC('i', '3', '2', NIL), // afxInt32
    afxFcc_I64      = AFX_MAKE_FCC('i', '6', '4', NIL), // afxInt64
    afxFcc_R        = AFX_MAKE_FCC('r', NIL, NIL, NIL), // afxReal
    afxFcc_R16      = AFX_MAKE_FCC('r', '1', '6', NIL), // afxReal16
    afxFcc_R32      = AFX_MAKE_FCC('r', '3', '2', NIL), // afxReal32

    // *** e2coree ***

    afxFcc_OBJ      = AFX_MAKE_FCC('o', 'b', 'j', '\0'), // afxHandle
    afxFcc_OBJC     = AFX_MAKE_FCC('o', 'b', 'j', 'c'), // afxInstanceConnection
    afxFcc_SLOT     = AFX_MAKE_FCC('s', 'l', 'o', 't'), // afxSlot
    afxFcc_HNDL     = AFX_MAKE_FCC('h', 'n', 'd', 'l'), // 
    afxFcc_CLS      = AFX_MAKE_FCC('o', 'b', 'j', 't'), // afxClass
    afxFcc_OBJK     = AFX_MAKE_FCC('o', 'b', 'j', 'k'), // afxObjack
    
    afxFcc_DBG      = AFX_MAKE_FCC('d', 'b', 'g', '\0'), // afxDebugger
    afxFcc_APP      = AFX_MAKE_FCC('a', 'p', 'p', '\0'), // afxApplication
    afxFcc_PROC     = AFX_MAKE_FCC('p', 'r', 'o', 'c'), // afxProcess
    afxFcc_XSS      = AFX_MAKE_FCC('x', 's', 's', '\0'), // afxScript
    afxFcc_SVC      = AFX_MAKE_FCC('s', 'v', 'c', '\0'), // afxService
    afxFcc_DEV      = AFX_MAKE_FCC('d', 'e', 'v', '\0'), // afxDevice
    afxFcc_SYS      = AFX_MAKE_FCC('s', 'y', 's', '\0'), // afxSystem

    //afxFcc_BUF     = AFX_MAKE_FCC('b', 'u', 'f'), // avxBuffer
    afxFcc_ARR      = AFX_MAKE_FCC('a', 'r', 'r', '\0'), // afxArray
    afxFcc_QUE      = AFX_MAKE_FCC('q', 'u', 'e', '\0'), // afxQueue
    afxFcc_FIFO     = AFX_MAKE_FCC('f', 'i', 'f', 'o'), // afxInterlockedQueue
    afxFcc_STAK     = AFX_MAKE_FCC('s', 't', 'a', 'k'), // afxStack
    afxFcc_STR      = (('q'<<8)|'s'),//AFX_MAKE_FCC('s', 't', 'r', '\0'), // afxString
    afxFcc_STRsB     = (('q' << 8) | 'z'),//AFX_MAKE_FCC('s', 't', 'r', '\0'), // afxString
#if !0
    afxFcc_STR0     = AFX_MAKE_FCC('s', 't', 'r', '0'), // afxString8
    afxFcc_STR1     = AFX_MAKE_FCC('s', 't', 'r', '1'), // afxString16
    afxFcc_STR2     = AFX_MAKE_FCC('s', 't', 'r', '2'), // afxString32
    afxFcc_STR3     = AFX_MAKE_FCC('s', 't', 'r', '3'), // afxString64
    afxFcc_STR4     = AFX_MAKE_FCC('s', 't', 'r', '4'), // afxString128
    afxFcc_STR5     = AFX_MAKE_FCC('s', 't', 'r', '5'), // afxString256
    afxFcc_STR6     = AFX_MAKE_FCC('s', 't', 'r', '6'), // afxString512
    afxFcc_STR7     = AFX_MAKE_FCC('s', 't', 'r', '7'), // afxString1024
    afxFcc_STR8     = AFX_MAKE_FCC('s', 't', 'r', '8'), // afxString2048
    afxFcc_STR9     = AFX_MAKE_FCC('s', 't', 'r', '9'), // afxString4096
    afxFcc_STRB     = AFX_MAKE_FCC('s', 't', 'r', 'b'), // afxStringBase
    afxFcc_STRE     = AFX_MAKE_FCC('s', 't', 'r', 'e'),
    afxFcc_STRF     = AFX_MAKE_FCC('s', 't', 'r', 'f'),
#endif
    afxFcc_URI      = AFX_MAKE_FCC('u', 'r', 'i', '\0'), // afxUri
    afxFcc_SRC      = AFX_MAKE_FCC('s', 'r', 'c', '\0'), // afxSource
    afxFcc_SIG      = AFX_MAKE_FCC('s', 'i', 'g', '\0'), // afxSignal
    afxFcc_SIGH     = AFX_MAKE_FCC('s', 'i', 'g', 'h'), // afxSignalHandler
    afxFcc_SIGC     = AFX_MAKE_FCC('s', 'i', 'g', 'c'), // afxSignalConnection
    afxFcc_MEM      = AFX_MAKE_FCC('m', 'e', 'm', '\0'), // afxMemory
    afxFcc_MMU      = AFX_MAKE_FCC('m', 'm', 'u', '\0'), // afxMmu
    afxFcc_DEVK      = AFX_MAKE_FCC('d', 'e', 'v', 'k'), // afxDevLink
    afxFcc_MPAG     = AFX_MAKE_FCC('m', 'p', 'a', 'g'), // afxMemPage
    //afxFcc_CHNK     = AFX_MAKE_FCC('m', 'e', 'm', '\0'), // afxMmu
    afxFcc_ALLS     = AFX_MAKE_FCC('a', 'l', 'l', 's'), // afxSlabMemory
    afxFcc_SLAB     = AFX_MAKE_FCC('s', 'l', 'a', 'b'), // afxSlabHeader
    afxFcc_ALLR     = AFX_MAKE_FCC('a', 'l', 'l', 'r'), // afxRegionMemory
    afxFcc_AREN     = AFX_MAKE_FCC('a', 'r', 'e', 'n'), // afxArena
    afxFcc_CHNK     = AFX_MAKE_FCC('c', 'h', 'n', 'k'),
    afxFcc_BLCK     = AFX_MAKE_FCC('b', 'l', 'c', 'k'),
    afxFcc_BLOB     = AFX_MAKE_FCC('b', 'l', 'o', 'b'), // afxBlob
    afxFcc_IOB      = AFX_MAKE_FCC('i', 'o', 'b', '\0'), // afxStream
    afxFcc_IOS      = AFX_MAKE_FCC('i', 'o', 's', '\0'), // afxStream
    afxFcc_CDC      = AFX_MAKE_FCC('c', 'd', 'c', '\0'), // afxCodec
    afxFcc_FSSP     = AFX_MAKE_FCC('f', 's', 's', 'p'), // afxStoragePoint
    afxFcc_STO      = AFX_MAKE_FCC('s', 't', 'o', '\0'), // afxStorage
    afxFcc_ARC      = AFX_MAKE_FCC('a', 'r', 'c', '\0'), // afxArchive
    afxFcc_RES      = AFX_MAKE_FCC('r', 'e', 's', '\0'), // afxResource
    afxFcc_XML      = AFX_MAKE_FCC('x', 'm', 'l', '\0'), // afxXml
    afxFcc_URD      = AFX_MAKE_FCC('u', 'r', 'd', '\0'), // afxUrd
    afxFcc_TGA      = AFX_MAKE_FCC('t', 'g', 'a', '\0'), // Targa
    afxFcc_STD      = AFX_MAKE_FCC('s', 't', 'd', '\0'), // for standard implementations
    afxFcc_SLCK     = AFX_MAKE_FCC('l', 'c', 'k', '\0'), // afxFutex
    afxFcc_BMP      = AFX_MAKE_FCC('b', 'm', 'p', '\0'), // afxBitmap
    afxFcc_MAP      = AFX_MAKE_FCC('m', 'a', 'p', '\0'), // afxMap
    afxFcc_DICT     = AFX_MAKE_FCC('d', 'i', 'c', 't'), // afxDictionary
    afxFcc_POOL     = AFX_MAKE_FCC('p', 'o', 'o', 'l'), // afxPool
    afxFcc_EXU      = AFX_MAKE_FCC('e', 'x', 'u', '\0'), // afxIoBridge
    afxFcc_XQUE     = AFX_MAKE_FCC('x', 'q', 'u', 'e'), // afxIoQueue
    afxFcc_IOC      = AFX_MAKE_FCC('i', 'o', 'c', '\0'), // afxIoContext
    afxFcc_XCMD     = AFX_MAKE_FCC('x', 'c', 'm', 'd'), // afxIoCmd

    afxFcc_EMEC     = AFX_MAKE_FCC('e', 's', 'y', 's'), // afxEventMechanism
    afxFcc_ETRG     = AFX_MAKE_FCC('e', 't', 'r', 'g'), // afxEventTrigger
    afxFcc_EHND     = AFX_MAKE_FCC('e', 'h', 'n', 'd'), // afxEventHandler

    afxFcc_FSYS     = AFX_MAKE_FCC('f', 's', 'y', 's'), // afxIoSystem
    afxFcc_FSMP     = AFX_MAKE_FCC('f', 's', 'm', 'p'), // afxIoSystemStoragePoint
    afxFcc_FILE     = AFX_MAKE_FCC('f', 'i', 'l', 'e'), // afxFile
    
    afxFcc_THR      = AFX_MAKE_FCC('t', 'h', 'r', '\0'), // afxThread
    afxFcc_THRP     = AFX_MAKE_FCC('t', 'h', 'r', 'p'), // afxThreadPool
    afxFcc_TXU      = AFX_MAKE_FCC('t', 'x', 'u', '\0'), // afxTxu
    afxFcc_MTX      = AFX_MAKE_FCC('m', 't', 'x', '\0'), // afxMutex

    afxFcc_EVNT     = AFX_MAKE_FCC('e', 'v', 'n', 't'), // afxEvent
    afxFcc_EVLP     = AFX_MAKE_FCC('e', 'v', 'l', 'p'), // afxEventLoop

    afxFcc_HID      = AFX_MAKE_FCC('h', 'i', 'd', '\0'), // afxHid
    afxFcc_KBD      = AFX_MAKE_FCC('k', 'b', 'd', '\0'), // afxKeyboard
    afxFcc_MSE      = AFX_MAKE_FCC('m', 's', 'e', '\0'), // afxMouse
    afxFcc_PAD      = AFX_MAKE_FCC('p', 'a', 'd', '\0'), // afxGamepad
    afxFcc_CTRL      = AFX_MAKE_FCC('c', 't', 'r', 'l'), // afxController
    afxFcc_DSO      = AFX_MAKE_FCC('d', 's', 'o', '\0'), // afxDso
    afxFcc_AGTR     = AFX_MAKE_FCC('a', 'g', 't', 'r'), // afxAggregator
    afxFcc_AGGR     = AFX_MAKE_FCC('a', 'g', 'g', 'r'), // afxAggregation

    afxFcc_ASI      = AFX_MAKE_FCC('a', 's', 'i', '\0'), // afxService
    afxFcc_EXE      = AFX_MAKE_FCC('e', 'x', 'e', '\0'), // afxExecutable
    afxFcc_MDLE     = AFX_MAKE_FCC('m', 'd', 'l', 'e'), // afxModule
    afxFcc_ICD      = AFX_MAKE_FCC('i', 'c', 'd', '\0'), // afxIcd
    afxFcc_DRV      = AFX_MAKE_FCC('d', 'r', 'v', '\0'), // afxDriver
    afxFcc_AUTR     = AFX_MAKE_FCC('a', 'u', 't', 'r'), // afxAutomator

    afxFcc_SES      = AFX_MAKE_FCC('e', 'n', 'v', '\0'), // afxSession
    afxFcc_SSH      = AFX_MAKE_FCC('s', 's', 'h', '\0'), // afxShell
    afxFcc_ENV      = AFX_MAKE_FCC('s', 'e', 's', '\0'), // afxEnvironment
    afxFcc_USYS     = AFX_MAKE_FCC('u', 's', 'y', 's'), // afxUserSystem

    // *** e2math ***

    afxFcc_V2D      = AFX_MAKE_FCC('v', '2', 'd', '\0'), // afxV2d
    afxFcc_V3D      = AFX_MAKE_FCC('v', '3', 'd', '\0'), // afxV3d
    afxFcc_V4D      = AFX_MAKE_FCC('v', '4', 'd', '\0'), // afxV4d
    afxFcc_QUAT     = AFX_MAKE_FCC('q', '\0', '\0', '\0'), // afxQuat
    afxFcc_V        = AFX_MAKE_FCC('v', '\0', '\0', '\0'), // afxVersor
    afxFcc_M2D      = AFX_MAKE_FCC('m', '2', 'd', '\0'), // afxM2d
    afxFcc_M3D      = AFX_MAKE_FCC('m', '3', 'd', '\0'), // afxM3d
    afxFcc_M4D      = AFX_MAKE_FCC('m', '4', 'd', '\0'), // afxV4d
    afxFcc_SPC      = AFX_MAKE_FCC('s', 'p', 'c', '\0'), // afxSpace

    // *** e2draw ***

    afxFcc_DSYS     = AFX_MAKE_FCC('d', 's', 'y', 's'), // afxDrawSystem
    afxFcc_DDRV     = AFX_MAKE_FCC('d', 'd', 'r', 'v'), // afxDrawDriver
    afxFcc_DDEV     = AFX_MAKE_FCC('d', 'd', 'e', 'v'), // afxDrawDevice
    afxFcc_VDU      = AFX_MAKE_FCC('v', 'd', 'u', '\0'), // afxVdu
    afxFcc_DOUT     = AFX_MAKE_FCC('d', 'o', 'u', 't'), // afxSurface
    afxFcc_DIN      = AFX_MAKE_FCC('d', 'i', 'n', '\0'), // afxDrawInput
    afxFcc_RCTX     = AFX_MAKE_FCC('r', 'w', 'e', '\0'), // arxRenderContext
    afxFcc_SCIO     = AFX_MAKE_FCC('s', 'c', 'i', 'o'), // arxScenario
    afxFcc_DDGE     = AFX_MAKE_FCC('d', 'd', 'g', 'e'), // afxDrawBridge
    afxFcc_DEXU     = AFX_MAKE_FCC('d', 'e', 'x', 'u'), // afxDrawBridge
    afxFcc_DQUE     = AFX_MAKE_FCC('d', 'q', 'u', 'e'), // afxDrawQueue
    afxFcc_DENG     = AFX_MAKE_FCC('d', 'e', 'n', 'g'), // afxTxu
    afxFcc_DTHR     = AFX_MAKE_FCC('d', 't', 'h', 'r'), // afxDrawThread
    afxFcc_CANV     = AFX_MAKE_FCC('c', 'a', 'n', 'v'), // avxCanvas
    afxFcc_FENC     = AFX_MAKE_FCC('f', 'e', 'n', 'c'), // avxFence
    afxFcc_SEM      = AFX_MAKE_FCC('s', 'e', 'm', '\0'), // afxSemaphore
    afxFcc_DCTX     = AFX_MAKE_FCC('d', 'c', 't', 'x'), // afxDrawContext
    afxFcc_DCMD     = AFX_MAKE_FCC('d', 'c', 'm', 'd'), // afxDrawCmd
    afxFcc_PIP      = AFX_MAKE_FCC('p', 'i', 'p', '\0'), // avxPipeline
    afxFcc_RAZR     = AFX_MAKE_FCC('r', 'a', 's', '\0'), // avxRasterizer
    afxFcc_VIN      = AFX_MAKE_FCC('v', 'i', 'n', '\0'), // avxVertexInput
    afxFcc_LEGT2    = AFX_MAKE_FCC('l', 'e', 'g', 't'), // avxLigature
    afxFcc_LEGO     = AFX_MAKE_FCC('l', 'e', 'g', 'o'), // avxLigature
    afxFcc_BSCH     = AFX_MAKE_FCC('b', 's', 'c', 'h'), // avxLigature
    afxFcc_LIGA     = AFX_MAKE_FCC('l', 'i', 'g', 'a'), // avxLigature
    afxFcc_QRYP     = AFX_MAKE_FCC('q', 'r', 'y', 'p'), // avxQueryPool
    afxFcc_SHD      = AFX_MAKE_FCC('s', 'h', 'd', '\0'), // afxShader
    afxFcc_SHDB     = AFX_MAKE_FCC('s', 'h', 'd', 'b'), // afxShaderBlueprint
    afxFcc_PIPB     = AFX_MAKE_FCC('p', 'i', 'p', 'b'), // avxPipelineConfig
    afxFcc_DRES     = AFX_MAKE_FCC('d', 'r', 'e', 's'), // afxDrawResourceSet
    afxFcc_DIOB     = AFX_MAKE_FCC('d', 'i', 'o', 'b'), // afxDrawIob
    afxFcc_CMDB     = AFX_MAKE_FCC('c', 'm', 'd', 'b'), // afxDrawContext
    afxFcc_DDBG     = AFX_MAKE_FCC('d', 'd', 'b', 'g'), // afxDrawDebugger
    afxFcc_DOP      = AFX_MAKE_FCC('d', 'o', 'p', '\0'), // afxDrawOperation
    afxFcc_DOPB     = AFX_MAKE_FCC('d', 'o', 'p', 'b'), // afxDrawOperationBlueprint
    afxFcc_DTEC     = AFX_MAKE_FCC('d', 't', 'e', 'c'), // arxTechnique
    afxFcc_DPAS     = AFX_MAKE_FCC('d', 'p', 'a', 's'), // afxDrawPass
    afxFcc_VPNT     = AFX_MAKE_FCC('v', 'p', 'n', 't'), // afxViewpoint
    afxFcc_OVY      = AFX_MAKE_FCC('o', 'v', 'y', '\0'), // afxOverlay
    afxFcc_WID      = AFX_MAKE_FCC('w', 'i', 'd', '\0'), // afxWidget
    afxFcc_WND      = AFX_MAKE_FCC('w', 'n', 'd', '\0'), // afxWindow
    afxFcc_WPP      = AFX_MAKE_FCC('w', 'p', 'p', '\0'), // afxWallpaper
    afxFcc_TYP      = AFX_MAKE_FCC('t', 'y', 'p', '\0'), // afxTypography
    afxFcc_FNT      = AFX_MAKE_FCC('f', 'n', 't', '\0'), // afxFont
    afxFcc_THEM     = AFX_MAKE_FCC('t', 'h', 'e', 'm'), // afxTheme
    afxFcc_LAY      = AFX_MAKE_FCC('l', 'a', 'y', '\0'), // afxLayout

    afxFcc_DUNI     = AFX_MAKE_FCC('d', 'u', 'n', 'i'), // afxDrawableUniverse
    afxFcc_DMDL     = AFX_MAKE_FCC('d', 'm', 'd', 'l'), // sglDrawableModel
    afxFcc_DMSH     = AFX_MAKE_FCC('d', 'm', 's', 'h'), // sglDrawableMesh
    afxFcc_DMSE     = AFX_MAKE_FCC('d', 'm', 's', 'e'), // sglDrawableMeshSection
    afxFcc_DMTL     = AFX_MAKE_FCC('d', 'm', 't', 'l'), // sglDrawableMaterial
    afxFcc_DSKY     = AFX_MAKE_FCC('d', 's', 'k', 'y'), // sglDrawableSky
    afxFcc_DTER     = AFX_MAKE_FCC('d', 't', 'e', 'r'), // sglDrawableTerrain
    afxFcc_DTEX     = AFX_MAKE_FCC('d', 't', 'e', 'x'), // sglDrawableSky

    // *** e2sound ***

    afxFcc_MSYS     = AFX_MAKE_FCC('m', 's', 'y', 's'), // afxMixSystem
    afxFcc_MDEV     = AFX_MAKE_FCC('m', 'o', 'u', 't'), // afxMixDevice
    afxFcc_MEXU     = AFX_MAKE_FCC('s', 'e', 'x', 'u'), // afxMixBridge
    afxFcc_MTHR     = AFX_MAKE_FCC('m', 't', 'h', 'r'), // afxMixThread
    afxFcc_MQUE     = AFX_MAKE_FCC('s', 'q', 'u', 'e'), // afxMixQueue
    afxFcc_MOUT     = AFX_MAKE_FCC('m', 'o', 'u', 't'), // afxMixOutput
    afxFcc_MIN      = AFX_MAKE_FCC('m', 'i', 'n', '\0'), // afxMixInput
    afxFcc_ASIO     = AFX_MAKE_FCC('a', 's', 'i', 'o'), // afxSink
    afxFcc_MIX      = AFX_MAKE_FCC('m', 'i', 'x', '\0'), // afxMixContext
    afxFcc_MCMD     = AFX_MAKE_FCC('m', 'c', 'm', 'd'), // amxMixCmd
    afxFcc_MSES     = AFX_MAKE_FCC('m', 's', 'e', 's'), // amxTracker
    afxFcc_MCDC     = AFX_MAKE_FCC('m', 'c', 'd', 'c'), // amxCodec
    afxFcc_MENC     = AFX_MAKE_FCC('m', 'e', 'n', 'c'), // amxEncoder
    afxFcc_MDEC     = AFX_MAKE_FCC('m', 'd', 'e', 'c'), // amxDecoder
    afxFcc_MSRC     = AFX_MAKE_FCC('m', 's', 'r', 'c'), // amxSource
    afxFcc_SCTX     = AFX_MAKE_FCC('s', 'c', 't', 'x'), // afxSoundContext
    //afxFcc_SBUF     = AFX_MAKE_FCC('s', 'b', 'u', 'f'), // afxWaveform
    afxFcc_MBUF     = AFX_MAKE_FCC('m', 'b', 'u', 'f'), // amxBuffer
    afxFcc_AUD      = AFX_MAKE_FCC('a', 'u', 'd', '\0'), // amxAudio
    afxFcc_TRAX     = AFX_MAKE_FCC('t', 'r', 'a', 'x'), // amxTracker
    afxFcc_VOX      = AFX_MAKE_FCC('v', 'o', 'a', '\0'), // amxVoice
    afxFcc_BROK     = AFX_MAKE_FCC('b', 'r', 'o', 'k'), // afxBroker
    afxFcc_SNDS     = AFX_MAKE_FCC('s', 'n', 'd', 's'), // amxSoundscape
    afxFcc_SND      = AFX_MAKE_FCC('s', 'n', 'd', '\0'), // amxSound
    afxFcc_VID      = AFX_MAKE_FCC('v', 'i', 'd', '\0'), // amxVideo
    afxFcc_VIDE     = AFX_MAKE_FCC('v', 'i', 'd', 'e'), // afxVideoEncoder
    afxFcc_VIDD     = AFX_MAKE_FCC('v', 'i', 'd', 'd'), // amxVideoDecoder

    // *** e2social ***

    afxFcc_SOCS     = AFX_MAKE_FCC('s', 'o', 'c', 's'), // afxSocialSystem
    afxFcc_SOCP     = AFX_MAKE_FCC('s', 'o', 'c', 'p'), // afxSocialPresence

    // *** e2simul ***

    afxFcc_SSYS     = AFX_MAKE_FCC('m', 's', 'y', 's'), // afxComboSystem
    afxFcc_SDEV     = AFX_MAKE_FCC('m', 'd', 'e', 'v'), // arxEngine
    afxFcc_SEXU     = AFX_MAKE_FCC('m', 'e', 'x', 'u'), // arxSimBridge
    afxFcc_SQUE     = AFX_MAKE_FCC('m', 'q', 'u', 'e'), // arxSimQueue
    afxFcc_CTX     = AFX_MAKE_FCC('c', 't', 'x', '\0'), // arxContext
    afxFcc_SIM      = AFX_MAKE_FCC('s', 'i', 'm', '\0'), // arxSimulation
    afxFcc_HUB      = AFX_MAKE_FCC('h', 'u', 'b', '\0'), // afxHub
    afxFcc_DAG      = AFX_MAKE_FCC('d', 'a', 'g', '\0'), // afxDag
    afxFcc_NOD      = AFX_MAKE_FCC('n', 'o', 'd', '\0'), // arxNode
    afxFcc_NODA     = AFX_MAKE_FCC('n', 'o', 'd', 'a'), // akxNodeAnnex (node annex)
    //afxFcc_ASIO     = AFX_MAKE_FCC('a', 's', 'i', 'o'), // akxAssetIoAssistent
    afxFcc_PVT      = AFX_MAKE_FCC('p', 'v', 't', '\0'), // arxNode

    afxFcc_IMG      = AFX_MAKE_FCC('i', 'm', 'g', '\0'), // afxImage
    afxFcc_SAMP     = AFX_MAKE_FCC('s', 'm', 'p', '\0'), // avxSampler
    afxFcc_BUF      = AFX_MAKE_FCC('b', 'u', 'f', '\0'), // avxBuffer
    afxFcc_VBUB     = AFX_MAKE_FCC('v', 'b', 'u', 'b'), // afxVertexBufferBlueprint
    afxFcc_VBUF     = AFX_MAKE_FCC('v', 'b', 'u', 'f'), // afxVertexBuffer
    afxFcc_IBUF     = AFX_MAKE_FCC('i', 'b', 'u', 'f'), // afxIndexBuffer
    afxFcc_IMGH     = AFX_MAKE_FCC('i', 'm', 'g', 'h'), // AfxImageHub
    afxFcc_IMIO     = AFX_MAKE_FCC('i', 'm', 'i', 'o'), // AfxImageIoAssistent
    afxFcc_RAS      = AFX_MAKE_FCC('t', 'e', 'x', '\0'), // avxRaster
    afxFcc_SURF     = AFX_MAKE_FCC('s', 'u', 'r', 'f'), // _avxCanvasBin
    afxFcc_TEXB     = AFX_MAKE_FCC('t', 'e', 'x', 'b'), // afxTextureBlueprint

    afxFcc_MDL      = AFX_MAKE_FCC('m', 'd', 'l', '\0'), // arxModel
    afxFcc_MDLB     = AFX_MAKE_FCC('m', 'd', 'l', 'b'), // arxModelBlueprint
    afxFcc_SKL      = AFX_MAKE_FCC('s', 'k', 'l', '\0'), // arxSkeleton
    afxFcc_SKLB     = AFX_MAKE_FCC('s', 'k', 'l', 'b'), // arxSkeletonBlueprint
    afxFcc_BONB     = AFX_MAKE_FCC('b', 'o', 'n', 'b'), // afxBoneBlueprint
    afxFcc_POSE     = AFX_MAKE_FCC('p', 'o', 's', 'e'), // arxPose
    afxFcc_POSB     = AFX_MAKE_FCC('p', 'o', 's', 'b'), // arxPoseBuffer
    afxFcc_PLCE     = AFX_MAKE_FCC('p', 'o', 's', 'b'), // arxPlacement
    afxFcc_MSHV     = AFX_MAKE_FCC('m', 's', 'h', 'v'), // arxMeshBias
    afxFcc_MSHC     = AFX_MAKE_FCC('m', 's', 'h', 'c'), // arxMeshConnection
    afxFcc_MSH      = AFX_MAKE_FCC('m', 's', 'h', '\0'), // arxMesh
    afxFcc_MSHL     = AFX_MAKE_FCC('r', 'm', 's', 'h'), // arxMeshLinkage
    afxFcc_MSHB     = AFX_MAKE_FCC('m', 's', 'h', 'b'), // arxMeshBlueprint
    afxFcc_GEO      = AFX_MAKE_FCC('g', 'e', 'o', '\0'), // afxGeometry
    afxFcc_VTD      = AFX_MAKE_FCC('v', 't', 'd', '\0'), // afxVertexData
    afxFcc_VTDS     = AFX_MAKE_FCC('v', 't', 'd', 's'), // akxVertexDataStream
    afxFcc_MSHT     = AFX_MAKE_FCC('m', 's', 'h', 't'), // arxMeshTopology
    afxFcc_MSHS     = AFX_MAKE_FCC('m', 's', 'h', 's'), // arxMeshSection
    afxFcc_MSHM     = AFX_MAKE_FCC('m', 's', 'h', 'm'), // arxMeshMorph
    afxFcc_MSHR     = AFX_MAKE_FCC('m', 's', 'h', 'r'), // _arxMeshLinkage
    afxFcc_MDLM     = AFX_MAKE_FCC('m', 'd', 'l', 'm'), // AfxModelingMechanism
    afxFcc_MDIO     = AFX_MAKE_FCC('m', 'd', 'i', 'o'), // akxModelIoAssistent
    afxFcc_SHAP     = AFX_MAKE_FCC('s', 'h', 'a', 'p'), // asxShape
    afxFcc_MTL      = AFX_MAKE_FCC('m', 't', 'l', '\0'), // arxMaterial
    afxFcc_MORP     = AFX_MAKE_FCC('m', 't', 'l', 'y'), // arxGeome
    afxFcc_MTLB     = AFX_MAKE_FCC('m', 't', 'l', 'b'), // afxMaterialBlueprint
    afxFcc_TXD      = AFX_MAKE_FCC('t', 'x', 'd', '\0'), // avxTxd
    afxFcc_TEX      = AFX_MAKE_FCC('t', 'e', 'x', '\0'), // afxTexture

    afxFcc_CAD      = AFX_MAKE_FCC('c', 'a', 'd', '\0'), // akxAsset

    afxFcc_UNI      = AFX_MAKE_FCC('u', 'n', 'i', '\0'), // afxUniverse
    afxFcc_CLST     = AFX_MAKE_FCC('c', 'l', 's', 't'), // afxCluster
    afxFcc_ENT      = AFX_MAKE_FCC('e', 'n', 't', '\0'), // arxEntity
    afxFcc_ASP      = AFX_MAKE_FCC('a', 's', 'p', '\0'), // afxAspect
    afxFcc_LIT      = AFX_MAKE_FCC('l', 'i', 't', '\0'), // akxLight
    afxFcc_CAM      = AFX_MAKE_FCC('c', 'a', 'm', '\0'), // arxCamera
    afxFcc_SCN      = AFX_MAKE_FCC('s', 'c', 'n', '\0'), // arxSky
    afxFcc_SKY      = AFX_MAKE_FCC('s', 'k', 'y', '\0'), // akxSky
    afxFcc_TERR     = AFX_MAKE_FCC('t', 'e', 'r', 'r'), // arxTerrain
    afxFcc_BRN      = AFX_MAKE_FCC('b', 'r', 'n', '\0'), // afxBrain
    afxFcc_BOD      = AFX_MAKE_FCC('b', 'o', 'd', '\0'), // arxBody
    afxFcc_AMUS     = AFX_MAKE_FCC('a', 'm', 'u', 's'), // akxAnimus
    afxFcc_MOTO     = AFX_MAKE_FCC('m', 'o', 't', '\0'), // arxCapstan
    afxFcc_BODC     = AFX_MAKE_FCC('p', 'u', 'p', '\0'), // arxBodyControl
    afxFcc_BODP     = AFX_MAKE_FCC('b', 'o', 'd', 'p'), // arxBodyPart
    afxFcc_ANI      = AFX_MAKE_FCC('a', 'n', 'i', '\0'), // arxAnimation
    afxFcc_ANIK     = AFX_MAKE_FCC('a', 'n', 'i', 'k'), // arxInstancedAnimation
    afxFcc_MOT      = AFX_MAKE_FCC('m', 'o', 't', '\0'), // arxMotion
    afxFcc_MOTV     = AFX_MAKE_FCC('m', 'o', 't', 'v'), // arxMotive
    afxFcc_CUR      = AFX_MAKE_FCC('c', 'u', 'r', '\0'), // arxCurve
    afxFcc_RND      = AFX_MAKE_FCC('r', 'n', 'd', '\0'), // arxRenderer
    afxFcc_SBUF     = AFX_MAKE_FCC('s', 'b', 'u', 'f'), // arxBuffer
    afxFcc_SMEM     = AFX_MAKE_FCC('s', 'm', 'e', 'm'), // asxMemory

    afxFcc_TER      = AFX_MAKE_FCC('t', 'e', 'r', '\0'), // arxTerrain
    
    afxFcc_CONS     = AFX_MAKE_FCC('c', 'o', 'n', 's'), // apxConstraint
    afxFcc_JNT      = AFX_MAKE_FCC('j', 'n', 't', '\0'), // apxJoint
    afxFcc_FLD      = AFX_MAKE_FCC('f', 'l', 'd', '\0'), // apxField
    afxFcc_PAWN     = AFX_MAKE_FCC('p', 'a', 'w', 'n'), // apxPawn
    afxFcc_COL      = AFX_MAKE_FCC('c', 'o', 'l', '\0'), // apxCollidable
    afxFcc_COLL     = AFX_MAKE_FCC('c', 'o', 'l', 'l'), // apxCollider

    //afxFcc_RCTX     = AFX_MAKE_FCC('d', 'i', 'n', '\0'), // arxRenderContext
    
    afxFcc_MAX_     = 0x7FFFFFFF
} afxFcc;

#endif//AFX_FCC_H
