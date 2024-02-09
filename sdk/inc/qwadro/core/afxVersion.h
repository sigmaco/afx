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

#ifndef AFX_VERSION_H
#define AFX_VERSION_H

#include "qwadro/core/afxCoreDefs.h"
#include "qwadro/core/afxString.h"

 /// The QWADRO Specification is regularly updated with bug fixes and clarifications.
 /// Occasionally new functionality is added to the core and at some point it is expected that there will be a desire to perform a large, 
 /// breaking change to the API. In order to indicate to developers how and when these changes are made to the specification, 
 /// and to provide a way to identify each set of changes, the QWADRO API maintains a version number.

 /// The major version indicates a significant change in the API, which will encompass a wholly new version of the specification.
 /// The minor version indicates the incorporation of new functionality into the core specification.
 /// The patch version indicates bug fixes, clarifications, and language improvements have been incorporated into the specification.

 /// Compatibility guarantees made about versions of the API sharing any of the same version numbers are documented in Core Versions
 /// The version number is used in several places in the API.In each such use, the version numbers are packed into a 32 - bit integer as follows :
 /// - The major version is a 10 - bit integer packed into bits 31 - 22.
 /// - The minor version number is a 10 - bit integer packed into bits 21 - 12.
 /// - The patch version number is a 12 - bit integer packed into bits 11 - 0.

#define AFX_MAKE_VERSION(major_, minor_, patch_) ((((afxNat32)(major_)) << 22) | (((afxNat32)(minor_)) << 12) | ((afxNat32)(patch_)))
#define AFX_VER_MAJOR(ver_) ((afxNat32)(ver_) >> 22)
#define AFX_VER_MINOR(ver_) (((afxNat32)(ver_) >> 12) & 0x3FFU)
#define AFX_VER_PATCH(ver_) ((afxNat32)(ver_) & 0xFFFU)


#define AFX_QWADRO_VER_MAJOR 2
#define AFX_QWADRO_VER_MINOR 7
#define AFX_QWADRO_CUSTOMIZATION 0
#define AFX_QWADRO_BUILD_NUM 30
#define AFX_QWADRO_VER_STRING AFX_STRINGIFY(AFX_QWADRO_VER_MAJOR.AFX_QWADRO_VER_MINOR.AFX_QWADRO_CUSTOMIZATION.AFX_QWADRO_BUILD_NUM)

#define AfxVersionsMatch AfxVersionsMatch_(AFX_QWADRO_VER_MAJOR, AFX_QWADRO_VER_MINOR, AFX_QWADRO_CUSTOMIZATION, AFX_QWADRO_BUILD_NUM)

AFX afxBool             AfxVersionsMatch_(afxInt verMajor, afxInt verMinor, afxInt customization, afxInt buildNum);

AFX void                AfxGetVersion(afxInt * MajorVersion, afxInt * MinorVersion, afxInt * Customization, afxInt * BuildNumber);

AFX afxString const *   AfxGetVersionString(void);


/// O Qwadro SDK é atualizado frequentemente, por isso é útil poder sempre garantir que a API e a ABI estejam sincronizadas.
/// A maneira mais simples de fazer isso é usar o seguinte macro:

/*
    if(AfxVersionsMatch())
    {
        // The .h matches the .dll, all is well
    }
    else
    {
        // The .h does not match the .dll, big trouble's brewing
    }
*/

/// Você pode descobrir os componentes numéricos individuais do Qwadro SDK que está usando assim:

/*
    // Find out what version of the .h file you're using
    afxInt verMajor = AFX_QWADRO_VER_MAJOR;
    afxInt verMinor = AFX_QWADRO_VER_MINOR;
    afxInt customization = AFX_QWADRO_CUSTOMIZATION;
    afxInt buildNum = AFX_QWADRO_BUILD_NUM;

    // Find out what version of the .dll you're using
    AfxGetVersion(&verMajor, &verMinor, &customization, &buildNum);
*/

/// ou você pode simplesmente obter strings pré-construídas que tenham o número de versão completo:

/*
    // Find out what version of the .h file you're using
    char const *verString = AFX_QWADRO_VER_STRING;

    // Find out what version of the .dll you're using
    char const *verString = AfxGetVersionString();

    // Find out if this is a pre-release or release version of the .h
    char const *releaseName = AfxReleaseName;
*/

#endif//AFX_VERSION_H