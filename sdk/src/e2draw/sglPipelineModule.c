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
                    AfxEcho("afxPipelineModule %p GPU-side data instanced.", pipm);
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

    afxDrawContext dctx = AfxPipelineModuleGetContext(pipm);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxDrawContextGetMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);
    afxDrawSystem dsys = AfxDrawContextGetDrawSystem(dctx);
    AfxAssertObject(dsys, AFX_FCC_DSYS);
    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    afxFile file;

    if (!(file = AfxFileSystemOpenFile(fsys, AFX_IO_FLAG_W, uri))) AfxThrowError();
    else
    {
        if (AfxPipelineModuleSerialize(pipm, &file->ios)) AfxThrowError();
        else
        {
            // success
        }
        AfxObjectRelease(&file->ios.obj);
    }
    return err;
}

_SGL afxError _AfxPipelineModuleSerialize(afxPipelineModule pipm, afxStream ios)
{
    afxError err = NIL;
    AfxAssertObject(pipm, AFX_FCC_PIPM);
    AfxAssertObject(ios, AFX_FCC_IOS);

    AfxStreamWrite(ios, pipm->code, pipm->len);

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
    {
        afxDrawContext dctx = AfxPipelineModuleGetContext(pipm);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxDrawContextGetMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        AfxDeallocate(mem, pipm->code);
    }
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
            afxDrawContext dctx = args->dctx;
            AfxAssertObject(dctx, AFX_FCC_DCTX);
            afxMemory mem = AfxDrawContextGetMemory(dctx);
            AfxAssertObject(mem, AFX_FCC_MEM);

            void* codecpy;

            if (!(codecpy = AfxAllocate(mem, args->len + sizeof(afxByte), AfxSpawnHint()))) AfxThrowError();
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
                    AfxDeallocate(mem, codecpy);
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
