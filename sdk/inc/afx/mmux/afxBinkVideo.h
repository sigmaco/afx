#ifndef AFX_BINK_VIDEO_H
#define AFX_BINK_VIDEO_H

#include "../src/e2coree/deps/bink/bink.h"
#include "afxMmuiDefs.h"
#include "afx/draw/res/afxTexture.h"
#include "afx/draw/res/afxBuffer.h"
#include "afx/draw/afxDrawSystem.h"

AFX_DEFINE_STRUCT(BINKFRAMETEXTURES)
{
    afxNat32                Ysize;
    afxNat32                cRsize;
    afxNat32                cBsize;
    afxNat32                Asize;
    afxLego    lego;
    afxSurface              Ytexture;
    afxSurface              cRtexture;
    afxSurface              cBtexture;
    afxSurface              Atexture;
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
    afxPipeline         pip; // actually two for experiments
    afxSampler          smp;

    afxNat64            Last_timer;
    afxNat32            Frame_count;
    afxNat32            Bink_microseconds;
    afxNat32            Render_microseconds;
};

AFX afxError AfxBinkDeploy(afxBinkVideo *bnk, afxDrawContext dctx);
AFX afxError AfxBinkDrop(afxBinkVideo *bnk);

AFX afxError AfxBinkOpen(afxBinkVideo *bnk, afxUri const *uri);
AFX afxError AfxBinkClose(afxBinkVideo *bnk);

AFX afxError AfxBinkDoFrame(afxBinkVideo *bnk, afxBool copyAll, afxBool neverSkip);
AFX afxError AfxBinkBlitFrame(afxBinkVideo *bnk, afxDrawScript dscr, afxCanvas canv, afxSurface surf);

#endif//AFX_BINK_VIDEO_H