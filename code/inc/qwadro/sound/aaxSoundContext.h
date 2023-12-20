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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SOUND_CONTEXT_H
#define AFX_SOUND_CONTEXT_H

#include "aaxSoundInput.h"
#include "aaxSoundOutput.h"
#include "qwadro/core/afxThread.h"

AFX_DEFINE_HANDLE(aaxSoundContext);

// LISTENER

/// Sets a game object's associated listeners.
/// All listeners that have previously been added via AddListener or set via SetListeners will be removed and replaced with the listeners in the array listenerObjs.
/// Calling this function will override the default set of listeners and emitterObj will now reference its own, unique set of listeners.

AFX afxError AfxSetListeners
(
    aaxSoundContext     sctx,
    afxObject           emitterObj, // Emitter object.  Must have been previously registered via RegisterGameObj.
    afxObject const*    listenerObjs, // Array of listener object IDs that will be activated for emitterObj. 
    afxNat32            listenerCnt // Length of array
);

/// Add a single listener to a object's set of associated listeners.
/// Any listeners that have previously been added or set via AddListener or SetListeners will remain as listeners and listenerObj will be added as an additional listener.
/// Calling this function will override the default set of listeners and emitterObj will now reference its own, unique set of listeners.

AFX afxResult AfxAddListener
(
    aaxSoundContext     sctx,
    afxObject           emitterObj, // Emitter object. Must have been previously registered via RegisterGameObj.
    afxObject           listenerObj // Listener object IDs that will be activated for emitterObj. 
);

/// Remove a single listener from a object's set of active listeners.
/// Calling this function will override the default set of listeners and emitterObj will now reference its own, unique set of listeners.

AFX afxResult AfxRemoveListener
(
    aaxSoundContext     sctx,
    afxObject           emitterObj, // Emitter object.
    afxObject           listenerObj // Listener object IDs that will be deactivated for emitterObj. Objects must have been previously registered.
);

/// Sets the default set of associated listeners for game objects that have not explicitly overridden their listener sets. 
/// Upon registration, all game objects reference the default listener set, until a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that object.
/// All default listeners that have previously been added via AddDefaultListener or set via SetDefaultListeners will be removed and replaced with the listeners in the array listenerObjs.

AFX afxResult AfxSetDefaultListeners
(
    aaxSoundContext     sctx,
    afxObject const*    listenerObjs, // Array of listener object IDs that will be activated for subsequent registrations. Objects must have been previously registered.
    afxNat32            istenerCnt // Length of array
);

/// Add a single listener to the default set of listeners. Upon registration, all objects reference the default listener set, 
/// until a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that object.

AFX afxResult AfxAddDefaultListener
(
    aaxSoundContext     sctx,
    afxObject           listenerObj // Listener object IDs that will be added to the default set of listeners.
);

/// Remove a single listener from the default set of listeners. Upon registration, all objects reference the default listener set, 
/// until a call to AddListener, RemoveListener, SetListeners or SetGameObjectOutputBusVolume is made on that object.

AFX afxResult AfxRemoveDefaultListener
(
    aaxSoundContext     sctx,
    afxObject           listenerObj // Listener object IDs that will be removed from the default set of listeners.
);

/// Resets the listener associations to the default listener(s), as set by SetDefaultListeners. 
/// This will also reset per-listener gains that have been set using SetGameObjectOutputBusVolume.

AFX afxResult AfxResetListenersToDefault
(
    aaxSoundContext     sctx,
    afxObject           emitterObj // Emitter object.
);

/// Sets a listener's spatialization parameters. This lets you define listener-specific volume offsets for each audio channel.
/// If spatialized is false, only volOffsets is used for this listener (3D positions  have no effect on the speaker distribution). 
/// Otherwise, volOffsets is added to the speaker distribution computed for this listener.

AFX afxResult AfxSetListenerSpatialization
(
    aaxSoundContext     sctx,
    afxObject           listenerID, // Listener game object ID
    afxBool             spatialized, // Spatialization toggle
    aaxChannelConfig    chanConfig, // Channel configuration associated with volumes volOffsets. Ignored if volOffsets is NULL.
    afxReal32*          volOffsets // Per-speaker volume offset, in dB.
);

#endif//AFX_SOUND_CONTEXT_H
