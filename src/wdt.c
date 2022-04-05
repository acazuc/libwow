#include "wdt.h"

#include "common.h"
#include "mpq.h"

#include <string.h>
#include <stdlib.h>

wow_wdt_file_t *wow_wdt_file_new(wow_mpq_file_t *mpq)
{
	wow_wdt_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	file->modf.data = NULL;
	file->mwmo.data = NULL;
	if (wow_mpq_read(mpq, &file->mver, sizeof(file->mver)) != sizeof(file->mver))
		goto err;
	if (memcmp(&file->mver.header.magic, "REVM", 4))
		goto err;
	if (wow_mpq_read(mpq, &file->mphd, sizeof(file->mphd)) != sizeof(file->mphd))
		goto err;
	if (memcmp(&file->mphd.header.magic, "DHPM", 4))
		goto err;
	if (wow_mpq_read(mpq, &file->main, sizeof(file->main)) != sizeof(file->main))
		goto err;
	if (memcmp(&file->main.header.magic, "NIAM", 4))
		goto err;
	if (wow_mpq_read(mpq, &file->mwmo.header, sizeof(file->mwmo.header)) != sizeof(file->mwmo.header))
		return file; /* MWMO is optional */
	if (file->mwmo.header.size)
	{
		file->mwmo.data_len = file->mwmo.header.size;
		file->mwmo.data = WOW_MALLOC(file->mwmo.data_len);
		if (!file->mwmo.data)
			goto err;
		wow_mpq_read(mpq, file->mwmo.data, file->mwmo.header.size);
		wow_mpq_read(mpq, &file->modf.header, sizeof(file->modf.header));
		file->modf.data_nb = file->modf.header.size / sizeof(*file->modf.data);
		file->modf.data = WOW_MALLOC(sizeof(*file->modf.data) * file->modf.data_nb);
		if (!file->modf.data)
			goto err;
		wow_mpq_read(mpq, file->modf.data, file->modf.header.size);
	}
	return file;

err:
	wow_wdt_file_delete(file);
	return NULL;
}

void wow_wdt_file_delete(wow_wdt_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file->mwmo.data);
	WOW_FREE(file->modf.data);
	WOW_FREE(file);
}
