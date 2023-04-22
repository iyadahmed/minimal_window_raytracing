#pragma once

#include <math.h>
#include <stdint.h>

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

typedef struct
{
    vec3_t origin;
    vec3_t direction;
} ray_t;

typedef struct
{
    vec3_t center;
    float radius;
} sphere_t;

typedef struct
{
    vec3_t color;
    vec3_t specular;
} lighting_t;

typedef struct
{
    vec3_t position;
    vec3_t color;
    float power;
} point_light_t;

struct vec3_uint32_hash_table_node_t
{
    vec3_t key;
    uint32_t value;
    struct vec3_uint32_hash_table_node_t *next;
};

float vec3_length_squared(vec3_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(vec3_t v)
{
    return sqrtf(vec3_length_squared(v));
}

vec3_t vec3_normalized(vec3_t v)
{
    float l = vec3_length(v);
    return (vec3_t){v.x / l, v.y / l, v.z / l};
}

vec3_t vec3_scale(vec3_t v, float scale)
{
    return (vec3_t){v.x * scale, v.y * scale, v.z * scale};
}

vec3_t vec3_add(vec3_t a, vec3_t b)
{
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

float vec3_dot(vec3_t a, vec3_t b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float clampf(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

bool ray_sphere_intersection(ray_t r, sphere_t s, float *t)
{
    vec3_t oc = {r.origin.x - s.center.x, r.origin.y - s.center.y, r.origin.z - s.center.z};
    float a = r.direction.x * r.direction.x + r.direction.y * r.direction.y + r.direction.z * r.direction.z;
    float b = 2.0f * (oc.x * r.direction.x + oc.y * r.direction.y + oc.z * r.direction.z);
    float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - s.radius * s.radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        *t = (-b - sqrtf(discriminant)) / (2.0f * a);
        return true;
    }
}

bool ray_sphere_intersection_with_normal_and_position(ray_t r, sphere_t s, float *t, vec3_t *normal, vec3_t *position)
{
    if (ray_sphere_intersection(r, s, t))
    {
        *position = vec3_add(r.origin, vec3_scale(r.direction, *t));
        *normal = vec3_scale(vec3_sub(*position, s.center), 1.0f / s.radius);
        return true;
    }
    else
    {
        return false;
    }
}

lighting_t
blinn_phong_shading(point_light_t pl, vec3_t surface_position, vec3_t surface_normal, vec3_t view_direction,
                    float specular_hardness)
{
    if (pl.power < 0)
    {
        return (lighting_t){{0, 0, 0},
                            {0, 0, 0}};
    }

    lighting_t out;

    vec3_t light_direction = vec3_sub(pl.position, surface_position);
    float distance_squared = vec3_length_squared(light_direction);
    float distance = sqrtf(distance_squared);
    light_direction = vec3_scale(light_direction, 1.0f / distance);

    // Diffuse
    float NdotL = vec3_dot(surface_normal, light_direction);
    float intensity = clampf(NdotL, 0.0f, 1.0f);
    out.color = vec3_scale(pl.color, intensity * pl.power / distance_squared);

    // Specular
    vec3_t half_vector = vec3_normalized(vec3_add(light_direction, view_direction));
    float NdotH = vec3_dot(surface_normal, half_vector);
    float specular_intensity = powf(clampf(NdotH, 0.0f, 1.0f), specular_hardness);
    out.specular = vec3_scale(pl.color, specular_intensity * pl.power / distance_squared);

    return out;
}
