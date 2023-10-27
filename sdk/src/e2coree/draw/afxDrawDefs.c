/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/draw/afxDrawDefs.h"
#include "afx/draw/afxVertexBuffer.h"


_AFX afxString const g_str_Qwadro = AFX_STRING_LITERAL("Qwadro");
_AFX afxString const g_str_Draw = AFX_STRING_LITERAL("Draw");
_AFX afxString const g_str_Shader = AFX_STRING_LITERAL("Shader");
_AFX afxString const g_str_Include = AFX_STRING_LITERAL("Include");
_AFX afxString const g_str_Resource = AFX_STRING_LITERAL("Resource");
_AFX afxString const g_str_In = AFX_STRING_LITERAL("In");
_AFX afxString const g_str_Out = AFX_STRING_LITERAL("Out");
_AFX afxString const g_str_stage = AFX_STRING_LITERAL("stage");
_AFX afxString const g_str_name = AFX_STRING_LITERAL("name");
_AFX afxString const g_str_entry = AFX_STRING_LITERAL("entry");
_AFX afxString const g_str_uri = AFX_STRING_LITERAL("uri");
_AFX afxString const g_str_set = AFX_STRING_LITERAL("set");
_AFX afxString const g_str_binding = AFX_STRING_LITERAL("binding");
_AFX afxString const g_str_type = AFX_STRING_LITERAL("type");
_AFX afxString const g_str_count = AFX_STRING_LITERAL("count");
_AFX afxString const g_str_location = AFX_STRING_LITERAL("location");
_AFX afxString const g_str_format = AFX_STRING_LITERAL("format");
_AFX afxString const g_str_topology = AFX_STRING_LITERAL("topology");
_AFX afxString const g_str_restart = AFX_STRING_LITERAL("restart");

_AFX afxString const g_str_DepthClampEnable = AFX_STRING_LITERAL("DepthClampEnable");
_AFX afxString const g_str_RasterizerDiscardEnable = AFX_STRING_LITERAL("RasterizerDiscardEnable");
_AFX afxString const g_str_FillMode = AFX_STRING_LITERAL("FillMode");
_AFX afxString const g_str_CullMode = AFX_STRING_LITERAL("CullMode");
_AFX afxString const g_str_FrontFace = AFX_STRING_LITERAL("FrontFace");
_AFX afxString const g_str_DepthBias = AFX_STRING_LITERAL("DepthBias");
_AFX afxString const g_str_enable = AFX_STRING_LITERAL("enable");
_AFX afxString const g_str_constant = AFX_STRING_LITERAL("constant");
_AFX afxString const g_str_slope = AFX_STRING_LITERAL("slope");
_AFX afxString const g_str_clamp = AFX_STRING_LITERAL("clamp");
_AFX afxString const g_str_LineWidth = AFX_STRING_LITERAL("LineWidth");

_AFX afxString const g_str_DepthTestEnable = AFX_STRING_LITERAL("DepthTestEnable");
_AFX afxString const g_str_DepthWriteEnable = AFX_STRING_LITERAL("DepthWriteEnable");
_AFX afxString const g_str_DepthCompareOp = AFX_STRING_LITERAL("DepthCompareOp");
_AFX afxString const g_str_DepthBoundsTest = AFX_STRING_LITERAL("DepthBoundsTest");
_AFX afxString const g_str_StencilTest = AFX_STRING_LITERAL("StencilTest");
_AFX afxString const g_str_Front = AFX_STRING_LITERAL("Front");
_AFX afxString const g_str_Back = AFX_STRING_LITERAL("Back");
_AFX afxString const g_str_fail = AFX_STRING_LITERAL("fail");
_AFX afxString const g_str_pass = AFX_STRING_LITERAL("pass");
_AFX afxString const g_str_depthFail = AFX_STRING_LITERAL("depthFail");
_AFX afxString const g_str_compare = AFX_STRING_LITERAL("compare");
_AFX afxString const g_str_min = AFX_STRING_LITERAL("min");
_AFX afxString const g_str_max = AFX_STRING_LITERAL("max");

_AFX afxString const g_str_true = AFX_STRING_LITERAL("true");
_AFX afxString const g_str_false = AFX_STRING_LITERAL("false");

_AFX afxString const g_str_DrawOperation = AFX_STRING_LITERAL("DrawOperation");
_AFX afxString const g_str_Operation = AFX_STRING_LITERAL("Operation");
_AFX afxString const g_str_Technique = AFX_STRING_LITERAL("Technique");
_AFX afxString const g_str_Pipeline = AFX_STRING_LITERAL("Pipeline");
_AFX afxString const g_str_Pass = AFX_STRING_LITERAL("Pass");
_AFX afxString const g_str_Rasterization = AFX_STRING_LITERAL("Rasterization");
_AFX afxString const g_str_DepthHandling = AFX_STRING_LITERAL("DepthHandling");
_AFX afxString const g_str_Input = AFX_STRING_LITERAL("Input");
_AFX afxString const g_str_PrimitiveTopology = AFX_STRING_LITERAL("PrimitiveTopology");

afxChar const *shdResTypeNames[] =
{
    "INPUT", // vertex input
    "OUTPUT", // color output?
    "SAMPLER",
    "COMBINED_IMAGE_SAMPLER", // when an image is bound without a sampler
    "SAMPLED_IMAGE",
    "STORAGE_IMAGE",
    "UNIFORM_TEXEL_BUFFER",
    "STORAGE_TEXEL_BUFFER",
    "CONSTANT_BUFFER",
    "STORAGE_BUFFER",
    "INPUT_ATTACHMENT",
    "PUSH_CONSTANT_BUFFER",
};

afxChar const *fmtNames[] =
{
    "REAL",
    "NAT",
    "INT",
    "V2D",
    "V2DN",
    "V2DI",
    "V3D",
    "V3DN",
    "V3DI",
    "V4D",
    "V4DN",
    "V4DI"
};

_AFX afxString const vtxFmtString[afxVertexFormat_TOTAL] =
{
    AFX_STRING_LITERAL("REAL"),
    AFX_STRING_LITERAL("V2D"),
    AFX_STRING_LITERAL("V3D"),
    AFX_STRING_LITERAL("V4D"),

    AFX_STRING_LITERAL("V4B"),
    AFX_STRING_LITERAL("v8B"),
};

_AFX afxString const shaderStageString[] =
{
    AFX_STRING_LITERAL(""),
    AFX_STRING_LITERAL("VERTEX"),
    AFX_STRING_LITERAL("FRAGMENT"),
    AFX_STRING_LITERAL("GEOMETRY"),
    AFX_STRING_LITERAL("TESS_CTRL"),
    AFX_STRING_LITERAL("TESS_EVAL"),
    AFX_STRING_LITERAL("COMPUTE"),
};

_AFX afxString const fillModeString[] =
{
    AFX_STRING_LITERAL("SOLID"),
    AFX_STRING_LITERAL("EDGE"),
    AFX_STRING_LITERAL("POINT")
};

_AFX afxString const frontFaceString[] =
{
    AFX_STRING_LITERAL("CCW"),
    AFX_STRING_LITERAL("CW")
};

_AFX afxString const cullModeString[] =
{
    AFX_STRING_LITERAL("NONE"),
    AFX_STRING_LITERAL("FRONT"),
    AFX_STRING_LITERAL("BACK"),
    AFX_STRING_LITERAL("BOTH")
};

_AFX afxString const compareOpString[] =
{
    AFX_STRING_LITERAL("NEVER"),
    AFX_STRING_LITERAL("LESS"),
    AFX_STRING_LITERAL("LEQUAL"),
    AFX_STRING_LITERAL("EQUAL"),
    AFX_STRING_LITERAL("NOTEQUAL"),
    AFX_STRING_LITERAL("GEQUAL"),
    AFX_STRING_LITERAL("GREATER"),
    AFX_STRING_LITERAL("ALWAYS")
};

_AFX afxString const stencilOpString[] =
{
    AFX_STRING_LITERAL("KEEP"),
    AFX_STRING_LITERAL("ZERO"),
    AFX_STRING_LITERAL("REPLACE"),
    AFX_STRING_LITERAL("INC_AND_CLAMP"),
    AFX_STRING_LITERAL("DEC_AND_CLAMP"),
    AFX_STRING_LITERAL("INVERT"),
    AFX_STRING_LITERAL("INC_AND_WRAP"),
    AFX_STRING_LITERAL("DEC_AND_WRAP")
};

_AFX afxString const logicOpString[] =
{
    AFX_STRING_LITERAL("CLEAR"),
    AFX_STRING_LITERAL("AND"),
    AFX_STRING_LITERAL("AND_REVERSE"),
    AFX_STRING_LITERAL("COPY"),
    AFX_STRING_LITERAL("AND_INVERTED"),
    AFX_STRING_LITERAL("NO_OP"),
    AFX_STRING_LITERAL("XOR"),
    AFX_STRING_LITERAL("OR"),
    AFX_STRING_LITERAL("NOR"),
    AFX_STRING_LITERAL("EQUIVALENT"),
    AFX_STRING_LITERAL("INVERT"),
    AFX_STRING_LITERAL("OR_REVERSE"),
    AFX_STRING_LITERAL("COPY_INVERTED"),
    AFX_STRING_LITERAL("OR_INVERTED"),
    AFX_STRING_LITERAL("NAND"),
    AFX_STRING_LITERAL("SET")
};

_AFX afxString const primTopologyString[] =
{
    AFX_STRING_LITERAL("POINT_LIST"),
    AFX_STRING_LITERAL("LINE_LIST"),
    AFX_STRING_LITERAL("LINE_STRIP"),
    AFX_STRING_LITERAL("TRI_LIST"),
    AFX_STRING_LITERAL("TRI_STRIP"),
    AFX_STRING_LITERAL("TRI_FAN"),
    AFX_STRING_LITERAL("LINE_LIST_ADJACENT"),
    AFX_STRING_LITERAL("LINE_STRIP_ADJACENT"),
    AFX_STRING_LITERAL("TRI_LIST_ADJACENT"),
    AFX_STRING_LITERAL("TRI_STRIP_ADJACENT"),
    AFX_STRING_LITERAL("PATCH_LIST")
};

afxNat const vtxFmtSizes[] =
{
    sizeof(afxReal),
    sizeof(afxV2d),
    sizeof(afxV3d),
    sizeof(afxV4d),
    sizeof(afxInt8[4]),
    sizeof(afxInt8[8]),
};

afxChar const * const vtxFmtNames[] =
{
    "X32R",
    "XY32R",
    "XYZ32R",
    "XYZW32R",
    "XYZW32Q"
};

_AFX afxNat AfxVertexFormatGetSize(afxVertexFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange(afxVertexFormat_TOTAL, fmt, 1);
    return vtxFmtSizes[fmt];
}

_AFX afxResult AfxResolveFormat(afxFormat fmt, afxFormat *subfmt, afxSize *subSiz, afxSize *vecSiz, afxSize *siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertRange(afxVertexFormat_TOTAL, fmt, 1);

    static struct { afxFormat subfmt; afxSize vecSiz; afxSize siz; afxSize subsiz; } list[] =
    {
        { afxFormat_REAL, 1, sizeof(afxReal), sizeof(afxReal) }, // afxFormat_REAL,
        { afxFormat_NAT, 1, sizeof(afxNat), sizeof(afxNat) }, // afxFormat_NAT,
        { afxFormat_INT, 1, sizeof(afxInt), sizeof(afxInt) }, // afxFormat_INT,
        { afxFormat_REAL, 2, sizeof(afxV2d), sizeof(afxReal) }, // afxFormat_V2D,
        { afxFormat_NAT, 2, sizeof(afxV2d), sizeof(afxNat) }, // afxFormat_V2DN,
        { afxFormat_INT, 2, sizeof(afxV2d), sizeof(afxInt) }, // afxFormat_V2DI,
        { afxFormat_REAL, 3, sizeof(afxV3d), sizeof(afxReal) }, // afxFormat_V3D,
        { afxFormat_NAT, 3, sizeof(afxV3d), sizeof(afxNat) }, // afxFormat_V3DN,
        { afxFormat_INT, 3, sizeof(afxV3d), sizeof(afxInt) }, // afxFormat_V3DI,
        { afxFormat_REAL, 4, sizeof(afxV4d), sizeof(afxReal) }, // afxFormat_V4D,
        { afxFormat_NAT, 4, sizeof(afxV4d), sizeof(afxNat) }, // afxFormat_V4DN,
        { afxFormat_INT, 4, sizeof(afxV4d), sizeof(afxInt) }, // afxFormat_V4DI,
    };

    if (subfmt)
        *subfmt = list[fmt].subfmt;

    if (vecSiz)
        *vecSiz = list[fmt].vecSiz;

    if (siz)
        *siz = list[fmt].siz;

    if (subSiz)
        *subSiz = list[fmt].subsiz;

    return 0;
}

_AFX afxPrimTopology AfxFindPrimitiveTopology(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxPrimTopology_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &primTopologyString[i]))
            return (afxPrimTopology)i;
    }

    return (afxPrimTopology)-1;
}

_AFX afxLogicOp AfxFindLogicOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxLogicOp_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &logicOpString[i]))
            return (afxLogicOp)i;
    }

    return (afxLogicOp)-1;
}

_AFX afxStencilOp AfxFindStencilOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxStencilOp_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &stencilOpString[i]))
            return (afxStencilOp)i;
    }

    return (afxStencilOp)-1;
}

_AFX afxCompareOp AfxFindCompareOp(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxCompareOp_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &compareOpString[i]))
            return (afxCompareOp)i;
    }

    return (afxCompareOp)-1;
}

_AFX afxCullMode AfxFindCullMode(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxCullMode_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &cullModeString[i]))
            return (afxCullMode)i;
    }

    return (afxCullMode)-1;
}

_AFX afxFrontFace AfxFindFrontFace(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxFrontFace_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &frontFaceString[i]))
            return (afxFrontFace)i;
    }

    return (afxFrontFace)-1;
}

_AFX afxFillMode AfxFindFillMode(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxFillMode_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &fillModeString[i]))
            return (afxFillMode)i;
    }

    return (afxFillMode)-1;
}

_AFX afxShaderStage AfxFindShaderStage(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxShaderStage_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &shaderStageString[i]))
            return (afxShaderStage)i;
    }

    return (afxShaderStage)0;
}

_AFX afxVertexFormat AfxFindVertexFormat(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertString(str);

    for (afxNat i = 0; i < afxVertexFormat_TOTAL; i++)
    {
        if (0 == AfxCompareStringCi(str, &vtxFmtString[i]))
            return (afxVertexFormat)i;
    }

    return (afxVertexFormat)-1;
}

_AFX afxString const* AfxStringifyShaderStage(afxShaderStage ss, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ss < afxShaderStage_TOTAL);
    AfxCopyString(out, &shaderStageString[ss]);
    return out;
}

_AFX afxString const* AfxStringifyFillMode(afxFillMode fm, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fm < afxFillMode_TOTAL);
    AfxCopyString(out, &fillModeString[fm]);
    return out;
}

_AFX afxString const* AfxStringifyFrontFace(afxFrontFace ff, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ff < afxFrontFace_TOTAL);
    AfxCopyString(out, &frontFaceString[ff]);
    return out;
}

_AFX afxString const* AfxStringifyCullMode(afxCullMode cm, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cm < afxCullMode_TOTAL);
    AfxCopyString(out, &cullModeString[cm]);
    return out;
}

_AFX afxString const* AfxStringifyCompareOp(afxCompareOp co, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(co < afxCompareOp_TOTAL);
    AfxCopyString(out, &compareOpString[co]);
    return out;
}

_AFX afxString const* AfxStringifyStencilOp(afxStencilOp so, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(so < afxStencilOp_TOTAL);
    AfxCopyString(out, &stencilOpString[so]);
    return out;
}

_AFX afxString const* AfxStringifyLogicOp(afxLogicOp lo, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lo < afxLogicOp_TOTAL);
    AfxCopyString(out, &logicOpString[lo]);
    return out;
}

_AFX afxString const* AfxStringifyPrimitiveTopology(afxPrimTopology pt, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pt < afxPrimTopology_TOTAL);
    AfxCopyString(out, &primTopologyString[pt]);
    return out;
}

_AFX afxString const* AfxStringifyVertexFormat(afxVertexFormat fmt, afxString *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fmt < afxVertexFormat_TOTAL);
    AfxCopyString(out, &vtxFmtString[fmt]);
    return out;
}

_AFX afxError _AfxAssertWhd(afxWhd total_, afxWhd base_, afxWhd range_, afxHint const hint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(total_);
    AfxAssert(base_);
    AfxAssert(range_);

    for (afxNat i = 0; i < 3; i++)
    {
        if ((err |= (!(total_[i] > base_[i] + range_[i]))))
        {
            AfxLogError(hint, "[%i] %s(%f) + %s(%f) is out of range [0, %f]", AfxStr((base_)), (base_), AfxStr((range_)), (range_), AfxStr((total_)), (total_));
        }
    }
    return err;
}
