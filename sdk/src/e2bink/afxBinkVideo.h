#ifndef AFX_BINK_VIDEO_H
#define AFX_BINK_VIDEO_H

#include "../dep/bink/bink.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/io/afxRaster.h"
#include "qwadro/draw/io/afxBuffer.h"
#include "qwadro/core/afxSystem.h"

AFX_DEFINE_STRUCT(BINKFRAMETEXTURES)
{
    afxNat      sizes[4]; // Y, cR, cB, A.
    afxRaster  surfaces[4]; // Y, cR, cB, A.
};

AFX_DEFINE_STRUCT(BINKTEXTURESET)
{
    // this is the Bink info on the textures
    BINKFRAMEBUFFERS        bink_buffers;
    //avxCanvas               canv[BINKMAXFRAMEBUFFERS];
    // this is the GPU info for the textures
    BINKFRAMETEXTURES       rsrc[BINKMAXFRAMEBUFFERS];
};

AFX_DEFINE_STRUCT(afxBinkVideo)
{
    afxBool             running;
    afxDrawContext      dctx;
    afxModule           binkw32;
    void                *bik;
    afxNat              whd[3];
    
    BINKTEXTURESET      set; // Bink Buffer
    afxPipeline         yv12ToRgbaPip;
    afxSampler          smp;

    afxNat64            Last_timer;
    afxNat32            Frame_count;
    afxNat32            Bink_microseconds;
    afxNat32            Render_microseconds;
};

#ifdef _AFX_BINK_VIDEO_C
#define AFXBINK _AFXEXPORT
#else
#define AFXBINK _AFXIMPORT
#endif

AFXBINK afxError AfxBinkDeploy(afxBinkVideo *bnk, afxDrawContext dctx);
AFXBINK afxError AfxBinkDrop(afxBinkVideo *bnk);

AFXBINK afxError AfxBinkOpen(afxBinkVideo *bnk, afxUri const *uri);
AFXBINK afxError AfxBinkClose(afxBinkVideo *bnk);

AFXBINK afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip);
AFXBINK afxError AfxBinkBlitFrame(afxBinkVideo *bnk, afxDrawScript dscr);

#endif//AFX_BINK_VIDEO_H