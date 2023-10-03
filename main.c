#include <minimal_window.h>
#include "lib.h"

int main()
{
    int width = 640, height = 480;
    minimal_window_create_fixed_size_window(width, height);

    point_light_t light1;
    light1.power = 1.0f;
    light1.color = (vec3_t){1.0f, 0.0f, 0.0f};
    light1.position = (vec3_t){-1.0f, -1.0f, 0.0f};

    int frame = 0;

    // Main loop
    while (minimal_window_process_events())
    {
        float frame_float = (float)frame;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                float x = (float)i / (float)width;
                float y = (float)j / (float)height;
                // Transform to NDC and correct aspect ratio
                x = 2 * x - 1;
                y = 2 * y - 1;
                x *= (float)width / (float)height;
                ray_t ray;
                ray.direction = vec3_normalized((vec3_t){x, y, -1.0f});
                ray.origin = (vec3_t){0 + sinf(frame_float * .01), 0 + cosf(frame_float * .01), 0};
                sphere_t sphere = {{0, 0, -2}, 1.0f};

                float depth;
                vec3_t position, normal;
                if (ray_sphere_intersection_with_normal_and_position(ray, sphere, &depth, &normal, &position))
                {
                    lighting_t lighting = blinn_phong_shading(light1, position, normal, ray.direction, 20.0f);
                    //                    uint8_t depth_clamped = (uint8_t) clampf(depth * 255, 0, 255);
                    uint8_t r = (uint8_t)clampf(lighting.color.x * 255, 0, 255);
                    uint8_t g = (uint8_t)clampf(lighting.color.y * 255, 0, 255);
                    uint8_t b = (uint8_t)clampf(lighting.color.z * 255, 0, 255);
                    minimal_window_draw_pixel(i, j, r, g, b);
                }
                else
                {
                    minimal_window_draw_pixel(i, j, 0, 0, 0);
                }
            }
        }
        minimal_window_request_update();
        frame++;
    }

    return 0;
}
