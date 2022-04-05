#ifndef WOW_BLS_H
#define WOW_BLS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

enum wow_bls_pixel_block
{
	WOW_BLS_PIXEL_PS_1_1,
	WOW_BLS_PIXEL_PS_1_2,
	WOW_BLS_PIXEL_PS_1_3,
	WOW_BLS_PIXEL_PS_1_4,
	WOW_BLS_PIXEL_PS_2_0,
	WOW_BLS_PIXEL_NV_REGISTER,
	WOW_BLS_PIXEL_NV_TEXTURE1,
	WOW_BLS_PIXEL_NV_TEXTURE2,
	WOW_BLS_PIXEL_NV_TEXTURE3,
	WOW_BLS_PIXEL_ATIFS,
	WOW_BLS_PIXEL_ARBFP1_0,
	WOW_BLS_PIXEL_UNK1,
};

enum wow_bls_vertex_block
{
	WOW_BLS_VERTEX_VS_1_1,
	WOW_BLS_VERTEX_VS_2_0,
	WOW_BLS_VERTEX_ARBVP1_0,
	WOW_BLS_VERTEX_UNK1,
	WOW_BLS_VERTEX_UNK2,
	WOW_BLS_VERTEX_UNK3,
};

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_bls_header_head_s
{
	uint32_t magic;
	uint32_t version;
	uint32_t permutation_count;
} wow_bls_header_head_t;

typedef struct wow_bls_header_vs_s
{
	uint32_t offsets[6];
} wow_bls_header_vs_t;

typedef struct wow_bls_header_ps_s
{
	uint32_t offsets[12];
} wow_bls_header_ps_t;

typedef struct wow_bls_header_s
{
	wow_bls_header_head_t header;
	union
	{
		wow_bls_header_vs_t vs;
		wow_bls_header_ps_t ps;
	};
} wow_bls_header_t;

enum wow_bls_shader_param_type
{
	WOW_BLS_SHADER_PARAM_VEC4    = 0x0,
	WOW_BLS_SHADER_PARAM_MAT34   = 0x1,
	WOW_BLS_SHADER_PARAM_MAT44   = 0x2,
	WOW_BLS_SHADER_PARAM_TEXTURE = 0x3,
	WOW_BLS_SHADER_PARAM_BUMPMAT = 0x4,
	WOW_BLS_SHADER_PARAM_VEC3    = 0x5,
	WOW_BLS_SHADER_PARAM_VEC2    = 0x6,
	WOW_BLS_SHADER_PARAM_VEC1    = 0x7,
	WOW_BLS_SHADER_PARAM_MAT33   = 0x8,
	WOW_BLS_SHADER_PARAM_STRUCT  = 0x9,
	WOW_BLS_SHADER_PARAM_ARRAY   = 0xA,
};

typedef struct wow_bls_shader_param_s
{
	char name[64];
	uint32_t binding;
	float f[16];
	uint32_t type;
	uint32_t unk1;
	uint32_t unk2;
} wow_bls_shader_param_t;

typedef struct wow_bls_shader_s
{
	wow_bls_shader_param_t *consts;
	uint32_t consts_nb;
	wow_bls_shader_param_t *params;
	uint32_t params_nb;
	uint8_t *code;
	uint32_t code_len;
} wow_bls_shader_t;

typedef struct wow_bls_block_s
{
	wow_bls_shader_t *shaders;
	uint32_t shaders_nb;
} wow_bls_block_t;

typedef struct wow_bls_file_s
{
	wow_bls_header_t header;
	wow_bls_block_t *blocks;
	uint32_t blocks_nb;
} wow_bls_file_t;

wow_bls_file_t *wow_bls_file_new(wow_mpq_file_t *mpq);
void wow_bls_file_delete(wow_bls_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
