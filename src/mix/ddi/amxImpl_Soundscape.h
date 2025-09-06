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

#ifndef AMX_IMPL___SOUNDSCAPE_H
#define AMX_IMPL___SOUNDSCAPE_H

#include "amxImpl_Executor.h"

#ifdef _AMX_SOUNDSCAPE_C
#ifdef _AMX_SOUNDSCAPE_IMPL
AFX_OBJECT(_amxSoundscape)
#else
AFX_OBJECT(amxSoundscape)
#endif
{
    afxChain        classes;
    afxClass        sndCls;

    afxFlags        flags;
    afxUnit         ambiOrder;

    afxTransform    placement;
    afxV3d          velocity;

    afxMixContext   mix;
    afxUnit         baseSmixIdx;
    afxUnit         exuIdx; // renderer
};
#endif

#ifdef _AMX_SOUND_C
#ifdef _AMX_SOUND_IMPL
AFX_OBJECT(_amxSound)
#else
AFX_OBJECT(amxSound)
#endif
{
    afxReal gain; // A value of 0.0 is meaningless with respect to a logarithmic scale; it is silent.
    afxBool looping;
    afxV3d position;
    afxV3d velocity;
    afxV3d direction;
    afxReal pitch; // A multiplier for the frequency (sample rate) of the source's buffer.
    afxReal innerConeAngle; // The angle covered by the inner cone, where the source will not attenuate.
    afxReal outerConeAngle; // The angle covered by the outer cone, where the source will be fully attenuated.

    afxBool playing;
    afxSphere bounding;

    afxTransform t;
    afxLink playQueue;

    struct
    {
        afxFlags   flags;
        afxReal         dtLocalClockPending;
        afxReal         localClock;
        afxReal         speed;
        afxReal         localDur;
        afxInt          currIterIdx;
        afxInt          iterCnt;
        afxReal         currWeight;
        //arxCapstanTiming  timing;
        afxUnit32       easeInValues;
        afxUnit32       easeOutValues;
        void*           userData[4];
    } ctrl;
    amxAudio    audio;
    afxUnit     chanCnt;
    afxUnit     offset;
    afxFlags    flags;
    afxReal     streamPos;
};
#endif

AMX afxClass const* _AmxSndsGetSndClass(amxSoundscape snds);

// Spherical Harmonics (SN3D + ACN).
// Spherical Harmonics module: Real-valued, SN3D-normalized, ACN channel ordering

int sh_num_channels(int order); // (N+1)^2

// Evaluate SN3D-normalized real SH basis for azimuth (radians), elevation (radians)
void sh_eval_sn3d(int order, float azimuth, float elevation, float* out);

// ACN channel index from degree l and order m
int sh_acn_index(int l, int m);

// Basic encoder: Takes a mono input and encodes to an ambisonic soundfield using SH basis.

typedef struct {
    int order;
    int num_channels;
    float azimuth;
    float elevation;
    float* sh_basis; // Cached basis
} AmbiEncoder;

// Create encoder for a fixed direction (azimuth/elevation in radians)
afxError ambi_encoder_create(AmbiEncoder* enc, int order, float azimuth, float elevation);
void ambi_encoder_process(AmbiEncoder* enc, const float* mono_in, float** ambi_out, int num_frames);
void ambi_encoder_destroy(AmbiEncoder* enc);

// Basic Ambisonic Decoder (to stereo/binaural)
// A basic stereo decoder that just applies a static decoding matrix (for now).
// For simplicity, we�ll do a basic stereo decoder that uses a precomputed decoding matrix from Ambisonic channels to 
// stereo (left and right). This avoids needing HRTFs for now; that's a deeper topic we can modularize later.

typedef struct {
    int order;
    int num_input_channels;   // Ambisonic input channels (based on order)
    int num_output_channels;  // Usually 2 (stereo), could be N
    float** decode_matrix;    // [out][in] � decode_matrix[0] = left channel weights
} AmbiDecoder;

// Creates a stereo decoder with basic decoding matrix
afxError ambi_decoder_create(AmbiDecoder* dec, int order);

// Process ambisonic input buffer [in_channels][frames] >> output stereo [2][frames]
void ambi_decoder_process(AmbiDecoder* dec, float** ambi_in, float** stereo_out, int num_frames);

void ambi_decoder_destroy(AmbiDecoder* dec);

// We can rotate the whole soundfield without touching the individual sources.
// How Ambisonic Rotation Works.
// Rotations in ambisonics are linear operations in SH space.
// Each ambisonic order has a rotation matrix that applies to the subspace of that order.
// The full rotation matrix is block-diagonal: one block per order (0th, 1st, 2nd, etc.).

typedef struct {
    int order;
    int num_channels;
    float** rotation_matrix; // [n_channels][n_channels]
} AmbiRotator;

// We'll implement 1st-order rotation using a hardcoded matrix from literature (per RFC 7845).

// Create a rotator from yaw/pitch/roll (in radians, ZYX order)
afxError ambi_rotator_create(AmbiRotator* rot, int order, float yaw, float pitch, float roll);

// Apply rotation to ambisonic buffer [in][frames] >> [out][frames]
void ambi_rotator_process(AmbiRotator* rot, float** in, float** out, int num_frames);

void ambi_rotator_destroy(AmbiRotator* rot);

// Implementing a HRTF-based binaural decoder, so we can render our ambisonic signals to realistic 3D audio over headphones.

// HRTF-Based Binaural Decoding
// We�ll project the ambisonic field to a set of virtual loudspeakers (e.g., 36 directions on a sphere).
// Each speaker is convolved with a corresponding HRTF for left/right ears.
// Result: realistic spatialization over headphones.
// This method is called virtual speaker decoding or Ambisonic Binaural Rendering (ABR).

// Define HRTF and Virtual Speaker Structures.

typedef struct {
    float azimuth;
    float elevation;
    float* ir_left;   // [ir_length]
    float* ir_right;  // [ir_length]
} HRTFEntry;

typedef struct {
    int num_directions;
    int ir_length;
    HRTFEntry* entries;
} HRTFSet;

typedef struct {
    int order;
    int num_channels;
    int num_frames;
    int ir_length;
    int num_speakers;

    float** speaker_signals;  // [speakers][frames]
    HRTFSet* hrtfs;
} BinauralDecoder;

// Load simplified HRTF set (IRs per direction in degrees)
afxError hrtf_load_dummy(HRTFSet* set, int num_speakers, int ir_length);

// Decode ambisonic buffer [channels][frames] >> stereo [2][frames]
afxError binaural_decoder_create(BinauralDecoder* decoder, int order, int num_frames, HRTFSet* hrtfs);
void binaural_decoder_process(BinauralDecoder* decoder, float** ambi, float** stereo_out);
void binaural_decoder_destroy(BinauralDecoder* decoder);
void hrtf_free(HRTFSet* hrtfs);

// Support for Multiple Moving Sources

typedef struct {
    float* buffer;          // input mono audio
    int length;             // number of samples
    float azimuth;          // current azimuth (radians)
    float elevation;        // current elevation (radians)

    // Or remove this if you animate externally.
    float(*position_func)(int t, float* az, float* el); // optional animation function
} SoundSource;

void binaural_decode_multiple(BinauralDecoder* bdec, HRTFSet* hrtfs,
    SoundSource* sources, int num_sources,
    float* stereo_out, int num_samples,
    float yaw, float pitch, float roll, int t_frame);

#endif//AMX_IMPL___SOUNDSCAPE_H
