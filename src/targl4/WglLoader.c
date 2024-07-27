/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
//#define _AFX_MODULE_C
//#define _AFX_ICD_C
#define _AVX_DRAW_C
#define _AFX_THREAD_C
#define _AVX_DRAW_THREAD_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_DRIVER_C
#include "sgl.h"
#include "WglLoader.h"
#include "qwadro/exec/afxSystem.h"
#pragma comment(lib, "opengl32")

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <dwmapi.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <assert.h>

static const char*Ebenezer = "Ebenezer";

_SGL afxString const targaSignature = AFX_STRING(
    "\n           :::::::::::     :::     :::::::::   ::::::::      :::               "
    "\n               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             "
    "\n               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            "
    "\n               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           "
    "\n               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           "
    "\n               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           "
    "\n               ###     ###     ### ###    ###  ########  ###     ###           "
    "\n                                                                               "
    "\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n                   (c) 2017 SIGMA, Engineering In Technology                   "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
);

_SGL const char *glVmtNames[] =
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
#if !0
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
#endif
    // ARB/EXT
    "wglChoosePixelFormatARB",
    "wglMakeContextCurrentARB",
    "wglGetCurrentReadDCARB",
    "wglCreateContextAttribsARB",
    "wglGetExtensionsStringARB",
    "wglSwapIntervalEXT",
};

_SGL afxError SglLoadWglVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;
    afxString128 tmp;
    AfxMakeString128(&tmp, 0);
    afxString name;
    afxString arbAndExt[2];
    AfxMakeString(&arbAndExt[0], 0, "ARB", 3);
    AfxMakeString(&arbAndExt[1], 0, "EXT", 3);

    char const **names = wglNames;

    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(opengl32);
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    PROC(WINAPI *f2)(LPCSTR) = NIL;

    if (!f)
        f = (void*)GetProcAddress(opengl32, "DrvGetProcAddress");
    else
        if ((f2 = (void*)f("wglGetProcAddress")))
            f = f2;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        AfxMakeString(&name, 0, names[base + i], 0);
        void *sym = NIL;

        if (f)
        {
            if ((sym = f((void*)name.start)))
            {
                if (echo)
                {
                    AfxLogEcho("%.3u %.*s core found.", i, AfxPushString(&name));
                }
            }
            else
            {
                for (afxNat j = 0; j < 2; j++)
                {
                    AfxFormatString(&tmp.str, "%.*s%.*s", AfxPushString(&name), AfxPushString(&arbAndExt[j]));

                    if ((sym = f((void*)tmp.buf)))
                    {
                        if (echo)
                        {
                            AfxLogY("%.3u %.*s ext found.", i, AfxPushString(&tmp.str));
                        }
                        break;
                    }
                    else if ((sym = GetProcAddress(opengl32, (void*)tmp.buf)))
                    {
                        if (echo)
                        {
                            AfxLogY("%.3u %.*s fallback found.", i, AfxPushString(&tmp.str));
                        }
                        break;
                    }
                }
            }
        }

        if (!sym && (sym = GetProcAddress(opengl32, (void*)name.start)))
        {
            if (echo)
            {
                AfxLogEcho("%.3u %.*s fallback found.", i, AfxPushString(&name));
            }
        }

        vmt[i] = sym;
    }
    return err;
}

_SGL afxError SglLoadGlVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;
    afxString128 tmp;
    AfxMakeString128(&tmp, 0);
    afxString name;
    afxString arbAndExt[2];
    AfxMakeString(&arbAndExt[0], 0, "ARB", 3);
    AfxMakeString(&arbAndExt[1], 0, "EXT", 3);
    
    char const **names = glVmtNames;

    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(opengl32);
    PROC(WINAPI *f)(LPCSTR) = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    PROC(WINAPI *f2)(LPCSTR) = NIL;

    if (!f)
        f = (void*)GetProcAddress(opengl32, "DrvGetProcAddress");
    else
        if ((f2 = (void*)f("wglGetProcAddress")))
            f = f2;

    afxNat baseAddonIdx = offsetof(glVmt, ReleaseShaderCompiler) / sizeof(void*);
    
    if (baseAddonIdx >= cnt)
        baseAddonIdx = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        AfxMakeString(&name, 0, names[base + i], 0);
        void *sym = NIL;

        if (f)
        {
            if ((sym = f((void*)name.start)))
            {
                if (i >= baseAddonIdx && echo)
                {
                    AfxLogEcho("%.3u %.*s core found.", i, AfxPushString(&name));
                }
            }
            else
            {
                for (afxNat j = 0; j < 2; j++)
                {
                    AfxFormatString(&tmp.str, "%.*s%.*s", AfxPushString(&name), AfxPushString(&arbAndExt[j]));

                    if ((sym = f((void*)tmp.buf)))
                    {
                        if (i >= baseAddonIdx && echo)
                        {
                            AfxLogY("%.3u %.*s ext found.", i, AfxPushString(&tmp.str));
                        }
                        break;
                    }
                    else if ((sym = GetProcAddress(opengl32, (void*)tmp.buf)))
                    {
                        if (i >= baseAddonIdx && echo)
                        {
                            AfxLogY("%.3u %.*s fallback found.", i, AfxPushString(&tmp.str));
                        }
                        break;
                    }
                }
            }
        }

        if (!sym && (sym = GetProcAddress(opengl32, (void*)name.start)))
        {
            if (i >= baseAddonIdx && echo)
            {
                AfxLogEcho("%.3u %.*s fallback found.", i, AfxPushString(&name));
            }
        }

        vmt[i] = sym;
    }
    return err;
}

_SGL void _LoadWglBaseSymbols(HMODULE opengl32, wglVmt* wgl)
{
    wgl->CreateContext = (void*)GetProcAddress(opengl32, "wglCreateContext");
    SglLoadWglVmt(opengl32, offsetof(wglVmt, CreateContextAttribsARB) / sizeof(void*), 1, (void**)&wgl->CreateContextAttribsARB, TRUE);
    wgl->DeleteContext = (void*)GetProcAddress(opengl32, "wglDeleteContext");
    wgl->GetCurrentContext = (void*)GetProcAddress(opengl32, "wglGetCurrentContext");
    wgl->GetCurrentDC = (void*)GetProcAddress(opengl32, "wglGetCurrentDC");
    wgl->GetProcAddress = (void*)GetProcAddress(opengl32, "wglGetProcAddress");
    wgl->MakeCurrent = (void*)GetProcAddress(opengl32, "wglMakeCurrent");
    wgl->ChoosePixelFormat = (void*)GetProcAddress(opengl32, "wglChoosePixelFormat"); // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
    SglLoadWglVmt(opengl32, offsetof(wglVmt, ChoosePixelFormatARB) / sizeof(void*), 1, (void**)&wgl->ChoosePixelFormatARB, TRUE);
    wgl->SetPixelFormat = (void*)GetProcAddress(opengl32, "wglSetPixelFormat"); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
    wgl->DescribePixelFormat = (void*)GetProcAddress(opengl32, "wglDescribePixelFormat");
    wgl->GetPixelFormat = (void*)GetProcAddress(opengl32, "wglGetPixelFormat");
    wgl->SwapBuffers = (void*)GetProcAddress(opengl32, "wglSwapBuffers");
    //wgl->SwapIntervalEXT = (void*)wgl->GetProcAddress("wglSwapIntervalEXT");
    SglLoadWglVmt(opengl32, offsetof(wglVmt, SwapIntervalEXT) / sizeof(void*), 1, (void**)&wgl->SwapIntervalEXT, TRUE);
    SglLoadWglVmt(opengl32, offsetof(wglVmt, GetExtensionsStringARB) / sizeof(void*), 1, (void**)&wgl->GetExtensionsStringARB, TRUE);

    if (!wgl->CreateContext)
        wgl->CreateContext = (void*)GetProcAddress(opengl32, "DrvCreateContext");

    if (!wgl->DeleteContext)
        wgl->DeleteContext = (void*)GetProcAddress(opengl32, "DrvDeleteContext");

    if (!wgl->GetCurrentContext)
        wgl->GetCurrentContext = (void*)GetProcAddress(opengl32, "DrvGetCurrentContext");

    if (!wgl->GetCurrentDC)
        wgl->GetCurrentDC = (void*)GetProcAddress(opengl32, "DrvGetCurrentDC");

    if (!wgl->GetProcAddress)
        wgl->GetProcAddress = (void*)GetProcAddress(opengl32, "DrvGetProcAddress");

    if (!wgl->MakeCurrent)
        wgl->MakeCurrent = (void*)GetProcAddress(opengl32, "DrvMakeCurrent");

    if (!wgl->ChoosePixelFormat)
        wgl->ChoosePixelFormat = (void*)GetProcAddress(opengl32, "DrvChoosePixelFormat");

    if (!wgl->SetPixelFormat)
        wgl->SetPixelFormat = (void*)GetProcAddress(opengl32, "DrvSetPixelFormat");

    if (!wgl->DescribePixelFormat)
        wgl->DescribePixelFormat = (void*)GetProcAddress(opengl32, "DrvDescribePixelFormat");

    if (!wgl->GetPixelFormat)
        wgl->GetPixelFormat = (void*)GetProcAddress(opengl32, "DrvGetPixelFormat");

    if (!wgl->SwapBuffers)
        wgl->SwapBuffers = (void*)GetProcAddress(opengl32, "DrvSwapBuffers");
}

_SGL void _LoadWglExtendedSymbols(HMODULE opengl32, wglVmt* wgl)
{
    SglLoadWglVmt(opengl32, offsetof(wglVmt, MakeContextCurrentARB) / sizeof(void*), 1, (void**)&wgl->MakeContextCurrentARB, TRUE);
    SglLoadWglVmt(opengl32, offsetof(wglVmt, GetCurrentReadDCARB) / sizeof(void*), 1, (void**)&wgl->GetCurrentReadDCARB, TRUE);

    SglLoadWglVmt(opengl32, offsetof(wglVmt, CreateContextAttribsARB) / sizeof(void*), 1, (void**)&wgl->CreateContextAttribsARB, TRUE);
    SglLoadWglVmt(opengl32, offsetof(wglVmt, ChoosePixelFormatARB) / sizeof(void*), 1, (void**)&wgl->ChoosePixelFormatARB, TRUE);
    //wgl->SwapIntervalEXT = (void*)wgl->GetProcAddress("wglSwapIntervalEXT");
    SglLoadWglVmt(opengl32, offsetof(wglVmt, SwapIntervalEXT) / sizeof(void*), 1, (void**)&wgl->SwapIntervalEXT, TRUE);
    SglLoadWglVmt(opengl32, offsetof(wglVmt, GetExtensionsStringARB) / sizeof(void*), 1, (void**)&wgl->GetExtensionsStringARB, TRUE);
}
