#ifndef WOW_WMO_H
#define WOW_WMO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_compound_s wow_mpq_compound_t;
typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_mcvp_s
{
	wow_chunk_header_t header;
	wow_vec4f_t *data;
	uint32_t data_nb;
} wow_mcvp_t;

typedef struct wow_mfog_data_s
{
	uint32_t flags;
	wow_vec3f_t pos;
	float small_radius;
	float large_radius;
	float fog_end1;
	float fog_start1;
	wow_vec4b_t color1;
	float fog_end2;
	float fog_start2;
	wow_vec4b_t color2;
} wow_mfog_data_t;

typedef struct wow_mfog_s
{
	wow_chunk_header_t header;
	wow_mfog_data_t *data;
	uint32_t data_nb;
} wow_mfog_t;

typedef struct wow_modd_data_s
{
	uint32_t name_index : 24;
	uint32_t flag_accept_proj_tex : 1;
	uint32_t flag_0x2 : 1;
	uint32_t flag_0x4 : 1;
	uint32_t flag_0x8 : 1;
	uint32_t padding : 4;
	wow_vec3f_t position;
	wow_quaternion_float_t rotation;
	float scale;
	wow_vec4b_t color;
} wow_modd_data_t;

typedef struct wow_modd_s
{
	wow_chunk_header_t header;
	wow_modd_data_t *data;
	uint32_t data_nb;
} wow_modd_t;

typedef struct wow_modn_s
{
	wow_chunk_header_t header;
	char *data;
	uint32_t data_len;
} wow_modn_t;

typedef struct wow_mods_data_s
{
	char name[20];
	uint32_t start_index;
	uint32_t count;
	char pad[4];
} wow_mods_data_t;

typedef struct wow_mods_s
{
	wow_chunk_header_t header;
	wow_mods_data_t *data;
	uint32_t data_nb;
} wow_mods_t;

enum wow_light_type
{
	WOW_LIGHT_OMNI = 0,
	WOW_LIGHT_SPOT = 1,
	WOW_LIGHT_DIRECT = 2,
	WOW_LIGHT_AMBIENT = 3,
};

typedef struct wow_molt_data_s
{
	uint8_t type;
	uint8_t use_atten;
	uint8_t pad[2];
	wow_vec4b_t color;
	wow_vec3f_t position;
	float intensity;
	float unknown[4];
	float atten_start;
	float atten_end;
} wow_molt_data_t;

typedef struct wow_molt_s
{
	wow_chunk_header_t header;
	wow_molt_data_t *data;
	uint32_t data_nb;
} wow_molt_t;

typedef struct wow_movb_data_s
{
	uint16_t first_vertex;
	uint16_t count;
} wow_movb_data_t;

typedef struct wow_movb_s
{
	wow_chunk_header_t header;
	wow_movb_data_t *data;
	uint32_t data_nb;
} wow_movb_t;

typedef struct wow_movv_s
{
	wow_chunk_header_t header;
	wow_vec3f_t *vertices;
	uint32_t vertices_nb;
} wow_movv_t;

typedef struct wow_mopr_data_s
{
	uint16_t portal_index;
	uint16_t group_index;
	int16_t side;
	uint16_t filler;
} wow_mopr_data_t;

typedef struct wow_mopr_s
{
	wow_chunk_header_t header;
	wow_mopr_data_t *data;
	uint32_t data_nb;
} wow_mopr_t;

typedef struct wow_mopt_data_s
{
	uint16_t start_vertex;
	uint16_t count;
	wow_vec3f_t normal;
	float distance;
} wow_mopt_data_t;

typedef struct wow_mopt_s
{
	wow_chunk_header_t header;
	wow_mopt_data_t *data;
	uint32_t data_nb;
} wow_mopt_t;

typedef struct wow_mopv_s
{
	wow_chunk_header_t header;
	wow_vec3f_t *vertices;
	uint32_t vertices_nb;
} wow_mopv_t;

typedef struct wow_mogi_data_s
{
	uint32_t flags;
	wow_vec3f_t aabb0;
	wow_vec3f_t aabb1;
	int32_t name_offset;
} wow_mogi_data_t;

typedef struct wow_mogi_s
{
	wow_chunk_header_t header;
	wow_mogi_data_t *data;
	uint32_t data_nb;
} wow_mogi_t;

#define WOW_MOMT_FLAGS_UNLIT    0x001
#define WOW_MOMT_FLAGS_UNFOGGED 0x002
#define WOW_MOMT_FLAGS_UNCULLED 0x004
#define WOW_MOMT_FLAGS_EXTLIGHT 0x008
#define WOW_MOMT_FLAGS_SIDN     0x010
#define WOW_MOMT_FLAGS_WINDOW   0x020
#define WOW_MOMT_FLAGS_CLAMP_S  0x040
#define WOW_MOMT_FLAGS_CLAMP_T  0x080
#define WOW_MOMT_FLAGS_UNKNOWN  0x100

typedef struct wow_momt_data_s
{
	/* 0x00 */ uint32_t flags;
	/* 0x04 */ uint32_t shader;
	/* 0x08 */ uint32_t blend_mode;
	/* 0x0C */ uint32_t texture1;
	/* 0x10 */ wow_vec4b_t emissive_color;
	/* 0x1C */ wow_vec4b_t sidn_emissive_color;
	/* 0x28 */ uint32_t texture2;
	/* 0x2C */ uint32_t diff_color;
	/* 0x30 */ uint32_t group_type;
	/* 0x34 */ uint32_t texture3;
	/* 0x38 */ uint32_t color2;
	/* 0x3C */ uint32_t flags2;
	/* 0x40 */ uint32_t run_time_data[4];
} wow_momt_data_t;

typedef struct wow_momt_s
{
	wow_chunk_header_t header;
	wow_momt_data_t *data;
	uint32_t data_nb;
} wow_momt_t;

typedef struct wow_motx_s
{
	wow_chunk_header_t header;
	char *data;
	uint32_t data_len;
} wow_motx_t;

#define WOW_MOHD_FLAGS_ATTENUATE_PORTAL_DISTANCE 0x1
#define WOW_MOHD_FLAGS_SKIP_BASE_COLOR           0x2
#define WOW_MOHD_FLAGS_USE_DBC_LIQUID_TYPE       0x4
#define WOW_MOHD_FLAGS_LIGHTEN_INTERIORS         0x8

typedef struct wow_mohd_s
{
	wow_chunk_header_t header;
	uint32_t textures_nb;
	uint32_t groups_nb;
	uint32_t portals_nb;
	uint32_t lights_nb;
	uint32_t models_nb;
	uint32_t doodad_defs_nb;
	uint32_t doodad_sets_nb;
	wow_vec4b_t ambient;
	uint32_t wmo_id;
	wow_vec3f_t aabb0;
	wow_vec3f_t aabb1;
	uint16_t flags;
	uint16_t num_lod;
} wow_mohd_t;

typedef struct wow_wmo_file_s
{
	wow_motx_t motx;
	wow_momt_t momt;
	wow_mver_t mver;
	wow_mohd_t mohd;
	wow_mogi_t mogi;
	wow_mopv_t mopv;
	wow_mopt_t mopt;
	wow_mopr_t mopr;
	wow_movv_t movv;
	wow_movb_t movb;
	wow_molt_t molt;
	wow_mods_t mods;
	wow_modn_t modn;
	wow_modd_t modd;
	wow_mfog_t mfog;
	wow_mcvp_t mcvp;
} wow_wmo_file_t;

wow_wmo_file_t *wow_wmo_file_new(wow_mpq_file_t *mpq);
void wow_wmo_file_delete(wow_wmo_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
