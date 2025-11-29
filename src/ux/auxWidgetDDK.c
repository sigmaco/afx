/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_WIDGET_C
#include "auxIcd.h"

_AUX _auxGuiElem* _AuxWidPushGuiElem(afxWidget wid, afxUnit id, afxUnit siz, afxCmdId* elemId)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    AFX_ASSERT(siz >= sizeof(_auxGuiElemHdr));

    _auxGuiElem* cmd = AfxRequestArena(&wid->cmdArena, siz, 1, NIL, 0);
    AFX_ASSERT(cmd);
    cmd->hdr.id = id;
    cmd->hdr.siz = siz;
    AfxMakeChain(&cmd->hdr.children, cmd);
    AfxPushLink(&cmd->hdr.parent, NIL);
    AFX_ASSERT(elemId);
    *elemId = AfxPushLink(&cmd->hdr.script, &wid->commands);
    AFX_ASSERT(id != 268895528);
    return cmd;
}

_AUX afxCmdId AfxGuiCommencePanel(afxWidget wid, afxPanelInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(CommencePanel), sizeof(cmd->CommencePanel), &elemId);
    AFX_ASSERT(cmd);
    cmd->CommencePanel.info = *info;

    //wid->inPanel = TRUE;

    AFX_ASSERT(!wid->currNode);
    wid->currNode = cmd;

    return 1;
}

_AUX afxCmdId AfxGuiConcludePanel(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(ConcludePanel), sizeof(cmd->ConcludePanel), &elemId);
    AFX_ASSERT(cmd);
    cmd->ConcludePanel.nothing = NIL;


    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(CommencePanel));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);
    //wid->inPanel = FALSE;
    
    return 0;
}

_AUX afxCmdId AfxGuiPushLayout(afxWidget wid, auxLayoutDirection dir)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PushLayout), sizeof(cmd->PushLayout), &elemId);
    AFX_ASSERT(cmd);
    cmd->PushLayout.dir = dir;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);
    wid->currNode = cmd;

    return elemId;
}

_AUX afxCmdId AfxGuiPopLayout(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    //afxCmdId elemId;
    //_auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PopLayout), sizeof(cmd->PopLayout), &elemId);
    //AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(PushLayout));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);

    return 0;
}

_AUX afxCmdId AfxGuiPushGroup(afxWidget wid, afxString const* caption)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PushGroup), sizeof(cmd->PushGroup), &elemId);
    AFX_ASSERT(cmd);
    cmd->PushGroup.caption = *caption;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);
    wid->currNode = cmd;

    return elemId;
}

_AUX afxCmdId AfxGuiPopGroup(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);
    
    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(PopGroup), sizeof(cmd->PopGroup), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AFX_ASSERT(wid->currNode->hdr.id == _AUX_GUI_ELEM_ID(PushGroup));
    wid->currNode = AfxGetLinker(&wid->currNode->hdr.parent);

    return elemId;
}

_AUX afxCmdId AfxGuiButton(afxWidget wid, afxButtonInfo const* info, afxUnit* optionIdx)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Button), sizeof(cmd->Button), &elemId);
    AFX_ASSERT(cmd);
    cmd->Button.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiLabel(afxWidget wid, afxLabelInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Label), sizeof(cmd->Label), &elemId);
    AFX_ASSERT(cmd);
    cmd->Label.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiTextInputInlined(afxWidget wid, afxTextInputInfo const* info)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(TextInputInlined), sizeof(cmd->TextInputInlined), &elemId);
    AFX_ASSERT(cmd);
    cmd->TextInputInlined.info = *info;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiCheckbox(afxWidget wid, afxCheckboxInfo const* info, afxBool* checked)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Checkbox), sizeof(cmd->Checkbox), &elemId);
    AFX_ASSERT(cmd);
    cmd->Checkbox.info = *info;
    cmd->Checkbox.checked = checked;

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiDisable(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Disable), sizeof(cmd->Disable), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}

_AUX afxCmdId AfxGuiSeparator(afxWidget wid)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_WID, 1, &wid);

    afxCmdId elemId;
    _auxGuiElem* cmd = _AuxWidPushGuiElem(wid, _AUX_GUI_ELEM_ID(Separator), sizeof(cmd->Separator), &elemId);
    AFX_ASSERT(cmd);

    AFX_ASSERT(wid->currNode);
    AfxPushLink(&cmd->hdr.parent, &wid->currNode->hdr.children);

    return elemId;
}
