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

#include "../impl/asxImplementation.h"

ASX afxChar const *shdResTypeNames[];

_ASX afxError AfxAssembleModelFromXml(afxSimulation sim, afxXml const* xml, afxUnit elemIdx, afxSkeleton skl, afxModel* model)
{
    afxError err = AFX_ERR_NONE;
    
    afxModelBlueprint config = { 0 };
    
    afxString const mdlAttrNames[] =
    {
        AfxStaticString("id"),
        AfxStaticString("position"),
        AfxStaticString("orientation"),
        AfxStaticString("scale"),
        AfxStaticString("lodType")
    };

    afxString name, content, acontent;
    afxUnit tagCnt = AfxCountXmlTags(xml, elemIdx);

    for (afxUnit i = 0; i < tagCnt; i++)
    {
        AfxQueryXmlTag(xml, elemIdx, i, &name, &acontent);

        switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(mdlAttrNames), mdlAttrNames))
        {
        case 0: // id
        {
            AfxMakeString32(&config.urn, &acontent);
            break;
        }
        case 1: // position
        {
            afxV3d position = { 0 };
            AfxScanString(&acontent, "%f %f %f", &position[0], &position[1], &position[2]);
            break;
        }
        case 2: // orientation
        {
            afxQuat orientation = { 0 };
            AfxScanString(&acontent, "%f %f %f %f", &orientation[0], &orientation[1], &orientation[2], &orientation[3]);
            break;
        }
        case 3: // scale
        {
            afxV3d scale = { 1, 1, 1 };
            AfxScanString(&acontent, "%f %f %f", &scale[0], &scale[1], &scale[2]);
            break;
        }
        case 4: // lodType
        {
            afxUnit lodType = 0;
            AfxScanString(&acontent, "%u", &lodType);
            break;
        }
        default:
        {
            AfxLogY("Tag '%.*s' not handled.", AfxPushString(&name));
            break;
        }
        }
    }

    afxString const mdlChildNames[] =
    {
        AfxStaticString("Joint"),
        AfxStaticString("Rig"),
        AfxStaticString("Attachment")
    };

    afxString const jntAttrNames[] =
    {
        AfxStaticString("id"),
        AfxStaticString("parent"),
        AfxStaticString("position"),
        AfxStaticString("orientation"),
        AfxStaticString("scale"),
        AfxStaticString("lodError"),
        AfxStaticString("iw")
    };

    afxString const rigAttrNames[] =
    {
        AfxStaticString("uri")
    };

    afxUnit rigCnt = 0;
    afxUnit jointCnt = 0;
    afxString joints[256];
    afxTransform locals[256];
    afxUnit parents[256];
    afxReal lodErrors[256];
    afxAtm3d iws[256];
    afxUri meshes[256];

    afxUnit childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxUnit i = 0; i < childCnt; i++)
    {
        afxUnit childElemIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childElemIdx, &name, &content);
        afxUnit childTagCnt = AfxCountXmlTags(xml, childElemIdx);

        switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(mdlChildNames), mdlChildNames))
        {
        case 0: // Joint
        {
            afxUnit tagCnt = AfxCountXmlTags(xml, elemIdx);
            afxV3d position = { 0 };
            afxQuat orientation = { 0, 0, 0, 1 };
            afxV3d scale = { 1, 1, 1 };
            afxReal lodError = -1.0;
            afxUnit parentIdx = AFX_INVALID_INDEX;
            afxAtm3d iw = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
            afxString id = { 0 };

            for (afxUnit j = 0; j < tagCnt; j++)
            {
                AfxQueryXmlTag(xml, elemIdx, j, &name, &acontent);

                switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(jntAttrNames), jntAttrNames))
                {
                case 0: // id
                {
                    id = acontent;
                    break;
                }
                case 1: // parentIdx
                {
                    AfxScanString(&acontent, "%u", &parentIdx);
                    break;
                }
                case 2: // position
                {
                    AfxScanString(&acontent, "%f %f %f", &position[0], &position[1], &position[2]);
                    break;
                }
                case 3: // orientation
                {
                    AfxScanString(&acontent, "%f %f %f %f", &orientation[0], &orientation[1], &orientation[2], &orientation[3]);
                    break;
                }
                case 4: // scale
                {
                    AfxScanString(&acontent, "%f %f %f", &scale[0], &scale[1], &scale[2]);
                    break;
                }
                case 5: // lodError
                {
                    AfxScanString(&acontent, "%f", &lodError);
                    break;
                }
                case 6: // iw
                {
                    AfxScanString(&acontent, "%f %f %f %f %f %f %f %f %f %f %f %f", &iw[0][0], &iw[0][1], &iw[0][2], &iw[1][0], &iw[1][1], &iw[1][2], &iw[2][0], &iw[2][1], &iw[2][2], &iw[3][0], &iw[3][1], &iw[3][2]);
                    break;
                }
                default: break;
                }
            }

            afxM3d ss;
            AfxM3dScaling(ss, scale);
            AfxSetTransform(&locals[jointCnt], position, orientation, ss, TRUE);
            AfxAtm3dCopy(iws[jointCnt], iw);
            lodErrors[jointCnt] = lodError;
            parents[jointCnt] = parentIdx;
            joints[jointCnt] = id;

            ++jointCnt;
            break;
        }
        case 1: // Rig
        {
            afxUri uri = { 0 };

            for (afxUnit j = 0; j < tagCnt; j++)
            {
                AfxQueryXmlTag(xml, elemIdx, j, &name, &acontent);

                switch (AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(rigAttrNames), rigAttrNames))
                {
                case 0: // uri
                {
                    AfxWrapUriString(&uri, &acontent);
                    break;
                }
                default: break;
                }
            }

            meshes[rigCnt] = uri;
            ++rigCnt;
            break;
        }
        case 2: // Attachment
        {
            break;
        }
        default: AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
            break;
        }
    }

    afxModel mdl;
    AfxAssembleModels(sim, 1, &config, &mdl);

    

    return err;
}

/* DFF CLUMP
    -> rwCLUMP
    {
    ----> rwSTRUCT // 12 bytes
        {
            afxUnit32   totalAtomics;
            afxUnit32   totalLights; // ver 0x33000+
            afxUnit32   totalCameras; // ver 0x33000+
        }
    ----> rwFRAMELIST
        {
    --------> rwSTRUCT // 4 bytes
            {
                afxUnit32   totalFrames;
            }
            // frames
            {
                afxM3d      rotationMatrix;
                afxV3d      position;
                afxUnit32   parentFrame;
                afxUnit32   matrixCreationFlag; // ignored
            }           frames[];
            // frame extensions (HAnimPLG, frame name)
        }
        // headless geometry list
        {
    --------> rwSTRUCT (geometry list)
            {
                afxUnit32   totalGeometries;
            }
            // for each geometry
    --------> rwGEOMETRY
            {
    ------------> rwSTRUCT // 16 bytes
                {
                    afxUnit32 fmtFlags;
                    afxUnit32 totalTriangles;
                    afxUnit32 totalVertices;
                    afxUnit32 totalMorphs;
                }
                // if NOT NATIVE, read prelit here. Per-vertice RGBA8.
                // if NOT NATIVE, read UV here. Per-vertice UV32f.
                // if NOT NATIVE, read triangles here. Per-triangle struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2 }. Not mistyped.
                
                // for each morph target
                {
                    afxSphere   bounding; // afxV3d + afxReal32
                    afxBool32   hasVertices;
                    afxBool32   hasNormals;

                    // if hasVertices, read the array of per-vertx afxV3d position here.
                    // if hasNormals, read the array of per-vertex afxV3d normal here.
                }
    ------------> rwMATERIALLIST
                {
    ----------------> rwSTRUCT
                    {
                        afxUnit32 totalMtlAndInsts;
                    }
                    // for each totalMtlAndInsts
                    {
                        // if chunk is rwMATERIAL, read material data
    --------------------> rwMATERIAL
                        {
    ------------------------> rwSTRUCT
                            {
                                afxUnit32 flags;
                                afxUnit32 rgba8;
                                afxUnit32 unused;
                                afxBool32 isTextured;
                                afxReal32 ambient;
                                afxReal32 specular;
                                afxReal32 diffuse;
                            }
    ------------------------> rwTEXTURE
                            {
    ----------------------------> rwSTRUCT
                                {
                                    afxUnit32 filterAddressFlags;
                                }
    ----------------------------> rwSTRING
                                {
                                    name;
                                }
    ----------------------------> rwSTRING
                                {
                                    alphaLayerName;
                                }
    ----------------------------> rwEXTENSION // texture
                                {
                                    
                                }
                            }
    ------------------------> rwEXTENSION // material
                            {
                                
                            }
                        }
                    }
                }
    ------------> rwEXTENSION // geometry
                {
    ----------------> rwBinMeshPLG [CONDITIONAL]
                    {
                        afxUnit32 flags;
                        afxUnit32 totalMeshes;
                        afxUnit32 totalIndices;
                        // for each mesh in totalMeshes
                        {
                            afxUnit32 totalIndices;
                            afxUnit32 materialIdx;
                            
                            // for each index in (ABOVE) totalIndices, read the afxUnit32-long index data.
                        }
                    }
    ----------------> rwSkinPLG [CONDITIONAL]
                    {
                        afxUnit8 totalBoneMatrices;
                        afxUnit8 totalUsedBones;
                        afxUnit8 maxWeightPerVtx;
                        afxUnit8 unused;
                        // for each totalUsedBones, read the afxUnit8-long index data.
                        // for each totalBoneMatrices, read the afxUnit8[4] nest of bone indices.
                        // for each geometry.totalVertices, read the afxReal[4] nest of matrix weights.
                        // for each totalBoneMatrices, read the afxM4d inverse matrix.
                        // skip the 12-bytes long skin split data.
                    }
    ----------------> rwBREAKABLE [CONDITIONAL]
                    {
                        afxUnit32 magicNumber;
                    }
                }
            }
        }
*/

/*
    EXTENSIBLE MODEL


*/

struct DFF
{
    struct CLUMP
    {
        afxUnit32 totalAtomics;
        afxUnit32 totalLights;
        afxUnit32 totalCameras;
        struct FRAMELIST
        {
            afxUnit32 totalFrames;
            struct FRAME
            {
                afxAtm3d m;
                afxUnit32 parentFrame;
                afxUnit32 matrixCreationFlag;
            }*frames;
            // ext name
            afxString32*frameExtNames;
            // ext hanim
            struct
            {
                afxUnit32 animVer; // 0x100 (256) in all GTAs 
                afxUnit32 boneID;
                afxUnit32 totalBonesInHierarchy; // boneCnt
                struct
                {
                    afxUnit32 hierAnimFlags;
                    afxUnit32 animFameSiz; // 36 in GTA
                    struct
                    {
                        afxUnit32 boneID;
                        afxUnit32 boneIndex;
                        afxUnit32 boneTypeFlags;
                    }*boneInfos; // totalBonesInHierarchy
                }*hierarchy;
            }*frameExtHanim;
            afxBool hasHanimExt;
        } fl;
        struct GEOMLIST
        {
            afxUnit32 totalGeometries;
            struct GEOMETRY
            {
                afxUnit32 fmtFlags;
                afxUnit32 totalTriangles;
                afxUnit32 totalVertices;
                afxUnit32 totalMorphs;
                afxUnit32* rgba;
                afxV2d* uv;
                afxV2d* uv2;
                afxUnit16* triangles; // e o mtlIdx?
                struct
                {
                    afxSphere   bounding; // afxV3d + afxReal32
                    afxBool32   hasVertices;
                    afxBool32   hasNormals;
                    afxV3d*     vertices;
                    afxV3d*     normals;
                } morphes[8];
                struct MTLIST
                {
                    afxUnit32 totalMtlAndInsts;
                    struct MATERIAL
                    {
                        afxUnit32 flags;
                        afxUnit32 rgba8;
                        afxUnit32 unused;
                        afxUnit32 texCnt;
                        afxReal32 ambient;
                        afxReal32 specular;
                        afxReal32 diffuse;
                        struct TEXTURE
                        {
                            afxUnit32 filterAddressFlags;
                            afxString32 name;
                            afxString32 mask;
                        } textures[1];
                    }*materials;
                } ml;
                // ext
                afxUnit attrCnt;
                struct MESH
                {
                    afxUnit32 flags;
                    afxUnit32 totalMeshes;
                    afxUnit32 totalIndices;
                    afxUnit32*indices;
                    afxMeshSection* sections; // [totalMeshes]
                } meshExt;
                struct SKIN
                {
                    afxUnit8 totalBoneMatrices;
                    afxUnit8 totalUsedBones;
                    afxUnit8 maxWeightPerVtx;
                    afxUnit8 unused;

                    afxUnit8*usedBones; // totalUsedBones
                    afxUnit8*vtxPivots; // totalVertices * 4
                    afxV4d*vtxWeights; // totalVertices
                    afxM4d*iws; // totalBoneMatrices
                } skinExt;
                afxBool hasSkinExt;
            }*geoms;
        } gl;
    } clumps[1];
    afxUnit clumpCnt;
};

afxError LoadRwsFrameList(afxStream in, afxArena* arena, struct FRAMELIST* fl)
{
    afxError err = NIL;
    afxChunkId cid;
    urdMark txdHdr, hdr;

    cid = AfxPullStreamChunk(in, &hdr);
    AFX_ASSERT(cid == afxChunkId_RW_FRAMELIST);
    cid = AfxPullStreamChunk(in, &hdr);
    AFX_ASSERT(cid == afxChunkId_STRUCT);
    afxUnit32 totalFrames;
    AfxReadStream(in, sizeof(totalFrames), 0, &totalFrames);
    fl->totalFrames = totalFrames;

    if (!fl->frames)
    {
        fl->frames = AfxRequestArenaUnit(arena, totalFrames * 56);
    }

    // for each of totalFrames.
    AfxReadStream(in, totalFrames * 56, 0, fl->frames);

    // frame extension data (HAnimPLG, frame name)
    for (afxUnit frameIdx = 0; frameIdx < totalFrames; frameIdx++)
    {
        urdMark extHdr;
        cid = AfxPullStreamChunk(in, &extHdr);
        AFX_ASSERT(cid == afxChunkId_EXTENSIONS);

        while (extHdr.siz)
        {
            if (!AfxReadNextSeriesHeader(in, &hdr))
            {
                AfxThrowError();
                break;
            }
            cid = hdr.fcc;

            if (cid == /*rwFRAME*/0x253F2FE)
            {
                if (!fl->frameExtNames)
                {
                    fl->frameExtNames = AfxRequestArenaUnit(arena, fl->totalFrames * sizeof(fl->frameExtNames[0]));
                    AfxZero(fl->frameExtNames, fl->totalFrames * sizeof(fl->frameExtNames[0]));
                }

                afxString32 name;
                AfxMakeString32(&fl->frameExtNames[frameIdx], NIL);
                fl->frameExtNames[frameIdx].str.len = hdr.siz;
                AfxReadStream(in, hdr.siz, 0, &fl->frameExtNames[frameIdx].buf);
            }
            else if (cid == /*rwHAnimPLG*/0x11E)
            {
                fl->hasHanimExt = TRUE;

                if (!fl->frameExtHanim)
                {
                    fl->frameExtHanim = AfxRequestArenaUnit(arena, fl->totalFrames * sizeof(fl->frameExtHanim[0]));
                    AfxZero(fl->frameExtHanim, fl->totalFrames * sizeof(fl->frameExtHanim[0]));
                }

                struct
                {
                    afxUnit32 animVer; // 0x100 (256) in all GTAs 
                    afxUnit32 boneID;
                    afxUnit32 totalBonesInHierarchy; // boneCnt
                } bi;
                AfxReadStream(in, sizeof(bi), 0, &bi);

                fl->frameExtHanim[frameIdx].animVer = bi.animVer;
                fl->frameExtHanim[frameIdx].boneID = bi.boneID;
                fl->frameExtHanim[frameIdx].totalBonesInHierarchy = bi.totalBonesInHierarchy;

                if (bi.totalBonesInHierarchy)
                {
                    if (!fl->frameExtHanim[frameIdx].hierarchy)
                    {
                        fl->frameExtHanim[frameIdx].hierarchy = AfxRequestArenaUnit(arena, bi.totalBonesInHierarchy * sizeof(fl->frameExtHanim[frameIdx].hierarchy[0]));
                        AfxZero(fl->frameExtHanim[frameIdx].hierarchy, bi.totalBonesInHierarchy * sizeof(fl->frameExtHanim[frameIdx].hierarchy[0]));
                    }

                    struct
                    {
                        afxUnit32 hierAnimFlags;
                        afxUnit32 animFameSiz; // 36 in GTA
                    } bf;
                    AfxReadStream(in, sizeof(bf), 0, &bf);

                    fl->frameExtHanim[frameIdx].hierarchy->hierAnimFlags = bf.hierAnimFlags;
                    fl->frameExtHanim[frameIdx].hierarchy->animFameSiz = bf.animFameSiz;

                    struct
                    {
                        afxUnit32 boneID;
                        afxUnit32 boneIndex;
                        afxUnit32 boneTypeFlags;
                    } bi2;

                    if (!fl->frameExtHanim[frameIdx].hierarchy->boneInfos)
                    {
                        fl->frameExtHanim[frameIdx].hierarchy->boneInfos = AfxRequestArenaUnit(arena, bi.totalBonesInHierarchy * sizeof(fl->frameExtHanim[frameIdx].hierarchy->boneInfos[0]));
                    }

                    for (afxUnit i = 0; i < bi.totalBonesInHierarchy; i++)
                    {
                        AfxReadStream(in, sizeof(bi2), 0, &bi2);

                        fl->frameExtHanim[frameIdx].hierarchy->boneInfos[i].boneID = bi2.boneID;
                        fl->frameExtHanim[frameIdx].hierarchy->boneInfos[i].boneIndex = bi2.boneIndex;
                        fl->frameExtHanim[frameIdx].hierarchy->boneInfos[i].boneTypeFlags = bi2.boneTypeFlags;
                    }
                }
            }
            else if (AfxAdvanceStream(in, hdr.siz))
            {
                AfxThrowError();
                return FALSE;
            }
            extHdr.siz -= (hdr.siz + sizeof(urdMark));
        }
    }
    return err;
}

afxError LoadRwsMaterialList(afxStream in, afxArena* arena, struct MTLIST* ml)
{
    afxError err = NIL;
    afxChunkId cid;
    urdMark txdHdr, hdr;

    cid = AfxPullStreamChunk(in, &hdr);
    AFX_ASSERT(cid == afxChunkId_RW_MTLL);
    {
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_STRUCT);
        afxUnit32 totalMtlAndInsts;
        AfxReadStream(in, sizeof(totalMtlAndInsts), 0, &totalMtlAndInsts);
        ml->totalMtlAndInsts = totalMtlAndInsts;
        AfxAdvanceStream(in, sizeof(afxUnit32) * totalMtlAndInsts);

        if (!ml->materials)
        {
            ml->materials = AfxRequestArenaUnit(arena, totalMtlAndInsts * sizeof(ml->materials[0]));
        }

        // for each totalMtlAndInsts
        for (afxUnit mtlIdx = 0; mtlIdx < totalMtlAndInsts; mtlIdx++)
        {
            cid = AfxPullStreamChunk(in, &hdr);
            AFX_ASSERT(cid == afxChunkId_RW_MTL);
            cid = AfxPullStreamChunk(in, &hdr);
            AFX_ASSERT(cid == afxChunkId_STRUCT);
            struct
            {
                afxUnit32 flags;
                afxUnit32 rgba8;
                afxUnit32 unused;
                afxUnit32 texCnt;
                afxReal32 ambient;
                afxReal32 specular;
                afxReal32 diffuse;
            } mtl;
            AfxReadStream(in, sizeof(mtl), 0, &mtl);

            ml->materials[mtlIdx].flags = mtl.flags;
            ml->materials[mtlIdx].rgba8 = mtl.rgba8;
            ml->materials[mtlIdx].unused = mtl.unused;
            ml->materials[mtlIdx].texCnt = mtl.texCnt;
            ml->materials[mtlIdx].ambient = mtl.ambient;
            ml->materials[mtlIdx].specular = mtl.specular;
            ml->materials[mtlIdx].diffuse = mtl.diffuse;

            {
                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_RW_TEX);
                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_STRUCT);
                afxUnit32 filterAddressFlags;
                AfxReadStream(in, sizeof(filterAddressFlags), 0, &filterAddressFlags);
                ml->materials[mtlIdx].textures[0].filterAddressFlags = filterAddressFlags;

                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_STRING);
                AfxMakeString32(&ml->materials[mtlIdx].textures[0].name, NIL);
                ml->materials[mtlIdx].textures[0].name.str.len = hdr.siz;
                AfxReadStream(in, hdr.siz, 0, &ml->materials[mtlIdx].textures[0].name.buf);

                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_STRING);
                AfxMakeString32(&ml->materials[mtlIdx].textures[0].mask, NIL);
                ml->materials[mtlIdx].textures[0].mask.str.len = hdr.siz;
                AfxReadStream(in, hdr.siz, 0, &ml->materials[mtlIdx].textures[0].mask.buf);

                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_EXTENSIONS);
                {
                    AfxAdvanceStream(in, hdr.siz);
                }
            }

            cid = AfxPullStreamChunk(in, &hdr);
            AFX_ASSERT(cid == afxChunkId_EXTENSIONS);
            {
                AfxAdvanceStream(in, hdr.siz);
            }
        }
    }
    return err;
}

afxError LoadRwsGeomList(afxStream in, afxArena* arena, struct GEOMLIST* gl)
{
    afxError err = NIL;

    afxChunkId cid;
    urdMark txdHdr, hdr;

    // geometry list
    cid = AfxPullStreamChunk(in, &hdr);
    AFX_ASSERT(cid == afxChunkId_RW_GEOLIST);
    cid = AfxPullStreamChunk(in, &hdr);
    AFX_ASSERT(cid == afxChunkId_STRUCT);

    afxUnit32 totalGeometries;
    AfxReadStream(in, sizeof(totalGeometries), 0, &totalGeometries);
    gl->totalGeometries = totalGeometries;

    if (!gl->geoms)
    {
        gl->geoms = AfxRequestArenaUnit(arena, gl->totalGeometries * sizeof(gl->geoms[0]));
        AfxZero(gl->geoms, gl->totalGeometries * sizeof(gl->geoms[0]));
    }

    // for each geometry
    for (afxUnit geoIdx = 0; geoIdx < totalGeometries; geoIdx++)
    {
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_RW_GEOM);
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_STRUCT);
        struct
        {
            afxUnit32 fmtFlags;
            afxUnit32 totalTriangles;
            afxUnit32 totalVertices;
            afxUnit32 totalMorphs;
        } geoHdr;
        AfxReadStream(in, sizeof(geoHdr), 0, &geoHdr);

        gl->geoms[geoIdx].fmtFlags = geoHdr.fmtFlags;
        gl->geoms[geoIdx].totalTriangles = geoHdr.totalTriangles;
        gl->geoms[geoIdx].totalVertices = geoHdr.totalVertices;
        gl->geoms[geoIdx].totalMorphs = geoHdr.totalMorphs;

        afxUnit attrCnt = 0;

        afxUnit posAttrIdx = AFX_INVALID_INDEX,
            nrmAttrIdx = AFX_INVALID_INDEX,
            uvAttrIdx = AFX_INVALID_INDEX,
            uv2AttrIdx = AFX_INVALID_INDEX,
            rgbaAttrIdx = AFX_INVALID_INDEX,
            pvtAttrIdx = AFX_INVALID_INDEX,
            wgtAttrIdx = AFX_INVALID_INDEX;

        afxSize baseUvOff = 0, baseUv2Off = 0, baseNrmOff = 0, baseRgbaOff = 0, basePosOff = 0;

        if (geoHdr.fmtFlags & /*rpGEOMETRYPOSITIONS*/0x2)
            posAttrIdx = attrCnt++;

        if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED*/0x4)
            uvAttrIdx = attrCnt++;

        if (geoHdr.fmtFlags & /*rpGEOMETRYPRELIT*/0x8)
            rgbaAttrIdx = attrCnt++;

        if (geoHdr.fmtFlags & /*rpGEOMETRYNORMALS*/0x10)
            nrmAttrIdx = attrCnt++;

        if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED2*/0x80)
            uv2AttrIdx = attrCnt++;

        if (!(geoHdr.fmtFlags & /*rpGEOMETRYNATIVE*/0x1000000)) // is NOT NATIVE
        {
            if (geoHdr.totalVertices)
            {
                if (geoHdr.fmtFlags & /*rpGEOMETRYPRELIT*/0x8)
                {
                    // if NOT NATIVE, read prelit here. Per-vertice RGBA8.
                    gl->geoms[geoIdx].rgba = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(gl->geoms[geoIdx].rgba[0]));
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxUnit32), 0, gl->geoms[geoIdx].rgba);
                    //AfxUploadGeometry(mshb.geo, rgbaAttrIdx, 0, mshb.vtxCnt, in, sizeof(afxRgba8));
                }

                if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED*/0x4)
                {
                    // if NOT NATIVE, read UV here. Per-vertice UV32f.
                    gl->geoms[geoIdx].uv = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(gl->geoms[geoIdx].uv[0]));
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV2d), 0, gl->geoms[geoIdx].uv);
                    //AfxUploadGeometry(mshb.geo, uvAttrIdx, 0, mshb.vtxCnt, in, sizeof(afxV2d));
                }

                if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED2*/0x80)
                {
                    // if NOT NATIVE, read UV here. Per-vertice UV32f.
                    gl->geoms[geoIdx].uv2 = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(gl->geoms[geoIdx].uv2[0]));
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV2d), 0, gl->geoms[geoIdx].uv2);
                    //AfxUploadGeometry(mshb.geo, uv2AttrIdx, 0, mshb.vtxCnt, in, sizeof(afxV2d));
                }

                if (geoHdr.totalTriangles)
                {
                    afxBool tristriped = geoHdr.fmtFlags & 0x1;
                    // if NOT NATIVE, read triangles here. Per-triangle struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2 }. Not mistyped.
                    //afxUnit* triangles = AfxGetMeshIndices(meshes[geoIdx], 0);

                    gl->geoms[geoIdx].triangles = AfxRequestArenaUnit(arena, geoHdr.totalTriangles * 3 * sizeof(afxUnit16));

                    for (afxUnit i = 0, total = geoHdr.totalTriangles; i < total; i++)
                    {
                        struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2; } tri;
                        AfxReadStream(in, sizeof(tri), 0, &tri);

                        gl->geoms[geoIdx].triangles[geoHdr.totalTriangles + 0] = tri.vtx0;
                        gl->geoms[geoIdx].triangles[geoHdr.totalTriangles + 1] = tri.vtx1;
                        gl->geoms[geoIdx].triangles[geoHdr.totalTriangles + 2] = tri.vtx2;
                    }
                }
            }
        }

        // for each morph target
        for (afxUnit32 morphIdx = 0; morphIdx < geoHdr.totalMorphs; ++morphIdx)
        {
            struct
            {
                afxV3d      boundingCentre;
                afxReal     boundingRadius; // afxV3d + afxReal32
                afxBool32   hasVertices;
                afxBool32   hasNormals;
            } morph;
            AfxReadStream(in, sizeof(morph), 0, &morph);

            AfxV3dCopy(gl->geoms[geoIdx].morphes[morphIdx].bounding.centre, morph.boundingCentre);
            gl->geoms[geoIdx].morphes[morphIdx].bounding.radius = morph.boundingRadius;
            gl->geoms[geoIdx].morphes[morphIdx].hasNormals = morph.hasNormals;
            gl->geoms[geoIdx].morphes[morphIdx].hasVertices = morph.hasVertices;

            if (morph.hasVertices)
            {
                // if hasVertices, read the array of per-vertex afxV3d position here.
                //AfxUploadGeometry(geo, posAttrIdx, 0, geoi.vtxCnt, in, sizeof(afxV3d));
                gl->geoms[geoIdx].morphes[morphIdx].vertices = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(afxV3d));
                AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV3d), 0, gl->geoms[geoIdx].morphes[morphIdx].vertices);
            }

            if (morph.hasNormals)
            {
                // if hasNormals, read the array of per-vertex afxV3d normal here.
                gl->geoms[geoIdx].morphes[morphIdx].normals = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(afxV3d));
                AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV3d), 0, gl->geoms[geoIdx].morphes[morphIdx].normals);
                //AfxUploadGeometry(geo, nrmAttrIdx, 0, geoi.vtxCnt, in, sizeof(afxV3d));
            }
        }

        gl->geoms[geoIdx].attrCnt = attrCnt;

        gl->geoms[geoIdx].ml.materials = NIL;
        LoadRwsMaterialList(in, arena, &gl->geoms[geoIdx].ml);

        urdMark extHdr;
        cid = AfxPullStreamChunk(in, &extHdr);
        AFX_ASSERT(cid == afxChunkId_EXTENSIONS);

        while (extHdr.siz)
        {
            if (!AfxReadNextSeriesHeader(in, &hdr))
            {
                AfxThrowError();
                break;
            }
            cid = hdr.fcc;

            if (cid == /*rwBinMeshPLG*/0x50E)
            {
                struct
                {
                    afxUnit32 flags;
                    afxUnit32 totalMeshes;
                    afxUnit32 totalIndices;
                } bm;
                AfxReadStream(in, sizeof(bm), 0, &bm);

                gl->geoms[geoIdx].meshExt.flags = bm.flags;
                gl->geoms[geoIdx].meshExt.totalIndices = bm.totalIndices;
                gl->geoms[geoIdx].meshExt.totalMeshes = bm.totalMeshes;

                gl->geoms[geoIdx].meshExt.indices = AfxRequestArenaUnit(arena, gl->geoms[geoIdx].meshExt.totalIndices * sizeof(afxUnit32));

                afxUnit baseIdx = 0;

                if (!gl->geoms[geoIdx].meshExt.sections)
                {
                    gl->geoms[geoIdx].meshExt.sections = AfxRequestArenaUnit(arena, gl->geoms[geoIdx].meshExt.totalMeshes * sizeof(gl->geoms[geoIdx].meshExt.sections[0]));
                }

                // for each mesh in totalMeshes
                for (afxUnit bmIdx = 0; bmIdx < bm.totalMeshes; bmIdx++)
                {
                    struct
                    {
                        afxUnit32 totalIndices;
                        afxUnit32 materialIdx;
                    } bms;
                    AfxReadStream(in, sizeof(bms), 0, &bms);

                    gl->geoms[geoIdx].meshExt.sections[bmIdx].mtlIdx = bms.materialIdx;
                    gl->geoms[geoIdx].meshExt.sections[bmIdx].baseTriIdx = baseIdx / 3;
                    gl->geoms[geoIdx].meshExt.sections[bmIdx].triCnt = bms.totalIndices / 3;

                    // for each index in (ABOVE) totalIndices, read the afxUnit32-long index data.
                    AfxReadStream(in, bms.totalIndices * sizeof(afxUnit32), 0, &gl->geoms[geoIdx].meshExt.indices[baseIdx]);
                    baseIdx += bms.totalIndices;
                }
            }
            else if (cid == /*rwSkinPLG*/0x116)
            {
                gl->geoms[geoIdx].hasSkinExt = TRUE;

                struct
                {
                    afxUnit8 totalBoneMatrices;
                    afxUnit8 totalUsedBones;
                    afxUnit8 maxWeightPerVtx;
                    afxUnit8 unused;
                } ski;
                AfxReadStream(in, sizeof(ski), 0, &ski);

                gl->geoms[geoIdx].skinExt.maxWeightPerVtx = ski.maxWeightPerVtx;
                gl->geoms[geoIdx].skinExt.totalBoneMatrices = ski.totalBoneMatrices;
                gl->geoms[geoIdx].skinExt.totalUsedBones = ski.totalUsedBones;
                gl->geoms[geoIdx].skinExt.unused = ski.unused;

                if (!gl->geoms[geoIdx].skinExt.usedBones)
                {
                    gl->geoms[geoIdx].skinExt.usedBones = AfxRequestArenaUnit(arena, ski.totalUsedBones * sizeof(afxUnit8));
                }

                // for each totalUsedBones, read the afxUnit8-long index data.
                AfxReadStream(in, ski.totalUsedBones * sizeof(afxUnit8), 0, gl->geoms[geoIdx].skinExt.usedBones);

                if (!gl->geoms[geoIdx].skinExt.vtxPivots)
                {
                    gl->geoms[geoIdx].skinExt.vtxPivots = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(afxUnit8[4]));
                }

                // for each geometry.totalVertices, read the afxUnit8[4] nest of bone indices.
                AfxReadStream(in, geoHdr.totalVertices * sizeof(afxUnit8[4]), 0, gl->geoms[geoIdx].skinExt.vtxPivots);

                if (!gl->geoms[geoIdx].skinExt.vtxWeights)
                {
                    gl->geoms[geoIdx].skinExt.vtxWeights = AfxRequestArenaUnit(arena, geoHdr.totalVertices * sizeof(afxReal32[4]));
                }

                // for each geometry.totalVertices, read the afxReal32[4] nest of matrix weights.
                AfxReadStream(in, geoHdr.totalVertices * sizeof(afxReal32[4]), 0, gl->geoms[geoIdx].skinExt.vtxWeights);

                if (!gl->geoms[geoIdx].skinExt.iws)
                {
                    gl->geoms[geoIdx].skinExt.iws = AfxRequestArenaUnit(arena, ski.totalBoneMatrices * sizeof(afxM4d));
                }

                // for each totalBoneMatrices, read the afxM4d inverse matrix.
                AfxReadStream(in, ski.totalBoneMatrices * sizeof(afxM4d), 0, gl->geoms[geoIdx].skinExt.iws);

                // skip the 12-bytes long skin split data.
                AfxAdvanceStream(in, 12);
            }
            else if (AfxAdvanceStream(in, hdr.siz))
            {
                AfxThrowError();
                return FALSE;
            }


            extHdr.siz -= (hdr.siz + sizeof(urdMark));
        }
    }

    return err;
}

afxError LoadRwDff(afxSimulation sim, afxDrawInput din, afxStream in, afxModel* model)
{
    afxError err = NIL;
    
    afxArena arena;
    AfxDeployArena(&arena, NIL, AfxHere());

    afxMesh meshes[32];
    struct DFF dff = {0};
    sizeof(dff);

    afxChunkId cid;
    urdMark txdHdr, hdr;
    if (!AfxFetchNextStreamChunk(in, afxChunkId_RW_CLUMP, &txdHdr)) AfxThrowError();
    else
    {
        AfxZero(&dff, sizeof(dff));

        struct
        {
            afxUnit32 totalAtomics;
            afxUnit32 totalLights;
            afxUnit32 totalCameras;
        } clumpHdr;
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_STRUCT);
        AfxReadStream(in, sizeof(clumpHdr), 0, &clumpHdr);
        dff.clumps[0].totalLights = clumpHdr.totalLights;
        dff.clumps[0].totalCameras = clumpHdr.totalCameras;
        dff.clumps[0].totalAtomics = clumpHdr.totalAtomics;

        LoadRwsFrameList(in, &arena, &dff.clumps[0].fl);

        LoadRwsGeomList(in, &arena, &dff.clumps[0].gl);

        afxString joints[256];

        for (afxUnit i = 0; i < dff.clumps[0].fl.totalFrames; i++)
        {
            AfxReflectString(&dff.clumps[0].fl.frameExtNames[i].str, &joints[i]);
        }

        afxUri urn = { 0 };
        //AfxWrapUriString(&uri, &dff.clumps[0]);
        // O nome não entrou na função

        afxModel mdl;
        afxModelBlueprint mdlb = { 0 };
        mdlb.jointCnt = dff.clumps[0].fl.totalFrames;
        mdlb.joints = joints;
        AfxMakeString32(&mdlb.urn, &urn.str);
        mdlb.rigCnt = clumpHdr.totalAtomics;
        AfxAssembleModels(sim, 1, &mdlb, &mdl);

        for (afxUnit i = 0; i < dff.clumps[0].fl.totalFrames; i++)
        {
            afxQuat q;
            afxTransform t;
            AfxQuatRotationFromM3d(q, dff.clumps[0].fl.frames[i].m);
            AfxSetTransform(&t, dff.clumps[0].fl.frames[i].m[3], q, AFX_M3D_IDENTITY, TRUE);
            AfxResetJointTransforms(mdl, i, 1, &t);

            AfxReparentJoints(mdl, i, 1, &dff.clumps[0].fl.frames[i].parentFrame, sizeof(afxUnit32));

            AfxResetJointMatrices(mdl, i, 1, NIL, sizeof(afxM4d));
        }

        for (afxUnit geoIdx = 0; geoIdx < dff.clumps[0].gl.totalGeometries; geoIdx++)
        {
            struct GEOMETRY* geo = &dff.clumps[0].gl.geoms[geoIdx];

            afxUnit attrCnt = 0;
            afxString vtxAttrs[16];

            afxUnit posAttrIdx = AFX_INVALID_INDEX,
                nrmAttrIdx = AFX_INVALID_INDEX,
                uvAttrIdx = AFX_INVALID_INDEX,
                uv2AttrIdx = AFX_INVALID_INDEX,
                rgbaAttrIdx = AFX_INVALID_INDEX,
                pvtAttrIdx = AFX_INVALID_INDEX,
                wgtAttrIdx = AFX_INVALID_INDEX;

            afxMask allAttrsEnabled = NIL;
            afxSize baseUvOff = 0, baseUv2Off = 0, baseNrmOff = 0, baseRgbaOff = 0, basePosOff = 0;

            if (geo->fmtFlags & /*rpGEOMETRYPOSITIONS*/0x2)
            {
                posAttrIdx = attrCnt++, allAttrsEnabled |= AFX_BIT(posAttrIdx);
                AfxMakeString(&vtxAttrs[posAttrIdx], 0, "pos", 0);
            }

            if (geo->fmtFlags & /*rpGEOMETRYTEXTURED*/0x4)
            {
                uvAttrIdx = attrCnt++, allAttrsEnabled |= AFX_BIT(uvAttrIdx);
                AfxMakeString(&vtxAttrs[uvAttrIdx], 0, "uv", 0);
            }

            if (geo->fmtFlags & /*rpGEOMETRYPRELIT*/0x8)
            {
                rgbaAttrIdx = attrCnt++, allAttrsEnabled |= AFX_BIT(rgbaAttrIdx);
                AfxMakeString(&vtxAttrs[rgbaAttrIdx], 0, "rgba", 0);
            }

            if (geo->fmtFlags & /*rpGEOMETRYNORMALS*/0x10)
            {
                nrmAttrIdx = attrCnt++, allAttrsEnabled |= AFX_BIT(nrmAttrIdx);
                AfxMakeString(&vtxAttrs[nrmAttrIdx], 0, "nrm", 0);
            }

            if (geo->fmtFlags & /*rpGEOMETRYTEXTURED2*/0x80)
            {
                uv2AttrIdx = attrCnt++, allAttrsEnabled |= AFX_BIT(uv2AttrIdx);
                AfxMakeString(&vtxAttrs[uv2AttrIdx], 0, "uv2", 0);
            }

            afxString materials[256];
            afxString biases[256] = {0};
            afxMeshBlueprint mshb = { 0 };
            mshb.vtxCnt = geo->totalVertices;
            mshb.attrCnt = geo->attrCnt;
            mshb.attrs = vtxAttrs;
            mshb.triCnt = geo->totalTriangles;
            mshb.secCnt = geo->meshExt.totalMeshes;
            mshb.mtlCnt = geo->ml.totalMtlAndInsts;
            mshb.materials = materials;
            //mshb.trisForSurfMap = geo->meshExt.trisForSurfMap;
            //mshb.baseTrisForSurfMap = geo->meshExt.baseTrisForSurfMap;
            //mshb.surfToMtlMap = geo->meshExt.surfToMtlMap;
            mshb.morphCnt = geo->totalMorphs;
            mshb.biases = biases;

            for (afxUnit i = 0; i < mshb.mtlCnt; i++)
            {
                AfxMakeString(&materials[i], 0, geo->ml.materials[i].textures[0].name.str.start, geo->ml.materials[i].textures[0].name.str.len);
            }

            if (geo->hasSkinExt)
            {
                mshb.biasCnt = geo->skinExt.totalUsedBones;
                
                for (afxUnit i = 0; i < mshb.biasCnt; i++)
                {
                    afxUnit boneId = geo->skinExt.usedBones[i];
                    AfxReflectString(&dff.clumps[0].fl.frameExtNames[boneId].str, &biases[i]);

                    AfxResetJointMatrices(mdl, boneId, 1, dff.clumps[0].gl.geoms[0].skinExt.iws[i], sizeof(afxM4d));
#if 0
                    afxBool found = FALSE;

                    for (afxUnit j = 0; j < dff.clumps[0].fl.totalFrames; j++)
                    {
                        if (boneId == dff.clumps[0].fl.frameExtHanim[j].boneID)
                        {
                            found = TRUE;
                            AfxReflectString(&dff.clumps[0].fl.frameExtNames[j].str, &biases[i]);
                            break;
                        }
                    }
                    
                    if (!found)
                    {
                        for (afxUnit j = 0; j < dff.clumps[0].fl.frameExtHanim[1].totalBonesInHierarchy; j++)
                        {
                            if (boneId == dff.clumps[0].fl.frameExtHanim[1].hierarchy->boneInfos[j].boneID)
                            {
                                found = TRUE;
                                AfxReflectString(&dff.clumps[0].fl.frameExtNames[dff.clumps[0].fl.frameExtHanim[1].hierarchy->boneInfos[j].boneIndex].str, &biases[i]);
                                break;
                            }
                        }


                        int a = 1;
                    }
#endif
                }
            }

            AfxCompileMeshes(sim, 1, &mshb, &meshes[geoIdx]);

            AfxFormatVertexAttribute(meshes[geoIdx], posAttrIdx, avxFormat_RGB32f, afxVertexFlag_ATV, &AfxString("pos"));
            AfxFormatVertexAttribute(meshes[geoIdx], nrmAttrIdx, avxFormat_RGB32f, afxVertexFlag_LTM, &AfxString("nrm"));

            for (afxUnit morphIdx = 0; morphIdx < geo->totalMorphs; morphIdx++)
            {
                afxMask morphedAttrs = NIL;

                if (geo->morphes[morphIdx].hasVertices)
                {
                    AFX_ASSERT(posAttrIdx != AFX_INVALID_INDEX);
                    morphedAttrs |= AFX_BIT(posAttrIdx);
                    AfxUpdateVertexData(meshes[geoIdx], posAttrIdx, morphIdx, 0, mshb.vtxCnt, geo->morphes[morphIdx].vertices, sizeof(afxV3d));
                }

                if (geo->morphes[morphIdx].hasNormals)
                {
                    AFX_ASSERT(nrmAttrIdx != AFX_INVALID_INDEX);
                    morphedAttrs |= AFX_BIT(nrmAttrIdx);
                    AfxUpdateVertexData(meshes[geoIdx], nrmAttrIdx, morphIdx, 0, mshb.vtxCnt, geo->morphes[morphIdx].normals, sizeof(afxV3d));
                }

                if (morphIdx == 0)
                {
                    morphedAttrs |= allAttrsEnabled;
                }

                afxMeshMorph mshm = { 0 };
                mshm.morphedAttrs = morphedAttrs;
                AfxChangeMeshMorphes(meshes[geoIdx], morphIdx, 1, &mshm);
            }

            if (rgbaAttrIdx != AFX_INVALID_INDEX)
            {
                AfxFormatVertexAttribute(meshes[geoIdx], rgbaAttrIdx, avxFormat_RGBA8un, NIL, &AfxString("rgba"));
                AfxUpdateVertexData(meshes[geoIdx], rgbaAttrIdx, 0, 0, mshb.vtxCnt, geo->rgba, sizeof(afxUnit8[4]));
            }

            if (uvAttrIdx != AFX_INVALID_INDEX)
            {
                AfxFormatVertexAttribute(meshes[geoIdx], uvAttrIdx, avxFormat_RG32f, NIL, &AfxString("uv"));
                AfxUpdateVertexData(meshes[geoIdx], uvAttrIdx, 0, 0, mshb.vtxCnt, geo->uv, sizeof(afxV2d));
            }

            if (uv2AttrIdx != AFX_INVALID_INDEX)
            {
                AfxFormatVertexAttribute(meshes[geoIdx], uv2AttrIdx, avxFormat_RG32f, NIL, &AfxString("uv0"));
                AfxUpdateVertexData(meshes[geoIdx], uv2AttrIdx, 0, 0, mshb.vtxCnt, geo->uv2, sizeof(afxV2d));
            }

            if (geo->hasSkinExt)
            {
                if (pvtAttrIdx != AFX_INVALID_INDEX)
                {
                    AfxFormatVertexAttribute(meshes[geoIdx], pvtAttrIdx, avxFormat_RGBA8un, NIL, &AfxString("pvt"));
                    AfxUpdateVertexData(meshes[geoIdx], pvtAttrIdx, 0, 0, mshb.vtxCnt, geo->skinExt.vtxPivots, sizeof(geo->skinExt.vtxPivots[0]));
                }

                if (wgtAttrIdx != AFX_INVALID_INDEX)
                {
                    AfxFormatVertexAttribute(meshes[geoIdx], wgtAttrIdx, avxFormat_RGBA32f, NIL, &AfxString("wgt"));
                    AfxUpdateVertexData(meshes[geoIdx], wgtAttrIdx, 0, 0, mshb.vtxCnt, geo->skinExt.vtxWeights, sizeof(geo->skinExt.vtxWeights[0]));
                }
            }

            for (afxUnit bmIdx = 0; bmIdx < geo->meshExt.totalMeshes; bmIdx++)
            {
                afxMeshSection sec = { 0 };
                sec.baseTriIdx = geo->meshExt.sections[bmIdx].baseTriIdx;
                sec.triCnt = geo->meshExt.sections[bmIdx].triCnt;
                sec.mtlIdx = geo->meshExt.sections[bmIdx].mtlIdx;
                AfxResetMeshSections(meshes[geoIdx], bmIdx, 1, &sec);
                AfxUpdateMeshIndices(meshes[geoIdx], geo->meshExt.sections[bmIdx].baseTriIdx, geo->meshExt.sections[bmIdx].triCnt, &geo->meshExt.indices[geo->meshExt.sections[bmIdx].baseTriIdx * 3], sizeof(afxUnit32));
            }

            afxM3d ltm, iltm;
            afxM3d srcAxisSys;
            AfxM3dSetTransposed(srcAxisSys, AFX_V3D_X, AFX_V3D_Z, AFX_V3D_Y);
            AfxM3dMultiply(ltm, AFX_M3D_IDENTITY, srcAxisSys);
            AfxM3dInvert(iltm, ltm);
            AfxTransformMeshes(ltm, iltm, AFX_EPSILON, AFX_V3D_ZERO, AFX_EPSILON, 3, 1, &meshes[geoIdx]);
        }

        AfxRigMeshes(mdl, NIL, 0, dff.clumps[0].gl.totalGeometries, meshes);

        *model = mdl;

        // TODO use arena
        // TODO map bones to frames
        // TODO vertex attribute names

        int a = 1;

    }

    AfxDismantleArena(&arena);

    return err;
}

_ASX afxError TestDffLoad(afxSimulation sim, afxDrawInput din, afxUri const* uri, afxModel* model)
{

    afxStream file;
    AfxOpenFile(uri, afxFileFlag_R, &file);

    afxModel mdl;
    LoadRwDff(sim, din, file, &mdl);
    *model = mdl;

    AfxDisposeObjects(1, &file);
}
