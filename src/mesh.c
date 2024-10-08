#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0, 0, 0},
    .scale = {1, 1, 1},
    .translation = {0, 0, 0}
};

void load_obj_file_data(char *filename) {
    FILE *file;
    char line[1024];
    int vertices_count = 0;
    int faces_count = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("The file cannot be opened");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            float x, y, z;
            if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                vec3_t vertex = {.x = x, .y = y, .z = z};
                array_push(mesh.vertices, vertex);
                vertices_count++;
            } else {
                perror("Failed to parse the file");
                exit(EXIT_FAILURE);
            }
        } else if (line[0] == 'f') {
            int a, b, c;
            if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &a, &b, &c) == 3) {
                face_t face = {.a = a, .b = b, .c = c, .color = 0xFFAAFFFF};
                array_push(mesh.faces, face);
                faces_count++;
            } else {
                perror("Failed to parse the file");
                exit(EXIT_FAILURE);
            }
        }
    }
    fclose(file);
    printf("Vertices: %d, triangles: %d", vertices_count, faces_count);
}
