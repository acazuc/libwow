#ifndef WOW_M2_H
#define WOW_M2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef struct wow_mpq_file_s wow_mpq_file_t;

typedef struct wow_m2_spline_vec3f_s
{
	wow_vec3f_t value;
	wow_vec3f_t in_tan;
	wow_vec3f_t out_tan;
} wow_m2_spline_vec3f_t;

typedef struct wow_m2_spline_float_s
{
	float value;
	float in_tan;
	float out_tan;
} wow_m2_spline_float_t;

typedef struct wow_m2_array_s
{
	uint32_t count;
	uint32_t offset;
} wow_m2_array_t;

typedef struct wow_m2_vertex_s
{
	wow_vec3f_t pos;
	uint8_t bone_weights[4];
	union
	{
		uint8_t bone_indices[4];
		uint32_t bone32;
	};
	wow_vec3f_t normal;
	wow_vec2f_t tex_coords[2];
} wow_m2_vertex_t;

typedef struct wow_m2_bounds_s
{
	wow_vec3f_t aabb0;
	wow_vec3f_t aabb1;
	float radius;
} wow_m2_bounds_t;

typedef struct wow_m2_range_s
{
	uint32_t minimum;
	uint32_t maximum;
} wow_m2_range_t;

#define WOW_M2_MATERIAL_FLAGS_UNLIT       0x01
#define WOW_M2_MATERIAL_FLAGS_UNFOGGED    0x02
#define WOW_M2_MATERIAL_FLAGS_UNCULLED    0x04
#define WOW_M2_MATERIAL_FLAGS_DEPTH_TEST  0x08
#define WOW_M2_MATERIAL_FLAGS_DEPTH_WRITE 0x10

typedef struct wow_m2_material_s
{
	uint16_t flags;
	uint16_t blend_mode;
} wow_m2_material_t;

typedef struct wow_m2_box_s
{
	wow_vec3f_t model_rotation_speed_min;
	wow_vec3f_t model_rotation_speed_max;
} wow_m2_box_t;

typedef struct wow_m2_track_base_int_s
{
	uint16_t type;
	uint16_t sequence;
	wow_m2_array_t interpolation_ranges;
	wow_m2_array_t timestamps;
} wow_m2_track_base_int_t;

typedef struct wow_m2_track_int_s
{
	wow_m2_track_base_int_t base;
	wow_m2_array_t values;
} wow_m2_track_int_t;

typedef struct wow_m2_camera_int_s
{
	uint32_t type;
	float fov;
	float far_clip;
	float near_clip;
	wow_m2_track_int_t position;
	wow_vec3f_t position_base;
	wow_m2_track_int_t target_position;
	wow_vec3f_t target_position_base;
	wow_m2_track_int_t roll;
} wow_m2_camera_int_t;

typedef struct wow_m2_event_s
{
	uint32_t identifier;
	uint32_t data;
	uint32_t bone;
	wow_vec3f_t position;
	wow_m2_track_base_int_t enabled;
} wow_m2_event_t;

#define WOW_M2_SKIN_SECTION_HAIR     0
#define WOW_M2_SKIN_SECTION_FACIAL1  1
#define WOW_M2_SKIN_SECTION_FACIAL2  2
#define WOW_M2_SKIN_SECTION_FACIAL3  3
#define WOW_M2_SKIN_SECTION_GLOVES   4
#define WOW_M2_SKIN_SECTION_BOOTS    5
#define WOW_M2_SKIN_SECTION_TAIL     6
#define WOW_M2_SKIN_SECTION_EARS     7
#define WOW_M2_SKIN_SECTION_WRISTS   8
#define WOW_M2_SKIN_SECTION_KNEEPADS 9
#define WOW_M2_SKIN_SECTION_CHEST    10
#define WOW_M2_SKIN_SECTION_PANTS    11
#define WOW_M2_SKIN_SECTION_TABARD   12
#define WOW_M2_SKIN_SECTION_TROUSERS 13
#define WOW_M2_SKIN_SECTION_UNK      14
#define WOW_M2_SKIN_SECTION_CLOAK    15
#define WOW_M2_SKIN_SECTION_MISC     16
#define WOW_M2_SKIN_SECTION_EYES     17
#define WOW_M2_SKIN_SECTION_BELT     18
#define WOW_M2_SKIN_SECTION_BONE     19

typedef struct wow_m2_skin_section_s
{
	/* 0x00 */ uint16_t skin_section_id;
	/* 0x02 */ uint16_t level;
	/* 0x04 */ uint16_t vertex_start;
	/* 0x06 */ uint16_t vertex_count;
	/* 0x08 */ uint16_t index_start;
	/* 0x0A */ uint16_t index_count;
	/* 0x0C */ uint16_t bone_count;
	/* 0x0E */ uint16_t bone_combo_index;
	/* 0x10 */ uint16_t bone_influences;
	/* 0x12 */ uint16_t center_bone_index;
	/* 0x14 */ wow_vec3f_t center_position;
	/* 0x20 */ wow_vec3f_t sort_center_position;
	/* 0x2C */ float sort_radius;
} wow_m2_skin_section_t;

#define WOW_M2_BATCH_FLAGS_INVERT      0x01
#define WOW_M2_BATCH_FLAGS_TRANSFORM   0x02
#define WOW_M2_BATCH_FLAGS_PROJECTED   0x04
#define WOW_M2_BATCH_FLAGS_UNKNOWN     0x08
#define WOW_M2_BATCH_FLAGS_BATCH       0x10
#define WOW_M2_BATCH_FLAGS_PROJECTED2  0x20
#define WOW_M2_BATCH_FLAGS_HAS_WEIGHTS 0x40

typedef struct wow_m2_batch_s
{
	/* 0x00 */ uint8_t flags;
	/* 0x01 */ int8_t priority_plane;
	/* 0x02 */ uint16_t shader_id;
	/* 0x04 */ uint16_t skin_section_index;
	/* 0x06 */ uint16_t geoset_index;
	/* 0x08 */ uint16_t color_index;
	/* 0x0A */ uint16_t material_index;
	/* 0x0C */ uint16_t material_layer;
	/* 0x0E */ uint16_t texture_count;
	/* 0x10 */ uint16_t texture_combo_index;
	/* 0x12 */ uint16_t texture_coord_combo_index;
	/* 0x13 */ uint16_t texture_weight_combo_index;
	/* 0x14 */ uint16_t texture_transform_combo_index;
} wow_m2_batch_t;

typedef struct wow_m2_skin_profile_int_s
{
	/* 0x00 */ wow_m2_array_t vertexes;  /* uint16_t */
	/* 0x08 */ wow_m2_array_t indices;   /* uint16_t */
	/* 0x10 */ wow_m2_array_t bones;     /* uint32_t */
	/* 0x18 */ wow_m2_array_t sections;  /* wow_m2_skin_section_t */
	/* 0x20 */ wow_m2_array_t batches;   /* m2_batch_t */
	/* 0x28 */ uint32_t bone_count_max;
} wow_m2_skin_profile_int_t;

typedef struct wow_m2_ribbon_s
{
	uint32_t ribbon_id;
	uint32_t bone_index;
	wow_vec3f_t position;
	wow_m2_array_t texture_indices;   /* uint16_t */
	wow_m2_array_t material_indices;  /* uint16_t */
	wow_m2_track_int_t color;         /* wow_vec3f_t */
	wow_m2_track_int_t alpha;         /* wow_vec3f_t */
	wow_m2_track_int_t height_above;  /* float */
	wow_m2_track_int_t height_below;  /* float */
	float edges_per_second;
	float edge_lifetime;
	float gravity;
	uint16_t texture_rows;
	uint16_t texture_cols;
	wow_m2_track_int_t tex_slot;      /* uint16_t */
	wow_m2_track_int_t visiblity;     /* uint8_t */
} wow_m2_ribbon_t;

typedef struct wow_m2_sequence_s
{
	uint16_t id;
	uint16_t variation_index;
	uint32_t start;
	uint32_t end;
	float movespeed;
	uint32_t flags;
	int16_t frequency;
	uint16_t padding;
	wow_m2_range_t replay;
	uint32_t blend_time;
	wow_m2_bounds_t bounds;
	int16_t variation_next;
	uint16_t alias_next;
} wow_m2_sequence_t;

#define WOW_M2_BONE_BILLBOARD               (WOW_M2_BONE_CYLINDRICAL_BILLBOARD | WOW_M2_BONE_SPHERICAL_BILLBOARD)
#define WOW_M2_BONE_CYLINDRICAL_BILLBOARD   (WOW_M2_BONE_CYLINDRICAL_X_BILLBOARD | WOW_M2_BONE_CYLINDRICAL_Y_BILLBOARD | WOW_M2_BONE_CYLINDRICAL_Z_BILLBOARD)
#define WOW_M2_BONE_SPHERICAL_BILLBOARD     0x0008
#define WOW_M2_BONE_CYLINDRICAL_X_BILLBOARD 0x0010
#define WOW_M2_BONE_CYLINDRICAL_Y_BILLBOARD 0x0020
#define WOW_M2_BONE_CYLINDRICAL_Z_BILLBOARD 0x0040
#define WOW_M2_BONE_TRANSFORMED             0x0200
#define WOW_M2_BONE_KINEMATIC               0x0400
#define WOW_M2_BONE_HELMET_ANIM_SCALED      0x1000

typedef struct wow_m2_bone_int_s
{
	int32_t key_bone_id;
	uint32_t flags;
	int16_t parent_bone;
	uint16_t submesh_id;
	uint32_t bone_name_crc;
	wow_m2_track_int_t translation; /* wow_vec3f_t */
	wow_m2_track_int_t rotation;    /* wow_quaternion_short_t */
	wow_m2_track_int_t scale;       /* wow_vec3f_t */
	wow_vec3f_t pivot;
} wow_m2_bone_int_t;

typedef struct wow_m2_loop_s
{
	uint32_t timestamp;
} wow_m2_loop_t;

typedef struct wow_m2_texture_weight_int_s
{
	wow_m2_track_int_t weight; /* int16_t */
} wow_m2_texture_weight_int_t;

typedef struct wow_m2_color_int_s
{
	wow_m2_track_int_t color; /* wow_vec3f_t */
	wow_m2_track_int_t alpha; /* int16_t */
} wow_m2_color_int_t;

typedef struct wow_m2_light_int_s
{
	uint16_t type;
	int16_t bone;
	wow_vec3f_t position;
	wow_m2_track_int_t ambient_color;     /* wow_vec3f_t */
	wow_m2_track_int_t ambient_intensity; /* float */
	wow_m2_track_int_t diffuse_color;     /* wow_vec3f_t */
	wow_m2_track_int_t diffuse_intensity; /* float */
	wow_m2_track_int_t attenuation_start; /* float */
	wow_m2_track_int_t attenuation_end;   /* float */
	wow_m2_track_int_t visibility;        /* uint8_t */
} wow_m2_light_int_t;

#define WOW_M2_TEXTURE_FLAG_CLAMP_S 0x1
#define WOW_M2_TEXTURE_FLAG_CLAMP_T 0x2

typedef struct wow_m2_texture_int_s
{
	uint32_t type;
	uint32_t flags;
	wow_m2_array_t filename;
} wow_m2_texture_int_t;

typedef struct wow_m2_texture_transform_int_s
{
	wow_m2_track_int_t translation; /* wow_vec3f_t */
	wow_m2_track_int_t rotation;    /* wow_quaternion_float_t */
	wow_m2_track_int_t scaling;     /* wow_vec3f_t */
} wow_m2_texture_transform_int_t;

typedef struct wow_m2_attachment_int_s
{
	uint32_t id;
	uint16_t bone;
	uint16_t unknown;
	wow_vec3f_t position;
	wow_m2_track_int_t animate_attached; /* uint8_t */
} wow_m2_attachment_int_t;

#define WOW_M2_PARTICLE_FLAG_LIGHTING   0x0000001
#define WOW_M2_PARTICLE_FLAG_UNK1       0x0000002
#define WOW_M2_PARTICLE_FLAG_UNK2       0x0000004
#define WOW_M2_PARTICLE_FLAG_WORLDUP    0x0000008
#define WOW_M2_PARTICLE_FLAG_DONTTRAIL  0x0000010
#define WOW_M2_PARTICLE_FLAG_UNLIT      0x0000020
#define WOW_M2_PARTICLE_FLAG_BURST      0x0000040
#define WOW_M2_PARTICLE_FLAG_MODELSPACE 0x0000080
#define WOW_M2_PARTICLE_FLAG_UNK3       0x0000100
#define WOW_M2_PARTICLE_FLAG_RANDOMIZED 0x0000200
#define WOW_M2_PARTICLE_FLAG_PINNED     0x0000400
#define WOW_M2_PARTICLE_FLAG_UNK4       0x0000800
#define WOW_M2_PARTICLE_FLAG_XYQUAD     0x0001000
#define WOW_M2_PARTICLE_FLAG_GROUND     0x0002000
#define WOW_M2_PARTICLE_FLAG_UNK5       0x0004000
#define WOW_M2_PARTICLE_FLAG_UNK6       0x0008000
#define WOW_M2_PARTICLE_FLAG_RAND_TEX   0x0010000
#define WOW_M2_PARTICLE_FLAG_OUTWARD    0x0020000
#define WOW_M2_PARTICLE_FLAG_INWARD     0x0040000
#define WOW_M2_PARTICLE_FLAG_SCALEVARY  0x0080000
#define WOW_M2_PARTICLE_FLAG_UNK7       0x0100000
#define WOW_M2_PARTICLE_FLAG_RAND_FLIP  0x0200000
#define WOW_M2_PARTICLE_FLAG_NO_DIST    0x0400000
#define WOW_M2_PARTICLE_FLAG_COMP_GRAV  0x0800000
#define WOW_M2_PARTICLE_FLAG_BONE_JOINT 0x1000000
#define WOW_M2_PARTICLE_FLAG_UNK8       0x2000000
#define WOW_M2_PARTICLE_FLAG_RATE_DIST  0x4000000
#define WOW_M2_PARTICLE_FLAG_UNK9       0x8000000

typedef struct wow_m2_particle_int_s
{
	/* 0x000 */ uint32_t id;
	/* 0x004 */ uint32_t flags;
	/* 0x008 */ wow_vec3f_t position;
	/* 0x014 */ uint16_t bone;
	/* 0x016 */ uint16_t texture;
	/* 0x018 */ wow_m2_array_t geometry_model_filename;  /* char */
	/* 0x020 */ wow_m2_array_t recursion_model_filename; /* char */
	union
	{
		struct
		{
			/* 0x028 */ uint8_t blending_type2;
			/* 0x029 */ uint8_t emitter_type2;
			/* 0x02A */ uint16_t particle_color_index;
		};
		struct
		{
			/* 0x028 */ uint16_t blending_type;
			/* 0x02A */ uint16_t emitter_type;
		};
	};
	/* 0x02C */ uint8_t particle_type;
	/* 0x02D */ uint8_t head_or_tail;
	/* 0x02E */ uint16_t texture_tile_rotation;
	/* 0x030 */ uint16_t texture_dimensions_rows;
	/* 0x032 */ uint16_t texture_dimensions_columns;
	/* 0x034 */ wow_m2_track_int_t emission_speed;       /* float */
	/* 0x050 */ wow_m2_track_int_t speed_variation;      /* float */
	/* 0x06C */ wow_m2_track_int_t vertical_range;       /* float */
	/* 0x088 */ wow_m2_track_int_t horizontal_range;     /* float */
	/* 0x0A4 */ wow_m2_track_int_t gravity;              /* float */
	/* 0x0C0 */ wow_m2_track_int_t lifespan;             /* float */
	/* 0x0DC */ wow_m2_track_int_t emission_rate;        /* float */
	/* 0x0F8 */ wow_m2_track_int_t emission_area_length; /* float */
	/* 0x114 */ wow_m2_track_int_t emission_area_width;  /* float */
	/* 0x130 */ wow_m2_track_int_t z_source;             /* float */
	/* 0x14C */ float mid_point;
	/* 0x150 */ wow_vec4b_t color_values[3];
	/* 0x15C */ float scale_values[3];
	/* 0x168 */ uint16_t lifespan_uv_anim[3];
	/* 0x16E */ uint16_t decay_uv_anim[3];
	/* 0x174 */ int16_t tail_uv_anim[2];
	/* 0x178 */ int16_t tail_decay_uv_anim[2];
	/* 0x17C */ float tail_length;
	/* 0x180 */ float twinkle_speed;
	/* 0x184 */ float twinkle_percent;
	/* 0x188 */ float twinkle_scale_min;
	/* 0x18C */ float twinkle_scale_max;
	/* 0x190 */ float burst_multiplier;
	/* 0x194 */ float drag;
	/* 0x198 */ float spin;
	/* 0x19C */ wow_m2_box_t tumble;
	/* 0x1B4 */ wow_vec3f_t wind_vector;
	/* 0x1C0 */ float wind_time;
	/* 0x1C4 */ float follow_speed1;
	/* 0x1C8 */ float follow_scale1;
	/* 0x1CC */ float follow_speed2;
	/* 0x1D0 */ float follow_scale2;
	/* 0x1D4 */ wow_m2_array_t spline_points;  /* wow_vec3f_t */
	/* 0x1DC */ wow_m2_track_int_t enabled_in; /* uint8_t */
} wow_m2_particle_int_t;

#define WOW_M2_HEADER_FLAG_TILT_X                     0x01
#define WOW_M2_HEADER_FLAG_TILT_Y                     0x02
#define WOW_M2_HEADER_FLAG_UNKNOWN1                   0x04
#define WOW_M2_HEADER_FLAG_USE_TEXTURE_COMBINER_COMBO 0x08
#define WOW_M2_HEADER_FLAG_UNKNOWN2                   0x10

typedef struct wow_m2_header_s
{
	/* 0x000 */ uint32_t magic;
	/* 0x004 */ uint32_t version;
	/* 0x008 */ wow_m2_array_t name; /* char */
	/* 0x010 */ uint32_t flags;
	/* 0x014 */ wow_m2_array_t global_loops;                    /* wow_m2_loop_t */
	/* 0x01C */ wow_m2_array_t sequences;                       /* wow_m2_sequence_t */
	/* 0x024 */ wow_m2_array_t sequence_lookups;                /* uint16_t */
	/* 0x02C */ wow_m2_array_t playable_animation_lookup;       /* uint8_t */
	/* 0x034 */ wow_m2_array_t bones;                           /* wow_m2_bone_int_t */
	/* 0x03C */ wow_m2_array_t key_bone_lookup_table;           /* uint16_t */
	/* 0x044 */ wow_m2_array_t vertexes;                        /* wow_m2_vertex_t */
	/* 0x04C */ wow_m2_array_t skin_profiles;                   /* wow_m2_skin_profile_t */
	/* 0x054 */ wow_m2_array_t colors;                          /* wow_m2_color_int_t */
	/* 0x05C */ wow_m2_array_t textures;                        /* wow_m2_texture_t */
	/* 0x064 */ wow_m2_array_t texture_weights;                 /* wow_m2_texture_weight_int_t */
	/* 0x06C */ wow_m2_array_t unknown;                         /* wow_m2_texture_transform_int_t */
	/* 0x074 */ wow_m2_array_t texture_transforms;              /* wow_m2_texture_transform_int_t */
	/* 0x07C */ wow_m2_array_t replacable_texture_lookup;       /* uint16_t */
	/* 0x084 */ wow_m2_array_t materials;                       /* wow_m2_material_t */
	/* 0x08C */ wow_m2_array_t bone_lookup_table;               /* uint16_t */
	/* 0x094 */ wow_m2_array_t texture_lookup_table;            /* uint16_t */
	/* 0x09C */ wow_m2_array_t texture_unit_lookup_table;       /* uint16_t */
	/* 0x0A4 */ wow_m2_array_t texture_weights_lookup_table;    /* uint16_t */
	/* 0x0AC */ wow_m2_array_t texture_transforms_lookup_table; /* uint16_t */
	/* 0x0B4 */ wow_vec3f_t aabb0;
	/* 0x0C0 */ wow_vec3f_t aabb1;
	/* 0x0CC */ float bounding_sphere_radius;
	/* 0x0D0 */ wow_vec3f_t caabb0;
	/* 0x0DC */ wow_vec3f_t caabb1;
	/* 0x0E8 */ float collision_sphere_radius;
	/* 0x0EC */ wow_m2_array_t collision_triangles;     /* uint16_t */
	/* 0x0F4 */ wow_m2_array_t collision_vertexes;      /* vec3 */
	/* 0x0FC */ wow_m2_array_t collision_normals;       /* vec3 */
	/* 0x104 */ wow_m2_array_t attachments;             /* wow_m2_attachment_t */
	/* 0x10C */ wow_m2_array_t attachment_lookup_table; /* uint16_t */
	/* 0x114 */ wow_m2_array_t events;                  /* wow_m2_event_t */
	/* 0x11C */ wow_m2_array_t lights;                  /* wow_m2_light_int_t */
	/* 0x124 */ wow_m2_array_t cameras;                 /* wow_m2_camera_int_t */
	/* 0x12C */ wow_m2_array_t camera_lookup_table;     /* uint16_t */
	/* 0x134 */ wow_m2_array_t ribbon_emitters;         /* wow_m2_ribbon_t */
	/* 0x13C */ wow_m2_array_t particles;               /* wow_m2_particle_t */
	/* 0x144 */ wow_m2_array_t texture_combiner_combos; /* uint16_t */
} wow_m2_header_t;

typedef struct wow_m2_skin_profile_bone_s
{
	union
	{
		uint8_t values[4];
		uint32_t u32;
	};
} wow_m2_skin_profile_bone_t;

typedef struct wow_m2_skin_profile_s
{
	wow_m2_skin_section_t *sections;
	uint32_t sections_nb;
	wow_m2_batch_t *batches;
	uint32_t batches_nb;
	uint16_t *vertexes;
	uint32_t vertexes_nb;
	uint16_t *indices;
	uint32_t indices_nb;
	wow_m2_skin_profile_bone_t *bones;
	uint32_t bones_nb;
} wow_m2_skin_profile_t;

typedef struct wow_m2_track_s
{
	uint16_t type;
	uint16_t sequence;
	wow_m2_range_t *interpolation_ranges;
	uint32_t interpolation_ranges_nb;
	uint32_t *timestamps;
	uint32_t timestamps_nb;
	void *values;
	uint32_t values_nb;
} wow_m2_track_t;

typedef struct wow_m2_texture_transform_s
{
	wow_m2_track_t translation; /* wow_vec3f_t */
	wow_m2_track_t rotation;    /* wow_quaternion_float_t */
	wow_m2_track_t scaling;     /* wow_vec3f_t */
} wow_m2_texture_transform_t;

typedef struct wow_m2_texture_weight_s
{
	wow_m2_track_t weight; /* int16_t */
} wow_m2_texture_weight_t;

typedef struct wow_m2_color_s
{
	wow_m2_track_t color; /* wow_vec3f_t */
	wow_m2_track_t alpha; /* int16_t */
} wow_m2_color_t;

typedef struct wow_m2_bone_s
{
	int32_t key_bone_id;
	uint32_t flags;
	int16_t parent_bone;
	uint16_t submesh_id;
	uint32_t bone_name_crc;
	wow_m2_track_t translation; /* wow_vec3f_t */
	wow_m2_track_t rotation;    /* wow_quaternion_short_t */
	wow_m2_track_t scale;       /* wow_vec3f_t */
	wow_vec3f_t pivot;
} wow_m2_bone_t;

typedef struct wow_m2_light_s
{
	uint16_t type;
	int16_t bone;
	wow_vec3f_t position;
	wow_m2_track_t ambient_color;     /* wow_vec3f_t */
	wow_m2_track_t ambient_intensity; /* float */
	wow_m2_track_t diffuse_color;     /* wow_vec3f_t */
	wow_m2_track_t diffuse_intensity; /* float */
	wow_m2_track_t attenuation_start; /* float */
	wow_m2_track_t attenuation_end;   /* float */
	wow_m2_track_t visibility;        /* uint8_t */
} wow_m2_light_t;

typedef struct wow_m2_camera_s
{
	uint32_t type;
	float fov;
	float far_clip;
	float near_clip;
	wow_m2_track_t position;        /* m2_spline_vec3f_t */
	wow_vec3f_t position_base;
	wow_m2_track_t target_position; /* m2_spline_vec3f_t */
	wow_vec3f_t target_position_base;
	wow_m2_track_t roll;            /* m2_spline_float_t */
} wow_m2_camera_t;

typedef struct wow_m2_particle_s
{
	uint32_t id;
	uint32_t flags;
	wow_vec3f_t position;
	uint16_t bone;
	uint16_t texture;
	char *geometry_model_filename;
	char *recursion_model_filename;
	union
	{
		struct
		{
			uint8_t blending_type2;
			uint8_t emitter_type2;
			uint16_t particle_color_index;
		};
		struct
		{
			uint16_t blending_type;
			uint16_t emitter_type;
		};
	};
	uint8_t particle_type;
	uint8_t head_or_tail;
	uint16_t texture_tile_rotation;
	uint16_t texture_dimensions_rows;
	uint16_t texture_dimensions_columns;
	wow_m2_track_t emission_speed;       /* float */
	wow_m2_track_t speed_variation;      /* float */
	wow_m2_track_t vertical_range;       /* float */
	wow_m2_track_t horizontal_range;     /* float */
	wow_m2_track_t gravity;              /* float */
	wow_m2_track_t lifespan;             /* float */
	wow_m2_track_t emission_rate;        /* float */
	wow_m2_track_t emission_area_length; /* float */
	wow_m2_track_t emission_area_width;  /* float */
	wow_m2_track_t z_source;             /* float */
	float mid_point;
	wow_vec4b_t color_values[3];
	float scale_values[3];
	uint16_t lifespan_uv_anim[3];
	uint16_t decay_uv_anim[3];
	int16_t tail_uv_anim[2];
	int16_t tail_decay_uv_anim[2];
	float tail_length;
	float twinkle_speed;
	float twinkle_percent;
	float twinkle_scale_min;
	float twinkle_scale_max;
	float burst_multiplier;
	float drag;
	float spin;
	wow_m2_box_t tumble;
	wow_vec3f_t wind_vector;
	float wind_time;
	float follow_speed1;
	float follow_scale1;
	float follow_speed2;
	float follow_scale2;
	wow_vec3f_t *spline_points; /* wow_vec3f_t */
	uint32_t spline_points_nb;
	wow_m2_track_t enabled_in;  /* uint8_t */
} wow_m2_particle_t;

typedef struct wow_m2_texture_s
{
	uint32_t type;
	uint32_t flags;
	char *filename;
} wow_m2_texture_t;

typedef struct wow_m2_attachment_s
{
	uint32_t id;
	uint16_t bone;
	uint16_t unknown;
	wow_vec3f_t position;
	wow_m2_track_t animate_attached; /* uint8_t */
} wow_m2_attachment_t;

typedef struct wow_m2_file_s
{
	wow_m2_texture_transform_t *texture_transforms;
	wow_m2_texture_weight_t *texture_weights;
	wow_m2_skin_profile_t *skin_profiles;
	wow_m2_attachment_t *attachments;
	wow_m2_material_t *materials;
	wow_m2_sequence_t *sequences;
	wow_m2_particle_t *particles;
	wow_m2_texture_t *textures;
	wow_m2_vertex_t *vertexes;
	wow_m2_camera_t *cameras;
	wow_m2_color_t *colors;
	wow_m2_light_t *lights;
	wow_m2_bone_t *bones;
	uint16_t *texture_transforms_lookups;
	uint16_t *texture_weights_lookups;
	uint16_t *texture_combiner_combos;
	uint16_t *texture_unit_lookups;
	uint16_t *attachment_lookups;
	uint16_t *key_bone_lookups;
	uint16_t *texture_lookups;
	uint16_t *bone_lookups;
	uint32_t texture_transforms_nb;
	uint32_t texture_weights_nb;
	uint32_t skin_profiles_nb;
	uint32_t attachments_nb;
	uint32_t materials_nb;
	uint32_t sequences_nb;
	uint32_t particles_nb;
	uint32_t textures_nb;
	uint32_t vertexes_nb;
	uint32_t cameras_nb;
	uint32_t colors_nb;
	uint32_t lights_nb;
	uint32_t bones_nb;
	uint32_t texture_transforms_lookups_nb;
	uint32_t texture_weights_lookups_nb;
	uint32_t texture_combiner_combos_nb;
	uint32_t texture_unit_lookups_nb;
	uint32_t attachment_lookups_nb;
	uint32_t key_bone_lookups_nb;
	uint32_t texture_lookups_nb;
	uint32_t bone_lookups_nb;
	char *name;
	wow_m2_header_t header;
} wow_m2_file_t;

wow_m2_file_t *wow_m2_file_new(wow_mpq_file_t *mpq);
void wow_m2_file_delete(wow_m2_file_t *file);

wow_m2_texture_transform_t *wow_m2_texture_transforms_dup(const wow_m2_texture_transform_t *dup, uint32_t nb);
void wow_m2_texture_transforms_delete(wow_m2_texture_transform_t *val, uint32_t nb);

wow_m2_texture_weight_t *wow_m2_texture_weights_dup(const wow_m2_texture_weight_t *dup, uint32_t nb);
void wow_m2_texture_weights_delete(wow_m2_texture_weight_t *val, uint32_t ptr);

wow_m2_attachment_t *wow_m2_attachments_dup(const wow_m2_attachment_t *dup, uint32_t nb);
void wow_m2_attachments_delete(wow_m2_attachment_t *val, uint32_t nb);

wow_m2_sequence_t *wow_m2_sequences_dup(const wow_m2_sequence_t *dup, uint32_t nb);
void wow_m2_sequences_delete(wow_m2_sequence_t *val, uint32_t nb);

wow_m2_particle_t *wow_m2_particles_dup(const wow_m2_particle_t *dup, uint32_t nb);
void wow_m2_particles_delete(wow_m2_particle_t *val, uint32_t nb);

wow_m2_texture_t *wow_m2_textures_dup(const wow_m2_texture_t *dup, uint32_t nb);
void wow_m2_textures_delete(wow_m2_texture_t *val, uint32_t nb);

wow_m2_camera_t *wow_m2_cameras_dup(const wow_m2_camera_t *dup, uint32_t nb);
void wow_m2_cameras_delete(wow_m2_camera_t *val, uint32_t nb);

wow_m2_color_t *wow_m2_colors_dup(const wow_m2_color_t *dup, uint32_t nb);
void wow_m2_colors_delete(wow_m2_color_t *val, uint32_t nb);

wow_m2_light_t *wow_m2_lights_dup(const wow_m2_light_t *dup, uint32_t nb);
void wow_m2_lights_delete(wow_m2_light_t *val, uint32_t nb);

wow_m2_bone_t *wow_m2_bones_dup(const wow_m2_bone_t *dup, uint32_t nb);
void wow_m2_bones_delete(wow_m2_bone_t *val, uint32_t nb);

#ifdef __cplusplus
}
#endif

#endif
