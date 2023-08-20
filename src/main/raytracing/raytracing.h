typedef struct
{
    vec3_t direction;
    vec3_t origin;
}ray_t;

vec4_t raytracer_per_pixel(vec2_t coord)
{
    ray_t ray =
    {
        .origin = {0,0,1},
        .direction = vec3_normalize((vec3_t) { coord.x, coord.y, -1.0f })
    };
    float r = 0.5f;

    float a = vec3_dot(ray.direction, ray.direction);
    float b = 2.0f * vec3_dot(ray.origin, ray.direction);
    float c = vec3_dot(ray.origin, ray.origin) - r * r;

    float D = b * b - 4.0f * a * c;
    if (D < 0.0f)
    {
        return (vec4_t) {1,1,1,1};
    }

    float two_a = (2.0f * a);
    float sqrt_d = sqrt(D);

    float t0 = (-b + sqrt_d) / two_a;
    float t = (-b - sqrt_d) / two_a;

    vec3_t hit_pos = vec3_add(ray.origin, vec3_scale(ray.direction, t));
    vec3_t normal = vec3_normalize(hit_pos);
    vec3_t light_dir = vec3_normalize((vec3_t) { sin(glfwGetTime()), 1, 1 });

    float d = f32_max(vec3_dot(normal, light_dir), 0.0);

    vec4_t sphere_color = { 1 * d,0 * d, 1 * d, 1};

    return sphere_color;

}


int main()
{
    window_t window =
    {
        .width = 800,
        .height = 600,
        .vsync = true,
        .title = "okapi",
        .cursor = true
    };
    window_create(&window);

    renderer_t renderer =
    {
        .window = &window,
        .clear_color = {1,1,1,1},
        .width = 800*2,
        .height = 600*2
    };
    renderer_create(&renderer);

    ray_t ray;

    //----------------------------------------//

    u8* texture_data = malloc(renderer.width * renderer.height * 4);

    texture_t screen_texture =
    {
        .data = texture_data,
        .width = renderer.width,
        .height = renderer.height,
        .channel_count = 4,
        .filter = false,
        .sRGB = false
    };

    texture_create(&screen_texture);

    while (!window.closed)
    {
        window_update(&window);

        //----------------------------------------//

        for (i32 y = 0; y < screen_texture.height; y++)
        {
            for (i32 x = 0; x < screen_texture.width; x++)
            {
                vec2_t coords = { (f32)x / (f32)screen_texture.width, (f32)y / (f32)screen_texture.height };
                coords.x = (coords.x * 2 - 1) * window.aspect;
                coords.y = coords.y * 2 - 1;
                vec4_t pixel = vec4_clamp(raytracer_per_pixel(coords), (vec4_t) { 0.0, 0.0, 0.0, 0.0 }, (vec4_t) { 1.0, 1.0, 1.0, 1.0 });


                texture_data[(x * 4) + (y * screen_texture.width * 4) + 0] = pixel.x * 255;
                texture_data[(x * 4) + (y * screen_texture.width * 4) + 1] = pixel.y * 255;
                texture_data[(x * 4) + (y * screen_texture.width * 4) + 2] = pixel.z * 255;
                texture_data[(x * 4) + (y * screen_texture.width * 4) + 3] = pixel.w * 255;
            }
        }

        texture_update_data(&screen_texture);

        //----------------------------------------//

        renderer.proj_mat = mat4_new(1);
        renderer.view_mat = mat4_new(1);

        renderer_start(&renderer);

        renderer_draw_quad(&renderer, &screen_texture, (vec4_t) { 1, 1, 1, 1 }, (vec3_t) { 0, 0, 0 }, (vec3_t) { 2, 2, 0 }, (vec4_t) { 0, 0, 0, 1 });

        renderer_end(&renderer);
    }

    texture_delete(&screen_texture);
    renderer_exit(&renderer);
    window_exit(&window);
}
