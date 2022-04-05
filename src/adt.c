#include "adt.h"

#include "common.h"
#include "mpq.h"

#include <string.h>
#include <stdlib.h>

wow_adt_file_t *wow_adt_file_new(wow_mpq_file_t *mpq)
{
	wow_adt_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	file->mwmo.data = NULL;
	file->modf.data = NULL;
	file->mmdx.data = NULL;
	file->mmid.data = NULL;
	file->mddf.data = NULL;
	file->mwid.data = NULL;
	for (uint32_t i = 0; i < 16 * 16; ++i)
	{
		wow_mcnk_t *mcnk = &file->mcnk[i];
		mcnk->mcly.data = NULL;
		mcnk->mcrf.doodads = NULL;
		mcnk->mcrf.wmos = NULL;
		mcnk->mcal.data = NULL;
		mcnk->mcse.data = NULL;
		mcnk->mclq.data = NULL;
	}
	file->mtex.data = NULL;
	file->textures = NULL;
	wow_mpq_read(mpq, &file->mver, sizeof(file->mver));
	wow_mpq_read(mpq, &file->mhdr, sizeof(file->mhdr));
	size_t mhdr_base = sizeof(file->mver) + sizeof(wow_chunk_header_t);
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mcin, SEEK_SET);
	wow_mpq_read(mpq, &file->mcin, sizeof(file->mcin));
	/* WMO
	 */
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mwmo, SEEK_SET);
	wow_mpq_read(mpq, &file->mwmo.header, sizeof(file->mwmo.header));
	file->mwmo.data_len = file->mwmo.header.size / sizeof(*file->mwmo.data);
	file->mwmo.data = WOW_MALLOC(file->mwmo.header.size);
	if (!file->mwmo.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mwmo.data, file->mwmo.header.size);
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mwid, SEEK_SET);
	wow_mpq_read(mpq, &file->mwid.header, sizeof(file->mwid.header));
	file->mwid.data_nb = file->mwid.header.size / sizeof(*file->mwid.data);
	file->mwid.data = WOW_MALLOC(file->mwid.header.size);
	if (!file->mwid.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mwid.data, file->mwid.header.size);
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.modf, SEEK_SET);
	wow_mpq_read(mpq, &file->modf.header, sizeof(file->modf.header));
	file->modf.data_nb = file->modf.header.size / sizeof(*file->modf.data);
	file->modf.data = WOW_MALLOC(file->modf.header.size);
	if (!file->modf.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->modf.data, file->modf.header.size);
	/* M2
	 */
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mmdx, SEEK_SET);
	wow_mpq_read(mpq, &file->mmdx.header, sizeof(file->mmdx.header));
	file->mmdx.data_len = file->mmdx.header.size;
	file->mmdx.data = WOW_MALLOC(file->mmdx.header.size);
	if (!file->mmdx.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mmdx.data, file->mmdx.header.size);
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mmid, SEEK_SET);
	wow_mpq_read(mpq, &file->mmid.header, sizeof(file->mmid.header));
	file->mmid.data_nb = file->mmid.header.size / sizeof(*file->mmid.data);
	file->mmid.data = WOW_MALLOC(file->mmid.header.size);
	if (!file->mmid.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mmid.data, file->mmid.header.size);
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mddf, SEEK_SET);
	wow_mpq_read(mpq, &file->mddf.header, sizeof(file->mddf.header));
	file->mddf.data_nb = file->mddf.header.size / sizeof(*file->mddf.data);
	file->mddf.data = WOW_MALLOC(file->mddf.header.size);
	if (!file->mddf.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mddf.data, file->mddf.header.size);
	for (size_t i = 0; i < 16 * 16; ++i)
	{
		wow_mcin_data_t *mcin = &file->mcin.data[i];
		wow_mcnk_t *mcnk = &file->mcnk[i];
		wow_mpq_seek(mpq, mcin->offset, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->header, sizeof(mcnk->header));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcvt, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcvt, sizeof(mcnk->mcvt));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcnr, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcnr, sizeof(mcnk->mcnr));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcly, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcly.header, sizeof(mcnk->mcly.header));

		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcrf, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcrf.header, sizeof(mcnk->mcrf.header));
		mcnk->mcrf.doodads_nb = mcnk->header.doodad_refs_nb;
		mcnk->mcrf.doodads = WOW_MALLOC(sizeof(*mcnk->mcrf.doodads) * mcnk->mcrf.doodads_nb);
		if (mcnk->mcrf.doodads == NULL)
		{
			wow_adt_file_delete(file);
			return NULL;
		}
		wow_mpq_read(mpq, mcnk->mcrf.doodads, sizeof(*mcnk->mcrf.doodads) * mcnk->mcrf.doodads_nb);
		mcnk->mcrf.wmos_nb = mcnk->header.map_obj_refs_nb;
		mcnk->mcrf.wmos = WOW_MALLOC(sizeof(*mcnk->mcrf.wmos) * mcnk->mcrf.wmos_nb);
		if (!mcnk->mcrf.wmos)
		{
			wow_adt_file_delete(file);
			return NULL;
		}
		wow_mpq_read(mpq, mcnk->mcrf.wmos, sizeof(*mcnk->mcrf.wmos) * mcnk->mcrf.wmos_nb);

		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcal, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcal.header, sizeof(mcnk->mcal.header));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcsh, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcsh, sizeof(mcnk->mcsh));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcse, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mcse.header, sizeof(mcnk->mcse.header));
		wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mclq, SEEK_SET);
		wow_mpq_read(mpq, &mcnk->mclq.header, sizeof(mcnk->mclq.header));
		mcnk->mclq.data_nb = 0;
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_RIVER)
			mcnk->mclq.data_nb++;
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_OCEAN)
			mcnk->mclq.data_nb++;
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_MAGMA)
			mcnk->mclq.data_nb++;
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_SLIME)
			mcnk->mclq.data_nb++;
		mcnk->mclq.data = WOW_MALLOC(sizeof(*mcnk->mclq.data) * mcnk->mclq.data_nb);
		if (!mcnk->mclq.data)
		{
			wow_adt_file_delete(file);
			return NULL;
		}
		uint32_t nlq = 0;
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_RIVER)
			wow_mpq_read(mpq, &mcnk->mclq.data[nlq++], sizeof(*mcnk->mclq.data));
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_OCEAN)
			wow_mpq_read(mpq, &mcnk->mclq.data[nlq++], sizeof(*mcnk->mclq.data));
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_MAGMA)
			wow_mpq_read(mpq, &mcnk->mclq.data[nlq++], sizeof(*mcnk->mclq.data));
		if (mcnk->header.flags & WOW_MCNK_FLAGS_LQ_SLIME)
			wow_mpq_read(mpq, &mcnk->mclq.data[nlq++], sizeof(*mcnk->mclq.data));
		/* wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mccv, SEEK_SET);
		 * wow_mpq_read(mpq, &mcnk->mccv, sizeof(mcnk->mccv));
		 * wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mclv, SEEK_SET);
		 * wow_mpq_read(mpq, &mcnk->mclv, sizeof(mcnk->mclv));
		 */
		if (mcnk->header.layers)
		{
			mcnk->mcal.data_len = 4096 * mcnk->header.layers;
			mcnk->mcal.data = WOW_MALLOC(mcnk->mcal.data_len);
			if (!mcnk->mcal.data)
			{
				wow_adt_file_delete(file);
				return NULL;
			}
			wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcal + sizeof(mcnk->mcal.header), SEEK_SET);
			wow_mpq_read(mpq, mcnk->mcal.data, mcnk->mcal.data_len);
			mcnk->mcly.data_nb = mcnk->header.layers;
			mcnk->mcly.data = WOW_MALLOC(sizeof(*mcnk->mcly.data) * mcnk->mcly.data_nb);
			if (!mcnk->mcly.data)
			{
				wow_adt_file_delete(file);
				return NULL;
			}
			for (uint8_t l = 0; l < mcnk->header.layers; ++l)
			{
				wow_mpq_seek(mpq, mcin->offset + mcnk->header.ofs_mcly + sizeof(mcnk->mcly.header) + l * sizeof(*mcnk->mcly.data), SEEK_SET);
				wow_mpq_read(mpq, &mcnk->mcly.data[l], sizeof(*mcnk->mcly.data));
			}
		}
	}
	wow_mpq_seek(mpq, mhdr_base + file->mhdr.mtex, SEEK_SET);
	wow_mpq_read(mpq, &file->mtex.header, sizeof(file->mtex.header));
	file->mtex.data_len = file->mtex.header.size;
	file->mtex.data = WOW_MALLOC(file->mtex.data_len);
	if (!file->mtex.data)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	wow_mpq_read(mpq, file->mtex.data, file->mtex.data_len);
	file->textures_nb = 0;
	for (char *tmp = file->mtex.data; tmp - file->mtex.data < file->mtex.data_len; tmp += strlen(tmp) + 1)
		file->textures_nb++;
	file->textures = WOW_MALLOC(sizeof(*file->textures) * file->textures_nb);
	if (!file->textures)
	{
		wow_adt_file_delete(file);
		return NULL;
	}
	uint32_t i = 0;
	for (char *tmp = file->mtex.data; tmp - file->mtex.data < file->mtex.data_len; tmp += strlen(tmp) + 1)
		file->textures[i++] = tmp;
	return file;
}

void wow_adt_file_delete(wow_adt_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file->mwmo.data);
	WOW_FREE(file->modf.data);
	WOW_FREE(file->mmdx.data);
	WOW_FREE(file->mmid.data);
	WOW_FREE(file->mddf.data);
	WOW_FREE(file->mwid.data);
	for (uint32_t i = 0; i < 16 * 16; ++i)
	{
		wow_mcnk_t *mcnk = &file->mcnk[i];
		WOW_FREE(mcnk->mcly.data);
		WOW_FREE(mcnk->mcrf.doodads);
		WOW_FREE(mcnk->mcrf.wmos);
		WOW_FREE(mcnk->mcal.data);
		WOW_FREE(mcnk->mcse.data);
		WOW_FREE(mcnk->mclq.data);
	}
	WOW_FREE(file->mtex.data);
	WOW_FREE(file->textures);
	WOW_FREE(file);
}
