#include "bls.h"

#include "common.h"
#include "mpq.h"

#include <stdlib.h>
#include <string.h>

static bool read_params(wow_bls_shader_param_t **params, uint32_t *nb, wow_mpq_file_t *mpq)
{
	wow_mpq_read(mpq, nb, sizeof(*nb));
	*params = WOW_MALLOC(sizeof(**params) * *nb);
	if (!*params)
		return false;
	wow_mpq_read(mpq, *params, sizeof(**params) * *nb);
	return true;
}

static bool read_block(wow_bls_file_t *file, wow_bls_block_t *block, wow_mpq_file_t *mpq)
{
	block->shaders_nb = file->header.header.permutation_count;
	block->shaders = WOW_MALLOC(sizeof(*block->shaders) * block->shaders_nb);
	if (!block->shaders)
		return false;
	memset(block->shaders, 0, sizeof(*block->shaders) * block->shaders_nb);
	for (uint32_t i = 0; i < block->shaders_nb; ++i)
	{
		wow_bls_shader_t *shader = &block->shaders[i];
		if (!read_params(&shader->params, &shader->params_nb, mpq))
			return false;
		if (!read_params(&shader->consts, &shader->consts_nb, mpq))
			return false;
		uint32_t unk;
		wow_mpq_read(mpq, &unk, sizeof(unk)); /* 2 for abs, 0 for dx9 / combiners */
		wow_mpq_read(mpq, &shader->code_len, sizeof(shader->code_len));
		shader->code = WOW_MALLOC(shader->code_len);
		if (!shader->code)
			return false;
		wow_mpq_read(mpq, shader->code, shader->code_len);
		if (shader->code_len % 4 != 0)
		{
			char pad[4];
			wow_mpq_read(mpq, pad, 4 - shader->code_len % 4);
		}
	}
	return true;
}

wow_bls_file_t *wow_bls_file_new(wow_mpq_file_t *mpq)
{
	wow_bls_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	file->blocks_nb = 0;
	file->blocks = NULL;
	wow_mpq_read(mpq, &file->header.header, sizeof(file->header.header));
	if (file->header.header.magic == (('S' << 0) | ('V' << 8) | ('X' << 16) | ('G' << 24)))
	{
		wow_mpq_read(mpq, &file->header.vs, sizeof(file->header.vs));
		file->blocks_nb = 6;
		file->blocks = WOW_MALLOC(sizeof(*file->blocks) * file->blocks_nb);
		if (!file->blocks)
		{
			wow_bls_file_delete(file);
			return NULL;
		}
		memset(file->blocks, 0, sizeof(*file->blocks) * file->blocks_nb);
		for (uint32_t i = 0; i < file->blocks_nb; ++i)
		{
			if (!file->header.vs.offsets[i])
				continue;
			wow_mpq_seek(mpq, file->header.vs.offsets[i], SEEK_SET);
			if (!read_block(file, &file->blocks[i], mpq))
			{
				wow_bls_file_delete(file);
				return NULL;
			}
		}
	}
	else if (file->header.header.magic == (('S' << 0) | ('P' << 8) | ('X' << 16) | ('G' << 24)))
	{
		wow_mpq_read(mpq, &file->header.ps, sizeof(file->header.ps));
		file->blocks_nb = 12;
		file->blocks = WOW_MALLOC(sizeof(*file->blocks) * file->blocks_nb);
		if (!file->blocks)
		{
			wow_bls_file_delete(file);
			return NULL;
		}
		memset(file->blocks, 0, sizeof(*file->blocks) * file->blocks_nb);
		for (uint32_t i = 0; i < file->blocks_nb; ++i)
		{
			if (!file->header.ps.offsets[i])
				continue;
			wow_mpq_seek(mpq, file->header.ps.offsets[i], SEEK_SET);
			if (!read_block(file, &file->blocks[i], mpq))
			{
				wow_bls_file_delete(file);
				return NULL;
			}
		}
	}
	else
	{
		wow_bls_file_delete(file);
		return NULL;
	}
	return file;
}

void wow_bls_file_delete(wow_bls_file_t *file)
{
	if (!file)
		return;
	for (size_t i = 0; i < file->blocks_nb; ++i)
	{
		for (size_t j = 0; j < file->blocks[i].shaders_nb; ++j)
		{
			WOW_FREE(file->blocks[i].shaders[j].consts);
			WOW_FREE(file->blocks[i].shaders[j].params);
			WOW_FREE(file->blocks[i].shaders[j].code);
		}
		WOW_FREE(file->blocks[i].shaders);
	}
	WOW_FREE(file->blocks);
	WOW_FREE(file);
}
