#ifndef WOW_TRS_H
#define WOW_TRS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_trs_file_s
{
} wow_trs_file_t;

wow_trs_file_t *wow_trs_file_new(wow_mpq_file_t *mpq);
void wow_trs_file_delete(wow_trs_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
