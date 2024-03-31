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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include <stdio.h>
#include <string.h>

#include "qwadro/afxQwadro.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/awxAsset.h"
#include "qwadro/mem/afxStack.h"

enum {
    STATE_LOAD_EMPTY = 0,
    STATE_LOAD_TRIANGLES = 1,
};

struct smd_vertex_weight
{
    int bone_ids[3];
    float bone_weights[3];
};

struct smd_vertex
{
    afxV3d position;
    afxV3d normal;
    afxV2d uvs;
    afxV4d color;
    afxV3d tangent;
    afxV3d binormal;
};

_AFXEXPORT afxError AfxLoadAssetsFromStudiomdl(afxSimulation sim, afxFlags flags, afxNat cnt, afxUri const file[], awxAsset cad[])
{

    int state = STATE_LOAD_EMPTY;
    char state_material[1024];

    vertex_hashtable* hashes = vertex_hashtable_new(1024);
    vertex_list* vert_list = vertex_list_new();
    int_list* tri_list = int_list_new();

    int allocated_weights = 1024;
    vertex_weight* weights = malloc(sizeof(vertex_weight) * allocated_weights);

    int vert_index = 0;

    afxStack vtxStack;
    afxStack triStack;
    AfxAllocatePagedStack(&vtxStack, sizeof(struct smd_vertex), 512, 0);

    afxFile fp;
    AfxOpenFiles(1, &fp, &file, file, afxFileFlag_R);
    char line[1024];

    while (!feof(AfxGetFileDescriptor(fp)))
    {
        fgets(line, sizeof(line), AfxGetFileDescriptor(fp));

        if (strstr(line, "end"))
        {
            state = STATE_LOAD_EMPTY;
        }

        if (state == STATE_LOAD_EMPTY)
        {
            int version;

            if (sscanf(line, "version %i", &version) > 0)
            {
                if (version != 1)
                {
                    AfxLogError("Don't know how to load version %i\n", version);
                }
            }

            if (strstr(line, "triangles"))
            {
                state = STATE_LOAD_TRIANGLES;
            }
        }

        if (state == STATE_LOAD_TRIANGLES)
        {
            int id, l1_id, l2_id, l3_id;
            int num_links = 0;
            float x, y, z, nx, ny, nz, u, v, l1_amount, l2_amount, l3_amount;
            
            if (sscanf(line, "%i %f %f %f %f %f %f %f %f %i %i %f %i %f %i %f", &id, &x, &y, &z, &nx, &ny, &nz, &u, &v, &num_links, &l1_id, &l1_amount, &l2_id, &l2_amount, &l3_id, &l3_amount) > 9)
            {

                if (num_links > 3) {
                    //warning("Loading file '%s'. More than 3 bones rigged to vertex (%i). Ignoring other bones", filename, num_links);
                    num_links = 3;
                }

                if (num_links == 0)
                {
                    AfxLogY("Vertex has no direct bone links");
                    num_links = 1;
                    l1_id = id;
                }

                struct smd_vertex vert;
                /* Swap y and z axis */
                
                AfxSetV3d(vert.position, x, z, y);
                AfxSetV3d(vert.normal, nx, nz, ny);
                AfxSetV2d(vert.uvs, u, v);
                AfxZeroV4d(vert.color);
                AfxZeroV3d(vert.tangent);
                AfxZeroV3d(vert.binormal);

                int vert_pos = vertex_hashtable_get(hashes, vert);

                /* Not already in hashtable */
                if (vert_pos == -1)
                {
                    vertex_hashtable_set(hashes, vert, vert_index);
                    vert_pos = vert_index;
                    vertex_list_push_back(vert_list, vert);
                    struct smd_vertex_weight vw;

                    switch (num_links)
                    {
                    case 3:
                        vw.bone_ids[0] = l1_id;
                        vw.bone_ids[1] = l2_id;
                        vw.bone_ids[2] = l3_id;
                        vw.bone_weights[0] = l1_amount;
                        vw.bone_weights[1] = l2_amount;
                        vw.bone_weights[2] = l3_amount;
                        break;
                    case 2:
                        vw.bone_ids[0] = l1_id;
                        vw.bone_ids[1] = l2_id;
                        vw.bone_ids[2] = 0;
                        vw.bone_weights[0] = l1_amount;
                        vw.bone_weights[1] = l2_amount;
                        vw.bone_weights[2] = 0;
                        break;
                    case 1:
                        vw.bone_ids[0] = l1_id;
                        vw.bone_ids[1] = 0;
                        vw.bone_ids[2] = 0;
                        vw.bone_weights[0] = 1;
                        vw.bone_weights[1] = 0;
                        vw.bone_weights[2] = 0;
                        break;
                    default:
                        AfxLogY(" Unrigged vertex!");
                        vw.bone_ids[0] = 0;
                        vw.bone_ids[1] = 0;
                        vw.bone_ids[2] = 0;
                        vw.bone_weights[0] = 1;
                        vw.bone_weights[1] = 0;
                        vw.bone_weights[2] = 0;
                        break;
                    }

                    float total = vw.bone_weights[0] + vw.bone_weights[1] + vw.bone_weights[2];

                    if (total != 0)
                    {
                        vw.bone_weights[0] /= total;
                        vw.bone_weights[1] /= total;
                        vw.bone_weights[2] /= total;
                    }

                    while (vert_pos >= allocated_weights)
                    {
                        allocated_weights = allocated_weights * 2;
                        weights = realloc(weights, sizeof(vertex_weight) * allocated_weights);
                    }

                    weights[vert_pos] = vw;

                    vert_index++;
                }
                int_list_push_back(tri_list, vert_pos);
            }
            else
            {
                if (vert_index == 0)
                {
                    strcpy(state_material, line);
                }
                else if (strcmp(state_material, line))
                {

                    strcpy(state_material, line);

                    mesh* m = malloc(sizeof(mesh));
                    m->num_verts = vert_list->num_items;
                    m->num_triangles = tri_list->num_items / 3;

                    m->verticies = malloc(sizeof(struct smd_vertex) * m->num_verts);
                    m->triangles = malloc(sizeof(int) * m->num_triangles * 3);

                    for (int i = 0; i < m->num_verts; i++)
                    {
                        m->verticies[i] = vertex_list_get(vert_list, i);
                    }

                    for (int i = 0; i < m->num_triangles * 3; i += 3)
                    {
                        m->triangles[i + 0] = int_list_get(tri_list, i + 2);
                        m->triangles[i + 1] = int_list_get(tri_list, i + 1);
                        m->triangles[i + 2] = int_list_get(tri_list, i + 0);
                    }

                    mesh_generate_tangents(m);
                    renderable_add_mesh_rigged(r, m, weights);
                    mesh_delete(m);

                    vertex_hashtable_delete(hashes);
                    vertex_list_delete(vert_list);
                    int_list_delete(tri_list);

                    vert_index = 0;
                    hashes = vertex_hashtable_new(1024);
                    vert_list = vertex_list_new();
                    tri_list = int_list_new();

                    allocated_weights = 1024;
                    weights = realloc(weights, sizeof(vertex_weight) * 1024);
                }
            }
        }
    }

    AfxReleaseObjects(1, &file);

    mesh* m = malloc(sizeof(mesh));
    m->num_verts = vert_list->num_items;
    m->num_triangles = tri_list->num_items / 3;
    m->verticies = malloc(sizeof(vertex) * m->num_verts);
    m->triangles = malloc(sizeof(int) * m->num_triangles * 3);

    for (int i = 0; i < m->num_verts; i++)
    {
        m->verticies[i] = vertex_list_get(vert_list, i);
    }

    for (int i = 0; i < m->num_triangles * 3; i += 3)
    {
        m->triangles[i + 0] = int_list_get(tri_list, i + 2);
        m->triangles[i + 1] = int_list_get(tri_list, i + 1);
        m->triangles[i + 2] = int_list_get(tri_list, i + 0);
    }

    mesh_generate_tangents(m);
    renderable_add_mesh_rigged(r, m, weights);
    mesh_delete(m);

    vertex_hashtable_delete(hashes);
    vertex_list_delete(vert_list);
    int_list_delete(tri_list);
    free(weights);

    fpath mat_file;
    fpath bmf_file;
    fpath fileid;

    SDL_PathFileLocation(mat_file.ptr, filename);
    SDL_PathFileLocation(bmf_file.ptr, filename);
    SDL_PathFileName(fileid.ptr, filename);

    strcat(mat_file.ptr, fileid.ptr);
    strcat(mat_file.ptr, ".mat");

    if (file_exists(mat_file)) {
        r->material = asset_hndl_new(mat_file);
    }

    strcat(bmf_file.ptr, fileid.ptr);
    strcat(bmf_file.ptr, ".bmf");
    bmf_save_file(r, bmf_file.ptr);

    return r;
}
