#include "wmo_group.h"

#include "common.h"
#include "mpq.h"

#include <string.h>
#include <stdlib.h>

wow_wmo_group_file_t *wow_wmo_group_file_new(wow_mpq_file_t *mpq)
{
	wow_wmo_group_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	memset(file, 0, sizeof(*file));
	wow_mpq_read(mpq, &file->mver, sizeof(file->mver));
	wow_mpq_read(mpq, &file->mogp, sizeof(file->mogp));
	wow_mpq_read(mpq, &file->mopy.header, sizeof(file->mopy.header));
	file->mopy.data_nb = file->mopy.header.size / sizeof(*file->mopy.data);
	file->mopy.data = WOW_MALLOC(file->mopy.header.size);
	if (!file->mopy.data)
		goto err;
	wow_mpq_read(mpq, file->mopy.data, file->mopy.header.size);
	wow_mpq_read(mpq, &file->movi.header, sizeof(file->movi.header));
	file->movi.indices_nb = file->movi.header.size / sizeof(*file->movi.indices);
	file->movi.indices = WOW_MALLOC(file->movi.header.size);
	if (!file->movi.indices)
		goto err;
	wow_mpq_read(mpq, file->movi.indices, file->movi.header.size);
	wow_mpq_read(mpq, &file->movt.header, sizeof(file->movt.header));
	file->movt.vertexes_nb = file->movt.header.size / sizeof(*file->movt.vertexes);
	file->movt.vertexes = WOW_MALLOC(file->movt.header.size);
	if (!file->movt.vertexes)
		goto err;
	wow_mpq_read(mpq, file->movt.vertexes, file->movt.header.size);
	wow_mpq_read(mpq, &file->monr.header, sizeof(file->monr.header));
	file->monr.normals_nb = file->monr.header.size / sizeof(*file->monr.normals);
	file->monr.normals = WOW_MALLOC(file->monr.header.size);
	if (!file->monr.normals)
		goto err;
	wow_mpq_read(mpq, file->monr.normals, file->monr.header.size);
	wow_mpq_read(mpq, &file->motv.header, sizeof(file->motv.header));
	file->motv.tex_coords_nb = file->motv.header.size / sizeof(*file->motv.tex_coords);
	file->motv.tex_coords = WOW_MALLOC(file->motv.header.size);
	if (!file->motv.tex_coords)
		goto err;
	wow_mpq_read(mpq, file->motv.tex_coords, file->motv.header.size);
	wow_mpq_read(mpq, &file->moba.header, sizeof(file->moba.header));
	file->moba.data_nb = file->moba.header.size / sizeof(*file->moba.data);
	file->moba.data = WOW_MALLOC(file->moba.header.size);
	if (!file->moba.data)
		goto err;
	wow_mpq_read(mpq, file->moba.data, file->moba.header.size);
	if (file->mogp.flags & WOW_MOGP_FLAGS_LIGHT)
	{
		wow_mpq_read(mpq, &file->molr.header, sizeof(file->molr.header));
		file->molr.lights_nb = file->molr.header.size / sizeof(*file->molr.lights);
		file->molr.lights = WOW_MALLOC(file->molr.header.size);
		if (!file->molr.lights)
			goto err;
		wow_mpq_read(mpq, file->molr.lights, file->molr.header.size);
	}
	if (file->mogp.flags & WOW_MOGP_FLAGS_DOODAD)
	{
		wow_mpq_read(mpq, &file->modr.header, sizeof(file->modr.header));
		file->modr.doodads_nb = file->modr.header.size / sizeof(*file->modr.doodads);
		file->modr.doodads = WOW_MALLOC(file->modr.header.size);
		if (!file->modr.doodads)
			goto err;
		wow_mpq_read(mpq, file->modr.doodads, file->modr.header.size);
	}
	if (file->mogp.flags & WOW_MOGP_FLAGS_BSP)
	{
		wow_mpq_read(mpq, &file->mobn.header, sizeof(file->mobn.header));
		file->mobn.nodes_nb = file->mobn.header.size / sizeof(*file->mobn.nodes);
		file->mobn.nodes = WOW_MALLOC(file->mobn.header.size);
		if (!file->mobn.nodes)
			goto err;
		wow_mpq_read(mpq, file->mobn.nodes, file->mobn.header.size);
		wow_mpq_read(mpq, &file->mobr.header, sizeof(file->mobr.header));
		file->mobr.indices_nb = file->mobr.header.size / sizeof(*file->mobr.indices);
		file->mobr.indices = WOW_MALLOC(file->mobr.header.size);
		if (!file->mobr.indices)
			goto err;
		wow_mpq_read(mpq, file->mobr.indices, file->mobr.header.size);
	}
	if (file->mogp.flags & WOW_MOGP_FLAGS_COLOR)
	{
		wow_mpq_read(mpq, &file->mocv.header, sizeof(file->mocv.header));
		file->mocv.colors_nb = file->mocv.header.size / sizeof(*file->mocv.colors);
		file->mocv.colors = WOW_MALLOC(file->mocv.header.size);
		if (!file->mocv.colors)
			goto err;
		wow_mpq_read(mpq, file->mocv.colors, file->mocv.header.size);
	}
	if (file->mogp.flags & WOW_MOGP_FLAGS_LIQUID)
	{
		wow_mpq_read(mpq, &file->mliq.header, sizeof(file->mliq.header) - 2);
		file->mliq.vertexes_nb = file->mliq.header.xverts * file->mliq.header.yverts;
		file->mliq.vertexes = WOW_MALLOC(file->mliq.header.xverts * file->mliq.header.yverts * sizeof(*file->mliq.vertexes));
		if (!file->mliq.vertexes)
			goto err;
		wow_mpq_read(mpq, file->mliq.vertexes, file->mliq.header.xverts * file->mliq.header.yverts * sizeof(*file->mliq.vertexes));
		file->mliq.tiles_nb = file->mliq.header.xtiles * file->mliq.header.ytiles;
		file->mliq.tiles = WOW_MALLOC(file->mliq.header.xtiles * file->mliq.header.ytiles * sizeof(*file->mliq.tiles));
		if (!file->mliq.tiles)
			goto err;
		wow_mpq_read(mpq, file->mliq.tiles, file->mliq.header.xtiles * file->mliq.header.ytiles * sizeof(*file->mliq.tiles));
	}
	return file;
err:
	wow_wmo_group_file_delete(file);
	return NULL;
}

void wow_wmo_group_file_delete(wow_wmo_group_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file->molr.lights);
	WOW_FREE(file->modr.doodads);
	WOW_FREE(file->mobn.nodes);
	WOW_FREE(file->mobr.indices);
	WOW_FREE(file->mocv.colors);
	WOW_FREE(file->mliq.vertexes);
	WOW_FREE(file->mliq.tiles);
	WOW_FREE(file->moba.data);
	WOW_FREE(file->motv.tex_coords);
	WOW_FREE(file->monr.normals);
	WOW_FREE(file->movt.vertexes);
	WOW_FREE(file->movi.indices);
	WOW_FREE(file->mopy.data);
	WOW_FREE(file);
}
