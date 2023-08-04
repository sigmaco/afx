/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>

#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#include "sglDrawDriver.h"
#include "sgl.h"
#include "afx/core/afxSystem.h"
#pragma comment(lib, "opengl32")

static const char *glVmtNames[] =
{
    // 100
    "glCullFace", // 0
    "glFrontFace",
    "glHint",
    "glLineWidth",
    "glPointSize",
    "glPolygonMode",
    "glScissor",
    "glTexParameterf",
    "glTexParameterfv",
    "glTexParameteri",
    "glTexParameteriv",
    "glTexImage1D",
    "glTexImage2D",
    "glDrawBuffer",
    "glClear",
    "glClearColor",
    "glClearStencil",
    "glClearDepth",
    "glStencilMask",
    "glColorMask",
    "glDepthMask",
    "glDisable",
    "glEnable",
    "glFinish",
    "glFlush",
    "glBlendFunc",
    "glLogicOp",
    "glStencilFunc",
    "glStencilOp",
    "glDepthFunc",
    "glPixelStoref",
    "glPixelStorei",
    "glReadBuffer",
    "glReadPixels",
    "glGetBooleanv",
    "glGetDoublev",
    "glGetError",
    "glGetFloatv",
    "glGetIntegerv",
    "glGetString",
    "glGetTexImage",
    "glGetTexParameterfv",
    "glGetTexParameteriv",
    "glGetTexLevelParameterfv",
    "glGetTexLevelParameteriv",
    "glIsEnabled",
    "glDepthRange",
    "glViewport", // 47
    // 110
    "glDrawArrays", // 0
    "glDrawElements",
    "glGetPointerv",
    "glPolygonOffset",
    "glCopyTexImage1D",
    "glCopyTexImage2D",
    "glCopyTexSubImage1D",
    "glCopyTexSubImage2D",
    "glTexSubImage1D",
    "glTexSubImage2D",
    "glBindTexture",
    "glDeleteTextures",
    "glGenTextures",
    "glIsTexture", // 13
    // 120
    "glDrawRangeElements", // 0
    "glTexImage3D",
    "glTexSubImage3D",
    "glCopyTexSubImage3D", // 3
    // 130
    "glActiveTexture", // 0
    "glSampleCoverage",
    "glCompressedTexImage3D",
    "glCompressedTexImage2D",
    "glCompressedTexImage1D",
    "glCompressedTexSubImage3D",
    "glCompressedTexSubImage2D",
    "glCompressedTexSubImage1D",
    "glGetCompressedTexImage", // 8
    // 140
    "glBlendFuncSeparate", // 0
    "glMultiDrawArrays",
    "glMultiDrawElements",
    "glPointParameterf",
    "glPointParameterfv",
    "glPointParameteri",
    "glPointParameteriv",
    "glBlendColor",
    "glBlendEquation", // 8
    // 150
    "glGenQueries", // 0
    "glDeleteQueries",
    "glIsQuery",
    "glBeginQuery",
    "glEndQuery",
    "glGetQueryiv",
    "glGetQueryObjectiv",
    "glGetQueryObjectuiv",
    "glBindBuffer",
    "glDeleteBuffers",
    "glGenBuffers",
    "glIsBuffer",
    "glBufferData",
    "glBufferSubData",
    "glGetBufferSubData",
    "glMapBuffer",
    "glUnmapBuffer",
    "glGetBufferParameteriv",
    "glGetBufferPointerv", // 18
    // 200
    "glBlendEquationSeparate", // 0
    "glDrawBuffers",
    "glStencilOpSeparate",
    "glStencilFuncSeparate",
    "glStencilMaskSeparate",
    "glAttachShader",
    "glBindAttribLocation",
    "glCompileShader",
    "glCreateProgram",
    "glCreateShader",
    "glDeleteProgram",
    "glDeleteShader",
    "glDetachShader",
    "glDisableVertexAttribArray",
    "glEnableVertexAttribArray",
    "glGetActiveAttrib",
    "glGetActiveUniform",
    "glGetAttachedShaders",
    "glGetAttribLocation",
    "glGetProgramiv",
    "glGetProgramInfoLog",
    "glGetShaderiv",
    "glGetShaderInfoLog",
    "glGetShaderSource",
    "glGetUniformLocation",
    "glGetUniformfv",
    "glGetUniformiv",
    "glGetVertexAttribdv",
    "glGetVertexAttribfv",
    "glGetVertexAttribiv",
    "glGetVertexAttribPointerv",
    "glIsProgram",
    "glIsShader",
    "glLinkProgram",
    "glShaderSource",
    "glUseProgram",
    "glUniform1f",
    "glUniform2f",
    "glUniform3f",
    "glUniform4f",
    "glUniform1i",
    "glUniform2i",
    "glUniform3i",
    "glUniform4i",
    "glUniform1fv",
    "glUniform2fv",
    "glUniform3fv",
    "glUniform4fv",
    "glUniform1iv",
    "glUniform2iv",
    "glUniform3iv",
    "glUniform4iv",
    "glUniformMatrix2fv",
    "glUniformMatrix3fv",
    "glUniformMatrix4fv",
    "glValidateProgram",
    "glVertexAttrib1d",
    "glVertexAttrib1dv",
    "glVertexAttrib1f",
    "glVertexAttrib1fv",
    "glVertexAttrib1s",
    "glVertexAttrib1sv",
    "glVertexAttrib2d",
    "glVertexAttrib2dv",
    "glVertexAttrib2f",
    "glVertexAttrib2fv",
    "glVertexAttrib2s",
    "glVertexAttrib2sv",
    "glVertexAttrib3d",
    "glVertexAttrib3dv",
    "glVertexAttrib3f",
    "glVertexAttrib3fv",
    "glVertexAttrib3s",
    "glVertexAttrib3sv",
    "glVertexAttrib4Nbv",
    "glVertexAttrib4Niv",
    "glVertexAttrib4Nsv",
    "glVertexAttrib4Nub",
    "glVertexAttrib4Nubv",
    "glVertexAttrib4Nuiv",
    "glVertexAttrib4Nusv",
    "glVertexAttrib4bv",
    "glVertexAttrib4d",
    "glVertexAttrib4dv",
    "glVertexAttrib4f",
    "glVertexAttrib4fv",
    "glVertexAttrib4iv",
    "glVertexAttrib4s",
    "glVertexAttrib4sv",
    "glVertexAttrib4ubv",
    "glVertexAttrib4uiv",
    "glVertexAttrib4usv",
    "glVertexAttribPointer", // 92
    // 210
    "glUniformMatrix2x3fv", // 0
    "glUniformMatrix3x2fv",
    "glUniformMatrix2x4fv",
    "glUniformMatrix4x2fv",
    "glUniformMatrix3x4fv",
    "glUniformMatrix4x3fv", // 5
    // 300
    "glColorMaski", // 0
    "glGetBooleani_v",
    "glGetIntegeri_v",
    "glEnablei",
    "glDisablei",
    "glIsEnabledi",
    "glBeginTransformFeedback",
    "glEndTransformFeedback",
    "glBindBufferRange",
    "glBindBufferBase",
    "glTransformFeedbackVaryings",
    "glGetTransformFeedbackVarying",
    "glClampColor",
    "glBeginConditionalRender",
    "glEndConditionalRender",
    "glVertexAttribIPointer",
    "glGetVertexAttribIiv",
    "glGetVertexAttribIuiv",
    "glVertexAttribI1i",
    "glVertexAttribI2i",
    "glVertexAttribI3i",
    "glVertexAttribI4i",
    "glVertexAttribI1ui",
    "glVertexAttribI2ui",
    "glVertexAttribI3ui",
    "glVertexAttribI4ui",
    "glVertexAttribI1iv",
    "glVertexAttribI2iv",
    "glVertexAttribI3iv",
    "glVertexAttribI4iv",
    "glVertexAttribI1uiv",
    "glVertexAttribI2uiv",
    "glVertexAttribI3uiv",
    "glVertexAttribI4uiv",
    "glVertexAttribI4bv",
    "glVertexAttribI4sv",
    "glVertexAttribI4ubv",
    "glVertexAttribI4usv",
    "glGetUniformuiv",
    "glBindFragDataLocation",
    "glGetFragDataLocation",
    "glUniform1ui",
    "glUniform2ui",
    "glUniform3ui",
    "glUniform4ui",
    "glUniform1uiv",
    "glUniform2uiv",
    "glUniform3uiv",
    "glUniform4uiv",
    "glTexParameterIiv",
    "glTexParameterIuiv",
    "glGetTexParameterIiv",
    "glGetTexParameterIuiv",
    "glClearBufferiv",
    "glClearBufferuiv",
    "glClearBufferfv",
    "glClearBufferfi",
    "glGetStringi",
    "glIsRenderbuffer",
    "glBindRenderbuffer",
    "glDeleteRenderbuffers",
    "glGenRenderbuffers",
    "glRenderbufferStorage",
    "glGetRenderbufferParameteriv",
    "glIsFramebuffer",
    "glBindFramebuffer",
    "glDeleteFramebuffers",
    "glGenFramebuffers",
    "glCheckFramebufferStatus",
    "glFramebufferTexture1D",
    "glFramebufferTexture2D",
    "glFramebufferTexture3D",
    "glFramebufferRenderbuffer",
    "glGetFramebufferAttachmentParameteriv",
    "glGenerateMipmap",
    "glBlitFramebuffer",
    "glRenderbufferStorageMultisample",
    "glFramebufferTextureLayer",
    "glMapBufferRange",
    "glFlushMappedBufferRange",
    "glBindVertexArray",
    "glDeleteVertexArrays",
    "glGenVertexArrays",
    "glIsVertexArray", // 83
    // 310
    "glDrawArraysInstanced", // 0
    "glDrawElementsInstanced",
    "glTexBuffer",
    "glPrimitiveRestartIndex",
    "glCopyBufferSubData",
    "glGetUniformIndices",
    "glGetActiveUniformsiv",
    "glGetActiveUniformName",
    "glGetUniformBlockIndex",
    "glGetActiveUniformBlockiv",
    "glGetActiveUniformBlockName",
    "glUniformBlockBinding", // 11
    // 320
    "glDrawElementsBaseVertex", // 0
    "glDrawRangeElementsBaseVertex",
    "glDrawElementsInstancedBaseVertex",
    "glMultiDrawElementsBaseVertex",
    "glProvokingVertex",
    "glFenceSync",
    "glIsSync",
    "glDeleteSync",
    "glClientWaitSync",
    "glWaitSync",
    "glGetInteger64v",
    "glGetSynciv",
    "glGetInteger64i_v",
    "glGetBufferParameteri64v",
    "glFramebufferTexture",
    "glTexImage2DMultisample",
    "glTexImage3DMultisample",
    "glGetMultisamplefv",
    "glSampleMaski", // 18
    // 330
    "glBindFragDataLocationIndexed", // 0
    "glGetFragDataIndex",
    "glGenSamplers",
    "glDeleteSamplers",
    "glIsSampler",
    "glBindSampler",
    "glSamplerParameteri",
    "glSamplerParameteriv",
    "glSamplerParameterf",
    "glSamplerParameterfv",
    "glSamplerParameterIiv",
    "glSamplerParameterIuiv",
    "glGetSamplerParameteriv",
    "glGetSamplerParameterIiv",
    "glGetSamplerParameterfv",
    "glGetSamplerParameterIuiv",
    "glQueryCounter",
    "glGetQueryObjecti64v",
    "glGetQueryObjectui64v",
    "glVertexAttribDivisor",
    "glVertexAttribP1ui",
    "glVertexAttribP1uiv",
    "glVertexAttribP2ui",
    "glVertexAttribP2uiv",
    "glVertexAttribP3ui",
    "glVertexAttribP3uiv",
    "glVertexAttribP4ui",
    "glVertexAttribP4uiv", // 27
    // 400
    "glMinSampleShading", // 0
    "glBlendEquationi",
    "glBlendEquationSeparatei",
    "glBlendFunci",
    "glBlendFuncSeparatei",
    "glDrawArraysIndirect",
    "glDrawElementsIndirect",
    "glUniform1d",
    "glUniform2d",
    "glUniform3d",
    "glUniform4d",
    "glUniform1dv",
    "glUniform2dv",
    "glUniform3dv",
    "glUniform4dv",
    "glUniformMatrix2dv",
    "glUniformMatrix3dv",
    "glUniformMatrix4dv",
    "glUniformMatrix2x3dv",
    "glUniformMatrix2x4dv",
    "glUniformMatrix3x2dv",
    "glUniformMatrix3x4dv",
    "glUniformMatrix4x2dv",
    "glUniformMatrix4x3dv",
    "glGetUniformdv",
    "glGetSubroutineUniformLocation",
    "glGetSubroutineIndex",
    "glGetActiveSubroutineUniformiv",
    "glGetActiveSubroutineUniformName",
    "glGetActiveSubroutineName",
    "glUniformSubroutinesuiv",
    "glGetUniformSubroutineuiv",
    "glGetProgramStageiv",
    "glPatchParameteri",
    "glPatchParameterfv",
    "glBindTransformFeedback",
    "glDeleteTransformFeedbacks",
    "glGenTransformFeedbacks",
    "glIsTransformFeedback",
    "glPauseTransformFeedback",
    "glResumeTransformFeedback",
    "glDrawTransformFeedback",
    "glDrawTransformFeedbackStream",
    "glBeginQueryIndexed",
    "glEndQueryIndexed",
    "glGetQueryIndexediv", // 45
    // 410
    "glReleaseShaderCompiler", // 0
    "glShaderBinary",
    "glGetShaderPrecisionFormat",
    "glDepthRangef",
    "glClearDepthf",
    "glGetProgramBinary",
    "glProgramBinary",
    "glProgramParameteri",
    "glUseProgramStages",
    "glActiveShaderProgram",
    "glCreateShaderProgramv",
    "glBindProgramPipeline",
    "glDeleteProgramPipelines",
    "glGenProgramPipelines",
    "glIsProgramPipeline",
    "glGetProgramPipelineiv",
    "glProgramUniform1i",
    "glProgramUniform1iv",
    "glProgramUniform1f",
    "glProgramUniform1fv",
    "glProgramUniform1d",
    "glProgramUniform1dv",
    "glProgramUniform1ui",
    "glProgramUniform1uiv",
    "glProgramUniform2i",
    "glProgramUniform2iv",
    "glProgramUniform2f",
    "glProgramUniform2fv",
    "glProgramUniform2d",
    "glProgramUniform2dv",
    "glProgramUniform2ui",
    "glProgramUniform2uiv",
    "glProgramUniform3i",
    "glProgramUniform3iv",
    "glProgramUniform3f",
    "glProgramUniform3fv",
    "glProgramUniform3d",
    "glProgramUniform3dv",
    "glProgramUniform3ui",
    "glProgramUniform3uiv",
    "glProgramUniform4i",
    "glProgramUniform4iv",
    "glProgramUniform4f",
    "glProgramUniform4fv",
    "glProgramUniform4d",
    "glProgramUniform4dv",
    "glProgramUniform4ui",
    "glProgramUniform4uiv",
    "glProgramUniformMatrix2fv",
    "glProgramUniformMatrix3fv",
    "glProgramUniformMatrix4fv",
    "glProgramUniformMatrix2dv",
    "glProgramUniformMatrix3dv",
    "glProgramUniformMatrix4dv",
    "glProgramUniformMatrix2x3fv",
    "glProgramUniformMatrix3x2fv",
    "glProgramUniformMatrix2x4fv",
    "glProgramUniformMatrix4x2fv",
    "glProgramUniformMatrix3x4fv",
    "glProgramUniformMatrix4x3fv",
    "glProgramUniformMatrix2x3dv",
    "glProgramUniformMatrix3x2dv",
    "glProgramUniformMatrix2x4dv",
    "glProgramUniformMatrix4x2dv",
    "glProgramUniformMatrix3x4dv",
    "glProgramUniformMatrix4x3dv",
    "glValidateProgramPipeline",
    "glGetProgramPipelineInfoLog",
    "glVertexAttribL1d",
    "glVertexAttribL2d",
    "glVertexAttribL3d",
    "glVertexAttribL4d",
    "glVertexAttribL1dv",
    "glVertexAttribL2dv",
    "glVertexAttribL3dv",
    "glVertexAttribL4dv",
    "glVertexAttribLPointer",
    "glGetVertexAttribLdv",
    "glViewportArrayv",
    "glViewportIndexedf",
    "glViewportIndexedfv",
    "glScissorArrayv",
    "glScissorIndexed",
    "glScissorIndexedv",
    "glDepthRangeArrayv",
    "glDepthRangeIndexed",
    "glGetFloati_v",
    "glGetDoublei_v", // 87
    // 420
    "glDrawArraysInstancedBaseInstance", // 0
    "glDrawElementsInstancedBaseInstance",
    "glDrawElementsInstancedBaseVertexBaseInstance",
    "glGetInternalformativ",
    "glGetActiveAtomicCounterBufferiv",
    "glBindImageTexture",
    "glMemoryBarrier",
    "glTexStorage1D",
    "glTexStorage2D",
    "glTexStorage3D",
    "glDrawTransformFeedbackInstanced",
    "glDrawTransformFeedbackStreamInstanced", // 11
    // 430
    "glClearBufferData", // 0
    "glClearBufferSubData",
    "glDispatchCompute",
    "glDispatchComputeIndirect",
    "glCopyImageSubData",
    "glFramebufferParameteri",
    "glGetFramebufferParameteriv",
    "glGetInternalformati64v",
    "glInvalidateTexSubImage",
    "glInvalidateTexImage",
    "glInvalidateBufferSubData",
    "glInvalidateBufferData",
    "glInvalidateFramebuffer",
    "glInvalidateSubFramebuffer",
    "glMultiDrawArraysIndirect",
    "glMultiDrawElementsIndirect",
    "glGetProgramInterfaceiv",
    "glGetProgramResourceIndex",
    "glGetProgramResourceName",
    "glGetProgramResourceiv",
    "glGetProgramResourceLocation",
    "glGetProgramResourceLocationIndex",
    "glShaderStorageBlockBinding",
    "glTexBufferRange",
    "glTexStorage2DMultisample",
    "glTexStorage3DMultisample",
    "glTextureView",
    "glBindVertexBuffer",
    "glVertexAttribFormat",
    "glVertexAttribIFormat",
    "glVertexAttribLFormat",
    "glVertexAttribBinding",
    "glVertexBindingDivisor",
    "glDebugMessageControl",
    "glDebugMessageInsert",
    "glDebugMessageCallback",
    "glGetDebugMessageLog",
    "glPushDebugGroup",
    "glPopDebugGroup",
    "glObjectLabel",
    "glGetObjectLabel",
    "glObjectPtrLabel",
    "glGetObjectPtrLabel", // 42
    // 440
    "glBufferStorage", // 0
    "glClearTexImage",
    "glClearTexSubImage",
    "glBindBuffersBase",
    "glBindBuffersRange",
    "glBindTextures",
    "glBindSamplers",
    "glBindImageTextures",
    "glBindVertexBuffers", // 8
    // 450
    "glClipControl", // 0
    "glCreateTransformFeedbacks",
    "glTransformFeedbackBufferBase",
    "glTransformFeedbackBufferRange",
    "glGetTransformFeedbackiv",
    "glGetTransformFeedbacki_v",
    "glGetTransformFeedbacki64_v",
    "glCreateBuffers",
    "glNamedBufferStorage",
    "glNamedBufferData",
    "glNamedBufferSubData",
    "glCopyNamedBufferSubData",
    "glClearNamedBufferData",
    "glClearNamedBufferSubData",
    "glMapNamedBuffer",
    "glMapNamedBufferRange",
    "glUnmapNamedBuffer",
    "glFlushMappedNamedBufferRange",
    "glGetNamedBufferParameteriv",
    "glGetNamedBufferParameteri64v",
    "glGetNamedBufferPointerv",
    "glGetNamedBufferSubData",
    "glCreateFramebuffers",
    "glNamedFramebufferRenderbuffer",
    "glNamedFramebufferParameteri",
    "glNamedFramebufferTexture",
    "glNamedFramebufferTextureLayer",
    "glNamedFramebufferCanvas",
    "glNamedFramebufferCanvass",
    "glNamedFramebufferReadBuffer",
    "glInvalidateNamedFramebufferData",
    "glInvalidateNamedFramebufferSubData",
    "glClearNamedFramebufferiv",
    "glClearNamedFramebufferuiv",
    "glClearNamedFramebufferfv",
    "glClearNamedFramebufferfi",
    "glBlitNamedFramebuffer",
    "glCheckNamedFramebufferStatus",
    "glGetNamedFramebufferParameteriv",
    "glGetNamedFramebufferAttachmentParameteriv",
    "glCreateRenderbuffers",
    "glNamedRenderbufferStorage",
    "glNamedRenderbufferStorageMultisample",
    "glGetNamedRenderbufferParameteriv",
    "glCreateTextures",
    "glTextureBuffer",
    "glTextureBufferRange",
    "glTextureStorage1D",
    "glTextureStorage2D",
    "glTextureStorage3D",
    "glTextureStorage2DMultisample",
    "glTextureStorage3DMultisample",
    "glTextureSubImage1D",
    "glTextureSubImage2D",
    "glTextureSubImage3D",
    "glCompressedTextureSubImage1D",
    "glCompressedTextureSubImage2D",
    "glCompressedTextureSubImage3D",
    "glCopyTextureSubImage1D",
    "glCopyTextureSubImage2D",
    "glCopyTextureSubImage3D",
    "glTextureParameterf",
    "glTextureParameterfv",
    "glTextureParameteri",
    "glTextureParameterIiv",
    "glTextureParameterIuiv",
    "glTextureParameteriv",
    "glGenerateTextureMipmap",
    "glBindTextureUnit",
    "glGetTextureImage",
    "glGetCompressedTextureImage",
    "glGetTextureLevelParameterfv",
    "glGetTextureLevelParameteriv",
    "glGetTextureParameterfv",
    "glGetTextureParameterIiv",
    "glGetTextureParameterIuiv",
    "glGetTextureParameteriv",
    "glCreateVertexArrays",
    "glDisableVertexArrayAttrib",
    "glEnableVertexArrayAttrib",
    "glVertexArrayElementBuffer",
    "glVertexArrayVertexBuffer",
    "glVertexArrayVertexBuffers",
    "glVertexArrayAttribBinding",
    "glVertexArrayAttribFormat",
    "glVertexArrayAttribIFormat",
    "glVertexArrayAttribLFormat",
    "glVertexArrayBindingDivisor",
    "glGetVertexArrayiv",
    "glGetVertexArrayIndexediv",
    "glGetVertexArrayIndexed64iv",
    "glCreateSamplers",
    "glCreateProgramPipelines",
    "glCreateQueries",
    "glGetQueryBufferObjecti64v",
    "glGetQueryBufferObjectiv",
    "glGetQueryBufferObjectui64v",
    "glGetQueryBufferObjectuiv",
    "glMemoryBarrierByRegion",
    "glGetTextureSubImage",
    "glGetCompressedTextureSubImage",
    "glGetGraphicsResetStatus",
    "glGetnCompressedTexImage",
    "glGetnTexImage",
    "glGetnUniformdv",
    "glGetnUniformfv",
    "glGetnUniformiv",
    "glGetnUniformuiv",
    "glReadnPixels",
    "glTextureBarrier", // 109
    // 460
    "glSpecializeShader", // 0
    "glMultiDrawArraysIndirectCount",
    "glMultiDrawElementsIndirectCount",
    "glPolygonOffsetClamp" // 3
};

_SGL int glVmtBases[][10] =
{
    {   0,   0,   0,   0,   0,   0,   0,   0, 0, 0 }, // version zero doesnt exist in GL
    {   0,  48,  62,  66,  75,  84,   0,   0, 0, 0 },
    { 103, 196,   0,   0,   0,   0,   0,   0, 0, 0 },
    { 202, 286, 298, 317,   0,   0,   0,   0, 0, 0 },
    { 345, 391, 479, 491, 534, 543, 653, 657, 0, 0 },
};

_SGL int glVmtRanges[][10] =
{
    {  0,  0,  0,  0, 0,   0, 0, 0, 0, 0 }, // version zero doesnt exist in GL
    { 48, 14,  4,  9, 9,  19, 0, 0, 0, 0 },
    { 93,  6,  0,  0, 0,   0, 0, 0, 0, 0 },
    { 84, 12, 19, 28, 0,   0, 0, 0, 0, 0 },
    { 46, 88, 12, 43, 9, 110, 4, 0, 0, 0 },
};

static const char *wglNames[] =
{
    "wglCreateContext",
    "wglDeleteContext",
    "wglGetCurrentContext",
    "wglGetCurrentDC",
    "wglGetProcAddress",
    "wglMakeCurrent",
    "wglChoosePixelFormat",
    "wglDescribePixelFormat",
    "wglSetPixelFormat",
    "wglGetPixelFormat",
    "wglSwapBuffers",

    // ARB/EXT
    "wglChoosePixelFormatARB",
    "wglMakeContextCurrentARB",
    "wglGetCurrentReadDCARB",
    "wglCreateContextAttribsARB",
    "wglGetExtensionsStringARB",
    "wglSwapIntervalEXT",
};

afxError SglLoadOpenGlVmt(HMODULE opengl32, afxNat base, afxNat cnt, afxChar const *names[], void* vmt[])
{
    afxError err = AFX_ERR_NONE;
    afxString128 tmp;
    AfxString128(&tmp);
    afxString name;
    afxString arbAndExt[2];
    AfxWrapStringLiteral(&arbAndExt[0], "ARB", 3);
    AfxWrapStringLiteral(&arbAndExt[1], "EXT", 3);
    
    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(opengl32);
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    
    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        AfxWrapStringLiteral(&name, names[base + i], 0);
        void *sym = NIL;

        if (f)
        {
            if (!(sym = f((void*)name.src.start)))
            {
                for (afxNat j = 0; j < 2; j++)
                {
                    AfxFormatString(&tmp.str, "%.*s%.*s", AfxPushString(&name), AfxPushString(&arbAndExt[j]));

                    if ((sym = f((void*)tmp.buf)))
                    {
                        AfxEcho("%.*s @ %i found.", AfxPushString(&tmp.str), i);
                        break;
                    }
                }
            }
            else
            {
                AfxEcho("%s @ %i found.", names[base + i], i);
            }
        }

        if (!sym && (sym = GetProcAddress(opengl32, names[base + i])))
        {
            AfxEcho("%s fallback @ %i found.", names[base + i], i);
        }

        vmt[base + i] = sym;
    }
    return err;
}

_SGL GLenum AfxToGlFrontFace(afxFrontFace ff)
{
    static GLenum const v[] =
    {
        GL_CCW,
        GL_CW
    };

    return v[ff];
};

_SGL GLenum AfxToGlCullMode(afxCullMode cm)
{
    static GLenum const v[] =
    {
        NIL,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK
    };

    return v[cm];
};

_SGL GLenum AfxToGlPolygonMode(afxFillMode pm)
{
    static GLenum const v[] =
    {
        GL_FILL,
        GL_LINE,
        GL_POINT
    };

    return v[pm];
};

_SGL GLenum SglToGlCompareOp(afxCompareOp co)
{
    static GLenum const v[] =
    {
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_GREATER,
        GL_ALWAYS
    };

    return v[co];
};

_SGL GLuint AfxToGlShaderStage(afxShaderStage s)
{
    static GLuint const v[] =
    {
        NIL,
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_GEOMETRY_SHADER,
        GL_TESS_CONTROL_SHADER,
        GL_TESS_EVALUATION_SHADER,
        GL_COMPUTE_SHADER,
        NIL
    };

    return v[s];
};

_SGL GLenum AfxToGlTopology(afxPrimTopology pm)
{
    static GLuint const v[] =
    {
        GL_POINTS,

        GL_LINES,
        GL_LINE_STRIP,

        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,

        GL_LINES_ADJACENCY,
        GL_LINE_STRIP_ADJACENCY,

        GL_TRIANGLES_ADJACENCY,
        GL_TRIANGLE_STRIP_ADJACENCY,

        GL_PATCHES
    };

    return v[pm];
};

_SGL GLenum AfxToGlBufferUsage(afxBufferUsage usage)
{
    static GLuint const v[] =
    {
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        GL_PIXEL_PACK_BUFFER,
        GL_PIXEL_UNPACK_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        GL_ARRAY_BUFFER,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_DRAW_INDIRECT_BUFFER
    };

    return v[usage];
};

_SGL GLenum AfxToGlStencilOp(afxStencilOp so)
{
    static GLuint const v[] =
    {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INCR,
        GL_DECR,
        GL_INVERT,
        GL_INCR_WRAP,
        GL_DECR_WRAP
    };

    return v[so];
}

_SGL afxChar const targaSigmaSignature[];
afxChar const targaSigmaSignature[] =
{
    "\n           :::::::::::     :::     :::::::::   ::::::::      :::               "
    "\n               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             "
    "\n               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            "
    "\n               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           "
    "\n               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           "
    "\n               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           "
    "\n               ###     ###     ### ###    ###  ########  ###     ###           "
    "\n                                                                               "
    "\n                  S I G M A   T E C H N O L O G Y   G R O U P                  "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n                           (c) 2017 Federacao SIGMA                            "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
};

_SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;
    
    if (!(rslt = wgl->MakeCurrent(hdc, hrc)))
        //if (!(rslt = wglMakeCurrent(hdc, hrc)))
            AfxThrowError();

    return rslt;
}

_SGL afxError SglSwapBuffers(HDC hdc, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;

    
    if (!(wgl->SwapBuffers(hdc)))
        if (!(SwapBuffers(hdc)))
            AfxThrowError();

    //SetWindowTextA(dout->wglWnd, dout->title.buf); // deadlocks mem threads on exit
    //UpdateWindow(dout->wglWnd);
    //AfxYieldThreading();

    return err;
}

_SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;
    int fmt;

    if (!(fmt = wgl->ChoosePixelFormat(hdc, ppfd)))
        if (!(fmt = ChoosePixelFormat(hdc, ppfd)))
            AfxThrowError();

    return fmt;
}

_SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;
    BOOL rslt;

    if (!(rslt = wgl->SetPixelFormat(hdc, format, ppfd)))
        if (!(rslt = SetPixelFormat(hdc, format, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = wgl->DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
        if (!(rslt = DescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd)))
            AfxThrowError();

    return rslt;
}

_SGL int SglGetPixelFormat(HDC hdc, wglVmt const *wgl)
{
    afxError err = AFX_ERR_NONE;
    int rslt;

    if (!(rslt = wgl->GetPixelFormat(hdc)))
        if (!(rslt = GetPixelFormat(hdc)))
            AfxThrowError();

    return rslt;
}

_SGL LRESULT WINAPI _SglWndHndlngPrcW32Callback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (void*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (dout)
    {
        AfxAssertObject(dout, AFX_FCC_DOUT);
        sglDoutIdd *idd = dout->idd;
        AfxAssert(idd->wnd == hWnd);

        switch (message)
        {
        case WM_CLOSE:
        {

            // Sent as a signal that a window or an application should terminate.
            // wParam = This parameter is not used.
            // lParam = This parameter is not used.
            // If an application processes this message, it should return zero.

            //_SglDoutProcess(dout);

            //while (0 < AfxReleaseObject(&dout->obj));
#if 0
            afxDrawContext dctx = AfxGetConnectedDrawOutputContext(dout);

            if (!dctx) AfxReleaseDrawOutput(dout);
            else
            {
                struct msgdecl { afxSize tid; afxSize hwnd; afxSize wParam; afxSize lParam; } *msgdecl = AfxAllocate(NIL, sizeof(struct msgdecl), AfxSpawnHint());
                msgdecl->tid = GetThreadId(dctx->thread);
                msgdecl->hwnd = hWnd;
                msgdecl->wParam = wParam;
                msgdecl->lParam = lParam;
                PostThreadMessageA(msgdecl->tid, message, NIL, msgdecl);
            }
#endif//0
            return 0; // handled. Also prevent process at end of this function
        }
        case WM_SIZE:
        {
            //if (dout->resizable)
            {
                /*
                    WM_SIZE
                        Sent to a window after its size has changed.

                    wParam --- The type of resizing requested. This parameter canv be one of the following values.
                        SIZE_MAXHIDE = 4 --- Message is sent to mem pop-up windows when some other window is maximized.
                        SIZE_MAXIMIZED = 2 --- The window has been maximized.
                        SIZE_MAXSHOW = 3 --- Message is sent to mem pop-up windows when some other window has been restored to its former size.
                        SIZE_MINIMIZED = 1 --- The window has been minimized.
                        SIZE_RESTORED = 0 --- The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

                    lParam
                        The low-order word of lParam specifies the new width of the client area.
                        The high-order word of lParam specifies the new height of the client area.

                */

                afxWhd whdNew = { LOWORD(lParam), HIWORD(lParam), 1 };

                if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
                {
                    AfxReadjustDrawOutput(dout, whdNew);
                }
            }
            //AfxDrawOutputProcess(dout);
            break;
        }
        case WM_DISPLAYCHANGE: // The WM_DISPLAYCHANGE message is sent to all windows when the display resolution has changed.
        {
            afxWhd const resolution = { GetDeviceCaps(idd->dc, HORZRES), GetDeviceCaps(idd->dc, VERTRES), GetDeviceCaps(idd->dc, PLANES) };
            dout->refreshRate = GetDeviceCaps(idd->dc, VREFRESH);
            afxReal64 physAspRatio = (afxReal64)GetDeviceCaps(idd->dc, HORZSIZE) / (afxReal64)GetDeviceCaps(idd->dc, VERTSIZE);
            
            AfxReadjustDrawOutputProportion(dout, physAspRatio, resolution);
            break;
        }
        case WM_STYLECHANGED: // Sent to a window after the SetWindowLong function has changed one or more of the window's styles.
        {
            // Sent to a window after the SetWindowLong function has changed one or more of the window's styles. A window receives this message through its WindowProc function.
            // wParam = Indicates whether the window's styles or extended window styles have changed. This parameter can be one or more of the following values.
            
            RECT rect;
            GetClientRect(idd->wnd, &(rect));
            afxWhd whdNew = { rect.right - rect.left, rect.bottom - rect.top, 1 };

            if (whdNew[0] * whdNew[1] * whdNew[2]) // don't set to zero
            {
                AfxReadjustDrawOutput(dout, whdNew);
            }
            break;
        }
        case WM_MOUSEMOVE:
        {
            // TODO build a kind of handler for draw input to deal with it.

            POINTS points = MAKEPOINTS(lParam);

            afxV2d curr = { AfxScalar(points.x), AfxScalar(points.y) };

            AfxV2dSub(dout->absCursorPos, curr, dout->absCursorMove);
            AfxV2dCopy(dout->absCursorPos, curr);

            afxV2d screen = { AfxScalar(dout->extent[0]), AfxScalar(dout->extent[1]) };

            AfxExtractV2dInNdc(dout->ndcCursorPos, dout->absCursorPos, screen);
            AfxExtractV2dInNdc(dout->ndcCursorMove, dout->absCursorMove, screen);

            //data2->breake = TRUE;
            break;
        }
        case WM_DROPFILES: // Sent when the user drops a file on the window of an application that has registered itself as a recipient of dropped files.
        {
            // TODO build a kind of handler for draw input to deal with it.

            afxFileDrop fdrop;
            AFX_ZERO(&fdrop);

            POINT ppt;
            HDROP hDrop = (HDROP)wParam;
            DragQueryPoint(hDrop, &ppt);

            fdrop.x = ppt.x;
            fdrop.y = ppt.y;
            afxNat cnt = DragQueryFileA(hDrop, 0xFFFFFFFF, NIL, NIL);

            AfxAcquireArray(NIL, &fdrop.files, sizeof(afxChar const*), 0, AfxSpawnHint());

            afxChar* name = NIL;
            afxNat len = 0;
            afxNat i;

            afxDrawContext dctx;
            AfxGetConnectedDrawOutputContext(dout, &dctx);
            AfxAssertObject(dctx, AFX_FCC_DCTX);
            afxMemory mem = AfxGetDrawContextMemory(dctx);
            AfxAssertObject(mem, AFX_FCC_MEM);

            for (i = 0; i < cnt; i++)
            {
                len = DragQueryFileA(hDrop, i, NIL, 0);

                if (!(name = AfxAllocate(mem, len + 1, 0, AfxSpawnHint()))) AfxError("");
                else
                {
                    DragQueryFileA(hDrop, i, name, len + 1);
                    afxNat arrIdx;
                    void *arrel = AfxInsertArrayUnit(&fdrop.files, &arrIdx);
                    AfxCopy(arrel, name, sizeof(name));
                }
            }

            afxEvent ev;
            AfxEventDeploy(&ev, AFX_EVENT_DOUT_DRAGNDROP, &dout->obj, &fdrop);
            AfxObjectEmitEvent(&dout->obj, &ev);

            for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
            {
                AfxEcho("%s", *(afxChar const**)AfxGetArrayUnit(&fdrop.files, i));
            }

            for (i = 0; i < AfxGetArrayPop(&fdrop.files); i++)
            {
                AfxDeallocate(mem, *(afxChar**)AfxGetArrayUnit(&fdrop.files, i));
            }

            AfxReleaseArray(&fdrop.files);

            DragFinish(hDrop);
            //data2->breake = TRUE;
            //return 0;
            break;
        }
        case WM_KILLFOCUS: // Sent to a window immediately before it loses the keyboard focus.
        {
            dout->focused = FALSE;
            // TODO pause draw input to reduce resource usage?

            //BinkPause(Back_Bink, 1);
            //BinkPause(Alpha_Bink, 1);
            break;
        }
        case WM_SETFOCUS: // Sent to a window after it has gained the keyboard focus.
        {
            dout->focused = TRUE;
            // TODO resume draw input of pause to reduce resource usage?

            //BinkPause(Back_Bink, 0);
            //BinkPause(Alpha_Bink, 0);
            break;
        }
        //case WM_PAINT: return(0);
        case WM_ERASEBKGND: return(0);
        default: break;
        }
        //_SglDoutProcess(dout);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
1  2  3  4
2  1  4  3
3  4  1  2
4  3  2  1
*/

_SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc, wglVmt const* wgl, HMODULE opengl32)
{
    AfxEntry("oglWndClss=%p,shareCtx=%p,phwnd=%p,phdc=%p,phrc=%p", oglWndClss, shareCtx, phwnd, phdc, phrc);
    afxError err = AFX_ERR_NONE;
    HGLRC(WINAPI*_wglCreateContext)(HDC);
    BOOL(WINAPI*_wglDeleteContext)(HGLRC);
    HGLRC(WINAPI*_wglGetCurrentContext)(VOID);
    HDC(WINAPI*_wglGetCurrentDC)(VOID);
    PROC(WINAPI*_wglGetProcAddress)(LPCSTR);
    BOOL(WINAPI*_wglMakeCurrent)(HDC, HGLRC);
    int(WINAPI*_wglChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    int(WINAPI*_wglDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
    BOOL(WINAPI*_wglSetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    int(WINAPI*_wglGetPixelFormat)(HDC hdc);

    _wglCreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
    _wglDeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
    _wglGetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
    _wglGetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
    _wglGetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    _wglMakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
    _wglChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    _wglSetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    _wglDescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
    _wglGetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");

    HDC bkpHdc = _wglGetCurrentDC();
    HGLRC bkpGlrc = _wglGetCurrentContext();

    HWND tmpHwnd = CreateWindowA(oglWndClss->lpszClassName, "", /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, oglWndClss->hInstance, NIL);

    if (!(tmpHwnd)) AfxThrowError();
    else
    {
        HDC tmpHdc = GetDC(tmpHwnd);
        //hdc = GetWindowDC(hwnd);

        if (!(tmpHdc)) AfxThrowError();
        else
        {
            PIXELFORMATDESCRIPTOR pfd = { 0 };
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DIRECT3D_ACCELERATED | PFD_SUPPORT_COMPOSITION | PFD_SWAP_EXCHANGE;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = GetDeviceCaps(tmpHdc, BITSPIXEL);
            //pfd.cDepthBits = 24;
            //pfd.cStencilBits = 8;

            int pxlfmt;

            if (!(pxlfmt = _wglChoosePixelFormat(tmpHdc, &(pfd))))
                if (!(pxlfmt = ChoosePixelFormat(tmpHdc, &(pfd))))
                    AfxThrowError();

            if (!_wglDescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd))
                if (!(DescribePixelFormat(tmpHdc, pxlfmt, sizeof(pfd), &pfd)))
                    AfxThrowError();

            if (!_wglSetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                if (!SetPixelFormat(tmpHdc, pxlfmt, &(pfd)))
                    AfxThrowError();
            
            HGLRC tmpHrc = _wglCreateContext(tmpHdc);

            if (!tmpHrc) AfxThrowError();
            else
            {
                if (!(_wglMakeCurrent(tmpHdc, tmpHrc))) AfxThrowError();
                else
                {
                    wglVmt wgl;
                    SglLoadOpenGlVmt(opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, wgl.ptr);
                    AfxAssert(wgl.ChoosePixelFormatARB);

                    HWND hwnd2 = CreateWindowA(oglWndClss->lpszClassName, "", /*WS_POPUP*/WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 1, 1, NIL, NIL, oglWndClss->hInstance, NIL);

                    if (!(hwnd2)) AfxThrowError();
                    else
                    {
                        HDC hdc2 = GetDC(hwnd2);

                        if (!(hdc2)) AfxThrowError();
                        else
                        {
                            /*
                            WGL_SWAP_METHOD_ARB
                            If the pixel format supports a back buffer, then this indicates
                            how they are swapped. If this attribute is set to
                            WGL_SWAP_EXCHANGE_ARB then swapping exchanges the front and back
                            buffer contents; if it is set to WGL_SWAP_COPY_ARB then swapping
                            copies the back buffer contents to the front buffer; if it is
                            set to WGL_SWAP_UNDEFINED_ARB then the back buffer contents are
                            copied to the front buffer but the back buffer contents are
                            undefined after the operation. If the pixel format does not
                            support a back buffer then this parameter is set to
                            WGL_SWAP_UNDEFINED_ARB. The <iLayerPlane> parameter is ignored
                            if this attribute is specified.
                            */
                            int pxlAttrPairs[][2] =
                            {
                                { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE },
                                { WGL_SUPPORT_OPENGL_ARB, GL_TRUE },
                                //{ WGL_SUPPORT_GDI_ARB, GL_TRUE },
                                { WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB },
                                { WGL_DOUBLE_BUFFER_ARB, GL_TRUE },
                                { WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB }, // WGL_TYPE_RGBA_FLOAT_ARB
                                { WGL_COLOR_BITS_ARB, GetDeviceCaps(hdc2, BITSPIXEL) },
                                //{ WGL_ALPHA_BITS_ARB, 8 },
                                //{ WGL_DEPTH_BITS_ARB, 0 },
                                //{ WGL_STENCIL_BITS_ARB, 0 },
                                //{ WGL_TRANSPARENT_ARB, GL_TRUE },
                                { WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB },
                                //{ WGL_SAMPLE_BUFFERS_ARB, GL_TRUE },  // works on Intel, didn't work on Mesa
                                //{ WGL_SAMPLES_ARB, 8 },  // works on Intel, didn't work on Mesa
                                //{ WGL_COLORSPACE_EXT, WGL_COLORSPACE_SRGB_EXT }, // WGL_COLORSPACE_LINEAR_EXT // works on Mesa, didn't work on Intel
                                //{ WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE }, // works on Mesa, didn't work on Intel
                                { NIL, NIL },
                            };

                            UINT formatCount;
                            pxlfmt = 0;

                            if (!wgl.ChoosePixelFormatARB(hdc2, &pxlAttrPairs[0][0], 0, 1, &(pxlfmt), &(formatCount)))
                                AfxThrowError();

                            AfxAssert(pxlfmt);
                            AfxAssert(formatCount);

                            AFX_ZERO(&pfd);
                            SglDescribePixelFormat(hdc2, pxlfmt, sizeof(pfd), &(pfd), &wgl);

                            if (!(SglSetPixelFormat(hdc2, pxlfmt, &(pfd), &wgl)))
                                AfxThrowError();

                            int ctxAttrPairs[][2] =
                            {
                                { WGL_CONTEXT_MAJOR_VERSION_ARB, 3 },
                                { WGL_CONTEXT_MINOR_VERSION_ARB, 3 },
                                { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB }, // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
                                { WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#ifdef _AFX_DEBUG
                                | WGL_CONTEXT_DEBUG_BIT_ARB
#endif
                                }, // |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB
                                { NIL, NIL }
                            };

                            tmpHrc = NIL;

                            HGLRC  hrc2 = wgl.CreateContextAttribsARB(hdc2, shareCtx, (void*)ctxAttrPairs);

                            if (!hrc2) AfxThrowError();
                            else
                            {
                                if (!wgl.MakeCurrent(hdc2, hrc2)) AfxThrowError();
                                else
                                {
                                    AfxAssert(phwnd);
                                    *phwnd = hwnd2;
                                    AfxAssert(phdc);
                                    *phdc = hdc2;
                                    AfxAssert(phrc);
                                    *phrc = hrc2;

                                    wgl.MakeCurrent(bkpHdc, bkpGlrc);
                                }

                                if (err && hrc2)
                                    wgl.DeleteContext(hrc2);
                            }

                            if (err && hdc2)
                                ReleaseDC(hwnd2, hdc2);
                        }

                        if (err && hwnd2)
                            DestroyWindow(hwnd2);
                    }
                }

                _wglMakeCurrent(bkpHdc, bkpGlrc);

                if (tmpHrc)
                    _wglDeleteContext(tmpHrc);
            }

            if (tmpHdc)
                ReleaseDC(tmpHwnd, tmpHdc);
        }

        if (tmpHwnd)
            DestroyWindow(tmpHwnd);
    }
    return err;
}

_SGL afxError _SglDthrProcessDpuDeletionQueue(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *idd = ddrv->idd;
    sglDpuIdd *dpu = &idd->dpus[unitIdx];
    wglVmt const* wgl = &dpu->wgl;
    glVmt const *gl = &dpu->gl;

    //HDC dc = wgl->GetCurrentDC();
    HGLRC rc = wgl->GetCurrentContext();
    AfxAssert(rc == dpu->glrc);

    if (/*dc != idd->wglDc || */rc != dpu->glrc)
    {
        if (!wgl->MakeCurrent(dpu->dc, dpu->glrc))
        {
            AfxThrowError();
            AfxError("DPU %u failed to be set.", unitIdx);
            wgl->MakeCurrent(dpu->dc, dpu->glrc);
        }
    }

    _sglDeleteGlRes *delRes;

    if (AfxTryEnterSlockExclusive(&dpu->deletionLock))
    {
        if ((delRes = AfxPullNextQueueUnit(&dpu->deletionQueue)))
        {
            switch (delRes->type)
            {
            case 0: // buf
                gl->DeleteBuffers(1, &delRes->gpuHandle);
                break;
            case 1: // tex
                gl->DeleteTextures(1, &delRes->gpuHandle);
                break;
            case 2: // surf
                gl->DeleteRenderbuffers(1, &delRes->gpuHandle);
                break;
            case 3: // canv
                gl->DeleteFramebuffers(1, &delRes->gpuHandle);
                break;
            case 4: // smp
                gl->DeleteSamplers(1, &delRes->gpuHandle);
                break;
            case 5: // pip
                gl->DeleteProgram(delRes->gpuHandle);
                break;
            case 6: // shd
                gl->DeleteShader(delRes->gpuHandle);
                break;
            default:
                AfxError("");
                break;
            }

            delRes->gpuHandle = 0;
            delRes->type = 0;

            AfxPopNextQueue(&dpu->deletionQueue);
        }
        AfxExitSlockExclusive(&dpu->deletionLock);
    }
    return err;
}

_SGL void _SglDeleteGlRes(afxDrawContext dctx, afxNat type, GLuint gpuHandle)
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawDriver ddrv = AfxGetDrawContextDriver(dctx);
    AfxAssertObject(ddrv, AFX_FCC_DDRV);    
    _sglDdrvIdd *idd = ddrv->idd;
    afxNat32 unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    sglDpuIdd*dpu = &idd->dpus[unitIdx];
    AfxEnterSlockExclusive(&dpu->deletionLock);
    _sglDeleteGlRes delRes;
    delRes.gpuHandle = gpuHandle;
    delRes.type = type;
    AfxPushQueueUnit(&dpu->deletionQueue, &delRes);
    AfxExitSlockExclusive(&dpu->deletionLock);
};

_SGL afxError _SglBuildDpu(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    _sglDdrvIdd *idd = ddrv->idd;
    //afxNat procUnitIdx = AfxGetThreadingUnit();
    sglDpuIdd *dpu = &idd->dpus[unitIdx];

    AfxAcquireSlock(&dpu->deletionLock);
    AfxAcquireQueue(&dpu->deletionQueue, sizeof(_sglDeleteGlRes), 32);

    wglVmt const* wgl = &dpu->wgl;

    if (_SglCreateCombinedDeviceContext(&idd->wndClss, idd->dpus[0].glrc, &(dpu->wnd), &(dpu->dc), &(dpu->glrc), wgl, idd->opengl32)) AfxThrowError();
    else
    {
        SglLoadOpenGlVmt(idd->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, &wgl->ptr[0]);

        if (!(wgl->MakeCurrent(dpu->dc, dpu->glrc))) AfxThrowError();
        else
        {
            SglLoadOpenGlVmt(idd->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, &wgl->ptr[0]);

            dpu->dcPxlFmt = SglGetPixelFormat(dpu->dc, wgl);

            afxNat aa = GetDeviceCaps(dpu->dc, HORZSIZE);
            afxNat ab = GetDeviceCaps(dpu->dc, VERTSIZE);
            afxNat ac = GetDeviceCaps(dpu->dc, HORZRES);
            afxNat ad = GetDeviceCaps(dpu->dc, VERTRES);
            afxNat ae = GetDeviceCaps(dpu->dc, LOGPIXELSX);
            afxNat af = GetDeviceCaps(dpu->dc, LOGPIXELSY);
            afxNat ag = GetDeviceCaps(dpu->dc, BITSPIXEL);
            afxNat ah = GetDeviceCaps(dpu->dc, PLANES);
            afxNat ai = GetDeviceCaps(dpu->dc, ASPECTX);
            afxNat aj = GetDeviceCaps(dpu->dc, ASPECTY);
            afxNat ak = GetDeviceCaps(dpu->dc, ASPECTXY);
            afxNat al = GetDeviceCaps(dpu->dc, VREFRESH);
            afxNat am = GetDeviceCaps(dpu->dc, BLTALIGNMENT);
            AfxAdvertise("%i %i %i %i %i %i %i %i %i %i %i %i %i ", aa, ab, ac, ad, ae, af, ag, ah, ai, aj, ak, al, am);


            HWND dsk = GetDesktopWindow();
            HDC dskWndDc2 = GetWindowDC(dsk);
            HDC dskWndDc = GetWindowDC(NULL);
            HDC dskDc2 = GetDC(dsk);
            HDC dskDc = GetDC(NULL);
            int dskPf = SglGetPixelFormat(dskDc, wgl);
            int dskWndPf = SglGetPixelFormat(dskWndDc, wgl);
            PIXELFORMATDESCRIPTOR pfd, pfd2;
            SglDescribePixelFormat(dskDc, dskPf, sizeof(pfd), &pfd, wgl);
            SglDescribePixelFormat(dskWndDc, dskWndPf, sizeof(pfd2), &pfd2, wgl);

            glVmt* gl = (glVmt*)&dpu->gl;
            SglLoadOpenGlVmt(idd->opengl32, 30, glVmtRanges[1][0] - 30, glVmtNames, &gl->ptr[0]); // only load get integer, string, etc.

            afxString ver;
            AfxWrapStringLiteral(&ver, (afxChar const*)gl->GetString(GL_VERSION), 0);
            AfxScanString(&ver, "%u.%u.%u", &dpu->verMajor, &dpu->verMinor, &dpu->verPatch);
            gl->GetIntegerv(GL_MAJOR_VERSION, (void*)&(dpu->verMajor));
            gl->GetIntegerv(GL_MINOR_VERSION, (void*)&(dpu->verMinor));

            SglLoadOpenGlVmt(idd->opengl32, 0, sizeof(glVmtNames) / sizeof(glVmtNames[0]), glVmtNames, &gl->ptr[0]);

            //afxString tmp;
            AfxWrapStringLiteral(&dpu->subsysName, (afxChar const*)gl->GetString(GL_RENDERER), 0);
            AfxWrapStringLiteral(&dpu->subsysVer, (afxChar const*)gl->GetString(GL_VERSION), 0);

            AfxAssist("DPU#%u %.*s", unitIdx, AfxPushString(&dpu->subsysName));
            AfxAssist("DPU#%u %.*s", unitIdx, AfxPushString(&dpu->subsysVer));

            AfxAssist("%s", wgl->GetExtensionsStringARB(dpu->dc));

            //gl->Enable(GL_FRAMEBUFFER_SRGB);
            gl->Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            //dthr->thr.proc = _SglDthrProcCb;


            //HDC dcBkp = _wglGetCurrentDC();
            //HGLRC rcBkp = _wglGetCurrentContext();

            //if (!_wglMakeCurrent(idd->dpus[unitIdx].wglDc, idd->dpus[unitIdx].wglGlrc))
                //AfxThrowError();
        }

        if (err)
        {
            wgl->MakeCurrent(NIL, NIL);
            wgl->DeleteContext(dpu->glrc);
            ReleaseDC(dpu->wnd, dpu->dc);
            DestroyWindow(dpu->wnd);
        }
    }

    return err;
}

_SGL afxError _SglDestroyDpu(afxDrawDriver ddrv, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);    
    _sglDdrvIdd *idd = ddrv->idd;
    sglDpuIdd *dpu = &idd->dpus[unitIdx];

    _SglDthrProcessDpuDeletionQueue(ddrv, unitIdx);
    AfxReleaseSlock(&dpu->deletionLock);
    AfxReleaseQueue(&dpu->deletionQueue);

    wglVmt const* wgl = &dpu->wgl;

    HDC dc = wgl->GetCurrentDC();
    HGLRC rc = wgl->GetCurrentContext();

    if (dc == dpu->dc || rc == dpu->glrc)
        wgl->MakeCurrent(NIL, NIL);

    wgl->DeleteContext(dpu->glrc);

    ReleaseDC(dpu->wnd, dpu->dc);
    DestroyWindow(dpu->wnd);
    return err;
}

_SGL afxError _SglDdrvVmtDtorCb(afxDrawDriver ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);

    _sglDdrvIdd *idd = ddrv->idd;

    UnregisterClass(idd->wndClss.lpszClassName, idd->wndClss.hInstance);
    
    afxMemory mem = AfxGetDrawMemory();
    AfxAssertObject(mem, AFX_FCC_MEM);

    for (afxNat i = 0; i < idd->dpuCnt; i++)
        if (_SglDestroyDpu(ddrv, i))
            AfxThrowError();

    AfxDeallocate(mem, idd->dpus);

    AfxDeallocate(mem, idd);
    ddrv->idd = NIL;

    return err;
}

_SGL afxBool _SglProcessContextCb(afxDrawContext dctx, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxDrawThread dthr = udd;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    dthr->dctx = dctx;

    if (dctx->vmt->proc(dctx, dthr))
        AfxThrowError();

    dthr->dctx = NIL;
    return FALSE; // don't interrupt curation;
}

_SGL afxError _SglDdrvVmtProcCb(afxDrawDriver ddrv, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

    _sglDdrvIdd *idd = ddrv->idd;

    if (unitIdx >= idd->dpuCnt)
        return err;

    dthr->portIdx = unitIdx;

    AfxCurateDrawContexts(ddrv, 0, AfxGetDrawContextCount(ddrv), _SglProcessContextCb, dthr);

    _SglDthrProcessDpuDeletionQueue(ddrv, unitIdx); // delete after is safer?

    return err;
}

_SGL _afxDdrvVmt SglDrvVmt =
{
    _SglDdrvVmtDtorCb,
    _SglDdrvVmtProcCb,
    _SglDdrvVmtDqueCb,
    _SglDdrvVmtDctxCb,
    _SglDdrvVmtDoutCb,
    _SglDdrvVmtDinCb
};

_SGL afxError _SglDdrvCtorCb(afxDrawDriver ddrv, afxDrawDriverSpecification const *spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    (void)spec;

    afxNat32 tid;
    AfxGetThreadingId(&tid);

    AfxAssert(mainThreadId == tid);

    afxMemory mem = AfxGetDrawMemory();
    AfxAssertObject(mem, AFX_FCC_MEM);

    ddrv->vmt = &SglDrvVmt;

    _sglDdrvIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());

    if (!idd) AfxThrowError();
    else
    {
        ddrv->idd = idd;

        if (!(idd->opengl32 = LoadLibraryA("opengl32.dll"))) AfxError("");
        else
        {
            //SglLoadOpenGlVmt(idd->opengl32, 0, sizeof(wglNames) / sizeof(wglNames[0]), wglNames, idd->p);
#if 0
            _wglCreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
            _wglDeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
            _wglGetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
            _wglGetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
            _wglGetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
            _wglMakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
            _wglChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
            _wglSetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
            _wglDescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
            _wglGetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");
            _wglSwapBuffers = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
#endif

            idd->wndClss.cbSize = sizeof(idd->wndClss); // only on EX
            idd->wndClss.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            idd->wndClss.lpfnWndProc = _SglWndHndlngPrcW32Callback;
            idd->wndClss.cbClsExtra = sizeof(ddrv);
            idd->wndClss.cbWndExtra = sizeof((afxDrawOutput)0);
            idd->wndClss.hInstance = GetModuleHandleA(NULL);
            idd->wndClss.hIcon = NULL;
            idd->wndClss.hCursor = NULL;
            idd->wndClss.hbrBackground = NULL;
            idd->wndClss.lpszMenuName = NULL;
            idd->wndClss.lpszClassName = "OpenGL/Vulkan Continuous Integration over Qwadro (c) 2017 SIGMA --- Public Test Build";
            idd->wndClss.hIconSm = NULL;

            if (!(RegisterClassEx(&(idd->wndClss)))) AfxThrowError();
            else
            {
                AfxComment("Listing mem available display devices...\n");

                //HDC hdc = NIL;
                afxNat idx = 0;
                DISPLAY_DEVICE ddev;
                while (1)
                {
                    ddev.cb = sizeof(ddev);

                    if (!(EnumDisplayDevices(NULL, idx, &ddev, NIL))) break;
                    else
                    {
                        AfxComment("#%u = %s (%s) %x", idx, ddev.DeviceString, ddev.DeviceName, ddev.StateFlags);
                        idx++;
                    }
                };

#if 0
                for (afxNat i = 0; i < AfxGetThreadingCapacity(); i++)
                {
                    afxDrawThreadSpecification dthrSpec = { 0 };
                    //dthrSpec.base.affinityMask = AFX_FLAG(0);

                    afxDrawThread dthr;

                    if (!(dthr = _AfxAcquireDrawThread(ddrv, &dthrSpec))) AfxThrowError();
                    else
                    {
                        AfxAssertObject(dthr, AFX_FCC_DTHR);
                        AfxSetThreadAffinity(&dthr->thr, i);
                        AfxRunThread(&dthr->thr);
                    }
                }
#endif

                idd->dpuCnt = AfxGetThreadingCapacity();

                idd->dpus = AfxAllocate(mem, idd->dpuCnt * sizeof(idd->dpus[0]), 0, AfxSpawnHint());

                AfxZero(idd->dpus, idd->dpuCnt * sizeof(idd->dpus[0]));

                for (afxNat i = 0; i < idd->dpuCnt; i++)
                {
                    if (_SglBuildDpu(ddrv, i))
                    {
                        AfxThrowError();

                        for (afxNat j = 0; j < i; j++)
                            if (_SglDestroyDpu(ddrv, j))
                                AfxThrowError();
                    }
                }

                afxNat unitIdx;
                AfxGetThreadingUnit(&unitIdx);

                wglVmt const* wgl = &idd->dpus[unitIdx].wgl;

                if (!wgl->MakeCurrent(idd->dpus[unitIdx].dc, idd->dpus[unitIdx].glrc))
                    AfxThrowError();

                idd->dpuCnt = 1;

                if (err)
                {
                    UnregisterClass(idd->wndClss.lpszClassName, idd->wndClss.hInstance);
                }
            }

            FreeLibrary(idd->opengl32);
        }

        if (err)
            AfxDeallocate(mem, idd);
    }

    return err;
}

_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;

    static afxString name, author, website, note;
    AfxWrapStringLiteral(&name, "OpenGL/Vulkan Continuous Integration over Qwadro", 0);
    AfxWrapStringLiteral(&author, "SIGMA Technology Group", 0);
    AfxWrapStringLiteral(&website, "www.sigmaco.org", 0);
    AfxWrapStringLiteral(&note, /*sigglSigmaSignature*/targaSigmaSignature, 0);
    static afxDrawDriverFeatures const features = { 0 };
    static afxDrawDriverSpecification spec = { 0 };
    spec.mdle = mdle;
    spec.name = &name;
    spec.author = &author;
    spec.website = &website;
    spec.note = &note;
    spec.verMajor = 0;
    spec.verMinor = 7;
    spec.verPatch = 2;
    spec.features = &features;
    spec.execPortCnt = AfxGetThreadingCapacity();
    spec.ctor = _SglDdrvCtorCb;
    
    afxDrawDriver ddrv;

    if (AfxRegisterDrawDriver(&spec, &ddrv)) AfxThrowError();
    else
    {
        AfxAssert(ddrv->vmt);
    }
    return err;
}
