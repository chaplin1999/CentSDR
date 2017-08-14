#include <arm_math.h>
#include "nanosdr.h"

int16_t buffer_i[AUDIO_BUFFER_LEN];
int16_t buffer_q[AUDIO_BUFFER_LEN];


const int16_t cos_sin_table[256][4] = {
    { -32767,   10,      0, -804 },
    { -32757,   29,   -804, -804 },
    { -32728,   50,  -1608, -802 },
    { -32678,   69,  -2410, -802 },
    { -32609,   88,  -3212, -799 },
    { -32521,  109,  -4011, -797 },
    { -32412,  127,  -4808, -794 },
    { -32285,  148,  -5602, -791 },
    { -32137,  166,  -6393, -786 },
    { -31971,  186,  -7179, -783 },
    { -31785,  205,  -7962, -777 },
    { -31580,  224,  -8739, -773 },
    { -31356,  243,  -9512, -766 },
    { -31113,  261, -10278, -761 },
    { -30852,  281, -11039, -754 },
    { -30571,  298, -11793, -746 },
    { -30273,  317, -12539, -740 },
    { -29956,  335, -13279, -731 },
    { -29621,  353, -14010, -722 },
    { -29268,  370, -14732, -714 },
    { -28898,  388, -15446, -705 },
    { -28510,  405, -16151, -695 },
    { -28105,  422, -16846, -684 },
    { -27683,  438, -17530, -674 },
    { -27245,  455, -18204, -664 },
    { -26790,  471, -18868, -651 },
    { -26319,  487, -19519, -640 },
    { -25832,  503, -20159, -628 },
    { -25329,  518, -20787, -616 },
    { -24811,  532, -21403, -602 },
    { -24279,  548, -22005, -589 },
    { -23731,  561, -22594, -576 },
    { -23170,  576, -23170, -561 },
    { -22594,  589, -23731, -548 },
    { -22005,  602, -24279, -532 },
    { -21403,  616, -24811, -518 },
    { -20787,  628, -25329, -503 },
    { -20159,  640, -25832, -487 },
    { -19519,  651, -26319, -471 },
    { -18868,  664, -26790, -455 },
    { -18204,  674, -27245, -438 },
    { -17530,  684, -27683, -422 },
    { -16846,  695, -28105, -405 },
    { -16151,  705, -28510, -388 },
    { -15446,  714, -28898, -370 },
    { -14732,  722, -29268, -353 },
    { -14010,  731, -29621, -335 },
    { -13279,  740, -29956, -317 },
    { -12539,  746, -30273, -298 },
    { -11793,  754, -30571, -281 },
    { -11039,  761, -30852, -261 },
    { -10278,  766, -31113, -243 },
    { -9512,  773, -31356, -224 },
    { -8739,  777, -31580, -205 },
    { -7962,  783, -31785, -186 },
    { -7179,  786, -31971, -166 },
    { -6393,  791, -32137, -148 },
    { -5602,  794, -32285, -127 },
    { -4808,  797, -32412, -109 },
    { -4011,  799, -32521,  -88 },
    { -3212,  802, -32609,  -69 },
    { -2410,  802, -32678,  -50 },
    { -1608,  804, -32728,  -29 },
    {  -804,  804, -32757,  -10 },
    {     0,  804, -32767,   10 },
    {   804,  804, -32757,   29 },
    {  1608,  802, -32728,   50 },
    {  2410,  802, -32678,   69 },
    {  3212,  799, -32609,   88 },
    {  4011,  797, -32521,  109 },
    {  4808,  794, -32412,  127 },
    {  5602,  791, -32285,  148 },
    {  6393,  786, -32137,  166 },
    {  7179,  783, -31971,  186 },
    {  7962,  777, -31785,  205 },
    {  8739,  773, -31580,  224 },
    {  9512,  766, -31356,  243 },
    { 10278,  761, -31113,  261 },
    { 11039,  754, -30852,  281 },
    { 11793,  746, -30571,  298 },
    { 12539,  740, -30273,  317 },
    { 13279,  731, -29956,  335 },
    { 14010,  722, -29621,  353 },
    { 14732,  714, -29268,  370 },
    { 15446,  705, -28898,  388 },
    { 16151,  695, -28510,  405 },
    { 16846,  684, -28105,  422 },
    { 17530,  674, -27683,  438 },
    { 18204,  664, -27245,  455 },
    { 18868,  651, -26790,  471 },
    { 19519,  640, -26319,  487 },
    { 20159,  628, -25832,  503 },
    { 20787,  616, -25329,  518 },
    { 21403,  602, -24811,  532 },
    { 22005,  589, -24279,  548 },
    { 22594,  576, -23731,  561 },
    { 23170,  561, -23170,  576 },
    { 23731,  548, -22594,  589 },
    { 24279,  532, -22005,  602 },
    { 24811,  518, -21403,  616 },
    { 25329,  503, -20787,  628 },
    { 25832,  487, -20159,  640 },
    { 26319,  471, -19519,  651 },
    { 26790,  455, -18868,  664 },
    { 27245,  438, -18204,  674 },
    { 27683,  422, -17530,  684 },
    { 28105,  405, -16846,  695 },
    { 28510,  388, -16151,  705 },
    { 28898,  370, -15446,  714 },
    { 29268,  353, -14732,  722 },
    { 29621,  335, -14010,  731 },
    { 29956,  317, -13279,  740 },
    { 30273,  298, -12539,  746 },
    { 30571,  281, -11793,  754 },
    { 30852,  261, -11039,  761 },
    { 31113,  243, -10278,  766 },
    { 31356,  224,  -9512,  773 },
    { 31580,  205,  -8739,  777 },
    { 31785,  186,  -7962,  783 },
    { 31971,  166,  -7179,  786 },
    { 32137,  148,  -6393,  791 },
    { 32285,  127,  -5602,  794 },
    { 32412,  109,  -4808,  797 },
    { 32521,   88,  -4011,  799 },
    { 32609,   69,  -3212,  802 },
    { 32678,   50,  -2410,  802 },
    { 32728,   29,  -1608,  804 },
    { 32757,   10,   -804,  804 },
    { 32767,  -10,      0,  804 },
    { 32757,  -29,    804,  804 },
    { 32728,  -50,   1608,  802 },
    { 32678,  -69,   2410,  802 },
    { 32609,  -88,   3212,  799 },
    { 32521, -109,   4011,  797 },
    { 32412, -127,   4808,  794 },
    { 32285, -148,   5602,  791 },
    { 32137, -166,   6393,  786 },
    { 31971, -186,   7179,  783 },
    { 31785, -205,   7962,  777 },
    { 31580, -224,   8739,  773 },
    { 31356, -243,   9512,  766 },
    { 31113, -261,  10278,  761 },
    { 30852, -281,  11039,  754 },
    { 30571, -298,  11793,  746 },
    { 30273, -317,  12539,  740 },
    { 29956, -335,  13279,  731 },
    { 29621, -353,  14010,  722 },
    { 29268, -370,  14732,  714 },
    { 28898, -388,  15446,  705 },
    { 28510, -405,  16151,  695 },
    { 28105, -422,  16846,  684 },
    { 27683, -438,  17530,  674 },
    { 27245, -455,  18204,  664 },
    { 26790, -471,  18868,  651 },
    { 26319, -487,  19519,  640 },
    { 25832, -503,  20159,  628 },
    { 25329, -518,  20787,  616 },
    { 24811, -532,  21403,  602 },
    { 24279, -548,  22005,  589 },
    { 23731, -561,  22594,  576 },
    { 23170, -576,  23170,  561 },
    { 22594, -589,  23731,  548 },
    { 22005, -602,  24279,  532 },
    { 21403, -616,  24811,  518 },
    { 20787, -628,  25329,  503 },
    { 20159, -640,  25832,  487 },
    { 19519, -651,  26319,  471 },
    { 18868, -664,  26790,  455 },
    { 18204, -674,  27245,  438 },
    { 17530, -684,  27683,  422 },
    { 16846, -695,  28105,  405 },
    { 16151, -705,  28510,  388 },
    { 15446, -714,  28898,  370 },
    { 14732, -722,  29268,  353 },
    { 14010, -731,  29621,  335 },
    { 13279, -740,  29956,  317 },
    { 12539, -746,  30273,  298 },
    { 11793, -754,  30571,  281 },
    { 11039, -761,  30852,  261 },
    { 10278, -766,  31113,  243 },
    {  9512, -773,  31356,  224 },
    {  8739, -777,  31580,  205 },
    {  7962, -783,  31785,  186 },
    {  7179, -786,  31971,  166 },
    {  6393, -791,  32137,  148 },
    {  5602, -794,  32285,  127 },
    {  4808, -797,  32412,  109 },
    {  4011, -799,  32521,   88 },
    {  3212, -802,  32609,   69 },
    {  2410, -802,  32678,   50 },
    {  1608, -804,  32728,   29 },
    {   804, -804,  32757,   10 },
    {     0, -804,  32767,  -10 },
    {  -804, -804,  32757,  -29 },
    { -1608, -802,  32728,  -50 },
    { -2410, -802,  32678,  -69 },
    { -3212, -799,  32609,  -88 },
    { -4011, -797,  32521, -109 },
    { -4808, -794,  32412, -127 },
    { -5602, -791,  32285, -148 },
    { -6393, -786,  32137, -166 },
    { -7179, -783,  31971, -186 },
    { -7962, -777,  31785, -205 },
    { -8739, -773,  31580, -224 },
    { -9512, -766,  31356, -243 },
    { -10278, -761,  31113, -261 },
    { -11039, -754,  30852, -281 },
    { -11793, -746,  30571, -298 },
    { -12539, -740,  30273, -317 },
    { -13279, -731,  29956, -335 },
    { -14010, -722,  29621, -353 },
    { -14732, -714,  29268, -370 },
    { -15446, -705,  28898, -388 },
    { -16151, -695,  28510, -405 },
    { -16846, -684,  28105, -422 },
    { -17530, -674,  27683, -438 },
    { -18204, -664,  27245, -455 },
    { -18868, -651,  26790, -471 },
    { -19519, -640,  26319, -487 },
    { -20159, -628,  25832, -503 },
    { -20787, -616,  25329, -518 },
    { -21403, -602,  24811, -532 },
    { -22005, -589,  24279, -548 },
    { -22594, -576,  23731, -561 },
    { -23170, -561,  23170, -576 },
    { -23731, -548,  22594, -589 },
    { -24279, -532,  22005, -602 },
    { -24811, -518,  21403, -616 },
    { -25329, -503,  20787, -628 },
    { -25832, -487,  20159, -640 },
    { -26319, -471,  19519, -651 },
    { -26790, -455,  18868, -664 },
    { -27245, -438,  18204, -674 },
    { -27683, -422,  17530, -684 },
    { -28105, -405,  16846, -695 },
    { -28510, -388,  16151, -705 },
    { -28898, -370,  15446, -714 },
    { -29268, -353,  14732, -722 },
    { -29621, -335,  14010, -731 },
    { -29956, -317,  13279, -740 },
    { -30273, -298,  12539, -746 },
    { -30571, -281,  11793, -754 },
    { -30852, -261,  11039, -761 },
    { -31113, -243,  10278, -766 },
    { -31356, -224,   9512, -773 },
    { -31580, -205,   8739, -777 },
    { -31785, -186,   7962, -783 },
    { -31971, -166,   7179, -786 },
    { -32137, -148,   6393, -791 },
    { -32285, -127,   5602, -794 },
    { -32412, -109,   4808, -797 },
    { -32521,  -88,   4011, -799 },
    { -32609,  -69,   3212, -802 },
    { -32678,  -50,   2410, -802 },
    { -32728,  -29,   1608, -804 },
    { -32757,  -10,    804, -804 }
};

static uint32_t
cos_sin(uint16_t phase)
{
    uint16_t idx = phase / 256;
    uint16_t mod = phase & 0xff;
    uint32_t r = __PKHBT(0x0100, mod, 16);
    uint32_t *e = (uint32_t *)&cos_sin_table[idx];
    uint32_t cd = e[0];
    uint32_t sd = e[1];
    int32_t c = __SMUAD(r, cd);
    int32_t s = __SMUAD(r, sd);
    c /= 256;
    s /= 256;
    return __PKHBT(s, c, 16);
}


uint16_t nco1_phase = 0;
uint16_t nco2_phase = 0;
#define SSB_NCO_PHASESTEP (65536L*SSB_FREQ_OFFSET/48000)


// Bi-Quad IIR Filter state
q15_t bq_i_state[4 * 3];
q15_t bq_q_state[4 * 3];
// 6th order elliptic lowpass filter fc=1300Hz
q15_t bq_coeffs[] = {
		  515, 0,   -906,   515, 30977, -14714,
		 5171, 0, -10087,  5171, 31760, -15739,
		16384, 0, -32182, 16384, 32165, -16253,
};

arm_biquad_casd_df1_inst_q15 bq_i = { 3, bq_i_state, bq_coeffs, 1};
arm_biquad_casd_df1_inst_q15 bq_q = { 3, bq_q_state, bq_coeffs, 1};


// 6th order elliptic lowpass filter fc=6*1300=7800Hz
q15_t bq_coeffs_am[] = {
		 1186, 0,   1108,  1186, 20883,  -8328,
		 6829, 0,  -4129,  6829, 17973, -13228,
		16384, 0, -14411, 16384, 16788, -15733
};

arm_biquad_casd_df1_inst_q15 bq_am_i = { 3, bq_i_state, bq_coeffs_am, 1};
arm_biquad_casd_df1_inst_q15 bq_am_q = { 3, bq_q_state, bq_coeffs_am, 1};

void
ssb_demod(int16_t *src, int16_t *dst, size_t len, int phasestep)
{
	q15_t *bufi = buffer_i;
	q15_t *bufq = buffer_q;
    int32_t *s = __SIMD32(src);
    int32_t *d = __SIMD32(dst);
    uint32_t i;

    // shift frequency
    for (i = 0; i < len/2; i++) {
		uint32_t cossin = cos_sin(nco1_phase);
		nco1_phase -= phasestep;
		uint32_t iq = *s++;
		*bufi++ = __SMLSDX(iq, cossin, 0) >> (15-0);
		*bufq++ = __SMLAD(iq, cossin, 0) >> (15-0);
	}
    disp_fetch_samples();

    // apply low pass filter
	arm_biquad_cascade_df1_q15(&bq_i, buffer_i, buffer_i, len/2);
	arm_biquad_cascade_df1_q15(&bq_q, buffer_q, buffer_q, len/2);

    // shift frequency inverse
	bufi = buffer_i;
	bufq = buffer_q;
    for (i = 0; i < len/2; i++) {
		uint32_t cossin = cos_sin(nco2_phase);
		nco2_phase += phasestep;
		uint32_t iq = __PKHBT(*bufi++, *bufq++, 16);
		uint32_t r = __SMLAD(iq, cossin, 0) >> (15-0);
        *d++ = __PKHBT(r, r, 16);
	}
}

__attribute__ ( ( always_inline ) ) __STATIC_INLINE
float _VSQRTF(float op1) {
  float result;
  __ASM volatile ("vsqrt.f32 %0,%1" : "=w"(result) : "w"(op1) );
  return(result);
}

void
am_demod(int16_t *src, int16_t *dst, size_t len)
#if defined(AM_FREQ_OFFSET) && AM_FREQ_OFFSET
{
#define PHASESTEP 65536L*AM_FREQ_OFFSET/48000

	q15_t *bufi = buffer_i;
	q15_t *bufq = buffer_q;
    int32_t *s = __SIMD32(src);
    int32_t *d = __SIMD32(dst);
    uint32_t i;
    for (i = 0; i < len/2; i++) {
      uint32_t cossin = 0x00007fff;//cos_sin(nco1_phase);
		//nco1_phase -= PHASESTEP;
		uint32_t iq = *s++;
		*bufi++ = __SMLSDX(iq, cossin, 0) >> (15-0);
		*bufq++ = __SMLAD(iq, cossin, 0) >> (15-0);
	}
    disp_fetch_samples();

    // apply low pass filter
	arm_biquad_cascade_df1_q15(&bq_am_i, buffer_i, buffer_i, len/2);
	arm_biquad_cascade_df1_q15(&bq_am_q, buffer_q, buffer_q, len/2);

	bufi = buffer_i;
	bufq = buffer_q;
    for (i = 0; i < len/2; i++) {
      int32_t x = *bufi++;
      int32_t y = *bufq++;
      int32_t z;
#define DCOFFSET 16383
      x = x/2;
      y = y/2;
      z = (int16_t)_VSQRTF((float)(x*x+y*y)) - DCOFFSET;
      //z = (int16_t)sqrtf(x*x+y*y) - DCOFFSET;
      *d++ = __PKHBT(z, z, 16);
	}
}
#else
{
  uint32_t i;
  for (i = 0; i < len; i += 2) {
    int32_t x = src[i];
    int32_t y = src[i+1];
    int32_t z;
#define DCOFFSET 16383
    x = x/2;
    y = y/2;
    z = (int16_t)_VSQRTF((float)(x*x+y*y)) - DCOFFSET;
    //z = (int16_t)sqrtf(x*x+y*y) - DCOFFSET;
    dst[i] = dst[i+1] = z;
  }
}
#endif

void
lsb_demod(int16_t *src, int16_t *dst, size_t len)
{
  ssb_demod(src, dst, len, -SSB_NCO_PHASESTEP);
}

void
usb_demod(int16_t *src, int16_t *dst, size_t len)
{
  ssb_demod(src, dst, len, SSB_NCO_PHASESTEP);
}

