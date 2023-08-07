#include "api.h"
#include "ecs/ecs.h"

void ecs_init(ecs_scene_t *scene)
{
    scene->entity_count = 0;
    for (entity_t e = 0; e <= MAX_ENTITIES; e++)
    {
        ecs_reset_entity_values(scene, e);
    }
}

void ecs_reset_entity_values(ecs_scene_t *scene, entity_t e)
{
    scene->data_components[e] = (data_component_s){
        .name = NULL,
        .type = NULL,
        .layer = 0,
    };
    scene->transform_components[e] = (transform_component_s){
        .pos = (vec3_t){0.0f, 0.0f, 0.0f},
        .rot = (vec3_t){0.0f, 0.0f, 0.0f},
        .scale = (vec3_t){1.0f, 1.0f, 1.0f}};
}

entity_t ecs_create_entity(ecs_scene_t *scene, const char *name)
{
    u32 e = 0;

    if (scene->entity_count != MAX_ENTITIES)
    {
        scene->entity_count += 1;
        e = scene->entity_count;
    }
    else
    {
        for (entity_t i = 1; i <= MAX_ENTITIES + 1; i++)
        {
            if (scene->data_components[i].name == NULL)
                e = i;
        }
        if (e == 0)
        {
            printf(LOG_ERROR "[scene]: maximum amount of entities reached!\n");
        }
    }
    scene->data_components[e].name = name;
    return e;
}

void ecs_delete_entity(ecs_scene_t *scene, entity_t *entity)
{
    if (entity != 0)
    {
        ecs_reset_entity_values(scene, *entity);
        *entity = 0;
        scene->entity_count -= 1;
    }
}

void ecs_update(ecs_scene_t *scene, window_t *window, renderer_t *renderer)
{
    for (u32 i = 1; i <= scene->entity_count; i++)
    {
        if (scene->data_components[i].active)
        {
            printf("Name: %s\n", scene->data_components[i].name);
        }
    }
}

void ecs_exit(ecs_scene_t *scene)
{
    for (entity_t e = 0; e <= MAX_ENTITIES + 1; e++)
    {
        ecs_reset_entity_values(scene, e);
    }
}