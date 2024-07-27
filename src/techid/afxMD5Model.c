

#define _CRT_SECURE_NO_WARNINGS 1

#define _AFX_SKELETON_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/exec/afxSystem.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/cad/afxMeshTopology.h"
#include "afxMD5Model.h"
#include <stdio.h>
#include <string.h>

//#include "afxMD5Model.h"

 /* Vectors */
typedef float vec2_t[2];
typedef float vec3_t[3];

/* Quaternion (x, y, z, w) */
typedef float quat4_t[4];

enum {
    X, Y, Z, W
};

/* Joint */
struct md5_joint_t
{
    char name[64];
    int parent;

    afxV3d pos;
    afxQuat orient;
};

/* Vertex */
struct md5_vertex_t
{
    afxV2d st;

    unsigned int start; /* start weight */
    unsigned int count; /* weight count */
};

/* Triangle */
struct md5_triangle_t
{
    unsigned int index[3];
};

/* Weight */
struct md5_weight_t
{
    unsigned int joint;
    afxReal bias;

    afxV3d pos;
};

/* Bounding box */
struct md5_bbox_t
{
    afxV3d min;
    afxV3d max;
};

/* MD5 mesh */
struct md5_mesh_t
{
    struct md5_vertex_t *vertices;
    akxIndexedTriangle *triangles;
    struct md5_weight_t *weights;

    unsigned int num_verts;
    unsigned int num_tris;
    unsigned int num_weights;

    char shader[256];
};

/* MD5 model structure */
struct md5_model_t
{
    struct md5_joint_t *baseSkel;
    struct md5_mesh_t *meshes;

    unsigned int num_joints;
    unsigned int num_meshes;
};

/* Animation data */
struct md5_anim_t
{
    int num_frames;
    int num_joints;
    int frameRate;

    struct md5_joint_t **skelFrames;
    struct md5_bbox_t *bboxes;
};

/* Animation info */
struct anim_info_t
{
    int curr_frame;
    int next_frame;

    double last_time;
    double max_time;
};

/**
 * Basic quaternion operations.
 */

void
Quat_computeW(quat4_t q)
{
    float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);

    if (t < 0.0f)
        q[W] = 0.0f;
    else
        q[W] = -sqrt(t);
}

void
Quat_normalize(quat4_t q)
{
    /* compute magnitude of the quaternion */
    float mag = sqrt((q[X] * q[X]) + (q[Y] * q[Y])
        + (q[Z] * q[Z]) + (q[W] * q[W]));

    /* check for bogus length, to protect against divide by zero */
    if (mag > 0.0f)
    {
        /* normalize it */
        float oneOverMag = 1.0f / mag;

        q[X] *= oneOverMag;
        q[Y] *= oneOverMag;
        q[Z] *= oneOverMag;
        q[W] *= oneOverMag;
    }
}

void
Quat_multQuat(const quat4_t qa, const quat4_t qb, quat4_t out)
{
    out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z]);
    out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y]);
    out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z]);
    out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X]);
}

void
Quat_multVec(const quat4_t q, const vec3_t v, quat4_t out)
{
    out[W] = -(q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z]);
    out[X] = (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y]);
    out[Y] = (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z]);
    out[Z] = (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X]);
}

void
Quat_rotatePoint(const quat4_t q, const vec3_t in, vec3_t out)
{
    quat4_t tmp, inv, final;

    inv[X] = -q[X]; inv[Y] = -q[Y];
    inv[Z] = -q[Z]; inv[W] = q[W];

    Quat_normalize(inv);

    Quat_multVec(q, in, tmp);
    Quat_multQuat(tmp, inv, final);

    out[X] = final[X];
    out[Y] = final[Y];
    out[Z] = final[Z];
}

int ReadMD5Model(const char *filename, struct md5_model_t *mdl)
{
    FILE *fp;
    char buff[512];
    int version;
    int curr_mesh = 0;
    afxNat i;

    fp = fopen(filename, "rb");
    if (!fp)
    {
        AfxLogError("Error: couldn't open \"%s\"!\n", filename);
        return 0;
    }

    while (!feof(fp))
    {
        /* Read whole line */
        fgets(buff, sizeof(buff), fp);

        if (sscanf(buff, " MD5Version %d", &version) == 1)
        {
            if (version != 10)
            {
                /* Bad version */
                AfxLogError("Error: bad model version\n");
                fclose(fp);
                return 0;
            }
        }
        else if (sscanf(buff, " numJoints %d", &mdl->num_joints) == 1)
        {
            if (mdl->num_joints > 0)
            {
                /* Allocate memory for base skeleton joints */
                mdl->baseSkel = AfxAllocate(mdl->num_joints, sizeof(struct md5_joint_t), 0, AfxHere());
            }
        }
        else if (sscanf(buff, " numMeshes %d", &mdl->num_meshes) == 1)
        {
            if (mdl->num_meshes > 0)
            {
                /* Allocate memory for meshes */
                mdl->meshes = AfxAllocate(mdl->num_meshes, sizeof(struct md5_mesh_t), 0, AfxHere());
            }
        }
        else if (strncmp(buff, "joints {", 8) == 0)
        {
            /* Read each joint */
            for (i = 0; i < mdl->num_joints; ++i)
            {
                struct md5_joint_t *joint = &mdl->baseSkel[i];

                /* Read whole line */
                fgets(buff, sizeof(buff), fp);

                if (sscanf(buff, "%s %d ( %f %f %f ) ( %f %f %f )",
                    joint->name, &joint->parent, &joint->pos[0],
                    &joint->pos[1], &joint->pos[2], &joint->orient[0],
                    &joint->orient[1], &joint->orient[2]) == 8)
                {
                    /* Compute the w component */
                    Quat_computeW(joint->orient);
                }
            }
        }
        else if (strncmp(buff, "mesh {", 6) == 0)
        {
            struct md5_mesh_t *mesh = &mdl->meshes[curr_mesh];
            int vert_index = 0;
            int tri_index = 0;
            int weight_index = 0;
            float fdata[4];
            int idata[3];

            while ((buff[0] != '}') && !feof(fp))
            {
                /* Read whole line */
                fgets(buff, sizeof(buff), fp);

                if (strstr(buff, "shader "))
                {
                    int quote = 0, j = 0;

                    /* Copy the shader name whithout the quote marks */
                    for (i = 0; i < (int)sizeof(buff) && (quote < 2); ++i)
                    {
                        if (buff[i] == '\"')
                            quote++;

                        if ((quote == 1) && (buff[i] != '\"'))
                        {
                            mesh->shader[j] = buff[i];
                            j++;
                        }
                    }
                }
                else if (sscanf(buff, " numverts %d", &mesh->num_verts) == 1)
                {
                    if (mesh->num_verts > 0)
                    {
                        /* Allocate memory for vertices */
                        mesh->vertices = AfxAllocate(mesh->num_verts, sizeof(struct md5_vertex_t), 0, AfxHere());
                    }

                    //if (mesh->num_verts > max_verts)
                        //max_verts = mesh->num_verts;
                }
                else if (sscanf(buff, " numtris %d", &mesh->num_tris) == 1)
                {
                    if (mesh->num_tris > 0)
                    {
                        /* Allocate memory for triangles */
                        mesh->triangles = AfxAllocate(mesh->num_tris, sizeof(struct md5_triangle_t), 0, AfxHere());
                    }

                    //if (mesh->num_tris > max_tris)
                        //max_tris = mesh->num_tris;
                }
                else if (sscanf(buff, " numweights %d", &mesh->num_weights) == 1)
                {
                    if (mesh->num_weights > 0)
                    {
                        /* Allocate memory for vertex weights */
                        mesh->weights = AfxAllocate(mesh->num_weights, sizeof(struct md5_weight_t), 0, AfxHere());
                    }
                }
                else if (sscanf(buff, " vert %d ( %f %f ) %d %d", &vert_index,
                    &fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
                {
                    /* Copy vertex data */
                    mesh->vertices[vert_index].st[0] = fdata[0];
                    mesh->vertices[vert_index].st[1] = fdata[1];
                    mesh->vertices[vert_index].start = idata[0];
                    mesh->vertices[vert_index].count = idata[1];
                }
                else if (sscanf(buff, " tri %d %d %d %d", &tri_index, &idata[0], &idata[1], &idata[2]) == 4)
                {
                    /* Copy triangle data */
                    mesh->triangles[tri_index][0] = idata[0];
                    mesh->triangles[tri_index][1] = idata[1];
                    mesh->triangles[tri_index][2] = idata[2];
                }
                else if (sscanf(buff, " weight %d %d %f ( %f %f %f )", &weight_index, &idata[0], &fdata[3], &fdata[0], &fdata[1], &fdata[2]) == 6)
                {
                    /* Copy vertex data */
                    mesh->weights[weight_index].joint = idata[0];
                    mesh->weights[weight_index].bias = fdata[3];
                    mesh->weights[weight_index].pos[0] = fdata[0];
                    mesh->weights[weight_index].pos[1] = fdata[1];
                    mesh->weights[weight_index].pos[2] = fdata[2];
                }
            }

            curr_mesh++;
        }
    }

    fclose(fp);

    return 1;
}

void
FreeModel(struct md5_model_t *mdl)
{
    afxNat i;

    if (mdl->baseSkel)
    {
        AfxDeallocate(mdl->baseSkel);
        mdl->baseSkel = NULL;
    }

    if (mdl->meshes)
    {
        /* Free mesh data */
        for (i = 0; i < mdl->num_meshes; ++i)
        {
            if (mdl->meshes[i].vertices)
            {
                AfxDeallocate(mdl->meshes[i].vertices);
                mdl->meshes[i].vertices = NULL;
            }

            if (mdl->meshes[i].triangles)
            {
                AfxDeallocate(mdl->meshes[i].triangles);
                mdl->meshes[i].triangles = NULL;
            }

            if (mdl->meshes[i].weights)
            {
                AfxDeallocate(mdl->meshes[i].weights);
                mdl->meshes[i].weights = NULL;
            }
        }

        AfxDeallocate(mdl->meshes);
        mdl->meshes = NULL;
    }
}

/**
 * Prepare a mesh for drawing.  Compute mesh's final vertex positions
 * given a skeleton.  Put the vertices in vertex arrays.
 */
void PrepareMesh(const struct md5_mesh_t *mesh, const struct md5_joint_t *skeleton, vec3_t *vertexArray, uint32_t *vertexIndices)
{
    afxNat i, j, k;

    /* Setup vertex indices */
    for (k = 0, i = 0; i < mesh->num_tris; ++i)
    {
        for (j = 0; j < 3; ++j, ++k)
            vertexIndices[k] = mesh->triangles[i][j];
    }

    /* Setup vertices */
    for (i = 0; i < mesh->num_verts; ++i)
    {
        vec3_t finalVertex = { 0.0f, 0.0f, 0.0f };

        /* Calculate final vertex to draw with weights */
        for (j = 0; j < mesh->vertices[i].count; ++j)
        {
            const struct md5_weight_t *weight = &mesh->weights[mesh->vertices[i].start + j];
            const struct md5_joint_t *joint = &skeleton[weight->joint];

            /* Calculate transformed vertex for this weight */
            vec3_t wv;
            Quat_rotatePoint(joint->orient, weight->pos, wv);

            /* The sum of all weight->bias should be 1.0 */
            finalVertex[0] += (joint->pos[0] + wv[0]) * weight->bias;
            finalVertex[1] += (joint->pos[1] + wv[1]) * weight->bias;
            finalVertex[2] += (joint->pos[2] + wv[2]) * weight->bias;
        }

        vertexArray[i][0] = finalVertex[0];
        vertexArray[i][1] = finalVertex[1];
        vertexArray[i][2] = finalVertex[2];
    }
}

DLLEXPORT afxError AfxLoadAssetsFromMd5(afxSimulation sim, afxFlags flags, afxUri const* file)
{
    afxError err = AFX_ERR_NONE;
    
    FILE *fp;
    char buff[512];
    int version;
    int curr_mesh = 0;
    afxNat i, num_joints = 0, num_meshes = 0;

    afxStringBase strb;
    AfxAcquireStringCatalogs(1, &strb);

    afxUri mdlName;
    afxUri2048 uri2;
    AfxMakeUri2048(&uri2, NIL);
    AfxClipUriTarget(&mdlName, file);
    AfxResolveUri(afxFileFlag_R, file, &uri2.uri);

    if (!(fp = fopen(AfxGetUriData(&uri2.uri, 0), "rb"))) AfxThrowError();
    else
    {
        struct md5_model_t md5;

        while (!feof(fp) && !err)
        {
            //AfxLogEcho(buff);
            /* Read whole line */
            fgets(buff, sizeof(buff), fp);

            if (sscanf(buff, " MD5Version %d", &version) == 1)
            {
                if (version != 10)
                {
                    /* Bad version */
                    AfxLogError("Error: bad model version\n");
                    fclose(fp);
                    return 0;
                }
            }
            else if (sscanf(buff, " numJoints %d", &md5.num_joints) == 1)
            {
                if (md5.num_joints > 0)
                {
                    /* Allocate memory for base skeleton joints */
                    md5.baseSkel = AfxAllocate(md5.num_joints, sizeof(struct md5_joint_t), 0, AfxHere());
                }
            }
            else if (sscanf(buff, " numMeshes %d", &md5.num_meshes) == 1)
            {
                if (md5.num_meshes > 0)
                {
                    /* Allocate memory for meshes */
                    md5.meshes = AfxAllocate(md5.num_meshes, sizeof(struct md5_mesh_t), 0, AfxHere());
                }
            }
            else if (strncmp(buff, "joints {", 8) == 0)
            {
                /* Read each joint */
                for (i = 0; i < md5.num_joints; ++i)
                {
                    struct md5_joint_t *joint = &md5.baseSkel[i];

                    /* Read whole line */
                    fgets(buff, sizeof(buff), fp);

                    if (sscanf(buff, "%s %d ( %f %f %f ) ( %f %f %f )",
                        joint->name, &joint->parent, &joint->pos[0],
                        &joint->pos[1], &joint->pos[2], &joint->orient[0],
                        &joint->orient[1], &joint->orient[2]) == 8)
                    {
                        /* Compute the w component */
                        Quat_computeW(joint->orient);
                    }
                }
            }
            else if (strncmp(buff, "mesh {", 6) == 0)
            {
                struct md5_mesh_t *mesh = &md5.meshes[curr_mesh];
                int vert_index = 0;
                int tri_index = 0;
                int weight_index = 0;
                float fdata[4];
                int idata[3];

                while ((buff[0] != '}') && !feof(fp))
                {
                    /* Read whole line */
                    fgets(buff, sizeof(buff), fp);

                    if (strstr(buff, "shader "))
                    {
                        int quote = 0, j = 0;

                        /* Copy the shader name whithout the quote marks */
                        for (i = 0; i < (int)sizeof(buff) && (quote < 2); ++i)
                        {
                            if (buff[i] == '\"')
                                quote++;

                            if ((quote == 1) && (buff[i] != '\"'))
                            {
                                mesh->shader[j] = buff[i];
                                j++;
                            }
                        }
                    }
                    else if (sscanf(buff, " numverts %d", &mesh->num_verts) == 1)
                    {
                        if (mesh->num_verts > 0)
                        {
                            /* Allocate memory for vertices */
                            mesh->vertices = AfxAllocate(mesh->num_verts, sizeof(mesh->vertices[0]), 0, AfxHere());
                        }

                        //if (mesh->num_verts > max_verts)
                            //max_verts = mesh->num_verts;
                    }
                    else if (sscanf(buff, " numtris %d", &mesh->num_tris) == 1)
                    {
                        if (mesh->num_tris > 0)
                        {
                            /* Allocate memory for triangles */
                            mesh->triangles = AfxAllocate(mesh->num_tris, sizeof(mesh->triangles[0]), 0, AfxHere());
                        }

                        //if (mesh->num_tris > max_tris)
                            //max_tris = mesh->num_tris;
                    }
                    else if (sscanf(buff, " numweights %d", &mesh->num_weights) == 1)
                    {
                        if (mesh->num_weights > 0)
                        {
                            /* Allocate memory for vertex weights */
                            mesh->weights = AfxAllocate(mesh->num_weights, sizeof(mesh->weights[0]), 0, AfxHere());
                        }
                    }
                    else if (sscanf(buff, " vert %d ( %f %f ) %d %d", &vert_index,
                        &fdata[0], &fdata[1], &idata[0], &idata[1]) == 5)
                    {
                        /* Copy vertex data */
                        mesh->vertices[vert_index].st[0] = fdata[0];
                        mesh->vertices[vert_index].st[1] = fdata[1];
                        mesh->vertices[vert_index].start = idata[0];
                        mesh->vertices[vert_index].count = idata[1];
                    }
                    else if (sscanf(buff, " tri %d %d %d %d", &tri_index, &idata[0], &idata[1], &idata[2]) == 4)
                    {
                        /* Copy triangle data */
                        mesh->triangles[tri_index][0] = idata[0];
                        mesh->triangles[tri_index][1] = idata[1];
                        mesh->triangles[tri_index][2] = idata[2];
                    }
                    else if (sscanf(buff, " weight %d %d %f ( %f %f %f )", &weight_index, &idata[0], &fdata[3], &fdata[0], &fdata[1], &fdata[2]) == 6)
                    {
                        /* Copy vertex data */
                        mesh->weights[weight_index].joint = idata[0];
                        mesh->weights[weight_index].bias = fdata[3];
                        mesh->weights[weight_index].pos[0] = fdata[0];
                        mesh->weights[weight_index].pos[1] = fdata[1];
                        mesh->weights[weight_index].pos[2] = fdata[2];
                    }
                }

                curr_mesh++;
            }
        }

        afxNat parentIdx[256];
        afxString pivots[256];
        afxTransform locals[256];
        akxSkeletonBlueprint sklb = { 0 };
        AfxMakeString32(&sklb.id, &mdlName.str);
        sklb.jointCnt = md5.num_joints;
        sklb.local = locals;
        sklb.jointId = pivots;
        sklb.parentIdx = parentIdx;

        for (afxNat j = 0; j < md5.num_joints; j++)
        {
            struct md5_joint_t *jnt = &md5.baseSkel[j];

            AfxMakeString(&pivots[j], 0, jnt->name, 0);
            parentIdx[j] = jnt->parent;
            AfxSetTransformWithIdentityCheck(&locals[j], jnt->pos, jnt->orient, AFX_M3D_IDENTITY);
        }

        afxSkeleton skl;
        AfxAssembleSkeletons(sim, 1, &sklb, &skl);
        AfxTryAssertObjects(1, &skl, afxFcc_SKL);

        afxNat totVtxCnt = 0;
        afxNat totTriCnt = 0;

        for (afxNat i = 0; i < md5.num_meshes; i++)
        {
            struct md5_mesh_t *mesh = &md5.meshes[i];

            totVtxCnt += mesh->num_verts;
            totTriCnt += mesh->num_tris;

        }

        afxArray meshRes;
        AfxAllocateArray(&meshRes, md5.num_meshes, sizeof(afxMeshBuilder), NIL);
        AfxReserveArraySpace(&meshRes, md5.num_meshes);

        for (afxNat i = 0; i < md5.num_meshes; i++)
        {
            struct md5_mesh_t *mesh = &md5.meshes[i];

            afxNat mshIdx;
            afxMeshBuilder* mshb = AfxInsertArrayUnits(&meshRes, 1, &mshIdx, NIL);
            AfxBeginMeshBuilding(mshb, &AfxStaticString(mesh->shader), mesh->num_verts, mesh->num_tris, 1, md5.num_joints);
            
            for (afxNat j = 0; j < md5.num_joints; j++)
            {
                struct md5_joint_t *jnt = &md5.baseSkel[j];

                afxString str;
                afxTransform t;
                AfxMakeString(&str, 0, jnt->name, 0);
                AfxRenameVertexPivots(mshb, j, 1, &str);
            }

            AfxReserveArraySpace(&mshb->biases, mesh->num_weights);

            for (afxNat j = 0; j < mesh->num_weights; j++)
            {
                struct md5_weight_t *w = &mesh->weights[j];
                AfxAssertRange(md5.num_joints, w->joint, 1);
                AfxAddVertexBiases(mshb, 1, (afxNat const[]) { w->joint }, (afxReal const[]) { w->bias });
            }

            for (afxNat j = 0; j < mesh->num_verts; j++)
            {
                struct md5_vertex_t *v = &mesh->vertices[j];
                afxV4d vp = { 0, 0, 0, 1 };

                afxNat baseBiasIdx = v->start;
                afxNat biasCnt = v->count;

                for (afxNat k = 0; k < v->count; k++)
                {
                    struct md5_weight_t *w = &mesh->weights[v->start + k];
                    AfxAssertRange(md5.num_joints, w->joint, 1);
                    AfxV3dAdd(vp, vp, w->pos);
                }
                AfxAssert(v->start == baseBiasIdx);
                AfxAssert(v->count == biasCnt);
                AfxUpdateVertices(mshb, j, 1, &baseBiasIdx, &biasCnt);
                AfxUpdateVertexPositions4(mshb, j, 1, &vp, sizeof(vp));
                AfxUpdateVertexWraps(mshb, j, 1, &v->st, sizeof(v->st));
                AfxCatchError(err);
            }

            AfxEmitTriangles(mshb, 0, 0, mesh->num_tris, mesh->triangles);
        }

        afxArray meshes;
        AfxAllocateArray(&meshes, md5.num_meshes, sizeof(afxMesh), NIL);
        AfxReserveArraySpace(&meshes, md5.num_meshes);

        if (AfxBuildMeshes(sim, strb, md5.num_meshes, meshRes.data, meshes.data))
            AfxThrowError();

        AfxTryAssertObjects(md5.num_meshes, meshes.data, afxFcc_MSH);
        meshes.cnt = md5.num_meshes;

        for (afxNat i = 0; i < md5.num_meshes; i++)
        {
            afxMeshBuilder* mshb = AfxGetArrayUnit(&meshRes, i);
            AfxEndMeshBuilding(mshb);
        }

        afxString tmp;
        afxUri nameUri;
        afxTransform wt;
        AfxResetTransform(&wt);
        AfxClipUriTarget(&uri2.uri, &nameUri);
        afxModel mdl;
        akxModelBlueprint mdlb = { 0 };
        mdlb.meshes = meshes.data;
        mdlb.skl = skl;
        mdlb.rigCnt = md5.num_meshes;
        AfxResetTransform(&mdlb.displacement);
        AfxMakeString32(&mdlb.id, AfxGetUriString(&nameUri));
        mdlb.strb = strb;
        AfxAssembleModel(sim, 1, &mdlb, &mdl);
        AfxTryAssertObjects(1, &mdl, afxFcc_MDL);

        AfxDeallocateArray(&meshRes);
        AfxReleaseObjects(md5.num_meshes, meshes.data);
        AfxDeallocateArray(&meshes);
        AfxReleaseObjects(1, &skl);
        AfxReleaseObjects(1, (void*[]) { mdl });

        fclose(fp);
    }
    return err;
}
