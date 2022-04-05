#ifndef WOW_WMO_GROUP_H
#define WOW_WMO_GROUP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_molr_s
{
	wow_chunk_header_t header;
	uint16_t *lights;
	uint32_t lights_nb;
} wow_molr_t;

typedef struct wow_modr_s
{
	wow_chunk_header_t header;
	uint16_t *doodads;
	uint32_t doodads_nb;
} wow_modr_t;

#define WOW_MOBN_NODE_FLAGS_XAXIS     0x0
#define WOW_MOBN_NODE_FLAGS_YAXIS     0x1
#define WOW_MOBN_NODE_FLAGS_ZAXIS     0x2
#define WOW_MOBN_NODE_FLAGS_AXIS_MASK 0x3
#define WOW_MOBN_NODE_FLAGS_LEAF      0x4
#define WOW_MOBN_NODE_FLAGS_NO_CHILD  0xffff

typedef struct wow_mobn_node_s
{
	uint16_t flags;
	int16_t neg_child;
	int16_t pos_child;
	uint16_t faces_nb;
	uint32_t face_start;
	float plane_dist;
} wow_mobn_node_t;

typedef struct wow_mobn_s
{
	wow_chunk_header_t header;
	wow_mobn_node_t *nodes;
	uint32_t nodes_nb;
} wow_mobn_t;

typedef struct wow_mobr_s
{
	wow_chunk_header_t header;
	uint16_t *indices;
	uint32_t indices_nb;
} wow_mobr_t;

typedef struct wow_mocv_s
{
	wow_chunk_header_t header;
	wow_vec4b_t *colors;
	uint32_t colors_nb;
} wow_mocv_t;

typedef struct wow_mliq_vert_s
{
	union
	{
		struct
		{
			uint8_t flow1;
			uint8_t flow2;
			uint8_t flow1_pct;
			uint8_t filler;
			float height;
		}  water;
		struct
		{
			int16_t s;
			int16_t t;
			float height;
		} magma;
	};
} wow_mliq_vert_t;

typedef struct wow_mliq_header_s
{
	wow_chunk_header_t header;
	uint32_t xverts;
	uint32_t yverts;
	uint32_t xtiles;
	uint32_t ytiles;
	wow_vec3f_t coords;
	uint16_t material;
} wow_mliq_header_t;

#define WOW_MLIQ_TILE_LIQUID_TYPE 0x0F
#define WOW_MLIQ_TILE_FISHABLE    0x40
#define WOW_MLIQ_TILE_SHARED      0x80

typedef struct wow_mliq_s
{
	wow_mliq_header_t header;
	wow_mliq_vert_t *vertexes;
	uint32_t vertexes_nb;
	uint8_t *tiles;
	uint32_t tiles_nb;
} wow_mliq_t;

typedef struct wow_moba_data_s
{
	wow_vec3s_t aabb0;
	wow_vec3s_t aabb1;
	uint32_t start_index;
	uint16_t count;
	uint16_t min_index;
	uint16_t max_index;
	uint8_t flag_unknown : 1;
	uint8_t material_id;
} wow_moba_data_t;

typedef struct wow_moba_s
{
	wow_chunk_header_t header;
	wow_moba_data_t *data;
	uint32_t data_nb;
} wow_moba_t;

typedef struct wow_motv_s
{
	wow_chunk_header_t header;
	wow_vec2f_t *tex_coords;
	uint32_t tex_coords_nb;
} wow_motv_t;

typedef struct wow_monr_s
{
	wow_chunk_header_t header;
	wow_vec3f_t *normals;
	uint32_t normals_nb;
} wow_monr_t;

typedef struct wow_movt_s
{
	wow_chunk_header_t header;
	wow_vec3f_t *vertexes;
	uint32_t vertexes_nb;
} wow_movt_t;

typedef struct wow_movi_s
{
	wow_chunk_header_t header;
	uint16_t *indices;
	uint32_t indices_nb;
} wow_movi_t;

#define WOW_MOPY_FLAGS_TRANSPARENT  0x01
#define WOW_MOPY_FLAGS_NOCAMCOLLIDE 0x02
#define WOW_MOPY_FLAGS_DETAIL       0x04
#define WOW_MOPY_FLAGS_COLLISION    0x08
#define WOW_MOPY_FLAGS_HINT         0x10
#define WOW_MOPY_FLAGS_RENDER       0x20
#define WOW_MOPY_FLAGS_UNKNOWN2     0x40
#define WOW_MOPY_FLAGS_COLLIDE_HIT  0x80

typedef struct wow_mopy_data_s
{
	uint8_t flags;
	uint8_t material_id;
} wow_mopy_data_t;

typedef struct wow_mopy_s
{
	wow_chunk_header_t header;
	wow_mopy_data_t *data;
	uint32_t data_nb;
} wow_mopy_t;

#define WOW_MOGP_FLAGS_BSP        0x00000001
#define WOW_MOGP_FLAGS_LIGHT_MAP  0x00000002
#define WOW_MOGP_FLAGS_COLOR      0x00000004
#define WOW_MOGP_FLAGS_OUTDOOR    0x00000008
#define WOW_MOGP_FLAGS_5          0x00000010
#define WOW_MOGP_FLAGS_6          0x00000020
#define WOW_MOGP_FLAGS_EXT_LIGHT  0x00000040
#define WOW_MOGP_FLAGS_8          0x00000080
#define WOW_MOGP_FLAGS_9          0x00000100
#define WOW_MOGP_FLAGS_LIGHT      0x00000200
#define WOW_MOGP_FLAGS_11         0x00000400
#define WOW_MOGP_FLAGS_DOODAD     0x00000800
#define WOW_MOGP_FLAGS_LIQUID     0x00001000
#define WOW_MOGP_FLAGS_INDOOR     0x00002000
#define WOW_MOGP_FLAGS_15         0x00004000
#define WOW_MOGP_FLAGS_16         0x00008000
#define WOW_MOGP_FLAGS_17         0x00010000
#define WOW_MOGP_FLAGS_MORI_MORB  0x00020000
#define WOW_MOGP_FLAGS_SKYBOX     0x00040000
#define WOW_MOGP_FLAGS_OCEAN      0x00080000
#define WOW_MOGP_FLAGS_21         0x00100000
#define WOW_MOGP_FLAGS_MOUNT      0x00200000
#define WOW_MOGP_FLAGS_23         0x00400000
#define WOW_MOGP_FLAGS_24         0x00800000
#define WOW_MOGP_FLAGS_COLOR2     0x01000000
#define WOW_MOGP_FLAGS_MOTV2      0x02000000
#define WOW_MOGP_FLAGS_ANTIPORTAL 0x04000000
#define WOW_MOGP_FLAGS_28         0x08000000
#define WOW_MOGP_FLAGS_29         0x10000000
#define WOW_MOGP_FLAGS_30         0x20000000
#define WOW_MOGP_FLAGS_MOTV3      0x40000000
#define WOW_MOGP_FLAGS_32         0x80000000

typedef struct wow_mogp_s
{
	wow_chunk_header_t header;
	uint32_t group_name;
	uint32_t descriptive_group_name;
	uint32_t flags;
	wow_vec3f_t aabb0;
	wow_vec3f_t aabb1;
	uint16_t portal_start;
	uint16_t portal_count;
	uint16_t trans_batch_count;
	uint16_t ext_batch_count;
	uint16_t int_batch_count;
	uint16_t padding_or_batch_type_d;
	uint8_t fog_ids[4];
	uint32_t group_liquid;
	uint32_t group_id;
	uint32_t flags2;
	uint32_t unk1;
} wow_mogp_t;

typedef struct wow_wmo_file_s wow_wmo_file_t;

typedef struct wow_wmo_group_file_s
{
	wow_mver_t mver;
	wow_mogp_t mogp;
	wow_mopy_t mopy;
	wow_movi_t movi;
	wow_movt_t movt;
	wow_monr_t monr;
	wow_motv_t motv;
	wow_moba_t moba;
	wow_molr_t molr;
	wow_modr_t modr;
	wow_mobn_t mobn;
	wow_mobr_t mobr;
	wow_mocv_t mocv;
	wow_mliq_t mliq;
} wow_wmo_group_file_t;

wow_wmo_group_file_t *wow_wmo_group_file_new(wow_mpq_file_t *mpq);
void wow_wmo_group_file_delete(wow_wmo_group_file_t *file);

#ifdef __cplusplus
}
#endif

#endif
