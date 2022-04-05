#ifndef WOW_TOC_H
#define WOW_TOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct wow_toc_file_s
{
	char **files;
	uint32_t files_nb;
	char *title;
	char *notes;
	char *deps;
	char *url;
	uint32_t version;
} wow_toc_file_t;

wow_toc_file_t *wow_toc_file_new(const uint8_t *data, uint32_t len);
void wow_toc_file_delete(wow_toc_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
