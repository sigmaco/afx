/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_WIDGET_DDK_H
#define AUX_WIDGET_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

#ifdef _AUX_UX_C

AFX_DEFINE_STRUCT(_auxCmdBatch)
{
    afxFcc          fcc;
    afxUnit         uniqId;
    afxLink         recyc;
    afxBool         once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool         deferred;
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.
    afxChain        commands;
    afxArena        cmdArenaCompiled;
};

AFX_DEFINE_STRUCT(_auxGuiElemHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;

    afxLink parent;
    afxChain children;
};

AFX_DEFINE_UNION(_auxGuiElem)
{
    _auxGuiElemHdr hdr;

    struct
    {
        _auxGuiElemHdr hdr;

        afxPanelInfo info;
    } CommencePanel;
    struct
    {
        _auxGuiElemHdr hdr;

        afxInt nothing;
    } ConcludePanel;
    struct
    {
        _auxGuiElemHdr  hdr;
        afxLink     stackLink;
        afxUnit     itemCnt;
        auxLayoutDirection dir;
    } PushLayout;
    struct
    {
        _auxGuiElemHdr hdr;

    } PopLayout;
    struct
    {
        _auxGuiElemHdr hdr;

        afxLink     stackLink;
        afxUnit     itemCnt;
        afxString   caption;
    } PushGroup;
    struct
    {
        _auxGuiElemHdr hdr;

    } PopGroup;
    struct
    {
        _auxGuiElemHdr hdr;

        afxLabelInfo info;
    } Label;
    struct
    {
        _auxGuiElemHdr hdr;

        afxButtonInfo info;
    } Button;
    struct
    {
        _auxGuiElemHdr hdr;

        afxCheckboxInfo info;
        afxBool* checked;
    } Checkbox;
    struct
    {
        _auxGuiElemHdr hdr;

    } Disable;
    struct
    {
        _auxGuiElemHdr hdr;

    } Separator;
    struct
    {
        _auxGuiElemHdr hdr;

        afxTextInputInfo info;
    }TextInputInlined;
    struct
    {
        _auxGuiElemHdr hdr;

        afxString2048 label;
        afxV4d color;
    } CommenceDebugScope;
    struct
    {
        _auxGuiElemHdr hdr;

        afxInt nothing;
    } ConcludeDebugScope;
    struct
    {
        _auxGuiElemHdr hdr;

        afxString2048 label;
        afxV4d color;
    } MarkDebugMilestone;
};

AFX_DEFINE_UNION(_auxGuiElemLut)
{
    struct
    {
        void* _reservedToBeZero;
        void* CommencePanel;
        void* ConcludePanel;
        void* PushLayout;
        void* PopLayout;
        void* PushGroup;
        void* PopGroup;
        void* Checkbox;
        void* Button;
        void* Label;
        void* Disable;
        void* Separator;
        void* TextInputInlined;
        void* CommenceDebugScope;
        void* ConcludeDebugScope;
        void* MarkDebugMilestone;
    };
    void(*f[])(void*, _auxGuiElem const*);
};

#define _AUX_GUI_ELEM_ID(cmdName_) (offsetof(_auxGuiElemLut, cmdName_) / sizeof(void*))

AUX _auxCmdBatch* _AuxGetCmdBatch(afxWidget wid, afxUnit idx);
AUX _auxGuiElem* _AuxWidPushGuiElem(afxWidget wid, afxUnit id, afxUnit siz, afxCmdId* cmdId);

AFX_DECLARE_STRUCT(_auxIddWid);
AFX_DECLARE_STRUCT(_auxDdiWid);

#ifdef _AUX_WIDGET_C
#ifdef _AUX_WIDGET_IMPL
AFX_OBJECT(_auxWidget)
#else
AFX_OBJECT(afxWidget)
#endif
{
    _auxIddWid*         idd;
    _auxDdiWid const*   ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxLink parent;
    afxChain children;

    afxTransform local;
    afxM4d iw;

    afxResult(*updateCb)(afxWidget wid, afxReal dt);
    afxError(*renderCb)(afxWidget wid, afxRect const* area, afxDrawContext dctx);


    afxChain        commands;
    afxArena        cmdArena; // owned by dsys data for specific port
    _auxGuiElem*        currNode;
};
#endif//_AUX_WIDGET_C
#endif//_AUX_UX_C

AUX afxClassConfig const _AUX_WID_CLASS_CONFIG;

#endif//AUX_WIDGET_DDK_H
