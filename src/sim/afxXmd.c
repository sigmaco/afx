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

_ASX afxError AfxAssembleModelFromXml(afxSimulation sim, afxXml const* xml, afxUnit elemIdx, arxSkeleton skl, arxModel* model)
{
    afxError err = AFX_ERR_NONE;
    
    arxModelBlueprint config = { 0 };
    
    afxString const mdlAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("position"),
        AFX_STRING("orientation"),
        AFX_STRING("scale"),
        AFX_STRING("lodType")
    };

    afxString name, content, acontent;
    afxUnit tagCnt = AfxCountXmlTags(xml, elemIdx);

    for (afxUnit i = 0; i < tagCnt; i++)
    {
        AfxQueryXmlTag(xml, elemIdx, i, &name, &acontent);

        afxUnit sIdx;
        if (!AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(mdlAttrNames), mdlAttrNames, &sIdx))
            continue;

        switch (sIdx)
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
            AfxReportY("Tag '%.*s' not handled.", AfxPushString(&name));
            break;
        }
        }
    }

    afxString const mdlChildNames[] =
    {
        AFX_STRING("Joint"),
        AFX_STRING("Rig"),
        AFX_STRING("Attachment")
    };

    afxString const jntAttrNames[] =
    {
        AFX_STRING("id"),
        AFX_STRING("parent"),
        AFX_STRING("position"),
        AFX_STRING("orientation"),
        AFX_STRING("scale"),
        AFX_STRING("lodError"),
        AFX_STRING("iw")
    };

    afxString const rigAttrNames[] =
    {
        AFX_STRING("uri")
    };

    afxUnit rigCnt = 0;
    afxUnit jointCnt = 0;
    afxString joints[256];
    afxTransform locals[256];
    afxUnit parents[256];
    afxReal lodErrors[256];
    afxM4r iws[256];
    afxUri meshes[256];

    afxUnit childCnt = AfxCountXmlChilds(xml, elemIdx);

    for (afxUnit i = 0; i < childCnt; i++)
    {
        afxUnit childElemIdx = AfxGetXmlChild(xml, elemIdx, i);
        AfxQueryXmlElement(xml, childElemIdx, &name, &content);
        afxUnit childTagCnt = AfxCountXmlTags(xml, childElemIdx);

        afxUnit sIdx;
        if (!AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(mdlChildNames), mdlChildNames, &sIdx))
            continue;

        switch (sIdx)
        {
        case 0: // Joint
        {
            afxUnit tagCnt = AfxCountXmlTags(xml, elemIdx);
            afxV3d position = { 0 };
            afxQuat orientation = { 0, 0, 0, 1 };
            afxV3d scale = { 1, 1, 1 };
            afxReal lodError = -1.0;
            afxUnit parentIdx = AFX_INVALID_INDEX;
            afxM4r iw = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } };
            afxString id = { 0 };

            for (afxUnit j = 0; j < tagCnt; j++)
            {
                AfxQueryXmlTag(xml, elemIdx, j, &name, &acontent);

                afxUnit sIdx;
                if (!AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(jntAttrNames), jntAttrNames, &sIdx))
                    continue;

                switch (sIdx)
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
            AfxM4rCopy(iws[jointCnt], iw);
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

                afxUnit sIdx;
                if (!AfxCompareStrings(&name, 0, TRUE, ARRAY_SIZE(rigAttrNames), rigAttrNames, &sIdx))
                    continue;

                switch (sIdx)
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
        default: AfxReportY("Node '%.*s' not handled.", AfxPushString(&name));
            break;
        }
    }

    arxModel mdl;
    ArxAssembleModels(sim, 1, &config, &mdl);

    

    return err;
}
