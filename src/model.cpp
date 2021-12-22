#include "model.h"

Model::Model(std::string path, int mode) {
  FILE * file = fopen(path.c_str(), "r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
    exit(-1);
  }

  std::vector<GLuint> vertexIndices, uvIndices, normalIndices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  while(1) {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF) break;

    if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      // printf("v: %f %f %f\n", vertex.x, vertex.y, vertex.z);
      temp_vertices.push_back(vertex);
      // vertices.push_back(Vertex{vertex, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f)});
    } else if (strcmp(lineHeader, "vt") == 0) {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      temp_uvs.push_back(uv);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      temp_normals.push_back(normal);
    } else if (strcmp( lineHeader, "f" ) == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

      if (mode == 0) {
        int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
        vertexIndex[0] = vertexIndex[0] - 1;
        vertexIndex[1] = vertexIndex[1] - 1;
        vertexIndex[2] = vertexIndex[2] - 1;
        if (matches != 3) {
          printf("File format unsupported in `f`: only %d matches\n", matches);
          exit(-1);
        }
        // TODO: calculate normal
      } else if (mode == 1) {
        int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
        vertexIndex[0] = vertexIndex[0] - 1;
        vertexIndex[1] = vertexIndex[1] - 1;
        vertexIndex[2] = vertexIndex[2] - 1;
        uvIndex[0] = uvIndex[0] - 1;
        uvIndex[1] = uvIndex[1] - 1;
        uvIndex[2] = uvIndex[2] - 1;
        if (matches != 6) {
          printf("File format unsupported in `f`: only %d matches\n", matches);
          exit(-1);
        }
        // TODO: calculate normal
      } else if (mode == 2) {
        int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
        vertexIndex[0] = vertexIndex[0] - 1;
        vertexIndex[1] = vertexIndex[1] - 1;
        vertexIndex[2] = vertexIndex[2] - 1;
        uvIndex[0] = uvIndex[0] - 1;
        uvIndex[1] = uvIndex[1] - 1;
        uvIndex[2] = uvIndex[2] - 1;
        normalIndex[0] = normalIndex[0] - 1;
        normalIndex[1] = normalIndex[1] - 1;
        normalIndex[2] = normalIndex[2] - 1;
        if (matches != 9) {
          printf("File format unsupported in `f`: only %d matches\n", matches);
          exit(-1);
        }
      } else if (mode == 3) {
        int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
        vertexIndex[0] = vertexIndex[0] - 1;
        vertexIndex[1] = vertexIndex[1] - 1;
        vertexIndex[2] = vertexIndex[2] - 1;
        normalIndex[0] = normalIndex[0] - 1;
        normalIndex[1] = normalIndex[1] - 1;
        normalIndex[2] = normalIndex[2] - 1;
        if (matches != 6) {
          printf("File format unsupported in `f`: only %d matches\n", matches);
          exit(-1);
        }
      } else {
        printf("Unknown mode %d\n", mode);
        exit(-1);
      }

      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices    .push_back(uvIndex[0]);
      uvIndices    .push_back(uvIndex[1]);
      uvIndices    .push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
  }

  for (GLuint i = 0; i < vertexIndices.size(); i++) {
    glm::vec3 vertex = temp_vertices[vertexIndices[i]];
    glm::vec3 normal;
    if (mode == 2 || mode == 3)normal = temp_normals[normalIndices[i]];
    else normal = glm::vec3(0.0f, 0.0f, 1.0f);
    Vertex v = Vertex{vertex, normal, glm::vec3(0.8f, 0.8f, 0.8f)};
    // TODO: uv
    vertices.push_back(v);
    indices.push_back(i);
  }
  printf("Loaded %lu vertice\n", vertices.size());
  printf("Loaded %lu/3 faces\n", indices.size());
};