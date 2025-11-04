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

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

// 5417

#define _AMX_MIX_C
//#define _AMX_MIX_BRIDGE_C
//#define _AMX_MIX_SYSTEM_C
//#define _AMX_MIX_QUEUE_C
#define _AMX_BUFFER_C
#define _AMX_AUDIO_C
#define _AMX_VOICE_C
#define _AMX_MIX_CONTEXT_C
#define _AMX_SINK_C
#include "amxImplementation.h"

int sh_num_channels(int order) {
    return (order + 1) * (order + 1);
}

int sh_acn_index(int l, int m) {
    return l * l + l + m;
}

// Associated Legendre polynomial P(l, m, x), for real SH
static float legendre(int l, int m, float x) {
    float pmm = 1.0f;
    if (m > 0) {
        float somx2 = sqrtf((1.0f - x) * (1.0f + x));
        float fact = 1.0f;
        for (int i = 1; i <= m; i++) {
            pmm *= -fact * somx2;
            fact += 2.0f;
        }
    }
    if (l == m) return pmm;
    float pmmp1 = x * (2.0f * m + 1.0f) * pmm;
    if (l == m + 1) return pmmp1;
    float pll = 0.0f;
    for (int ll = m + 2; ll <= l; ll++) {
        pll = ((2.0f * ll - 1.0f) * x * pmmp1 - (ll + m - 1.0f) * pmm) / (ll - m);
        pmm = pmmp1;
        pmmp1 = pll;
    }
    return pll;
}

// Real SH basis (SN3D norm, ACN ordering)
void sh_eval_sn3d(int order, float azimuth, float elevation, float* out) {
    int index = 0;
    float cos_theta = cosf(elevation);
    for (int l = 0; l <= order; ++l) {
        for (int m = -l; m <= l; ++m) {
            float k = (m == 0) ? 1.0f : sqrtf(2.0f);
            float norm = k * sqrtf((2.0f * l + 1.0f) /
                (4.0f * AFX_PI) *
                tgammaf(l - abs(m) + 1) /
                tgammaf(l + abs(m) + 1));
            float leg = legendre(l, abs(m), cos_theta);
            float value = norm * leg *
                (m < 0 ? sinf(abs(m) * azimuth) :
                (m > 0 ? cosf(m * azimuth) : 1.0f));
            out[sh_acn_index(l, m)] = value;
        }
    }
}

afxError ambi_encoder_create(AmbiEncoder* enc, int order, float azimuth, float elevation) {
    enc->order = order;
    enc->num_channels = sh_num_channels(order);
    enc->azimuth = azimuth;
    enc->elevation = elevation;
    enc->sh_basis = malloc(sizeof(float) * enc->num_channels);
    sh_eval_sn3d(order, azimuth, elevation, enc->sh_basis);
    return 0;
}

void ambi_encoder_process(AmbiEncoder* enc, const float* mono_in, float** ambi_out, int num_frames) {
    for (int c = 0; c < enc->num_channels; ++c) {
        for (int i = 0; i < num_frames; ++i) {
            ambi_out[c][i] = mono_in[i] * enc->sh_basis[c];
        }
    }
}

void ambi_encoder_destroy(AmbiEncoder* enc) {
    if (enc) {
        free(enc->sh_basis);
    }
}

// Simple stereo decoding matrix for up to 1st order
// You can replace this with better matrices (e.g. pseudoinverse, or HRTF-based)

static float stereo_decode_matrix_1st_order[2][4] = {
    // W     Y     Z     X   (ACN: 0 1 2 3)
    { 1.0f, 0.0f, 0.0f, 1.0f }, // Left
    { 1.0f, 0.0f, 0.0f, -1.0f } // Right
};

afxError ambi_decoder_create(AmbiDecoder* dec, int order) {
    if (order > 1) return 0; // For now, only support 1st-order

    dec->order = order;
    dec->num_input_channels = sh_num_channels(order);
    dec->num_output_channels = 2;

    dec->decode_matrix = malloc(sizeof(float*) * 2);
    for (int ch = 0; ch < 2; ++ch) {
        dec->decode_matrix[ch] = malloc(sizeof(float) * dec->num_input_channels);
        AfxCopy(dec->decode_matrix[ch], stereo_decode_matrix_1st_order[ch],
            sizeof(float) * dec->num_input_channels);
    }

    return 0;
}

void ambi_decoder_process(AmbiDecoder* dec, float** ambi_in, float** stereo_out, int num_frames) {
    for (int ch = 0; ch < dec->num_output_channels; ++ch) {
        for (int i = 0; i < num_frames; ++i) {
            float sample = 0.0f;
            for (int k = 0; k < dec->num_input_channels; ++k) {
                sample += dec->decode_matrix[ch][k] * ambi_in[k][i];
            }
            stereo_out[ch][i] = sample;
        }
    }
}

void ambi_decoder_destroy(AmbiDecoder* dec) {
    if (!dec) return;
    for (int ch = 0; ch < dec->num_output_channels; ++ch) {
        free(dec->decode_matrix[ch]);
    }
    free(dec->decode_matrix);
}

static void build_1st_order_rotation_matrix(float yaw, float pitch, float roll, float matrix[4][4]) {
    // Use basic 3D rotation matrices (Z-Y-X order)

    // Convenience
    float cy = cosf(yaw), sy = sinf(yaw);
    float cp = cosf(pitch), sp = sinf(pitch);
    float cr = cosf(roll), sr = sinf(roll);

    // Full 3x3 rotation matrix (R = Rz * Ry * Rx)
    float R[3][3] = {
        {
            cy * cp,
            cy * sp * sr - sy * cr,
            cy * sp * cr + sy * sr
        },
        {
            sy * cp,
            sy * sp * sr + cy * cr,
            sy * sp * cr - cy * sr
        },
        {
            -sp,
            cp * sr,
            cp * cr
        }
    };

    // First-order SH basis: ACN = [0=W, 1=Y, 2=Z, 3=X]
    // Rotation affects X/Y/Z components (1st order)
    // We'll set up a 4x4 identity and fill the 3x3 rotation block

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;

    // X = 3, Y = 1, Z = 2
    int map[3] = { 3, 1, 2 }; // X, Y, Z >> ACN

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            matrix[map[i]][map[j]] = R[i][j];
        }
    }
}

afxError ambi_rotator_create(AmbiRotator* rot, int order, float yaw, float pitch, float roll) {
    if (order > 1) return 0; // only support 1st-order for now

    int n = sh_num_channels(order);

    rot->order = order;
    rot->num_channels = n;
    rot->rotation_matrix = malloc(sizeof(float*) * n);
    for (int i = 0; i < n; ++i) {
        rot->rotation_matrix[i] = calloc(n, sizeof(float));
    }

    // Fill 1st-order matrix
    float m4[4][4];
    build_1st_order_rotation_matrix(yaw, pitch, roll, m4);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            rot->rotation_matrix[i][j] = m4[i][j];

    return 0;
}

void ambi_rotator_process(AmbiRotator* rot, float** in, float** out, int num_frames) {
    int n = rot->num_channels;
    for (int i = 0; i < n; ++i) {
        for (int f = 0; f < num_frames; ++f) {
            float sum = 0.0f;
            for (int j = 0; j < n; ++j) {
                sum += rot->rotation_matrix[i][j] * in[j][f];
            }
            out[i][f] = sum;
        }
    }
}

void ambi_rotator_destroy(AmbiRotator* rot) {
    if (!rot) return;
    for (int i = 0; i < rot->num_channels; ++i) {
        free(rot->rotation_matrix[i]);
    }
    free(rot->rotation_matrix);
}

// Dummy HRTF Loader
// Later you can load SOFA or HRIR files. For now, generate basic L/R IRs per direction.

static float db_to_gain(float db) {
    return powf(10.0f, db / 20.0f);
}

// Generate a dummy HRTF with delay-based panning (not realistic, but useful for structure)
afxError hrtf_load_dummy(HRTFSet* set, int num_speakers, int ir_length) {
    
    set->num_directions = num_speakers;
    set->ir_length = ir_length;
    set->entries = calloc(num_speakers, sizeof(HRTFEntry));

    for (int i = 0; i < num_speakers; ++i) {
        float az = 2 * AFX_PI * i / num_speakers;
        float el = 0.0f;

        set->entries[i].azimuth = az;
        set->entries[i].elevation = el;
        set->entries[i].ir_left = calloc(ir_length, sizeof(float));
        set->entries[i].ir_right = calloc(ir_length, sizeof(float));

        int delay_left = ir_length / 2 + (int)(10 * sinf(az)); // simulate ITD
        int delay_right = ir_length / 2 - (int)(10 * sinf(az));

        if (delay_left >= 0 && delay_left < ir_length)
            set->entries[i].ir_left[delay_left] = db_to_gain(-2.0f); // simple gain
        if (delay_right >= 0 && delay_right < ir_length)
            set->entries[i].ir_right[delay_right] = db_to_gain(-2.0f);
    }

    /*
        The dummy HRTF works as a minimal prototype and proof of concept; but it won’t sound realistic or spatially accurate.

        If you want to hear convincing 3D audio, you’ll want to load real HRTFs from a dataset (like MIT Kemar, CIPIC, or IRCAM);
        typically in SOFA format; and integrate them as impulse responses in your binaural decoder.

        What the Dummy HRTF Does:

        It simulates basic interaural time differences (ITD) by adding tiny delays to left/right impulse responses based on speaker azimuth.
        The IRs are very short and very simple; basically a few samples with small delays and gains.
        This results in a crude spatial effect: you might hear the sound shift slightly left or right depending on direction.
        It does not simulate spectral cues (ILDs, pinna filtering, elevation cues, frequency shaping), so it won’t sound realistic or “3D” as a real HRTF would.

        When is the Dummy Good Enough?

        For testing and development, it’s perfect:
        You can verify your pipeline works end-to-end (encode >> rotate >> decode >> binaural output).
        For understanding the structure of binaural decoding.
        For debugging convolution and data flow.

        When You’ll Need Real HRTFs?
        To get realistic 3D spatialization over headphones.
        To capture elevation and front/back cues.
        To simulate real-ear filtering from the shape of your head and ears.
        For professional or serious spatial audio applications.
    */

    return 0;
}

afxError hrtf_load_improved(HRTFSet* set, int num_speakers, int ir_length)
{
    /*
        Improved version of your dummy HRTF loader with these features:

        What this does:
        ILD: Left and right ear gain vary with azimuth; the farther the source from the ear, the quieter it sounds there.
        Multi-tap IR: Instead of one impulse, we have 3 echoes spaced 5 samples apart, each quieter than the last.
        ITD remains as the primary spatial cue with a simple delay difference.

    */

    set->num_directions = num_speakers;
    set->ir_length = ir_length;
    set->entries = calloc(num_speakers, sizeof(HRTFEntry));

    for (int i = 0; i < num_speakers; ++i) {
        float az = 2 * AFX_PI * i / num_speakers;  // 0..2Pi
        float el = 0.0f;

        set->entries[i].azimuth = az;
        set->entries[i].elevation = el;
        set->entries[i].ir_left = calloc(ir_length, sizeof(float));
        set->entries[i].ir_right = calloc(ir_length, sizeof(float));

        // Basic ITD delay
        int delay_left = ir_length / 2 + (int)(10 * sinf(az));
        int delay_right = ir_length / 2 - (int)(10 * sinf(az));

        // Simple ILD: reduce gain on contralateral ear based on azimuth
        float ild_factor = 0.3f;  // adjust for stronger/weaker ILD
        float left_gain = 1.0f - ild_factor * fabsf(sinf(az));
        float right_gain = 1.0f - ild_factor * fabsf(sinf(az));

        // Multi-tap IR: 3 taps with decay to simulate reflections
        for (int tap = 0; tap < 3; ++tap) {
            int dl = delay_left + tap * 5;
            int dr = delay_right + tap * 5;
            float tap_gain = powf(0.6f, tap); // decay

            if (dl >= 0 && dl < ir_length)
                set->entries[i].ir_left[dl] += left_gain * tap_gain;
            if (dr >= 0 && dr < ir_length)
                set->entries[i].ir_right[dr] += right_gain * tap_gain;
        }
    }

    return 0;
}

#define NUM_AZIMUTH 16
#define NUM_ELEVATION 5  // e.g., -45°, -22.5°, 0°, 22.5°, 45°

HRTFSet* hrtf_load_spherical_grid(int ir_length)
{
    // Modify Speaker Layout to Include Elevations.

    int num_directions = NUM_AZIMUTH * NUM_ELEVATION;
    HRTFSet* set = malloc(sizeof(HRTFSet));
    set->num_directions = num_directions;
    set->ir_length = ir_length;
    set->entries = calloc(num_directions, sizeof(HRTFEntry));

    for (int el_idx = 0; el_idx < NUM_ELEVATION; ++el_idx) {
        // Map index to elevation angle in radians (-45° to +45°)
        float el = (el_idx / (float)(NUM_ELEVATION - 1)) * AFX_PI / 2 - AFX_PI / 4;

        for (int az_idx = 0; az_idx < NUM_AZIMUTH; ++az_idx) {
            float az = 2 * AFX_PI * az_idx / NUM_AZIMUTH;

            int i = el_idx * NUM_AZIMUTH + az_idx;
            set->entries[i].azimuth = az;
            set->entries[i].elevation = el;
            set->entries[i].ir_left = calloc(ir_length, sizeof(float));
            set->entries[i].ir_right = calloc(ir_length, sizeof(float));

            // Update IR Generation to Use Elevation Cues

            // Calculate ITD delays based on azimuth
            int delay_left = ir_length / 2 + (int)(10 * sinf(az));
            int delay_right = ir_length / 2 - (int)(10 * sinf(az));

            // ILD gain based on azimuth
            float ild_factor = 0.3f;
            float left_gain = 1.0f - ild_factor * fabsf(sinf(az));
            float right_gain = 1.0f - ild_factor * fabsf(sinf(az));

            // Elevation factor for frequency shaping
            float el_factor = sinf(el); // [-1,1]

            // Multi-tap IR generation with elevation-dependent EQ
            for (int tap = 0; tap < 3; ++tap) {
                int dl = delay_left + tap * 5;
                int dr = delay_right + tap * 5;

                // EQ factor: boost highs for positive elevation, attenuate for negative
                float eq_factor = 1.0f + 0.5f * el_factor * (tap / 2.0f);

                float tap_gain = powf(0.6f, tap) * eq_factor;

                if (dl >= 0 && dl < ir_length)
                    set->entries[i].ir_left[dl] += left_gain * tap_gain;
                if (dr >= 0 && dr < ir_length)
                    set->entries[i].ir_right[dr] += right_gain * tap_gain;
            }

        }
    }

    return set;
}


// Interpolating between the nearest HRTF entries to get smooth spatial transitions, 
// which is essential for realistic binaural rendering when our sound source moves.

// Implementing Directional Interpolation Between HRTFs.

/*
    Basic approach: Bilinear interpolation on spherical grid

    Given a target direction (azimuth az_t, elevation el_t):
        Find the four nearest grid points surrounding the target:
            Two elevations: el1 and el2 (just below and above el_t)
            Two azimuths for each elevation: az1 and az2 (just left and right of az_t)
        Compute interpolation weights based on the distance to each point.
        Interpolate the IRs (left and right) weighted by these factors.
*/

// Wrap azimuth in [0, 2Pi)
float wrap_azimuth(float az) {
    while (az < 0) az += 2 * AFX_PI;
    while (az >= 2 * AFX_PI) az -= 2 * AFX_PI;
    return az;
}

// Find nearest grid indices and interpolation weights
void find_interpolation_indices_weights(HRTFSet* set, float az_t, float el_t,
    int* idx_ll, int* idx_lr,
    int* idx_ul, int* idx_ur,
    float* w_ll, float* w_lr,
    float* w_ul, float* w_ur) {
    // Clamp elevation index range
    float el_min = -AFX_PI / 4;  // -45 degrees
    float el_max = AFX_PI / 4;  // +45 degrees

    int num_az = NUM_AZIMUTH;
    int num_el = NUM_ELEVATION;

    az_t = wrap_azimuth(az_t);

    // Elevation fractional index
    float el_pos = (el_t - el_min) / (el_max - el_min) * (num_el - 1);
    int el_idx_low = (int)floorf(el_pos);
    int el_idx_high = el_idx_low + 1;
    if (el_idx_high >= num_el) el_idx_high = num_el - 1;
    if (el_idx_low < 0) el_idx_low = 0;

    float el_frac = el_pos - el_idx_low;

    // Azimuth fractional index
    float az_pos = az_t / (2 * AFX_PI) * num_az;
    int az_idx_low = (int)floorf(az_pos) % num_az;
    int az_idx_high = (az_idx_low + 1) % num_az;
    float az_frac = az_pos - floorf(az_pos);

    // Calculate indices in set->entries
    *idx_ll = el_idx_low * num_az + az_idx_low;
    *idx_lr = el_idx_low * num_az + az_idx_high;
    *idx_ul = el_idx_high * num_az + az_idx_low;
    *idx_ur = el_idx_high * num_az + az_idx_high;

    // Calculate weights (bilinear interpolation)
    *w_ll = (1 - az_frac) * (1 - el_frac);
    *w_lr = az_frac * (1 - el_frac);
    *w_ul = (1 - az_frac) * el_frac;
    *w_ur = az_frac * el_frac;
}

// Interpolating the IRs

// Interpolate two IR arrays of length ir_length with weight w
void interp_ir(float* out, float const* ir, float weight, int ir_length) {
    for (int i = 0; i < ir_length; ++i)
        out[i] += weight * ir[i];
}

void interpolate_hrtf(HRTFSet* set, float az_t, float el_t, float* out_left, float* out_right) {
    int idx_ll, idx_lr, idx_ul, idx_ur;
    float w_ll, w_lr, w_ul, w_ur;
    int ir_length = set->ir_length;  // Get it from the HRTFSet struct

    // Zero output buffers
    AfxFill(out_left, 0, ir_length * sizeof(float));
    AfxFill(out_right, 0, ir_length * sizeof(float));

    find_interpolation_indices_weights(set, az_t, el_t,
        &idx_ll, &idx_lr, &idx_ul, &idx_ur,
        &w_ll, &w_lr, &w_ul, &w_ur);

    // Interpolate LEFT ear
    interp_ir(out_left, set->entries[idx_ll].ir_left, w_ll, ir_length);
    interp_ir(out_left, set->entries[idx_lr].ir_left, w_lr, ir_length);
    interp_ir(out_left, set->entries[idx_ul].ir_left, w_ul, ir_length);
    interp_ir(out_left, set->entries[idx_ur].ir_left, w_ur, ir_length);

    // Interpolate RIGHT ear
    interp_ir(out_right, set->entries[idx_ll].ir_right, w_ll, ir_length);
    interp_ir(out_right, set->entries[idx_lr].ir_right, w_lr, ir_length);
    interp_ir(out_right, set->entries[idx_ul].ir_right, w_ul, ir_length);
    interp_ir(out_right, set->entries[idx_ur].ir_right, w_ur, ir_length);
}



// Decode + Convolve to Binaural

afxError binaural_decoder_create(BinauralDecoder* dec,int order, int num_frames, HRTFSet* hrtfs) {
    int num_speakers = hrtfs->num_directions;
    int num_channels = sh_num_channels(order);

    dec->order = order;
    dec->num_channels = num_channels;
    dec->num_frames = num_frames;
    dec->ir_length = hrtfs->ir_length;
    dec->num_speakers = num_speakers;
    dec->hrtfs = hrtfs;

    dec->speaker_signals = malloc(sizeof(float*) * num_speakers);
    for (int i = 0; i < num_speakers; ++i)
        dec->speaker_signals[i] = calloc(num_frames, sizeof(float));

    return 0;
}

// Simple FIR convolution
static void convolve(const float* x, int x_len, const float* h, int h_len, float* y) {
    AfxFill(y, 0, sizeof(float) * (x_len + h_len - 1));
    for (int i = 0; i < x_len; ++i) {
        for (int j = 0; j < h_len; ++j) {
            y[i + j] += x[i] * h[j];
        }
    }
}

void binaural_decoder_process(BinauralDecoder* dec, float** ambi, float** stereo_out) {
    int N = dec->order;
    int num_frames = dec->num_frames;

    for (int s = 0; s < dec->num_speakers; ++s) {
        HRTFEntry* h = &dec->hrtfs->entries[s];
        float sh[64];
        sh_eval_sn3d(N, h->azimuth, h->elevation, sh);

        for (int i = 0; i < num_frames; ++i) {
            float sample = 0.0f;
            for (int c = 0; c < dec->num_channels; ++c) {
                sample += sh[c] * ambi[c][i];
            }
            dec->speaker_signals[s][i] = sample;
        }
    }

    int out_len = num_frames + dec->ir_length - 1;
    float* left = calloc(out_len, sizeof(float));
    float* right = calloc(out_len, sizeof(float));

    for (int s = 0; s < dec->num_speakers; ++s) {
        float* sig = dec->speaker_signals[s];
        HRTFEntry* h = &dec->hrtfs->entries[s];

        float* conv_l = calloc(out_len, sizeof(float));
        float* conv_r = calloc(out_len, sizeof(float));

        convolve(sig, num_frames, h->ir_left, dec->ir_length, conv_l);
        convolve(sig, num_frames, h->ir_right, dec->ir_length, conv_r);

        for (int i = 0; i < out_len; ++i) {
            left[i] += conv_l[i];
            right[i] += conv_r[i];
        }

        free(conv_l);
        free(conv_r);
    }

    AfxCopy(stereo_out[0], left, sizeof(float) * num_frames);
    AfxCopy(stereo_out[1], right, sizeof(float) * num_frames);

    free(left);
    free(right);
}

void binaural_decoder_destroy(BinauralDecoder* dec) {
    for (int i = 0; i < dec->num_speakers; ++i) {
        free(dec->speaker_signals[i]);
    }
    free(dec->speaker_signals);
}

void hrtf_free(HRTFSet* hrtf) {
    for (int i = 0; i < hrtf->num_directions; ++i) {
        free(hrtf->entries[i].ir_left);
        free(hrtf->entries[i].ir_right);
    }
    free(hrtf->entries);
}

void binaural_decode(BinauralDecoder* bdec, HRTFSet* hrtfs, float* mono_in, float* stereo_out, int num_samples, float az_t, float el_t) {
    // Updated binaural_decode() (simplified, block-based)
    // An example for a block of decoded mono samples.

    // This assumes mono_signal is already decoded from ambisonics to a directional mono signal(e.g., via basic decoding).
    // In real-time, azimuth and elevation would come from head-tracking or a moving source.
    // We can optimize this further by:
    // Precomputing interpolated HRTFs for known directions
    // Using FFT convolution for longer IRs



    int ir_len = hrtfs->ir_length;

    // Allocate interpolation buffers
    float* ir_left = calloc(ir_len, sizeof(float));
    float* ir_right = calloc(ir_len, sizeof(float));

    // Interpolate HRTF for desired direction
    interpolate_hrtf(hrtfs, az_t, el_t, ir_left, ir_right);

    // Clear output
    AfxFill(stereo_out, 0, sizeof(float) * num_samples * 2);

    // Convolve mono signal with interpolated HRTF IRs
    for (int n = 0; n < num_samples; ++n) {
        for (int k = 0; k < ir_len; ++k) {
            if (n - k >= 0) {
                stereo_out[2 * n + 0] += mono_in[n - k] * ir_left[k];
                stereo_out[2 * n + 1] += mono_in[n - k] * ir_right[k];
            }
        }
    }

    free(ir_left);
    free(ir_right);
}

// Add Listener Rotation to Binaural Decoding

/*
    Concept

    Your source has a direction:
    source_direction = (azimuth_source, elevation_source)

    The listener rotates their head by some yaw/pitch/roll:
    listener_orientation = (yaw, pitch, roll)

    To simulate head-relative direction:

    Final HRTF direction = inverse(listener rotation) * source direction

    Implementation (simplified: only yaw and pitch)

    Let’s assume you ignore roll for now (which is common), and just apply yaw (horizontal head turn) and pitch (looking up/down).
*/

void apply_head_rotation(float az_src, float el_src,
    float yaw, float pitch,
    float* az_out, float* el_out)
{
    // Convert source direction to 3D unit vector
    float x = cosf(el_src) * cosf(az_src);
    float y = cosf(el_src) * sinf(az_src);
    float z = sinf(el_src);

    // Apply inverse yaw rotation (about Z axis)
    float x1 = cosf(-yaw) * x - sinf(-yaw) * y;
    float y1 = sinf(-yaw) * x + cosf(-yaw) * y;
    float z1 = z;

    // Apply inverse pitch rotation (about Y axis)
    float x2 = cosf(-pitch) * x1 + sinf(-pitch) * z1;
    float y2 = y1;
    float z2 = -sinf(-pitch) * x1 + cosf(-pitch) * z1;

    // Convert back to azimuth/elevation
    *az_out = atan2f(y2, x2);       // [-Pi, Pi]
    *el_out = asinf(z2);            // [-Pi/2, Pi/2]
}

/*
    Usage in our decoder:

    float az_rel, el_rel;
    apply_head_rotation(az_src, el_src, yaw, pitch, &az_rel, &el_rel);
    interpolate_hrtf(hrtf_set, az_rel, el_rel, ir_left, ir_right);
*/

// Full 3D Head Rotation + Time-Varying Listener Orientation

// Rotation order: Yaw (Z), Pitch (Y), Roll (X)
void build_rotation_matrix(float yaw, float pitch, float roll, float R[3][3]) {
    float cy = cosf(yaw), sy = sinf(yaw);
    float cp = cosf(pitch), sp = sinf(pitch);
    float cr = cosf(roll), sr = sinf(roll);

    // R = Rz(yaw) * Ry(pitch) * Rx(roll)
    R[0][0] = cy * cp;
    R[0][1] = cy * sp * sr - sy * cr;
    R[0][2] = cy * sp * cr + sy * sr;

    R[1][0] = sy * cp;
    R[1][1] = sy * sp * sr + cy * cr;
    R[1][2] = sy * sp * cr - cy * sr;

    R[2][0] = -sp;
    R[2][1] = cp * sr;
    R[2][2] = cp * cr;
}

// Apply Inverse Rotation to Source Direction Vector

void apply_full_head_rotation(float az_src, float el_src,
    float yaw, float pitch, float roll,
    float* az_out, float* el_out)
{
    // Convert source (az, el) to 3D vector
    float x = cosf(el_src) * cosf(az_src);
    float y = cosf(el_src) * sinf(az_src);
    float z = sinf(el_src);

    // Build rotation matrix
    float R[3][3];
    build_rotation_matrix(yaw, pitch, roll, R);

    // Inverse rotation = transpose of R
    float xr = R[0][0] * x + R[1][0] * y + R[2][0] * z;
    float yr = R[0][1] * x + R[1][1] * y + R[2][1] * z;
    float zr = R[0][2] * x + R[1][2] * y + R[2][2] * z;

    // Back to az/el
    *az_out = atan2f(yr, xr);
    *el_out = asinf(fmaxf(-1.0f, fminf(1.0f, zr)));  // clamp to avoid NaN
}

// Simulate Time-Varying Head Rotation
// For example, over time t, you can animate the listener's head orientation:
/*
    float yaw   = sinf(t * 0.01f) * M_PI / 4;   // turn head side to side
    float pitch = sinf(t * 0.005f) * M_PI / 8;  // nod head up/down
    float roll  = sinf(t * 0.007f) * M_PI / 16; // tilt head

    float az_rel, el_rel;
    apply_full_head_rotation(az_src, el_src, yaw, pitch, roll, &az_rel, &el_rel);
*/
// You can update t once per audio block or sample frame.

/*
    Integrate into Decoder.

    apply_full_head_rotation(az_src, el_src, yaw, pitch, roll, &az_rel, &el_rel);
    interpolate_hrtf(hrtf_set, az_rel, el_rel, ir_left, ir_right);

    And then use that for convolution as before.
*/

// Updated Multisource Binaural Decode Function
/*
    Each sound source should have:
        Its own audio signal (e.g. a buffer or stream)
        Its own position (azimuth + elevation) that can change over time
        A per-source interpolation and convolution with the appropriate HRTF
        A final mixdown to the stereo output

    Mixing Multiple Sources

    We'll now modify the decoder to:
    For each sound source:
        Get its position at time t
        Apply head rotation to compute relative direction
        Interpolate HRTF
        Convolve its signal
    Add the stereo output to the global mix buffer
*/

void binaural_decode_multiple(BinauralDecoder* bdec, HRTFSet* hrtfs,
    SoundSource* sources, int num_sources,
    float* stereo_out, int num_samples,
    float yaw, float pitch, float roll, int t_frame)
{
    int ir_len = hrtfs->ir_length;

    float* ir_left = calloc(ir_len, sizeof(float));
    float* ir_right = calloc(ir_len, sizeof(float));
    float* temp_L = calloc(num_samples, sizeof(float));
    float* temp_R = calloc(num_samples, sizeof(float));

    // Clear stereo out
    AfxFill(stereo_out, 0, sizeof(float) * num_samples * 2);

    for (int s = 0; s < num_sources; ++s) {
        SoundSource* src = &sources[s];

        // Optional: animate source position
        // update_position_func(t_frame, &src->azimuth, &src->elevation);

        float az_rel, el_rel;
        apply_full_head_rotation(src->azimuth, src->elevation,
            yaw, pitch, roll,
            &az_rel, &el_rel);

        interpolate_hrtf(hrtfs, az_rel, el_rel, ir_left, ir_right);

        // Clear temp stereo buffer
        AfxFill(temp_L, 0, sizeof(float) * num_samples);
        AfxFill(temp_R, 0, sizeof(float) * num_samples);

        // Convolve source mono with IRs
        for (int n = 0; n < num_samples; ++n) {
            for (int k = 0; k < ir_len; ++k) {
                if (n - k >= 0 && n - k < src->length) {
                    temp_L[n] += src->buffer[n - k] * ir_left[k];
                    temp_R[n] += src->buffer[n - k] * ir_right[k];
                }
            }
        }

        // Add to final output mix
        for (int n = 0; n < num_samples; ++n) {
            stereo_out[2 * n + 0] += temp_L[n];
            stereo_out[2 * n + 1] += temp_R[n];
        }
    }

    free(ir_left);
    free(ir_right);
    free(temp_L);
    free(temp_R);
}

/*
    Example: Animate Source Positions Over Time

    for (int frame = 0; frame < num_frames; ++frame) {
    float t = frame * block_duration;

    sources[0].azimuth = sinf(t * 0.2f) * M_PI / 3;
    sources[0].elevation = 0.1f * sinf(t * 0.1f);

    sources[1].azimuth = -cosf(t * 0.3f) * M_PI / 2;
    sources[1].elevation = 0;

    binaural_decode_multiple(decoder, hrtfs, sources, num_sources,
                             stereo_output, block_size,
                             yaw, pitch, roll, frame);
}
*/

// Add Spatial Panning Fallback

/*
    When Do We Use Fallback?

    The requested direction is outside the HRTF set's elevation range.
    Or HRTF interpolation isn't desirable (e.g. fallback mode, performance constraints).
    Or you’re simulating mono/stereo sources with just azimuth-based panning

    Strategy

    We’ll use equal-power panning based on azimuth:
    Left gain = cos(azimuth)
    Right gain = sin(azimuth)
    Normalize to maintain perceived loudness
    Optional: Use V curve for more perceptually linear panning.
*/

void spatial_pan_fallback(float* mono_in, int num_samples, float azimuth, float* out_left, float* out_right) {
    float gain_l = cosf(azimuth);
    float gain_r = sinf(azimuth);

    // Normalize for equal power
    float norm = sqrtf(gain_l * gain_l + gain_r * gain_r);
    gain_l /= norm;
    gain_r /= norm;

    for (int i = 0; i < num_samples; ++i) {
        out_left[i] = mono_in[i] * gain_l;
        out_right[i] = mono_in[i] * gain_r;
    }
}

/*
    Integrate into Binaural Decode

    Modify your interpolate_hrtf() call site to detect fallback condition.
    For example:

    bool use_fallback = (el_rel < -M_PI/4 || el_rel > M_PI/4);  // elevation out of bounds

    if (use_fallback) {
        spatial_pan_fallback(src->buffer, num_samples, az_rel, temp_L, temp_R);
    } else {
        interpolate_hrtf(hrtf_set, az_rel, el_rel, ir_left, ir_right);
        // convolution code as before...
    }

    This keeps audio flowing even if HRTF interpolation fails.

    Fallback in a Multisource Scene

    Each source can individually fall back:

    for (int s = 0; s < num_sources; ++s) {
        ...
        bool fallback = (el_rel < min_el || el_rel > max_el);

        if (fallback) {
            spatial_pan_fallback(src->buffer, num_samples, az_rel, temp_L, temp_R);
        } else {
            interpolate_hrtf(...);
            convolve(...);
        }

        mix_to_output(...);
    }


*/

// Early Reflections & Reverb

