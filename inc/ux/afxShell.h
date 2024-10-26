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

/*
    Copyright (c) 2017 SIGMA FEDERATION.

    SIGMA FEDERATION ("SIGMA") supplies this software to you in
    consideration of your agreement to the following terms, and your use,
    installation, modification or redistribution of this SIGMA software
    constitutes acceptance of these terms.  If you do not agree with these
    terms, please do not use, install, modify or redistribute this SIGMA
    software.

    In consideration of your agreement to abide by the following terms, and
    subject to these terms, SIGMA grants you a personal, non-exclusive
    license, under SIGMA's copyrights in this original SIGMA software (the
    SIGMA Software), to use, reproduce, modify and redistribute the
    SIGMA Software, with or without modifications, in source and/or binary
    forms; provided that if you redistribute the SIGMA Software, you must
    retain the copyright notice of SIGMA, this notice and the following
    text and disclaimers in all such redistributions of the SIGMA Software.
    Neither the name, trademarks, service marks nor logos of SIGMA
    FEDERATION may be used to endorse or promote products derived from the
    SIGMA Software without specific prior written permission from SIGMA.
    Except as expressly stated in this notice, no other rights or licenses
    express or implied, are granted by SIGMA herein, including but not
    limited to any patent rights that may be infringed by your derivative
    works or by other works in which the SIGMA Software may be
    incorporated. No hardware is licensed hereunder.

    THE SIGMA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT
    WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
    INCLUDING WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE,
    NON-INFRINGEMENT, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
    ITS USE AND OPERATION EITHER ALONE OR IN COMBINATION WITH OTHER
    PRODUCTS.

    IN NO EVENT SHALL SIGMA BE LIABLE FOR ANY SPECIAL, INDIRECT,
    INCIDENTAL, EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, LOST PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
    USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY
    OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE
    SIGMA SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT,
    TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
    SIGMA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

  //////////////////////////////////////////////////////////////////////////////
 // The Unified Shell                                                        //
//////////////////////////////////////////////////////////////////////////////

/// The Unified I/O System, also called Unified Shell, is a full integration between Qwadro systems to improve the user experience and unify it with all the applications

#ifndef AUX_SHELL_H
#define AUX_SHELL_H

#include "qwadro/inc/ux/afxUxDefs.h"
// ux
#include "qwadro/inc/ux/afxHid.h"
#include "qwadro/inc/ux/afxGamepad.h"
#include "qwadro/inc/ux/afxWindow.h"
#include "qwadro/inc/ux/afxWidget.h"
#include "qwadro/inc/ux/afxSession.h"

AFX_DEFINE_STRUCT(afxShellCaps)
{
    afxBool8        windows;
};

AUX afxBool         AfxGetUxEventId(afxString const* name, auxEventId* id);
AUX afxBool         AfxTriggerUxEventId(afxString const* name, afxObject target);
AUX afxError        AfxRegisterUxEventId(afxString const* name, auxEventId* id);
//AUX afxBool         AfxAddUxEventHandler(afxString const* name, afxObject target, fn, afxUnit priority);

AUX afxTime         AfxPollInput(void);

AUX afxClass const* AuxGetShellClass(void);

// SHELL DISCOVERY /////////////////////////////////////////////////////////////

AUX afxUnit         AfxInvokeShells(afxUnit first, afxUnit cnt, afxBool(*f)(afxShell, void*), void* udd);
AUX afxUnit         AfxEvokeShells(afxBool(*flt)(afxShell, void*), void* fdd, afxUnit first, afxUnit cnt, afxShell shells[]);
AUX afxUnit         AfxEnumerateShells(afxUnit first, afxUnit cnt, afxShell shells[]);

AUX afxBool         AfxGetShell(afxUnit sshId, afxShell* shell);

// SHELL HANDLING //////////////////////////////////////////////////////////////

AUX afxUnit         AfxInvokeSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxBool(*f)(afxSession, void*), void *udd);
AUX afxUnit         AfxEvokeSessions(afxShell ssh, afxBool(*flt)(afxSession, void*), void* fdd, afxUnit first, afxUnit cnt, afxSession sessions[]);
AUX afxUnit         AfxEnumerateSessions(afxShell ssh, afxUnit first, afxUnit cnt, afxSession sessions[]);

AUX afxClass const* AfxGetMouseClass(afxShell ssh);
AUX afxClass const* AfxGetKeyboardClass(afxShell ssh);
AUX afxClass const* AfxGetControllerClass(afxShell ssh);
AUX afxClass const* AfxGetSessionClass(afxShell ssh);

AUX void*           AfxGetShellIdd(afxShell ssh);

AUX afxBool         AfxIsShellPrompt(afxShell ssh);

AUX afxBool         AfxPollEvents();

// SHELL CONTEXTUALIZATION /////////////////////////////////////////////////////



#endif//AUX_SHELL_H
