#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer cameraAndLights
// {
//
//   struct SCENE_DATA
//   {
//       
//       float4 lightDirection;         // Offset:    0
//       float4 lightColor;             // Offset:   16
//       float4 lightAmbient;           // Offset:   32
//       float4 lightPos;               // Offset:   48
//       float4x4 gwViewMatrix;         // Offset:   64
//       float4x4 gwProjectionMatrix;   // Offset:  128
//       float4 padding[4];             // Offset:  192
//
//   } cameraAndLights;                 // Offset:    0 Size:   256
//
// }
//
// cbuffer meshInfo
// {
//
//   struct MESH_DATA
//   {
//       
//       float4x4 gwWorldMatrix;        // Offset:    0
//       float4x4 gwWorldMatrixTwo;     // Offset:   64
//       
//       struct OBJ_ATTRIBUTES
//       {
//           
//           float3 Kd;                 // Offset:  128
//           float d;                   // Offset:  140
//           float3 Ks;                 // Offset:  144
//           float Ns;                  // Offset:  156
//           float3 Ka;                 // Offset:  160
//           float sharpness;           // Offset:  172
//           float3 Tf;                 // Offset:  176
//           float Ni;                  // Offset:  188
//           float3 Ke;                 // Offset:  192
//           uint illum;                // Offset:  204
//
//       } material;                    // Offset:  128
//       uint padding[12];              // Offset:  208
//
//   } meshInfo;                        // Offset:    0 Size:   388
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      ID      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- ------- -------------- ------
// cameraAndLights                   cbuffer      NA          NA     CB0            cb0      1 
// meshInfo                          cbuffer      NA          NA     CB1            cb1      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// NORMAL                   0   xyz         1     NONE   float   xyz 
// WORLD                    0   xyz         2     NONE   float   xyz 
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_5_1
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[0:0][4], immediateIndexed, space=0
dcl_constantbuffer CB1[1:1][13], immediateIndexed, space=0
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xyz
dcl_output o0.xyzw
dcl_temps 2
dp3 r0.x, v1.xyzx, v1.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, v1.xyzx
dp3_sat r0.w, -CB0[0][0].xyzx, r0.xyzx
mul r0.w, r0.w, CB0[0][1].x
add r1.xyz, -v2.xyzx, CB0[0][3].xyzx
dp3 r1.w, r1.xyzx, r1.xyzx
rsq r1.w, r1.w
mad r1.xyz, r1.xyzx, r1.wwww, -CB0[0][0].xyzx
dp3 r1.w, r1.xyzx, r1.xyzx
rsq r1.w, r1.w
mul r1.xyz, r1.wwww, r1.xyzx
dp3_sat r0.x, r1.xyzx, r0.xyzx
log r0.x, r0.x
mul r0.x, r0.x, CB1[1][9].w
exp r0.x, r0.x
mul r1.xyz, CB0[0][1].xyzx, CB1[1][9].xyzx
mul r0.xyz, r0.xxxx, r1.xyzx
mad_sat r1.xyz, r0.wwww, CB1[1][8].xxxx, CB0[0][2].xyzx
mad r0.xyz, r1.xyzx, CB1[1][8].xyzx, r0.xyzx
add o0.xyz, r0.xyzx, CB1[1][12].xyzx
mov o0.w, l(0)
ret 
// Approximately 23 instruction slots used
#endif

const BYTE PixelShader[] =
{
     68,  88,  66,  67, 175, 105, 
    185, 145, 186, 251, 187, 242, 
     84,  10, 164,  89,  57,  39, 
    146, 103,   1,   0,   0,   0, 
     88,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    228,   4,   0,   0,  88,   5, 
      0,   0, 140,   5,   0,   0, 
    188,   8,   0,   0,  82,  68, 
     69,  70, 168,   4,   0,   0, 
      2,   0,   0,   0, 168,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   1,   5, 
    255, 255,   0,   5,   0,   0, 
    128,   4,   0,   0,  19,  19, 
     68,  37,  60,   0,   0,   0, 
     24,   0,   0,   0,  40,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    140,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 156,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  99,  97, 109, 101, 
    114,  97,  65, 110, 100,  76, 
    105, 103, 104, 116, 115,   0, 
    109, 101, 115, 104,  73, 110, 
    102, 111,   0, 171, 171, 171, 
    140,   0,   0,   0,   1,   0, 
      0,   0, 216,   0,   0,   0, 
      0,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   0,   0,   0,   1,   0, 
      0,   0,  92,   2,   0,   0, 
    144,   1,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    140,   0,   0,   0,   0,   0, 
      0,   0,   0,   1,   0,   0, 
      2,   0,   0,   0,  56,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  83,  67, 
     69,  78,  69,  95,  68,  65, 
     84,  65,   0, 108, 105, 103, 
    104, 116,  68, 105, 114, 101, 
     99, 116, 105, 111, 110,   0, 
    102, 108, 111,  97, 116,  52, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     26,   1,   0,   0, 108, 105, 
    103, 104, 116,  67, 111, 108, 
    111, 114,   0, 108, 105, 103, 
    104, 116,  65, 109,  98, 105, 
    101, 110, 116,   0, 108, 105, 
    103, 104, 116,  80, 111, 115, 
      0, 103, 119,  86, 105, 101, 
    119,  77,  97, 116, 114, 105, 
    120,   0, 102, 108, 111,  97, 
    116,  52, 120,  52,   0, 171, 
      3,   0,   3,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 118,   1,   0,   0, 
    103, 119,  80, 114, 111, 106, 
    101,  99, 116, 105, 111, 110, 
     77,  97, 116, 114, 105, 120, 
      0, 112,  97, 100, 100, 105, 
    110, 103,   0, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     26,   1,   0,   0,  11,   1, 
      0,   0,  36,   1,   0,   0, 
      0,   0,   0,   0,  72,   1, 
      0,   0,  36,   1,   0,   0, 
     16,   0,   0,   0,  83,   1, 
      0,   0,  36,   1,   0,   0, 
     32,   0,   0,   0,  96,   1, 
      0,   0,  36,   1,   0,   0, 
     48,   0,   0,   0, 105,   1, 
      0,   0, 128,   1,   0,   0, 
     64,   0,   0,   0, 164,   1, 
      0,   0, 128,   1,   0,   0, 
    128,   0,   0,   0, 183,   1, 
      0,   0, 192,   1,   0,   0, 
    192,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,  64,   0, 
      0,   0,   7,   0, 228,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   1,   0,   0, 156,   0, 
      0,   0,   0,   0,   0,   0, 
    132,   1,   0,   0,   2,   0, 
      0,   0,  92,   4,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  77,  69,  83,  72, 
     95,  68,  65,  84,  65,   0, 
    103, 119,  87, 111, 114, 108, 
    100,  77,  97, 116, 114, 105, 
    120,   0, 103, 119,  87, 111, 
    114, 108, 100,  77,  97, 116, 
    114, 105, 120,  84, 119, 111, 
      0, 109,  97, 116, 101, 114, 
    105,  97, 108,   0,  79,  66, 
     74,  95,  65,  84,  84,  82, 
     73,  66,  85,  84,  69,  83, 
      0,  75, 100,   0, 102, 108, 
    111,  97, 116,  51,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 200,   2,   0,   0, 
    100,   0, 102, 108, 111,  97, 
    116,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 246,   2, 
      0,   0,  75, 115,   0,  78, 
    115,   0,  75,  97,   0, 115, 
    104,  97, 114, 112, 110, 101, 
    115, 115,   0,  84, 102,   0, 
     78, 105,   0,  75, 101,   0, 
    105, 108, 108, 117, 109,   0, 
    100, 119, 111, 114, 100,   0, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  66,   3,   0,   0, 
    197,   2,   0,   0, 208,   2, 
      0,   0,   0,   0,   0,   0, 
    244,   2,   0,   0, 252,   2, 
      0,   0,  12,   0,   0,   0, 
     32,   3,   0,   0, 208,   2, 
      0,   0,  16,   0,   0,   0, 
     35,   3,   0,   0, 252,   2, 
      0,   0,  28,   0,   0,   0, 
     38,   3,   0,   0, 208,   2, 
      0,   0,  32,   0,   0,   0, 
     41,   3,   0,   0, 252,   2, 
      0,   0,  44,   0,   0,   0, 
     51,   3,   0,   0, 208,   2, 
      0,   0,  48,   0,   0,   0, 
     54,   3,   0,   0, 252,   2, 
      0,   0,  60,   0,   0,   0, 
     57,   3,   0,   0, 208,   2, 
      0,   0,  64,   0,   0,   0, 
     60,   3,   0,   0,  72,   3, 
      0,   0,  76,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
     20,   0,   0,   0,  10,   0, 
    108,   3,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 182,   2,   0,   0, 
      0,   0,  19,   0,   1,   0, 
      1,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  66,   3,   0,   0, 
    142,   2,   0,   0, 128,   1, 
      0,   0,   0,   0,   0,   0, 
    156,   2,   0,   0, 128,   1, 
      0,   0,  64,   0,   0,   0, 
    173,   2,   0,   0, 228,   3, 
      0,   0, 128,   0,   0,   0, 
    183,   1,   0,   0,   8,   4, 
      0,   0, 208,   0,   0,   0, 
      5,   0,   0,   0,   1,   0, 
     64,   0,   0,   0,   4,   0, 
     44,   4,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 132,   2,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78, 108,   0,   0,   0, 
      3,   0,   0,   0,   8,   0, 
      0,   0,  80,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  92,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   7,   7, 
      0,   0,  99,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,   7,   7, 
      0,   0,  83,  86,  95,  80, 
     79,  83,  73,  84,  73,  79, 
     78,   0,  78,  79,  82,  77, 
     65,  76,   0,  87,  79,  82, 
     76,  68,   0, 171, 171, 171, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171,  83,  72, 
     69,  88,  40,   3,   0,   0, 
     81,   0,   0,   0, 202,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   7,  70, 142, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  89,   0, 
      0,   7,  70, 142,  48,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0,   0,   0, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   1,   0, 
      0,   0,  98,  16,   0,   3, 
    114,  16,  16,   0,   2,   0, 
      0,   0, 101,   0,   0,   3, 
    242,  32,  16,   0,   0,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  16,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  18, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  18,  16,   0,   1,   0, 
      0,   0,  16,  32,   0,  10, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70, 130,  48, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  10, 128,  48,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  18,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
     70, 130,  48,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  12, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   1,   0, 
      0,   0,  70, 130,  48, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  68,   0,   0,   5, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,  32,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     47,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   9, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  58, 128, 
     48,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   9,   0, 
      0,   0,  25,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,  11, 114,   0,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  70, 130,  48,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   9,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,  32, 
      0,  13, 114,   0,  16,   0, 
      1,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
      6, 128,  48,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  70, 130, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  50,   0,   0,  11, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     48,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   9, 114,  32,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  48,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     12,   0,   0,   0,  54,   0, 
      0,   5, 130,  32,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     23,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  21,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0
};
