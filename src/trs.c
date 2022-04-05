#include "trs.h"

#include "common.h"
#include "mpq.h"

#include <stdlib.h>

wow_trs_file_t *wow_trs_file_new(wow_mpq_file_t *mpq)
{
	(void)mpq;
	wow_trs_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	return file;
}

void wow_trs_file_delete(wow_trs_file_t *file)
{
	if (!file)
		return;
	WOW_FREE(file);
}
