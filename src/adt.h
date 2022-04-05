#ifndef WOW_ADT_H
#define WOW_ADT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_file_s wow_mpq_file_t;

#define WOW_MHDR_FLAGS_MFBO      0x1
#define WOW_MHDR_FLAGS_NORTHREND 0x2

typedef struct wow_mhdr_s
{
	wow_chunk_header_t header;
	uint32_t flags;
	uint32_t mcin;
	uint32_t mtex;
	uint32_t mmdx;
	uint32_t mmid;
	uint32_t mwmo;
	uint32_t mwid;
	uint32_t mddf;
	uint32_t modf;
	uint32_t mfbo;
	uint32_t mh2o;
	uint32_t mtxf;
	uint32_t unused[4];
} wow_mhdr_t;

typedef struct wow_mfbo_s
{
	wow_chunk_header_t header;
	uint16_t maximum[3][3];
	uint16_t minimum[3][3];
} wow_mfbo_t;

typedef struct wow_mmdx_s
{
	wow_chunk_header_t header;
	char *data;
	uint32_t data_len;
} wow_mmdx_t;

typedef struct wow_mmid_s
{
	wow_chunk_header_t header;
	uint32_t *data;
	uint32_t data_nb;
} wow_mmid_t;

#define WOW_MDDF_FLAGS_BIODOME   0x01
#define WOW_MDDF_FLAGS_SHRUBBERY 0x02
#define WOW_MDDF_FLAGS_UNKNOWN1  0x04
#define WOW_MDDF_FLAGS_UNKNOWN2  0x08
#define WOW_MDDF_FLAGS_LIQUID    0x10
#define WOW_MDDF_FLAGS_FILEDATA  0x20
#define WOW_MDDF_FLAGS_UNKNOWN3  0x40

typedef struct wow_mddf_data_s
{
	uint32_t name_id;
	uint32_t unique_id;
	wow_vec3f_t position;
	wow_vec3f_t rotation;
	uint16_t scale;
	uint16_t flags;
} wow_mddf_data_t;

typedef struct wow_mddf_s
{
	wow_chunk_header_t header;
	wow_mddf_data_t *data;
	uint32_t data_nb;
} wow_mddf_t;

typedef struct wow_mwid_t
{
	wow_chunk_header_t header;
	uint32_t *data;
	uint32_t data_nb;
} wow_mwid_t;

typedef struct wow_mcin_data_s
{
	uint32_t offset;
	uint32_t size;
	uint32_t flags;
	uint32_t async_id;
} wow_mcin_data_t;

typedef struct wow_mcin_s
{
	wow_chunk_header_t header;
	wow_mcin_data_t data[16 * 16];
} wow_mcin_t;

typedef struct wow_mcvt_s
{
	wow_chunk_header_t header;
	float height[9 * 9 + 8 * 8];
} wow_mcvt_t;

typedef struct wow_mcnr_s
{
	wow_chunk_header_t header;
	int8_t normal[(9 * 9 + 8 * 8) * 3];
} wow_mcnr_t;

typedef struct wow_mcly_data_flags_s
{
	uint32_t animation_rotation : 3;
	uint32_t animation_speed : 3;
	uint32_t animation_enabled : 1;
	uint32_t overbright : 1;
	uint32_t use_alpha_map : 1;
	uint32_t alpha_map_compressed : 1;
	uint32_t use_cube_map_reflection : 1;
	uint32_t unknown_0x800 : 1;
	uint32_t unknown_0x1000 : 1;
	uint32_t unused : 19;
} wow_mcly_data_flags_t;

typedef struct wow_mcly_data_s
{
	uint32_t texture_id;
	wow_mcly_data_flags_t flags;
	uint32_t offset_in_mcal;
	uint32_t effect_id;
} wow_mcly_data_t;

typedef struct wow_mcly_s
{
	wow_chunk_header_t header;
	wow_mcly_data_t *data;
	uint32_t data_nb;
} wow_mcly_t;

typedef struct wow_mcrf_s
{
	wow_chunk_header_t header;
	uint32_t *doodads;
	uint32_t doodads_nb;
	uint32_t *wmos;
	uint32_t wmos_nb;
} wow_mcrf_t;

typedef struct wow_mcal_s
{
	wow_chunk_header_t header;
	uint8_t *data;
	uint32_t data_len;
} wow_mcal_t;

typedef struct wow_mcsh_s
{
	wow_chunk_header_t header;
	uint8_t shadow[64][8];
} wow_mcsh_t;

typedef struct wow_mcse_data_s
{
	uint32_t sound_point_id;
	uint32_t sound_name_id;
	wow_vec3f_t pos;
	float min_distance;
	float max_distance;
	float cutoff_distance;
	uint16_t start_time;
	uint16_t end_time;
	uint16_t mode;
	uint16_t group_silence_min;
	uint16_t group_silence_max;
	uint16_t play_instances_min;
	uint16_t play_instances_max;
	uint8_t loop_count_min;
	uint8_t loop_count_max;
	uint16_t inter_sound_gap_min;
	uint16_t inter_sound_gap_max;
} wow_mcse_data_t;

typedef struct wow_mcse_s
{
	wow_chunk_header_t header;
	wow_mcse_data_t *data;
	uint32_t data_nb;
} wow_mcse_t;

#define WOW_MCLQ_TILE_LIQUID_OCEAN 0x01
#define WOW_MCLQ_TILE_LIQUID_RIVER 0x04
#define WOW_MCLQ_TILE_LIQUID_MAGMA 0x06
#define WOW_MCLQ_TILE_LIQUID_SLIME 0x07
#define WOW_MCLQ_TILE_HIDDEN       0x08
#define WOW_MCLQ_TILE_LIQUID_TYPE  0x0f
#define WOW_MCLQ_TILE_MUST_SWIM    0x40
#define WOW_MCLQ_TILE_FATIGUE      0x80

typedef struct wow_mclq_data_s
{
	float height_min;
	float height_max;
	struct
	{
		union
		{
			struct
			{
				uint8_t depth;
				char flow0_pct;
				char flow1_pct;
				char padding;
				float height;
			} water;
			struct
			{
				uint8_t depth;
				char foam;
				char wet;
				char padding;
				float height;
			} ocean;
			struct
			{
				uint16_t s;
				uint16_t t;
				float height;
			} magma;
		};
	} verts[9 * 9];
	uint8_t tiles[8][8];
	uint32_t n_flowvs;
	struct
	{
		wow_vec3f_t sphere_pos;
		float sphere_radius;
		wow_vec3f_t dir;
		float velocity;
		float amplitude;
		float frequency;
	} flows[2];
} wow_mclq_data_t;

typedef struct wow_mclq_s
{
	wow_chunk_header_t header;
	wow_mclq_data_t *data;
	uint32_t data_nb;
} wow_mclq_t;

typedef struct wow_mccv_s
{
	wow_chunk_header_t header;
	wow_vec4b_t data[9 * 9 + 8 * 8];
} wow_mccv_t;

typedef struct wow_mclv_s
{
	wow_chunk_header_t header;
	wow_vec4b_t data[9 * 9 + 8 * 8];
} wow_mclv_t;

#define WOW_MCNK_FLAGS_MCSH       0x001
#define WOW_MCNK_FLAGS_IMPASS     0x002
#define WOW_MCNK_FLAGS_LQ_RIVER   0x004
#define WOW_MCNK_FLAGS_LQ_OCEAN   0x008
#define WOW_MCNK_FLAGS_LQ_MAGMA   0x010
#define WOW_MCNK_FLAGS_LQ_SLIME   0x020
#define WOW_MCNK_FLAGS_MCCV       0x040
#define WOW_MCNK_FLAGS_UNKNOWN1   0x080
#define WOW_MCNK_FLAGS_UNKNOWN2   0x100
#define WOW_MCNK_FLAGS_FIX_MCAL   0x200
#define WOW_MCNK_FLAGS_HIGH_HOLES 0x400

typedef struct wow_mcnk_header_s
{
	wow_chunk_header_t header;
	uint32_t flags;
	uint32_t index_x;
	uint32_t index_y;
	uint32_t layers;
	uint32_t doodad_refs_nb;
	uint32_t ofs_mcvt;
	uint32_t ofs_mcnr;
	uint32_t ofs_mcly;
	uint32_t ofs_mcrf;
	uint32_t ofs_mcal;
	uint32_t size_mcal;
	uint32_t ofs_mcsh;
	uint32_t size_mcsh;
	uint32_t area_id;
	uint32_t map_obj_refs_nb;
	uint16_t holes;
	uint16_t unknown;
	uint16_t low_quality_texture[8];
	union
	{
		uint8_t no_effect_doodad[8];
		uint64_t no_effect_doodads;
	};
	uint32_t ofs_mcse;
	uint32_t snd_emitters_nb;
	uint32_t ofs_mclq;
	uint32_t size_mclq;
	wow_vec3f_t position;
	uint32_t ofs_mccv;
	uint32_t ofs_mclv;
	uint32_t unused;
} wow_mcnk_header_t;

typedef struct wow_mcnk_s
{
	wow_mcnk_header_t header;
	wow_mcvt_t mcvt;
	wow_mcnr_t mcnr;
	wow_mcly_t mcly;
	wow_mcrf_t mcrf;
	wow_mcal_t mcal;
	wow_mcsh_t mcsh;
	wow_mcse_t mcse;
	wow_mclq_t mclq;
	/* wow_mccv_t mccv;
	 * wow_mclv_t mclv;
	 */
} wow_mcnk_t;

typedef struct wow_mtex_s
{
	wow_chunk_header_t header;
	char *data;
	uint32_t data_len;
} wow_mtex_t;

typedef struct wow_adt_file_s
{
	char **textures;
	uint32_t textures_nb;
	wow_mver_t mver;
	wow_mhdr_t mhdr;
	wow_mcin_t mcin;
	wow_mtex_t mtex;
	wow_mwmo_t mwmo;
	wow_mwid_t mwid;
	wow_modf_t modf;
	wow_mmdx_t mmdx;
	wow_mmid_t mmid;
	wow_mddf_t mddf;
	wow_mfbo_t mfbo;
	wow_mcnk_t mcnk[16 * 16];
} wow_adt_file_t;

wow_adt_file_t *wow_adt_file_new(wow_mpq_file_t *file);
void wow_adt_file_delete(wow_adt_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
