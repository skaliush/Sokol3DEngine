#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector.h"

typedef struct {
    vec3_t *vertices;
    face_t *faces;
    vec3_t rotation;
} mesh_t;

extern mesh_t mesh;

void load_obj_file_data(char *filename);

#endif
