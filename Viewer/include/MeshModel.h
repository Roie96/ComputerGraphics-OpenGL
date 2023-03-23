#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"
#include "MatrixTransform.h"
#include "Material.h"
#include <glad/glad.h>


struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class MeshModel : public MatrixTransform
{
public:
    MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords, const std::string& model_name = "");
    virtual ~MeshModel();
    const Face& GetFace(int index) const;
    int GetFacesCount() const;
    const std::string& GetModelName() const;
    const glm::vec3& GetVertices(int index) const;
    int GetVerticesSizes() const;
    const glm::vec3& get_model_color();
    void set_model_color(const glm::vec4& color_);
    glm::mat4 NormolizeModel();
    const glm::vec3& GetNormals(int index) const;
    void SetAxes(int i);
    bool GetAxes(int i) const;
    void ResetAxes();
    void SetModelBoundaries();
    float GetModelBoundaries(int i) const;
    const glm::vec3 Getcolor_faces(int i);
    void Color_faces();
    float getRandomColor();
    std::string GetMaterialName() { return material_name; }
    void SetMaterial(const std::string name, const Material& material);
    std::vector<glm::vec3>& GetLightColors() { return light_colors; }
    Material material;
    int canonicalProjection = 0;
    
    int mapTex = 0; // 0 = none, 1 = toon, 2 = Normal, 3 = Environment
    
    const std::vector<Vertex>& GetModelVertices();
    GLuint GetVAO() const;
    
private:
    std::vector<Face> faces;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> textureCoords;
    std::vector<Vertex> modelVertices;
    GLuint vbo;
    GLuint vao;
    std::string model_name;
    glm::vec3 color;

    
    
    std::string material_name;
    std::vector<glm::vec3> light_colors;
    float model_boundaries[6];
    std::vector<glm::vec3> color_faces;

    //local
    glm::mat4 local_matrix;
    glm::vec3 move_factor_local;
    glm::vec3 rotate_factor_local;
    float scale_factor_local;

    //world
    glm::mat4 world_matrix;
    glm::vec3 rotate_factor_world;
    glm::vec3 move_factor_world;
    float scale_factor_world;


    bool display_axes[10] = { false, false, false, false, false, false, false,
        
        //fill traingle with random color
        false,
        // draw model edges
        false,
        // grey scale
        false,};
    
};
