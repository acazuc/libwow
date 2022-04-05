#ifndef WOW_COMMON_H
#define WOW_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct wow_memory_s
{
	void *(*malloc)(size_t size);
	void *(*realloc)(void *ptr, size_t size);
	void (*free)(void *ptr);
} wow_memory_t;

#define WOW_MALLOC(size) (wow_memory.malloc ? wow_memory.malloc(size) : malloc(size))
#define WOW_REALLOC(ptr, size) (wow_memory.realloc ? wow_memory.realloc(ptr, size) : realloc(ptr, size))
#define WOW_FREE(ptr) (wow_memory.free ? wow_memory.free(ptr) : free(ptr))

extern wow_memory_t wow_memory;

typedef struct wow_vec2f_s
{
	float x;
	float y;
} wow_vec2f_t;

typedef struct wow_vec3f_s
{
	float x;
	float y;
	float z;
} wow_vec3f_t;

typedef struct wow_vec3s_s
{
	int16_t x;
	int16_t y;
	int16_t z;
} wow_vec3s_t;

typedef struct wow_vec4f_s
{
	float x;
	float y;
	float z;
	float w;
} wow_vec4f_t;

typedef struct wow_vec4b_s
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
	uint8_t w;
} wow_vec4b_t;

typedef struct wow_quaternion_float_s
{
	float x;
	float y;
	float z;
	float w;
} wow_quaternion_float_t;

typedef struct wow_quaternion_short_s
{
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t w;
} wow_quaternion_short_t;

typedef struct wow_chunk_header_s
{
	uint32_t magic;
	uint32_t size;
} wow_chunk_header_t;

typedef struct wow_mver_s
{
	wow_chunk_header_t header;
	uint32_t version;
} wow_mver_t;

typedef struct wow_mwmo_s
{
	wow_chunk_header_t header;
	char *data;
	uint32_t data_len;
} wow_mwmo_t;

#define WOW_MODF_FLAGS_DESTROYABLE 0x1
#define WOW_MODF_FLAGS_LOD         0x2
#define WOW_MODF_FLAGS_UNKNOWN     0x4
#define WOW_MODF_FLAGS_FILEDATA    0x8

typedef struct wow_modf_data_s
{
	uint32_t name_id;
	uint32_t unique_id;
	wow_vec3f_t position;
	wow_vec3f_t rotation;
	wow_vec3f_t aabb0; 
	wow_vec3f_t aabb1;
	uint16_t flags;
	uint16_t doodad_set;
	uint16_t name_set;
	uint16_t unk;
} wow_modf_data_t;

typedef struct wow_modf_s
{
	wow_chunk_header_t header;
	wow_modf_data_t *data;
	uint32_t data_nb;
} wow_modf_t;

#ifdef __cplusplus
}
#endif

#endif
