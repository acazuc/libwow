#include "blp.h"

#include "common.h"
#include "mpq.h"

#include <string.h>
#include <stdlib.h>

wow_blp_file_t *wow_blp_file_new(wow_mpq_file_t *mpq)
{
	wow_blp_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	file->mipmaps = NULL;
	file->mipmaps_nb = 0;
	if (wow_mpq_read(mpq, &file->header, sizeof(file->header)) != sizeof(file->header))
		goto err;
	if (file->header.type != 1)
		goto err;
	if (file->header.has_mipmaps)
	{
		file->mipmaps_nb = 0;
		while (file->mipmaps_nb < 16 && file->header.mipmap_offsets[file->mipmaps_nb] && file->header.mipmap_lengths[file->mipmaps_nb] && file->header.width / (1 << file->mipmaps_nb) && file->header.height / (1 << file->mipmaps_nb))
			++file->mipmaps_nb;
	}
	else
	{
		file->mipmaps_nb = 1;
	}
	file->mipmaps = WOW_MALLOC(sizeof(*file->mipmaps) * file->mipmaps_nb);
	if (!file->mipmaps)
		goto err;
	memset(file->mipmaps, 0, sizeof(*file->mipmaps) * file->mipmaps_nb);
	switch (file->header.compression)
	{
		case 1:
		{
			for (uint32_t i = 0; i < file->mipmaps_nb; ++i)
			{
				wow_blp_mipmap_t *mipmap = &file->mipmaps[i];
				mipmap->width = file->header.width / (1 << i);
				mipmap->height = file->header.height / (1 << i);
				mipmap->data_len = file->header.mipmap_lengths[i];
				mipmap->data = WOW_MALLOC(mipmap->data_len);
				if (!mipmap->data)
					goto err;
				wow_mpq_seek(mpq, file->header.mipmap_offsets[i], SEEK_SET);
				wow_mpq_read(mpq, mipmap->data, file->header.mipmap_lengths[i]);
			}
			break;
		}
		case 2:
		{
			uint32_t width = file->header.width;
			uint32_t height = file->header.height;
			uint32_t mult;
			switch (file->header.alpha_type)
			{
				case 0:
					mult = 8;
					break;
				case 1:
					mult = 16;
					break;
				case 7:
					mult = 16;
					break;
				default:
					goto err;
			}
			for (uint32_t i = 0; i < file->mipmaps_nb; ++i)
			{
				wow_blp_mipmap_t *mipmap = &file->mipmaps[i];
				mipmap->width = width;
				mipmap->height = height;
				mipmap->data_len = ((width + 3) / 4) * ((height + 3) / 4) * mult;
				mipmap->data = WOW_MALLOC(mipmap->data_len);
				if (!mipmap->data)
					goto err;
				wow_mpq_seek(mpq, file->header.mipmap_offsets[i], SEEK_SET);
				wow_mpq_read(mpq, mipmap->data, file->header.mipmap_lengths[i]);
				width /= 2;
				height /= 2;
				if (width == 0)
					width = 1;
				if (height == 0)
					height = 1;
			}
			break;
		}
		case 3:
		{
			for (uint32_t i = 0; i < file->mipmaps_nb; ++i)
			{
				wow_blp_mipmap_t *mipmap = &file->mipmaps[i];
				mipmap->width = file->header.width / (1 << i);
				mipmap->height = file->header.height / (1 << i);
				mipmap->data_len = file->header.mipmap_lengths[i];
				mipmap->data = WOW_MALLOC(mipmap->data_len);
				if (!mipmap->data)
					goto err;
				wow_mpq_seek(mpq, file->header.mipmap_offsets[i], SEEK_SET);
				wow_mpq_read(mpq, mipmap->data, file->header.mipmap_lengths[i]);
			}
			break;
		}
	}
	return file;

err:
	wow_blp_file_delete(file);
	return NULL;
}

void wow_blp_file_delete(wow_blp_file_t *file)
{
	if (!file)
		return;
	if (file->mipmaps)
	{
		for (uint32_t i = 0; i < file->mipmaps_nb; ++i)
			WOW_FREE(file->mipmaps[i].data);
		WOW_FREE(file->mipmaps);
	}
	WOW_FREE(file);
}
