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

#include "../dev/AmxPuppetImplKit.h"
#include "../dev/AmxCadImplKit.h"
#include "../dev/AmxImplKit.h"

AMX afxChar const *shdResTypeNames[];

_AMX afxError AfxAssembleModelFromXml(afxSimulation sim, afxXml const* xml, afxUnit elemIdx, afxSkeleton skl, afxModel* model)
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

        switch (AfxCompareStrings(&name, TRUE, ARRAY_SIZE(mdlAttrNames), mdlAttrNames))
        {
        case 0: // id
        {
            AfxMakeString32(&config.skl.urn, &acontent);
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

        switch (AfxCompareStrings(&name, TRUE, ARRAY_SIZE(mdlChildNames), mdlChildNames))
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

                switch (AfxCompareStrings(&name, TRUE, ARRAY_SIZE(jntAttrNames), jntAttrNames))
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

            AfxSetTransform(&locals[jointCnt], position, orientation, scale, TRUE);
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

                switch (AfxCompareStrings(&name, TRUE, ARRAY_SIZE(rigAttrNames), rigAttrNames))
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
    AfxAssembleModels(sim, skl, 1, &config, &mdl);

    

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
                
                // ext name
                afxString32 name;
                // ext hanim
                struct
                {
                    afxUnit32 animVer; // 0x100 (256) in all GTAs 
                    afxUnit32 boneID;
                    afxUnit32 totalBonesInHierarchy; // boneCnt
                } hanim;

            } frames[128];
            struct
            {
                afxUnit32 hierAnimFlags;
                afxUnit32 animFameSiz; // 36 in GTA
                struct
                {
                    afxUnit32 boneID;
                    afxUnit32 boneIndex;
                    afxUnit32 boneTypeFlags;
                } boneInfos[128]; // totalBonesInHierarchy
            } frameExtHanimX;
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
                } morphes[32];
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
                    } materials[64];
                } ml;
                // ext
                struct MESH
                {
                    afxUnit32 flags;
                    afxUnit32 totalMeshes;
                    afxUnit32 totalIndices;
                    afxUnit32*indices;
                    afxUnit32 surfToMtlMap[64];
                    afxUnit32 baseTrisForSurfMap[64];
                    afxUnit32 trisForSurfMap[64];
                } meshExt;
                struct SKIN
                {
                    afxUnit8 totalBoneMatrices;
                    afxUnit8 totalUsedBones;
                    afxUnit8 maxWeightPerVtx;
                    afxUnit8 unused;

                    afxUnit8 bones[8]; // totalUsedBones
                    afxUnit8 pivots[128][4]; // totalVertices
                    afxV4d weights[128]; // totalVertices
                    afxM4d iws[128]; // totalBoneMatrices
                } skinExt;
            } geometries[1];
        } gl;
    } clumps[1];
    afxUnit clumpCnt;
};

afxError LoadRwDff(afxSimulation sim, afxDrawInput din, afxStream in, afxModel* model)
{
    afxError err = NIL;
    
    afxMesh meshes[32];
    struct DFF dff = {0};
    sizeof(dff);

    afxChunkId cid;
    urdMark txdHdr, hdr;
    if (!AfxFetchNextStreamChunk(in, afxChunkId_RW_CLUMP, &txdHdr)) AfxThrowError();
    else
    {
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

        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_RW_FRAMELIST);
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_STRUCT);
        afxUnit32 totalFrames;
        AfxReadStream(in, sizeof(totalFrames), 0, &totalFrames);
        dff.clumps[0].fl.totalFrames = totalFrames;
        
        // for each of totalFrames.
        AfxReadStream(in, totalFrames * 56, 0, dff.clumps[0].fl.frames);
            
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
                    afxString32 name;
                    AfxMakeString32(&dff.clumps[0].fl.frames[frameIdx].name, NIL);
                    dff.clumps[0].fl.frames[frameIdx].name.str.len = hdr.siz;
                    AfxReadStream(in, hdr.siz, 0, &dff.clumps[0].fl.frames[frameIdx].name.buf);
                }
                else if (cid == /*rwHAnimPLG*/0x11E)
                {
                    struct
                    {
                        afxUnit32 animVer; // 0x100 (256) in all GTAs 
                        afxUnit32 boneID;
                        afxUnit32 totalBonesInHierarchy; // boneCnt
                    } bi;
                    AfxReadStream(in, sizeof(bi), 0, &bi);

                    dff.clumps[0].fl.frames[frameIdx].hanim.animVer = bi.animVer;
                    dff.clumps[0].fl.frames[frameIdx].hanim.boneID = bi.boneID;
                    dff.clumps[0].fl.frames[frameIdx].hanim.totalBonesInHierarchy = bi.totalBonesInHierarchy;

                    if (bi.totalBonesInHierarchy)
                    {
                        struct
                        {
                            afxUnit32 hierAnimFlags;
                            afxUnit32 animFameSiz; // 36 in GTA
                        } bf;
                        AfxReadStream(in, sizeof(bf), 0, &bf);

                        dff.clumps[0].fl.frameExtHanimX.hierAnimFlags = bf.hierAnimFlags;
                        dff.clumps[0].fl.frameExtHanimX.animFameSiz = bf.animFameSiz;

                        struct
                        {
                            afxUnit32 boneID;
                            afxUnit32 boneIndex;
                            afxUnit32 boneTypeFlags;
                        } bi2;
                        for (afxUnit i = 0; i < bi.totalBonesInHierarchy; i++)
                        {
                            AfxReadStream(in, sizeof(bi2), 0, &bi2);

                            dff.clumps[0].fl.frameExtHanimX.boneInfos[frameIdx].boneID = bi2.boneID;
                            dff.clumps[0].fl.frameExtHanimX.boneInfos[frameIdx].boneIndex = bi2.boneIndex;
                            dff.clumps[0].fl.frameExtHanimX.boneInfos[frameIdx].boneTypeFlags = bi2.boneTypeFlags;
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

        // geometry list
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_RW_GEOLIST);
        cid = AfxPullStreamChunk(in, &hdr);
        AFX_ASSERT(cid == afxChunkId_STRUCT);

        afxUnit32 totalGeometries;
        AfxReadStream(in, sizeof(totalGeometries), 0, &totalGeometries);
        dff.clumps[0].gl.totalGeometries = totalGeometries;

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

            dff.clumps[0].gl.geometries[0].fmtFlags = geoHdr.fmtFlags;
            dff.clumps[0].gl.geometries[0].totalTriangles = geoHdr.totalTriangles;
            dff.clumps[0].gl.geometries[0].totalVertices = geoHdr.totalVertices;
            dff.clumps[0].gl.geometries[0].totalMorphs = geoHdr.totalMorphs;

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
                if (geoHdr.fmtFlags & /*rpGEOMETRYPRELIT*/0x8)
                {
                    // if NOT NATIVE, read prelit here. Per-vertice RGBA8.
                    dff.clumps[0].gl.geometries[0].rgba = AfxAllocate(geoHdr.totalVertices, sizeof(dff.clumps[0].gl.geometries[0].rgba[0]), 0, AfxHere());
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxUnit32), 0, dff.clumps[0].gl.geometries[0].rgba);
                    //AfxUploadGeometry(mshb.geo, rgbaAttrIdx, 0, mshb.vtxCnt, in, sizeof(afxRgba8));
                }

                if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED*/0x4)
                {
                    // if NOT NATIVE, read UV here. Per-vertice UV32f.
                    dff.clumps[0].gl.geometries[0].uv = AfxAllocate(geoHdr.totalVertices, sizeof(dff.clumps[0].gl.geometries[0].uv[0]), 0, AfxHere());
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV2d), 0, dff.clumps[0].gl.geometries[0].uv);
                    //AfxUploadGeometry(mshb.geo, uvAttrIdx, 0, mshb.vtxCnt, in, sizeof(afxV2d));
                }

                if (geoHdr.fmtFlags & /*rpGEOMETRYTEXTURED2*/0x80)
                {
                    // if NOT NATIVE, read UV here. Per-vertice UV32f.
                    dff.clumps[0].gl.geometries[0].uv2 = AfxAllocate(geoHdr.totalVertices, sizeof(dff.clumps[0].gl.geometries[0].uv2[0]), 0, AfxHere());
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV2d), 0, dff.clumps[0].gl.geometries[0].uv2);
                    //AfxUploadGeometry(mshb.geo, uv2AttrIdx, 0, mshb.vtxCnt, in, sizeof(afxV2d));
                }

                afxBool tristriped = geoHdr.fmtFlags & 0x1;
                // if NOT NATIVE, read triangles here. Per-triangle struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2 }. Not mistyped.
                //afxUnit* triangles = AfxGetMeshIndices(meshes[geoIdx], 0);

                dff.clumps[0].gl.geometries[0].triangles = AfxAllocate(geoHdr.totalTriangles * 3, sizeof(afxUnit16), 0, AfxHere());

                if (tristriped)
                {
                    for (afxUnit i = 0, total = geoHdr.totalTriangles; i < total; i++)
                    {
                        struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2; } tri;
                        AfxReadStream(in, sizeof(tri), 0, &tri);

                        if (i % 2 == 0)
                        {
                            // Even index: v0, v1, v2
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 0] = tri.vtx0;
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 1] = tri.vtx1;
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 2] = tri.vtx2;
                        }
                        else
                        {
                            // Odd index: v1, v2, v0
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 0] = tri.vtx1;
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 1] = tri.vtx2;
                            dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 2] = tri.vtx0;
                        }
                    }
                }
                else
                {
                    for (afxUnit i = 0, total = geoHdr.totalTriangles; i < total; i++)
                    {
                        struct { afxUnit16 vtx1, vtx0, mtlIdx, vtx2; } tri;
                        AfxReadStream(in, sizeof(tri), 0, &tri);

                        dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 0] = tri.vtx0;
                        dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 1] = tri.vtx1;
                        dff.clumps[0].gl.geometries[0].triangles[geoHdr.totalTriangles + 2] = tri.vtx2;
                    }
                }
            }

            // for each morph target
            for (afxUnit32 morphIdx = 0; morphIdx < geoHdr.totalMorphs; ++morphIdx)
            {
                struct
                {
                    afxSphere   bounding; // afxV3d + afxReal32
                    afxBool32   hasVertices;
                    afxBool32   hasNormals;
                } morph;
                AfxReadStream(in, sizeof(morph), 0, &morph);

                dff.clumps[0].gl.geometries[0].morphes[morphIdx].bounding = morph.bounding;
                dff.clumps[0].gl.geometries[0].morphes[morphIdx].hasNormals = morph.hasNormals;
                dff.clumps[0].gl.geometries[0].morphes[morphIdx].hasVertices = morph.hasVertices;

                afxGeometry geo;
                afxGeometryInfo geoi = { 0 };
                geoi.vtxCnt = geoHdr.totalVertices;
                geoi.attrCnt = attrCnt;

                afxUnit posAttrIdx = AFX_INVALID_INDEX, nrmAttrIdx = AFX_INVALID_INDEX;

                if (morph.hasVertices)
                    posAttrIdx = geoi.attrCnt++;

                if (morph.hasNormals)
                    nrmAttrIdx = geoi.attrCnt++;

                if (morph.hasVertices)
                {
                    // if hasVertices, read the array of per-vertex afxV3d position here.
                    //AfxUploadGeometry(geo, posAttrIdx, 0, geoi.vtxCnt, in, sizeof(afxV3d));
                    dff.clumps[0].gl.geometries[0].morphes[morphIdx].vertices = AfxAllocate(geoHdr.totalVertices, sizeof(afxV3d), 0, AfxHere());
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV3d), 0, dff.clumps[0].gl.geometries[0].morphes[morphIdx].vertices);
                }

                if (morph.hasNormals)
                {
                    // if hasNormals, read the array of per-vertex afxV3d normal here.
                    dff.clumps[0].gl.geometries[0].morphes[morphIdx].normals = AfxAllocate(geoHdr.totalVertices, sizeof(afxV3d), 0, AfxHere());
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxV3d), 0, dff.clumps[0].gl.geometries[0].morphes[morphIdx].normals);
                    //AfxUploadGeometry(geo, nrmAttrIdx, 0, geoi.vtxCnt, in, sizeof(afxV3d));
                }
            }

            cid = AfxPullStreamChunk(in, &hdr);
            AFX_ASSERT(cid == afxChunkId_RW_MTLL);
            {
                cid = AfxPullStreamChunk(in, &hdr);
                AFX_ASSERT(cid == afxChunkId_STRUCT);
                afxUnit32 totalMtlAndInsts;
                AfxReadStream(in, sizeof(totalMtlAndInsts), 0, &totalMtlAndInsts);
                dff.clumps[0].gl.geometries[0].ml.totalMtlAndInsts = totalMtlAndInsts;
                AfxAdvanceStream(in, sizeof(afxUnit32) * totalMtlAndInsts);

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

                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].flags = mtl.flags;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].rgba8 = mtl.rgba8;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].unused = mtl.unused;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].texCnt = mtl.texCnt;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].ambient = mtl.ambient;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].specular = mtl.specular;
                    dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].diffuse = mtl.diffuse;

                    {
                        cid = AfxPullStreamChunk(in, &hdr);
                        AFX_ASSERT(cid == afxChunkId_RW_TEX);
                        cid = AfxPullStreamChunk(in, &hdr);
                        AFX_ASSERT(cid == afxChunkId_STRUCT);
                        afxUnit32 filterAddressFlags;
                        AfxReadStream(in, sizeof(filterAddressFlags), 0, &filterAddressFlags);
                        dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].filterAddressFlags = filterAddressFlags;

                        cid = AfxPullStreamChunk(in, &hdr);
                        AFX_ASSERT(cid == afxChunkId_STRING);
                        AfxMakeString32(&dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].name, NIL);
                        dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].name.str.len = hdr.siz;
                        AfxReadStream(in, hdr.siz, 0, &dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].name.buf);

                        cid = AfxPullStreamChunk(in, &hdr);
                        AFX_ASSERT(cid == afxChunkId_STRING);
                        AfxMakeString32(&dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].mask, NIL);
                        dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].mask.str.len = hdr.siz;
                        AfxReadStream(in, hdr.siz, 0, &dff.clumps[0].gl.geometries[0].ml.materials[mtlIdx].textures[0].mask.buf);

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

                    dff.clumps[0].gl.geometries[0].meshExt.flags = bm.flags;
                    dff.clumps[0].gl.geometries[0].meshExt.totalIndices = bm.totalIndices;
                    dff.clumps[0].gl.geometries[0].meshExt.totalMeshes = bm.totalMeshes;

                    dff.clumps[0].gl.geometries[0].meshExt.indices = AfxAllocate(dff.clumps[0].gl.geometries[0].meshExt.totalIndices, sizeof(afxUnit32), 0, AfxHere());

                    afxUnit baseIdx = 0;

                    // for each mesh in totalMeshes
                    for (afxUnit bmIdx = 0; bmIdx < bm.totalMeshes; bmIdx++)
                    {
                        struct
                        {
                            afxUnit32 totalIndices;
                            afxUnit32 materialIdx;
                        } bms;
                        AfxReadStream(in, sizeof(bms), 0, &bms);

                        dff.clumps[0].gl.geometries[0].meshExt.surfToMtlMap[bmIdx] = bms.materialIdx;
                        dff.clumps[0].gl.geometries[0].meshExt.baseTrisForSurfMap[bmIdx] = baseIdx;
                        dff.clumps[0].gl.geometries[0].meshExt.trisForSurfMap[bmIdx] = bms.totalIndices;

                        // for each index in (ABOVE) totalIndices, read the afxUnit32-long index data.
                        AfxReadStream(in, bms.totalIndices * sizeof(afxUnit32), 0, &dff.clumps[0].gl.geometries[0].meshExt.indices[baseIdx]);
                        baseIdx += bms.totalIndices;
                    }
                }
                else if (cid == /*rwSkinPLG*/0x116)
                {
                    struct
                    {
                        afxUnit8 totalBoneMatrices;
                        afxUnit8 totalUsedBones;
                        afxUnit8 maxWeightPerVtx;
                        afxUnit8 unused;
                    } ski;
                    AfxReadStream(in, sizeof(ski), 0, &ski);

                    dff.clumps[0].gl.geometries[0].skinExt.maxWeightPerVtx = ski.maxWeightPerVtx;
                    dff.clumps[0].gl.geometries[0].skinExt.totalBoneMatrices = ski.totalBoneMatrices;
                    dff.clumps[0].gl.geometries[0].skinExt.totalUsedBones = ski.totalUsedBones;
                    dff.clumps[0].gl.geometries[0].skinExt.unused = ski.unused;

                    // for each totalUsedBones, read the afxUnit8-long index data.
                    afxUnit8 bones[256];
                    AfxReadStream(in, ski.totalUsedBones * sizeof(afxUnit8), 0, dff.clumps[0].gl.geometries[0].skinExt.bones);

                    // for each geometry.totalVertices, read the afxUnit8[4] nest of bone indices.
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxUnit8[4]), 0, dff.clumps[0].gl.geometries[0].skinExt.pivots);
                    
                    // for each geometry.totalVertices, read the afxReal32[4] nest of matrix weights.
                    AfxReadStream(in, geoHdr.totalVertices * sizeof(afxReal32[4]), 0, dff.clumps[0].gl.geometries[0].skinExt.weights);

                    // for each totalBoneMatrices, read the afxM4d inverse matrix.
                    AfxReadStream(in, ski.totalBoneMatrices * sizeof(afxM4d), 0, dff.clumps[0].gl.geometries[0].skinExt.iws);

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

            afxGeometry geo;
            afxGeometryInfo geoi = { 0 };
            geoi.vtxCnt = geoHdr.totalVertices;
            geoi.attrCnt = attrCnt;
            AfxAcquireGeometries(din, 1, &geoi, &geo);
            AfxResetGeometry(geo, posAttrIdx, afxVertexFormat_V3D, afxVertexFlag_ATV, &AfxString("pos"));
            AfxResetGeometry(geo, nrmAttrIdx, afxVertexFormat_V3D, afxVertexFlag_LTM, &AfxString("nrm"));

            if (posAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, posAttrIdx, afxVertexFormat_V3D, NIL, &AfxString("pos"));
                AfxUpdateGeometry(geo, posAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].morphes[0].vertices, sizeof(afxV3d));
                AfxDeallocate(dff.clumps[0].gl.geometries[0].morphes[0].vertices);
                dff.clumps[0].gl.geometries[0].morphes[0].vertices = NIL;
            }

            if (nrmAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, nrmAttrIdx, afxVertexFormat_V3D, NIL, &AfxString("nrm"));
                AfxUpdateGeometry(geo, nrmAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].morphes[0].normals, sizeof(afxV3d));
                AfxDeallocate(dff.clumps[0].gl.geometries[0].morphes[0].normals);
                dff.clumps[0].gl.geometries[0].morphes[0].normals = NIL;
            }

            if (rgbaAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, rgbaAttrIdx, afxVertexFormat_U32, NIL, &AfxString("rgba"));
                AfxUpdateGeometry(geo, rgbaAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].rgba, sizeof(afxUnit32));
                AfxDeallocate(dff.clumps[0].gl.geometries[0].rgba);
                dff.clumps[0].gl.geometries[0].rgba = NIL;
            }

            if (uvAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, uvAttrIdx, afxVertexFormat_V2D, NIL, &AfxString("uv"));
                AfxUpdateGeometry(geo, uvAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].uv, sizeof(afxV2d));
                AfxDeallocate(dff.clumps[0].gl.geometries[0].uv);
                dff.clumps[0].gl.geometries[0].uv = NIL;
            }

            if (uv2AttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, uv2AttrIdx, afxVertexFormat_V2D, NIL, &AfxString("uv0"));
                AfxUpdateGeometry(geo, uv2AttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].uv2, sizeof(afxV2d));
                AfxDeallocate(dff.clumps[0].gl.geometries[0].uv2);
                dff.clumps[0].gl.geometries[0].uv2 = NIL;
            }

            if (pvtAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, pvtAttrIdx, afxVertexFormat_U8x4, NIL, &AfxString("pvt"));
                AfxUpdateGeometry(geo, pvtAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].skinExt.pivots, sizeof(dff.clumps[0].gl.geometries[0].skinExt.pivots[0]));
            }

            if (wgtAttrIdx != AFX_INVALID_INDEX)
            {
                AfxResetGeometry(geo, wgtAttrIdx, afxVertexFormat_V4D, NIL, &AfxString("wgt"));
                AfxUpdateGeometry(geo, wgtAttrIdx, 0, geoi.vtxCnt, dff.clumps[0].gl.geometries[0].skinExt.weights, sizeof(dff.clumps[0].gl.geometries[0].skinExt.weights[0]));
            }

            afxMeshBlueprint mshb = { 0 };
            mshb.geo = geo;
            mshb.vtxCnt = geoHdr.totalVertices;
            mshb.triCnt = geoHdr.totalTriangles;
            mshb.secCnt = dff.clumps[0].gl.geometries[0].meshExt.totalMeshes;
            mshb.trisForSurfMap = dff.clumps[0].gl.geometries[0].meshExt.trisForSurfMap;
            mshb.baseTrisForSurfMap = dff.clumps[0].gl.geometries[0].meshExt.baseTrisForSurfMap;
            mshb.surfToMtlMap = dff.clumps[0].gl.geometries[0].meshExt.surfToMtlMap;
            mshb.extraMorphCnt = geoHdr.totalMorphs - 1;
            AfxAssembleMeshes(din, 1, &mshb, &meshes[geoIdx]);

            for (afxUnit bmIdx = 0; bmIdx < dff.clumps[0].gl.geometries[0].meshExt.totalMeshes; bmIdx++)
            {
                AfxUpdateMeshIndices(meshes[geoIdx], dff.clumps[0].gl.geometries[0].meshExt.baseTrisForSurfMap[bmIdx] / 3, dff.clumps[0].gl.geometries[0].meshExt.trisForSurfMap[bmIdx] / 3, dff.clumps[0].gl.geometries[0].meshExt.indices, sizeof(afxUnit32));
            }

            for (afxUnit morphIdx = 1; morphIdx < geoHdr.totalMorphs; morphIdx++) // start at 1
            {
                afxMeshMorph mshm = { 0 };
                mshm.vtxCnt = geoHdr.totalVertices;
                mshm.geo = geo;
                AfxChangeMeshMorphes(meshes[geoIdx], morphIdx, 1, &mshm);
                AfxReleaseObjects(1, &geo);
            }
        }

        afxString joints[256];

        for (afxUnit i = 0; i < dff.clumps[0].fl.totalFrames; i++)
        {
            AfxReflectString(&dff.clumps[0].fl.frames[i].name.str, &joints[i]);
        }

        afxUri urn = { 0 };
        //AfxWrapUriString(&uri, &dff.clumps[0]);
        // O nome não entrou na função

        afxModel mdl;
        afxModelBlueprint mdlb = { 0 };
        mdlb.skl.jointCnt = dff.clumps[0].fl.totalFrames;
        mdlb.skl.joints = joints;
        AfxMakeString32(&mdlb.skl.urn, &urn.str);
        mdlb.rigCnt = clumpHdr.totalAtomics;
        AfxAssembleModels(sim, NIL, 1, &mdlb, &mdl);

        afxSkeleton skl;
        AfxGetModelSkeleton(mdl, &skl);

        for (afxUnit i = 0; i < dff.clumps[0].fl.totalFrames; i++)
        {
            afxQuat q;
            afxTransform t;
            AfxQuatRotationFromM3d(q, dff.clumps[0].fl.frames[i].m);
            AfxSetTransform(&t, dff.clumps[0].fl.frames[i].m[3], q, AFX_M3D_IDENTITY, TRUE);
            AfxResetSkeletonTransforms(skl, i, 1, &t);

            AfxReparentSkeletonJoints(skl, i, 1, &dff.clumps[0].fl.frames[i].parentFrame, sizeof(afxUnit32));

            AfxResetSkeletonMatrices(skl, i, 1, dff.clumps[0].gl.geometries[0].skinExt.iws[i], sizeof(afxM4d));
        }

        AfxRigMeshes(mdl, 0, dff.clumps[0].gl.totalGeometries, meshes, NIL);

        *model = mdl;

        int a = 1;

    }
    return err;
}

_AMX afxError TestDffLoad(afxSimulation sim, afxDrawInput din, afxUri const* uri, afxModel* model)
{

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    AfxAcquireStream(1, &iobi, &iob);
    AfxReopenFile(iob, uri, afxFileFlag_R);

    afxModel mdl;
    LoadRwDff(sim, din, iob, &mdl);
    *model = mdl;

    AfxReleaseObjects(1, &iob);
}
