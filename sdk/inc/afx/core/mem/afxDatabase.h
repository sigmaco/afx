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

#ifndef AFX_DATABASE_H
#define AFX_DATABASE_H

#include "afxMemContext.h"

AFX_DEFINE_HANDLE(afxDatabase);

AFX_DEFINE_STRUCT(afxIndexedStringData) // at afxString._data
{
    afxDatabase db;
    afxNat      tableId,
                idx;
    afxNat32    crc32;
};

AFX_DEFINE_STRUCT(afxDatabaseEntry)
{
    afxNat32    crc32;
    afxNat      range;
};

AFX_DEFINE_STRUCT(afxDatabaseTable)
{
    afxNat  id;
    afxLink linkage;
    afxNat  cnt; // count of entries
};

AFX_OBJECT(afxDatabase)
{
    afxObject       obj;
    afxMemContext    all;
    afxList         entries;
    afxList         tables;
};

AFX_DEFINE_STRUCT(afxDatabaseStrategy)
{
    afxNat  maxLen, // max lenght of every strings, useful to force small names, such as 32-char long names for textures.
            cap,    // total amount of strings supported.
            stock;  // amount of space to preallocate.
};

AFX afxDatabase     AfxDatabaseAcquire(void *sys, afxDatabaseStrategy const *strategy);

AFX afxNat          AfxDatabaseIndex(afxDatabase *db, afxString const *str);
AFX afxNat          AfxDatabaseIndexFromRaw(afxDatabase *db, afxChar const *src, afxNat range);
AFX afxNat          AfxDatabaseDeindex(afxDatabase *db, afxString const *str);

AFX afxChar const*  AfxDatabaseGetConstData(afxDatabase *db, afxNat idx, afxString const *str);
AFX afxNat          AfxDatabaseGetSize(afxDatabase *db, afxNat idx, afxString const *str);

#endif//AFX_DATABASE_H