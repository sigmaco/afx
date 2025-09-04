/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ARX_SKELETON_C
#define _ARX_MODEL_C
#define _ARX_MESH_C
#define _ARX_MESH_TOPOLOGY_C
//#define _AVX_GEOMETRY_C
#define _ASX_SIMULATION_C
#include "../../draw/ddi/avxImplementation.h"
#include "../ddi/arxImpl_Input.h"

#pragma push(pack, 1)

AFX_DEFINE_STRUCT(M4D_MSHS_HDR)
// SIGMA FEDERATION, SERIALIZED MESH SECTION HEADER
{
    arxMeshSectionFlags flags;
    afxUnit32 mtlIdx;
    afxUnit32 baseTriIdx;
    afxUnit32 triCnt;
};

AFX_DEFINE_STRUCT(M4D_MSHV_HDR)
// SIGMA FEDERATION, SERIALIZED MESH BIAS HEADER
{
    arxMeshBiasFlags flags;
    afxUnit trisBaseOff;
    afxUnit32 triCnt;
};

AFX_DEFINE_STRUCT(M4D_MSHM_HDR)
// SIGMA FEDERATION, SERIALIZED MESH MORPH HEADER
{
    arxMeshMorphFlags flags;
    afxUnit morphedAttrCnt;
    afxMask32 morphedAttrs;
    afxUnit baseVtx;
};

AFX_DEFINE_STRUCT(M4D_VTX_ATTR_HDR)
// SIGMA FEDERATION, SERIALIZED VERTEX ATTRIBUTE HEADER
{
    arxVertexFlags  flags;
    afxUnit32       baseDataOffset;
    avxFormat       encodedFmt;
    avxFormat       decodedFmt;
};

AFX_DEFINE_STRUCT(M4D_MSH_HDR)
// SIGMA FEDERATION, SERIALIZED 4D MESH HEADER
{
    arxMeshFlags    flags;
    avxTopology     topology; // actually only TRILIST
    afxUnit         triCnt; // count of primitives.
    afxUnit32       sideToAdjacentMapBaseOff; // [edgeCnt]
    afxUnit         mtlCnt; // used by sections
    afxUnit32       mtlIdsBaseOff; // [mtlCnt]
    afxUnit         secCnt;
    afxUnit32       sectionsBaseOff; // [secCnt]
    afxUnit         biasCnt;
    afxUnit32       biasesBaseOff;
    // nested bias identifier strings for fast lookup.
    afxUnit32       biasIdsBaseOff;
    afxUnit         jointsForTriCnt;
    afxUnit32       jointsForTriMapBaseOff;
    afxUnit         triToJointCnt;
    afxUnit32       triToJointMapBaseOff;

    // VERTEX DATA
    afxUnit         vtxCnt;
    afxUnit32       vtxToVtxMapBaseOff; // [vtxCnt]
    afxUnit32       vtxToTriMapBaseOff; // [vtxCnt]
    afxUnit         minIdxSiz;
    afxUnit         idxCnt; // count of primitive indices.
    afxUnit32       indicesBaseOff; // [idxCnt] --- indices into primitive vertices.
    afxUnit         attrCnt; // used by morphes.
    afxUnit32       vtxAttrIdsBaseOff;
    afxUnit32       vtxAttrInfoBaseOff;

    // SHAPE DATA
    afxUnit         morphCnt;
    afxUnit32       morphsBaseOff;
    // nested section AABB for fast lookup
    afxUnit32       secAabbBaseOff; // [morphCnt][secCnt]
    // nested bias OBB for fast lookup.
    afxUnit32       biasObbBaseOff; // [morphCnt][biasCnt]
    // nested morph tags for fast lookup.
    afxUnit32       morphTagsBaseOff; // [morphCnt]

};

AFX_DEFINE_STRUCT(M4D_MDL_HDR)
// SIGMA FEDERATION, SERIALIZED 4D MODEL HEADER
{
    afxUnit8        fcc[4];
    afxUnit32       hdrSiz;
    afxUnit32       hdrVer;
    afxUnit32       segCnt;
    afxUnit32       segStart;
    afxUnit32       sdbSiz;
    afxUnit32       sdbStart;

    afxM3d          basis;
    afxV3d          origin;
    afxReal         unitsPerMeter;

    afxUnit32       flags;
    afxUnit32       lodType;
    afxUnit32       jntCnt;
    afxUnit32       jntPiBase;
    afxUnit32       jntLtBase;
    afxUnit32       jntIwBase;
    afxUnit32       jntLeBase;
    afxUnit32       jntIdBase;
    afxUnit32       jntUddBase;
    afxTransform    displacement;
    afxUnit32       mshCnt;
    afxUnit32       mshIdBase;
    afxUnit32       mshInfoBase;
    afxUnit32       rigCnt;
    afxUnit32       rigBase;

    // look for joint extensions
    // look for rig extensions
};

AFX_DEFINE_STRUCT(M4D_MSHR_HDR)
// SIGMA FEDERATION, MODEL-MESH RIG HEADER
{
    afxUnit32       mshIdx;
    afxUnit32       flags;
    // transplanted skeleton is a runtime thing
};

#pragma pop(pack)

_ASX afxUnit ArxMeasureNestedStrings(afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    AFX_ASSERT(strings);
    afxUnit totalSiz = 0;
    
    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    } sub;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];
        totalSiz += s->len;
    }
    totalSiz += sizeof(sub) * cnt;
    return totalSiz;
}

_ASX afxError ArxWriteNestedStrings(afxStream out, afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(strings);

    afxUnit siz = ArxMeasureNestedStrings(cnt, strings);

    urdMark mark = { 0 };
    mark.fcc = afxChunkId_STRING;
    mark.siz = siz;

    if (AfxWriteStream(out, sizeof(mark), 0, &mark))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];

        if (AfxWriteStream(out, s->len, 0, s->start))
        {
            AfxThrowError();
            return err;
        }
    }

    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    }
    d =
    {
        .start = 0,
        .len = 0
    };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];
        d.len = s->len;

        if (AfxWriteStream(out, sizeof(d), 0, &d))
        {
            AfxThrowError();
            return err;
        }
        d.start += d.len;
    }
    return err;
}

_ASX afxError ArxReadNestedStrings(afxStream in, afxUnit cnt, afxString strings[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(strings);

    urdMark hdr2;
    if (afxChunkId_STRING != AfxPullStreamChunk(in, &hdr2))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT(4096 >= hdr2.siz);
    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);
    AfxReadString(&sdb.s, in, hdr2.siz);

    struct
    {
        afxUnit32 start;
        afxUnit32 len;
    } d;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];

        if (AfxReadStream(in, sizeof(d), 0, &d))
        {
            AfxThrowError();
            break;
        }
        *s = AfxExcerptString(&sdb.s, d.start, d.len);
    }
    return err;
}

_ASX afxError ArxWriteMappedStrings(afxStream out, afxString* sdb, afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(strings);

    struct
    {
        afxUnit32 len;
        afxUnit32 start;
    }
    hdr = { 0 };

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* s = &strings[i];

        hdr.start = sdb->len;
        hdr.len = s->len;

        if (AfxCatenateStrings(sdb, 1, s))
            AfxThrowError();

        if (AfxWriteStream(out, sizeof(hdr), 0, &hdr))
        {
            AfxThrowError();
        }
    }
    return err;
}

_ASX afxError ArxReadMappedStrings(afxStream in, afxString const* sdb, afxUnit cnt, afxString strings[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(strings);

    struct
    {
        afxUnit32 len;
        afxUnit32 start;
    } hdr;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString* s = &strings[i];

        if (AfxReadStream(in, sizeof(hdr), 0, &hdr))
        {
            AfxResetStrings(1, s);
            AfxThrowError();
        }
        else
        {
            *s = AfxExcerptString(sdb, hdr.start, hdr.len);
        }
    }
    return err;
}

_ASX afxError ArxArchiveMeshes(afxStream out, afxString* sdb, afxUnit cnt, arxMesh meshes[])
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_MSH, cnt, meshes);
    afxUnit mshIdx = 0;

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());
    M4D_MSH_HDR* mshHdrs = AfxRequestFromArena(&arena, sizeof(mshHdrs[0]), cnt, NIL, 0);

    afxSize mshHdrOffBkp = AfxAskStreamPosn(out);
    // skip the room to the headers
    AfxAdvanceStream(out, cnt * sizeof(M4D_MSH_HDR));
    afxSize endOfHdrs = AfxAskStreamPosn(out);

    do
    {
        arxMesh msh = meshes[mshIdx];
        
        M4D_MSH_HDR mshHdr = { 0 };
#if 0
        mshHdr.fcc[0] = 'm';
        mshHdr.fcc[1] = 's';
        mshHdr.fcc[2] = 'h';
        mshHdr.fcc[3] = '\0';
        mshHdr.hdrSiz = sizeof(mshHdr) - sizeof(urdMark);
        AfxDumpString(&msh->urn, 0, AFX_MIN(32, msh->urn.len), mshHdr.urn);
#endif
        mshHdr.flags = msh->flags;
        mshHdr.topology = msh->topology;
        mshHdr.triCnt = msh->triCnt;
        mshHdr.vtxCnt = msh->vtxCnt;
        mshHdr.morphCnt = msh->morphCnt;
        mshHdr.mtlCnt = msh->mtlCnt;
        mshHdr.secCnt = msh->secCnt;
        mshHdr.biasCnt = msh->biasCnt;
        mshHdr.idxCnt = msh->idxCnt;
        mshHdr.attrCnt = msh->attrCnt;

        // WRITE TOPOLOGY DATA

        if (msh->sideToAdjacentMap)
        {
            mshHdr.sideToAdjacentMapBaseOff = AfxAskStreamPosn(out);

            if (AfxWriteStream(out, msh->triCnt * sizeof(msh->sideToAdjacentMap[0]), 0, msh->sideToAdjacentMap))
                AfxThrowError();
        }

        // write materials' identifier strings.

        mshHdr.mtlIdsBaseOff = AfxAskStreamPosn(out);
        ArxWriteMappedStrings(out, sdb, msh->mtlCnt, msh->materials);

        // write sections' descrptor structures.

        mshHdr.sectionsBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.secCnt; i++)
        {
            arxMeshSection* sec = &msh->sections[i];
            M4D_MSHS_HDR mshs;
            mshs.baseTriIdx = sec->baseTriIdx;
            mshs.triCnt = sec->triCnt;
            mshs.flags = sec->flags;
            mshs.mtlIdx = sec->mtlIdx;

            if (AfxWriteStream(out, sizeof(mshs), 0, &mshs))
                AfxThrowError();
        }

        // write biases' descriptor structures

        mshHdr.biasesBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.biasCnt; i++)
        {
            arxMeshBias* v = &msh->biases[i];
            M4D_MSHV_HDR mshv;
            mshv.triCnt = v->triCnt;
            mshv.flags = v->flags;

            if (AfxWriteStream(out, sizeof(mshv), 0, &mshv))
                AfxThrowError();
        }

        // write biases' identifier strings.

        mshHdr.biasIdsBaseOff = AfxAskStreamPosn(out);
        ArxWriteMappedStrings(out, sdb, msh->biasCnt, msh->biasId);

        if (msh->jointsForTriMap)
        {
            mshHdr.jointsForTriMapBaseOff = AfxAskStreamPosn(out);
            AfxWriteStream(out, msh->jointsForTriCnt * sizeof(msh->jointsForTriMap[0]), 0, msh->jointsForTriMap);
        }

        if (msh->triToJointMap)
        {
            mshHdr.triToJointMapBaseOff = AfxAskStreamPosn(out);
            AfxWriteStream(out, msh->triToJointCnt * sizeof(msh->triToJointMap[0]), 0, msh->triToJointMap);
        }

        // WRITE VERTEX DATA

        if (msh->vtxToVtxMap)
        {
            mshHdr.vtxToVtxMapBaseOff = AfxAskStreamPosn(out);
            AfxWriteStream(out, msh->vtxCnt * sizeof(msh->vtxToVtxMap), 0, msh->vtxToVtxMap);
        }

        if (msh->vtxToTriMap)
        {
            mshHdr.vtxToTriMapBaseOff = AfxAskStreamPosn(out);
            AfxWriteStream(out, msh->vtxCnt * sizeof(msh->vtxToTriMap[0]), 0, msh->vtxToTriMap);
        }

        mshHdr.indicesBaseOff = AfxAskStreamPosn(out);
        AfxWriteStream(out, msh->idxCnt * sizeof(msh->indices[0]), 0, msh->indices);

        // write attributes' identifier strings.

        mshHdr.vtxAttrIdsBaseOff = AfxAskStreamPosn(out);
        ArxWriteMappedStrings(out, sdb, msh->attrCnt, msh->attrIds);

        // write attributes' info and data

        M4D_VTX_ATTR_HDR msvas[16] = { 0 };
        mshHdr.vtxAttrInfoBaseOff = AfxAskStreamPosn(out);
        AfxAdvanceStream(out, sizeof(msvas[0]) * msh->attrCnt);

        for (afxUnit i = 0; i < msh->attrCnt; i++)
        {
            msvas[i].encodedFmt = msh->attrInfo[i].fmt;
            msvas[i].decodedFmt = msh->attrInfo[i].fmt;
            msvas[i].flags = msh->attrInfo[i].flags;

            avxFormatDescription pfd;
            AvxDescribeFormats(1, &msvas[i].encodedFmt, &pfd);

            afxSize fmtSiz = pfd.stride;// AfxVertexFormatGetSize(msvas[i].encodedFmt);
            msvas[i].baseDataOffset = AfxAskStreamPosn(out);
            AfxWriteStream(out, fmtSiz * msh->vtxCnt * msh->morphCnt, 0, msh->vtxAttrData[i]);
        }

        afxSize bkpPos = AfxAskStreamPosn(out);
        AfxSeekStream(out, mshHdr.vtxAttrInfoBaseOff, afxSeekOrigin_BEGIN);
        AfxWriteStream(out, sizeof(msvas[0]) * msh->attrCnt, 0, msvas);
        AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

        // SHAPE DATA

        mshHdr.morphsBaseOff = AfxAskStreamPosn(out);

        for (afxUnit i = 0; i < mshHdr.morphCnt; i++)
        {
            arxMeshMorph* m = &msh->morphs[i];
            M4D_MSHM_HDR mshm;
            mshm.flags = m->flags;
            mshm.baseVtx = m->baseVtx;
            mshm.morphedAttrs = m->affectedAttrs;
            mshm.morphedAttrCnt = m->affectedAttrCnt;

            if (AfxWriteStream(out, sizeof(mshm), 0, &mshm))
                AfxThrowError();
        }

        mshHdr.secAabbBaseOff = AfxAskStreamPosn(out);
        AfxWriteStream(out, msh->morphCnt * msh->secCnt * sizeof(msh->secAabb[0]), 0, msh->secAabb);

        mshHdr.biasObbBaseOff = AfxAskStreamPosn(out);
        AfxWriteStream(out, msh->morphCnt * msh->biasCnt * sizeof(msh->biasObb[0]), 0, msh->biasObb);

        mshHdr.morphTagsBaseOff = AfxAskStreamPosn(out);
        ArxWriteMappedStrings(out, sdb, msh->morphCnt, msh->morphTags);

        // write mesh extensions
        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        AfxWriteStream(out, sizeof(extMark), 0, &extMark);

        // write mesh info
        //mshHdr.hdrSiz = AfxAskStreamPosn(out) - mshHdrOffBkp - sizeof(urdMark);

        mshHdrs[mshIdx] = mshHdr;

    } while (!err && (++mshIdx < cnt));

    // write out all headers
    afxUnit bkpPos = AfxAskStreamPosn(out);
    AfxSeekStream(out, mshHdrOffBkp, afxSeekOrigin_BEGIN);
    if (AfxWriteStream(out, cnt * sizeof(mshHdrs[0]), 0, mshHdrs))
        AfxThrowError();
    AFX_ASSERT(endOfHdrs == AfxAskStreamPosn(out));

    AfxSeekStream(out, bkpPos, afxSeekOrigin_BEGIN);

    AfxDismantleArena(&arena);

    return err;
}

_ASX afxError ArxDownloadModel(arxModel mdl, afxStream out)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit mdlIdx = 0;

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxUnit uniqueMeshCnt = 0;
    arxMesh meshes[256];

    M4D_MDL_HDR mdlHdr = { 0 };
    mdlHdr.fcc[0] = 'm';
    mdlHdr.fcc[1] = '4';
    mdlHdr.fcc[2] = 'd';
    mdlHdr.fcc[3] = '\0';
    mdlHdr.hdrSiz = sizeof(mdlHdr) - sizeof(urdMark);
    AfxM3dReset(mdlHdr.basis);
    AfxV3dZero(mdlHdr.origin);
    mdlHdr.unitsPerMeter = 1.f;
    mdlHdr.lodType = mdl->lodType;
    mdlHdr.jntCnt = mdl->jntCnt;
    mdlHdr.flags = mdl->flags;
    mdlHdr.displacement = mdl->displace;
    mdlHdr.rigCnt = mdl->rigCnt;

    afxSize hdrPosBkp = AfxAskStreamPosn(out);
    AfxAdvanceStream(out, sizeof(mdlHdr));

    // write the skeleton
    {
        // write joints' parent indices

        mdlHdr.jntPiBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.jntCnt * sizeof(afxUnit32), 0, mdl->jntPi))
            AfxThrowError();

        // write joints' local transforms

        mdlHdr.jntLtBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.jntCnt * sizeof(afxTransform), 0, mdl->jntLt))
            AfxThrowError();

        // write joints' inverse world matrices

        mdlHdr.jntIwBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.jntCnt * sizeof(afxM4d), 0, mdl->jntIw))
            AfxThrowError();

        // write joints' LOD error values

        mdlHdr.jntLeBase = AfxAskStreamPosn(out);
        if (AfxWriteStream(out, mdlHdr.jntCnt * sizeof(afxReal32), 0, mdl->jntLe))
            AfxThrowError();

        // write joints' identifier strings (to string database)
        mdlHdr.jntIdBase = AfxAskStreamPosn(out);

        if (ArxWriteMappedStrings(out, &sdb.s, mdlHdr.jntCnt, mdl->jntId))
            AfxThrowError();
    }

    mdlHdr.mshIdBase = AfxAskStreamPosn(out);

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        _arxRiggedMesh* rig = mdl->rigs[i];
        if (!rig) continue;
        arxMesh msh = rig->msh;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        if (msh)
        {
            afxBool skip = FALSE;
            for (afxUnit j = 0; j < i; j++)
            {
                _arxRiggedMesh* prevRig = mdl->rigs[j];

                // must skip this mesh if this has already been included before.
                if (!prevRig || (prevRig->msh == msh))
                {
                    skip = TRUE;
                    break;
                }
            }

            if (skip) continue;  // skip mesh if it has already been included before.
            else
            {
                meshes[uniqueMeshCnt++] = msh;
                ArxWriteMappedStrings(out, &sdb.s, 1, &msh->urn);
            }
        }
    }

    mdlHdr.mshCnt = uniqueMeshCnt;
    mdlHdr.rigBase = AfxAskStreamPosn(out);

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        _arxRiggedMesh* rig = mdl->rigs[i];
        if (!rig) continue;
        arxMesh msh = rig->msh;
        AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

        if (msh)
        {
            afxUnit mshIdx = i;

            for (afxUnit j = 0; j < i; j++)
            {
                _arxRiggedMesh* prevRig = mdl->rigs[j];

                // reuse mesh already included before.
                if (prevRig && (prevRig->msh == msh))
                {
                    mshIdx = j;
                    break;
                }
            }

            M4D_MSHR_HDR rigi = { 0 };
            rigi.flags = rig->flags;
            rigi.mshIdx = mshIdx;

            if (AfxWriteStream(out, sizeof(rigi), 0, &rigi))
                AfxThrowError();
        }
    }

    mdlHdr.mshInfoBase = AfxAskStreamPosn(out);

    if (ArxArchiveMeshes(out, &sdb.s, uniqueMeshCnt, meshes))
        AfxThrowError();

    {
        // write model extensions
        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        AfxWriteStream(out, sizeof(extMark), 0, &extMark);
    }

    {
        // write string database
        mdlHdr.sdbStart = AfxAskStreamPosn(out);
        mdlHdr.sdbSiz = sdb.s.len;

        if (AfxWriteStream(out, mdlHdr.sdbSiz, 0, sdb.s.start))
        {
            AfxThrowError();
            return err;
        }
    }

    // write model header
    mdlHdr.hdrSiz = AfxAskStreamPosn(out) - hdrPosBkp - sizeof(urdMark);
    
    if (AfxWriteStreamAt(out, hdrPosBkp, sizeof(mdlHdr), 0, &mdlHdr))
        AfxThrowError();

    AfxAdvanceStream(out, mdlHdr.hdrSiz);

    return err;
}

_ASX afxError ArxArchiveModel(arxModel mdl, afxUri const* uri)
{
    afxError err = NIL;
    afxStream iob;
    afxStreamInfo info = {0};
    info.flags = afxIoFlag_WRITEABLE;
    info.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &info, &iob);

    AfxReopenFile(iob, uri, afxFileFlag_W);
    ArxDownloadModel(mdl, iob);
    AfxDisposeObjects(1, &iob);

    return err;
}

_ASX afxError ArxUploadModel(arxGeome morp, afxString const* urn, afxStream in, arxModel* model)
{
    afxError err = NIL;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(model);

    // Qwadro doesn't use afxChunkID_STRUCT

    afxArena arena;
    AfxMakeArena(&arena, NIL, AfxHere());

#pragma push(pack, 1)
    M4D_MDL_HDR mdlHdr;
#pragma pop(pack)

    afxString4096 sdb;
    AfxMakeString4096(&sdb, NIL);

    afxString strings[256];
    arxMesh meshes[256];
    arxModelFlags rigFlags[256];

    if (AfxReadStream(in, sizeof(mdlHdr), 0, &mdlHdr))
        AfxThrowError();

    afxSize posBkp = AfxAskStreamPosn(in);
    AfxSeekStream(in, mdlHdr.sdbStart, afxSeekOrigin_BEGIN);
    AfxReadString(&sdb.s, in, mdlHdr.sdbSiz);

    AfxSeekStream(in, mdlHdr.jntIdBase, afxSeekOrigin_BEGIN);
    ArxReadMappedStrings(in, &sdb.s, mdlHdr.jntCnt, strings);

    arxModel mdl;
    arxModelBlueprint mdlb = { 0 };
    mdlb.jointCnt = mdlHdr.jntCnt;
    mdlb.joints = strings;
    mdlb.lodType = mdlHdr.lodType;
    mdlb.rigCnt = mdlHdr.rigCnt;
    mdlb.displace = mdlHdr.displacement;
    AfxMakeString32(&mdlb.urn, urn);
    ArxAssembleModels(morp, 1, &mdlb, &mdl);

    {
        afxUnit32* parentIdx = AfxRequestFromArena(&arena, sizeof(afxUnit32), mdlHdr.jntCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntPiBase, mdlHdr.jntCnt * sizeof(parentIdx[0]), 0, parentIdx))
            AfxThrowError();

        ArxReparentJoints(mdl, 0, mdlb.jointCnt, parentIdx, sizeof(parentIdx[0]));

        afxTransform* local = AfxRequestFromArena(&arena, sizeof(afxTransform), mdlHdr.jntCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntLtBase, mdlHdr.jntCnt * sizeof(local[0]), 0, local))
            AfxThrowError();

        ArxResetJointTransforms(mdl, 0, mdlb.jointCnt, local);

        afxM4d* iw = AfxRequestFromArena(&arena, sizeof(afxM4d), mdlHdr.jntCnt, NIL, 0);
        AfxSeekStream(in, mdlHdr.jntIwBase, afxSeekOrigin_BEGIN);
        if (AfxReadStreamAt(in, mdlHdr.jntIwBase, mdlHdr.jntCnt * sizeof(iw[0]), 0, iw))
            AfxThrowError();

        ArxResetJointInversors(mdl, 0, mdlb.jointCnt, iw, sizeof(iw[0]));

        afxReal* lodErr = AfxRequestFromArena(&arena, sizeof(afxReal32), mdlHdr.jntCnt, NIL, 0);
        if (AfxReadStreamAt(in, mdlHdr.jntLeBase, mdlHdr.jntCnt * sizeof(lodErr[0]), 0, lodErr))
            AfxThrowError();

        ArxResetJointLodErrors(mdl, 0, mdlb.jointCnt, lodErr);
    }

    AfxSeekStream(in, mdlHdr.mshIdBase, afxSeekOrigin_BEGIN);

    if (ArxReadMappedStrings(in, &sdb.s, mdlHdr.mshCnt, strings))
        AfxThrowError();

    AfxSeekStream(in, mdlHdr.mshInfoBase, afxSeekOrigin_BEGIN);

    arxMeshBlueprint mshb = { 0 };
    
    M4D_MSH_HDR* mshHdrs = AfxRequestFromArena(&arena, sizeof(mshHdrs[0]), mdlHdr.mshCnt, NIL, 0);

    if (AfxReadStream(in, mdlHdr.mshCnt * sizeof(M4D_MSH_HDR), 0, mshHdrs))
        AfxThrowError();

    arxMeshBlueprint* mshbs = AfxRequestFromArena(&arena, sizeof(mshbs[0]), mdlHdr.mshCnt, NIL, 0);

    for (afxUnit i = 0; i < mdlHdr.mshCnt; i++)
    {
        M4D_MSH_HDR* mshHdr = &mshHdrs[i];
        arxMeshBlueprint* mshb = &mshbs[i];
        AfxZero(mshb, sizeof(mshb[0]));

        afxString* attrs = AfxRequestFromArena(&arena, sizeof(attrs[0]), mshHdr->attrCnt, NIL, 0);
        AfxSeekStream(in, mshHdr->vtxAttrIdsBaseOff, afxSeekOrigin_BEGIN);
        ArxReadMappedStrings(in, &sdb.s, mshHdr->attrCnt, attrs);
        mshb->attrCnt = mshHdr->attrCnt;
        mshb->attrs = attrs;

        afxString* biases = AfxRequestFromArena(&arena, sizeof(biases[0]), mshHdr->biasCnt, NIL, 0);
        AfxSeekStream(in, mshHdr->biasIdsBaseOff, afxSeekOrigin_BEGIN);
        ArxReadMappedStrings(in, &sdb.s, mshHdr->biasCnt, biases);
        mshb->biasCnt = mshHdr->biasCnt;
        mshb->biases = biases;

        afxString* morphTags = AfxRequestFromArena(&arena, sizeof(morphTags[0]), mshHdr->morphCnt, NIL, 0);
        AfxSeekStream(in, mshHdr->morphTagsBaseOff, afxSeekOrigin_BEGIN);
        ArxReadMappedStrings(in, &sdb.s, mshHdr->morphCnt, morphTags);
        mshb->morphCnt = mshHdr->morphCnt;
        mshb->morphTags = morphTags;

        afxString* materials = AfxRequestFromArena(&arena, sizeof(materials[0]), mshHdr->mtlCnt, NIL, 0);
        AfxSeekStream(in, mshHdr->mtlIdsBaseOff, afxSeekOrigin_BEGIN);
        ArxReadMappedStrings(in, &sdb.s, mshHdr->mtlCnt, materials);
        mshb->materials = materials;

        mshb->mtlCnt = mshHdr->mtlCnt;
        mshb->secCnt = mshHdr->secCnt;
        mshb->triCnt = mshHdr->triCnt;
        mshb->vtxCnt = mshHdr->vtxCnt;


    }

    ArxBuildMeshes(morp, mdlHdr.mshCnt, mshbs, meshes);

    for (afxUnit i = 0; i < mdlHdr.mshCnt; i++)
    {
        M4D_MSH_HDR* mshHdr = &mshHdrs[i];
        arxMesh msh = meshes[i];

        AfxSeekStream(in, mshHdr->indicesBaseOff, afxSeekOrigin_BEGIN);
        ArxUploadMeshIndices(msh, 0, mshHdr->triCnt, in, sizeof(afxUnit32));

        for (afxUnit j = 0; j < mshHdr->attrCnt; j++)
        {
            AfxSeekStream(in, mshHdr->vtxAttrInfoBaseOff + j * sizeof(M4D_VTX_ATTR_HDR), afxSeekOrigin_BEGIN);
            M4D_VTX_ATTR_HDR vaHdr;
            AfxReadStream(in, sizeof(vaHdr), 0, &vaHdr);

            ArxFormatVertexAttribute(msh, j, vaHdr.decodedFmt, vaHdr.flags, NIL);
            AfxSeekStream(in, vaHdr.baseDataOffset, afxSeekOrigin_BEGIN);

            for (afxUnit k = 0; k < mshHdr->morphCnt; k++)
            {
                avxFormatDescription pfd;
                AvxDescribeFormats(1, &vaHdr.encodedFmt, &pfd);
                afxUnit stride = pfd.stride;// AfxVertexFormatGetSize(vaHdr.encodedFmt);
                ArxUploadVertexData(msh, j, k, 0, mshHdr->vtxCnt, in, stride);
            }
        }

        AfxSeekStream(in, mshHdr->sectionsBaseOff, afxSeekOrigin_BEGIN);

        for (afxUnit k = 0; k < mshHdr->secCnt; k++)
        {
            M4D_MSHS_HDR mshsHdr;
            AfxReadStream(in, sizeof(mshsHdr), 0, &mshsHdr);
            arxMeshSection mshs = { 0 };
            mshs.baseTriIdx = mshsHdr.baseTriIdx;
            mshs.triCnt = mshsHdr.triCnt;
            mshs.mtlIdx = mshsHdr.mtlIdx;
            mshs.flags = mshsHdr.flags;
            ArxSectionizeMesh(msh, k, 1, &mshs);
        }

        AfxSeekStream(in, mshHdr->morphsBaseOff, afxSeekOrigin_BEGIN);

        for (afxUnit k = 0; k < mshHdr->morphCnt; k++)
        {
            M4D_MSHM_HDR mshmHdr;
            AfxReadStream(in, sizeof(mshmHdr), 0, &mshmHdr);
            arxMeshMorph mshm = { 0 };
            mshm.affectedAttrs = mshmHdr.morphedAttrs;
            mshm.flags = mshmHdr.flags;
            ArxReformMesh(msh, k, 1, &mshm);
        }
    }

    AfxSeekStream(in, mdlHdr.rigBase, afxSeekOrigin_BEGIN);

    for (afxUnit i = 0; i < mdlHdr.rigCnt; i++)
    {
        M4D_MSHR_HDR mshrHdr;
        AfxReadStream(in, sizeof(mshrHdr), 0, &mshrHdr);

        ArxRigMeshes(mdl, NIL, i, 1, &meshes[mshrHdr.mshIdx]);

        _arxRiggedMesh* rig = mdl->rigs[i];
        if (!rig) continue;
        rig->flags |= mshrHdr.flags;
    }

    AfxDismantleArena(&arena);

    *model = mdl;

    return err;
}

_ASX afxError ArxLoadModel(arxGeome morp, afxString const* urn, afxUri const* uri, arxModel* model)
{
    afxError err = NIL;
    afxStream iob;
    afxStreamInfo info = { 0 };
    info.flags = afxIoFlag_READABLE;
    info.usage = afxIoUsage_FILE;
    AfxAcquireStream(1, &info, &iob);

    AfxReopenFile(iob, uri, afxFileFlag_R);
    ArxUploadModel(morp, urn, iob, model);
    AfxDisposeObjects(1, &iob);

    return err;
}
