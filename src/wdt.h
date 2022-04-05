#ifndef WOW_WDT_H
#define WOW_WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_file_s wow_mpq_file_t;

#define WOW_MPHD_FLAG_WMO        0x01
#define WOW_MPHD_FLAG_MCCV       0x02
#define WOW_MPHD_FLAG_BIG_ALPHA  0x04
#define WOW_MPHD_FLAG_M2_SORT    0x08
#define WOW_MPHD_FLAG_MCLV       0x10
#define WOW_MPHD_FLAG_REVERSE    0x20
#define WOW_MPHD_FLAG_0x40       0x40
#define WOW_MPHD_FLAG_HEIGHT_TEX 0x80

typedef struct wow_mphd_s
{
	wow_chunk_header_t header;
	uint32_t flags;
	uint32_t something;
	uint32_t padding[6];
} wow_mphd_t;

#define WOW_MAIN_FLAG_ADT 0x1

typedef struct wow_main_s
{
	wow_chunk_header_t header;
	struct
	{
		uint32_t flags;
		uint32_t padding;
	} data[64 * 64];
} wow_main_t;

typedef struct wow_wdt_file_s
{
	wow_mver_t mver;
	wow_mphd_t mphd;
	wow_main_t main;
	wow_mwmo_t mwmo;
	wow_modf_t modf;
} wow_wdt_file_t;

wow_wdt_file_t *wow_wdt_file_new(wow_mpq_file_t *mpq);
void wow_wdt_file_delete(wow_wdt_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
