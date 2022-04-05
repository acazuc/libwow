#ifndef WOW_WDL_H
#define WOW_WDL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_mare_s
{
	wow_chunk_header_t header;
	int16_t data[17 * 17 + 16 * 16];
} wow_mare_t;

typedef struct wow_maof_s
{
	wow_chunk_header_t header;
	uint32_t offsets[64][64];
} wow_maof_t;

typedef struct wow_wdl_file_s
{
	wow_mver_t mver;
	wow_maof_t maof;
	wow_mare_t mare[64][64];
} wow_wdl_file_t;

wow_wdl_file_t *wow_wdl_file_new(wow_mpq_file_t *mpq);
void wow_wdl_file_delete(wow_wdl_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
