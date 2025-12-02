/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SEMAPHORE_C
#include "afxSystemDDK.h"

_AFX void* AfxGetSemaphoreContext(afxSemaphore sem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEM, 1, &sem);
    void* ctx = AfxGetHost(sem);
    //AfxAssertObjects(1, &ctx, afxFcc_DEVK);
    return ctx;
}

_AFX afxError _AfxSemStdDtor(afxSemaphore sem)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEM, 1, &sem);

    sem->value = NIL;

    return err;
}

_AFX afxError _AfxSemStdCtor(afxSemaphore sem, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SEM, 1, &sem);

    sem->value = NIL;

    return err;
}

_AFX afxClassConfig const _AfxSemStdImplementation =
{
    .fcc = afxFcc_SEM,
    .name = "Semaphore",
    .desc = "Device Synchronization Semaphore",
    .fixedSiz = sizeof(AFX_OBJECT(afxSemaphore)),
    .mmu = NIL,
    .ctor = (void*)_AfxSemStdCtor,
    .dtor = (void*)_AfxSemStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireSemaphores(void* ctx, afxUnit cnt, afxSemaphore semaphores[])
{
    afxError err = { 0 };
    //AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(semaphores);

    afxClass* cls = _AfxGetSemaphoreClass(ctx);
    AFX_ASSERT_CLASS(cls, afxFcc_SEM);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)semaphores, (void const*[]) { ctx }))
        AfxThrowError();

    return err;
}
