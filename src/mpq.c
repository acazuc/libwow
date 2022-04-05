#include "mpq.h"

#include "common.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <zlib.h>

static uint32_t crypt_table[0x500];

static bool file_seek(FILE *file, uint32_t off)
{
#ifdef _WIN32
	return _fseeki64(file, off, SEEK_SET);
#else
	return fseek(file, off, SEEK_SET);
#endif
}

void wow_mpq_init_crypt_table()
{
	uint32_t seed = 0x00100001;
	for (uint32_t i = 0; i < 0x100; ++i)
	{
		for (uint32_t j = 0; j <= 4; ++j)
		{
			uint32_t idx = i + 0x100 * j;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			uint32_t tmp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			uint32_t tmp2 = (seed & 0xFFFF);
			tmp1 |= tmp2;
			crypt_table[idx] = tmp1;
		}
	}
}

static void decrypt_table(void *data, size_t len, uint32_t key)
{
	uint32_t seed = 0xEEEEEEEE;
	uint32_t *wdata = (uint32_t*)data;
	len /= 4;
	for (size_t i = 0; i < len; ++i)
	{
		seed += crypt_table[WOW_MPQ_CRYPT_OFFSET_DECRYPT_TABLE + (key & 0xFF)];
		uint32_t ch = wdata[i] ^ (key + seed);
		wdata[i] = ch;
		key = ((~key << 0x15) + 0x11111111) | (key >> 0x0B);
		seed = ch + seed + (seed << 5) + 3;
	}
}

uint32_t wow_mpq_hash_string(const char *str, uint32_t hash_type)
{
	uint32_t seed1 = 0x7FED7FED;
	uint32_t seed2 = 0xEEEEEEEE;
	while (*str)
	{
		uint8_t ch = toupper(*str++);
		seed1 = crypt_table[hash_type + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

wow_mpq_archive_t *wow_mpq_archive_new(const char *filename)
{
	wow_mpq_archive_t *archive = WOW_MALLOC(sizeof(*archive));
	FILE *file = NULL;
	if (!archive)
		return NULL;
	archive->block_table = NULL;
	archive->hash_table = NULL;
	archive->filename = WOW_MALLOC(strlen(filename) + 1);
	if (!archive->filename)
		goto err;
	strcpy(archive->filename, filename);
	file = fopen(filename, "rb");
	if (!file)
		goto err;
	if (!fread((char*)&archive->header, sizeof(archive->header), 1, file))
		goto err;
	if (archive->header.format_version >= 1)
	{
		if (!fread((char*)&archive->header2, sizeof(archive->header2), 1, file))
			goto err;
	}
	if (archive->header.block_table_size)
	{
		archive->block_table = WOW_MALLOC(sizeof(*archive->block_table) * archive->header.block_table_size);
		if (file_seek(file, archive->header.block_table_pos))
			goto err;
		if (!fread((char*)archive->block_table, sizeof(*archive->block_table) * archive->header.block_table_size, 1, file))
			goto err;
		decrypt_table(archive->block_table, sizeof(*archive->block_table) * archive->header.block_table_size, WOW_MPQ_KEY_BLOCK_TABLE);
	}
	if (archive->header.hash_table_size)
	{
		archive->hash_table = WOW_MALLOC(sizeof(*archive->hash_table) * archive->header.hash_table_size);
		if (file_seek(file, archive->header.hash_table_pos))
			goto err;
		if (!fread((char*)archive->hash_table, sizeof(*archive->hash_table) * archive->header.hash_table_size, 1, file))
			goto err;
		decrypt_table(archive->hash_table, sizeof(*archive->hash_table) * archive->header.hash_table_size, WOW_MPQ_KEY_HASH_TABLE);
	}
	fclose(file);
	return archive;

err:
	if (file)
		fclose(file);
	wow_mpq_archive_delete(archive);
	return NULL;
}

void wow_mpq_archive_delete(wow_mpq_archive_t *archive)
{
	if (archive == NULL)
		return;
	WOW_FREE(archive->block_table);
	WOW_FREE(archive->hash_table);
	WOW_FREE(archive->filename);
	WOW_FREE(archive);
}

wow_mpq_compound_t *wow_mpq_compound_new()
{
	wow_mpq_compound_t *compound = WOW_MALLOC(sizeof(*compound));
	if (!compound)
		return NULL;
	compound->archives = NULL;
	compound->archives_nb = 0;
	return compound;
}

void wow_mpq_compound_delete(wow_mpq_compound_t *compound)
{
	if (!compound)
		return;
	for (size_t i = 0; i < compound->archives_nb; ++i)
	{
		WOW_FREE(compound->archives[i].buffer);
		fclose(compound->archives[i].file);
	}
	WOW_FREE(compound->archives);
	WOW_FREE(compound);
}

bool wow_mpq_compound_add_archive(wow_mpq_compound_t *compound, const wow_mpq_archive_t *archive)
{
	wow_mpq_archive_view_t *archives = WOW_REALLOC(compound->archives, sizeof(*compound->archives) * (compound->archives_nb + 1));
	if (!archives)
		return false;
	compound->archives = archives;
	compound->archives[compound->archives_nb].archive = archive;
	compound->archives[compound->archives_nb].buffer = WOW_MALLOC((size_t)512 << archive->header.block_size);
	if (!compound->archives[compound->archives_nb].buffer)
		return false;
	compound->archives[compound->archives_nb].file = fopen(archive->filename, "rb");
	if (!compound->archives[compound->archives_nb].file)
	{
		WOW_FREE(compound->archives[compound->archives_nb].buffer);
		return false; /* don't care of realloc since archives_nb isn't updated */
	}
	compound->archives_nb++;
	return true;
}

static bool read_sector(const wow_mpq_block_t *block, uint32_t offset, uint32_t in_size, uint32_t out_size, FILE *file, uint8_t *data, size_t *data_size, uint8_t *buffer)
{
	/* XXX: check for CRC */
	if (file_seek(file, offset))
		return false;
	uint8_t compression = 0;
	if ((block->flags & WOW_MPQ_BLOCK_COMPRESS) && in_size < out_size)
	{
		in_size--;
		if (!fread((char*)&compression, 1, 1, file))
			return false;
	}
	switch (compression)
	{
		case WOW_MPQ_COMPRESSION_NONE:
		{
			size_t old_size = *data_size;
			*data_size = *data_size + out_size;
			if (!fread((char*)(data + old_size), out_size, 1, file))
				return false;
			break;
		}
		case WOW_MPQ_COMPRESSION_ZLIB:
		{
			if (!fread((char*)buffer, in_size, 1, file))
				return false;
			z_stream zstream;
			memset(&zstream, 0, sizeof(zstream));
			if (inflateInit(&zstream) != Z_OK)
				return false;
			zstream.avail_in = in_size;
			zstream.next_in = buffer;
			zstream.avail_out = out_size;
			zstream.next_out = data + *data_size;
			int ret = inflate(&zstream,  Z_FINISH);
			inflateEnd(&zstream);
			if (ret != Z_STREAM_END)
				return false;
			*data_size += out_size - zstream.avail_out;
			inflateEnd(&zstream);
			break;
		}
		default:
			return false;
	}
	return true;
}

static uint8_t *read_block(const wow_mpq_archive_view_t *archive_view, const wow_mpq_block_t *block, size_t *data_size)
{
	uint8_t *data = WOW_MALLOC(block->file_size);
	if (!data)
		return NULL;
	uint32_t max_sector_size = 512 << archive_view->archive->header.block_size;
	uint32_t *sectors = NULL;
	uint32_t sectors_nb = 0;
	uint32_t sectors_capacity = 2 + (block->file_size + max_sector_size - 1) / max_sector_size;
	uint32_t tmp;
	*data_size = 0;
	if (!(block->flags & WOW_MPQ_BLOCK_COMPRESS) || (block->flags & WOW_MPQ_BLOCK_SINGLE_UNIT))
	{
		uint8_t *buffer;
		if (block->flags & WOW_MPQ_BLOCK_COMPRESS)
		{
			/* buffer is too short for full file */
			buffer = WOW_MALLOC(block->block_size);
			if (!buffer)
				goto err;
		}
		else
		{
			buffer = archive_view->buffer;
		}
		if (!read_sector(block, block->offset, block->block_size, block->file_size, archive_view->file, data, data_size, buffer))
		{
			if (buffer != archive_view->buffer)
				WOW_FREE(buffer);
			goto err;
		}
		if (buffer != archive_view->buffer)
			WOW_FREE(buffer);
		return data;
	}
	if (file_seek(archive_view->file, block->offset))
	{
		WOW_FREE(data);
		return NULL;
	}
	sectors = WOW_MALLOC(sizeof(*sectors) * sectors_capacity);
	if (!sectors)
		goto err;
	while (true)
	{
		if (!fread((char*)&tmp, sizeof(tmp), 1, archive_view->file))
			goto err;
		/*if (block.flags & WOW_MPQ_BLOCK_ENCRYPTED)
			decrypt_table(&val, sizeof(val), KEY_BLOCK_TABLE);*/
		sectors[sectors_nb++] = tmp;
		if (tmp > block->block_size)
			goto err;
		if (tmp == block->block_size)
			break;
	}
	if (block->flags & WOW_MPQ_BLOCK_SECTOR_CRC)
		sectors_nb--; /* XXX: don't bother with CRC */
	for (size_t i = 0; i < sectors_nb; ++i)
	{
		uint32_t offset = block->offset + sectors[i];
		uint32_t in_size = sectors[i + 1] - sectors[i];
		uint32_t out_size = block->file_size - *data_size;
		if (out_size > max_sector_size)
			out_size = max_sector_size;
		if (!read_sector(block, offset, in_size, out_size, archive_view->file, data, data_size, archive_view->buffer))
			goto err;
		if (*data_size >= block->file_size)
			break;
	}
	WOW_FREE(sectors);
	return data;

err:
	WOW_FREE(sectors);
	WOW_FREE(data);
	return NULL;
}

static const wow_mpq_block_t *get_block(const wow_mpq_archive_t *archive, uint32_t bucket, uint32_t name_a, uint32_t name_b)
{
	if (!archive->hash_table || !archive->block_table)
		return NULL;
	uint32_t start_hash = bucket & (archive->header.hash_table_size - 1);
	for (uint32_t i = start_hash; i < archive->header.hash_table_size; ++i)
	{
		const wow_mpq_hash_t *hash = &archive->hash_table[i];
		if (hash->name_a == name_a && hash->name_b == name_b)
		{
			uint32_t block_index = hash->block_index;
			if (block_index >= archive->header.block_table_size)
				return NULL;
			const wow_mpq_block_t *block = &archive->block_table[block_index];
			if (block->flags & WOW_MPQ_BLOCK_DELETE_MARKER)
				return NULL;
			return block;
		}
		if (hash->block_index == 0xFFFFFFFF)
			return NULL;
	}
	return NULL;
}

wow_mpq_file_t *get_file(const wow_mpq_archive_view_t *archive_view, uint32_t bucket, uint32_t name_a, uint32_t name_b)
{
	const wow_mpq_block_t *block = get_block(archive_view->archive, bucket, name_a, name_b);
	if (!block)
		return NULL;
	size_t data_size;
	uint8_t *data = read_block(archive_view, block, &data_size);
	if (!data)
		return NULL;
	wow_mpq_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
	{
		WOW_FREE(data);
		return NULL;
	}
	file->data = data;
	file->size = data_size;
	file->pos = 0;
	return file;
}

const wow_mpq_block_t *wow_mpq_get_archive_block(const wow_mpq_archive_view_t *archive_view, const char *filename)
{
	uint32_t bucket = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_BUCKET);
	uint32_t name_a = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_A);
	uint32_t name_b = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_B);
	return get_block(archive_view->archive, bucket, name_a, name_b);
}

wow_mpq_file_t *wow_mpq_get_archive_file(const wow_mpq_archive_view_t *archive_view, const char *filename)
{
	uint32_t bucket = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_BUCKET);
	uint32_t name_a = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_A);
	uint32_t name_b = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_B);
	return get_file(archive_view, bucket, name_a, name_b);
}

const wow_mpq_block_t *wow_mpq_get_block(const wow_mpq_compound_t *compound, const char *filename)
{
	uint32_t bucket = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_BUCKET);
	uint32_t name_a = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_A);
	uint32_t name_b = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_B);
	for (size_t i = 0; i < compound->archives_nb; ++i)
	{
		const wow_mpq_block_t *block = get_block(compound->archives[i].archive, bucket, name_a, name_b);
		if (block)
			return block;
	}
	return NULL;
}

wow_mpq_file_t *wow_mpq_get_file(const wow_mpq_compound_t *compound, const char *filename)
{
	uint32_t bucket = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_BUCKET);
	uint32_t name_a = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_A);
	uint32_t name_b = wow_mpq_hash_string(filename, WOW_MPQ_CRYPT_OFFSET_HASH_NAME_B);
	for (size_t i = 0; i < compound->archives_nb; ++i)
	{
		wow_mpq_file_t *file = get_file(&compound->archives[i], bucket, name_a, name_b);
		if (file)
			return file;
	}
	return NULL;
}

void wow_mpq_file_delete(wow_mpq_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file->data);
	WOW_FREE(file);
}

uint32_t wow_mpq_read(wow_mpq_file_t *file, void *data, uint32_t size)
{
	if (size > file->size - file->pos)
		size = file->size - file->pos;
	if (!size)
		return 0;
	memcpy(data, file->data + file->pos, size);
	file->pos += size;
	return size;
}

int32_t wow_mpq_seek(wow_mpq_file_t *file, int32_t offset, int32_t whence)
{
	uint32_t base;
	switch (whence)
	{
		case SEEK_SET:
			base = 0;
			break;
		case SEEK_CUR:
			base = file->pos;
			break;
		case SEEK_END:
			base = file->size;
			break;
		default:
			return -1;
	}
	if (offset < 0 && (uint32_t)-offset > base)
		return -1;
	if (offset > 0 && base + offset > file->size)
		return -1;
	file->pos = base + offset;
	return file->pos;
}
