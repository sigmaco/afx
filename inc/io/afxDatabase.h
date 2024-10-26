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

#ifndef AFX_DATABASE_H
#define AFX_DATABASE_H

#include "qwadro/inc/mem/afxMemory.h"

AFX_DEFINE_STRUCT(afxIndexedStringData) // at afxString._data
{
    afxDatabase db;
    afxUnit      tableId,
                idx;
    afxUnit32    crc32;
};

AFX_DEFINE_STRUCT(afxDatabaseEntry)
{
    afxUnit32    crc32;
    afxUnit      range;
};

AFX_DEFINE_STRUCT(afxDatabaseTable)
{
    afxUnit  id;
    afxLink linkage;
    afxUnit  cnt; // count of entries
};

AFX_OBJECT(afxDatabase)
{
    afxMmu    all;
    afxList         entries;
    afxList         tables;
};

AFX_DEFINE_STRUCT(afxDatabaseStrategy)
{
    afxUnit  maxLen, // max lenght of every strings, useful to force small names, such as 32-char long names for textures.
            cap,    // total amount of strings supported.
            stock;  // amount of space to preallocate.
};

AFX afxDatabase     AfxDatabaseAcquire(void *sys, afxDatabaseStrategy const *strategy);

AFX afxUnit          AfxDatabaseIndex(afxDatabase *db, afxString const *str);
AFX afxUnit          AfxDatabaseIndexFromRaw(afxDatabase *db, afxChar const *src, afxUnit range);
AFX afxUnit          AfxDatabaseDeindex(afxDatabase *db, afxString const *str);

AFX afxChar const*  AfxDatabaseGetConstData(afxDatabase *db, afxUnit idx, afxString const *str);
AFX afxUnit          AfxDatabaseGetSize(afxDatabase *db, afxUnit idx, afxString const *str);

#endif//AFX_DATABASE_H
