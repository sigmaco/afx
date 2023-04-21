#include "sgl.h"

#include "../e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/pipelining/afxPipelineModule.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/io/afxUri.h"

typedef struct
{
    afxDrawContext dctx;
    afxByte const *code;
    afxNat len;
} _afxShdrCtorArgs;

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDqueSyncPipm(afxDrawQueue dque, afxPipelineModule pipm, afxShaderStage stage)
{
    //AfxEntry("pipm=%p", pipm);
    afxError err = NIL;
    AfxAssertObject(pipm, AFX_FCC_PIPM);

    sglVmt const* gl = &dque->wglVmt;

    if ((pipm->updFlags & SGL_UPD_FLAG_DEVICE))
    {
        if ((pipm->updFlags & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (pipm->glHandle)
            {
                gl->DeleteShader(pipm->glHandle); _SglThrowErrorOccuried();
                pipm->glHandle = NIL;
            }

            AfxAssert(NIL == pipm->glHandle);

            if (!(pipm->glHandle = gl->CreateShader(AfxToGlShaderStage(stage))))
            {
                AfxThrowError();
                _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(pipm);
                AfxAssert(gl->IsShader(pipm->glHandle));
                const GLint codeLens[] = { pipm->len };
                GLchar const* const codes[] = { (GLchar const*)pipm->code };
                gl->ShaderSource(pipm->glHandle, 1, codes, codeLens); _SglThrowErrorOccuried();
                gl->CompileShader(pipm->glHandle); _SglThrowErrorOccuried();
                GLint status = 0;
                gl->GetShaderiv(pipm->glHandle, GL_COMPILE_STATUS, &status); _SglThrowErrorOccuried();

                if (status == GL_FALSE)
                {
                    AfxThrowError();
                    afxChar str[1024];
                    gl->GetShaderInfoLog(pipm->glHandle, sizeof(str), NIL, (GLchar*)str); _SglThrowErrorOccuried();
                    AfxError(str);
                    gl->DeleteShader(pipm->glHandle); _SglThrowErrorOccuried();
                    pipm->glHandle = NIL;
                }
                else
                {
                    pipm->compiled = TRUE;
                    pipm->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    AfxEcho("Shader %p reinstanced.", pipm);
                }
            }
        }
        else if ((pipm->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(pipm->glHandle);
            
            AfxThrowError(); // can't be modified
        }
    }
    return err;
}

_SGL afxError _AfxPipelineModuleDownload(afxPipelineModule pipm, afxUri const *uri)
{
    afxError err = NIL;
    AfxAssertObject(pipm, AFX_FCC_PIPM);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!(AfxUriIsBlank(uri)));

    //afxDrawSystem dsys = AfxGetShaderHub(pipm);
    afxStream ios;

    if (!(ios = AfxSystemAcquireStream(AfxSystemGet(), uri, "w"))) AfxThrowError();
    else
    {
        if (AfxPipelineModuleSerialize(pipm, ios)) AfxThrowError();
        else
        {
            // success
        }
        AfxObjectRelease(&ios->obj);
    }
    return err;
}

_SGL afxError _AfxPipelineModuleSerialize(afxPipelineModule pipm, afxStream ios)
{
    afxError err = NIL;
    AfxAssertObject(pipm, AFX_FCC_PIPM);
    AfxAssertObject(ios, AFX_FCC_IOS);

    AfxStreamWrite(ios, 1, &pipm->len, (void const*[]) { pipm->code });

    return err;
}

_SGL afxError _AfxPipmDtor(afxPipelineModule pipm)
{
    afxError err = NIL;
    AfxAssertObject(pipm, AFX_FCC_PIPM);

    if (pipm->glHandle)
    {
        afxDrawContext dctx = AfxPipelineModuleGetContext(pipm);
        _SglEnqueueGlResourceDeletion(dctx, 0, 6, pipm->glHandle);
        pipm->glHandle = 0;
    }

    if (pipm->code)
        AfxDeallocate(NIL, pipm->code);

    return err;
}

_SGL afxPipmImpl const _AfxStdPipmImpl;

afxPipmImpl const _AfxStdPipmImpl =
{
    _AfxPipelineModuleDownload,
    _AfxPipelineModuleSerialize
};

_SGL afxError _AfxPipmCtor(afxPipelineModule pipm, _afxShdrCtorArgs *args)
{
    AfxEntry("pipm=%p", pipm);
    afxError err = NIL;

    AfxAssertObject(args->dctx, AFX_FCC_DCTX);

    if (!args->code || !args->code[0]) AfxThrowError();
    else
    {
        if (2 > args->len) AfxThrowError();
        else
        {
            void* codecpy;

            if (!(codecpy = AfxAllocate(NIL, args->len + sizeof(afxByte), AfxSpawnHint()))) AfxThrowError();
            else
            {
                AfxCopy(codecpy, args->code, args->len);
                ((afxByte*)codecpy)[args->len + sizeof(afxByte)] = '\0';

                pipm->code = codecpy;
                pipm->len = args->len;

                pipm->glHandle = NIL;
                pipm->updFlags = SGL_UPD_FLAG_DEVICE_INST;
                pipm->compiled = FALSE;

                if(err)
                    AfxDeallocate(NIL, codecpy);
            }
        }
    }
    return err;
}

_SGL afxPipelineModule _AfxDrawContextBuildPipelineModule(afxDrawContext dctx, afxByte const code[], afxNat len)
{
    afxError err = NIL;
    AfxEntry("dctx=%p,code=%p,len=%u", dctx, code, len);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxPipelineModule pipm = NIL;

    _afxShdrCtorArgs args =
    {
        dctx,
        code,
        len
    };

    AfxAssert(code);
    AfxAssert(len);

    if (!(pipm = AfxObjectAcquire(AfxDrawContextGetPipelineModuleClass(dctx), &args, AfxSpawnHint())))
        AfxThrowError();

    return pipm;
}

_SGL afxPipelineModule _AfxDrawContextUploadPipelineModuleGlsl(afxDrawContext dctx, afxUri const *uri)
{
    afxError err = NIL;
    AfxEntry("dctx=%p", dctx);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxPipelineModule pipm = NIL;
    afxStream ios;

    if (!(ios = AfxSystemAcquireStream(AfxSystemGet(), uri, "r"))) AfxThrowError();
    else
    {
        AfxAssertObject(ios, AFX_FCC_IOS);
        afxNat size;

        if (!(size = AfxStreamMeasure(ios))) AfxThrowError();
        else
        {
            void* code;

            if (!(code = AfxAllocate(NIL, size + sizeof(afxByte), AfxSpawnHint()))) AfxThrowError();
            else
            {
                AfxStreamRead(ios, 1, &size, &code);
                ((afxByte*)code)[size] = '\0';

                if (!(pipm = AfxDrawContextBuildPipelineModule(dctx, code, size)))
                    AfxThrowError();

                AfxDeallocate(NIL, code);
            }
        }
        AfxObjectRelease(&ios->obj);
    }
    return pipm;
}

_SGL afxPipelineModule _AfxDrawContextUploadPipelineModule(afxDrawContext dctx, afxUri const *uri)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertType(uri, AFX_FCC_URI);
    AfxAssert(!AfxUriIsBlank(uri));

    afxUri fext;
    AfxUriForkExtension(&fext, uri);

    if (AfxUriIsBlank(uri)) AfxThrowError();
    else
    {
        afxPipelineModule pipm = NIL;

        if (0 == AfxStringCompareRawCi(AfxUriGetStringConst(&fext), 0, 4, ".urd"))
        {
            AfxAdvertise("Uniform Resource Dictionary (.urd) is NOT supported yet.");
            AfxThrowError();
        }
        else
        {
            if (!(pipm = _AfxDrawContextUploadPipelineModuleGlsl(dctx, uri))) AfxThrowError();
            else
            {
                return pipm;
            }
        }
    }
    return NIL;
}

_SGL void _AfxIteratorCompareResourceNameCiPipm(afxIterator *iter)
{
    struct { afxUri const *name; afxObject *obj; } *data = iter->udd;
    afxResource res = iter->voidItem;

    afxUri name;
    AfxResourceExcerptName(res, &name);

    if (!AfxUriIsBlank(&name))
    {
        if (0 == AfxStringCompareCi(AfxUriGetStringConst(data->name), AfxUriGetStringConst(&name)))
        {
            data->obj = &res->obj;
            iter->abort = TRUE;
        }
    }
}

_SGL afxPipelineModule _AfxDrawContextFindPipelineModule(afxDrawContext dctx, afxUri const *name)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(!AfxUriIsBlank(name));

    struct { afxUri const *name; afxObject *obj; } data = { name, NIL };
    //AfxClassForEveryInstance(&dsys->pipmObjt, FALSE, _AfxIteratorCompareResourceNameCiPipm, &data);

    if (data.obj)
        return (afxPipelineModule)data.obj;

    return NIL;
}

_SGL afxPipelineModule _AfxDrawContextFetchPipelineModule(afxDrawContext dctx, afxUri const *uri)
{
    afxError err = NIL;
    AfxEntry("dctx=%p,uri=%.*s", dctx, AfxPushString(AfxUriGetStringConst(uri)));
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssertType(uri, AFX_FCC_URI);

    afxUri name;
    AfxUriForkName(&name, uri);
    afxPipelineModule pipm = AfxDrawContextFindPipelineModule(dctx, &name);

    if (pipm) AfxObjectReacquire(&pipm->obj, NIL, NIL, NIL, NIL);
    else
    {
        if (!(pipm = AfxDrawContextUploadPipelineModule(dctx, uri)))
            AfxThrowError();
    }
    return pipm;
}

_SGL afxClassSpecification const _AfxPipmClassSpec;

afxClassSpecification const _AfxPipmClassSpec =
{
    AFX_FCC_PIPM,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxPipelineModule)),
    NIL,
    (void*)_AfxPipmCtor,
    (void*)_AfxPipmDtor,
    "Pipeline Module",
    &_AfxStdPipmImpl
};
