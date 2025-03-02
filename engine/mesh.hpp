#pragma once

#include "buffer/index_buffer.hpp"
#include "buffer/vertex_buffer.hpp"
#include "external/glm.hpp"


namespace Ely {

struct ElysiumDependencies;
struct Vertex;

class Mesh {
   private:
    ElysiumDependencies& deps;
    glm::mat4 model{1.f};

   public:
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;

    Mesh(ElysiumDependencies& deps, const char* path, const char* basePath = "");
    Mesh(ElysiumDependencies& deps, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    ~Mesh();

    void Move(const glm::vec3& axis);
    void Scale(const glm::vec3& axis);
    void Rotate(float radians, const glm::vec3& axis);
};

}   // namespace Ely