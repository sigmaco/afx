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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AMX_SKELETON_C
#define _AMX_MODEL_C
#define _AVX_MESH_C
#include "../dev/AmxPuppetImplKit.h"
#include "../dev/AmxCadImplKit.h"
#include "../dev/AmxImplKit.h"

_AMX afxUnit AfxMeasureNestedStrings(afxUnit cnt, afxString const strings[])
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
    totalSiz += sizeof(urdMark);
    return totalSiz;
}

_AMX afxError AfxWriteNestedStrings(afxStream out, afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AFX_ASSERT(strings);

    afxUnit siz = AfxMeasureNestedStrings(cnt, strings);

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

_AMX afxError AfxReadNestedStrings(afxStream in, afxUnit cnt, afxString strings[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    AfxReadString(&sdb.str, in, hdr2.siz);

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
        AfxExcerptString(&sdb.str, d.start, d.len, s);
    }
    return err;
}

_AMX afxError AfxStoreSkeletons(afxStream out, afxUnit cnt, afxSkeleton skeletons[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssertObjects(cnt, skeletons, afxFcc_SKL);
    afxUnit sklIdx = 0;

    do
    {
        afxSkeleton skl = skeletons[sklIdx];
        afxUnit jointCnt = skl->jointCnt;

        urdMark mark = { 0 };
        mark.fcc = afxChunkId_QW_SKL;

        /*
            HEADER
            -------
            u32     flags
            u32     lodType
            c8      urn[32]
            u32     jointCnt
            xfrm    locals[jointCnt]
            m4dc    matrices[jointCnt]
            u32     parents[jointCnt]
            f32     lodErrors[jointCnt]
            c8      tags[jointCnt]
            -------
            extensions
        */
#pragma push(pack, 1)
        struct
        {
            afxChar8    urn[32];
            afxUnit32   flags;
            afxUnit32   lodType;
            afxUnit32   jointCnt;
        }
#pragma pop(pack)
        nfo =
        {
            .flags = skl->flags,
            .lodType = skl->lodType,
            .jointCnt = skl->jointCnt
        };

        mark.siz += sizeof(nfo);
        mark.siz += jointCnt * sizeof(afxTransform);
        mark.siz += jointCnt * sizeof(afxM4d);
        mark.siz += jointCnt * sizeof(afxUnit32);
        mark.siz += jointCnt * sizeof(afxReal32);
        mark.siz += AfxMeasureNestedStrings(skl->jointCnt, skl->joints);

        if (AfxWriteStream(out, sizeof(mark), 0, &mark))
            AfxThrowError();

        AfxDumpString(&skl->urn, 0, AfxMin(32, skl->urn.len), nfo.urn);

        // Qwadro doesn't use afxChunkID_STRUCT

        if (AfxWriteStream(out, sizeof(nfo), 0, &nfo))
            AfxThrowError();

        if (AfxWriteStream(out, jointCnt * sizeof(afxTransform), 0, skl->local))
            AfxThrowError();

        if (AfxWriteStream(out, jointCnt * sizeof(afxM4d), 0, skl->iw))
            AfxThrowError();

        if (AfxWriteStream(out, jointCnt * sizeof(afxUnit32), 0, skl->parentIdx))
            AfxThrowError();

        if (AfxWriteStream(out, jointCnt * sizeof(afxReal32), 0, skl->lodError))
            AfxThrowError();

        if (AfxWriteNestedStrings(out, jointCnt, skl->joints))
            AfxThrowError();

        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        AfxWriteStream(out, sizeof(extMark), 0, &extMark);

    } while (!err && (++sklIdx < cnt));

    return err;
}

_AMX afxError AfxLoadSkeleton(afxSimulation sim, afxStream in, urdMark const* hdr, afxSkeleton* skeleton)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(skeleton);

    // Qwadro doesn't use afxChunkID_STRUCT

#pragma push(pack, 1)
    struct
    {
        afxChar8    urn[32];
        afxUnit32   flags;
        afxUnit32   lodType;
        afxUnit32   jointCnt;
    } nfo;
#pragma pop(pack)

    if (AfxReadStream(in, sizeof(nfo), 0, &nfo))
        AfxThrowError();

    afxArena arena;
    AfxDeployArena(&arena, NIL, AfxHere());

    afxUnit32* parentIdx = AfxAllocateArena(&arena, nfo.jointCnt * sizeof(afxUnit32));
    afxTransform* local = AfxAllocateArena(&arena, nfo.jointCnt * sizeof(afxTransform));
    afxM4d* iw = AfxAllocateArena(&arena, nfo.jointCnt * sizeof(afxM4d));
    afxReal* lodErr = AfxAllocateArena(&arena, nfo.jointCnt * sizeof(afxReal32));
    afxString* tags = AfxAllocateArena(&arena, nfo.jointCnt * sizeof(afxString));

    if (AfxReadStream(in, nfo.jointCnt * sizeof(local[0]), 0, local))
        AfxThrowError();

    if (AfxReadStream(in, nfo.jointCnt * sizeof(iw[0]), 0, iw))
        AfxThrowError();

    if (AfxReadStream(in, nfo.jointCnt * sizeof(parentIdx[0]), 0, parentIdx))
        AfxThrowError();

    if (AfxReadStream(in, nfo.jointCnt * sizeof(lodErr[0]), 0, lodErr))
        AfxThrowError();

    AfxReadNestedStrings(in, nfo.jointCnt, tags);

    urdMark extHdr;
    afxChunkId cid = AfxPullStreamChunk(in, &extHdr);
    AFX_ASSERT(cid == afxChunkId_EXTENSIONS);
    {
        AfxAdvanceStream(in, extHdr.siz);
    }

    afxSkeleton skl;
    afxSkeletonBlueprint sklb = { 0 };
    AfxMakeString(&sklb.urn.str, 0, nfo.urn, 0);

    sklb.joints = tags;
    AfxAssembleSkeletons(sim, 1, &sklb, &skl);

    AfxResetSkeletonLodErrors(skl, 0, sklb.jointCnt, lodErr);
    AfxReparentSkeletonJoints(skl, 0, sklb.jointCnt, parentIdx, sizeof(parentIdx[0]));
    AfxResetSkeletonTransforms(skl, 0, sklb.jointCnt, local);
    AfxResetSkeletonMatrices(skl, 0, sklb.jointCnt, iw, sizeof(iw[0]));

    AfxDismantleArena(&arena);

    *skeleton = skl;

    return err;
}

_AMX afxError AfxStoreModels(afxStream out, afxUnit cnt, afxModel models[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssertObjects(cnt, models, afxFcc_MDL);
    afxUnit mdlIdx = 0;

    do
    {
        afxModel mdl = models[mdlIdx];
        afxUnit rigCnt = mdl->rigCnt;

        afxString meshes[256];
        afxString origSkels[256];

        for (afxUnit i = 0; i < rigCnt; i++)
        {
            afxString s = { 0 };

            if (mdl->rigs[i].msh)
            {
                AfxGetMeshUrn(mdl->rigs[i].msh, &meshes[i]);
            }
            else meshes[i] = s;

            if (mdl->rigs[i].sklOrig)
            {
                AfxGetSkeletonUrn(mdl->rigs[i].sklOrig, &origSkels[i]);
            }
            else origSkels[i] = s;
        }

        urdMark mark = { 0 };
        mark.fcc = afxChunkId_QW_MDL;

        /*
            HEADER
            -------
            c8              urn[32];
            u32             flags;
            c8              skl[32];
            afxTransform    displacement;
            u32             rigCnt;

            c32             meshes[rigCnt];
            c32             origSkels[rigCnt];
            u32             rigFlags[rigCnt];
            -------
            extensions
        */
#pragma push(pack, 1)
        struct
        {
            afxChar8        urn[32];
            afxUnit32       flags;
            afxChar8        skl[32];
            afxTransform    displacement;
            afxUnit32       rigCnt;
        }
#pragma pop(pack)
        nfo =
        {
            .flags = mdl->flags,
            .displacement = mdl->displacement,
            .rigCnt = rigCnt
        };

        struct
        {
            afxUnit32   flags;
        } rig;

        mark.siz += sizeof(nfo);
        mark.siz += AfxMeasureNestedStrings(rigCnt, meshes);
        mark.siz += AfxMeasureNestedStrings(rigCnt, origSkels);
        mark.siz += rigCnt * sizeof(rig.flags);

        if (AfxWriteStream(out, sizeof(mark), 0, &mark))
            AfxThrowError();

        AfxDumpString(&mdl->urn, 0, AfxMin(32, mdl->urn.len), nfo.urn);

        AfxDumpString(&mdl->skl->urn, 0, AfxMin(32, mdl->skl->urn.len), nfo.skl);

        // Qwadro doesn't use afxChunkID_STRUCT

        if (AfxWriteStream(out, sizeof(nfo), 0, &nfo))
            AfxThrowError();
        
        if (AfxWriteNestedStrings(out, rigCnt, meshes))
            AfxThrowError();

        if (AfxWriteNestedStrings(out, rigCnt, origSkels))
            AfxThrowError();

        for (afxUnit i = 0; i < rigCnt; i++)
        {
            if (AfxWriteStream(out, sizeof(mdl->rigs[i].flags), 0, &mdl->rigs[i].flags))
                AfxThrowError();
        }

        urdMark extMark = { 0 };
        extMark.fcc = afxChunkId_EXTENSIONS;
        AfxWriteStream(out, sizeof(extMark), 0, &extMark);

    } while (!err && (++mdlIdx < cnt));

    return err;
}

_AMX afxError AfxLoadModel(afxSimulation sim, afxStream in, urdMark const* hdr, afxModel* model)
{
    afxError err = NIL;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(model);

    // Qwadro doesn't use afxChunkID_STRUCT

#pragma push(pack, 1)
    struct
    {
        afxChar8        urn[32];
        afxUnit32       flags;
        afxChar8        skl[32];
        afxTransform    displacement;
        afxUnit32       rigCnt;
    } nfo;
#pragma pop(pack)

    afxString meshes[256];
    afxString origSkels[256];
    afxModelFlags rigFlags[256];

    if (AfxReadStream(in, sizeof(nfo), 0, &nfo))
        AfxThrowError();

    if (AfxReadNestedStrings(in, nfo.rigCnt, meshes))
        AfxThrowError();

    if (AfxReadNestedStrings(in, nfo.rigCnt, origSkels))
        AfxThrowError();

    if (AfxReadStream(in, nfo.rigCnt * sizeof(afxMeshRigFlags), 0, rigFlags))
        AfxThrowError();

    urdMark extHdr;
    afxChunkId cid = AfxPullStreamChunk(in, &extHdr);
    AFX_ASSERT(cid == afxChunkId_EXTENSIONS);
    {
        AfxAdvanceStream(in, extHdr.siz);
    }

    afxModel mdl;
    afxModelBlueprint mdlb = { 0 };
    AfxMakeString(&mdlb.skl.urn.str, 0, nfo.urn, 0);

    sklb.joints = tags;
    AfxAssembleModels(sim, NIL, 1, &mdlb, &mdl);

    AfxResetSkeletonLodErrors(skl, 0, sklb.jointCnt, lodErr);
    AfxReparentSkeletonJoints(skl, 0, sklb.jointCnt, parentIdx, sizeof(parentIdx[0]));
    AfxResetSkeletonTransforms(skl, 0, sklb.jointCnt, local);
    AfxResetSkeletonMatrices(skl, 0, sklb.jointCnt, iw, sizeof(iw[0]));

    AfxDismantleArena(&arena);

    *model = mdl;

    return err;
}
