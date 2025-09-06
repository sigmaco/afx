/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

/*
    Audio is an inherently analog feature of the natural world. 
    As an object vibrates, it causes the molecules surrounding it to vibrate as well. 
    These molecules affect the ones adjacent to them, and so forth, 
    propagating the vibration in the form of a wave outward from the source until the amplitude of the wave (its volume) fades away with distance. 
    The granularity of an audio wave in the real world, then, is that of an individual molecule of the medium through which the sound wave is traveling.

    On Earth, the medium most audio travels through is the air. 
    Some audio does travel through water, or even through the rock comprising the planet itself (if you've ever heard the 
    rumble or boom of an earthquake, you've experienced this phenomenon), but nearly all of the sounds you hear every day 
    travel to your ears through the air.

    The sounds a person hears every day are, then, actually vibrations in the air which cause the inner workings of the ear. 
    The farther the air molecules move with each pulse of the wave, the higher the amplitude of the wave, and the louder the sound is. 
    The faster the molecules vibrate, the higher the frequency of the wave.

    The higher the amplitude (height) of the wave, the louder the sound is at that instant. 
    The shorter the wavelength (the closer together the crests of the wave are), the higher the frequency (or pitch) of the sound that's produced.
*/

/*
    From a physics perspective, 4D sound would involve considering sound waves in a four-dimensional 
    spacetime rather than the typical three dimensions of space (length, width, and height). 
    In this context, sound would still be a mechanical wave, but the fourth dimension would introduce 
    new considerations related to time and possibly spacetime curvature or relativity.

    Sound in the Context of Spacetime:

    In regular three-dimensional space, sound is a longitudinal wave that propagates through a medium 
    (like air, water, or solids) by compressing and rarefying the particles in that medium. 
    This occurs in three spatial dimensions—forward, backward, up, down, and side to side.

    To bring time into the equation, you need to consider how sound waves behave and evolve over time. 
    The fourth dimension in this case would likely be time, where sound waves would still travel through 
    the medium but would now include the temporal evolution of the sound as it moves.

    1. Four-Dimensional Spacetime:
    In the framework of special relativity, we treat space and time as a unified entity called spacetime. 
    This is a four-dimensional continuum with three spatial dimensions (x, y, z) and one temporal dimension (t). 
    In this context, any wave, including sound, could be described by a 4D wave equation that incorporates both 
    the movement of sound in space and its progression over time.

    For sound to be treated as a wave in 4D spacetime, we would need to solve wave equations (like the d'Alembert 
    equation for relativistic waves) that account for the propagation of the wave through 4D spacetime.

    2. Sound Propagation in 4D:
    In 3D space, sound travels as a wave through a medium, with the speed of sound being constant for a given 
    medium (e.g., in air at 20°C, it's about 343 m/s). However, in a 4D spacetime framework, we'd have to consider 
    the effects of relativity—particularly if the sound is moving close to the speed of light or involves extreme gravitational fields.

    Relativistic Effects: If we were dealing with sound at speeds close to the speed of light, relativistic effects 
    (such as time dilation or length contraction) would influence how sound propagates. The sound waves might be 
    stretched or compressed in time (due to time dilation), or the medium through which sound is traveling could be 
    altered by relativistic speeds.

    Medium Behavior: In 4D, the properties of the medium might also change with the introduction of the time component. 
    The wave might not simply propagate through space as it does in 3D; it could interact with time or with the fabric 
    of spacetime itself.

    3. Spacetime Curvature:
    If you consider general relativity, where spacetime is curved by the presence of mass and energy, sound waves could 
    be influenced by the curvature of spacetime. For example, in a strong gravitational field (near a black hole or neutron star), 
    sound might behave differently, possibly being distorted or redshifted (in frequency) as it travels through the curved spacetime. 
    This means the way sound propagates could change based on the geometry of spacetime.

    4. Higher-Dimensional Theories:
    In certain higher-dimensional theories of physics (such as string theory or theories of extra dimensions), sound might exist 
    in a more complex way, involving not just the familiar three spatial dimensions but also higher-dimensional spatial or temporal 
    components. In these cases, sound could potentially "leak" into higher dimensions or interact with these extra dimensions, 
    creating an even more complex wave behavior that would be difficult to describe using traditional physics.

    In a physics perspective, 4D sound is an extension of the typical sound wave description into a four-dimensional spacetime framework. 
    This involves treating sound as a wave that propagates through 4D spacetime (3 spatial dimensions and 1 temporal dimension).
    Considering how relativistic effects might influence sound propagation, such as time dilation or spacetime curvature.
    Sound could also behave differently in strong gravitational fields, due to the way spacetime itself bends and stretches.
    However, it's important to note that sound, by definition, requires a medium (such as air or water) to propagate, and it can't 
    travel through a vacuum. In a 4D spacetime context, the sound would still require a medium to propagate, but the spatial dimensions 
    and time would interact in new, more complex ways than what we're used to in everyday 3D space.
*/

// Voice
// each of the notes or sounds able to be produced simultaneously by a musical instrument (especially an electronic one) or a computer.
// (in an electronic musical instrument) each of a number of preset or programmable tones.

// Acoustic
// the properties or qualities of a room or building that determine how sound is transmitted in it.

// Sound field
// A sound field refers to the spatial distribution of sound waves in a given environment. 
// It describes how sound propagates through space, the way it interacts with surfaces, 
// and the resulting pattern of pressure variations that we perceive as sound. 
// The characteristics of a sound field can vary based on factors like the frequency of the sound, 
// the shape of the room, and the materials that sound waves interact with (such as walls, furniture, or absorptive materials).

#ifndef AMX_SOUND_H
#define AMX_SOUND_H

#include "qwadro/mix/afxMixDefs.h"

AFX_DEFINE_STRUCT(amxSoundInfo)
{
    amxAudio        aud;
    afxTransform    origin;
    afxV3d          velocity;
    afxUnit         playBegin;
    afxUnit         playLen;
    afxUnit         iterCnt;
    afxUnit         iterBegin;
    afxUnit         iterLen;
};

AMX afxError    AmxDoSound(amxSoundscape snds, amxSoundInfo const* info, afxUnit* sndId);

AMX afxError    AfxStopSound(amxSoundscape snds, afxUnit sndId);

////////////////////////////////////////////////////////////////////////////////

AMX void        AmxTransformSound(amxSound snd, afxTransform const* t);

AMX afxUnit     AmxAccumulateSoundSamples(amxSound snd, afxUnit sampleCnt, afxUnit chCnt, afxUnit freq, afxUnit bufCap, afxReal output[]);

AMX afxError    AmxRewindSound(amxSound snd);
AMX afxBool     AmxHasSoundEnded(amxSound snd);

#endif//AMX_SOUND_H
