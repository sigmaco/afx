
#define _AMX_C
#define _AMX_CURVE_C
#include "../../dev/AmxImplKit.h"

void CubicCoefficients(const afxReal ti_3, const afxReal ti_2, const afxReal ti_1, const afxReal ti, const afxReal ti1, const afxReal ti2, const afxReal t, afxReal *ci_3, afxReal *ci_2, afxReal *ci_1, afxReal *ci)
{
    double v11; // st7
    double v12; // st6
    afxReal L0; // ST00_4
    afxReal L1_1; // ST04_4
    afxReal L1_2; // ST08_4
    double v16; // st6
    double v17; // st5
    double v18; // st4
    double v19; // st5
    afxReal ti_2a; // [esp+14h] [ebp+8h]
    afxReal L2_3; // [esp+18h] [ebp+Ch]
    afxReal tia; // [esp+1Ch] [ebp+10h]
    afxReal ta; // [esp+28h] [ebp+1Ch]

    v11 = t - ti_1;
    v12 = t - ti_2;
    L0 = v11 / (ti - ti_1);
    L1_1 = v12 / (ti - ti_2);
    L1_2 = v11 / (ti1 - ti_1);
    ta = (t - ti_3) / (ti - ti_3);
    v16 = v12 / (ti1 - ti_2);
    ti_2a = v11 / (ti2 - ti_1);
    v17 = 1.0 - L0;
    v18 = v17;
    v19 = (1.0 - L1_1) * v17;
    tia = L1_2 * L0;
    *ci_3 = (1.0 - ta) * v19;
    L2_3 = (1.0 - L1_2) * L0 + v18 * L1_1;
    *ci_2 = (1.0 - v16) * L2_3 + v19 * ta;
    *ci_1 = (1.0 - ti_2a) * tia + L2_3 * v16;
    *ci = tia * ti_2a;
}

void SampleBSpline0x1(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    *ResultAliased = *piAliased;
}

void SampleBSpline0x2(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    *ResultAliased = *piAliased;
    ResultAliased[1] = piAliased[1];
}

void SampleBSpline0x3(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    *ResultAliased = *piAliased;
    ResultAliased[1] = piAliased[1];
    ResultAliased[2] = piAliased[2];
}

void SampleBSpline0x4(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    *ResultAliased = *piAliased;
    ResultAliased[1] = piAliased[1];
    ResultAliased[2] = piAliased[2];
    ResultAliased[3] = piAliased[3];
}

void SampleBSpline0x9(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    *ResultAliased = *piAliased;
    ResultAliased[1] = piAliased[1];
    ResultAliased[2] = piAliased[2];
    ResultAliased[3] = piAliased[3];
    ResultAliased[4] = piAliased[4];
    ResultAliased[5] = piAliased[5];
    ResultAliased[6] = piAliased[6];
    ResultAliased[7] = piAliased[7];
    ResultAliased[8] = piAliased[8];
}

void SampleBSpline1x1(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    double v4; // st7

    v4 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v4) * *(piAliased - 1) + v4 * *piAliased;
}

void SampleBSpline1x2(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    double v4; // st7

    v4 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v4) * *(piAliased - 2) + v4 * *piAliased;
    ResultAliased[1] = (1.0 - v4) * *(piAliased - 1) + v4 * piAliased[1];
}

void SampleBSpline1x3(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    double v4; // st7

    v4 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v4) * *(piAliased - 3) + v4 * *piAliased;
    ResultAliased[1] = (1.0 - v4) * *(piAliased - 2) + v4 * piAliased[1];
    ResultAliased[2] = (1.0 - v4) * *(piAliased - 1) + v4 * piAliased[2];
}

void SampleBSpline1x3n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // ecx
    afxReal *v5; // eax
    double v6; // st7
    double v7; // st7
    double v8; // st4

    v4 = piAliased;
    v5 = ResultAliased;
    v6 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v6) * *(piAliased - 3) + v6 * *piAliased;
    ResultAliased[1] = (1.0 - v6) * *(v4 - 2) + v6 * v4[1];
    v7 = (1.0 - v6) * *(v4 - 1) + v6 * v4[2];
    ResultAliased[2] = v7;
    v8 = (3.0 - (*ResultAliased * *ResultAliased + v5[1] * v5[1] + v7 * v7)) * 0.5;
    *v5 = v8 * *v5;
    ResultAliased[1] = v8 * ResultAliased[1];
    ResultAliased[2] = v8 * ResultAliased[2];
}

void SampleBSpline1x4(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    double v4; // st7

    v4 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v4) * *(piAliased - 4) + v4 * *piAliased;
    ResultAliased[1] = (1.0 - v4) * *(piAliased - 3) + v4 * piAliased[1];
    ResultAliased[2] = (1.0 - v4) * *(piAliased - 2) + v4 * piAliased[2];
    ResultAliased[3] = (1.0 - v4) * *(piAliased - 1) + v4 * piAliased[3];
}

void SampleBSpline1x4n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // ecx
    afxReal *v5; // eax
    double v6; // st7
    double v7; // st7
    long double v8; // st3

    v4 = piAliased;
    v5 = ResultAliased;
    v6 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v6) * *(piAliased - 4) + v6 * *piAliased;
    ResultAliased[1] = (1.0 - v6) * *(v4 - 3) + v6 * v4[1];
    ResultAliased[2] = (1.0 - v6) * *(v4 - 2) + v6 * v4[2];
    v7 = (1.0 - v6) * *(v4 - 1) + v6 * v4[3];
    ResultAliased[3] = v7;
    v8 = sqrt(*ResultAliased * *ResultAliased + ResultAliased[1] * ResultAliased[1] + v5[2] * v5[2] + v7 * v7);
    *v5 = 1.0 / v8 * *v5;
    v5[1] = 1.0 / v8 * v5[1];
    ResultAliased[2] = 1.0 / v8 * ResultAliased[2];
    ResultAliased[3] = 1.0 / v8 * ResultAliased[3];
}

void SampleBSpline1x9(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7

    v4 = piAliased;
    v5 = (t - *(tiAliased - 1)) / (*tiAliased - *(tiAliased - 1));
    *ResultAliased = (1.0 - v5) * *(piAliased - 9) + v5 * *piAliased;
    ResultAliased[1] = (1.0 - v5) * *(v4 - 8) + v5 * v4[1];
    ResultAliased[2] = (1.0 - v5) * *(v4 - 7) + v5 * v4[2];
    ResultAliased[3] = (1.0 - v5) * *(v4 - 6) + v5 * v4[3];
    ResultAliased[4] = (1.0 - v5) * *(v4 - 5) + v5 * v4[4];
    ResultAliased[5] = (1.0 - v5) * *(v4 - 4) + v5 * v4[5];
    ResultAliased[6] = (1.0 - v5) * *(v4 - 3) + v5 * v4[6];
    ResultAliased[7] = (1.0 - v5) * *(piAliased - 2) + v5 * piAliased[7];
    ResultAliased[8] = (1.0 - v5) * *(piAliased - 1) + v5 * piAliased[8];
}

void SampleBSpline2x1(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    double v6; // st6
    double v7; // st5
    afxReal v8; // ST00_4
    double v9; // st7
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = *(tiAliased - 1);
    v7 = *(tiAliased - 2);
    v8 = t - v6;
    tiAliaseda = v8 / (v5 - v6);
    ta = tiAliaseda + (t - v7) / (v5 - v7) - (t - v7) / (v5 - v7) * tiAliaseda;
    v9 = v8 / (v4[1] - v6) * tiAliaseda;
    *ResultAliased = (1.0 - ta) * *(piAliased - 2) + (ta - v9) * *(piAliased - 1) + v9 * *piAliased;
}

void SampleBSpline2x2(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    double v6; // st6
    double v7; // st5
    afxReal v8; // ST00_4
    double v9; // st7
    double v10; // st7
    double v11; // st6
    double v12; // st5
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = *(tiAliased - 1);
    v7 = *(tiAliased - 2);
    v8 = t - v6;
    tiAliaseda = v8 / (v5 - v6);
    v9 = (t - v7) / (v5 - v7);
    ta = tiAliaseda + v9 - v9 * tiAliaseda;
    v10 = v8 / (v4[1] - v6) * tiAliaseda;
    v11 = ta - v10;
    v12 = 1.0 - ta;
    *ResultAliased = v12 * *(piAliased - 4) + v11 * *(piAliased - 2) + v10 * *piAliased;
    ResultAliased[1] = v12 * *(piAliased - 3) + v11 * *(piAliased - 1) + v10 * piAliased[1];
}

void SampleBSpline2x3(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    double v6; // st6
    double v7; // st5
    afxReal v8; // ST00_4
    double v9; // st7
    double v10; // st7
    double v11; // st6
    double v12; // st5
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = *(tiAliased - 1);
    v7 = *(tiAliased - 2);
    v8 = t - v6;
    tiAliaseda = v8 / (v5 - v6);
    v9 = (t - v7) / (v5 - v7);
    ta = tiAliaseda + v9 - v9 * tiAliaseda;
    v10 = v8 / (v4[1] - v6) * tiAliaseda;
    v11 = ta - v10;
    v12 = 1.0 - ta;
    *ResultAliased = v12 * *(piAliased - 6) + v11 * *(piAliased - 3) + v10 * *piAliased;
    ResultAliased[1] = v12 * *(piAliased - 5) + v11 * *(piAliased - 2) + v10 * piAliased[1];
    ResultAliased[2] = v12 * *(piAliased - 4) + v11 * *(piAliased - 1) + v10 * piAliased[2];
}

void SampleBSpline2x3n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    const afxReal *v6; // ecx
    double v7; // st6
    double v8; // st5
    afxReal v9; // ST00_4
    double v10; // st7
    double v11; // rtt
    double v12; // st6
    afxReal *v13; // eax
    double v14; // st7
    double v15; // st6
    double v16; // st5
    double v17; // st7
    double v18; // st4
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = piAliased;
    v7 = *(tiAliased - 1);
    v8 = *(tiAliased - 2);
    v9 = t - v7;
    tiAliaseda = v9 / (v5 - v7);
    v10 = (t - v8) / (v5 - v8);
    v11 = v7;
    ta = tiAliaseda + v10 - v10 * tiAliaseda;
    v12 = v4[1];
    v13 = ResultAliased;
    v14 = v9 / (v12 - v11) * tiAliaseda;
    v15 = ta - v14;
    v16 = 1.0 - ta;
    *ResultAliased = v16 * *(piAliased - 6) + v15 * *(piAliased - 3) + v14 * *piAliased;
    ResultAliased[1] = v16 * *(v6 - 5) + v15 * *(v6 - 2) + v14 * v6[1];
    v17 = v16 * *(v6 - 4) + v15 * *(v6 - 1) + v14 * v6[2];
    ResultAliased[2] = v17;
    v18 = (3.0 - (*ResultAliased * *ResultAliased + v13[1] * v13[1] + v17 * v17)) * 0.5;
    *v13 = v18 * *v13;
    ResultAliased[1] = v18 * ResultAliased[1];
    ResultAliased[2] = v18 * ResultAliased[2];
}

void SampleBSpline2x4(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    double v6; // st6
    double v7; // st5
    afxReal v8; // ST00_4
    double v9; // st7
    double v10; // rtt
    double v11; // st6
    const afxReal *v12; // eax
    double v13; // st7
    double v14; // st6
    double v15; // st5
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = *(tiAliased - 1);
    v7 = *(tiAliased - 2);
    v8 = t - v6;
    tiAliaseda = v8 / (v5 - v6);
    v9 = (t - v7) / (v5 - v7);
    v10 = v6;
    ta = tiAliaseda + v9 - v9 * tiAliaseda;
    v11 = v4[1];
    v12 = piAliased;
    v13 = v8 / (v11 - v10) * tiAliaseda;
    v14 = ta - v13;
    v15 = 1.0 - ta;
    *ResultAliased = v15 * *(piAliased - 8) + v14 * *(piAliased - 4) + v13 * *piAliased;
    ResultAliased[1] = v15 * *(v12 - 7) + v14 * *(v12 - 3) + v13 * v12[1];
    ResultAliased[2] = v15 * *(v12 - 6) + v14 * *(v12 - 2) + v13 * piAliased[2];
    ResultAliased[3] = v15 * *(piAliased - 5) + v14 * *(piAliased - 1) + v13 * piAliased[3];
}

void SampleBSpline2x4n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    const afxReal *v6; // ecx
    double v7; // st6
    double v8; // st5
    afxReal v9; // ST00_4
    double v10; // st7
    double v11; // rtt
    double v12; // st6
    afxReal *v13; // eax
    double v14; // st7
    double v15; // st6
    double v16; // st5
    double v17; // st7
    long double v18; // st3
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = piAliased;
    v7 = *(tiAliased - 1);
    v8 = *(tiAliased - 2);
    v9 = t - v7;
    tiAliaseda = v9 / (v5 - v7);
    v10 = (t - v8) / (v5 - v8);
    v11 = v7;
    ta = tiAliaseda + v10 - v10 * tiAliaseda;
    v12 = v4[1];
    v13 = ResultAliased;
    v14 = v9 / (v12 - v11) * tiAliaseda;
    v15 = ta - v14;
    v16 = 1.0 - ta;
    *ResultAliased = v16 * *(piAliased - 8) + v15 * *(piAliased - 4) + v14 * *piAliased;
    ResultAliased[1] = v16 * *(v6 - 7) + v15 * *(v6 - 3) + v14 * v6[1];
    ResultAliased[2] = v16 * *(v6 - 6) + v15 * *(v6 - 2) + v14 * v6[2];
    v17 = v16 * *(v6 - 5) + v15 * *(v6 - 1) + v14 * v6[3];
    ResultAliased[3] = v17;
    v18 = sqrt(*ResultAliased * *ResultAliased + ResultAliased[1] * ResultAliased[1] + v13[2] * v13[2] + v17 * v17);
    *v13 = 1.0 / v18 * *v13;
    v13[1] = 1.0 / v18 * v13[1];
    ResultAliased[2] = 1.0 / v18 * ResultAliased[2];
    ResultAliased[3] = 1.0 / v18 * ResultAliased[3];
}

void SampleBSpline2x9(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    double v5; // st7
    double v6; // st6
    double v7; // st5
    afxReal v8; // ST00_4
    double v9; // st7
    double v10; // rtt
    double v11; // st6
    const afxReal *v12; // eax
    double v13; // st7
    double v14; // st6
    double v15; // st5
    afxReal tiAliaseda; // [esp+8h] [ebp+4h]
    afxReal ta; // [esp+10h] [ebp+Ch]

    v4 = tiAliased;
    v5 = *tiAliased;
    v6 = *(tiAliased - 1);
    v7 = *(tiAliased - 2);
    v8 = t - v6;
    tiAliaseda = v8 / (v5 - v6);
    v9 = (t - v7) / (v5 - v7);
    v10 = v6;
    ta = tiAliaseda + v9 - v9 * tiAliaseda;
    v11 = v4[1];
    v12 = piAliased;
    v13 = v8 / (v11 - v10) * tiAliaseda;
    v14 = ta - v13;
    v15 = 1.0 - ta;
    *ResultAliased = v15 * *(piAliased - 18) + v14 * *(piAliased - 9) + v13 * *piAliased;
    ResultAliased[1] = v15 * *(v12 - 17) + v14 * *(v12 - 8) + v13 * v12[1];
    ResultAliased[2] = v15 * *(v12 - 16) + v14 * *(v12 - 7) + v13 * v12[2];
    ResultAliased[3] = v15 * *(v12 - 15) + v14 * *(v12 - 6) + v13 * v12[3];
    ResultAliased[4] = v15 * *(v12 - 14) + v14 * *(v12 - 5) + v13 * v12[4];
    ResultAliased[5] = v15 * *(v12 - 13) + v14 * *(v12 - 4) + v13 * v12[5];
    ResultAliased[6] = v15 * *(v12 - 12) + v14 * *(v12 - 3) + v13 * v12[6];
    ResultAliased[7] = v15 * *(v12 - 11) + v14 * *(v12 - 2) + v13 * piAliased[7];
    ResultAliased[8] = v15 * *(piAliased - 10) + v14 * *(piAliased - 1) + v13 * piAliased[8];
}

void SampleBSpline3x1(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 3)
        + c2 * *(piAliased - 2)
        + c1 * *(piAliased - 1)
        + c0 * *piAliased;
}

void SampleBSpline3x2(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 6)
        + c2 * *(piAliased - 4)
        + c1 * *(piAliased - 2)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 5) + c2 * *(v4 - 3) + c1 * *(v4 - 1) + c0 * v4[1];
}

void SampleBSpline3x3(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 9)
        + c2 * *(piAliased - 6)
        + c1 * *(piAliased - 3)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 8) + c2 * *(v4 - 5) + c1 * *(v4 - 2) + c0 * v4[1];
    v5[2] = *(afxReal *)&tiAliased * *(v4 - 7) + c2 * *(v4 - 4) + c1 * *(v4 - 1) + c0 * v4[2];
}

void SampleBSpline3x3n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    double v6; // st7
    double v7; // st4
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 9)
        + c2 * *(piAliased - 6)
        + c1 * *(piAliased - 3)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 8) + c2 * *(v4 - 5) + c1 * *(v4 - 2) + c0 * v4[1];
    v6 = *(afxReal *)&tiAliased * *(v4 - 7) + c2 * *(v4 - 4) + c1 * *(v4 - 1) + c0 * v4[2];
    v5[2] = v6;
    v7 = (3.0 - (*v5 * *v5 + v5[1] * v5[1] + v6 * v6)) * 0.5;
    *v5 = v7 * *v5;
    v5[1] = v7 * v5[1];
    v5[2] = v7 * v5[2];
}

void SampleBSpline3x4(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 12)
        + c2 * *(piAliased - 8)
        + c1 * *(piAliased - 4)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 11) + c2 * *(v4 - 7) + c1 * *(v4 - 3) + c0 * v4[1];
    v5[2] = *(afxReal *)&tiAliased * *(v4 - 10) + c2 * *(v4 - 6) + c1 * *(v4 - 2) + c0 * v4[2];
    v5[3] = *(afxReal *)&tiAliased * *(v4 - 9) + c2 * *(v4 - 5) + c1 * *(v4 - 1) + c0 * v4[3];
}

void SampleBSpline3x4n(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    double v6; // st7
    long double v7; // st3
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 12)
        + c2 * *(piAliased - 8)
        + c1 * *(piAliased - 4)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 11) + c2 * *(v4 - 7) + c1 * *(v4 - 3) + c0 * v4[1];
    v5[2] = *(afxReal *)&tiAliased * *(v4 - 10) + c2 * *(v4 - 6) + c1 * *(v4 - 2) + c0 * v4[2];
    v6 = *(afxReal *)&tiAliased * *(v4 - 9) + c2 * *(v4 - 5) + c1 * *(v4 - 1) + c0 * v4[3];
    v5[3] = v6;
    v7 = sqrt(*v5 * *v5 + v5[1] * v5[1] + v5[2] * v5[2] + v6 * v6);
    *v5 = 1.0 / v7 * *v5;
    v5[1] = 1.0 / v7 * v5[1];
    v5[2] = 1.0 / v7 * v5[2];
    v5[3] = 1.0 / v7 * v5[3];
}

void SampleBSpline3x9(const afxReal *tiAliased, const afxReal *piAliased, afxReal t, afxReal *ResultAliased)
{
    const afxReal *v4; // eax
    afxReal *v5; // ecx
    afxReal c2; // [esp+0h] [ebp-Ch]
    afxReal c1; // [esp+4h] [ebp-8h]
    afxReal c0; // [esp+8h] [ebp-4h]

    CubicCoefficients(
        *(tiAliased - 3),
        *(tiAliased - 2),
        *(tiAliased - 1),
        *tiAliased,
        tiAliased[1],
        tiAliased[2],
        t,
        (afxReal *)&tiAliased,
        &c2,
        &c1,
        &c0);
    v4 = piAliased;
    v5 = ResultAliased;
    *ResultAliased = *(afxReal *)&tiAliased * *(piAliased - 27)
        + c2 * *(piAliased - 18)
        + c1 * *(piAliased - 9)
        + c0 * *piAliased;
    v5[1] = *(afxReal *)&tiAliased * *(v4 - 26) + c2 * *(v4 - 17) + c1 * *(v4 - 8) + c0 * v4[1];
    v5[2] = *(afxReal *)&tiAliased * *(v4 - 25) + c2 * *(v4 - 16) + c1 * *(v4 - 7) + c0 * v4[2];
    v5[3] = *(afxReal *)&tiAliased * *(v4 - 24) + c2 * *(v4 - 15) + c1 * *(v4 - 6) + c0 * v4[3];
    v5[4] = *(afxReal *)&tiAliased * *(v4 - 23) + c2 * *(v4 - 14) + c1 * *(v4 - 5) + c0 * v4[4];
    v5[5] = *(afxReal *)&tiAliased * *(v4 - 22) + c2 * *(v4 - 13) + c1 * *(v4 - 4) + c0 * v4[5];
    v5[6] = *(afxReal *)&tiAliased * *(v4 - 21) + c2 * *(v4 - 12) + c1 * *(v4 - 3) + c0 * v4[6];
    v5[7] = *(afxReal *)&tiAliased * *(v4 - 20) + c2 * *(v4 - 11) + c1 * *(v4 - 2) + c0 * v4[7];
    v5[8] = *(afxReal *)&tiAliased * *(v4 - 19) + c2 * *(v4 - 10) + c1 * *(v4 - 1) + c0 * v4[8];
}

void SampleBSpline(int Degree, int Dimension, afxBool Normalize, const afxReal *ti, const afxReal *pi, afxReal t, afxReal *Result)
{
    afxError err;

    static void(*Evaluators[4][10])(const afxReal *, const afxReal *, afxReal, afxReal *) =
    {
      {
        NULL,
        SampleBSpline0x1,
        SampleBSpline0x2,
        SampleBSpline0x3,
        SampleBSpline0x4,
        NULL,
        NULL,
        NULL,
        NULL,
        SampleBSpline0x9
      },
      {
        NULL,
        SampleBSpline1x1,
        SampleBSpline1x2,
        SampleBSpline1x3,
        SampleBSpline1x4,
        NULL,
        NULL,
        NULL,
        NULL,
        SampleBSpline1x9
      },
      {
        NULL,
        SampleBSpline2x1,
        SampleBSpline2x2,
        SampleBSpline2x3,
        SampleBSpline2x4,
        NULL,
        NULL,
        NULL,
        NULL,
        SampleBSpline2x9
      },
      {
        NULL,
        SampleBSpline3x1,
        SampleBSpline3x2,
        SampleBSpline3x3,
        SampleBSpline3x4,
        NULL,
        NULL,
        NULL,
        NULL,
        SampleBSpline3x9
      }
    };

    static void(*NormalizedEvaluators[4][10])(const afxReal *, const afxReal *, afxReal, afxReal *) =
    {
      {
        NULL,
        SampleBSpline0x1,
        SampleBSpline0x2,
        SampleBSpline0x3,
        SampleBSpline0x4,
        NULL,
        NULL,
        NULL,
        NULL,
        SampleBSpline0x9
      },
      {
        NULL,
        NULL,
        NULL,
        SampleBSpline1x3n,
        SampleBSpline1x4n,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
      },
      {
        NULL,
        NULL,
        NULL,
        SampleBSpline2x3n,
        SampleBSpline2x4n,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
      },
      {
        NULL,
        NULL,
        NULL,
        SampleBSpline3x3n,
        SampleBSpline3x4n,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
      }
    };

    AFX_ASSERT(!(Degree < 0 || Degree >= 4));
    AFX_ASSERT(Dimension >= 0 && Dimension < 10);
    afxInt v7 = Dimension + 10 * Degree;

    if (Normalize)
    {
        if (NormalizedEvaluators[0][v7])
            NormalizedEvaluators[0][v7](ti, pi, t, Result);
        else
            AfxThrowError();
    }
    else
    {
        if (Evaluators[0][v7])
            Evaluators[0][v7](ti, pi, t, Result);
        else
            AfxThrowError();
    }
}

char ConstructBSplineBuffers(int Dimension, afxCurve PrevCurve, afxCurve Curve, afxCurve NextCurve, float PrevCurveDuration, float CurveDuration, float NextCurveDuration, int KnotIndex, float *ti, float *pi, float **tiPtr, float **piPtr, const float *IdentityVector)
{
    int v13; // edi
    int v14; // eax
    int v15; // esi
    int v16; // ebp
    afxBool v17; // dl
    afxBool v18; // bl
    afxCurve v19; // eax
    int v21; // ebx
    int v22; // ebp
    float *v23; // edx
    float *v24; // eax
    int v25; // ecx
    int v26; // ebp
    afxBool v27; // zf
    float *v28; // ecx
    double v29; // st7
    const float *v30; // eax
    int v31; // edx
    int v32; // ebp
    float *v33; // edi
    float *v34; // ecx
    float *v35; // eax
    int v36; // ebx
    int v37; // eax
    int v38; // ebp
    float *v39; // edi
    double v40; // st7
    float *v41; // ecx
    float *v42; // edi
    const float *v43; // eax
    int v44; // ecx
    int v45; // edx
    int v46; // eax
    int v47; // eax
    float *v48; // edi
    float *v49; // ebp
    int v50; // ebx
    double v51; // st7
    afxBool Underflow; // [esp+13h] [ebp-15h]
    int NumCurveKnots; // [esp+14h] [ebp-14h]
    signed int TimeOffset; // [esp+18h] [ebp-10h]
    int TimeOffseta; // [esp+18h] [ebp-10h]
    int TimeOffsetb; // [esp+18h] [ebp-10h]
    float TimeOffsetc; // [esp+18h] [ebp-10h]
    int KnotCount; // [esp+1Ch] [ebp-Ch]
    float *tiDst; // [esp+20h] [ebp-8h]
    float *tiDsta; // [esp+20h] [ebp-8h]
    int v61; // [esp+24h] [ebp-4h]
    float *Curvea; // [esp+34h] [ebp+Ch]
    float *Curveb; // [esp+34h] [ebp+Ch]
    afxCurve Curvec; // [esp+34h] [ebp+Ch]
    float *Curved; // [esp+34h] [ebp+Ch]
    afxCurve Curvee; // [esp+34h] [ebp+Ch]
    float Curvef; // [esp+34h] [ebp+Ch]
    afxBool KnotIndexa; // [esp+48h] [ebp+20h]
    int KnotIndexb; // [esp+48h] [ebp+20h]
    int KnotIndexc; // [esp+48h] [ebp+20h]

    v13 = AfxGetCurveDegree(Curve);
    v14 = AfxCountCurveKnots(Curve);

    if (v14 > 0 && NextCurve)
        --v14;
    v15 = KnotIndex;
    v16 = KnotIndex - v13;
    v17 = KnotIndex - v13 < 0;
    v18 = 2 * v13 + KnotIndex - v13 > v14;
    KnotCount = 2 * v13;
    Underflow = KnotIndex - v13 < 0;
    KnotIndexa = 2 * v13 + KnotIndex - v13 > v14;
    if (!v17 && !v18)
    {
        v19 = (Curve->fmt == afxCurveFormat_DaK32fC32f) ? Curve : NIL;
        if (v19)
        {
            *tiPtr = &v19->knots[v15];
            *piPtr = &v19->ctrls[v15 * Dimension];
        }
        else
        {
            *tiPtr = &ti[v13];
            *piPtr = &pi[Dimension * v13];
            AfxExtractCurveKnotValues(Curve, v16, KnotCount, ti, pi, IdentityVector);
        }
        return 0;
    }
    *tiPtr = &ti[v13];
    *piPtr = &pi[Dimension * v13];
    v21 = 0;
    NumCurveKnots = 2 * v13;
    TimeOffset = 0;
    if (v17)
    {
        v22 = -v16;
        v21 = v22;
        NumCurveKnots = 2 * v13 - v22;
        TimeOffset = v22;
        v16 = 0;
    }
    if (KnotIndexa)
        NumCurveKnots = v14 - v16;
    if (NumCurveKnots > 0)
    {
        AfxExtractCurveKnotValues(Curve, v16, NumCurveKnots, &ti[v21], &pi[Dimension * v21], IdentityVector);
        v17 = Underflow;
    }
    if (v17)
    {
        if (PrevCurve)
        {
            if (AfxIsCurveIdentity(PrevCurve))
            {
                v28 = ti;
                Curveb = pi;
                v29 = ti[v21] - (double)TimeOffset * PrevCurveDuration;
                if (v21 > 0)
                {
                    TimeOffsetb = v21;
                    do
                    {
                        *v28 = v29;
                        ++v28;
                        v30 = IdentityVector;
                        if (Dimension)
                        {
                            afxUnit i = 0;
                            v31 = Dimension;
                            do
                            {
                                //Curveb[v31] = v30[i];
                                *(float *)((char *)v30 + ((char *)Curveb - (char *)IdentityVector)) = *v30;
                                ++v30;
                                --v31;
                                ++i;
                            } while (v31);
                        }
                        v29 = v29 + PrevCurveDuration;
                        v27 = TimeOffsetb == 1;
                        Curveb += Dimension;
                        --TimeOffsetb;
                    } while (!v27);
                }
            }
            else
            {
                v37 = AfxCountCurveKnots(PrevCurve);

                if (v37 > 1)
                    --v37;

                v38 = v37 - 1;
                TimeOffsetc = -PrevCurveDuration;
                v39 = &ti[v21 - 1];
                v61 = v37 - 1;
                Curved = &pi[Dimension * (v21 - 1)];
                if (v21 - 1 >= 0)
                {
                    tiDst = (float *)v21;
                    do
                    {
                        AfxExtractCurveKnotValues(PrevCurve, v38, 1, v39, Curved, IdentityVector);
                        *v39 = TimeOffsetc + *v39;
                        --v39;
                        --v38;
                        Curved -= Dimension;
                        if (v38 < 0)
                        {
                            v38 = v61;
                            TimeOffsetc = TimeOffsetc - PrevCurveDuration;
                        }
                        tiDst = (float *)((char *)tiDst - 1);
                    } while (tiDst);
                }
            }
        }
        else
        {
            v23 = ti;
            Curvea = pi;
            v24 = &pi[Dimension * v21];
            if (v21 > 0)
            {
                TimeOffseta = v21;
                do
                {
                    *v23 = ti[v21];
                    ++v23;
                    if (Dimension)
                    {
                        v25 = (char *)Curvea - (char *)v24;
                        v26 = Dimension;
                        do
                        {
                            *(float *)((char *)v24 + v25) = *v24;
                            ++v24;
                            --v26;
                        } while (v26);
                        v24 = &pi[Dimension * v21];
                    }
                    v27 = TimeOffseta == 1;
                    Curvea += Dimension;
                    --TimeOffseta;
                } while (!v27);
            }
        }
    }
    if (!KnotIndexa)
        return 1;
    v32 = v21 + NumCurveKnots - 1;
    if (!NextCurve)
    {
        v33 = &pi[Dimension * v32];
        v34 = &ti[v32 + 1];
        Curvec = (afxCurve)&v33[Dimension];
        if (v21 + NumCurveKnots < KnotCount)
        {
            KnotIndexb = KnotCount - (v21 + NumCurveKnots);
            do
            {
                *v34 = ti[v32];
                ++v34;
                v35 = &pi[Dimension * v32];
                if (Dimension)
                {
                    afxUnit i = 0;
                    v36 = Dimension;
                    do
                    {
                        *(float *)((char *)v35 + ((char *)Curvec - (char *)v33)) = *v35;
                        ++v35;
                        --v36;
                        ++i;
                    } while (v36);
                }
                v27 = KnotIndexb == 1;
                Curvec = (afxCurve)((char *)Curvec + 4 * Dimension);
                --KnotIndexb;
            } while (!v27);
            return 1;
        }
        return 1;
    }
    if (AfxIsCurveIdentity(NextCurve))
    {
        v40 = ti[v32];
        v41 = &pi[Dimension * (v21 + NumCurveKnots)];
        v42 = &ti[v32 + 1];
        Curvee = (afxCurve)&pi[Dimension * (v21 + NumCurveKnots)];
        if (v21 > 0)
        {
            do
            {
                v40 = v40 + NextCurveDuration;
                v43 = IdentityVector;
                ++v42;
                *(v42 - 1) = v40;
                if (Dimension)
                {
                    v44 = (char *)v41 - (char *)IdentityVector;
                    v45 = Dimension;
                    do
                    {
                        *(float *)((char *)v43 + v44) = *v43;
                        ++v43;
                        --v45;
                    } while (v45);
                }
                //v41 = (float *)(&Curvee->CurveData.Type + Dimension);//v41 = (float *)(&Curvee->CurveData.Type + Dimension);
                --v21;
                Curvee = (afxCurve)((char *)Curvee + 4 * Dimension);
            } while (v21);
        }
        return 1;
    }
    v46 = AfxCountCurveKnots(NextCurve);
    KnotIndexc = v46;
    if (v46 > 1)
        KnotIndexc = v46 - 1;
    v47 = v21 + NumCurveKnots;
    v48 = &ti[v32 + 1];
    v49 = &pi[Dimension * (v21 + NumCurveKnots)];
    v50 = 0;
    Curvef = CurveDuration;
    if (v47 >= KnotCount)
        return 1;
    tiDsta = (float *)(KnotCount - v47);
    do
    {
        AfxExtractCurveKnotValues(NextCurve, v50, 1, v48, v49, IdentityVector);
        v51 = Curvef + *v48;
        ++v48;
        *(v48 - 1) = v51;
        v49 += Dimension;
        if (++v50 >= KnotIndexc)
        {
            v50 = 0;
            Curvef = Curvef + NextCurveDuration;
        }
        tiDsta = (float *)((char *)tiDsta - 1);
    } while (tiDsta);
    return 1;
}

void EvaluateCurve(const struct sample_context *Context, afxCurve Curve, const afxReal *IdentityVector, int Dimension, afxReal *Result, afxBool CurveIsNormalized)
{
    afxReal *tiBuffer; // [esp+Ch] [ebp-F8h]
    afxReal *piBuffer; // [esp+10h] [ebp-F4h]
    afxReal tiBufferStack[6]; // [esp+14h] [ebp-F0h]
    afxReal piBufferStack[54]; // [esp+2Ch] [ebp-D8h]

    if (AfxIsCurveIdentity(Curve))
    {
        for (afxUnit i = Dimension; i-- > 0;)
            Result[i] = IdentityVector[i];
    }
    else if (AfxIsCurveConstantOrIdentity(Curve))
    {
        AfxExtractCurveKnotValue(Curve, 0, Result, IdentityVector);
    }
    else
    {
        afxUnit Degree = AfxGetCurveDegree(Curve);
        afxUnit v8 = AfxFindCurveKnot(Curve, Context->LocalClock);

        if (ConstructBSplineBuffers(
            Dimension,
            (Context->UnderflowLoop != 0 ? Curve : 0),
            Curve,
            (Context->OverflowLoop != 0 ? Curve : 0),
            Context->LocalDuration,
            Context->LocalDuration,
            Context->LocalDuration,
            v8,
            tiBufferStack,
            piBufferStack,
            &tiBuffer,
            &piBuffer,
            IdentityVector)
            && CurveIsNormalized
            && Dimension == 4)
        {
            AfxEnsureQuaternionContinuity(Degree + 1, piBufferStack);
        }
        AfxCountCurveKnots(Curve);
        Smt2.SampleBSpline(Degree, Dimension, CurveIsNormalized, tiBuffer, piBuffer, Context->LocalClock, Result);
    }
}

_AMX smt2 Smt2 = 
{
    .CubicCoefficients = (void*)CubicCoefficients,
    .SampleBSpline = (void*)SampleBSpline,
    .ConstructBSplineBuffers = ConstructBSplineBuffers,
    .EvaluateCurve = EvaluateCurve
};
