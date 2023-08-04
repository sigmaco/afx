#ifndef AFX_BINK_VIDEO_H
#define AFX_BINK_VIDEO_H

#include "../src/e2coree/deps/bink/bink.h"
#include "afx/mmux/afxMmuiDefs.h"
#include "afx/draw/res/afxTexture.h"
#include "afx/draw/res/afxBuffer.h"
#include "afx/draw/afxDrawSystem.h"

AFX_DEFINE_STRUCT(BINKFRAMETEXTURES)
{
    afxNat      sizes[4]; // Y, cR, cB, A.
    afxSurface  surfaces[4]; // Y, cR, cB, A.
};

AFX_DEFINE_STRUCT(BINKTEXTURESET)
{
    // this is the Bink info on the textures
    BINKFRAMEBUFFERS        bink_buffers;
    afxCanvas               canv[BINKMAXFRAMEBUFFERS];
    // this is the GPU info for the textures
    BINKFRAMETEXTURES       rsrc[BINKMAXFRAMEBUFFERS];
};

AFX_DEFINE_STRUCT(afxBinkVideo)
{
    afxDrawContext      dctx;
    afxModule           binkw32;
    void                *bik;
    afxNat              whd[3];
    
    BINKTEXTURESET      set; // Bink Buffer
    afxPipeline         yv12ToRgbaPip;
    afxSampler          smp;
    afxVertexBuffer     vbuf;

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

AFXBINK afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip, afxNat outBufIdx, afxDrawScript dscr, afxCanvas canv, afxSurface surf);
AFXBINK afxError AfxBinkBlitFrame(afxBinkVideo *bnk, afxDrawScript dscr, afxCanvas canv, afxSurface surf);

#endif//AFX_BINK_VIDEO_H