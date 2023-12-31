#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "../e2coree/deps/granny2/granny.h"
#pragma comment(lib, "../../deps/granny2/granny2")

#include "afxGranny2Model.h"
#include "qwadro/sim/modeling/awxMesh.h"
#include "qwadro/sim/awxNode.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sim/afxSimulation.h"

void PrintFileStatistics(granny_file* GrannyFile)
{
    afxError err = NIL;
    AfxAssert(GrannyFile);
    AfxComment("granny_file\n-----------\n");

    /* Each Granny file contains 1 or more sections that may be loaded, freed, and
       compressed independantly.  By default, the exporter creates several, in order to
       try to segregate ephemeral data that is loaded into the rendering API (for
       instance, vertex buffers and textures) from data that Granny needs to access at
       runtime.  This allows you to release the memory for the ephemeral data if you need
       to save on memory at runtime.  Use the $FreeFileSection API to do this.
     */
    granny_grn_section* SectionArray = GrannyGetGRNSectionArray(GrannyFile->Header);
    AfxAssert(SectionArray);

    AfxComment("File contains: %d sections.\n", GrannyFile->SectionCount);

    {
        for (granny_int32x Section = 0; Section < GrannyFile->SectionCount; ++Section)
        {
            if (GrannyFile->Sections[Section])
            {
                AfxComment("  Section %d: present", Section);

                // DataSize is the sizeof the data on disk, ExpandedDataSize is the size in memory.  If
                // they match, this section was written without Oodle1 compression.
                if (SectionArray[Section].DataSize == SectionArray[Section].ExpandedDataSize)
                {
                    AfxComment(" (uncompressed)\n");
                }
                else
                {
                    AfxComment(" (compressed)\n");
                }
            }
            else
            {
                // This will likely never happen, since we haven't called GrannyFreeFileSection
                AfxComment("  Section %d: already freed (or empty)\n", Section);
            }
        }
    }

    AfxComment("\n");
}

void PrintFileInfoStatistics(granny_file_info* GrannyFileInfo)
{
    afxError err = NIL;
    AfxAssert(GrannyFileInfo);
    AfxComment("granny_file_info\n----------------\n");

    /* If the $art_tool_info is present, show some relevant facts about the file.  See
       $TutorialTransformFile for details on how Granny uses this information to compute
       automatic basis conversion transforms. */
    if (GrannyFileInfo->ArtToolInfo)
    {
        granny_art_tool_info* ToolInfo = GrannyFileInfo->ArtToolInfo;

        AfxComment("File created by %s (%d.%d)\n", ToolInfo->FromArtToolName, ToolInfo->ArtToolMajorRevision, ToolInfo->ArtToolMinorRevision);
        AfxComment("  Coordinate system\n"
            "    Units per meter: %f\n"
            "       Right Vector: (%f %f %f)\n"
            "          Up Vector: (%f %f %f)\n"
            "        Back Vector: (%f %f %f)\n\n",
            ToolInfo->UnitsPerMeter,
            ToolInfo->RightVector[0], ToolInfo->RightVector[1], ToolInfo->RightVector[2],
            ToolInfo->UpVector[0], ToolInfo->UpVector[1], ToolInfo->UpVector[2],
            ToolInfo->BackVector[0], ToolInfo->BackVector[1], ToolInfo->BackVector[2]);
    }
    else
    {
        AfxComment("No Art tool information, GrannyComputeBasisConversion won't work on this file.\n");
    }

    /* The ExporterInfo member of $file_info encodes information about the version of Granny
       that created this file.  Note that you canv remove this member with the preprocessor
       or your own tools without affecting Granny's ability to load files created with
       older versions of the API. */
    if (GrannyFileInfo->ExporterInfo)
    {
        AfxComment("File created with version %d.%d.%d.%d of Granny.\n",
            GrannyFileInfo->ExporterInfo->ExporterMajorRevision,
            GrannyFileInfo->ExporterInfo->ExporterMinorRevision,
            GrannyFileInfo->ExporterInfo->ExporterCustomization,
            GrannyFileInfo->ExporterInfo->ExporterBuildNumber);
    }

    /* FromFileName encodes the name of the source file that was used to create the file.
       For instance, in the case of the default file, Gryphon.mb, which is the Maya file
       containing the model for the Gryphon example model. */
    if (GrannyFileInfo->FromFileName)
    {
        AfxComment("Source file: %s\n\n", GrannyFileInfo->FromFileName);
    }

    /* Now dump the number of some key structures contained in the $file_info.  Note that
       for convenience, several more arrays are exposed at the $file_info level.  For
       instance, $(skeleton)s canv be found in this structure.  We're not dumping these
       because they are slightly redundant, you canv find them as sub-members of the major
       structures.  Skeletons canv be found as sub-members of the Models, for example. */
    AfxComment("Number of    Textures: %d\n"
        "            Materials: %d\n"
        "               Meshes: %d\n"
        "               Models: %d\n"
        "           Animations: %d\n",
        GrannyFileInfo->TextureCount,
        GrannyFileInfo->MaterialCount,
        GrannyFileInfo->MeshCount,
        GrannyFileInfo->ModelCount,
        GrannyFileInfo->AnimationCount);
    AfxComment("\n");
}

afxResult AfxLoadAssetsFromGranny2File(afxSimulation sim, afxUri const *uri, afxArray *assets)
{
    afxError err = NIL;
    AfxEntry("sim=%p", sim);

    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxMmu* allocator = AfxGetSimulationMemory(sim);
    (void)allocator;

    granny_file* GrannyFile = GrannyReadEntireFile(AfxGetUriData(uri, 0));
    if (GrannyFile == 0)
    {
        AfxError("Error: unable to load %s as a Granny file.\n", AfxGetUriData(uri, 0));
        return EXIT_FAILURE;
    }
    else
    {
        PrintFileStatistics(GrannyFile);
    }

    granny_file_info* Info = GrannyGetFileInfo(GrannyFile);
    if (Info == 0)
    {
        AfxError("Could not extract a granny_file_info from the file.\n");
        return EXIT_FAILURE;
    }
    else
    {
        PrintFileInfoStatistics(Info);
    }

    for (granny_int32x ModelIdx = 0; ModelIdx < Info->ModelCount; ++ModelIdx)
    {
        granny_model* GrannyModel = Info->Models[ModelIdx];
        AfxAssert(GrannyModel);

        afxM4d InitialMatrix;
        GrannyBuildCompositeTransform4x4(&GrannyModel->InitialPlacement, (granny_real32 *)InitialMatrix);

        afxArray mshbs;
        mshbs = AfxAllocateArray(GrannyModel->MeshBindingCount, sizeof(awxMeshBlueprint), NIL);
        AfxReserveArraySpace(&mshbs, GrannyModel->MeshBindingCount);

        /* Create the meshes that are bound to this model. */
        for (granny_int32x MeshIdx = 0; MeshIdx < GrannyModel->MeshBindingCount; ++MeshIdx)
        {
            granny_mesh* GrannyMesh = GrannyModel->MeshBindings[MeshIdx].Mesh;
            AfxAssert(GrannyMesh);
            granny_int32 GroupCount = GrannyGetMeshTriangleGroupCount(GrannyMesh);
            granny_int32 BoneBindingCount = GrannyMesh->BoneBindingCount;
            granny_int32 MaterialBindingCount = GrannyMesh->MaterialBindingCount;

            afxUri mshName;
            AfxMakeUri(&mshName, GrannyMesh->Name, 0);
            int vtxcnt = GrannyGetMeshVertexCount(GrannyMesh);
            int idxcnt = GrannyGetMeshIndexCount(GrannyMesh);

            afxNat mshIdx2;
            awxMeshBlueprint *mshb = AfxInsertArrayUnit(&mshbs, &mshIdx2);
            AfxMeshBlueprintDeploy(mshb, sim, &mshName, vtxcnt, 1, GroupCount, BoneBindingCount, MaterialBindingCount);

            void* vertices = GrannyGetMeshVertices(GrannyMesh);
            granny_data_type_definition*vtxType = GrannyGetMeshVertexType(GrannyMesh);
            int bufferSize;
            void* VertexBuffer = NIL;

            if (GrannyMeshIsRigid(GrannyMesh))
            {
                bufferSize = vtxcnt * sizeof(granny_pnt332_vertex);
                VertexBuffer = AfxAllocate(NIL, 1, bufferSize, 0, AfxHint());
                AfxAssert(VertexBuffer);
                GrannyCopyMeshVertices(GrannyMesh, GrannyPNT332VertexType, VertexBuffer);

                //for (int i = 0; i < vtxcnt; i++)
                {
                    granny_pnt332_vertex* vtx = VertexBuffer;

                    afxV4d* xyzw = AfxAllocate(NIL, sizeof(afxV4d), vtxcnt, AfxCookHint());
                    afxV4d v;

                    for (int i = 0; i < vtxcnt; i++)
                        AfxCopyV4d(AfxSetV4dV3d(vtx[i].Position, v), xyzw[i]);

                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_XYZW], vtxcnt, sizeof(afxV4d), xyzw);
                    AfxDeallocate(NIL, xyzw);

                    msh->vertices[AFX_VERTEX_ATTR_NRM] = AfxBuffer_Acquire(vtxcnt * sizeof(afxV3d));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_NRM], vtxcnt, sizeof(afxV3d), vtx->Normal);

                    msh->vertices[AFX_VERTEX_ATTR_UV] = AfxBuffer_Acquire(vtxcnt * sizeof(afxV2d));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_UV], vtxcnt, sizeof(afxV2d), vtx->UV);

                }

                AfxDeallocate(NIL, VertexBuffer);
            }
            else
            {
                bufferSize = vtxcnt * sizeof(granny_pwnt3432_vertex);
                VertexBuffer = AfxAllocate(NIL, 1, bufferSize, AfxCookHint());
                AfxAssert(VertexBuffer);
                GrannyCopyMeshVertices(GrannyMesh, GrannyPWNT3432VertexType, VertexBuffer);

                //for (int i = 0; i < vtxcnt; i++)
                {
                    granny_pwnt3432_vertex* vtx = VertexBuffer;

                    afxV4d* xyzw = AfxAllocate(NIL, sizeof(afxV4d), vtxcnt, AfxCookHint());
                    afxV4d v;

                    for (int i = 0; i < vtxcnt; i++)
                        AfxCopyV4d(AfxSetV4dV3d(vtx[i].Position, v), xyzw[i]);

                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_XYZW], vtxcnt, sizeof(afxV4d), xyzw);
                    AfxDeallocate(NIL, xyzw);

                    msh->vertices[AFX_VERTEX_ATTR_NRM] = AfxBuffer_Acquire(vtxcnt * sizeof(afxV3d));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_NRM], vtxcnt, sizeof(afxV3d), vtx->Normal);

                    msh->vertices[AFX_VERTEX_ATTR_UV] = AfxBuffer_Acquire(vtxcnt * sizeof(afxV2d));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_UV], vtxcnt, sizeof(afxV2d), vtx->UV);

                    msh->vertices[AFX_VERTEX_ATTR_WWWW] = AfxBuffer_Acquire(vtxcnt * sizeof(afxU8[4]));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_WWWW], vtxcnt, sizeof(afxU8[4]), vtx->UV);

                    msh->vertices[AFX_VERTEX_ATTR_IIII] = AfxBuffer_Acquire(vtxcnt * sizeof(afxU8[4]));
                    AfxBuffer_Write(msh->vertices[AFX_VERTEX_ATTR_IIII], vtxcnt, sizeof(afxU8[4]), vtx->UV);
                }

                AfxDeallocate(NIL, VertexBuffer);
            }

            // Create the index buffer
            {
                AfxMeshBlueprintSetTopology(mshb, afxPrimTopology_TRI_LIST, idxcnt, GrannyGetMeshIndices(GrannyMesh), GrannyGetMeshBytesPerIndex(GrannyMesh), 0);

                // Get the triangle groups
                granny_tri_material_group *Groups = GrannyGetMeshTriangleGroups(GrannyMesh);

                for (int i = 0; i < GroupCount; i++)
                {
                    AfxMeshBlueprintAddSection(mshb, NIL, Groups[i].MaterialIndex, Groups[i].TriFirst * 3, Groups[i].TriCount * 3, NIL, 0, 0);                    
                }

                for (int i = 0; i < MaterialBindingCount; i++)
                {
                    granny_material* Material = GrannyMesh->MaterialBindings[i].Material;
                    
                    afxUri mtlName, texName;
                    AfxMakeUri(&mtlName, Material->Name, 0);
                    AfxMakeUri(&texName, Material->Texture ? Material->Texture->FromFileName : 0, 0);
                    awxMaterialBlueprint mtlb;
                    AfxMaterialBlueprintDeploy(&mtlb, sim, &mtlName, Material->MapCount, &texName);
#if 0
                    for (granny_int32x MapIdx = 0; MapIdx < Material->MapCount; ++MapIdx)
                    {
                        granny_material_map* Map = &Material->Maps[MapIdx];
                        granny_texture* Texture = Map->Material->Texture;
                        avxRaster* tex = (Texture) ? AfxAcquireTexture(sim->dsys, Texture->Width ? Texture->Width : 512, Texture->Height ? Texture->Height : 512) : NIL;

                        if (tex)
                        {
                            afxChar texPath[2048];
                            AfxFormatString(texPath, "%s/%s", fpath, AfxFindPathObject(Texture->FromFileName));
                            AfxAdvertise("IMG NAME %s", texPath);

                            afxImage* img = AfxImage_Acquire(sim->dsys, tex->width, tex->height, AFX_RGBA8);
                            AfxAddTextureMap(tex, Map->Usage, img);
                            AfxReleaseObject(&img->obj);

                            AfxInputRasterMapFromFile(tex, Map->Usage, texPath);
                            //AfxRenameTexture(tex, Map->Material->Texture->FromFileName);
                        }

                        if (tex)
                        {
                            AfxAddMaterialMap(mtl, Map->Usage, tex);
                            AfxReleaseObject(&tex->obj);
                        }
                    }
#endif
                    awxMaterial mtl;
                    AfxBuildMaterials(sim, 1, &mtl, &mtlb);
                    AfxMaterialBlueprintDiscard(&mtlb);
                    AfxMeshBlueprintAddMaterial(mshb, mtl);
                }

                for (int i = 0; i < BoneBindingCount; i++)
                {
                    granny_bone_binding* bb = &GrannyMesh->BoneBindings[i];

                    afxString boneName;
                    AfxMakeString(&boneName, bb->BoneName, 0);
                    AfxMeshBlueprintAddArticulation(mshb, &boneName, bb->TriangleCount, bb->TriangleIndices);
                }
            }
        }

        for (afxNat i = 0; i < AfxCountArrayElements(&mshbs); i++)
        {
            awxMeshBlueprint*mshb2 = AfxGetArrayUnit(&mshbs, i);
            AfxMeshBlueprintDiscard(mshb2);
        }

        AfxDeallocateArray(&mshbs);
    }

    for (granny_int32x AnimIdx = 0; AnimIdx < Info->AnimationCount; ++AnimIdx)
    {
        granny_animation* GrannyAnim = Info->Animations[AnimIdx];
        AfxAssert(GrannyAnim);

        for (granny_int32 TrackIdx = 0; TrackIdx < GrannyAnim->TrackGroupCount; TrackIdx++)
        {
            granny_track_group* TrackGroup = GrannyAnim->TrackGroups[TrackIdx];


        }
    }

    GrannyFreeFile(GrannyFile);
    return NIL;
}
