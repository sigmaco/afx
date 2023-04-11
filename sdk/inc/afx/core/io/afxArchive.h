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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// afxArchive allows you to save a complex network of objects in a permanent
// binary form (usually disk storage) that persists after those objects are deleted.

#ifndef AFX_ARCHIVE_H
#define AFX_ARCHIVE_H

#include "afxStream.h"

AFX_DEFINE_HANDLE(afxArchive);

AFX_DEFINE_INTERFACE(afxArchive, AfxArchive)
{
    afxNat      (*Read)(afxArchive arch, void *to, afxSize len);
    afxObject*  (*ReadObject)(afxArchive arch, afxClass const *meta);
    afxResult   (*ReadString)(afxArchive arch, afxString *str);

    afxNat      (*Write)(afxArchive arch, void const *from, afxSize len);
    afxResult   (*WriteObject)(afxArchive arch, afxObject const *obj);
    afxResult   (*WriteString)(afxArchive arch, afxString const *str);
};

AFX_OBJECT(afxArchive)
{
    AFX_OBJECT(afxStream)   ios;
};

#endif//AFX_ARCHIVE_H