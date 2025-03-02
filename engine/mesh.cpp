#include "mesh.hpp"

#include "dependencies.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <filesystem>

namespace Ely {

Mesh::Mesh(ElysiumDependencies& deps, const char* path, const char* basePath) : deps{deps} {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    namespace fs = std::filesystem;
    fs::path filePath(std::string(basePath) + path);
    std::string meshPath = fs::canonical(filePath).generic_string();

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.position = {attrib.vertices[3 * index.vertex_index + 0], attrib.vertices[3 * index.vertex_index + 1],
                               attrib.vertices[3 * index.vertex_index + 2]};

            /* vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                               1.0f - attrib.texcoords[2 * index.texcoord_index + 1]}; */

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    vertexBuffer = new VertexBuffer(deps, vertices);
    indexBuffer = new IndexBuffer(deps, indices);
}

Mesh::Mesh(ElysiumDependencies& deps, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : deps{deps} {
    vertexBuffer = new VertexBuffer(deps, vertices);
    indexBuffer = new IndexBuffer(deps, indices);
}

Mesh::~Mesh() {
    delete vertexBuffer;
    delete indexBuffer;
}

void Mesh::Move(const glm::vec3& axis) { model = glm::translate(model, axis); }
void Mesh::Scale(const glm::vec3& axis) { model = glm::scale(model, axis); }
void Mesh::Rotate(float radians, const glm::vec3& axis) { model = glm::rotate(model, radians, axis); }

}   // namespace Ely