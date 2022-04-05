#include "wmo.h"

#include "wmo_group.h"
#include "common.h"
#include "mpq.h"

#include <stdlib.h>
#include <string.h>

wow_wmo_file_t *wow_wmo_file_new(wow_mpq_file_t *mpq)
{
	wow_wmo_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	memset(file, 0, sizeof(*file));
	wow_mpq_read(mpq, &file->mver, sizeof(file->mver));
	wow_mpq_read(mpq, &file->mohd, sizeof(file->mohd));
	wow_mpq_read(mpq, &file->motx.header, sizeof(file->motx.header));
	file->motx.data_len = file->motx.header.size;
	file->motx.data = WOW_MALLOC(file->motx.data_len + 1);
	if (!file->motx.data)
		goto err;
	file->motx.data[file->motx.data_len] = 0;
	wow_mpq_read(mpq, file->motx.data, file->motx.header.size);
	wow_mpq_read(mpq, &file->momt.header, sizeof(file->momt.header));
	file->momt.data_nb = file->mohd.textures_nb;
	file->momt.data = WOW_MALLOC(sizeof(*file->momt.data) * file->mohd.textures_nb);
	if (!file->momt.data)
		goto err;
	wow_mpq_read(mpq, file->momt.data, sizeof(*file->momt.data) * file->mohd.textures_nb);
	{
		wow_chunk_header_t mogn;
		wow_mpq_read(mpq, &mogn, sizeof(mogn));
		wow_mpq_seek(mpq, mogn.size, SEEK_CUR);
	}
	wow_mpq_read(mpq, &file->mogi.header, sizeof(file->mogi.header));
	file->mogi.data_nb = file->mohd.groups_nb;
	file->mogi.data = WOW_MALLOC(sizeof(*file->mogi.data) * file->mohd.groups_nb);
	if (!file->mogi.data)
		goto err;
	wow_mpq_read(mpq, file->mogi.data, sizeof(*file->mogi.data) * file->mohd.groups_nb);
	{
		wow_chunk_header_t mosb;
		wow_mpq_read(mpq, &mosb, sizeof(mosb));
		wow_mpq_seek(mpq, mosb.size, SEEK_CUR);
	}
	wow_mpq_read(mpq, &file->mopv.header, sizeof(file->mopv.header));
	file->mopv.vertices_nb = file->mopv.header.size / sizeof(*file->mopv.vertices);
	file->mopv.vertices = WOW_MALLOC(file->mopv.header.size);
	if (!file->mopv.vertices)
		goto err;
	wow_mpq_read(mpq, file->mopv.vertices, file->mopv.header.size);
	wow_mpq_read(mpq, &file->mopt.header, sizeof(file->mopt.header));
	file->mopt.data_nb = file->mopt.header.size / sizeof(*file->mopt.data);
	file->mopt.data = WOW_MALLOC(file->mopt.header.size);
	if (!file->mopt.data)
		goto err;
	wow_mpq_read(mpq, file->mopt.data, file->mopt.header.size);
	wow_mpq_read(mpq, &file->mopr.header, sizeof(file->mopr.header));
	file->mopr.data_nb = file->mopr.header.size / sizeof(*file->mopr.data);
	file->mopr.data = WOW_MALLOC(file->mopr.header.size);
	if (!file->mopt.data)
		goto err;
	wow_mpq_read(mpq, file->mopr.data, file->mopr.header.size);
	wow_mpq_read(mpq, &file->movv.header, sizeof(file->movv.header));
	file->movv.vertices_nb = file->movv.header.size / sizeof(*file->movv.vertices);
	file->movv.vertices = WOW_MALLOC(file->movv.header.size);
	if (!file->movv.vertices)
		goto err;
	wow_mpq_read(mpq, file->movv.vertices, file->movv.header.size);
	wow_mpq_read(mpq, &file->movb.header, sizeof(file->movb.header));
	file->movb.data_nb = file->movb.header.size / (sizeof(*file->movb.data));
	file->movb.data = WOW_MALLOC(file->movb.header.size);
	if (!file->movb.data)
		goto err;
	wow_mpq_read(mpq, file->movb.data, file->movb.header.size);
	wow_mpq_read(mpq, &file->molt.header, sizeof(file->molt.header));
	file->molt.data_nb = file->mohd.lights_nb;
	file->molt.data = WOW_MALLOC(sizeof(*file->molt.data) * file->mohd.lights_nb);
	if (!file->molt.data)
		goto err;
	wow_mpq_read(mpq, file->molt.data, sizeof(*file->molt.data) * file->mohd.lights_nb);
	wow_mpq_read(mpq, &file->mods.header, sizeof(file->mods.header));
	file->mods.data_nb = file->mohd.doodad_sets_nb;
	file->mods.data = WOW_MALLOC(sizeof(*file->mods.data) * file->mohd.doodad_sets_nb);
	if (!file->mods.data)
		goto err;
	wow_mpq_read(mpq, file->mods.data, sizeof(*file->mods.data) * file->mohd.doodad_sets_nb);
	wow_mpq_read(mpq, &file->modn.header, sizeof(file->modn.header));
	file->modn.data_len = file->modn.header.size;
	file->modn.data = WOW_MALLOC(file->modn.header.size);
	if (!file->modn.data)
		goto err;
	wow_mpq_read(mpq, file->modn.data, file->modn.header.size);
	wow_mpq_read(mpq, &file->modd.header, sizeof(file->modd.header));
	file->modd.data_nb = file->modd.header.size / sizeof(*file->modd.data);
	file->modd.data = WOW_MALLOC(file->modd.header.size);
	if (!file->modd.data)
		goto err;
	wow_mpq_read(mpq, file->modd.data, file->modd.header.size);
	wow_mpq_read(mpq, &file->mfog.header, sizeof(file->mfog.header));
	file->mfog.data_nb = file->mfog.header.size / sizeof(*file->mfog.data);
	file->mfog.data = WOW_MALLOC(file->mfog.header.size);
	if (!file->mfog.data)
		goto err;
	wow_mpq_read(mpq, file->mfog.data, file->mfog.header.size);
	if (wow_mpq_read(mpq, &file->mcvp.header, sizeof(file->mcvp.header)))
	{
		file->mcvp.data_nb = file->mcvp.header.size / sizeof(*file->mcvp.data);
		file->mcvp.data = WOW_MALLOC(file->mcvp.header.size);
		if (!file->mcvp.data)
			goto err;
		wow_mpq_read(mpq, file->mcvp.data, file->mcvp.header.size);
	}
	return file;

err:
	wow_wmo_file_delete(file);
	return NULL;
}

void wow_wmo_file_delete(wow_wmo_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file->motx.data);
	WOW_FREE(file->momt.data);
	WOW_FREE(file->mogi.data);
	WOW_FREE(file->mopv.vertices);
	WOW_FREE(file->mopt.data);
	WOW_FREE(file->mopr.data);
	WOW_FREE(file->movv.vertices);
	WOW_FREE(file->movb.data);
	WOW_FREE(file->molt.data);
	WOW_FREE(file->mods.data);
	WOW_FREE(file->modn.data);
	WOW_FREE(file->modd.data);
	WOW_FREE(file->mfog.data);
	WOW_FREE(file->mcvp.data);
	WOW_FREE(file);
}
