#pragma once
#include "vulkanModel.hpp"
#include "vulkanRenderer.hpp"
#include "glm/glm.hpp"

// û??CPP?ļ?
struct component {};

struct PointLightComponent : component
{
    glm::vec4 lightColor{ 1, 1, 1, 1 };
};

struct TransformComponent : component
{
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};
    
    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 mat4()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f}};
    }
    glm::mat3 normalMatrix()
    {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 invScale = 1.0f / scale;

        return glm::mat3{
            {
                invScale.x * (c1 * c3 + s1 * s2 * s3),
                invScale.x * (c2 * s3),
                invScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                invScale.y * (c3 * s1 * s2 - c1 * s3),
                invScale.y * (c2 * c3),
                invScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                invScale.z * (c2 * s1),
                invScale.z * (-s2),
                invScale.z * (c1 * c2),
            },
        };
    }
};

class vulkanGameobject
{
public:
    using id_t = unsigned int;
    
    // temp function
    static vulkanGameobject makePointLight(glm::vec4 col = glm::vec4(1), float rad = 0.1f)
    {
        vulkanGameobject result = createGameObject();
        result.color = col;
        result.transform.scale.x = rad;
        result.pointLight = std::make_unique<PointLightComponent>();
        result.pointLight->lightColor = col;
        return result;
    }

    static vulkanGameobject createGameObject()
    {
        static id_t currentId = 0;
        return vulkanGameobject{currentId++};
    }
    vulkanGameobject(const vulkanGameobject &) = delete;
    vulkanGameobject &operator=(const vulkanGameobject &) = delete;
    vulkanGameobject(vulkanGameobject &&) = default;
    vulkanGameobject &operator=(vulkanGameobject &&) = default;
    id_t getId() { return id; }
    std::shared_ptr<vulkanModel> model{};
    glm::vec3 color{};
    TransformComponent transform{};
    std::unique_ptr<PointLightComponent> pointLight = 0;
    vulkanGameobject(id_t objId) : id{objId} {}
    id_t id;
};