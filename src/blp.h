#ifndef WOW_BLP_H
#define WOW_BLP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_blp_header_s
{
	uint8_t magic[4];
	uint32_t type;
	uint8_t compression;
	uint8_t alpha_depth;
	uint8_t alpha_type;
	uint8_t has_mipmaps;
	uint32_t width;
	uint32_t height;
	uint32_t mipmap_offsets[16];
	uint32_t mipmap_lengths[16];
	uint32_t palette[256];
} wow_blp_header_t;

typedef struct wow_blp_mipmap_s
{
	uint32_t width;
	uint32_t height;
	uint8_t *data;
	uint32_t data_len;
} wow_blp_mipmap_t;

typedef struct wow_blp_file_s
{
	wow_blp_header_t header;
	wow_blp_mipmap_t *mipmaps;
	uint32_t mipmaps_nb;
} wow_blp_file_t;

wow_blp_file_t *wow_blp_file_new(wow_mpq_file_t *mpq);
void wow_blp_file_delete(wow_blp_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
