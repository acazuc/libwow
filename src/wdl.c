#include "wdl.h"

#include "common.h"
#include "mpq.h"

#include <string.h>
#include <stdlib.h>

wow_wdl_file_t *wow_wdl_file_new(wow_mpq_file_t *mpq)
{
	wow_wdl_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	wow_mpq_read(mpq, &file->mver, sizeof(file->mver));
	wow_mpq_read(mpq, &file->maof.header, sizeof(file->maof.header));
	wow_mpq_read(mpq, file->maof.offsets, file->maof.header.size);
	for (uint32_t z = 0; z < 64; ++z)
	{
		for (uint32_t x = 0; x < 64; ++x)
		{
			if (!file->maof.offsets[z][x])
			{
				memset(&file->mare[z][x], 0, sizeof(file->mare[z][x]));
				continue;
			}
			wow_mpq_seek(mpq, file->maof.offsets[z][x], SEEK_SET);
			wow_mpq_read(mpq, &file->mare[z][x], sizeof(file->mare[z][x]));
		}
	}
	return file;
}

void wow_wdl_file_delete(wow_wdl_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file);
}
