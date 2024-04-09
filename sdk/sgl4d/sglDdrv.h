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

#ifndef AFX_STD_DRAW_DRIVER_DEFS_H
#define AFX_STD_DRAW_DRIVER_DEFS_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

 //#include "../src/e2coree/deps/gl/glcorearb.h"
 //#include "../src/e2coree/deps/gl/wglext.h"

#include "../dep/gl/glad.h"
#include "../dep/gl/glad_wgl.h"

#include <windows.h>
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/dev/afxDrawStream.h"

#ifndef AFX_DRAW_DRIVER_SRC
#   ifdef _DEBUG
#       define SGL _AFXIMPORT extern 
#       define SGLINL _AFXIMPORT extern inline
#   else
#       define SGL _AFXIMPORT extern 
#       define SGLINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _SGL _AFXEXPORT
#       define SGL _AFXEXPORT extern 
#       define _SGLINL _AFXEXPORT inline
#       define SGLINL _AFXEXPORT extern inline
#   else
#       define _SGL _AFXEXPORT
#       define SGL _AFXEXPORT extern 
#       define _SGLINL _AFXEXPORT inline
#       define SGLINL _AFXEXPORT extern inline
#   endif
#endif

#if 0
SGL HMODULE opengl32;
#if !0
SGL HGLRC(WINAPI* _wglCreateContext)(HDC);
SGL BOOL(WINAPI* _wglDeleteContext)(HGLRC);
SGL HGLRC(WINAPI* _wglGetCurrentContext)(VOID);
SGL HDC(WINAPI* _wglGetCurrentDC)(VOID);
SGL PROC(WINAPI* _wglGetProcAddress)(LPCSTR);
SGL BOOL(WINAPI* _wglMakeCurrent)(HDC, HGLRC);
SGL int(WINAPI* _wglChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
SGL int(WINAPI* _wglDescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
SGL BOOL(WINAPI* _wglSetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd);
SGL int(WINAPI* _wglGetPixelFormat)(HDC hdc);
SGL BOOL(WINAPI* _wglSwapBuffers)(HDC);

#endif
#endif

SGL void* glcGetProcAddress(void *deviceHandle, const GLchar *funcName);

#define SGL_DEFAULT_CTX_VER_MAJOR 3
#define SGL_DEFAULT_CTX_VER_MINOR 3
#define SGL_DEFAULT_CTX_LEGACY_SUPPORT FALSE
#define SGL_DEFAULT_CTX_EXTENSIBLE_SUPPORT TRUE

#define GL_VMT_RANGE_100 48
#define GL_VMT_RANGE_110 14
#define GL_VMT_RANGE_120 4
#define GL_VMT_RANGE_130 9
#define GL_VMT_RANGE_140 9
#define GL_VMT_RANGE_150 19
#define GL_VMT_RANGE_200 93
#define GL_VMT_RANGE_210 6
#define GL_VMT_RANGE_300 84
#define GL_VMT_RANGE_310 12
#define GL_VMT_RANGE_320 19
#define GL_VMT_RANGE_330 28
#define GL_VMT_RANGE_400 46
#define GL_VMT_RANGE_410 88
#define GL_VMT_RANGE_420 12
#define GL_VMT_RANGE_430 43
#define GL_VMT_RANGE_440 9
#define GL_VMT_RANGE_450 110
#define GL_VMT_RANGE_460 4

#define GL_VMT_BASE_100 0
#define GL_VMT_BASE_110 GL_VMT_BASE_100 + GL_VMT_RANGE_100
#define GL_VMT_BASE_120 GL_VMT_BASE_110 + GL_VMT_RANGE_110
#define GL_VMT_BASE_130 GL_VMT_BASE_120 + GL_VMT_RANGE_120
#define GL_VMT_BASE_140 GL_VMT_BASE_130 + GL_VMT_RANGE_130
#define GL_VMT_BASE_150 GL_VMT_BASE_140 + GL_VMT_RANGE_140
#define GL_VMT_BASE_200 GL_VMT_BASE_150 + GL_VMT_RANGE_150
#define GL_VMT_BASE_210 GL_VMT_BASE_200 + GL_VMT_RANGE_200
#define GL_VMT_BASE_300 GL_VMT_BASE_210 + GL_VMT_RANGE_210
#define GL_VMT_BASE_310 GL_VMT_BASE_300 + GL_VMT_RANGE_300
#define GL_VMT_BASE_320 GL_VMT_BASE_310 + GL_VMT_RANGE_310
#define GL_VMT_BASE_330 GL_VMT_BASE_320 + GL_VMT_RANGE_320
#define GL_VMT_BASE_400 GL_VMT_BASE_330 + GL_VMT_RANGE_330
#define GL_VMT_BASE_410 GL_VMT_BASE_400 + GL_VMT_RANGE_400
#define GL_VMT_BASE_420 GL_VMT_BASE_410 + GL_VMT_RANGE_410
#define GL_VMT_BASE_430 GL_VMT_BASE_420 + GL_VMT_RANGE_420
#define GL_VMT_BASE_440 GL_VMT_BASE_430 + GL_VMT_RANGE_430
#define GL_VMT_BASE_450 GL_VMT_BASE_440 + GL_VMT_RANGE_440
#define GL_VMT_BASE_460 GL_VMT_BASE_450 + GL_VMT_RANGE_450

#define SGL_VMT_SIZE_100 48 // 0-47
#define SGL_VMT_SIZE_110 SGL_VMT_SIZE_100 + 14 // 0-13
#define SGL_VMT_SIZE_120 SGL_VMT_SIZE_110 + 4 // 0-3
#define SGL_VMT_SIZE_130 SGL_VMT_SIZE_120 + 9 // 0-8
#define SGL_VMT_SIZE_140 SGL_VMT_SIZE_130 + 9 // 0-8
#define SGL_VMT_SIZE_150 SGL_VMT_SIZE_140 + 19 // 0-18
#define SGL_VMT_SIZE_200 SGL_VMT_SIZE_150 + 93 // 0-92
#define SGL_VMT_SIZE_210 SGL_VMT_SIZE_200 + 6 // 0-5
#define SGL_VMT_SIZE_300 SGL_VMT_SIZE_210 + 84 // 0-83
#define SGL_VMT_SIZE_310 SGL_VMT_SIZE_300 + 12 // 0-11
#define SGL_VMT_SIZE_320 SGL_VMT_SIZE_310 + 19 // 0-18
#define SGL_VMT_SIZE_330 SGL_VMT_SIZE_320 + 28 // 0-27
#define SGL_VMT_SIZE_400 SGL_VMT_SIZE_330 + 46 // 0-45
#define SGL_VMT_SIZE_410 SGL_VMT_SIZE_400 + 88 // 0-87
#define SGL_VMT_SIZE_420 SGL_VMT_SIZE_410 + 12 // 0-11
#define SGL_VMT_SIZE_430 SGL_VMT_SIZE_420 + 43 // 0-42
#define SGL_VMT_SIZE_440 SGL_VMT_SIZE_430 + 9 // 0-8
#define SGL_VMT_SIZE_450 SGL_VMT_SIZE_440 + 110 // 0-109
#define SGL_VMT_SIZE_460 SGL_VMT_SIZE_450 + 4 // 0-3

typedef union glVmt
{
    struct
    {
        struct
        {
            // 100
            PFNGLCULLFACEPROC CullFace;
            PFNGLFRONTFACEPROC FrontFace;
            PFNGLHINTPROC Hint;
            PFNGLLINEWIDTHPROC LineWidth;
            PFNGLPOINTSIZEPROC PointSize;
            PFNGLPOLYGONMODEPROC PolygonMode;
            PFNGLSCISSORPROC Scissor;
            PFNGLTEXPARAMETERFPROC TexParameterf;
            PFNGLTEXPARAMETERFVPROC TexParameterfv;
            PFNGLTEXPARAMETERIPROC TexParameteri;
            PFNGLTEXPARAMETERIVPROC TexParameteriv;
            PFNGLTEXIMAGE1DPROC TexImage1D;
            PFNGLTEXIMAGE2DPROC TexImage2D;
            PFNGLDRAWBUFFERPROC DrawBuffer;
            PFNGLCLEARPROC Clear;
            PFNGLCLEARCOLORPROC ClearColor;
            PFNGLCLEARSTENCILPROC ClearStencil;
            PFNGLCLEARDEPTHPROC ClearDepth;
            PFNGLSTENCILMASKPROC StencilMask;
            PFNGLCOLORMASKPROC ColorMask;
            PFNGLDEPTHMASKPROC DepthMask;
            PFNGLDISABLEPROC Disable;
            PFNGLENABLEPROC Enable;
            PFNGLFINISHPROC Finish;
            PFNGLFLUSHPROC Flush;
            PFNGLBLENDFUNCPROC BlendFunc;
            PFNGLLOGICOPPROC LogicOp;
            PFNGLSTENCILFUNCPROC StencilFunc;
            PFNGLSTENCILOPPROC StencilOp;
            PFNGLDEPTHFUNCPROC DepthFunc;
            PFNGLPIXELSTOREFPROC PixelStoref;
            PFNGLPIXELSTOREIPROC PixelStorei;
            PFNGLREADBUFFERPROC ReadBuffer;
            PFNGLREADPIXELSPROC ReadPixels;
            PFNGLGETBOOLEANVPROC GetBooleanv;
            PFNGLGETDOUBLEVPROC GetDoublev;
            PFNGLGETERRORPROC GetError;
            PFNGLGETFLOATVPROC GetFloatv;
            PFNGLGETINTEGERVPROC GetIntegerv; // 38
            PFNGLGETSTRINGPROC GetString;
            PFNGLGETTEXIMAGEPROC GetTexImage;
            PFNGLGETTEXPARAMETERFVPROC GetTexParameterfv;
            PFNGLGETTEXPARAMETERIVPROC GetTexParameteriv;
            PFNGLGETTEXLEVELPARAMETERFVPROC GetTexLevelParameterfv;
            PFNGLGETTEXLEVELPARAMETERIVPROC GetTexLevelParameteriv;
            PFNGLISENABLEDPROC IsEnabled;
            PFNGLDEPTHRANGEPROC DepthRange;
            PFNGLVIEWPORTPROC Viewport;
        };
        struct
        {
            // 110
            PFNGLDRAWARRAYSPROC DrawArrays;
            PFNGLDRAWELEMENTSPROC DrawElements;
            PFNGLGETPOINTERVPROC GetPointerv;
            PFNGLPOLYGONOFFSETPROC PolygonOffset;
            PFNGLCOPYTEXIMAGE1DPROC CopyTexImage1D;
            PFNGLCOPYTEXIMAGE2DPROC CopyTexImage2D;
            PFNGLCOPYTEXSUBIMAGE1DPROC CopyTexSubImage1D;
            PFNGLCOPYTEXSUBIMAGE2DPROC CopyTexSubImage2D;
            PFNGLTEXSUBIMAGE1DPROC TexSubImage1D;
            PFNGLTEXSUBIMAGE2DPROC TexSubImage2D;
            PFNGLBINDTEXTUREPROC BindTexture;
            PFNGLDELETETEXTURESPROC DeleteTextures;
            PFNGLGENTEXTURESPROC GenTextures;
            PFNGLISTEXTUREPROC IsTexture;
        };
        struct
        {
            // 120
            PFNGLDRAWRANGEELEMENTSPROC DrawRangeElements;
            PFNGLTEXIMAGE3DPROC TexImage3D;
            PFNGLTEXSUBIMAGE3DPROC TexSubImage3D;
            PFNGLCOPYTEXSUBIMAGE3DPROC CopyTexSubImage3D;
        };
        struct
        {// 130
            PFNGLACTIVETEXTUREPROC ActiveTexture;
            PFNGLSAMPLECOVERAGEPROC SmapleCoverage;
            PFNGLCOMPRESSEDTEXIMAGE3DPROC CompressedTexImage3D;
            PFNGLCOMPRESSEDTEXIMAGE2DPROC CompressedTexImage2D;
            PFNGLCOMPRESSEDTEXIMAGE1DPROC CompressedTexImage1D;
            PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC CompressedTexSubImage3D;
            PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC CompressedTexSubImage2D;
            PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC CompressedTexSubImage1D;
            PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;
        };
        struct
        {
            // 140
            PFNGLBLENDFUNCSEPARATEPROC BlendFuncSeparate;
            PFNGLMULTIDRAWARRAYSPROC MultiDrawArrays;
            PFNGLMULTIDRAWELEMENTSPROC MultiDrawElements;
            PFNGLPOINTPARAMETERFPROC PointParameterf;
            PFNGLPOINTPARAMETERFVPROC PointParameterfv;
            PFNGLPOINTPARAMETERIPROC PointParameteri;
            PFNGLPOINTPARAMETERIVPROC PointParameteriv;
            PFNGLBLENDCOLORPROC BlendColor;
            PFNGLBLENDEQUATIONPROC BlendEquation;
        };
        struct
        {
            // 150
            PFNGLGENQUERIESPROC GenQueries;
            PFNGLDELETEQUERIESPROC DeleteQueries;
            PFNGLISQUERYPROC IsQuery;
            PFNGLBEGINQUERYPROC BeginQuery;
            PFNGLENDQUERYPROC EndQuery;
            PFNGLGETQUERYIVPROC GetQueryiv;
            PFNGLGETQUERYOBJECTIVPROC GetQueryObjectiv;
            PFNGLGETQUERYOBJECTUIVPROC GetQueryObjectuiv;
            PFNGLBINDBUFFERPROC BindBuffer;
            PFNGLDELETEBUFFERSPROC DeleteBuffers;
            PFNGLGENBUFFERSPROC GenBuffers;
            PFNGLISBUFFERPROC IsBuffer;
            PFNGLBUFFERDATAPROC BufferData;
            PFNGLBUFFERSUBDATAPROC BufferSubData;
            PFNGLGETBUFFERSUBDATAPROC GetBufferSubData;
            PFNGLMAPBUFFERPROC MapBuffer;
            PFNGLUNMAPBUFFERPROC UnmapBuffer;
            PFNGLGETBUFFERPARAMETERIVPROC GetBufferParameteriv;
            PFNGLGETBUFFERPOINTERVPROC GetBufferPointerv;
        };
        struct
        {
            // 200
            PFNGLBLENDEQUATIONSEPARATEPROC BlendEquationSeparate;
            PFNGLDRAWBUFFERSPROC DrawBuffers;
            PFNGLSTENCILOPSEPARATEPROC StencilOpSeparate;
            PFNGLSTENCILFUNCSEPARATEPROC StencilFuncSeparate;
            PFNGLSTENCILMASKSEPARATEPROC StencilMaskSeparate;
            PFNGLATTACHSHADERPROC AttachShader;
            PFNGLBINDATTRIBLOCATIONPROC BindAttribLocation;
            PFNGLCOMPILESHADERPROC CompileShader;
            PFNGLCREATEPROGRAMPROC CreateProgram;
            PFNGLCREATESHADERPROC CreateShader;
            PFNGLDELETEPROGRAMPROC DeleteProgram;
            PFNGLDELETESHADERPROC DeleteShader;
            PFNGLDETACHSHADERPROC DetachShader;
            PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
            PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
            PFNGLGETACTIVEATTRIBPROC GetActiveAttrib;
            PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;
            PFNGLGETATTACHEDSHADERSPROC GetAttachedShader;
            PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
            PFNGLGETPROGRAMIVPROC GetProgramiv;
            PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
            PFNGLGETSHADERIVPROC GetShaderiv;
            PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
            PFNGLGETSHADERSOURCEPROC GetShaderSource;
            PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
            PFNGLGETUNIFORMFVPROC GetUniformfv;
            PFNGLGETUNIFORMIVPROC GetUniformiv;
            PFNGLGETVERTEXATTRIBDVPROC GetVertexAttribdv;
            PFNGLGETVERTEXATTRIBFVPROC GetVertexAttribfv;
            PFNGLGETVERTEXATTRIBIVPROC GetVertexAttribiv;
            PFNGLGETVERTEXATTRIBPOINTERVPROC GetVertexAttribPointerv;
            PFNGLISPROGRAMPROC IsProgram;
            PFNGLISSHADERPROC IsShader;
            PFNGLLINKPROGRAMPROC LinkProgram;
            PFNGLSHADERSOURCEPROC ShaderSource;
            PFNGLUSEPROGRAMPROC UseProgram;
            PFNGLUNIFORM1FPROC Uniform1f;
            PFNGLUNIFORM2FPROC Uniform2f;
            PFNGLUNIFORM3FPROC Uniform3f;
            PFNGLUNIFORM4FPROC Uniform4f;
            PFNGLUNIFORM1IPROC Uniform1i;
            PFNGLUNIFORM2IPROC Uniform2i;
            PFNGLUNIFORM3IPROC Uniform3i;
            PFNGLUNIFORM4IPROC Uniform4i;
            PFNGLUNIFORM1FVPROC Uniform1fv;
            PFNGLUNIFORM2FVPROC Uniform2fv;
            PFNGLUNIFORM3FVPROC Uniform3fv;
            PFNGLUNIFORM4FVPROC Uniform4fv;
            PFNGLUNIFORM1IVPROC Uniform1iv;
            PFNGLUNIFORM2IVPROC Uniform2iv;
            PFNGLUNIFORM3IVPROC Uniform3iv;
            PFNGLUNIFORM4IVPROC Uniform4iv;
            PFNGLUNIFORMMATRIX2FVPROC UniformMatrix2fv;
            PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3fv;
            PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
            PFNGLVALIDATEPROGRAMPROC ValidateProgram;
            PFNGLVERTEXATTRIB1DPROC VertexAttrib1d;
            PFNGLVERTEXATTRIB1DVPROC VertexAttrib1dv;
            PFNGLVERTEXATTRIB1FPROC VertexAttrib1f;
            PFNGLVERTEXATTRIB1FVPROC VertexAttrib1fv;
            PFNGLVERTEXATTRIB1SPROC VertexAttrib1s;
            PFNGLVERTEXATTRIB1SVPROC VertexAttrib1sv;
            PFNGLVERTEXATTRIB2DPROC VertexAttrib2d;
            PFNGLVERTEXATTRIB2DVPROC VertexAttrib2dv;
            PFNGLVERTEXATTRIB2FPROC VertexAttrib2f;
            PFNGLVERTEXATTRIB2FVPROC VertexAttrib2fv;
            PFNGLVERTEXATTRIB2SPROC VertexAttrib2s;
            PFNGLVERTEXATTRIB2SVPROC VertexAttrib2sv;
            PFNGLVERTEXATTRIB3DPROC VertexAttrib3d;
            PFNGLVERTEXATTRIB3DVPROC VertexAttrib3dv;
            PFNGLVERTEXATTRIB3FPROC VertexAttrib3f;
            PFNGLVERTEXATTRIB3FVPROC VertexAttrib3fv;
            PFNGLVERTEXATTRIB3SPROC VertexAttrib3s;
            PFNGLVERTEXATTRIB3SVPROC VertexAttrib3sv;
            PFNGLVERTEXATTRIB4NBVPROC VertexAttrib4nbv;
            PFNGLVERTEXATTRIB4NIVPROC VertexAttrib4niv;
            PFNGLVERTEXATTRIB4NSVPROC VertexAttrib4nsv;
            PFNGLVERTEXATTRIB4NUBPROC VertexAttrib4nub;
            PFNGLVERTEXATTRIB4NUBVPROC VertexAttrib4nubv;
            PFNGLVERTEXATTRIB4NUIVPROC VertexAttrib4nuiv;
            PFNGLVERTEXATTRIB4NUSVPROC VertexAttrib4nusv;
            PFNGLVERTEXATTRIB4BVPROC VertexAttrib4bv;
            PFNGLVERTEXATTRIB4DPROC VertexAttrib4d;
            PFNGLVERTEXATTRIB4DVPROC VertexAttrib4dv;
            PFNGLVERTEXATTRIB4FPROC VertexAttrib4f;
            PFNGLVERTEXATTRIB4FVPROC VertexAttrib4fv;
            PFNGLVERTEXATTRIB4IVPROC VertexAttrib4iv;
            PFNGLVERTEXATTRIB4SPROC VertexAttrib4s;
            PFNGLVERTEXATTRIB4SVPROC VertexAttrib4sv;
            PFNGLVERTEXATTRIB4UBVPROC VertexAttrib4ubv;
            PFNGLVERTEXATTRIB4UIVPROC VertexAttrib4uiv;
            PFNGLVERTEXATTRIB4USVPROC VertexAttrib4usv;
            PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
        };
        struct
        {
            // 210
            PFNGLUNIFORMMATRIX2X3FVPROC UniformMatrix2x3fv;
            PFNGLUNIFORMMATRIX3X2FVPROC UniformMatrix3x2fv;
            PFNGLUNIFORMMATRIX2X4FVPROC UniformMatrix2x4fv;
            PFNGLUNIFORMMATRIX4X2FVPROC UniformMatrix4x2fv;
            PFNGLUNIFORMMATRIX3X4FVPROC UniformMatrix3x4fv;
            PFNGLUNIFORMMATRIX4X3FVPROC UniformMatrix4x3fv;
        };
        struct
        {
            // 300
            PFNGLCOLORMASKIPROC ColorMaski;
            PFNGLGETBOOLEANI_VPROC GetBooleani_v;
            PFNGLGETINTEGERI_VPROC GetIntegeri_v;
            PFNGLENABLEIPROC Enablei;
            PFNGLDISABLEIPROC Disablei;
            PFNGLISENABLEDIPROC IsEnabledi;
            PFNGLBEGINTRANSFORMFEEDBACKPROC BeginTransformFeedback;
            PFNGLENDTRANSFORMFEEDBACKPROC EndTransformFeedback;
            PFNGLBINDBUFFERRANGEPROC BindBufferRange;
            PFNGLBINDBUFFERBASEPROC BindBufferBase;
            PFNGLTRANSFORMFEEDBACKVARYINGSPROC TransformFeedbackVaryings;
            PFNGLGETTRANSFORMFEEDBACKVARYINGPROC TransformFeedbackVarying;
            PFNGLCLAMPCOLORPROC ClampColor;
            PFNGLBEGINCONDITIONALRENDERPROC BeginConditionalRender;
            PFNGLENDCONDITIONALRENDERPROC EndConditionalRender;
            PFNGLVERTEXATTRIBIPOINTERPROC VertexAttribIPointer;
            PFNGLGETVERTEXATTRIBIIVPROC GetVertexAttribIiv;
            PFNGLGETVERTEXATTRIBIUIVPROC GetVertexAttribIuiv;
            PFNGLVERTEXATTRIBI1IPROC VertexAttribI1i;
            PFNGLVERTEXATTRIBI2IPROC VertexAttribI2i;
            PFNGLVERTEXATTRIBI3IPROC VertexAttribI3i;
            PFNGLVERTEXATTRIBI4IPROC VertexAttribI4i;
            PFNGLVERTEXATTRIBI1UIPROC VertexAttribI1ui;
            PFNGLVERTEXATTRIBI2UIPROC VertexAttribI2ui;
            PFNGLVERTEXATTRIBI3UIPROC VertexAttribI3ui;
            PFNGLVERTEXATTRIBI4UIPROC VertexAttribI4ui;
            PFNGLVERTEXATTRIBI1IVPROC VertexAttribI1iv;
            PFNGLVERTEXATTRIBI2IVPROC VertexAttribI21iv;
            PFNGLVERTEXATTRIBI3IVPROC VertexAttribI3v;
            PFNGLVERTEXATTRIBI4IVPROC VertexAttribI4iv;
            PFNGLVERTEXATTRIBI1UIVPROC VertexAttribI1uiv;
            PFNGLVERTEXATTRIBI2UIVPROC VertexAttribI2uiv;
            PFNGLVERTEXATTRIBI3UIVPROC VertexAttribI3uiv;
            PFNGLVERTEXATTRIBI4UIVPROC VertexAttribI4uiv;
            PFNGLVERTEXATTRIBI4BVPROC VertexAttribI4bv;
            PFNGLVERTEXATTRIBI4SVPROC VertexAttribI4sv;
            PFNGLVERTEXATTRIBI4UBVPROC VertexAttribI4ubv;
            PFNGLVERTEXATTRIBI4USVPROC VertexAttribI4usv;
            PFNGLGETUNIFORMUIVPROC GetUniformuiv;
            PFNGLBINDFRAGDATALOCATIONPROC BindFragDataLocation;
            PFNGLGETFRAGDATALOCATIONPROC GetFragDataLocation;
            PFNGLUNIFORM1UIPROC Uniform1ui;
            PFNGLUNIFORM2UIPROC Uniform2ui;
            PFNGLUNIFORM3UIPROC Uniform3ui;
            PFNGLUNIFORM4UIPROC Uniform4ui;
            PFNGLUNIFORM1UIVPROC Uniform1uiv;
            PFNGLUNIFORM2UIVPROC Uniform2uiv;
            PFNGLUNIFORM3UIVPROC Uniform3uiv;
            PFNGLUNIFORM4UIVPROC Uniform4uiv;
            PFNGLTEXPARAMETERIIVPROC TexParameterIiv;
            PFNGLTEXPARAMETERIUIVPROC TexParameterIuiv;
            PFNGLGETTEXPARAMETERIIVPROC GetTexParameterIiv;
            PFNGLGETTEXPARAMETERIUIVPROC GetTexParameterIuiv;
            PFNGLCLEARBUFFERIVPROC ClearBufferiv;
            PFNGLCLEARBUFFERUIVPROC ClearBufferuiv;
            PFNGLCLEARBUFFERFVPROC ClearBufferfv;
            PFNGLCLEARBUFFERFIPROC ClearBufferfi;
            PFNGLGETSTRINGIPROC GetStringi;
            PFNGLISRENDERBUFFERPROC IsRenderBuffer;
            PFNGLBINDRENDERBUFFERPROC BindRenderbuffer;
            PFNGLDELETERENDERBUFFERSPROC DeleteRenderbuffers;
            PFNGLGENRENDERBUFFERSPROC GenRenderbuffers;
            PFNGLRENDERBUFFERSTORAGEPROC RenderbufferStorage;
            PFNGLGETRENDERBUFFERPARAMETERIVPROC GetRenderBufferParameteriv;
            PFNGLISFRAMEBUFFERPROC IsFramebuffer;
            PFNGLBINDFRAMEBUFFERPROC BindFramebuffer;
            PFNGLDELETEFRAMEBUFFERSPROC DeleteFramebuffers;
            PFNGLGENFRAMEBUFFERSPROC GenFramebuffers;
            PFNGLCHECKFRAMEBUFFERSTATUSPROC CheckFramebufferStatus;
            PFNGLFRAMEBUFFERTEXTURE1DPROC FramebufferTexture1D;
            PFNGLFRAMEBUFFERTEXTURE2DPROC FramebufferTexture2D;
            PFNGLFRAMEBUFFERTEXTURE3DPROC FramebufferTexture3D;
            PFNGLFRAMEBUFFERRENDERBUFFERPROC FramebufferRenderbuffer;
            PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetFramebufferAttachmentParameteriv;
            PFNGLGENERATEMIPMAPPROC GenerateMipmap;
            PFNGLBLITFRAMEBUFFERPROC BlitFramebuffer;
            PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC RenderbufferStorageMultisample;
            PFNGLFRAMEBUFFERTEXTURELAYERPROC FramebufferTextureLayer;
            PFNGLMAPBUFFERRANGEPROC MapBufferRange;
            PFNGLFLUSHMAPPEDBUFFERRANGEPROC FlushMappedBufferRange;
            PFNGLBINDVERTEXARRAYPROC BindVertexArray;
            PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
            PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
            PFNGLISVERTEXARRAYPROC IsVertexArray;
        };
        struct
        {
            // 310
            PFNGLDRAWARRAYSINSTANCEDPROC DrawArraysInstanced;
            PFNGLDRAWELEMENTSINSTANCEDPROC DrawElementsInstanced;
            PFNGLTEXBUFFERPROC TexBuffer;
            PFNGLPRIMITIVERESTARTINDEXPROC PrimitiveRestartIndex;
            PFNGLCOPYBUFFERSUBDATAPROC CopyBufferSubData;
            PFNGLGETUNIFORMINDICESPROC GetUniformIndices;
            PFNGLGETACTIVEUNIFORMSIVPROC GetActiveUniformsiv;
            PFNGLGETACTIVEUNIFORMNAMEPROC GetActiveUniformName;
            PFNGLGETUNIFORMBLOCKINDEXPROC GetUniformBlockIndex;
            PFNGLGETACTIVEUNIFORMBLOCKIVPROC GetActiveUniformBlockiv;
            PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC GetActiveUniformBlockName;
            PFNGLUNIFORMBLOCKBINDINGPROC UniformBlockBinding;
        };
        struct
        {
            // 320
            PFNGLDRAWELEMENTSBASEVERTEXPROC DrawElementsBaseVertex;
            PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC DrawRangeElementsBaseVertex;
            PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC DrawElementsInstancedBaseVertex;
            PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC MultidrawElementsBaseVertex;
            PFNGLPROVOKINGVERTEXPROC ProvokingVertex;
            PFNGLFENCESYNCPROC FenceSync;
            PFNGLISSYNCPROC IsSync;
            PFNGLDELETESYNCPROC DeleteSync;
            PFNGLCLIENTWAITSYNCPROC ClientWaitSync;
            PFNGLWAITSYNCPROC WaitSync;
            PFNGLGETINTEGER64VPROC GetInteger64v;
            PFNGLGETSYNCIVPROC GetSynciv;
            PFNGLGETINTEGER64I_VPROC GetInteger64I_v;
            PFNGLGETBUFFERPARAMETERI64VPROC GetBufferParameterI64v;
            PFNGLFRAMEBUFFERTEXTUREPROC FramebufferTexture;
            PFNGLTEXIMAGE2DMULTISAMPLEPROC TexImage2DMultisample;
            PFNGLTEXIMAGE3DMULTISAMPLEPROC TexImage3DMultisample;
            PFNGLGETMULTISAMPLEFVPROC GetMultisamplefv;
            PFNGLSAMPLEMASKIPROC SampleMaski;
        };
        struct
        {
            // 330
            PFNGLBINDFRAGDATALOCATIONINDEXEDPROC BindFragDataLocationIndexed;
            PFNGLGETFRAGDATAINDEXPROC GetFragDataIndex;
            PFNGLGENSAMPLERSPROC GenSamplers;
            PFNGLDELETESAMPLERSPROC DeleteSamplers;
            PFNGLISSAMPLERPROC IsSampler;
            PFNGLBINDSAMPLERPROC BindSampler;
            PFNGLSAMPLERPARAMETERIPROC SamplerParameteri;
            PFNGLSAMPLERPARAMETERIVPROC SamplerParameteriv;
            PFNGLSAMPLERPARAMETERFPROC SamplerParameterf;
            PFNGLSAMPLERPARAMETERFVPROC SamplerParameterfv;
            PFNGLSAMPLERPARAMETERIIVPROC SamplerParameterIiv;
            PFNGLSAMPLERPARAMETERIUIVPROC SamplerParameterIuiv;
            PFNGLGETSAMPLERPARAMETERIVPROC GetSamplerParamteriv;
            PFNGLGETSAMPLERPARAMETERIIVPROC GetSamplerParamterIiv;
            PFNGLGETSAMPLERPARAMETERFVPROC GetSamplerParamterfv;
            PFNGLGETSAMPLERPARAMETERIUIVPROC GetSamplerParamterIuiv;
            PFNGLQUERYCOUNTERPROC QueryCounter;
            PFNGLGETQUERYOBJECTI64VPROC GetQueryObjecti64v;
            PFNGLGETQUERYOBJECTUI64VPROC GetQueryObjectui64v;
            PFNGLVERTEXATTRIBDIVISORPROC VertexAttribDivisor;
            PFNGLVERTEXATTRIBP1UIPROC VertexAttribP1ui;
            PFNGLVERTEXATTRIBP1UIVPROC VertexAttribP1uiv;
            PFNGLVERTEXATTRIBP2UIPROC VertexAttribP2ui;
            PFNGLVERTEXATTRIBP2UIVPROC VertexAttribP2uiv;
            PFNGLVERTEXATTRIBP3UIPROC VertexAttribP3ui;
            PFNGLVERTEXATTRIBP3UIVPROC VertexAttribP3uiv;
            PFNGLVERTEXATTRIBP4UIPROC VertexAttribP4ui;
            PFNGLVERTEXATTRIBP4UIVPROC VertexAttribP4uiv;
        };
        struct
        {
            // 400
            PFNGLMINSAMPLESHADINGPROC MinSampleShading;
            PFNGLBLENDEQUATIONIPROC BlendEquationi;
            PFNGLBLENDEQUATIONSEPARATEIPROC BlendEquationSeparatei;
            PFNGLBLENDFUNCIPROC BlendFunci;
            PFNGLBLENDFUNCSEPARATEIPROC BlendFuncSeparatei;
            PFNGLDRAWARRAYSINDIRECTPROC DrawArraysIndirect;
            PFNGLDRAWELEMENTSINDIRECTPROC DrawElementsIndirect;
            PFNGLUNIFORM1DPROC Uniform1d;
            PFNGLUNIFORM2DPROC Uniform2d;
            PFNGLUNIFORM3DPROC Uniform3d;
            PFNGLUNIFORM4DPROC Uniform4d;
            PFNGLUNIFORM1DVPROC Uniform1dv;
            PFNGLUNIFORM2DVPROC Uniform2dv;
            PFNGLUNIFORM3DVPROC Uniform3dv;
            PFNGLUNIFORM4DVPROC Uniform4dv;
            PFNGLUNIFORMMATRIX2DVPROC UniformMatrix2dv;
            PFNGLUNIFORMMATRIX3DVPROC UniformMatrix3dv;
            PFNGLUNIFORMMATRIX4DVPROC UniformMatrix4dv;
            PFNGLUNIFORMMATRIX2X3DVPROC UniformMatrix2x3dv;
            PFNGLUNIFORMMATRIX2X4DVPROC UniformMatrix2x4dv;
            PFNGLUNIFORMMATRIX3X2DVPROC UniformMatrix3x2dv;
            PFNGLUNIFORMMATRIX3X4DVPROC UniformMatrix3x4dv;
            PFNGLUNIFORMMATRIX4X2DVPROC UniformMatrix4x2dv;
            PFNGLUNIFORMMATRIX4X3DVPROC UniformMatrix4x3dv;
            PFNGLGETUNIFORMDVPROC GetUniformdv;
            PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC GetSubroutineUniformLocation;
            PFNGLGETSUBROUTINEINDEXPROC GetSubroutineIndex;
            PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC GetActiveSubroutineUniformiv;
            PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC GetActiveSubroutineUniformName;
            PFNGLGETACTIVESUBROUTINENAMEPROC GetActiveSubroutineName;
            PFNGLUNIFORMSUBROUTINESUIVPROC UniformSubroutinesuiv;
            PFNGLGETUNIFORMSUBROUTINEUIVPROC GetUniformSubroutineuiv;
            PFNGLGETPROGRAMSTAGEIVPROC GetProgramStage;
            PFNGLPATCHPARAMETERIPROC PatchParameterI;
            PFNGLPATCHPARAMETERFVPROC PatchParameterfv;
            PFNGLBINDTRANSFORMFEEDBACKPROC BindTransformFeedback;
            PFNGLDELETETRANSFORMFEEDBACKSPROC DeleteTransformFeedbacks;
            PFNGLGENTRANSFORMFEEDBACKSPROC GenTransformFeedbacks;
            PFNGLISTRANSFORMFEEDBACKPROC IsTransformFeedback;
            PFNGLPAUSETRANSFORMFEEDBACKPROC PauseTransformFeedback;
            PFNGLRESUMETRANSFORMFEEDBACKPROC ResumeTransformFeedback;
            PFNGLDRAWTRANSFORMFEEDBACKPROC DrawTransformFeedback;
            PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC DrawTransformFeedbackStream;
            PFNGLBEGINQUERYINDEXEDPROC BeginQueryIndexed;
            PFNGLENDQUERYINDEXEDPROC EndQueryIndexed;
            PFNGLGETQUERYINDEXEDIVPROC GetQueryIndexediv;
        };
        struct
        {
            // 410
            PFNGLRELEASESHADERCOMPILERPROC ReleaseShaderCompiler;
            PFNGLSHADERBINARYPROC ShaderBinary;
            PFNGLGETSHADERPRECISIONFORMATPROC GetShaderPrecisionFormat;
            PFNGLDEPTHRANGEFPROC DepthRangef;
            PFNGLCLEARDEPTHFPROC ClearDepthf;
            PFNGLGETPROGRAMBINARYPROC GetProgramBinary;
            PFNGLPROGRAMBINARYPROC ProgramBinary;
            PFNGLPROGRAMPARAMETERIPROC ProgramParameteri;
            PFNGLUSEPROGRAMSTAGESPROC UseProgramStages;
            PFNGLACTIVESHADERPROGRAMPROC ActiveShaderProgram;
            PFNGLCREATESHADERPROGRAMVPROC CreateShaderProgramv;
            PFNGLBINDPROGRAMPIPELINEPROC BindProgramPipeline;
            PFNGLDELETEPROGRAMPIPELINESPROC DeleteProgramPipelines;
            PFNGLGENPROGRAMPIPELINESPROC GenProgramPipelines;
            PFNGLISPROGRAMPIPELINEPROC IsProgramPipeline;
            PFNGLGETPROGRAMPIPELINEIVPROC GetProgramPipelineiv;
            PFNGLPROGRAMUNIFORM1IPROC ProgramUniform1i;
            PFNGLPROGRAMUNIFORM1IVPROC ProgramUniform1iv;
            PFNGLPROGRAMUNIFORM1FPROC ProgramUniform1f;
            PFNGLPROGRAMUNIFORM1FVPROC ProgramUniform1fv;
            PFNGLPROGRAMUNIFORM1DPROC ProgramUniform1d;
            PFNGLPROGRAMUNIFORM1DVPROC ProgramUniform1dv;
            PFNGLPROGRAMUNIFORM1UIPROC ProgramUniform1ui;
            PFNGLPROGRAMUNIFORM1UIVPROC ProgramUniform1uiv;
            PFNGLPROGRAMUNIFORM2IPROC ProgramUniform2i;
            PFNGLPROGRAMUNIFORM2IVPROC ProgramUniform2iv;
            PFNGLPROGRAMUNIFORM2FPROC ProgramUniform2f;
            PFNGLPROGRAMUNIFORM2FVPROC ProgramUniform2fv;
            PFNGLPROGRAMUNIFORM2DPROC ProgramUniform2d;
            PFNGLPROGRAMUNIFORM2DVPROC ProgramUniform2dv;
            PFNGLPROGRAMUNIFORM2UIPROC ProgramUniform2ui;
            PFNGLPROGRAMUNIFORM2UIVPROC ProgramUniform2uiv;
            PFNGLPROGRAMUNIFORM3IPROC ProgramUniform3i;
            PFNGLPROGRAMUNIFORM3IVPROC ProgramUniform3iv;
            PFNGLPROGRAMUNIFORM3FPROC ProgramUniform3f;
            PFNGLPROGRAMUNIFORM3FVPROC ProgramUniform3fv;
            PFNGLPROGRAMUNIFORM3DPROC ProgramUniform3d;
            PFNGLPROGRAMUNIFORM3DVPROC ProgramUniform3dv;
            PFNGLPROGRAMUNIFORM3UIPROC ProgramUniform3ui;
            PFNGLPROGRAMUNIFORM3UIVPROC ProgramUniform3uiv;
            PFNGLPROGRAMUNIFORM4IPROC ProgramUniform4i;
            PFNGLPROGRAMUNIFORM4IVPROC ProgramUniform4iv;
            PFNGLPROGRAMUNIFORM4FPROC ProgramUniform4f;
            PFNGLPROGRAMUNIFORM4FVPROC ProgramUniform4fv;
            PFNGLPROGRAMUNIFORM4DPROC ProgramUniform4d;
            PFNGLPROGRAMUNIFORM4DVPROC ProgramUniform4dv;
            PFNGLPROGRAMUNIFORM4UIPROC ProgramUniform4ui;
            PFNGLPROGRAMUNIFORM4UIVPROC ProgramUniform4uiv;
            PFNGLPROGRAMUNIFORMMATRIX2FVPROC ProgramUniformMatrix2fv;
            PFNGLPROGRAMUNIFORMMATRIX3FVPROC ProgramUniformMatrix3fv;
            PFNGLPROGRAMUNIFORMMATRIX4FVPROC ProgramUniformMatrix4fv;
            PFNGLPROGRAMUNIFORMMATRIX2DVPROC ProgramUniformMatrix2dv;
            PFNGLPROGRAMUNIFORMMATRIX3DVPROC ProgramUniformMatrix3dv;
            PFNGLPROGRAMUNIFORMMATRIX4DVPROC ProgramUniformMatrix4dv;
            PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC ProgramUniformMatrix2x3fv;
            PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC ProgramUniformMatrix3x2fv;
            PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC ProgramUniformMatrix2x4fv;
            PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC ProgramUniformMatrix4x2fv;
            PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC ProgramUniformMatrix3x4fv;
            PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC ProgramUniformMatrix4x3fv;
            PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC ProgramUniformMatrix2x3dv;
            PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC ProgramUniformMatrix3x2dv;
            PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC ProgramUniformMatrix2x4dv;
            PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC ProgramUniformMatrix4x2dv;
            PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC ProgramUniformMatrix3x4dv;
            PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC ProgramUniformMatrix4x3dv;
            PFNGLVALIDATEPROGRAMPIPELINEPROC ValidateProgramPipeline;
            PFNGLGETPROGRAMPIPELINEINFOLOGPROC GetProgramPipelineInfoLog;
            PFNGLVERTEXATTRIBL1DPROC VertexAttribL1d;
            PFNGLVERTEXATTRIBL2DPROC VertexAttribL2d;
            PFNGLVERTEXATTRIBL3DPROC VertexAttribL3d;
            PFNGLVERTEXATTRIBL4DPROC VertexAttribL4d;
            PFNGLVERTEXATTRIBL1DVPROC VertexAttribL1dv;
            PFNGLVERTEXATTRIBL2DVPROC VertexAttribL2dv;
            PFNGLVERTEXATTRIBL3DVPROC VertexAttribL3dv;
            PFNGLVERTEXATTRIBL4DVPROC VertexAttribL4dv;
            PFNGLVERTEXATTRIBLPOINTERPROC VertexAttribLPointer;
            PFNGLGETVERTEXATTRIBLDVPROC GetVertexAttribLdv;
            PFNGLVIEWPORTARRAYVPROC ViewportArrayv;
            PFNGLVIEWPORTINDEXEDFPROC ViewportIndexedf;
            PFNGLVIEWPORTINDEXEDFVPROC ViewportIndexedfv;
            PFNGLSCISSORARRAYVPROC ScissorArrayv;
            PFNGLSCISSORINDEXEDPROC ScissorIndexed;
            PFNGLSCISSORINDEXEDVPROC ScissorIndexedv;
            PFNGLDEPTHRANGEARRAYVPROC DepthRangeArrayv;
            PFNGLDEPTHRANGEINDEXEDPROC DepthRangeIndexed;
            PFNGLGETFLOATI_VPROC GetFloati_v;
            PFNGLGETDOUBLEI_VPROC GetDoublei_v;
        };
        struct
        {
            // 420
            PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC DrawArraysInstancedBaseInstance;
            PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC DrawElementsInstancedBaseInstance;
            PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC DrawElementsInstancedBaseVertexBaseInstance;
            PFNGLGETINTERNALFORMATIVPROC GetInternalformativ;
            PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC GetActiveAtomicCounterBufferiv;
            PFNGLBINDIMAGETEXTUREPROC BindImageTexture;
            PFNGLMEMORYBARRIERPROC MemoryBarrier;
            PFNGLTEXSTORAGE1DPROC TexStorage1D;
            PFNGLTEXSTORAGE2DPROC TexStorage2D;
            PFNGLTEXSTORAGE3DPROC TexStorage3D;
            PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC DrawTransformFeedbackInstanced;
            PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC DrawTransformFeedbackStreamInstanced;
        };
        struct
        {
            // 430
            PFNGLCLEARBUFFERDATAPROC ClearBufferData;
            PFNGLCLEARBUFFERSUBDATAPROC ClearBufferSubData;
            PFNGLDISPATCHCOMPUTEPROC DispatchCompute;
            PFNGLDISPATCHCOMPUTEINDIRECTPROC DispatchComputeIndirect;
            PFNGLCOPYIMAGESUBDATAPROC CopyImageSubData;
            PFNGLFRAMEBUFFERPARAMETERIPROC FramebufferParameteri;
            PFNGLGETFRAMEBUFFERPARAMETERIVPROC GetFramebufferParameteriv;
            PFNGLGETINTERNALFORMATI64VPROC GetInternalformati64v;
            PFNGLINVALIDATETEXSUBIMAGEPROC InvalidateTexSubImage;
            PFNGLINVALIDATETEXIMAGEPROC InvalidateTexImage;
            PFNGLINVALIDATEBUFFERSUBDATAPROC InvalidateBufferSubData;
            PFNGLINVALIDATEBUFFERDATAPROC InvalidateBufferData;
            PFNGLINVALIDATEFRAMEBUFFERPROC InvalidateFramebuffer;
            PFNGLINVALIDATESUBFRAMEBUFFERPROC InvalidateSubFramebuffer;
            PFNGLMULTIDRAWARRAYSINDIRECTPROC MultiDrawArraysIndirect;
            PFNGLMULTIDRAWELEMENTSINDIRECTPROC MultiDrawElementsIndirect;
            PFNGLGETPROGRAMINTERFACEIVPROC GetProgramInterfaceiv;
            PFNGLGETPROGRAMRESOURCEINDEXPROC GetProgramResourceIndex;
            PFNGLGETPROGRAMRESOURCENAMEPROC GetProgramResourceName;
            PFNGLGETPROGRAMRESOURCEIVPROC GetProgramResourceiv;
            PFNGLGETPROGRAMRESOURCELOCATIONPROC GetProgramResourceLocation;
            PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC GetProgramResourceLocationIndex;
            PFNGLSHADERSTORAGEBLOCKBINDINGPROC ShaderStorageBlockBinding;
            PFNGLTEXBUFFERRANGEPROC TexBufferRange;
            PFNGLTEXSTORAGE2DMULTISAMPLEPROC TexStorage2DMultisample;
            PFNGLTEXSTORAGE3DMULTISAMPLEPROC TexStorage3DMultisample;
            PFNGLTEXTUREVIEWPROC TextureView;
            PFNGLBINDVERTEXBUFFERPROC BindVertexBuffer;
            PFNGLVERTEXATTRIBFORMATPROC VertexAttribFormat;
            PFNGLVERTEXATTRIBIFORMATPROC VertexAttribIFormat;
            PFNGLVERTEXATTRIBLFORMATPROC VertexAttribLFormat;
            PFNGLVERTEXATTRIBBINDINGPROC VertexAttribBinding;
            PFNGLVERTEXBINDINGDIVISORPROC VertexBindingDivisor;
            PFNGLDEBUGMESSAGECONTROLPROC DebugMessageControl;
            PFNGLDEBUGMESSAGEINSERTPROC DebugMessageInsert;
            PFNGLDEBUGMESSAGECALLBACKPROC DebugMessageCallback;
            PFNGLGETDEBUGMESSAGELOGPROC GetDebugMessageLog;
            PFNGLPUSHDEBUGGROUPPROC PushDebugGroup;
            PFNGLPOPDEBUGGROUPPROC PopDebugGroup;
            PFNGLOBJECTLABELPROC ObjectLabel;
            PFNGLGETOBJECTLABELPROC GetObjectLabel;
            PFNGLOBJECTPTRLABELPROC ObjectPtrLabel;
            PFNGLGETOBJECTPTRLABELPROC GetObjectPtrLabel;
        };
        struct
        {
            // 440
            PFNGLBUFFERSTORAGEPROC BufferStorage;
            PFNGLCLEARTEXIMAGEPROC ClearTexImage;
            PFNGLCLEARTEXSUBIMAGEPROC ClearTexSubImage;
            PFNGLBINDBUFFERSBASEPROC BindBuffersBase;
            PFNGLBINDBUFFERSRANGEPROC BindBuffersRange;
            PFNGLBINDTEXTURESPROC BindTextures;
            PFNGLBINDSAMPLERSPROC BindSamplers;
            PFNGLBINDIMAGETEXTURESPROC BindImageTextures;
            PFNGLBINDVERTEXBUFFERSPROC BindVertexBuffers;
        };
        struct
        {
            // 450
            PFNGLCLIPCONTROLPROC ClipControl;
            PFNGLCREATETRANSFORMFEEDBACKSPROC CreateTransformFeedbacks;
            PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC TransformFeedbackBufferBase;
            PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC TransformFeedbackBufferRange;
            PFNGLGETTRANSFORMFEEDBACKIVPROC GetTransformFeedbackiv;
            PFNGLGETTRANSFORMFEEDBACKI_VPROC GetTransformFeedbacki_v;
            PFNGLGETTRANSFORMFEEDBACKI64_VPROC GetTransformFeedbacki64_v;
            PFNGLCREATEBUFFERSPROC CreateBuffers;
            PFNGLNAMEDBUFFERSTORAGEPROC NamedBufferStorage;
            PFNGLNAMEDBUFFERDATAPROC NamedBufferData;
            PFNGLNAMEDBUFFERSUBDATAPROC NamedBufferSubData;
            PFNGLCOPYNAMEDBUFFERSUBDATAPROC CopyNamedBufferSubData;
            PFNGLCLEARNAMEDBUFFERDATAPROC ClearNamedBufferData;
            PFNGLCLEARNAMEDBUFFERSUBDATAPROC ClearNamedBufferSubData;
            PFNGLMAPNAMEDBUFFERPROC MapNamedBuffer;
            PFNGLMAPNAMEDBUFFERRANGEPROC MapNamedBufferRange;
            PFNGLUNMAPNAMEDBUFFERPROC UnmapNamedBuffer;
            PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC FlushMappedNamedBufferRange;
            PFNGLGETNAMEDBUFFERPARAMETERIVPROC GetNamedBufferParameteriv;
            PFNGLGETNAMEDBUFFERPARAMETERI64VPROC GetNamedBufferParameteri64v;
            PFNGLGETNAMEDBUFFERPOINTERVPROC GetNamedBufferPointerv;
            PFNGLGETNAMEDBUFFERSUBDATAPROC GetNamedBufferSubData;
            PFNGLCREATEFRAMEBUFFERSPROC CreateFramebuffers;
            PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC NamedFramebufferRenderbuffer;
            PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC NamedFramebufferParameteri;
            PFNGLNAMEDFRAMEBUFFERTEXTUREPROC NamedFramebufferTexture;
            PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC NamedFramebufferTextureLayer;
            PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC NamedFramebufferDrawBuffer;
            PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC NamedFramebufferDrawBuffers;
            PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC NamedFramebufferReadBuffer;
            PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC InvalidateNamedFramebufferData;
            PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC InvalidateNamedFramebufferSubData;
            PFNGLCLEARNAMEDFRAMEBUFFERIVPROC ClearNamedFramebufferiv;
            PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC ClearNamedFramebufferuiv;
            PFNGLCLEARNAMEDFRAMEBUFFERFVPROC ClearNamedFramebufferfv;
            PFNGLCLEARNAMEDFRAMEBUFFERFIPROC ClearNamedFramebufferfi;
            PFNGLBLITNAMEDFRAMEBUFFERPROC BlitNamedFramebuffer;
            PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC CheckNamedFramebufferStatus;
            PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC GetNamedFramebufferParameteriv;
            PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC GetNamedFramebufferAttachmentParameteriv;
            PFNGLCREATERENDERBUFFERSPROC CreateRenderbuffers;
            PFNGLNAMEDRENDERBUFFERSTORAGEPROC NamedRenderbufferStorage;
            PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC NamedRenderbufferStorageMultisample;
            PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC GetNamedRenderbufferParameteriv;
            PFNGLCREATETEXTURESPROC CreateTextures;
            PFNGLTEXTUREBUFFERPROC TextureBuffer;
            PFNGLTEXTUREBUFFERRANGEPROC TextureBufferRange;
            PFNGLTEXTURESTORAGE1DPROC TextureStorage1D;
            PFNGLTEXTURESTORAGE2DPROC TextureStorage2D;
            PFNGLTEXTURESTORAGE3DPROC TextureStorage3D;
            PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC TextureStorage2DMultisample;
            PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC TextureStorage3DMultisample;
            PFNGLTEXTURESUBIMAGE1DPROC TextureSubImage1D;
            PFNGLTEXTURESUBIMAGE2DPROC TextureSubImage2D;
            PFNGLTEXTURESUBIMAGE3DPROC TextureSubImage3D;
            PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC CompressedTextureSubImage1D;
            PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC CompressedTextureSubImage2D;
            PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC CompressedTextureSubImage3D;
            PFNGLCOPYTEXTURESUBIMAGE1DPROC CopyTextureSubImage1D;
            PFNGLCOPYTEXTURESUBIMAGE2DPROC CopyTextureSubImage2D;
            PFNGLCOPYTEXTURESUBIMAGE3DPROC CopyTextureSubImage3D;
            PFNGLTEXTUREPARAMETERFPROC TextureParameterf;
            PFNGLTEXTUREPARAMETERFVPROC TextureParameterfv;
            PFNGLTEXTUREPARAMETERIPROC TextureParameteri;
            PFNGLTEXTUREPARAMETERIIVPROC TextureParameterIiv;
            PFNGLTEXTUREPARAMETERIUIVPROC TextureParameterIuiv;
            PFNGLTEXTUREPARAMETERIVPROC TextureParameteriv;
            PFNGLGENERATETEXTUREMIPMAPPROC GenerateTextureMipmap;
            PFNGLBINDTEXTUREUNITPROC BindTextureUnit;
            PFNGLGETTEXTUREIMAGEPROC GetTextureImage;
            PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC GetCompressedTextureImage;
            PFNGLGETTEXTURELEVELPARAMETERFVPROC GetTextureLevelParameterfv;
            PFNGLGETTEXTURELEVELPARAMETERIVPROC GetTextureLevelParameteriv;
            PFNGLGETTEXTUREPARAMETERFVPROC GetTextureParameterfv;
            PFNGLGETTEXTUREPARAMETERIIVPROC GetTextureParameterIiv;
            PFNGLGETTEXTUREPARAMETERIUIVPROC GetTextureParameterIuiv;
            PFNGLGETTEXTUREPARAMETERIVPROC GetTextureParameteriv;
            PFNGLCREATEVERTEXARRAYSPROC CreateVertexArrays;
            PFNGLDISABLEVERTEXARRAYATTRIBPROC DisableVertexArrayAttrib;
            PFNGLENABLEVERTEXARRAYATTRIBPROC EnableVertexArrayAttrib;
            PFNGLVERTEXARRAYELEMENTBUFFERPROC VertexArrayElementBuffer;
            PFNGLVERTEXARRAYVERTEXBUFFERPROC VertexArrayVertexBuffer;
            PFNGLVERTEXARRAYVERTEXBUFFERSPROC VertexArrayVertexBuffers;
            PFNGLVERTEXARRAYATTRIBBINDINGPROC VertexArrayAttribBinding;
            PFNGLVERTEXARRAYATTRIBFORMATPROC VertexArrayAttribFormat;
            PFNGLVERTEXARRAYATTRIBIFORMATPROC VertexArrayAttribIFormat;
            PFNGLVERTEXARRAYATTRIBLFORMATPROC VertexArrayAttribLFormat;
            PFNGLVERTEXARRAYBINDINGDIVISORPROC VertexArrayBindingDivisor;
            PFNGLGETVERTEXARRAYIVPROC GetVertexArrayiv;
            PFNGLGETVERTEXARRAYINDEXEDIVPROC GetVertexArrayIndexediv;
            PFNGLGETVERTEXARRAYINDEXED64IVPROC GetVertexArrayIndexed64iv;
            PFNGLCREATESAMPLERSPROC CreateSamplers;
            PFNGLCREATEPROGRAMPIPELINESPROC CreateProgramPipelines;
            PFNGLCREATEQUERIESPROC CreateQueries;
            PFNGLGETQUERYBUFFEROBJECTI64VPROC GetQueryBufferObjecti64v;
            PFNGLGETQUERYBUFFEROBJECTIVPROC GetQueryBufferObjectiv;
            PFNGLGETQUERYBUFFEROBJECTUI64VPROC GetQueryBufferObjectui64v;
            PFNGLGETQUERYBUFFEROBJECTUIVPROC GetQueryBufferObjectuiv;
            PFNGLMEMORYBARRIERBYREGIONPROC MemoryBarrierByRegion;
            PFNGLGETTEXTURESUBIMAGEPROC GetTextureSubImage;
            PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC GetCompressedTextureSubImage;
            PFNGLGETGRAPHICSRESETSTATUSPROC GetGraphicsResetStatus;
            PFNGLGETNCOMPRESSEDTEXIMAGEPROC GetnCompressedTexImage;
            PFNGLGETNTEXIMAGEPROC GetnTexImage;
            PFNGLGETNUNIFORMDVPROC GetnUniformdv;
            PFNGLGETNUNIFORMFVPROC GetnUniformfv;
            PFNGLGETNUNIFORMIVPROC GetnUniformiv;
            PFNGLGETNUNIFORMUIVPROC GetnUniformuiv;
            PFNGLREADNPIXELSPROC ReadnPixels;
            PFNGLTEXTUREBARRIERPROC TextureBarrier;
        };
        struct
        {
            // 460
            PFNGLSPECIALIZESHADERPROC SpecializeShader;
            PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC MultiDrawArraysIndirectCount;
            PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC MultiDrawElementsIndirectCount;
            PFNGLPOLYGONOFFSETCLAMPPROC PolygonOffsetClamp;
        };
    };
    void* ptr;
} glVmt;

SGL GLenum AfxToGlFrontFace(afxFrontFace ff);
SGL GLenum AfxToGlCullMode(afxCullMode cm);
SGL GLenum AfxToGlFillMode(afxFillMode pm);
SGL GLenum SglToGlCompareOp(afxCompareOp co);
SGL GLuint AfxToGlShaderStage(afxShaderStage s);
SGL GLuint AfxToGlShaderStageBit(afxShaderStage s);
SGL GLenum AfxToGlTopology(afxPrimTopology pm);
SGL GLenum AfxToGlBufferUsage(afxInt bt);
SGL GLenum AfxToGlStencilOp(afxStencilOp so);
SGL GLenum AfxToGlBlendOp(afxBlendOp f);
SGL GLenum AfxToGlBlendFactor(afxBlendFactor f);
SGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxNat32* stride);
SGL GLenum AfxToGlLogicOp(afxLogicOp f);
SGL void SglToGlFormat(afxPixelFormat pf, GLint *fmt, GLenum *layout, GLenum *type);
SGL GLenum SglToGlImageDimensions(afxNat d);
SGL GLenum SglToGlTexelFilterMode(afxTexelFilter tfm);
SGL GLenum SglToGlTexelFilterModeMipmapped(afxTexelFilter min, afxTexelFilter mip);
SGL GLenum SglToGlTexWrapMode(afxTexelAddress twm);
SGL GLenum SglToGlColorSwizzle(afxColorSwizzle swizzle);
SGL void SglToGlColorSwizzling(afxColorSwizzling const *swizzling, GLenum *arrayedSwizzles);
SGL void SglDetermineGlTargetInternalFormatType(afxRaster tex, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type);

#ifdef _AFX_DEBUG
#   define _SglThrowErrorOccuried() { GLenum err = gl->GetError(); switch (err) { case GL_NO_ERROR: break; case 1280: AfxLogError("GL: INVAL_PARAM"); break; case 1281: AfxLogError("GL: INVAL_VALUE"); break; case 1282: AfxLogError("GL: INVAL_OP"); break; case 1283: AfxLogError("GL: STACK_OVERFLOW"); break; case 1284: AfxLogError("GL: STACK_UNDERFLOW"); break; case 1285: AfxLogError("GL: OUT_OF_MEM"); break; default: AfxLogError("GL: %d", err); break; }}
#else
#   define _SglThrowErrorOccuried()
#endif//AFX_DONT_DEBUG



#endif//AFX_STD_DRAW_DRIVER_DEFS_H