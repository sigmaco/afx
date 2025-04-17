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

// Advanced User Experience Extensions for Qwadro

/*
    Widgets are smaller graphical elements or controls within a window that enable user interaction. 
    Widgets are used to allow users to interact with the system or application without directly manipulating the window. 
    They are often part of a GUI and provide specific functions, like displaying information or allowing user input.

    Their buttons allow the user to initiate an action. Their text boxes/fields enable users to input text.
    Their sliders are used to select a value from a range. Their checkboxes/radio buttons let users select options.
    Their labels display text or other information. Their menus provide a list of options or commands.
*/

/*
    This API is designed to be a hierarchical, container-based widget system, where all GUI elements live within a Widget, 
    which acts as:
        A panel (visual container),
        A manager (controls children),
        An allocator scope (for batching resources),
        A handle owner (so the user never touches raw nodes).

    Designed to be elegant, safe, and to scale well.
    It is basically a high-level object that acts like a mini-UI "scene."
*/

#ifndef AUX_WIDGET_H
#define AUX_WIDGET_H

#include "qwadro/inc/ux/afxUxDefs.h"
#include "qwadro/inc/draw/math/avxColor.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/math/afxTransform.h"

typedef enum afxWidgetFlag
{
    afxWidgetFlag_STATIONARY    = AFX_BIT(0),
    afxWidgetFlag_BORDERLESS    = AFX_BIT(1),
    afxWidgetFlag_MINIMIZABLE   = AFX_BIT(2),
    afxWidgetFlag_RESIZABLE     = AFX_BIT(3),
    afxWidgetFlag_UNSCROLLABLE  = AFX_BIT(4),
    afxWidgetFlag_SCALABLE      = AFX_BIT(5),
    afxWidgetFlag_UNTITLED      = AFX_BIT(6),
    afxWidgetFlag_SECONDARY     = AFX_BIT(7),
    afxWidgetFlag_INTANGIBLE    = AFX_BIT(8),
} afxWidgetFlags;

typedef enum afxWidgetType
{
    //NIL
    afxWidgetType_PANEL         = AFX_MAKE_FCC('w', 'p', 'a', 'n'),
    afxWidgetType_LABEL         = AFX_MAKE_FCC('w', 'l', 'a', 'b'),
    afxWidgetType_BUTTON        = AFX_MAKE_FCC('w', 'b', 'u', 't'),
    afxWidgetType_CHECKBOX      = AFX_MAKE_FCC('w', 'c', 'h', 'k'),
    afxWidgetType_EDITBOX       = AFX_MAKE_FCC('w', 'e', 'd', 't'),
    afxWidgetType_SLIDER        = AFX_MAKE_FCC('w', 's', 'l', 'd'),
    afxWidgetType_DROPDOWN      = AFX_MAKE_FCC('w', 'd', 'r', 'p'),
    afxWidgetType_PROGRESSBAR   = AFX_MAKE_FCC('w', 'p', 'r', 'o'),
    afxWidgetType_PICTURE       = AFX_MAKE_FCC('w', 'p', 'i', 'c'),
    afxWidgetType_VIDEO         = AFX_MAKE_FCC('w', 'v', 'i', 'd'),
    afxWidgetType_GROUP         = AFX_MAKE_FCC('w', 'g', 'r', 'p'),

    afxWidgetType_TYPE_TOTAL
} afxWidgetType;

AFX_DEFINE_STRUCT(afxWidgetStyle)
{
    afxRect     rect;
};

// screen overlay is a draw input device that has been idealized to be an widget layer.

AFX_DEFINE_STRUCT(afxWidgetVertex)
{
    afxV2d      xy, uv;
};

/*
    This layout engine can measure child sizes, arrange rows/columns with wrapping, respect margins and padding, and handle centering, stretching, shrinking.
*/

AFX_DEFINE_STRUCT(auxLayout)
{
    afxReal width, height;          // desired size (-1 for auto)
    afxReal minWidth, minHeight;
    afxReal maxWidth, maxHeight;

    afxV4d margin;              // top, right, bottom, left
    afxV4d padding;

    afxInt expandX : 1;
    afxInt expandY : 1;
    afxInt alignX;                   // 0=left, 1=center, 2=right
    afxInt alignY;                   // 0=top, 1=center, 2=bottom
};

AFX_DECLARE_STRUCT(afxWidgetImplementation);
AFX_DECLARE_STRUCT(afxWidgetImplementationData);

typedef afxResult(*afxWidgetDispatcher)(afxWidget,auxEvent const*);

AFX_DEFINE_STRUCT(afxWidgetConfig)
{
    afxRect         area;
    afxString       title;
    afxString       name;
    afxWidget       parent;
    afxUri const*   uri;
    afxResult       (*f)(afxWidget, afxUri const*, void *data);
};

AUX afxError AfxTestWidget(afxWidget wid);
AUX afxError AfxResetWidget(afxWidget wid);
AUX afxError AfxDoWidgetInput(afxWidget wid);
AUX afxError AfxDrawWidget(afxWidget wid, afxWhd const whd, afxDrawContext dctx);

AUX afxError AuxGuiAddCombo(afxWidget wid);
AUX afxError AuxGuiAddToggle(afxWidget wid);
AUX afxError AuxGuiAddSlider(afxWidget wid);

AUX afxError AuxGuiPushGroup(afxWidget wid);
AUX afxError AuxGuiPopGroup(afxWidget wid);

AUX afxError AuxGuiBeginPanel(afxWidget wid, afxRect area, afxFlags flags, afxString const name);
AUX afxError AuxGuiEndPanel(afxWidget wid);

////////////////////////////////////////////////////////////////////////////////

AUX afxError    AfxAcquireWidgets(afxWindow wnd, afxUnit cnt, afxWidgetConfig const cfg[], afxWidget widgets[]);

#endif//AUX_WIDGET_H
