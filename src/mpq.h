#ifndef WOW_MPQ_H
#define WOW_MPQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define WOW_MPQ_CRYPT_OFFSET_HASH_BUCKET   0x000
#define WOW_MPQ_CRYPT_OFFSET_HASH_NAME_A   0x100
#define WOW_MPQ_CRYPT_OFFSET_HASH_NAME_B   0x200
#define WOW_MPQ_CRYPT_OFFSET_HASH_FILE_KEY 0x300
#define WOW_MPQ_CRYPT_OFFSET_DECRYPT_TABLE 0x400

#define WOW_MPQ_KEY_HASH_TABLE  0xC3AF3770 /* hash("(hash table)" , WOW_MPQ_CRYPT_OFFSET_HASH_FILE_KEY) */
#define WOW_MPQ_KEY_BLOCK_TABLE 0xEC83B3A3 /* hash("(block table)", WOW_MPQ_CRYPT_OFFSET_HASH_FILE_KEY) */

#define WOW_MPQ_BLOCK_IMPLODE       0x00000100
#define WOW_MPQ_BLOCK_COMPRESS      0x00000200
#define WOW_MPQ_BLOCK_ENCRYPTED     0x00010000
#define WOW_MPQ_BLOCK_FIX_KEY       0x00020000
#define WOW_MPQ_BLOCK_PATCH_FILE    0x00100000
#define WOW_MPQ_BLOCK_DELETE_MARKER 0x02000000
#define WOW_MPQ_BLOCK_SINGLE_UNIT   0x01000000
#define WOW_MPQ_BLOCK_SECTOR_CRC    0x04000000
#define WOW_MPQ_BLOCK_EXISTS        0x80000000

#define WOW_MPQ_COMPRESSION_NONE   0x00
#define WOW_MPQ_COMPRESSION_ZLIB   0x02
#define WOW_MPQ_COMPRESSION_PKWARE 0x08
#define WOW_MPQ_COMPRESSION_BZIP2  0x10
#define WOW_MPQ_COMPRESSION_SPARSE 0x20
#define WOW_MPQ_COMPRESSION_LZMA   0x12

typedef struct wow_mpq_header_s
{
	uint32_t id;
	uint32_t header_size;
	uint32_t archive_size;
	uint16_t format_version;
	uint16_t block_size;
	uint32_t hash_table_pos;
	uint32_t block_table_pos;
	uint32_t hash_table_size;
	uint32_t block_table_size;
} wow_mpq_header_t;

typedef struct wow_mpq_header2_s
{
	uint64_t hi_block_table_pos;
	uint16_t hash_table_posHi;
	uint16_t block_table_posHi;
} wow_mpq_header2_t;

typedef struct wow_mpq_block_s
{
	uint32_t offset;
	uint32_t block_size;
	uint32_t file_size;
	uint32_t flags;
} wow_mpq_block_t;

typedef struct wow_mpq_hash_s
{
	uint32_t name_a;
	uint32_t name_b;
	uint16_t lc_locale;
	uint16_t platform;
	uint32_t block_index;
} wow_mpq_hash_t;

typedef struct wow_mpq_archive_s
{
	wow_mpq_header2_t header2;
	wow_mpq_header_t header;
	wow_mpq_block_t *block_table;
	wow_mpq_hash_t *hash_table;
	char *filename;
} wow_mpq_archive_t;

typedef struct wow_mpq_archive_view_s
{
	const wow_mpq_archive_t *archive;
	uint8_t *buffer; /* compression buffer */
	FILE *file;
} wow_mpq_archive_view_t;

typedef struct wow_mpq_compound_s
{
	wow_mpq_archive_view_t *archives;
	uint32_t archives_nb;
} wow_mpq_compound_t;

typedef struct wow_mpq_file_s
{
	uint8_t *data;
	uint32_t size;
	uint32_t pos;
} wow_mpq_file_t;

void wow_mpq_init_crypt_table();
uint32_t wow_mpq_hash_string(const char *str, uint32_t hash_type);

wow_mpq_archive_t *wow_mpq_archive_new(const char *filename);
void wow_mpq_archive_delete(wow_mpq_archive_t *archive);

wow_mpq_compound_t *wow_mpq_compound_new();
void wow_mpq_compound_delete(wow_mpq_compound_t *compound);
bool wow_mpq_compound_add_archive(wow_mpq_compound_t *compound, const wow_mpq_archive_t *archive);
const wow_mpq_block_t *wow_mpq_get_archive_block(const wow_mpq_archive_view_t *archive, const char *filename);
wow_mpq_file_t *wow_mpq_get_archive_file(const wow_mpq_archive_view_t *archive, const char *filename);
const wow_mpq_block_t *wow_mpq_get_block(const wow_mpq_compound_t *compound, const char *filename);
wow_mpq_file_t *wow_mpq_get_file(const wow_mpq_compound_t *compound, const char *filename);

void wow_mpq_file_delete(wow_mpq_file_t *file);
uint32_t wow_mpq_read(wow_mpq_file_t *file, void *data, uint32_t len);
int32_t wow_mpq_seek(wow_mpq_file_t *file, int32_t offset, int32_t whence);

#ifdef __cplusplus
}
#endif

#endif
