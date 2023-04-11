#ifndef AFX_CANVAS2_H
#define AFX_CANVAS2_H

#include "afx/core/diag/afxDebug.h"
#include "afxWidgetDefs.h"
#include "../math/afxVector.h"

// canvas não deveria ser um frame incluindo o node para resolução matemática?

typedef enum afxAlign
{
    AFX_ALIGN_LEFT     = (1 << 0),
    AFX_ALIGN_CENTER   = (1 << 1),
    AFX_ALIGN_RIGHT    = (1 << 2),
    AFX_ALIGN_TOP      = (1 << 3),
    AFX_ALIGN_MIDDLE   = (1 << 4),
    AFX_ALIGN_BOTTOM   = (1 << 5),
} afxAlign;

AFXINL void AfxDeployRect(afxRect* rc) { AfxAssert(rc); AfxV2dFill(rc->offset, 0.f); AfxV2dFill(rc->extent, 1.f); };
AFXINL void AfxSetRectOrigin(afxRect* rc, afxV2d const offset) { AfxAssert2(rc, offset); AfxV2dCopy(rc->offset, offset); }
AFXINL void AfxSetRectExtent(afxRect* rc, afxV2d const extent) { AfxAssert2(rc, extent); AfxV2dCopy(rc->extent, extent); }

AFX_DEFINE_STRUCT(afxViewport2)
{
    afxV2d      origin, extent;
    afxV2d      absOrigin, absExtent; // product
    afxAlign    align;
};

AFXINL afxBool  AfxTestCanvas2Alignment(afxViewport2 const *canv, afxAlign align) { AfxAssert2(canv, align); return (canv->align & align); }
AFXINL void     AfxSetCanvas2Alignment(afxViewport2 *canv, afxAlign align) { AfxAssert2(canv, align); int a = canv->align; a |= (int)align; canv->align = (afxAlign)a; }
AFXINL void     AfxClearCanvas2Alignment(afxViewport2 *canv, afxAlign align) { AfxAssert2(canv, align); int a = canv->align; a &= ~(int)align; canv->align = (afxAlign)a; }

AFXINL void     AfxSetCanvas2(afxViewport2 *canv, afxV2d const origin, afxV2d const extent, afxAlign align) { AfxAssert3(canv, origin, extent); AfxV2dCopy(canv->origin, origin); AfxV2dCopy(canv->extent, extent); AfxSetCanvas2Alignment(canv, align); }

AFXINL void     AfxRepositionCanvas2(afxViewport2 *canv, afxV2d const origin) { AfxAssert2(canv, origin); AfxV2dCopy(canv->origin, origin); }

AFXINL void     AfxResizeCanvas2(afxViewport2 *canv, afxV2d const extent) { AfxAssert2(canv, extent); AfxV2dCopy(canv->extent, extent); }

#define AfxPackRect(_x_, _y_, _w_, _h_) (afxRect*){ (_x_), (_y_), (_w_), (_h_) }

#endif//AFX_CANVAS2_H