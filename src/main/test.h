/*
#define ALIVE 1
#define DEAD 0
#define GRID_SIZE 200

    unsigned char texture_data[GRID_SIZE * GRID_SIZE * 4];
    for (int i = 0; i < GRID_SIZE * GRID_SIZE * 4; i += 4)
    {
        texture_data[i + 0] = 0;
        texture_data[i + 1] = 0;
        texture_data[i + 2] = 0;
        texture_data[i + 3] = 0;
    }
    texture_t texture = texture_create_from_data(texture_data, (vec2_t){GRID_SIZE, GRID_SIZE});

    typedef enum
    {
        none,
        sand,
        water

    } particle_type;
    typedef struct
    {
        particle_type type;
        float life_time;
        vec2_t velocity;
        vec4_t color;
        bool updated;
    } particle_t;

    particle_t particle_grid[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            particle_grid[i][j].type = none;
        }
    }



    -----------s



if (key_pressed(&window, KEY_SPACE))
        {
            for (int x = 0; x < GRID_SIZE; x++)
            {
                for (int y = 0; y < GRID_SIZE; y++)
                {
                    if (rand() % 100 == 0)
                    {
                        particle_grid[x][y].type = sand;
                    }
                }
            }
        }
        if (key_pressed(&window, KEY_ESCAPE))
        {
            for (int x = 0; x < GRID_SIZE; x++)
            {
                for (int y = 0; y < GRID_SIZE; y++)
                {
                    if (y > 0 && x > 0 && x < GRID_SIZE - 1)
                    {
                        if (rand() % 100 == 0)
                        {
                            particle_grid[x][y].type = none;
                        }
                    }
                }
            }
        }

        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int y = 0; y < GRID_SIZE; y++)
            {

                if (particle_grid[x][y].type == sand && y > 0)
                {
                    if (particle_grid[x][y - 1].type == none)
                    {
                        particle_grid[x][y - 1].type = sand;
                        particle_grid[x][y].type = none;
                    }
                    else if (particle_grid[x - 1][y - 1].type == none && x > 0)
                    {
                        particle_grid[x - 1][y - 1].type = sand;
                        particle_grid[x][y].type = none;
                    }
                    else if (particle_grid[x + 1][y - 1].type == none && x < GRID_SIZE - 1)
                    {
                        particle_grid[x + 1][y - 1].type = sand;
                        particle_grid[x][y].type = none;
                    }
                }
            }
        }

        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int y = 0; y < GRID_SIZE; y++)
            {
                if (particle_grid[x][y].type == none)
                    particle_grid[x][y].color = (vec4_t){1, 1, 1, 0};
                else if (particle_grid[x][y].type == sand)
                    particle_grid[x][y].color = (vec4_t){1, 1, 1, 1};
                else if (particle_grid[x][y].type == water)
                    particle_grid[x][y].color = (vec4_t){1, 1, 1, .8};

                texture_data[(x * 4) + (y * GRID_SIZE * 4) + 0] = particle_grid[x][y].color.x * 255;
                texture_data[(x * 4) + (y * GRID_SIZE * 4) + 1] = particle_grid[x][y].color.y * 255;
                texture_data[(x * 4) + (y * GRID_SIZE * 4) + 2] = particle_grid[x][y].color.z * 255;
                texture_data[(x * 4) + (y * GRID_SIZE * 4) + 3] = particle_grid[x][y].color.w * 255;
            }
        }

        if (mouse_button_pressed(&window, MOUSE_BUTTON_LEFT))
        {
            int pos_x = (int)(mouse_position(&window).x / (800 / GRID_SIZE));
            int pos_y = GRID_SIZE - (int)(mouse_position(&window).y / (800 / GRID_SIZE));

            if (pos_x < GRID_SIZE && pos_y < GRID_SIZE)
            {
                if (key_pressed(&window, KEY_LEFT_SHIFT))
                    particle_grid[pos_x][pos_y].type = water;
                else
                    particle_grid[pos_x][pos_y].type = sand;
            }
        }
        texture_update_data(&texture, texture_data, (vec2_t){GRID_SIZE, GRID_SIZE});



 */

/*
window_t window;
    window.size.x = 1000;
    window.size.y = 800;
    window.title = "Hello";
    window.vsync = 1;
    window_init(&window);

    renderer_t renderer;
    renderer.clear_color = (vec4_t){.4, .8, 1, 1};
    renderer_init(&renderer);

    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    shader_t shader = shader_create("./res/shaders/basic.vert", "./res/shaders/basic.frag");
    shader_bind(&shader);

    vertex_array_t va;
    va = vertex_array_create(vertices, sizeof(vertices), indices, sizeof(indices), 0);
    vertex_array_push_attribute(0, 3, 8 * sizeof(float), 0);
    vertex_array_push_attribute(1, 3, 8 * sizeof(float), 3 * sizeof(float));
    vertex_array_push_attribute(2, 2, 8 * sizeof(float), 6 * sizeof(float));
    vertex_array_bind(&va);

    vertex_array_unbind(&va);

    mat4_t model = mat4_new(1);
    model = mat4_scale(model,.3);

    mat4_t view = mat4_new(1);
    mat4_t proj = mat4_ortho_aspect(window.size.x / window.size.y, 1, -10.0f, 10.0f);

    unsigned char texture_data[16 * 16 * 4];
    for (int i = 0; i < 16 * 16 * 4; i += 4)
    {
        texture_data[i + 0] = 255;
        texture_data[i + 1] = rand() % 256;
        texture_data[i + 2] = rand() % 256;
        texture_data[i + 3] = 255;
    }

    texture_t texture = texture_create_from_file("./res/textures/hornet.png");

    while (!window.closed)
    {
        window_update(&window);
        renderer_clear(&renderer);

        texture_bind(&texture, 1);

        float speed;
        if (key_pressed(&window, KEY_LEFT_SHIFT))
        {
            speed = 0.1;
        }
        else
        {
            speed = 0.02;
        }
        if (key_pressed(&window, KEY_A))
        {
            model = mat4_translate(model, (vec3_t){-speed, 0, 0});
        }
        if (key_pressed(&window, KEY_D))
        {
            model = mat4_translate(model, (vec3_t){speed, 0, 0});
        }
        if (key_pressed(&window, KEY_SPACE))
        {
            model = mat4_translate(model, (vec3_t){0, speed, 0});
        }
        if (key_pressed(&window, KEY_LEFT_CONTROL))
        {
            model = mat4_translate(model, (vec3_t){0, -speed, 0});
        }
        if (key_pressed(&window, KEY_R))
        {
            model = mat4_rotate_z(model, 1);
        }

        shader_bind(&shader);
        vertex_array_bind(&va);

        shader_set_uniform_mat4(&shader, "u_model", model);
        shader_set_uniform_mat4(&shader, "u_view", view);
        shader_set_uniform_mat4(&shader, "u_proj", proj);
        shader_set_uniform_int(&shader, "u_texture", 1);


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    renderer_exit(&renderer);
    window_exit(&window); */

/* FPS camera


float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    // make sure the user stays at the ground level
    Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)


*/