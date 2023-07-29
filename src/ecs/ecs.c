#include "api.h"
#include "ecs/ecs.h"

void ecs_init(ecs_t *ecs)
{
    ecs->entity_count = 0;
    for (entity_t e = 0; e <= MAX_ENTITIES; e++)
    {
        ecs_reset_entity_values(ecs, e);
    }
}

void ecs_reset_entity_values(ecs_t *ecs, entity_t e)
{
    ecs->data_components[e] = (data_component_s){
        .name = NULL,
        .type = NULL,
        .layer = 0,
    };
    ecs->transform_components[e] = (transform_component_s){
        .position = (vec3_t){0.0f, 0.0f, 0.0f},
        .scale = (vec3_t){1.0f, 1.0f, 1.0f}};
    ecs->sprite_components[e] = (sprite_component_s){
        .active = false,
        .tint_color = (vec4_t){1.0f, 1.0f, 1.0f, 1.0f},
        .sub_texture = NULL,
        .texture = NULL};
    ecs->camera_components[e] = (camera_component_s){
        .active = false,
        .ortho_size = 1.0f};
}

entity_t ecs_create_entity(ecs_t *ecs, const char *name)
{
    unsigned int e = 0;

    if (ecs->entity_count != MAX_ENTITIES)
    {
        ecs->entity_count += 1;
        e = ecs->entity_count;
    }
    else
    {
        for (entity_t i = 1; i <= MAX_ENTITIES + 1; i++)
        {
            if (ecs->data_components[i].name == NULL)
                e = i;
        }
        if (e == 0)
        {
            printf(LOG_ERROR "[ecs]: maximum amount of entities reached!\n");
        }
    }
    ecs->data_components[e].name = name;
    return e;
}

void ecs_delete_entity(ecs_t *ecs, entity_t *entity)
{
    if (entity != 0)
    {
        ecs_reset_entity_values(ecs, *entity);
        *entity = 0;
        ecs->entity_count -= 1;
    }
}

void ecs_update(ecs_t *ecs, window_t *window, renderer_t *renderer)
{
    for (unsigned int i = 1; i <= ecs->entity_count; i++)
    {
        if (ecs->camera_components[i].active)
        {
            float aspect = (float)window->size.x / (float)window->size.y;

            renderer->proj_mat = mat4_ortho_aspect(aspect, ecs->camera_components[i].ortho_size, -10.0f, 10.0f);
            renderer->view_mat = mat4_translate(mat4_new(1.0f), (vec3_t){-ecs->transform_components[i].position.x, -ecs->transform_components[i].position.y, 0});
        }

        if (ecs->sprite_components[i].active)
        {
            if (ecs->sprite_components[i].texture != NULL && ecs->sprite_components[i].sub_texture != NULL)
            {
                ecs->transform_components[i].position = (vec3_t){
                    round(ecs->transform_components[i].position.x * ecs->sprite_components[i].sub_texture->sub_tex_size.x) / ecs->sprite_components[i].sub_texture->sub_tex_size.x, 
                    round(ecs->transform_components[i].position.y * ecs->sprite_components[i].sub_texture->sub_tex_size.x) / ecs->sprite_components[i].sub_texture->sub_tex_size.x, 
                    ecs->transform_components[i].position.z};
                renderer_draw_sub_texture(
                    renderer,
                    ecs->sprite_components[i].texture,
                    ecs->sprite_components[i].sub_texture,
                    ecs->transform_components[i].position,
                    ecs->transform_components[i].scale,
                    ecs->sprite_components[i].tint_color);
            }
            else if (ecs->sprite_components[i].sub_texture == NULL)
            {
                ecs->transform_components[i].position = (vec3_t){
                    round(ecs->transform_components[i].position.x * ecs->sprite_components[i].texture->size.x) / ecs->sprite_components[i].texture->size.x, 
                    round(ecs->transform_components[i].position.y * ecs->sprite_components[i].texture->size.x) / ecs->sprite_components[i].texture->size.x, 
                    ecs->transform_components[i].position.z};
                    
                renderer_draw_quad(
                    renderer,
                    ecs->sprite_components[i].texture,
                    ecs->transform_components[i].position,
                    ecs->transform_components[i].scale,
                    ecs->sprite_components[i].tint_color);
            }
            
        }
    }
}

void ecs_exit(ecs_t *ecs)
{
    for (entity_t e = 0; e <= MAX_ENTITIES + 1; e++)
    {
        ecs_reset_entity_values(ecs, e);
    }
}