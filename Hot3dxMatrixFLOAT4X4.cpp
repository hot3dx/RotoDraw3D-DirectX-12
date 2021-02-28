#pragma once

/****************************************************************************
 *
 * Matrix
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
 // Comparison operations
 //------------------------------------------------------------------------------

 //------------------------------------------------------------------------------

#include "pch.h"

// Return true if any entry in the matrix is NaN
inline bool XM_CALLCONV XMFLOAT4X4IsNaN
(
    XMFLOAT4X4 M
)
{
#if defined(_XM_NO_INTRINSICS_)
    size_t i = 16;
    auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
    do {
        // Fetch value into integer unit
        uint32_t uTest = pWork[0];
        // Remove sign
        uTest &= 0x7FFFFFFFU;
        // NaN is 0x7F800001 through 0x7FFFFFFF inclusive
        uTest -= 0x7F800001U;
        if (uTest < 0x007FFFFFU) {
            break;      // NaN found
        }
        ++pWork;        // Next entry
    } while (--i);
    return (i != 0);      // i == 0 if nothing matched
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    // Load in registers
    XMFLOAT4 vX = M.r[0];
    XMFLOAT4 vY = M.r[1];
    XMFLOAT4 vZ = M.r[2];
    XMFLOAT4 vW = M.r[3];
    // Test themselves to check for NaN
    vX = vmvnq_u32(vceqq_f32(vX, vX));
    vY = vmvnq_u32(vceqq_f32(vY, vY));
    vZ = vmvnq_u32(vceqq_f32(vZ, vZ));
    vW = vmvnq_u32(vceqq_f32(vW, vW));
    // Or all the results
    vX = vorrq_u32(vX, vZ);
    vY = vorrq_u32(vY, vW);
    vX = vorrq_u32(vX, vY);
    // If any tested true, return true
    int8x8x2_t vTemp = vzip_u8(vget_low_u8(vX), vget_high_u8(vX));
    vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
    uint32_t r = vget_lane_u32(vTemp.val[1], 1);
    return (r != 0);
#elif defined(_XM_SSE_INTRINSICS_)
    // Load in registers
    XMFLOAT4 vX = { M._11, M._12, M._13, M._14 };
    XMFLOAT4 vY = { M._21, M._22, M._23, M._24 };
    XMFLOAT4 vZ = { M._31, M._32, M._33, M._34 };
    XMFLOAT4 vW = { M._41, M._42, M._43, M._44 };
    // Test themselves to check for NaN
    vX = _mm_cmpneq_ps(XMLoadFloat4(&vX), XMLoadFloat4(&vX));
    vY = _mm_cmpneq_ps(XMLoadFloat4(&vY), XMLoadFloat4(&vY));
    vZ = _mm_cmpneq_ps(XMLoadFloat4(&vZ), XMLoadFloat4(&vZ));
    vW = _mm_cmpneq_ps(XMLoadFloat4(&vW), XMLoadFloat4(&vW));
    // Or all the results
    vX = _mm_or_ps(vX, vZ);
    vY = _mm_or_ps(vY, vW);
    vX = _mm_or_ps(vX, vY);
    // If any tested true, return true
    return (_mm_movemask_ps(vX) != 0);
#else
#endif
}



//------------------------------------------------------------------------------

// Return true if any entry in the matrix is +/-INF
inline bool XM_CALLCONV XMFLOAT4X4IsInfinite
(
    XMFLOAT4X4 M
)
{
#if defined(_XM_NO_INTRINSICS_)
    size_t i = 16;
    auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
    do {
        // Fetch value into integer unit
        uint32_t uTest = pWork[0];
        // Remove sign
        uTest &= 0x7FFFFFFFU;
        // INF is 0x7F800000
        if (uTest == 0x7F800000U) {
            break;      // INF found
        }
        ++pWork;        // Next entry
    } while (--i);
    return (i != 0);      // i == 0 if nothing matched
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    // Mask off the sign bits
    XMFLOAT4 vTemp1 = vandq_u32(M.r[0], g_XMAbsMask);
    XMFLOAT4 vTemp2 = vandq_u32(M.r[1], g_XMAbsMask);
    XMFLOAT4 vTemp3 = vandq_u32(M.r[2], g_XMAbsMask);
    XMFLOAT4 vTemp4 = vandq_u32(M.r[3], g_XMAbsMask);
    // Compare to infinity
    vTemp1 = vceqq_f32(vTemp1, g_XMInfinity);
    vTemp2 = vceqq_f32(vTemp2, g_XMInfinity);
    vTemp3 = vceqq_f32(vTemp3, g_XMInfinity);
    vTemp4 = vceqq_f32(vTemp4, g_XMInfinity);
    // Or the answers together
    vTemp1 = vorrq_u32(vTemp1, vTemp2);
    vTemp3 = vorrq_u32(vTemp3, vTemp4);
    vTemp1 = vorrq_u32(vTemp1, vTemp3);
    // If any are infinity, the signs are true.
    int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
    vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
    uint32_t r = vget_lane_u32(vTemp.val[1], 1);
    return (r != 0);
#elif defined(_XM_SSE_INTRINSICS_)
    // Mask off the sign bits
    XMFLOAT4 vTemp1 = _mm_and_ps(M.r[0], g_XMAbsMask);
    XMFLOAT4 vTemp2 = _mm_and_ps(M.r[1], g_XMAbsMask);
    XMFLOAT4 vTemp3 = _mm_and_ps(M.r[2], g_XMAbsMask);
    XMFLOAT4 vTemp4 = _mm_and_ps(M.r[3], g_XMAbsMask);
    // Compare to infinity
    vTemp1 = _mm_cmpeq_ps(vTemp1, g_XMInfinity);
    vTemp2 = _mm_cmpeq_ps(vTemp2, g_XMInfinity);
    vTemp3 = _mm_cmpeq_ps(vTemp3, g_XMInfinity);
    vTemp4 = _mm_cmpeq_ps(vTemp4, g_XMInfinity);
    // Or the answers together
    vTemp1 = _mm_or_ps(vTemp1, vTemp2);
    vTemp3 = _mm_or_ps(vTemp3, vTemp4);
    vTemp1 = _mm_or_ps(vTemp1, vTemp3);
    // If any are infinity, the signs are true.
    return (_mm_movemask_ps(vTemp1) != 0);
#endif
}

//------------------------------------------------------------------------------

// Return true if the XMFLOAT4X4 is equal to identity
inline bool XM_CALLCONV XMFLOAT4X4IsIdentity
(
    XMFLOAT4X4 M
)
{
#if defined(_XM_NO_INTRINSICS_)
    // Use the integer pipeline to reduce branching to a minimum
    auto pWork = reinterpret_cast<const uint32_t*>(&M.m[0][0]);
    // Convert 1.0f to zero and or them together
    uint32_t uOne = pWork[0] ^ 0x3F800000U;
    // Or all the 0.0f entries together
    uint32_t uZero = pWork[1];
    uZero |= pWork[2];
    uZero |= pWork[3];
    // 2nd row
    uZero |= pWork[4];
    uOne |= pWork[5] ^ 0x3F800000U;
    uZero |= pWork[6];
    uZero |= pWork[7];
    // 3rd row
    uZero |= pWork[8];
    uZero |= pWork[9];
    uOne |= pWork[10] ^ 0x3F800000U;
    uZero |= pWork[11];
    // 4th row
    uZero |= pWork[12];
    uZero |= pWork[13];
    uZero |= pWork[14];
    uOne |= pWork[15] ^ 0x3F800000U;
    // If all zero entries are zero, the uZero==0
    uZero &= 0x7FFFFFFF;    // Allow -0.0f
    // If all 1.0f entries are 1.0f, then uOne==0
    uOne |= uZero;
    return (uOne == 0);
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    XMFLOAT4 vTemp1 = vceqq_f32(M.r[0], g_XMIdentityR0);
    XMFLOAT4 vTemp2 = vceqq_f32(M.r[1], g_XMIdentityR1);
    XMFLOAT4 vTemp3 = vceqq_f32(M.r[2], g_XMIdentityR2);
    XMFLOAT4 vTemp4 = vceqq_f32(M.r[3], g_XMIdentityR3);
    vTemp1 = vandq_u32(vTemp1, vTemp2);
    vTemp3 = vandq_u32(vTemp3, vTemp4);
    vTemp1 = vandq_u32(vTemp1, vTemp3);
    int8x8x2_t vTemp = vzip_u8(vget_low_u8(vTemp1), vget_high_u8(vTemp1));
    vTemp = vzip_u16(vTemp.val[0], vTemp.val[1]);
    uint32_t r = vget_lane_u32(vTemp.val[1], 1);
    return (r == 0xFFFFFFFFU);
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4 vTemp1 = _mm_cmpeq_ps(M.r[0], g_XMIdentityR0);
    XMFLOAT4 vTemp2 = _mm_cmpeq_ps(M.r[1], g_XMIdentityR1);
    XMFLOAT4 vTemp3 = _mm_cmpeq_ps(M.r[2], g_XMIdentityR2);
    XMFLOAT4 vTemp4 = _mm_cmpeq_ps(M.r[3], g_XMIdentityR3);
    vTemp1 = _mm_and_ps(vTemp1, vTemp2);
    vTemp3 = _mm_and_ps(vTemp3, vTemp4);
    vTemp1 = _mm_and_ps(vTemp1, vTemp3);
    return (_mm_movemask_ps(vTemp1) == 0x0f);
#endif
}

//------------------------------------------------------------------------------
// Computation operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Perform a 4x4 matrix multiply by a 4x4 matrix
inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Multiply
(
    XMFLOAT4X4 M1,
    XMFLOAT4X4 M2
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMFLOAT4X4 mResult;
    // Cache the invariants in registers
    float x = M1.m[0][0];
    float y = M1.m[0][1];
    float z = M1.m[0][2];
    float w = M1.m[0][3];
    // Perform the operation on the first row
    mResult.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
    mResult.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
    mResult.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
    mResult.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
    // Repeat for all the other rows
    x = M1.m[1][0];
    y = M1.m[1][1];
    z = M1.m[1][2];
    w = M1.m[1][3];
    mResult.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
    mResult.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
    mResult.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
    mResult.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
    x = M1.m[2][0];
    y = M1.m[2][1];
    z = M1.m[2][2];
    w = M1.m[2][3];
    mResult.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
    mResult.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
    mResult.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
    mResult.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
    x = M1.m[3][0];
    y = M1.m[3][1];
    z = M1.m[3][2];
    w = M1.m[3][3];
    mResult.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
    mResult.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
    mResult.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
    mResult.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
    return mResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    XMFLOAT4X4 mResult;
    float32x2_t VL = vget_low_f32(M1.r[0]);
    float32x2_t VH = vget_high_f32(M1.r[0]);
    // Perform the operation on the first row
    XMFLOAT4 vX = vmulq_lane_f32(M2.r[0], VL, 0);
    XMFLOAT4 vY = vmulq_lane_f32(M2.r[1], VL, 1);
    XMFLOAT4 vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    XMFLOAT4 vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    mResult.r[0] = vaddq_f32(vZ, vW);
    // Repeat for the other 3 rows
    VL = vget_low_f32(M1.r[1]);
    VH = vget_high_f32(M1.r[1]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    mResult.r[1] = vaddq_f32(vZ, vW);
    VL = vget_low_f32(M1.r[2]);
    VH = vget_high_f32(M1.r[2]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    mResult.r[2] = vaddq_f32(vZ, vW);
    VL = vget_low_f32(M1.r[3]);
    VH = vget_high_f32(M1.r[3]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    mResult.r[3] = vaddq_f32(vZ, vW);
    return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 mResult;
    // Splat the component X,Y,Z then W
#if defined(_XM_AVX_INTRINSICS_)
    XMFLOAT4 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
    XMFLOAT4 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
    XMFLOAT4 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
    XMFLOAT4 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
    // Use vW to hold the original row
    XMFLOAT4 vW = M1.r[0];
    XMFLOAT4 vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    XMFLOAT4 vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    XMFLOAT4 vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    // Perform the operation on the first row
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    // Perform a binary add to reduce cumulative errors
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    mResult.r[0] = vX;
    // Repeat for the other 3 rows
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
    vW = M1.r[1];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    mResult.r[1] = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
    vW = M1.r[2];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    mResult.r[2] = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
    vW = M1.r[3];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    mResult.r[3] = vX;
    return mResult;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4MultiplyTranspose
(
    XMFLOAT4X4 M1,
    XMFLOAT4X4 M2
)
{
#if defined(_XM_NO_INTRINSICS_)
    XMFLOAT4X4 mResult;
    // Cache the invariants in registers
    float x = M2.m[0][0];
    float y = M2.m[1][0];
    float z = M2.m[2][0];
    float w = M2.m[3][0];
    // Perform the operation on the first row
    mResult.m[0][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
    mResult.m[0][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
    mResult.m[0][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
    mResult.m[0][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
    // Repeat for all the other rows
    x = M2.m[0][1];
    y = M2.m[1][1];
    z = M2.m[2][1];
    w = M2.m[3][1];
    mResult.m[1][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
    mResult.m[1][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
    mResult.m[1][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
    mResult.m[1][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
    x = M2.m[0][2];
    y = M2.m[1][2];
    z = M2.m[2][2];
    w = M2.m[3][2];
    mResult.m[2][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
    mResult.m[2][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
    mResult.m[2][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
    mResult.m[2][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
    x = M2.m[0][3];
    y = M2.m[1][3];
    z = M2.m[2][3];
    w = M2.m[3][3];
    mResult.m[3][0] = (M1.m[0][0] * x) + (M1.m[0][1] * y) + (M1.m[0][2] * z) + (M1.m[0][3] * w);
    mResult.m[3][1] = (M1.m[1][0] * x) + (M1.m[1][1] * y) + (M1.m[1][2] * z) + (M1.m[1][3] * w);
    mResult.m[3][2] = (M1.m[2][0] * x) + (M1.m[2][1] * y) + (M1.m[2][2] * z) + (M1.m[2][3] * w);
    mResult.m[3][3] = (M1.m[3][0] * x) + (M1.m[3][1] * y) + (M1.m[3][2] * z) + (M1.m[3][3] * w);
    return mResult;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float32x2_t VL = vget_low_f32(M1.r[0]);
    float32x2_t VH = vget_high_f32(M1.r[0]);
    // Perform the operation on the first row
    XMFLOAT4 vX = vmulq_lane_f32(M2.r[0], VL, 0);
    XMFLOAT4 vY = vmulq_lane_f32(M2.r[1], VL, 1);
    XMFLOAT4 vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    XMFLOAT4 vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    float32x4_t r0 = vaddq_f32(vZ, vW);
    // Repeat for the other 3 rows
    VL = vget_low_f32(M1.r[1]);
    VH = vget_high_f32(M1.r[1]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    float32x4_t r1 = vaddq_f32(vZ, vW);
    VL = vget_low_f32(M1.r[2]);
    VH = vget_high_f32(M1.r[2]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    float32x4_t r2 = vaddq_f32(vZ, vW);
    VL = vget_low_f32(M1.r[3]);
    VH = vget_high_f32(M1.r[3]);
    vX = vmulq_lane_f32(M2.r[0], VL, 0);
    vY = vmulq_lane_f32(M2.r[1], VL, 1);
    vZ = vmlaq_lane_f32(vX, M2.r[2], VH, 0);
    vW = vmlaq_lane_f32(vY, M2.r[3], VH, 1);
    float32x4_t r3 = vaddq_f32(vZ, vW);

    // Transpose result
    float32x4x2_t P0 = vzipq_f32(r0, r2);
    float32x4x2_t P1 = vzipq_f32(r1, r3);

    float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
    float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

    XMFLOAT4X4 mResult;
    mResult.r[0] = T0.val[0];
    mResult.r[1] = T0.val[1];
    mResult.r[2] = T1.val[0];
    mResult.r[3] = T1.val[1];
    return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
    // Splat the component X,Y,Z then W
#if defined(_XM_AVX_INTRINSICS_)
    XMFLOAT4 vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 0);
    XMFLOAT4 vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 1);
    XMFLOAT4 vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 2);
    XMFLOAT4 vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[0]) + 3);
#else
    // Use vW to hold the original row
    XMFLOAT4 vW = M1.r[0];
    XMFLOAT4 vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    XMFLOAT4 vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    XMFLOAT4 vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    // Perform the operation on the first row
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    // Perform a binary add to reduce cumulative errors
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    XMFLOAT4 r0 = vX;
    // Repeat for the other 3 rows
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[1]) + 3);
#else
    vW = M1.r[1];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    XMFLOAT4 r1 = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[2]) + 3);
#else
    vW = M1.r[2];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    XMFLOAT4 r2 = vX;
#if defined(_XM_AVX_INTRINSICS_)
    vX = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 0);
    vY = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 1);
    vZ = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 2);
    vW = _mm_broadcast_ss(reinterpret_cast<const float*>(&M1.r[3]) + 3);
#else
    vW = M1.r[3];
    vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
    vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
    vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
    vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
#endif
    vX = _mm_mul_ps(vX, M2.r[0]);
    vY = _mm_mul_ps(vY, M2.r[1]);
    vZ = _mm_mul_ps(vZ, M2.r[2]);
    vW = _mm_mul_ps(vW, M2.r[3]);
    vX = _mm_add_ps(vX, vZ);
    vY = _mm_add_ps(vY, vW);
    vX = _mm_add_ps(vX, vY);
    XMFLOAT4 r3 = vX;

    // x.x,x.y,y.x,y.y
    XMFLOAT4 vTemp1 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(1, 0, 1, 0));
    // x.z,x.w,y.z,y.w
    XMFLOAT4 vTemp3 = _mm_shuffle_ps(r0, r1, _MM_SHUFFLE(3, 2, 3, 2));
    // z.x,z.y,w.x,w.y
    XMFLOAT4 vTemp2 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(1, 0, 1, 0));
    // z.z,z.w,w.z,w.w
    XMFLOAT4 vTemp4 = _mm_shuffle_ps(r2, r3, _MM_SHUFFLE(3, 2, 3, 2));

    XMFLOAT4X4 mResult;
    // x.x,y.x,z.x,w.x
    mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
    // x.y,y.y,z.y,w.y
    mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
    // x.z,y.z,z.z,w.z
    mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
    // x.w,y.w,z.w,w.w
    mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
    return mResult;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Transpose
(
    XMFLOAT4X4 M
)
{
#if defined(_XM_NO_INTRINSICS_)

    // Original matrix:
    //
    //     m00m01m02m03
    //     m10m11m12m13
    //     m20m21m22m23
    //     m30m31m32m33

    XMFLOAT4X4 P;
    P.r[0] = XMFLOAT4MergeXY(M.r[0], M.r[2]); // m00m20m01m21
    P.r[1] = XMFLOAT4MergeXY(M.r[1], M.r[3]); // m10m30m11m31
    P.r[2] = XMFLOAT4MergeZW(M.r[0], M.r[2]); // m02m22m03m23
    P.r[3] = XMFLOAT4MergeZW(M.r[1], M.r[3]); // m12m32m13m33

    XMFLOAT4X4 MT;
    MT.r[0] = XMFLOAT4MergeXY(P.r[0], P.r[1]); // m00m10m20m30
    MT.r[1] = XMFLOAT4MergeZW(P.r[0], P.r[1]); // m01m11m21m31
    MT.r[2] = XMFLOAT4MergeXY(P.r[2], P.r[3]); // m02m12m22m32
    MT.r[3] = XMFLOAT4MergeZW(P.r[2], P.r[3]); // m03m13m23m33
    return MT;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float32x4x2_t P0 = vzipq_f32(M.r[0], M.r[2]);
    float32x4x2_t P1 = vzipq_f32(M.r[1], M.r[3]);

    float32x4x2_t T0 = vzipq_f32(P0.val[0], P1.val[0]);
    float32x4x2_t T1 = vzipq_f32(P0.val[1], P1.val[1]);

    XMFLOAT4X4 mResult;
    mResult.r[0] = T0.val[0];
    mResult.r[1] = T0.val[1];
    mResult.r[2] = T1.val[0];
    mResult.r[3] = T1.val[1];
    return mResult;
#elif defined(_XM_SSE_INTRINSICS_)
    // x.x,x.y,y.x,y.y
    XMFLOAT4 vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
    // x.z,x.w,y.z,y.w
    XMFLOAT4 vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
    // z.x,z.y,w.x,w.y
    XMFLOAT4 vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
    // z.z,z.w,w.z,w.w
    XMFLOAT4 vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
    XMFLOAT4X4 mResult;

    // x.x,y.x,z.x,w.x
    mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
    // x.y,y.y,z.y,w.y
    mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
    // x.z,y.z,z.z,w.z
    mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
    // x.w,y.w,z.w,w.w
    mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
    return mResult;
#endif
}

//------------------------------------------------------------------------------
// Return the inverse and the determinant of a 4x4 matrix
_Use_decl_annotations_
inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Inverse
(
    XMFLOAT4* pDeterminant,
    XMFLOAT4X4  M
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

    XMFLOAT4X4 MT = XMFLOAT4X4Transpose(M);

    XMFLOAT4 V0[4], V1[4];
    V0[0] = XMFLOAT4Swizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[2]);
    V1[0] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[3]);
    V0[1] = XMFLOAT4Swizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[0]);
    V1[1] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[1]);
    V0[2] = XMFLOAT4Permute<XM_PERMUTE_0X, XM_PERMUTE_0Z, XM_PERMUTE_1X, XM_PERMUTE_1Z>(MT.r[2], MT.r[0]);
    V1[2] = XMFLOAT4Permute<XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_1W>(MT.r[3], MT.r[1]);

    XMFLOAT4 D0 = XMFLOAT4Multiply(V0[0], V1[0]);
    XMFLOAT4 D1 = XMFLOAT4Multiply(V0[1], V1[1]);
    XMFLOAT4 D2 = XMFLOAT4Multiply(V0[2], V1[2]);

    V0[0] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[2]);
    V1[0] = XMFLOAT4Swizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[3]);
    V0[1] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W>(MT.r[0]);
    V1[1] = XMFLOAT4Swizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(MT.r[1]);
    V0[2] = XMFLOAT4Permute<XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_1W>(MT.r[2], MT.r[0]);
    V1[2] = XMFLOAT4Permute<XM_PERMUTE_0X, XM_PERMUTE_0Z, XM_PERMUTE_1X, XM_PERMUTE_1Z>(MT.r[3], MT.r[1]);

    D0 = XMFLOAT4NegativeMultiplySubtract(V0[0], V1[0], D0);
    D1 = XMFLOAT4NegativeMultiplySubtract(V0[1], V1[1], D1);
    D2 = XMFLOAT4NegativeMultiplySubtract(V0[2], V1[2], D2);

    V0[0] = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y>(MT.r[1]);
    V1[0] = XMFLOAT4Permute<XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X>(D0, D2);
    V0[1] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_X>(MT.r[0]);
    V1[1] = XMFLOAT4Permute<XM_PERMUTE_0W, XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_0Z>(D0, D2);
    V0[2] = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y>(MT.r[3]);
    V1[2] = XMFLOAT4Permute<XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0W, XM_PERMUTE_0X>(D1, D2);
    V0[3] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_X>(MT.r[2]);
    V1[3] = XMFLOAT4Permute<XM_PERMUTE_0W, XM_PERMUTE_1W, XM_PERMUTE_0Y, XM_PERMUTE_0Z>(D1, D2);

    XMFLOAT4 C0 = XMFLOAT4Multiply(V0[0], V1[0]);
    XMFLOAT4 C2 = XMFLOAT4Multiply(V0[1], V1[1]);
    XMFLOAT4 C4 = XMFLOAT4Multiply(V0[2], V1[2]);
    XMFLOAT4 C6 = XMFLOAT4Multiply(V0[3], V1[3]);

    V0[0] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y, XM_SWIZZLE_Z>(MT.r[1]);
    V1[0] = XMFLOAT4Permute<XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1X>(D0, D2);
    V0[1] = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y>(MT.r[0]);
    V1[1] = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_0X>(D0, D2);
    V0[2] = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y, XM_SWIZZLE_Z>(MT.r[3]);
    V1[2] = XMFLOAT4Permute<XM_PERMUTE_0W, XM_PERMUTE_0X, XM_PERMUTE_0Y, XM_PERMUTE_1Z>(D1, D2);
    V0[3] = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_Z, XM_SWIZZLE_W, XM_SWIZZLE_Y>(MT.r[2]);
    V1[3] = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1Z, XM_PERMUTE_0X>(D1, D2);

    C0 = XMFLOAT4NegativeMultiplySubtract(V0[0], V1[0], C0);
    C2 = XMFLOAT4NegativeMultiplySubtract(V0[1], V1[1], C2);
    C4 = XMFLOAT4NegativeMultiplySubtract(V0[2], V1[2], C4);
    C6 = XMFLOAT4NegativeMultiplySubtract(V0[3], V1[3], C6);

    V0[0] = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_X>(MT.r[1]);
    V1[0] = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_1X, XM_PERMUTE_0Z>(D0, D2);
    V0[1] = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Z>(MT.r[0]);
    V1[1] = XMFLOAT4Permute<XM_PERMUTE_1Y, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1X>(D0, D2);
    V0[2] = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_W, XM_SWIZZLE_X>(MT.r[3]);
    V1[2] = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_1W, XM_PERMUTE_1Z, XM_PERMUTE_0Z>(D1, D2);
    V0[3] = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_W, XM_SWIZZLE_X, XM_SWIZZLE_Z>(MT.r[2]);
    V1[3] = XMFLOAT4Permute<XM_PERMUTE_1W, XM_PERMUTE_0X, XM_PERMUTE_0W, XM_PERMUTE_1Z>(D1, D2);

    XMFLOAT4 C1 = XMFLOAT4NegativeMultiplySubtract(V0[0], V1[0], C0);
    C0 = XMFLOAT4MultiplyAdd(V0[0], V1[0], C0);
    XMFLOAT4 C3 = XMFLOAT4MultiplyAdd(V0[1], V1[1], C2);
    C2 = XMFLOAT4NegativeMultiplySubtract(V0[1], V1[1], C2);
    XMFLOAT4 C5 = XMFLOAT4NegativeMultiplySubtract(V0[2], V1[2], C4);
    C4 = XMFLOAT4MultiplyAdd(V0[2], V1[2], C4);
    XMFLOAT4 C7 = XMFLOAT4MultiplyAdd(V0[3], V1[3], C6);
    C6 = XMFLOAT4NegativeMultiplySubtract(V0[3], V1[3], C6);

    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Select(C0, C1, g_XMSelect0101.v);
    R.r[1] = XMFLOAT4Select(C2, C3, g_XMSelect0101.v);
    R.r[2] = XMFLOAT4Select(C4, C5, g_XMSelect0101.v);
    R.r[3] = XMFLOAT4Select(C6, C7, g_XMSelect0101.v);

    XMFLOAT4 Determinant = XMFLOAT44Dot(R.r[0], MT.r[0]);

    if (pDeterminant != nullptr)
        *pDeterminant = Determinant;

    XMFLOAT4 Reciprocal = XMFLOAT4Reciprocal(Determinant);

    XMFLOAT4X4 Result;
    Result.r[0] = XMFLOAT4Multiply(R.r[0], Reciprocal);
    Result.r[1] = XMFLOAT4Multiply(R.r[1], Reciprocal);
    Result.r[2] = XMFLOAT4Multiply(R.r[2], Reciprocal);
    Result.r[3] = XMFLOAT4Multiply(R.r[3], Reciprocal);
    return Result;

#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 MT = XMFLOAT4X4Transpose(M);
    XMFLOAT4 V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
    XMFLOAT4 V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
    XMFLOAT4 V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
    XMFLOAT4 V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
    XMFLOAT4 V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
    XMFLOAT4 V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));

    XMFLOAT4 D0 = _mm_mul_ps(V00, V10);
    XMFLOAT4 D1 = _mm_mul_ps(V01, V11);
    XMFLOAT4 D2 = _mm_mul_ps(V02, V12);

    V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
    V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
    V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
    V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
    V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
    V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    D0 = _mm_sub_ps(D0, V00);
    D1 = _mm_sub_ps(D1, V01);
    D2 = _mm_sub_ps(D2, V02);
    // V11 = D0Y,D0W,D2Y,D2Y
    V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
    V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
    V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
    V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
    V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
    // V13 = D1Y,D1W,D2W,D2W
    XMFLOAT4 V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
    V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
    V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
    XMFLOAT4 V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
    V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

    XMFLOAT4 C0 = _mm_mul_ps(V00, V10);
    XMFLOAT4 C2 = _mm_mul_ps(V01, V11);
    XMFLOAT4 C4 = _mm_mul_ps(V02, V12);
    XMFLOAT4 C6 = _mm_mul_ps(V03, V13);

    // V11 = D0X,D0Y,D2X,D2X
    V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
    V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
    V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
    V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
    V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
    // V13 = D1X,D1Y,D2Z,D2Z
    V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
    V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
    V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
    V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
    V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    V03 = _mm_mul_ps(V03, V13);
    C0 = _mm_sub_ps(C0, V00);
    C2 = _mm_sub_ps(C2, V01);
    C4 = _mm_sub_ps(C4, V02);
    C6 = _mm_sub_ps(C6, V03);

    V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
    // V10 = D0Z,D0Z,D2X,D2Y
    V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
    V10 = XM_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
    V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
    // V11 = D0X,D0W,D2X,D2Y
    V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
    V11 = XM_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
    V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
    // V12 = D1Z,D1Z,D2Z,D2W
    V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
    V12 = XM_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
    V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
    // V13 = D1X,D1W,D2Z,D2W
    V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
    V13 = XM_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

    V00 = _mm_mul_ps(V00, V10);
    V01 = _mm_mul_ps(V01, V11);
    V02 = _mm_mul_ps(V02, V12);
    V03 = _mm_mul_ps(V03, V13);
    XMFLOAT4 C1 = _mm_sub_ps(C0, V00);
    C0 = _mm_add_ps(C0, V00);
    XMFLOAT4 C3 = _mm_add_ps(C2, V01);
    C2 = _mm_sub_ps(C2, V01);
    XMFLOAT4 C5 = _mm_sub_ps(C4, V02);
    C4 = _mm_add_ps(C4, V02);
    XMFLOAT4 C7 = _mm_add_ps(C6, V03);
    C6 = _mm_sub_ps(C6, V03);

    C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
    C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
    C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
    C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
    C0 = XM_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
    C2 = XM_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
    C4 = XM_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
    C6 = XM_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
    // Get the determinate
    XMFLOAT4 vTemp = XMFLOAT44Dot(C0, MT.r[0]);
    if (pDeterminant != nullptr)
        *pDeterminant = vTemp;
    vTemp = _mm_div_ps(g_XMOne, vTemp);
    XMFLOAT4X4 mResult;
    mResult.r[0] = _mm_mul_ps(C0, vTemp);
    mResult.r[1] = _mm_mul_ps(C2, vTemp);
    mResult.r[2] = _mm_mul_ps(C4, vTemp);
    mResult.r[3] = _mm_mul_ps(C6, vTemp);
    return mResult;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4 XM_CALLCONV XMFLOAT4X4Determinant
(
    XMFLOAT4X4 M
)
{
    static const XMFLOAT4 Sign = { { { 1.0f, -1.0f, 1.0f, -1.0f } } };

    XMFLOAT4 V0 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[2]);
    XMFLOAT4 V1 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[3]);
    XMFLOAT4 V2 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[2]);
    XMFLOAT4 V3 = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[3]);
    XMFLOAT4 V4 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[2]);
    XMFLOAT4 V5 = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[3]);

    XMFLOAT4 P0 = XMFLOAT4Multiply(V0, V1);
    XMFLOAT4 P1 = XMFLOAT4Multiply(V2, V3);
    XMFLOAT4 P2 = XMFLOAT4Multiply(V4, V5);

    V0 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[2]);
    V1 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[3]);
    V2 = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[2]);
    V3 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[3]);
    V4 = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[2]);
    V5 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[3]);

    P0 = XMFLOAT4NegativeMultiplySubtract(V0, V1, P0);
    P1 = XMFLOAT4NegativeMultiplySubtract(V2, V3, P1);
    P2 = XMFLOAT4NegativeMultiplySubtract(V4, V5, P2);

    V0 = XMFLOAT4Swizzle<XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_W, XM_SWIZZLE_Z>(M.r[1]);
    V1 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Y>(M.r[1]);
    V2 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_X>(M.r[1]);

    XMFLOAT4 S = XMFLOAT4Multiply(M.r[0], Sign.v);
    XMFLOAT4 R = XMFLOAT4Multiply(V0, P0);
    R = XMFLOAT4NegativeMultiplySubtract(V1, P1, R);
    R = XMFLOAT4MultiplyAdd(V2, P2, R);

    return XMFLOAT44Dot(S, R);
}

#define XM3RANKDECOMPOSE(a, b, c, x, y, z)      \
    if((x) < (y))                   \
    {                               \
        if((y) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 1;                \
            (c) = 0;                \
        }                           \
        else                        \
        {                           \
            (a) = 1;                \
                                    \
            if((x) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 0;            \
            }                       \
            else                    \
            {                       \
                (b) = 0;            \
                (c) = 2;            \
            }                       \
        }                           \
    }                               \
    else                            \
    {                               \
        if((x) < (z))               \
        {                           \
            (a) = 2;                \
            (b) = 0;                \
            (c) = 1;                \
        }                           \
        else                        \
        {                           \
            (a) = 0;                \
                                    \
            if((y) < (z))           \
            {                       \
                (b) = 2;            \
                (c) = 1;            \
            }                       \
            else                    \
            {                       \
                (b) = 1;            \
                (c) = 2;            \
            }                       \
        }                           \
    }

#define XM3_DECOMP_EPSILON 0.0001f

_Use_decl_annotations_
inline bool XM_CALLCONV XMFLOAT4X4Decompose
(
    XMFLOAT4* outScale,
    XMFLOAT4* outRotQuat,
    XMFLOAT4* outTrans,
    XMFLOAT4X4 M
)
{
    static const XMFLOAT4* pvCanonicalBasis[3] = {
        &g_XMIdentityR0.v,
        &g_XMIdentityR1.v,
        &g_XMIdentityR2.v
    };

    assert(outScale != nullptr);
    assert(outRotQuat != nullptr);
    assert(outTrans != nullptr);

    // Get the translation
    outTrans[0] = M.r[3];

    XMFLOAT4* ppvBasis[3];
    XMFLOAT4X4 matTemp;
    ppvBasis[0] = &matTemp.r[0];
    ppvBasis[1] = &matTemp.r[1];
    ppvBasis[2] = &matTemp.r[2];

    matTemp.r[0] = M.r[0];
    matTemp.r[1] = M.r[1];
    matTemp.r[2] = M.r[2];
    matTemp.r[3] = g_XMIdentityR3.v;

    auto pfScales = reinterpret_cast<float*>(outScale);

    size_t a, b, c;
    XMFLOAT4GetXPtr(&pfScales[0], XMFLOAT43Length(ppvBasis[0][0]));
    XMFLOAT4GetXPtr(&pfScales[1], XMFLOAT43Length(ppvBasis[1][0]));
    XMFLOAT4GetXPtr(&pfScales[2], XMFLOAT43Length(ppvBasis[2][0]));
    pfScales[3] = 0.f;

    XM3RANKDECOMPOSE(a, b, c, pfScales[0], pfScales[1], pfScales[2])

        if (pfScales[a] < XM3_DECOMP_EPSILON)
        {
            ppvBasis[a][0] = pvCanonicalBasis[a][0];
        }
    ppvBasis[a][0] = XMFLOAT43Normalize(ppvBasis[a][0]);

    if (pfScales[b] < XM3_DECOMP_EPSILON)
    {
        size_t aa, bb, cc;
        float fAbsX, fAbsY, fAbsZ;

        fAbsX = fabsf(XMFLOAT4GetX(ppvBasis[a][0]));
        fAbsY = fabsf(XMFLOAT4GetY(ppvBasis[a][0]));
        fAbsZ = fabsf(XMFLOAT4GetZ(ppvBasis[a][0]));

        XM3RANKDECOMPOSE(aa, bb, cc, fAbsX, fAbsY, fAbsZ)

            ppvBasis[b][0] = XMFLOAT43Cross(ppvBasis[a][0], pvCanonicalBasis[cc][0]);
    }

    ppvBasis[b][0] = XMFLOAT43Normalize(ppvBasis[b][0]);

    if (pfScales[c] < XM3_DECOMP_EPSILON)
    {
        ppvBasis[c][0] = XMFLOAT43Cross(ppvBasis[a][0], ppvBasis[b][0]);
    }

    ppvBasis[c][0] = XMFLOAT43Normalize(ppvBasis[c][0]);

    float fDet = XMFLOAT4GetX(XMFLOAT4X4Determinant(matTemp));

    // use Kramer's rule to check for handedness of coordinate system
    if (fDet < 0.0f)
    {
        // switch coordinate system by negating the scale and inverting the basis FLOAT4 on the x-axis
        pfScales[a] = -pfScales[a];
        ppvBasis[a][0] = XMFLOAT4Negate(ppvBasis[a][0]);

        fDet = -fDet;
    }

    fDet -= 1.0f;
    fDet *= fDet;

    if (XM3_DECOMP_EPSILON < fDet)
    {
        // Non-SRT matrix encountered
        return false;
    }

    // generate the quaternion from the matrix
    outRotQuat[0] = XMQuaternionRotationMatrix(matTemp);
    return true;
}

#undef XM3_DECOMP_EPSILON
#undef XM3RANKDECOMPOSE

//------------------------------------------------------------------------------
// Transformation operations
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Identity()
{
    XMFLOAT4X4 M;
    M.r[0] = g_XMIdentityR0.v;
    M.r[1] = g_XMIdentityR1.v;
    M.r[2] = g_XMIdentityR2.v;
    M.r[3] = g_XMIdentityR3.v;
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Set
(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
)
{
    XMFLOAT4X4 M;
#if defined(_XM_NO_INTRINSICS_)
    M.m[0][0] = m00; M.m[0][1] = m01; M.m[0][2] = m02; M.m[0][3] = m03;
    M.m[1][0] = m10; M.m[1][1] = m11; M.m[1][2] = m12; M.m[1][3] = m13;
    M.m[2][0] = m20; M.m[2][1] = m21; M.m[2][2] = m22; M.m[2][3] = m23;
    M.m[3][0] = m30; M.m[3][1] = m31; M.m[3][2] = m32; M.m[3][3] = m33;
#else
    M.r[0] = XMFLOAT4Set(m00, m01, m02, m03);
    M.r[1] = XMFLOAT4Set(m10, m11, m12, m13);
    M.r[2] = XMFLOAT4Set(m20, m21, m22, m23);
    M.r[3] = XMFLOAT4Set(m30, m31, m32, m33);
#endif
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Translation
(
    float OffsetX,
    float OffsetY,
    float OffsetZ
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMFLOAT4X4 M;
    M.m[0][0] = 1.0f;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = 1.0f;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = 1.0f;
    M.m[2][3] = 0.0f;

    M.m[3][0] = OffsetX;
    M.m[3][1] = OffsetY;
    M.m[3][2] = OffsetZ;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
    XMFLOAT4X4 M;
    M.r[0] = g_XMIdentityR0.v;
    M.r[1] = g_XMIdentityR1.v;
    M.r[2] = g_XMIdentityR2.v;
    M.r[3] = XMFLOAT4Set(OffsetX, OffsetY, OffsetZ, 1.f);
    return M;
#endif
}


//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4TranslationFromFLOAT4
(
    XMFLOAT4 Offset
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMFLOAT4X4 M;
    M.m[0][0] = 1.0f;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = 1.0f;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = 1.0f;
    M.m[2][3] = 0.0f;

    M.m[3][0] = Offset.FLOAT44_f32[0];
    M.m[3][1] = Offset.FLOAT44_f32[1];
    M.m[3][2] = Offset.FLOAT44_f32[2];
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_SSE_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)
    XMFLOAT4X4 M;
    M.r[0] = g_XMIdentityR0.v;
    M.r[1] = g_XMIdentityR1.v;
    M.r[2] = g_XMIdentityR2.v;
    M.r[3] = XMFLOAT4Select(g_XMIdentityR3.v, Offset, g_XMSelect1110.v);
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Scaling
(
    float ScaleX,
    float ScaleY,
    float ScaleZ
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMFLOAT4X4 M;
    M.m[0][0] = ScaleX;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = ScaleY;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = ScaleZ;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(ScaleX, Zero, 0);
    M.r[1] = vsetq_lane_f32(ScaleY, Zero, 1);
    M.r[2] = vsetq_lane_f32(ScaleZ, Zero, 2);
    M.r[3] = g_XMIdentityR3.v;
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    M.r[0] = _mm_set_ps(0, 0, 0, ScaleX);
    M.r[1] = _mm_set_ps(0, 0, ScaleY, 0);
    M.r[2] = _mm_set_ps(0, ScaleZ, 0, 0);
    M.r[3] = g_XMIdentityR3.v;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4ScalingFromFLOAT4
(
    XMFLOAT4 Scale
)
{
#if defined(_XM_NO_INTRINSICS_)

    XMFLOAT4X4 M;
    M.m[0][0] = Scale.FLOAT44_f32[0];
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = Scale.FLOAT44_f32[1];
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = Scale.FLOAT44_f32[2];
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    XMFLOAT4X4 M;
    M.r[0] = vandq_u32(Scale, g_XMMaskX);
    M.r[1] = vandq_u32(Scale, g_XMMaskY);
    M.r[2] = vandq_u32(Scale, g_XMMaskZ);
    M.r[3] = g_XMIdentityR3.v;
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    M.r[0] = _mm_and_ps(Scale, g_XMMaskX);
    M.r[1] = _mm_and_ps(Scale, g_XMMaskY);
    M.r[2] = _mm_and_ps(Scale, g_XMMaskZ);
    M.r[3] = g_XMIdentityR3.v;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationX
(
    float Angle
)
{
#if defined(_XM_NO_INTRINSICS_)

    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    XMFLOAT4X4 M;
    M.m[0][0] = 1.0f;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = fCosAngle;
    M.m[1][2] = fSinAngle;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = -fSinAngle;
    M.m[2][2] = fCosAngle;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4 T1 = vsetq_lane_f32(fCosAngle, Zero, 1);
    T1 = vsetq_lane_f32(fSinAngle, T1, 2);

    XMFLOAT4 T2 = vsetq_lane_f32(-fSinAngle, Zero, 1);
    T2 = vsetq_lane_f32(fCosAngle, T2, 2);

    XMFLOAT4X4 M;
    M.r[0] = g_XMIdentityR0.v;
    M.r[1] = T1;
    M.r[2] = T2;
    M.r[3] = g_XMIdentityR3.v;
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    float    SinAngle;
    float    CosAngle;
    XMScalarSinCos(&SinAngle, &CosAngle, Angle);

    XMFLOAT4 vSin = _mm_set_ss(SinAngle);
    XMFLOAT4 vCos = _mm_set_ss(CosAngle);
    // x = 0,y = cos,z = sin, w = 0
    vCos = _mm_shuffle_ps(vCos, vSin, _MM_SHUFFLE(3, 0, 0, 3));
    XMFLOAT4X4 M;
    M.r[0] = g_XMIdentityR0;
    M.r[1] = vCos;
    // x = 0,y = sin,z = cos, w = 0
    vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 1, 2, 0));
    // x = 0,y = -sin,z = cos, w = 0
    vCos = _mm_mul_ps(vCos, g_XMNegateY);
    M.r[2] = vCos;
    M.r[3] = g_XMIdentityR3;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationY
(
    float Angle
)
{
#if defined(_XM_NO_INTRINSICS_)

    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    XMFLOAT4X4 M;
    M.m[0][0] = fCosAngle;
    M.m[0][1] = 0.0f;
    M.m[0][2] = -fSinAngle;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = 1.0f;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = fSinAngle;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fCosAngle;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4 T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
    T0 = vsetq_lane_f32(-fSinAngle, T0, 2);

    XMFLOAT4 T2 = vsetq_lane_f32(fSinAngle, Zero, 0);
    T2 = vsetq_lane_f32(fCosAngle, T2, 2);

    XMFLOAT4X4 M;
    M.r[0] = T0;
    M.r[1] = g_XMIdentityR1.v;
    M.r[2] = T2;
    M.r[3] = g_XMIdentityR3.v;
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    float    SinAngle;
    float    CosAngle;
    XMScalarSinCos(&SinAngle, &CosAngle, Angle);

    XMFLOAT4 vSin = _mm_set_ss(SinAngle);
    XMFLOAT4 vCos = _mm_set_ss(CosAngle);
    // x = sin,y = 0,z = cos, w = 0
    vSin = _mm_shuffle_ps(vSin, vCos, _MM_SHUFFLE(3, 0, 3, 0));
    XMFLOAT4X4 M;
    M.r[2] = vSin;
    M.r[1] = g_XMIdentityR1;
    // x = cos,y = 0,z = sin, w = 0
    vSin = XM_PERMUTE_PS(vSin, _MM_SHUFFLE(3, 0, 1, 2));
    // x = cos,y = 0,z = -sin, w = 0
    vSin = _mm_mul_ps(vSin, g_XMNegateZ);
    M.r[0] = vSin;
    M.r[3] = g_XMIdentityR3;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationZ
(
    float Angle
)
{
#if defined(_XM_NO_INTRINSICS_)

    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    XMFLOAT4X4 M;
    M.m[0][0] = fCosAngle;
    M.m[0][1] = fSinAngle;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = -fSinAngle;
    M.m[1][1] = fCosAngle;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = 1.0f;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = 0.0f;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4 T0 = vsetq_lane_f32(fCosAngle, Zero, 0);
    T0 = vsetq_lane_f32(fSinAngle, T0, 1);

    XMFLOAT4 T1 = vsetq_lane_f32(-fSinAngle, Zero, 0);
    T1 = vsetq_lane_f32(fCosAngle, T1, 1);

    XMFLOAT4X4 M;
    M.r[0] = T0;
    M.r[1] = T1;
    M.r[2] = g_XMIdentityR2.v;
    M.r[3] = g_XMIdentityR3.v;
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    float    SinAngle;
    float    CosAngle;
    XMScalarSinCos(&SinAngle, &CosAngle, Angle);

    XMFLOAT4 vSin = _mm_set_ss(SinAngle);
    XMFLOAT4 vCos = _mm_set_ss(CosAngle);
    // x = cos,y = sin,z = 0, w = 0
    vCos = _mm_unpacklo_ps(vCos, vSin);
    XMFLOAT4X4 M;
    M.r[0] = vCos;
    // x = sin,y = cos,z = 0, w = 0
    vCos = XM_PERMUTE_PS(vCos, _MM_SHUFFLE(3, 2, 0, 1));
    // x = cos,y = -sin,z = 0, w = 0
    vCos = _mm_mul_ps(vCos, g_XMNegateX);
    M.r[1] = vCos;
    M.r[2] = g_XMIdentityR2;
    M.r[3] = g_XMIdentityR3;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationRollPitchYaw
(
    float Pitch,
    float Yaw,
    float Roll
)
{
    XMFLOAT4 Angles = XMFLOAT4Set(Pitch, Yaw, Roll, 0.0f);
    return XMFLOAT4X4RotationRollPitchYawFromFLOAT4(Angles);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationRollPitchYawFromFLOAT4
(
    XMFLOAT4 Angles // <Pitch, Yaw, Roll, undefined>
)
{
    XMFLOAT4 Q = XMQuaternionRotationRollPitchYawFromFLOAT4(Angles);
    return XMFLOAT4X4RotationQuaternion(Q);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationNormal
(
    XMFLOAT4 NormalAxis,
    float     Angle
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    XMFLOAT4 A = XMFLOAT4Set(fSinAngle, fCosAngle, 1.0f - fCosAngle, 0.0f);

    XMFLOAT4 C2 = XMFLOAT4SplatZ(A);
    XMFLOAT4 C1 = XMFLOAT4SplatY(A);
    XMFLOAT4 C0 = XMFLOAT4SplatX(A);

    XMFLOAT4 N0 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_W>(NormalAxis);
    XMFLOAT4 N1 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_W>(NormalAxis);

    XMFLOAT4 V0 = XMFLOAT4Multiply(C2, N0);
    V0 = XMFLOAT4Multiply(V0, N1);

    XMFLOAT4 R0 = XMFLOAT4Multiply(C2, NormalAxis);
    R0 = XMFLOAT4MultiplyAdd(R0, NormalAxis, C1);

    XMFLOAT4 R1 = XMFLOAT4MultiplyAdd(C0, NormalAxis, V0);
    XMFLOAT4 R2 = XMFLOAT4NegativeMultiplySubtract(C0, NormalAxis, V0);

    V0 = XMFLOAT4Select(A, R0, g_XMSelect1110.v);
    XMFLOAT4 V1 = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_1Y, XM_PERMUTE_1Z, XM_PERMUTE_0X>(R1, R2);
    XMFLOAT4 V2 = XMFLOAT4Permute<XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_0Y, XM_PERMUTE_1X>(R1, R2);

    XMFLOAT4X4 M;
    M.r[0] = XMFLOAT4Permute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0W>(V0, V1);
    M.r[1] = XMFLOAT4Permute<XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_1W, XM_PERMUTE_0W>(V0, V1);
    M.r[2] = XMFLOAT4Permute<XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z, XM_PERMUTE_0W>(V0, V2);
    M.r[3] = g_XMIdentityR3.v;
    return M;

#elif defined(_XM_SSE_INTRINSICS_)
    float    fSinAngle;
    float    fCosAngle;
    XMScalarSinCos(&fSinAngle, &fCosAngle, Angle);

    XMFLOAT4 C2 = _mm_set_ps1(1.0f - fCosAngle);
    XMFLOAT4 C1 = _mm_set_ps1(fCosAngle);
    XMFLOAT4 C0 = _mm_set_ps1(fSinAngle);

    XMFLOAT4 N0 = XM_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 0, 2, 1));
    XMFLOAT4 N1 = XM_PERMUTE_PS(NormalAxis, _MM_SHUFFLE(3, 1, 0, 2));

    XMFLOAT4 V0 = _mm_mul_ps(C2, N0);
    V0 = _mm_mul_ps(V0, N1);

    XMFLOAT4 R0 = _mm_mul_ps(C2, NormalAxis);
    R0 = _mm_mul_ps(R0, NormalAxis);
    R0 = _mm_add_ps(R0, C1);

    XMFLOAT4 R1 = _mm_mul_ps(C0, NormalAxis);
    R1 = _mm_add_ps(R1, V0);
    XMFLOAT4 R2 = _mm_mul_ps(C0, NormalAxis);
    R2 = _mm_sub_ps(V0, R2);

    V0 = _mm_and_ps(R0, g_XMMask3);
    XMFLOAT4 V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 1, 2, 0));
    V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 3, 2, 1));
    XMFLOAT4 V2 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(0, 0, 1, 1));
    V2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 0, 2, 0));

    R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(1, 0, 3, 0));
    R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 2, 0));

    XMFLOAT4X4 M;
    M.r[0] = R2;

    R2 = _mm_shuffle_ps(V0, V1, _MM_SHUFFLE(3, 2, 3, 1));
    R2 = XM_PERMUTE_PS(R2, _MM_SHUFFLE(1, 3, 0, 2));
    M.r[1] = R2;

    V2 = _mm_shuffle_ps(V2, V0, _MM_SHUFFLE(3, 2, 1, 0));
    M.r[2] = V2;
    M.r[3] = g_XMIdentityR3.v;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationAxis
(
    XMFLOAT4 Axis,
    float     Angle
)
{
    assert(!XMFLOAT43Equal(Axis, XMFLOAT4Zero()));
    assert(!XMFLOAT43IsInfinite(Axis));

    XMFLOAT4 Normal = XMFLOAT43Normalize(Axis);
    return XMFLOAT4X4RotationNormal(Normal, Angle);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4RotationQuaternion
(
    XMFLOAT4 Quaternion
)
{
#if defined(_XM_NO_INTRINSICS_) || defined(_XM_ARM_NEON_INTRINSICS_)

    static const XMFLOAT4 Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

    XMFLOAT4 Q0 = XMFLOAT4Add(Quaternion, Quaternion);
    XMFLOAT4 Q1 = XMFLOAT4Multiply(Quaternion, Q0);

    XMFLOAT4 V0 = XMFLOAT4Permute<XM_PERMUTE_0Y, XM_PERMUTE_0X, XM_PERMUTE_0X, XM_PERMUTE_1W>(Q1, Constant1110.v);
    XMFLOAT4 V1 = XMFLOAT4Permute<XM_PERMUTE_0Z, XM_PERMUTE_0Z, XM_PERMUTE_0Y, XM_PERMUTE_1W>(Q1, Constant1110.v);
    XMFLOAT4 R0 = XMFLOAT4Subtract(Constant1110, V0);
    R0 = XMFLOAT4Subtract(R0, V1);

    V0 = XMFLOAT4Swizzle<XM_SWIZZLE_X, XM_SWIZZLE_X, XM_SWIZZLE_Y, XM_SWIZZLE_W>(Quaternion);
    V1 = XMFLOAT4Swizzle<XM_SWIZZLE_Z, XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_W>(Q0);
    V0 = XMFLOAT4Multiply(V0, V1);

    V1 = XMFLOAT4SplatW(Quaternion);
    XMFLOAT4 V2 = XMFLOAT4Swizzle<XM_SWIZZLE_Y, XM_SWIZZLE_Z, XM_SWIZZLE_X, XM_SWIZZLE_W>(Q0);
    V1 = XMFLOAT4Multiply(V1, V2);

    XMFLOAT4 R1 = XMFLOAT4Add(V0, V1);
    XMFLOAT4 R2 = XMFLOAT4Subtract(V0, V1);

    V0 = XMFLOAT4Permute<XM_PERMUTE_0Y, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z>(R1, R2);
    V1 = XMFLOAT4Permute<XM_PERMUTE_0X, XM_PERMUTE_1Z, XM_PERMUTE_0X, XM_PERMUTE_1Z>(R1, R2);

    XMFLOAT4X4 M;
    M.r[0] = XMFLOAT4Permute<XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0W>(R0, V0);
    M.r[1] = XMFLOAT4Permute<XM_PERMUTE_1Z, XM_PERMUTE_0Y, XM_PERMUTE_1W, XM_PERMUTE_0W>(R0, V0);
    M.r[2] = XMFLOAT4Permute<XM_PERMUTE_1X, XM_PERMUTE_1Y, XM_PERMUTE_0Z, XM_PERMUTE_0W>(R0, V1);
    M.r[3] = g_XMIdentityR3.v;
    return M;

#elif defined(_XM_SSE_INTRINSICS_)
    static const XMFLOAT4  Constant1110 = { { { 1.0f, 1.0f, 1.0f, 0.0f } } };

    XMFLOAT4 Q0 = _mm_add_ps(Quaternion, Quaternion);
    XMFLOAT4 Q1 = _mm_mul_ps(Quaternion, Q0);

    XMFLOAT4 V0 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
    V0 = _mm_and_ps(V0, g_XMMask3);
    XMFLOAT4 V1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
    V1 = _mm_and_ps(V1, g_XMMask3);
    XMFLOAT4 R0 = _mm_sub_ps(Constant1110, V0);
    R0 = _mm_sub_ps(R0, V1);

    V0 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
    V1 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
    V0 = _mm_mul_ps(V0, V1);

    V1 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
    XMFLOAT4 V2 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
    V1 = _mm_mul_ps(V1, V2);

    XMFLOAT4 R1 = _mm_add_ps(V0, V1);
    XMFLOAT4 R2 = _mm_sub_ps(V0, V1);

    V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
    V0 = XM_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
    V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
    V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

    Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
    Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

    XMFLOAT4X4 M;
    M.r[0] = Q1;

    Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
    Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
    M.r[1] = Q1;

    Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
    M.r[2] = Q1;
    M.r[3] = g_XMIdentityR3;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Transformation2D
(
    XMFLOAT4 ScalingOrigin,
    float     ScalingOrientation,
    XMFLOAT4 Scaling,
    XMFLOAT4 RotationOrigin,
    float     Rotation,
    XMFLOAT4 Translation
)
{
    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    XMFLOAT4 VScalingOrigin = XMFLOAT4Select(g_XMSelect1100.v, ScalingOrigin, g_XMSelect1100.v);
    XMFLOAT4 NegScalingOrigin = XMFLOAT4Negate(VScalingOrigin);

    XMFLOAT4X4 MScalingOriginI = XMFLOAT4X4TranslationFromFLOAT4(NegScalingOrigin);
    XMFLOAT4X4 MScalingOrientation = XMFLOAT4X4RotationZ(ScalingOrientation);
    XMFLOAT4X4 MScalingOrientationT = XMFLOAT4X4Transpose(MScalingOrientation);
    XMFLOAT4 VScaling = XMFLOAT4Select(g_XMOne.v, Scaling, g_XMSelect1100.v);
    XMFLOAT4X4 MScaling = XMFLOAT4X4ScalingFromFLOAT4(VScaling);
    XMFLOAT4 VRotationOrigin = XMFLOAT4Select(g_XMSelect1100.v, RotationOrigin, g_XMSelect1100.v);
    XMFLOAT4X4 MRotation = XMFLOAT4X4RotationZ(Rotation);
    XMFLOAT4 VTranslation = XMFLOAT4Select(g_XMSelect1100.v, Translation, g_XMSelect1100.v);

    XMFLOAT4X4 M = XMFLOAT4X4Multiply(MScalingOriginI, MScalingOrientationT);
    M = XMFLOAT4X4Multiply(M, MScaling);
    M = XMFLOAT4X4Multiply(M, MScalingOrientation);
    M.r[3] = XMFLOAT4Add(M.r[3], VScalingOrigin);
    M.r[3] = XMFLOAT4Subtract(M.r[3], VRotationOrigin);
    M = XMFLOAT4X4Multiply(M, MRotation);
    M.r[3] = XMFLOAT4Add(M.r[3], VRotationOrigin);
    M.r[3] = XMFLOAT4Add(M.r[3], VTranslation);

    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Transformation
(
    XMFLOAT4 ScalingOrigin,
    XMFLOAT4 ScalingOrientationQuaternion,
    XMFLOAT4 Scaling,
    XMFLOAT4 RotationOrigin,
    XMFLOAT4 RotationQuaternion,
    XMFLOAT4 Translation
)
{
    // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
    //         MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    XMFLOAT4 VScalingOrigin = XMFLOAT4Select(g_XMSelect1110.v, ScalingOrigin, g_XMSelect1110.v);
    XMFLOAT4 NegScalingOrigin = XMFLOAT4Negate(ScalingOrigin);

    XMFLOAT4X4 MScalingOriginI = XMFLOAT4X4TranslationFromFLOAT4(NegScalingOrigin);
    XMFLOAT4X4 MScalingOrientation = XMFLOAT4X4RotationQuaternion(ScalingOrientationQuaternion);
    XMFLOAT4X4 MScalingOrientationT = XMFLOAT4X4Transpose(MScalingOrientation);
    XMFLOAT4X4 MScaling = XMFLOAT4X4ScalingFromFLOAT4(Scaling);
    XMFLOAT4 VRotationOrigin = XMFLOAT4Select(g_XMSelect1110.v, RotationOrigin, g_XMSelect1110.v);
    XMFLOAT4X4 MRotation = XMFLOAT4X4RotationQuaternion(RotationQuaternion);
    XMFLOAT4 VTranslation = XMFLOAT4Select(g_XMSelect1110.v, Translation, g_XMSelect1110.v);

    XMFLOAT4X4 M;
    M = XMFLOAT4X4Multiply(MScalingOriginI, MScalingOrientationT);
    M = XMFLOAT4X4Multiply(M, MScaling);
    M = XMFLOAT4X4Multiply(M, MScalingOrientation);
    M.r[3] = XMFLOAT4Add(M.r[3], VScalingOrigin);
    M.r[3] = XMFLOAT4Subtract(M.r[3], VRotationOrigin);
    M = XMFLOAT4X4Multiply(M, MRotation);
    M.r[3] = XMFLOAT4Add(M.r[3], VRotationOrigin);
    M.r[3] = XMFLOAT4Add(M.r[3], VTranslation);
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4AffineTransformation2D
(
    XMFLOAT4 Scaling,
    XMFLOAT4 RotationOrigin,
    float     Rotation,
    XMFLOAT4 Translation
)
{
    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    XMFLOAT4 VScaling = XMFLOAT4Select(g_XMOne.v, Scaling, g_XMSelect1100.v);
    XMFLOAT4X4 MScaling = XMFLOAT4X4ScalingFromFLOAT4(VScaling);
    XMFLOAT4 VRotationOrigin = XMFLOAT4Select(g_XMSelect1100.v, RotationOrigin, g_XMSelect1100.v);
    XMFLOAT4X4 MRotation = XMFLOAT4X4RotationZ(Rotation);
    XMFLOAT4 VTranslation = XMFLOAT4Select(g_XMSelect1100.v, Translation, g_XMSelect1100.v);

    XMFLOAT4X4 M;
    M = MScaling;
    M.r[3] = XMFLOAT4Subtract(M.r[3], VRotationOrigin);
    M = XMFLOAT4X4Multiply(M, MRotation);
    M.r[3] = XMFLOAT4Add(M.r[3], VRotationOrigin);
    M.r[3] = XMFLOAT4Add(M.r[3], VTranslation);
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4AffineTransformation
(
    XMFLOAT4 Scaling,
    XMFLOAT4 RotationOrigin,
    XMFLOAT4 RotationQuaternion,
    XMFLOAT4 Translation
)
{
    // M = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

    XMFLOAT4X4 MScaling = XMFLOAT4X4ScalingFromFLOAT4(Scaling);
    XMFLOAT4 VRotationOrigin = XMFLOAT4Select(g_XMSelect1110.v, RotationOrigin, g_XMSelect1110.v);
    XMFLOAT4X4 MRotation = XMFLOAT4X4RotationQuaternion(RotationQuaternion);
    XMFLOAT4 VTranslation = XMFLOAT4Select(g_XMSelect1110.v, Translation, g_XMSelect1110.v);

    XMFLOAT4X4 M;
    M = MScaling;
    M.r[3] = XMFLOAT4Subtract(M.r[3], VRotationOrigin);
    M = XMFLOAT4X4Multiply(M, MRotation);
    M.r[3] = XMFLOAT4Add(M.r[3], VRotationOrigin);
    M.r[3] = XMFLOAT4Add(M.r[3], VTranslation);
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Reflect
(
    XMFLOAT4 ReflectionPlane
)
{
    assert(!XMFLOAT43Equal(ReflectionPlane, XMFLOAT4Zero()));
    assert(!XMPlaneIsInfinite(ReflectionPlane));

    static const XMFLOAT4 NegativeTwo = { { { -2.0f, -2.0f, -2.0f, 0.0f } } };

    XMFLOAT4 P = XMPlaneNormalize(ReflectionPlane);
    XMFLOAT4 S = XMFLOAT4Multiply(P, NegativeTwo);

    XMFLOAT4 A = XMFLOAT4SplatX(P);
    XMFLOAT4 B = XMFLOAT4SplatY(P);
    XMFLOAT4 C = XMFLOAT4SplatZ(P);
    XMFLOAT4 D = XMFLOAT4SplatW(P);

    XMFLOAT4X4 M;
    M.r[0] = XMFLOAT4MultiplyAdd(A, S, g_XMIdentityR0.v);
    M.r[1] = XMFLOAT4MultiplyAdd(B, S, g_XMIdentityR1.v);
    M.r[2] = XMFLOAT4MultiplyAdd(C, S, g_XMIdentityR2.v);
    M.r[3] = XMFLOAT4MultiplyAdd(D, S, g_XMIdentityR3.v);
    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Shadow
(
    XMFLOAT4 ShadowPlane,
    XMFLOAT4 LightPosition
)
{
    static const XMFLOAT4U32 Select0001 = { { { XM_SELECT_0, XM_SELECT_0, XM_SELECT_0, XM_SELECT_1 } } };

    assert(!XMFLOAT43Equal(ShadowPlane, XMFLOAT4Zero()));
    assert(!XMPlaneIsInfinite(ShadowPlane));

    XMFLOAT4 P = XMPlaneNormalize(ShadowPlane);
    XMFLOAT4 Dot = XMPlaneDot(P, LightPosition);
    P = XMFLOAT4Negate(P);
    XMFLOAT4 D = XMFLOAT4SplatW(P);
    XMFLOAT4 C = XMFLOAT4SplatZ(P);
    XMFLOAT4 B = XMFLOAT4SplatY(P);
    XMFLOAT4 A = XMFLOAT4SplatX(P);
    Dot = XMFLOAT4Select(Select0001.v, Dot, Select0001.v);

    XMFLOAT4X4 M;
    M.r[3] = XMFLOAT4MultiplyAdd(D, LightPosition, Dot);
    Dot = XMFLOAT4RotateLeft(Dot, 1);
    M.r[2] = XMFLOAT4MultiplyAdd(C, LightPosition, Dot);
    Dot = XMFLOAT4RotateLeft(Dot, 1);
    M.r[1] = XMFLOAT4MultiplyAdd(B, LightPosition, Dot);
    Dot = XMFLOAT4RotateLeft(Dot, 1);
    M.r[0] = XMFLOAT4MultiplyAdd(A, LightPosition, Dot);
    return M;
}

//------------------------------------------------------------------------------
// View and projection initialization operations
//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4LookAtLH
(
    XMFLOAT4 EyePosition,
    XMFLOAT4 FocusPosition,
    XMFLOAT4 UpDirection
)
{
    XMFLOAT4 EyeDirection = XMFLOAT4Subtract(FocusPosition, EyePosition);
    return XMFLOAT4X4LookToLH(EyePosition, EyeDirection, UpDirection);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4LookAtRH
(
    XMFLOAT4 EyePosition,
    XMFLOAT4 FocusPosition,
    XMFLOAT4 UpDirection
)
{
    XMFLOAT4 NegEyeDirection = XMFLOAT4Subtract(EyePosition, FocusPosition);
    return XMFLOAT4X4LookToLH(EyePosition, NegEyeDirection, UpDirection);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4LookToLH
(
    XMFLOAT4 EyePosition,
    XMFLOAT4 EyeDirection,
    XMFLOAT4 UpDirection
)
{
    assert(!XMFLOAT43Equal(EyeDirection, XMFLOAT4Zero()));
    assert(!XMFLOAT43IsInfinite(EyeDirection));
    assert(!XMFLOAT43Equal(UpDirection, XMFLOAT4Zero()));
    assert(!XMFLOAT43IsInfinite(UpDirection));

    XMFLOAT4 R2 = XMFLOAT43Normalize(EyeDirection);

    XMFLOAT4 R0 = XMFLOAT43Cross(UpDirection, R2);
    R0 = XMFLOAT43Normalize(R0);

    XMFLOAT4 R1 = XMFLOAT43Cross(R2, R0);

    XMFLOAT4 NegEyePosition = XMFLOAT4Negate(EyePosition);

    XMFLOAT4 D0 = XMFLOAT43Dot(R0, NegEyePosition);
    XMFLOAT4 D1 = XMFLOAT43Dot(R1, NegEyePosition);
    XMFLOAT4 D2 = XMFLOAT43Dot(R2, NegEyePosition);

    XMFLOAT4X4 M;
    M.r[0] = XMFLOAT4Select(D0, R0, g_XMSelect1110.v);
    M.r[1] = XMFLOAT4Select(D1, R1, g_XMSelect1110.v);
    M.r[2] = XMFLOAT4Select(D2, R2, g_XMSelect1110.v);
    M.r[3] = g_XMIdentityR3.v;

    M = XMFLOAT4X4Transpose(M);

    return M;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4LookToRH
(
    XMFLOAT4 EyePosition,
    XMFLOAT4 EyeDirection,
    XMFLOAT4 UpDirection
)
{
    XMFLOAT4 NegEyeDirection = XMFLOAT4Negate(EyeDirection);
    return XMFLOAT4X4LookToLH(EyePosition, NegEyeDirection, UpDirection);
}

//------------------------------------------------------------------------------

#ifdef _PREFAST_
#pragma prefast(push)
#pragma prefast(disable:28931, "PREfast noise: Esp:1266")
#endif

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveLH
(
    float ViewWidth,
    float ViewHeight,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (FarZ - NearZ);

    XMFLOAT4X4 M;
    M.m[0][0] = TwoNearZ / ViewWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = TwoNearZ / ViewHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = -fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (FarZ - NearZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, g_XMIdentityR3.v, 2);
    M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (FarZ - NearZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        TwoNearZ / ViewWidth,
        TwoNearZ / ViewHeight,
        fRange,
        -fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // TwoNearZ / ViewWidth,0,0,0
    M.r[0] = vTemp;
    // 0,TwoNearZ / ViewHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=-fRange * NearZ,0,1.0f
    vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,1.0f
    vTemp = _mm_setzero_ps();
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,-fRange * NearZ,0
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveRH
(
    float ViewWidth,
    float ViewHeight,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (NearZ - FarZ);

    XMFLOAT4X4 M;
    M.m[0][0] = TwoNearZ / ViewWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = TwoNearZ / ViewHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = -1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (NearZ - FarZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(TwoNearZ / ViewWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(TwoNearZ / ViewHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, g_XMNegIdentityR3.v, 2);
    M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float TwoNearZ = NearZ + NearZ;
    float fRange = FarZ / (NearZ - FarZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        TwoNearZ / ViewWidth,
        TwoNearZ / ViewHeight,
        fRange,
        fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // TwoNearZ / ViewWidth,0,0,0
    M.r[0] = vTemp;
    // 0,TwoNearZ / ViewHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=-fRange * NearZ,0,-1.0f
    vValues = _mm_shuffle_ps(vValues, g_XMNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,-1.0f
    vTemp = _mm_setzero_ps();
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,-fRange * NearZ,0
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveFovLH
(
    float FovAngleY,
    float AspectRatio,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
    assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

    float Height = CosFov / SinFov;
    float Width = Height / AspectRatio;
    float fRange = FarZ / (FarZ - NearZ);

    XMFLOAT4X4 M;
    M.m[0][0] = Width;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = Height;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = -fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

    float fRange = FarZ / (FarZ - NearZ);
    float Height = CosFov / SinFov;
    float Width = Height / AspectRatio;
    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(Width, Zero, 0);
    M.r[1] = vsetq_lane_f32(Height, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, g_XMIdentityR3.v, 2);
    M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

    float fRange = FarZ / (FarZ - NearZ);
    // Note: This is recorded on the stack
    float Height = CosFov / SinFov;
    XMFLOAT4 rMem = {
        Height / AspectRatio,
        Height,
        fRange,
        -fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // CosFov / SinFov,0,0,0
    XMFLOAT4X4 M;
    M.r[0] = vTemp;
    // 0,Height / AspectRatio,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=-fRange * NearZ,0,1.0f
    vTemp = _mm_setzero_ps();
    vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,1.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,-fRange * NearZ,0.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveFovRH
(
    float FovAngleY,
    float AspectRatio,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
    assert(!XMScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

    float Height = CosFov / SinFov;
    float Width = Height / AspectRatio;
    float fRange = FarZ / (NearZ - FarZ);

    XMFLOAT4X4 M;
    M.m[0][0] = Width;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = Height;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = -1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
    float fRange = FarZ / (NearZ - FarZ);
    float Height = CosFov / SinFov;
    float Width = Height / AspectRatio;
    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(Width, Zero, 0);
    M.r[1] = vsetq_lane_f32(Height, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, g_XMNegIdentityR3.v, 2);
    M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    float    SinFov;
    float    CosFov;
    XMScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);
    float fRange = FarZ / (NearZ - FarZ);
    // Note: This is recorded on the stack
    float Height = CosFov / SinFov;
    XMFLOAT4 rMem = {
        Height / AspectRatio,
        Height,
        fRange,
        fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // CosFov / SinFov,0,0,0
    XMFLOAT4X4 M;
    M.r[0] = vTemp;
    // 0,Height / AspectRatio,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=-fRange * NearZ,0,-1.0f
    vTemp = _mm_setzero_ps();
    vValues = _mm_shuffle_ps(vValues, g_XMNegIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,-1.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,fRange * NearZ,0.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveOffCenterLH
(
    float ViewLeft,
    float ViewRight,
    float ViewBottom,
    float ViewTop,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
    assert(!XMScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (FarZ - NearZ);

    XMFLOAT4X4 M;
    M.m[0][0] = TwoNearZ * ReciprocalWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = TwoNearZ * ReciprocalHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
    M.m[2][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
    M.m[2][2] = fRange;
    M.m[2][3] = 1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = -fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (FarZ - NearZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
    M.r[2] = XMFLOAT4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
        -(ViewTop + ViewBottom) * ReciprocalHeight,
        fRange,
        1.0f);
    M.r[3] = vsetq_lane_f32(-fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (FarZ - NearZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        TwoNearZ * ReciprocalWidth,
        TwoNearZ * ReciprocalHeight,
        -fRange * NearZ,
        0
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // TwoNearZ*ReciprocalWidth,0,0,0
    M.r[0] = vTemp;
    // 0,TwoNearZ*ReciprocalHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // 0,0,fRange,1.0f
    M.r[2] = XMFLOAT4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
        -(ViewTop + ViewBottom) * ReciprocalHeight,
        fRange,
        1.0f);
    // 0,0,-fRange * NearZ,0.0f
    vValues = _mm_and_ps(vValues, g_XMMaskZ);
    M.r[3] = vValues;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4PerspectiveOffCenterRH
(
    float ViewLeft,
    float ViewRight,
    float ViewBottom,
    float ViewTop,
    float NearZ,
    float FarZ
)
{
    assert(NearZ > 0.f && FarZ > 0.f);
    assert(!XMScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
    assert(!XMScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (NearZ - FarZ);

    XMFLOAT4X4 M;
    M.m[0][0] = TwoNearZ * ReciprocalWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = TwoNearZ * ReciprocalHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = (ViewLeft + ViewRight) * ReciprocalWidth;
    M.m[2][1] = (ViewTop + ViewBottom) * ReciprocalHeight;
    M.m[2][2] = fRange;
    M.m[2][3] = -1.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = fRange * NearZ;
    M.m[3][3] = 0.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (NearZ - FarZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);

    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(TwoNearZ * ReciprocalWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(TwoNearZ * ReciprocalHeight, Zero, 1);
    M.r[2] = XMFLOAT4Set((ViewLeft + ViewRight) * ReciprocalWidth,
        (ViewTop + ViewBottom) * ReciprocalHeight,
        fRange,
        -1.0f);
    M.r[3] = vsetq_lane_f32(fRange * NearZ, Zero, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float TwoNearZ = NearZ + NearZ;
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = FarZ / (NearZ - FarZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        TwoNearZ * ReciprocalWidth,
        TwoNearZ * ReciprocalHeight,
        fRange * NearZ,
        0
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // TwoNearZ*ReciprocalWidth,0,0,0
    M.r[0] = vTemp;
    // 0,TwoNearZ*ReciprocalHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // 0,0,fRange,1.0f
    M.r[2] = XMFLOAT4Set((ViewLeft + ViewRight) * ReciprocalWidth,
        (ViewTop + ViewBottom) * ReciprocalHeight,
        fRange,
        -1.0f);
    // 0,0,-fRange * NearZ,0.0f
    vValues = _mm_and_ps(vValues, g_XMMaskZ);
    M.r[3] = vValues;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4OrthographicLH
(
    float ViewWidth,
    float ViewHeight,
    float NearZ,
    float FarZ
)
{
    assert(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float fRange = 1.0f / (FarZ - NearZ);

    XMFLOAT4X4 M;
    M.m[0][0] = 2.0f / ViewWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = 2.0f / ViewHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = -fRange * NearZ;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float fRange = 1.0f / (FarZ - NearZ);

    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
    M.r[3] = vsetq_lane_f32(-fRange * NearZ, g_XMIdentityR3.v, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float fRange = 1.0f / (FarZ - NearZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        2.0f / ViewWidth,
        2.0f / ViewHeight,
        fRange,
        -fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // 2.0f / ViewWidth,0,0,0
    M.r[0] = vTemp;
    // 0,2.0f / ViewHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=-fRange * NearZ,0,1.0f
    vTemp = _mm_setzero_ps();
    vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,0.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,-fRange * NearZ,1.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4OrthographicRH
(
    float ViewWidth,
    float ViewHeight,
    float NearZ,
    float FarZ
)
{
    assert(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float fRange = 1.0f / (NearZ - FarZ);

    XMFLOAT4X4 M;
    M.m[0][0] = 2.0f / ViewWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = 2.0f / ViewHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 0.0f;

    M.m[3][0] = 0.0f;
    M.m[3][1] = 0.0f;
    M.m[3][2] = fRange * NearZ;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float fRange = 1.0f / (NearZ - FarZ);

    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(2.0f / ViewWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(2.0f / ViewHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
    M.r[3] = vsetq_lane_f32(fRange * NearZ, g_XMIdentityR3.v, 2);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float fRange = 1.0f / (NearZ - FarZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        2.0f / ViewWidth,
        2.0f / ViewHeight,
        fRange,
        fRange * NearZ
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // 2.0f / ViewWidth,0,0,0
    M.r[0] = vTemp;
    // 0,2.0f / ViewHeight,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    M.r[1] = vTemp;
    // x=fRange,y=fRange * NearZ,0,1.0f
    vTemp = _mm_setzero_ps();
    vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3, _MM_SHUFFLE(3, 2, 3, 2));
    // 0,0,fRange,0.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 0, 0, 0));
    M.r[2] = vTemp;
    // 0,0,fRange * NearZ,1.0f
    vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 1, 0, 0));
    M.r[3] = vTemp;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4OrthographicOffCenterLH
(
    float ViewLeft,
    float ViewRight,
    float ViewBottom,
    float ViewTop,
    float NearZ,
    float FarZ
)
{
    assert(!XMScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
    assert(!XMScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (FarZ - NearZ);

    XMFLOAT4X4 M;
    M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 0.0f;

    M.m[3][0] = -(ViewLeft + ViewRight) * ReciprocalWidth;
    M.m[3][1] = -(ViewTop + ViewBottom) * ReciprocalHeight;
    M.m[3][2] = -fRange * NearZ;
    M.m[3][3] = 1.0f;
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (FarZ - NearZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
    M.r[3] = XMFLOAT4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
        -(ViewTop + ViewBottom) * ReciprocalHeight,
        -fRange * NearZ,
        1.0f);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (FarZ - NearZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        fReciprocalWidth,
        fReciprocalHeight,
        fRange,
        1.0f
    };
    XMFLOAT4 rMem2 = {
        -(ViewLeft + ViewRight),
        -(ViewTop + ViewBottom),
        -NearZ,
        1.0f
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // fReciprocalWidth*2,0,0,0
    vTemp = _mm_add_ss(vTemp, vTemp);
    M.r[0] = vTemp;
    // 0,fReciprocalHeight*2,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    vTemp = _mm_add_ps(vTemp, vTemp);
    M.r[1] = vTemp;
    // 0,0,fRange,0.0f
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskZ);
    M.r[2] = vTemp;
    // -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
    vValues = _mm_mul_ps(vValues, rMem2);
    M.r[3] = vValues;
    return M;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4OrthographicOffCenterRH
(
    float ViewLeft,
    float ViewRight,
    float ViewBottom,
    float ViewTop,
    float NearZ,
    float FarZ
)
{
    assert(!XMScalarNearEqual(ViewRight, ViewLeft, 0.00001f));
    assert(!XMScalarNearEqual(ViewTop, ViewBottom, 0.00001f));
    assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

#if defined(_XM_NO_INTRINSICS_)

    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (NearZ - FarZ);

    XMFLOAT4X4 M;
    M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
    M.m[0][1] = 0.0f;
    M.m[0][2] = 0.0f;
    M.m[0][3] = 0.0f;

    M.m[1][0] = 0.0f;
    M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
    M.m[1][2] = 0.0f;
    M.m[1][3] = 0.0f;

    M.m[2][0] = 0.0f;
    M.m[2][1] = 0.0f;
    M.m[2][2] = fRange;
    M.m[2][3] = 0.0f;

    M.r[3] = XMFLOAT4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
        -(ViewTop + ViewBottom) * ReciprocalHeight,
        fRange * NearZ,
        1.0f);
    return M;

#elif defined(_XM_ARM_NEON_INTRINSICS_)
    float ReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float ReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (NearZ - FarZ);
    const XMFLOAT4 Zero = vdupq_n_f32(0);
    XMFLOAT4X4 M;
    M.r[0] = vsetq_lane_f32(ReciprocalWidth + ReciprocalWidth, Zero, 0);
    M.r[1] = vsetq_lane_f32(ReciprocalHeight + ReciprocalHeight, Zero, 1);
    M.r[2] = vsetq_lane_f32(fRange, Zero, 2);
    M.r[3] = XMFLOAT4Set(-(ViewLeft + ViewRight) * ReciprocalWidth,
        -(ViewTop + ViewBottom) * ReciprocalHeight,
        fRange * NearZ,
        1.0f);
    return M;
#elif defined(_XM_SSE_INTRINSICS_)
    XMFLOAT4X4 M;
    float fReciprocalWidth = 1.0f / (ViewRight - ViewLeft);
    float fReciprocalHeight = 1.0f / (ViewTop - ViewBottom);
    float fRange = 1.0f / (NearZ - FarZ);
    // Note: This is recorded on the stack
    XMFLOAT4 rMem = {
        fReciprocalWidth,
        fReciprocalHeight,
        fRange,
        1.0f
    };
    XMFLOAT4 rMem2 = {
        -(ViewLeft + ViewRight),
        -(ViewTop + ViewBottom),
        NearZ,
        1.0f
    };
    // Copy from memory to SSE register
    XMFLOAT4 vValues = rMem;
    XMFLOAT4 vTemp = _mm_setzero_ps();
    // Copy x only
    vTemp = _mm_move_ss(vTemp, vValues);
    // fReciprocalWidth*2,0,0,0
    vTemp = _mm_add_ss(vTemp, vTemp);
    M.r[0] = vTemp;
    // 0,fReciprocalHeight*2,0,0
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskY);
    vTemp = _mm_add_ps(vTemp, vTemp);
    M.r[1] = vTemp;
    // 0,0,fRange,0.0f
    vTemp = vValues;
    vTemp = _mm_and_ps(vTemp, g_XMMaskZ);
    M.r[2] = vTemp;
    // -(ViewLeft + ViewRight)*fReciprocalWidth,-(ViewTop + ViewBottom)*fReciprocalHeight,fRange*-NearZ,1.0f
    vValues = _mm_mul_ps(vValues, rMem2);
    M.r[3] = vValues;
    return M;
#endif
}

#ifdef _PREFAST_
#pragma prefast(pop)
#endif

/****************************************************************************
 *
 * XMFLOAT4X4 operators and methods
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------

inline XMFLOAT4X4::XMFLOAT4X4
(
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33
)
{
    r[0] = XMFLOAT4Set(m00, m01, m02, m03);
    r[1] = XMFLOAT4Set(m10, m11, m12, m13);
    r[2] = XMFLOAT4Set(m20, m21, m22, m23);
    r[3] = XMFLOAT4Set(m30, m31, m32, m33);
}

//------------------------------------------------------------------------------
_Use_decl_annotations_
inline XMFLOAT4X4::XMFLOAT4X4
(
    const float* pArray
)
{
    assert(pArray != nullptr);
    r[0] = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pArray));
    r[1] = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pArray + 4));
    r[2] = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pArray + 8));
    r[3] = XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pArray + 12));
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XMFLOAT4X4::operator- () const
{
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Negate(r[0]);
    R.r[1] = XMFLOAT4Negate(r[1]);
    R.r[2] = XMFLOAT4Negate(r[2]);
    R.r[3] = XMFLOAT4Negate(r[3]);
    return R;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4& XM_CALLCONV XMFLOAT4X4::operator+= (XMFLOAT4X4 M)
{
    r[0] = XMFLOAT4Add(r[0], M.r[0]);
    r[1] = XMFLOAT4Add(r[1], M.r[1]);
    r[2] = XMFLOAT4Add(r[2], M.r[2]);
    r[3] = XMFLOAT4Add(r[3], M.r[3]);
    return *this;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4& XM_CALLCONV XMFLOAT4X4::operator-= (XMFLOAT4X4 M)
{
    r[0] = XMFLOAT4Subtract(r[0], M.r[0]);
    r[1] = XMFLOAT4Subtract(r[1], M.r[1]);
    r[2] = XMFLOAT4Subtract(r[2], M.r[2]);
    r[3] = XMFLOAT4Subtract(r[3], M.r[3]);
    return *this;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4& XM_CALLCONV XMFLOAT4X4::operator*=(XMFLOAT4X4 M)
{
    *this = XMFLOAT4X4Multiply(*this, M);
    return *this;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4& XMFLOAT4X4::operator*= (float S)
{
    r[0] = XMFLOAT4Scale(r[0], S);
    r[1] = XMFLOAT4Scale(r[1], S);
    r[2] = XMFLOAT4Scale(r[2], S);
    r[3] = XMFLOAT4Scale(r[3], S);
    return *this;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4& XMFLOAT4X4::operator/= (float S)
{
#if defined(_XM_NO_INTRINSICS_)
    XMFLOAT4 vS = XMFLOAT4Replicate(S);
    r[0] = XMFLOAT4Divide(r[0], vS);
    r[1] = XMFLOAT4Divide(r[1], vS);
    r[2] = XMFLOAT4Divide(r[2], vS);
    r[3] = XMFLOAT4Divide(r[3], vS);
    return *this;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
    float32x4_t vS = vdupq_n_f32(S);
    r[0] = vdivq_f32(r[0], vS);
    r[1] = vdivq_f32(r[1], vS);
    r[2] = vdivq_f32(r[2], vS);
    r[3] = vdivq_f32(r[3], vS);
#else
    // 2 iterations of Newton-Raphson refinement of reciprocal
    float32x2_t vS = vdup_n_f32(S);
    float32x2_t R0 = vrecpe_f32(vS);
    float32x2_t S0 = vrecps_f32(R0, vS);
    R0 = vmul_f32(S0, R0);
    S0 = vrecps_f32(R0, vS);
    R0 = vmul_f32(S0, R0);
    float32x4_t Reciprocal = vcombine_u32(R0, R0);
    r[0] = vmulq_f32(r[0], Reciprocal);
    r[1] = vmulq_f32(r[1], Reciprocal);
    r[2] = vmulq_f32(r[2], Reciprocal);
    r[3] = vmulq_f32(r[3], Reciprocal);
#endif
    return *this;
#elif defined(_XM_SSE_INTRINSICS_)
    __m128 vS = _mm_set_ps1(S);
    r[0] = _mm_div_ps(r[0], vS);
    r[1] = _mm_div_ps(r[1], vS);
    r[2] = _mm_div_ps(r[2], vS);
    r[3] = _mm_div_ps(r[3], vS);
    return *this;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4::operator+ (XMFLOAT4X4 M) const
{
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Add(r[0], M.r[0]);
    R.r[1] = XMFLOAT4Add(r[1], M.r[1]);
    R.r[2] = XMFLOAT4Add(r[2], M.r[2]);
    R.r[3] = XMFLOAT4Add(r[3], M.r[3]);
    return R;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4::operator- (XMFLOAT4X4 M) const
{
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Subtract(r[0], M.r[0]);
    R.r[1] = XMFLOAT4Subtract(r[1], M.r[1]);
    R.r[2] = XMFLOAT4Subtract(r[2], M.r[2]);
    R.r[3] = XMFLOAT4Subtract(r[3], M.r[3]);
    return R;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4::operator*(XMFLOAT4X4 M) const
{
    return XMFLOAT4X4Multiply(*this, M);
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XMFLOAT4X4::operator* (float S) const
{
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Scale(r[0], S);
    R.r[1] = XMFLOAT4Scale(r[1], S);
    R.r[2] = XMFLOAT4Scale(r[2], S);
    R.r[3] = XMFLOAT4Scale(r[3], S);
    return R;
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XMFLOAT4X4::operator/ (float S) const
{
#if defined(_XM_NO_INTRINSICS_)
    XMFLOAT4 vS = XMFLOAT4Replicate(S);
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Divide(r[0], vS);
    R.r[1] = XMFLOAT4Divide(r[1], vS);
    R.r[2] = XMFLOAT4Divide(r[2], vS);
    R.r[3] = XMFLOAT4Divide(r[3], vS);
    return R;
#elif defined(_XM_ARM_NEON_INTRINSICS_)
#if defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
    float32x4_t vS = vdupq_n_f32(S);
    XMFLOAT4X4 R;
    R.r[0] = vdivq_f32(r[0], vS);
    R.r[1] = vdivq_f32(r[1], vS);
    R.r[2] = vdivq_f32(r[2], vS);
    R.r[3] = vdivq_f32(r[3], vS);
#else
    // 2 iterations of Newton-Raphson refinement of reciprocal
    float32x2_t vS = vdup_n_f32(S);
    float32x2_t R0 = vrecpe_f32(vS);
    float32x2_t S0 = vrecps_f32(R0, vS);
    R0 = vmul_f32(S0, R0);
    S0 = vrecps_f32(R0, vS);
    R0 = vmul_f32(S0, R0);
    float32x4_t Reciprocal = vcombine_u32(R0, R0);
    XMFLOAT4X4 R;
    R.r[0] = vmulq_f32(r[0], Reciprocal);
    R.r[1] = vmulq_f32(r[1], Reciprocal);
    R.r[2] = vmulq_f32(r[2], Reciprocal);
    R.r[3] = vmulq_f32(r[3], Reciprocal);
#endif
    return R;
#elif defined(_XM_SSE_INTRINSICS_)
    __m128 vS = _mm_set_ps1(S);
    XMFLOAT4X4 R;
    R.r[0] = _mm_div_ps(r[0], vS);
    R.r[1] = _mm_div_ps(r[1], vS);
    R.r[2] = _mm_div_ps(r[2], vS);
    R.r[3] = _mm_div_ps(r[3], vS);
    return R;
#endif
}

//------------------------------------------------------------------------------

inline XMFLOAT4X4 XM_CALLCONV operator*
(
    float S,
    XMFLOAT4X4 M
    )
{
    XMFLOAT4X4 R;
    R.r[0] = XMFLOAT4Scale(M.r[0], S);
    R.r[1] = XMFLOAT4Scale(M.r[1], S);
    R.r[2] = XMFLOAT4Scale(M.r[2], S);
    R.r[3] = XMFLOAT4Scale(M.r[3], S);
    return R;
}

/****************************************************************************
 *
 * XMFLOAT3X3 operators
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
_Use_decl_annotations_
inline XMFLOAT3X3::XMFLOAT3X3
(
    const float* pArray
)
{
    assert(pArray != nullptr);
    for (size_t Row = 0; Row < 3; Row++)
    {
        for (size_t Column = 0; Column < 3; Column++)
        {
            m[Row][Column] = pArray[Row * 3 + Column];
        }
    }
}

/****************************************************************************
 *
 * XMFLOAT4X3 operators
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
_Use_decl_annotations_
inline XMFLOAT4X3::XMFLOAT4X3
(
    const float* pArray
)
{
    assert(pArray != nullptr);

    m[0][0] = pArray[0];
    m[0][1] = pArray[1];
    m[0][2] = pArray[2];

    m[1][0] = pArray[3];
    m[1][1] = pArray[4];
    m[1][2] = pArray[5];

    m[2][0] = pArray[6];
    m[2][1] = pArray[7];
    m[2][2] = pArray[8];

    m[3][0] = pArray[9];
    m[3][1] = pArray[10];
    m[3][2] = pArray[11];
}

/****************************************************************************
*
* XMFLOAT3X4 operators
*
****************************************************************************/

//------------------------------------------------------------------------------
_Use_decl_annotations_
inline XMFLOAT3X4::XMFLOAT3X4
(
    const float* pArray
)
{
    assert(pArray != nullptr);

    m[0][0] = pArray[0];
    m[0][1] = pArray[1];
    m[0][2] = pArray[2];
    m[0][3] = pArray[3];

    m[1][0] = pArray[4];
    m[1][1] = pArray[5];
    m[1][2] = pArray[6];
    m[1][3] = pArray[7];

    m[2][0] = pArray[8];
    m[2][1] = pArray[9];
    m[2][2] = pArray[10];
    m[2][3] = pArray[11];
}

/****************************************************************************
 *
 * XMFLOAT4X4 operators
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
_Use_decl_annotations_
inline XMFLOAT4X4::XMFLOAT4X4
(
    const float* pArray
)
{
    assert(pArray != nullptr);

    m[0][0] = pArray[0];
    m[0][1] = pArray[1];
    m[0][2] = pArray[2];
    m[0][3] = pArray[3];

    m[1][0] = pArray[4];
    m[1][1] = pArray[5];
    m[1][2] = pArray[6];
    m[1][3] = pArray[7];

    m[2][0] = pArray[8];
    m[2][1] = pArray[9];
    m[2][2] = pArray[10];
    m[2][3] = pArray[11];

    m[3][0] = pArray[12];
    m[3][1] = pArray[13];
    m[3][2] = pArray[14];
    m[3][3] = pArray[15];
}

