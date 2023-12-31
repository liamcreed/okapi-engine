#pragma once

#define MAX_ENTITIES 100
typedef u32 entity_t;

typedef struct
{
    const char *name;
    const char *type;
    bool active;
} data_component_s;

typedef struct
{
    vec3 position;
    vec3 scale;
    vec4 rotation;
} transform_component_s;


typedef struct
{
    u32 entity_count;
    data_component_s data_components[MAX_ENTITIES + 1];
    transform_component_s transform_components[MAX_ENTITIES + 1];
} ecs_scene_t;

void ecs_reset_entity_values(ecs_scene_t *ecs, entity_t e);
void ecs_init(ecs_scene_t *ecs);
void ecs_update(ecs_scene_t *ecs, window_t* window, renderer_t* renderer);
void ecs_exit(ecs_scene_t *ecs);

entity_t ecs_create_entity(ecs_scene_t *ecs, const char *name);
void ecs_delete_entity(ecs_scene_t *ecs, entity_t *entity);


