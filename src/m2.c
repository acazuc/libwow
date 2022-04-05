#include "m2.h"

#include "common.h"
#include "mpq.h"

#include <stdlib.h>
#include <string.h>

static void *array_read(size_t data_size, uint32_t *elem_nb, wow_m2_array_t *array, wow_mpq_file_t *mpq)
{
	void *data = WOW_MALLOC(data_size * array->count);
	if (!data)
		return NULL;
	wow_mpq_seek(mpq, array->offset, SEEK_SET);
	wow_mpq_read(mpq, data, data_size * array->count);
	if (elem_nb)
		*elem_nb = array->count;
	return data;
}

static bool read_track(wow_m2_track_t *track, size_t data_size, wow_m2_track_int_t *track_int, wow_mpq_file_t *mpq)
{
	track->type = track_int->base.type;
	track->sequence = track_int->base.sequence;
	track->interpolation_ranges = array_read(sizeof(wow_m2_range_t), &track->interpolation_ranges_nb, &track_int->base.interpolation_ranges, mpq);
	if (!track->interpolation_ranges)
		return false;
	track->timestamps = array_read(sizeof(uint32_t), &track->timestamps_nb, &track_int->base.timestamps, mpq);
	if (!track->timestamps)
		return false;
	track->values = array_read(data_size, &track->values_nb, &track_int->values, mpq);
	if (!track->values)
		return false;
	return true;
}

static bool read_texture_transforms(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_texture_transform_int_t *texture_transforms = array_read(sizeof(*texture_transforms), &file->texture_transforms_nb, &file->header.texture_transforms, mpq);
	if (!texture_transforms)
		goto err;
	file->texture_transforms = WOW_MALLOC(sizeof(*file->texture_transforms) * file->texture_transforms_nb);
	if (!file->texture_transforms)
		goto err;
	memset(file->texture_transforms, 0, sizeof(*file->texture_transforms) * file->texture_transforms_nb);
	for (uint32_t i = 0; i < file->texture_transforms_nb; ++i)
	{
		wow_m2_texture_transform_t *texture_transform = &file->texture_transforms[i];
		if (!read_track(&texture_transform->translation, sizeof(wow_vec3f_t), &texture_transforms[i].translation, mpq)
		 || !read_track(&texture_transform->rotation, sizeof(wow_quaternion_float_t), &texture_transforms[i].rotation, mpq)
		 || !read_track(&texture_transform->scaling, sizeof(wow_vec3f_t), &texture_transforms[i].scaling, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(texture_transforms);
	return ret;
}

static bool read_texture_weights(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_texture_weight_int_t *texture_weights = array_read(sizeof(*texture_weights), &file->texture_weights_nb, &file->header.texture_weights, mpq);
	if (!texture_weights)
		goto err;
	file->texture_weights = WOW_MALLOC(sizeof(*file->texture_weights) * file->texture_weights_nb);
	if (!file->texture_weights)
		goto err;
	memset(file->texture_weights, 0, sizeof(*file->texture_weights) * file->texture_weights_nb);
	for (uint32_t i = 0;i  < file->texture_weights_nb; ++i)
	{
		wow_m2_texture_weight_t *texture_weight = &file->texture_weights[i];
		if (!read_track(&texture_weight->weight, sizeof(int16_t), &texture_weights[i].weight, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(texture_weights);
	return ret;
}

static bool read_skin_profiles(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_skin_profile_int_t *skin_profiles = array_read(sizeof(*skin_profiles), &file->skin_profiles_nb, &file->header.skin_profiles, mpq);
	if (!skin_profiles)
		goto err;
	file->skin_profiles = WOW_MALLOC(sizeof(*file->skin_profiles) * file->skin_profiles_nb);
	if (!file->skin_profiles)
		goto err;
	memset(file->skin_profiles, 0, sizeof(*file->skin_profiles) * file->skin_profiles_nb);
	for (uint32_t i = 0; i < file->skin_profiles_nb; ++i)
	{
		wow_m2_skin_profile_t *skin_profile = &file->skin_profiles[i];
		skin_profile->sections = array_read(sizeof(*skin_profile->sections), &skin_profile->sections_nb, &skin_profiles[i].sections, mpq);
		if (!skin_profile->sections)
			goto err;
		skin_profile->batches = array_read(sizeof(*skin_profile->batches), &skin_profile->batches_nb, &skin_profiles[i].batches, mpq);
		if (!skin_profile->batches)
			goto err;
		skin_profile->vertexes = array_read(sizeof(*skin_profile->vertexes), &skin_profile->vertexes_nb, &skin_profiles[i].vertexes, mpq);
		if (!skin_profile->vertexes)
			goto err;
		skin_profile->indices = array_read(sizeof(*skin_profile->indices), &skin_profile->indices_nb, &skin_profiles[i].indices, mpq);
		if (!skin_profile->indices)
			goto err;
		skin_profile->bones = array_read(sizeof(*skin_profile->bones), &skin_profile->bones_nb, &skin_profiles[i].bones, mpq);
		if (!skin_profile->bones)
			goto err;
	}
	ret = true;

err:
	WOW_FREE(skin_profiles);
	return ret;
}

static bool read_attachments(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_attachment_int_t *attachments = array_read(sizeof(*attachments), &file->attachments_nb, &file->header.attachments, mpq);
	if (!attachments)
		goto err;
	file->attachments = WOW_MALLOC(sizeof(*file->attachments) * file->attachments_nb);
	if (!file->attachments)
		goto err;
	memset(file->attachments, 0, sizeof(*file->attachments) * file->attachments_nb);
	for (uint32_t i = 0; i < file->attachments_nb; ++i)
	{
		wow_m2_attachment_t *attachment = &file->attachments[i];
		attachment->id = attachments[i].id;
		attachment->bone = attachments[i].bone;
		attachment->unknown = attachments[i].unknown;
		attachment->position = attachments[i].position;
		if (!read_track(&attachment->animate_attached, sizeof(uint8_t), &attachments[i].animate_attached, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(attachments);
	return ret;
}

static bool read_particles(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_particle_int_t *particles = array_read(sizeof(*particles), &file->particles_nb, &file->header.particles, mpq);
	if (!particles)
		goto err;
	file->particles = WOW_MALLOC(sizeof(*file->particles) * file->particles_nb);
	if (!file->particles)
		goto err;
	memset(file->particles, 0, sizeof(*file->particles) * file->particles_nb);
	for (uint32_t i = 0; i < file->particles_nb; ++i)
	{
		wow_m2_particle_t *particle = &file->particles[i];
		particle->id = particles[i].id;
		particle->flags = particles[i].flags;
		particle->position = particles[i].position;
		particle->bone = particles[i].bone;
		particle->texture = particles[i].texture;
		particle->geometry_model_filename = array_read(sizeof(*particle->geometry_model_filename), NULL, &particles[i].geometry_model_filename, mpq);
		if (!particle->geometry_model_filename)
			goto err;
		particle->recursion_model_filename = array_read(sizeof(*particle->recursion_model_filename), NULL, &particles[i].recursion_model_filename, mpq);
		if (!particle->recursion_model_filename)
			goto err;
		particle->blending_type = particles[i].blending_type;
		particle->emitter_type = particles[i].emitter_type;
		particle->particle_type = particles[i].particle_type;
		particle->head_or_tail = particles[i].head_or_tail;
		particle->texture_tile_rotation = particles[i].texture_tile_rotation;
		particle->texture_dimensions_rows = particles[i].texture_dimensions_rows;
		particle->texture_dimensions_columns = particles[i].texture_dimensions_columns;
		if (!read_track(&particle->emission_speed, sizeof(float), &particles[i].emission_speed, mpq)
		 || !read_track(&particle->speed_variation, sizeof(float), &particles[i].speed_variation, mpq)
		 || !read_track(&particle->vertical_range, sizeof(float), &particles[i].vertical_range, mpq)
		 || !read_track(&particle->horizontal_range, sizeof(float), &particles[i].horizontal_range, mpq)
		 || !read_track(&particle->gravity, sizeof(float), &particles[i].gravity, mpq)
		 || !read_track(&particle->lifespan, sizeof(float), &particles[i].lifespan, mpq)
		 || !read_track(&particle->emission_rate, sizeof(float), &particles[i].emission_rate, mpq)
		 || !read_track(&particle->emission_area_length, sizeof(float), &particles[i].emission_area_length, mpq)
		 || !read_track(&particle->emission_area_width, sizeof(float), &particles[i].emission_area_width, mpq)
		 || !read_track(&particle->z_source, sizeof(float), &particles[i].z_source, mpq))
			goto err;
		particle->mid_point = particles[i].mid_point;
		memcpy(particle->color_values, particles[i].color_values, sizeof(particle->color_values));
		memcpy(particle->scale_values, particles[i].scale_values, sizeof(particle->scale_values));
		memcpy(particle->lifespan_uv_anim, particles[i].lifespan_uv_anim, sizeof(particle->lifespan_uv_anim));
		memcpy(particle->decay_uv_anim, particles[i].decay_uv_anim, sizeof(particle->decay_uv_anim));
		memcpy(particle->tail_uv_anim, particles[i].tail_uv_anim, sizeof(particle->tail_uv_anim));
		memcpy(particle->tail_decay_uv_anim, particles[i].tail_decay_uv_anim, sizeof(particle->tail_decay_uv_anim));
		particle->tail_length = particles[i].tail_length;
		particle->twinkle_speed = particles[i].twinkle_speed;
		particle->twinkle_percent = particles[i].twinkle_percent;
		particle->twinkle_scale_min = particles[i].twinkle_scale_min;
		particle->twinkle_scale_max = particles[i].twinkle_scale_max;
		particle->burst_multiplier = particles[i].burst_multiplier;
		particle->drag = particles[i].drag;
		particle->spin = particles[i].spin;
		particle->tumble = particles[i].tumble;
		particle->wind_vector = particles[i].wind_vector;
		particle->wind_time = particles[i].wind_time;
		particle->follow_speed1 = particles[i].follow_speed1;
		particle->follow_scale1 = particles[i].follow_scale1;
		particle->follow_speed2 = particles[i].follow_speed2;
		particle->follow_scale2 = particles[i].follow_scale2;
		particle->spline_points = array_read(sizeof(*particle->spline_points), &particle->spline_points_nb, &particles[i].spline_points, mpq);
		if (!particle->spline_points)
			goto err;
		if (!read_track(&particle->enabled_in, sizeof(uint8_t), &particles[i].enabled_in, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(particles);
	return ret;
}

static bool read_textures(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_texture_int_t *textures = array_read(sizeof(*textures), &file->textures_nb, &file->header.textures, mpq);
	if (!textures)
		goto err;
	file->textures = WOW_MALLOC(sizeof(*file->textures) * file->textures_nb);
	if (!file->textures)
		goto err;
	memset(file->textures, 0, sizeof(*file->textures) * file->textures_nb);
	for (uint32_t i = 0; i < file->textures_nb; ++i)
	{
		wow_m2_texture_t *texture = &file->textures[i];
		texture->type = textures[i].type;
		texture->flags = textures[i].flags;
		texture->filename = array_read(sizeof(*texture->filename), NULL, &textures[i].filename, mpq);
		if (!texture->filename)
			goto err;
	}
	ret = true;

err:
	WOW_FREE(textures);
	return ret;
}

static bool read_cameras(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_camera_int_t *cameras = array_read(sizeof(*cameras), &file->cameras_nb, &file->header.cameras, mpq);
	if (!cameras)
		goto err;
	file->cameras = WOW_MALLOC(sizeof(*file->cameras) * file->cameras_nb);
	if (!file->cameras)
		goto err;
	memset(file->cameras, 0, sizeof(*file->cameras) * file->cameras_nb);
	for (uint32_t i = 0; i < file->cameras_nb; ++i)
	{
		wow_m2_camera_t *camera = &file->cameras[i];
		camera->type = cameras[i].type;
		camera->fov = cameras[i].fov;
		camera->far_clip = cameras[i].far_clip;
		camera->near_clip = cameras[i].near_clip;
		camera->position_base = cameras[i].position_base;
		camera->target_position_base = cameras[i].target_position_base;
		if (!read_track(&camera->position, sizeof(wow_m2_spline_vec3f_t), &cameras[i].position, mpq)
		 || !read_track(&camera->target_position, sizeof(wow_m2_spline_vec3f_t), &cameras[i].target_position, mpq)
		 || !read_track(&camera->roll, sizeof(wow_m2_spline_float_t), &cameras[i].roll, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(cameras);
	return ret;
}

static bool read_colors(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_color_int_t *colors = array_read(sizeof(*colors), &file->colors_nb, &file->header.colors, mpq);
	if (!colors)
		goto err;
	file->colors = WOW_MALLOC(sizeof(*file->colors) * file->colors_nb);
	if (!file->colors)
		goto err;
	memset(file->colors, 0, sizeof(*file->colors) * file->colors_nb);
	for (uint32_t i = 0; i < file->colors_nb; ++i)
	{
		wow_m2_color_t *color = &file->colors[i];
		if (!read_track(&color->color, sizeof(wow_vec3f_t), &colors[i].color, mpq)
		 || !read_track(&color->alpha, sizeof(int16_t), &colors[i].alpha, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(colors);
	return ret;
}

static bool read_lights(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_light_int_t *lights = array_read(sizeof(*lights), &file->lights_nb, &file->header.lights, mpq);
	if (!lights)
		goto err;
	file->lights = WOW_MALLOC(sizeof(*file->lights) * file->lights_nb);
	if (!file->lights)
		goto err;
	memset(file->lights, 0, sizeof(*file->lights) * file->lights_nb);
	for (uint32_t i = 0; i < file->lights_nb; ++i)
	{
		wow_m2_light_t *light = &file->lights[i];
		light->type = lights[i].type;
		light->bone = lights[i].bone;
		light->position = lights[i].position;
		if (!read_track(&light->ambient_color, sizeof(wow_vec3f_t), &lights[i].ambient_color, mpq)
		 || !read_track(&light->ambient_intensity, sizeof(float), &lights[i].ambient_intensity, mpq)
		 || !read_track(&light->diffuse_color, sizeof(wow_vec3f_t), &lights[i].diffuse_color, mpq)
		 || !read_track(&light->diffuse_intensity, sizeof(float), &lights[i].diffuse_intensity, mpq)
		 || !read_track(&light->attenuation_start, sizeof(float), &lights[i].attenuation_start, mpq)
		 || !read_track(&light->attenuation_end, sizeof(float), &lights[i].attenuation_end, mpq)
		 || !read_track(&light->visibility, sizeof(uint8_t), &lights[i].visibility, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(lights);
	return ret;
}

static bool read_bones(wow_m2_file_t *file, wow_mpq_file_t *mpq)
{
	bool ret = false;
	wow_m2_bone_int_t *bones = array_read(sizeof(*bones), &file->bones_nb, &file->header.bones, mpq);
	if (!bones)
		goto err;
	file->bones = WOW_MALLOC(sizeof(*file->bones) * file->bones_nb);
	if (!file->bones)
		goto err;
	memset(file->bones, 0, sizeof(*file->bones) * file->bones_nb);
	for (uint32_t i = 0; i < file->bones_nb; ++i)
	{
		wow_m2_bone_t *bone = &file->bones[i];
		bone->key_bone_id = bones[i].key_bone_id;
		bone->flags = bones[i].flags;
		bone->parent_bone = bones[i].parent_bone;
		bone->submesh_id = bones[i].submesh_id;
		bone->bone_name_crc = bones[i].bone_name_crc;
		bone->pivot = bones[i].pivot;
		if (!read_track(&bone->translation, sizeof(wow_vec3f_t), &bones[i].translation, mpq)
		 || !read_track(&bone->rotation, sizeof(wow_quaternion_short_t), &bones[i].rotation, mpq)
		 || !read_track(&bone->scale, sizeof(wow_vec3f_t), &bones[i].scale, mpq))
			goto err;
	}
	ret = true;

err:
	WOW_FREE(bones);
	return ret;
}

wow_m2_file_t *wow_m2_file_new(wow_mpq_file_t *mpq)
{
	wow_m2_file_t *file = WOW_MALLOC(sizeof(*file));
	if (!file)
		return NULL;
	memset(file, 0, sizeof(*file));
	wow_mpq_read(mpq, &file->header, sizeof(file->header));
	file->materials = array_read(sizeof(*file->materials), &file->materials_nb, &file->header.materials, mpq);
	if (!file->materials)
		goto err;
	file->sequences = array_read(sizeof(*file->sequences), &file->sequences_nb, &file->header.sequences, mpq);
	if (!file->sequences)
		goto err;
	file->vertexes = array_read(sizeof(*file->vertexes), &file->vertexes_nb, &file->header.vertexes, mpq);
	if (!file->vertexes)
		goto err;
	file->texture_transforms_lookups = array_read(sizeof(*file->texture_transforms_lookups), &file->texture_transforms_lookups_nb, &file->header.texture_transforms_lookup_table, mpq);
	if (!file->texture_transforms_lookups)
		goto err;
	file->texture_weights_lookups = array_read(sizeof(*file->texture_weights_lookups), &file->texture_weights_lookups_nb, &file->header.texture_weights_lookup_table, mpq);
	if (!file->texture_weights_lookups)
		goto err;
	if (file->header.flags & WOW_M2_HEADER_FLAG_USE_TEXTURE_COMBINER_COMBO)
	{
		file->texture_combiner_combos = array_read(sizeof(*file->texture_combiner_combos), &file->texture_combiner_combos_nb, &file->header.texture_combiner_combos, mpq);
		if (!file->texture_combiner_combos)
			goto err;
	}
	file->texture_unit_lookups = array_read(sizeof(*file->texture_unit_lookups), &file->texture_unit_lookups_nb, &file->header.texture_unit_lookup_table, mpq);
	if (!file->texture_unit_lookups)
		goto err;
	file->attachment_lookups = array_read(sizeof(*file->attachment_lookups), &file->attachment_lookups_nb, &file->header.attachment_lookup_table, mpq);
	if (!file->attachment_lookups)
		goto err;
	file->key_bone_lookups = array_read(sizeof(*file->key_bone_lookups), &file->key_bone_lookups_nb, &file->header.key_bone_lookup_table, mpq);
	if (!file->key_bone_lookups)
		goto err;
	file->texture_lookups = array_read(sizeof(*file->texture_lookups), &file->texture_lookups_nb, &file->header.texture_lookup_table, mpq);
	if (!file->texture_lookups)
		goto err;
	file->bone_lookups = array_read(sizeof(*file->bone_lookups), &file->bone_lookups_nb, &file->header.bone_lookup_table, mpq);
	if (!file->bone_lookups)
		goto err;
	if (!read_texture_transforms(file, mpq))
		goto err;
	if (!read_texture_weights(file, mpq))
		goto err;
	if (!read_skin_profiles(file, mpq))
		goto err;
	if (!read_attachments(file, mpq))
		goto err;
	if (!read_particles(file, mpq))
		goto err;
	if (!read_textures(file, mpq))
		goto err;
	if (!read_cameras(file, mpq))
		goto err;
	if (!read_colors(file, mpq))
		goto err;
	if (!read_lights(file, mpq))
		goto err;
	if (!read_bones(file, mpq))
		goto err;
	file->name = array_read(sizeof(*file->name), NULL, &file->header.name, mpq);
	if (!file->name)
		goto err;
	return file;

err:
	wow_m2_file_delete(file);
	return NULL;
}

static bool track_dup(wow_m2_track_t *track, const wow_m2_track_t *dup, size_t data_size)
{
	track->type = dup->type;
	track->sequence = dup->sequence;
	track->interpolation_ranges_nb = dup->interpolation_ranges_nb;
	track->interpolation_ranges = WOW_MALLOC(sizeof(*track->interpolation_ranges) * track->interpolation_ranges_nb);
	if (!track->interpolation_ranges)
		return false;
	memcpy(track->interpolation_ranges, dup->interpolation_ranges, sizeof(*track->interpolation_ranges) * track->interpolation_ranges_nb);
	track->timestamps_nb = dup->timestamps_nb;
	track->timestamps = WOW_MALLOC(sizeof(*track->timestamps) * track->timestamps_nb);
	if (!track->timestamps)
		return false;
	memcpy(track->timestamps, dup->timestamps, sizeof(*track->timestamps) * track->timestamps_nb);
	track->values_nb = dup->values_nb;
	track->values = WOW_MALLOC(data_size * track->values_nb);
	if (!track->values)
		return false;
	memcpy(track->values, dup->values, data_size * track->values_nb);
	return true;
}

static void track_free(wow_m2_track_t *track)
{
	WOW_FREE(track->interpolation_ranges);
	WOW_FREE(track->timestamps);
	WOW_FREE(track->values);
}

void wow_m2_file_delete(wow_m2_file_t *file)
{
	if (!file)
		return;
	wow_m2_texture_transforms_delete(file->texture_transforms, file->texture_transforms_nb);
	wow_m2_texture_weights_delete(file->texture_weights, file->texture_weights_nb);
	for (uint32_t i = 0; i < file->skin_profiles_nb; ++i)
	{
		WOW_FREE(file->skin_profiles[i].sections);
		WOW_FREE(file->skin_profiles[i].batches);
		WOW_FREE(file->skin_profiles[i].vertexes);
		WOW_FREE(file->skin_profiles[i].indices);
		WOW_FREE(file->skin_profiles[i].bones);
	}
	WOW_FREE(file->skin_profiles);
	WOW_FREE(file->materials);
	wow_m2_attachments_delete(file->attachments, file->attachments_nb);
	wow_m2_sequences_delete(file->sequences, file->sequences_nb);
	wow_m2_particles_delete(file->particles, file->particles_nb);
	wow_m2_textures_delete(file->textures, file->textures_nb);
	WOW_FREE(file->vertexes);
	wow_m2_cameras_delete(file->cameras, file->cameras_nb);
	wow_m2_colors_delete(file->colors, file->colors_nb);
	wow_m2_lights_delete(file->lights, file->lights_nb);
	wow_m2_bones_delete(file->bones, file->bones_nb);
	WOW_FREE(file->texture_transforms_lookups);
	WOW_FREE(file->texture_weights_lookups);
	WOW_FREE(file->texture_combiner_combos);
	WOW_FREE(file->texture_unit_lookups);
	WOW_FREE(file->attachment_lookups);
	WOW_FREE(file->key_bone_lookups);
	WOW_FREE(file->texture_lookups);
	WOW_FREE(file->bone_lookups);
	WOW_FREE(file->name);
	WOW_FREE(file);
}

wow_m2_texture_transform_t *wow_m2_texture_transforms_dup(const wow_m2_texture_transform_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_texture_transform_t *texture_transforms = WOW_MALLOC(sizeof(*texture_transforms) * nb);
	if (!texture_transforms)
		return NULL;
	memset(texture_transforms, 0, sizeof(*texture_transforms) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_texture_transform_t *texture_transform = &texture_transforms[i];
		if (!track_dup(&texture_transform->translation, &dup[i].translation, sizeof(wow_vec3f_t))
		 || !track_dup(&texture_transform->rotation, &dup[i].rotation, sizeof(wow_quaternion_float_t))
		 || !track_dup(&texture_transform->scaling, &dup[i].scaling, sizeof(wow_vec3f_t)))
		{
			wow_m2_texture_transforms_delete(texture_transforms, i + 1);
			return NULL;
		}
	}
	return texture_transforms;
}

void wow_m2_texture_transforms_delete(wow_m2_texture_transform_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		track_free(&val[i].translation);
		track_free(&val[i].rotation);
		track_free(&val[i].scaling);
	}
	WOW_FREE(val);
}

wow_m2_texture_weight_t *wow_m2_texture_weights_dup(const wow_m2_texture_weight_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_texture_weight_t *texture_weights = WOW_MALLOC(sizeof(*texture_weights) * nb);
	if (!texture_weights)
		return NULL;
	memset(texture_weights, 0, sizeof(*texture_weights) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_texture_weight_t *texture_weight = &texture_weights[i];
		if (!track_dup(&texture_weight->weight, &dup[i].weight, sizeof(int16_t)))
		{
			wow_m2_texture_weights_delete(texture_weights, i + 1);
			return NULL;
		}
	}
	return texture_weights;
}

void wow_m2_texture_weights_delete(wow_m2_texture_weight_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
		track_free(&val[i].weight);
	WOW_FREE(val);
}

wow_m2_attachment_t *wow_m2_attachments_dup(const wow_m2_attachment_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_attachment_t *attachments = WOW_MALLOC(sizeof(*attachments) * nb);
	if (!attachments)
		return NULL;
	memset(attachments, 0, sizeof(*attachments));
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_attachment_t *attachment = &attachments[i];
		attachment->id = dup[i].id;
		attachment->bone = dup[i].bone;
		attachment->unknown = dup[i].unknown;
		attachment->position = dup[i].position;
		if (!track_dup(&attachment->animate_attached, &dup[i].animate_attached, sizeof(uint8_t)))
			goto err;
	}
	return attachments;

err:
	WOW_FREE(attachments);
	return NULL;
}

void wow_m2_attachments_delete(wow_m2_attachment_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
		track_free(&val[i].animate_attached);
	WOW_FREE(val);
}

wow_m2_sequence_t *wow_m2_sequences_dup(const wow_m2_sequence_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_sequence_t *sequences = WOW_MALLOC(sizeof(*sequences) * nb);
	if (!sequences)
		return NULL;
	memcpy(sequences, dup, sizeof(*sequences) * nb);
	return sequences;
}

void wow_m2_sequences_delete(wow_m2_sequence_t *val, uint32_t nb)
{
	(void)nb;
	if (!val)
		return;
	WOW_FREE(val);
}

wow_m2_particle_t *wow_m2_particles_dup(const wow_m2_particle_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_particle_t *particles = WOW_MALLOC(sizeof(*particles) * nb);
	if (!particles)
		return NULL;
	memset(particles, 0, sizeof(*particles) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_particle_t *particle = &particles[i];
		particle->id = dup[i].id;
		particle->flags = dup[i].flags;
		particle->position = dup[i].position;
		particle->bone = dup[i].bone;
		particle->texture = dup[i].texture;
		particle->geometry_model_filename = WOW_MALLOC(strlen(dup[i].geometry_model_filename) + 1);
		if (!particle->geometry_model_filename)
			goto err;
		strcpy(particle->geometry_model_filename, dup[i].geometry_model_filename);
		particle->recursion_model_filename = WOW_MALLOC(strlen(dup[i].recursion_model_filename) + 1);
		if (!particle->recursion_model_filename)
			goto err;
		strcpy(particle->recursion_model_filename, dup[i].recursion_model_filename);
		particle->blending_type = dup[i].blending_type;
		particle->emitter_type = dup[i].emitter_type;
		particle->particle_type = dup[i].particle_type;
		particle->head_or_tail = dup[i].head_or_tail;
		particle->texture_tile_rotation = dup[i].texture_tile_rotation;
		particle->texture_dimensions_rows = dup[i].texture_dimensions_rows;
		particle->texture_dimensions_columns = dup[i].texture_dimensions_columns;
		if (!track_dup(&particle->emission_speed, &dup[i].emission_speed, sizeof(float))
		 || !track_dup(&particle->speed_variation, &dup[i].speed_variation, sizeof(float))
		 || !track_dup(&particle->vertical_range, &dup[i].vertical_range, sizeof(float))
		 || !track_dup(&particle->horizontal_range, &dup[i].horizontal_range, sizeof(float))
		 || !track_dup(&particle->gravity, &dup[i].gravity, sizeof(float))
		 || !track_dup(&particle->lifespan, &dup[i].lifespan, sizeof(float))
		 || !track_dup(&particle->emission_rate, &dup[i].emission_rate, sizeof(float))
		 || !track_dup(&particle->emission_area_length, &dup[i].emission_area_length, sizeof(float))
		 || !track_dup(&particle->emission_area_width, &dup[i].emission_area_width, sizeof(float))
		 || !track_dup(&particle->z_source, &dup[i].z_source, sizeof(float)))
			goto err;
		particle->mid_point = dup[i].mid_point;
		memcpy(particle->color_values, dup[i].color_values, sizeof(particle->color_values));
		memcpy(particle->scale_values, dup[i].scale_values, sizeof(particle->scale_values));
		memcpy(particle->lifespan_uv_anim, dup[i].lifespan_uv_anim, sizeof(particle->lifespan_uv_anim));
		memcpy(particle->decay_uv_anim, dup[i].decay_uv_anim, sizeof(particle->decay_uv_anim));
		memcpy(particle->tail_uv_anim, dup[i].tail_uv_anim, sizeof(particle->tail_uv_anim));
		memcpy(particle->tail_decay_uv_anim, dup[i].tail_decay_uv_anim, sizeof(particle->tail_decay_uv_anim));
		particle->tail_length = dup[i].tail_length;
		particle->twinkle_speed = dup[i].twinkle_speed;
		particle->twinkle_percent = dup[i].twinkle_percent;
		particle->twinkle_scale_min = dup[i].twinkle_scale_min;
		particle->twinkle_scale_max = dup[i].twinkle_scale_max;
		particle->burst_multiplier = dup[i].burst_multiplier;
		particle->drag = dup[i].drag;
		particle->spin = dup[i].spin;
		particle->tumble = dup[i].tumble;
		particle->wind_vector = dup[i].wind_vector;
		particle->wind_time = dup[i].wind_time;
		particle->follow_speed1 = dup[i].follow_speed1;
		particle->follow_scale1 = dup[i].follow_scale1;
		particle->follow_speed2 = dup[i].follow_speed2;
		particle->follow_scale2 = dup[i].follow_scale2;
		particle->spline_points = WOW_MALLOC(sizeof(*particle->spline_points) * particle->spline_points_nb);
		if (!particle->spline_points)
			goto err;
		memcpy(particle->spline_points, dup[i].spline_points, sizeof(*particle->spline_points) * particle->spline_points_nb);
		particle->spline_points_nb = dup[i].spline_points_nb;
		if (!track_dup(&particle->enabled_in, &dup[i].enabled_in, sizeof(uint8_t)))
			goto err;
	}
	return particles;

err:
	WOW_FREE(particles);
	return NULL;
}

void wow_m2_particles_delete(wow_m2_particle_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		WOW_FREE(val[i].geometry_model_filename);
		WOW_FREE(val[i].recursion_model_filename);
		track_free(&val[i].emission_speed);
		track_free(&val[i].speed_variation);
		track_free(&val[i].vertical_range);
		track_free(&val[i].horizontal_range);
		track_free(&val[i].gravity);
		track_free(&val[i].lifespan);
		track_free(&val[i].emission_rate);
		track_free(&val[i].emission_area_length);
		track_free(&val[i].emission_area_width);
		track_free(&val[i].z_source);
		WOW_FREE(val[i].spline_points);
		track_free(&val[i].enabled_in);
	}
	WOW_FREE(val);
}

wow_m2_texture_t *wow_m2_textures_dup(const wow_m2_texture_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_texture_t *textures = WOW_MALLOC(sizeof(*textures) * nb);
	if (!textures)
		return NULL;
	memset(textures, 0, sizeof(*textures) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_texture_t *texture = &textures[i];
		texture->type = dup[i].type;
		texture->flags = dup[i].flags;
		if (dup[i].filename)
		{
			size_t len = strlen(dup[i].filename);
			texture->filename = WOW_MALLOC(len + 1);
			if (!texture->filename)
				goto err;
			memcpy(texture->filename, dup[i].filename, len);
			texture->filename[len] = '\0';
		}
	}
	return textures;

err:
	WOW_FREE(textures);
	return NULL;
}

void wow_m2_textures_delete(wow_m2_texture_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		WOW_FREE(val[i].filename);
	}
	WOW_FREE(val);
}

wow_m2_camera_t *wow_m2_cameras_dup(const wow_m2_camera_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_camera_t *cameras = WOW_MALLOC(sizeof(*cameras) * nb);
	if (!cameras)
		return NULL;
	memset(cameras, 0, sizeof(*cameras) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_camera_t *camera = &cameras[i];
		camera->type = dup[i].type;
		camera->fov = dup[i].fov;
		camera->far_clip = dup[i].far_clip;
		camera->near_clip = dup[i].near_clip;
		camera->position_base = dup[i].position_base;
		camera->target_position_base = dup[i].target_position_base;
		if (!track_dup(&camera->position, &dup[i].position, sizeof(wow_m2_spline_vec3f_t))
		 || !track_dup(&camera->target_position, &dup[i].target_position, sizeof(wow_m2_spline_vec3f_t))
		 || !track_dup(&camera->roll, &dup[i].roll, sizeof(wow_m2_spline_float_t)))
		{
			wow_m2_cameras_delete(cameras, i + 1);
			return NULL;
		}
	}
	return cameras;
}

void wow_m2_cameras_delete(wow_m2_camera_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		track_free(&val[i].position);
		track_free(&val[i].target_position);
		track_free(&val[i].roll);
	}
	WOW_FREE(val);
}

wow_m2_color_t *wow_m2_colors_dup(const wow_m2_color_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_color_t *colors = WOW_MALLOC(sizeof(*colors) * nb);
	if (!colors)
		return NULL;
	memset(colors, 0, sizeof(*colors) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_color_t *color = &colors[i];
		if (!track_dup(&color->color, &dup[i].color, sizeof(wow_vec3f_t))
		 || !track_dup(&color->alpha, &dup[i].alpha, sizeof(int16_t)))
		{
			wow_m2_colors_delete(colors, i + 1);
			return NULL;
		}
	}
	return colors;
}

void wow_m2_colors_delete(wow_m2_color_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		track_free(&val[i].color);
		track_free(&val[i].alpha);
	}
	WOW_FREE(val);
}

wow_m2_light_t *wow_m2_lights_dup(const wow_m2_light_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_light_t *lights = WOW_MALLOC(sizeof(*lights) * nb);
	if (!lights)
		return NULL;
	memset(lights, 0, sizeof(*lights) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_light_t *light = &lights[i];
		light->type = dup[i].type;
		light->bone = dup[i].bone;
		light->position = dup[i].position;
		if (!track_dup(&light->ambient_color, &dup[i].ambient_color, sizeof(wow_vec3f_t))
		 || !track_dup(&light->ambient_intensity, &dup[i].ambient_intensity, sizeof(float))
		 || !track_dup(&light->diffuse_color, &dup[i].diffuse_color, sizeof(wow_vec3f_t))
		 || !track_dup(&light->diffuse_intensity, &dup[i].diffuse_intensity, sizeof(float))
		 || !track_dup(&light->attenuation_start, &dup[i].attenuation_start, sizeof(float))
		 || !track_dup(&light->attenuation_end, &dup[i].attenuation_end, sizeof(float))
		 || !track_dup(&light->visibility, &dup[i].visibility, sizeof(uint8_t)))
		{
			wow_m2_lights_delete(lights, i + 1);
			return NULL;
		}
	}
	return lights;

}

void wow_m2_lights_delete(wow_m2_light_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		track_free(&val[i].ambient_color);
		track_free(&val[i].ambient_intensity);
		track_free(&val[i].diffuse_color);
		track_free(&val[i].diffuse_intensity);
		track_free(&val[i].attenuation_start);
		track_free(&val[i].attenuation_end);
		track_free(&val[i].visibility);
	}
	WOW_FREE(val);
}

wow_m2_bone_t *wow_m2_bones_dup(const wow_m2_bone_t *dup, uint32_t nb)
{
	if (!dup || !nb)
		return NULL;
	wow_m2_bone_t *bones = WOW_MALLOC(sizeof(*bones) * nb);
	if (!bones)
		return NULL;
	memset(bones, 0, sizeof(*bones) * nb);
	for (uint32_t i = 0; i < nb; ++i)
	{
		wow_m2_bone_t *bone = &bones[i];
		bone->key_bone_id = dup[i].key_bone_id;
		bone->flags = dup[i].flags;
		bone->parent_bone = dup[i].parent_bone;
		bone->submesh_id = dup[i].submesh_id;
		bone->bone_name_crc = dup[i].bone_name_crc;
		bone->pivot = dup[i].pivot;
		if (!track_dup(&bone->translation, &dup[i].translation, sizeof(wow_vec3f_t))
		 || !track_dup(&bone->rotation, &dup[i].rotation, sizeof(wow_quaternion_short_t))
		 || !track_dup(&bone->scale, &dup[i].scale, sizeof(wow_vec3f_t)))
		{
			wow_m2_bones_delete(bones, i + 1);
			return NULL;
		}
	}
	return bones;
}

void wow_m2_bones_delete(wow_m2_bone_t *val, uint32_t nb)
{
	if (!val)
		return;
	for (uint32_t i = 0; i < nb; ++i)
	{
		track_free(&val[i].translation);
		track_free(&val[i].rotation);
		track_free(&val[i].scale);
	}
	WOW_FREE(val);
}
