#ifndef WOW_DBC_H
#define WOW_DBC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_dbc_header_s
{
	uint32_t magic;
	uint32_t record_count;
	uint32_t field_count;
	uint32_t record_size;
	uint32_t string_block_size;
} wow_dbc_header_t;

typedef struct wow_dbc_file_s wow_dbc_file_t;

typedef struct wow_dbc_row_s
{
	const wow_dbc_file_t *file;
	const uint8_t *ptr;
} wow_dbc_row_t;

struct wow_dbc_file_s
{
	wow_dbc_header_t header;
	uint8_t *data;
	uint32_t data_size;
	char *strings;
	uint32_t strings_size;
};

wow_dbc_file_t *wow_dbc_file_new(wow_mpq_file_t *mpq);
void wow_dbc_file_delete(wow_dbc_file_t *file);
wow_dbc_row_t wow_dbc_get_row(const wow_dbc_file_t *file, uint32_t row);
const void *wow_dbc_get_ptr(const wow_dbc_row_t *row, uint32_t column_offset);
int8_t wow_dbc_get_i8(const wow_dbc_row_t *row, uint32_t column_offest);
uint8_t wow_dbc_get_u8(const wow_dbc_row_t *row, uint32_t column_offset);
int16_t wow_dbc_get_i16(const wow_dbc_row_t *row, uint32_t column_offset);
uint16_t wow_dbc_get_u16(const wow_dbc_row_t *row, uint32_t column_offset);
int32_t wow_dbc_get_i32(const wow_dbc_row_t *row, uint32_t column_offset);
uint32_t wow_dbc_get_u32(const wow_dbc_row_t *row, uint32_t column_offset);
int64_t wow_dbc_get_i64(const wow_dbc_row_t *row, uint32_t column_offset);
uint64_t wow_dbc_get_u64(const wow_dbc_row_t *row, uint32_t column_offset);
float wow_dbc_get_flt(const wow_dbc_row_t *row, uint32_t column_offset);
const char *wow_dbc_get_str(const wow_dbc_row_t *row, uint32_t column_offset);

enum wow_dbc_def_type
{
	WOW_DBC_TYPE_I8,
	WOW_DBC_TYPE_U8,
	WOW_DBC_TYPE_I16,
	WOW_DBC_TYPE_U16,
	WOW_DBC_TYPE_I32,
	WOW_DBC_TYPE_U32,
	WOW_DBC_TYPE_I64,
	WOW_DBC_TYPE_U64,
	WOW_DBC_TYPE_STR,
	WOW_DBC_TYPE_LSTR,
	WOW_DBC_TYPE_FLT,
	WOW_DBC_TYPE_END
};

typedef struct wow_dbc_def_s
{
	enum wow_dbc_def_type type;
	const char *name;
} wow_dbc_def_t;

extern const wow_dbc_def_t wow_dbc_animation_data_def[];
extern const wow_dbc_def_t wow_dbc_area_poi_def[];
extern const wow_dbc_def_t wow_dbc_area_table_def[];
extern const wow_dbc_def_t wow_dbc_char_base_info_def[];
extern const wow_dbc_def_t wow_dbc_char_hair_geosets_def[];
extern const wow_dbc_def_t wow_dbc_char_sections_def[];
extern const wow_dbc_def_t wow_dbc_char_start_outfit_def[];
extern const wow_dbc_def_t wow_dbc_char_titles_def[];
extern const wow_dbc_def_t wow_dbc_character_facial_hair_styles_def[];
extern const wow_dbc_def_t wow_dbc_chr_classes_def[];
extern const wow_dbc_def_t wow_dbc_chr_races_def[];
extern const wow_dbc_def_t wow_dbc_creature_display_info_def[];
extern const wow_dbc_def_t wow_dbc_creature_display_info_extra_def[];
extern const wow_dbc_def_t wow_dbc_creature_model_data_def[];
extern const wow_dbc_def_t wow_dbc_helmet_geoset_vis_data_def[];
extern const wow_dbc_def_t wow_dbc_item_def[];
extern const wow_dbc_def_t wow_dbc_item_class_def[];
extern const wow_dbc_def_t wow_dbc_item_display_info_def[];
extern const wow_dbc_def_t wow_dbc_item_set_def[];
extern const wow_dbc_def_t wow_dbc_item_sub_class_def[];
extern const wow_dbc_def_t wow_dbc_light_int_band_def[];
extern const wow_dbc_def_t wow_dbc_light_float_band_def[];
extern const wow_dbc_def_t wow_dbc_map_def[];
extern const wow_dbc_def_t wow_dbc_name_gen_def[];
extern const wow_dbc_def_t wow_dbc_spell_def[];
extern const wow_dbc_def_t wow_dbc_spell_icon_def[];
extern const wow_dbc_def_t wow_dbc_talent_def[];
extern const wow_dbc_def_t wow_dbc_talent_tab_def[];
extern const wow_dbc_def_t wow_dbc_world_map_area_def[];
extern const wow_dbc_def_t wow_dbc_world_map_continent_def[];
extern const wow_dbc_def_t wow_dbc_world_map_overlay_def[];
extern const wow_dbc_def_t wow_dbc_wow_error_strings_def[];

#ifdef __cplusplus
}
#endif

#endif
