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

#ifndef AFX_VERSION_H
#define AFX_VERSION_H

#include "qwadro/inc/base/afxCoreDefs.h"
#include "qwadro/inc/base/afxString.h"

 // The QWADRO Specification is regularly updated with bug fixes and clarifications.
 // Occasionally new functionality is added to the core and at some point it is expected that there will be a desire to perform a large, 
 // breaking change to the API. In order to indicate to developers how and when these changes are made to the specification, 
 // and to provide a way to identify each set of changes, the QWADRO API maintains a version number.

 // The major version indicates a significant change in the API, which will encompass a wholly new version of the specification.
 // The minor version indicates the incorporation of new functionality into the core specification.
 // The patch version indicates bug fixes, clarifications, and language improvements have been incorporated into the specification.

 // Compatibility guarantees made about versions of the API sharing any of the same version numbers are documented in Core Versions
 // The version number is used in several places in the API.In each such use, the version numbers are packed into a 32 - bit integer as follows :
 // - The major version is a 10 - bit integer packed into bits 31 - 22.
 // - The minor version number is a 10 - bit integer packed into bits 21 - 12.
 // - The patch version number is a 12 - bit integer packed into bits 11 - 0.

#define AFX_MAKE_VERSION(major_, minor_, patch_) ((((afxUnit32)(major_)) << 22) | (((afxUnit32)(minor_)) << 12) | ((afxUnit32)(patch_)))
#define AFX_VER_MAJOR(ver_) ((afxUnit32)(ver_) >> 22)
#define AFX_VER_MINOR(ver_) (((afxUnit32)(ver_) >> 12) & 0x3FFU)
#define AFX_VER_PATCH(ver_) ((afxUnit32)(ver_) & 0xFFFU)

#define AFX_QWADRO_VER_MAJOR 0
#define AFX_QWADRO_VER_MINOR 1
#define AFX_QWADRO_VER_PATCH 404

/// O Qwadro SDK é atualizado frequentemente, por isso é útil poder sempre garantir que a API e a ABI estejam sincronizadas.
/// A maneira mais simples de fazer isso é usar as seguintes funções.

AFX afxBool             AfxTestSystemCompatibility(afxUnit verMajor, afxUnit verMinor, afxUnit verPatch);

AFX void                AfxGetSystemVersion(afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch);

AFX afxString const *   AfxGetSystemVersionString(void);

#endif//AFX_VERSION_H
