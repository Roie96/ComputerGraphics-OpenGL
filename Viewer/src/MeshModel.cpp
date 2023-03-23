#include "MeshModel.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <random>


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords ,const std::string& model_name) :
    faces(faces),
    vertices(vertices),
    normals(normals)
{
    int i = model_name.find_last_of('.');
    this->model_name = model_name.substr(0, i);
    this->color = glm::vec3(0.f, 0.f, 0.f);

    glm::mat4 scaleMat = NormolizeModel();

    auto faces_count = faces.size();
    for (size_t i = 0; i < faces_count; i++)
        light_colors.push_back(glm::vec3(getRandomColor(), getRandomColor(), getRandomColor()));

    material = {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f};
    modelVertices.reserve(3 * faces.size());
    for (int i = 0; i < faces.size(); i++)
    {
        Face currentFace = faces.at(i);
        for (int j = 0; j < 3; j++){
            int vertexIndex = currentFace.GetVertexIndex(j) - 1;
            
            Vertex vertex;
            glm::vec4 v(vertices[vertexIndex], 1.f);
            v = scaleMat * v;
//            v = v/v.w;
            vertex.position = v;
            vertex.normal = normals[vertexIndex];

            if (textureCoords.size() > 0)
            {
                int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
                vertex.textureCoords = textureCoords[textureCoordsIndex];
            }
            
            modelVertices.push_back(vertex);
        }
        // Computing the tangents and bitangents
        
//        glm::vec3&  v0 = modelVertices[modelVertices.size()-2].position;
//        glm::vec3&  v1 = modelVertices[modelVertices.size()-1].position;
//        glm::vec3&  v2 = modelVertices[modelVertices.size()].position;
//
//        glm::vec2&  uv0 = modelVertices[modelVertices.size()-2].textureCoords;
//        glm::vec2&  uv1 = modelVertices[modelVertices.size()-1].textureCoords;
//        glm::vec2&  uv2 = modelVertices[modelVertices.size()].textureCoords;
//
//        // Calculate the edges of the triangle
//        glm::vec3 edge1= v1-v0;
//        glm::vec3 edge2 = v2-v0;
//
//        // Calculate the difference in texture coordinates along the edges
//        glm::vec2 deltaUV1 = uv1-uv0;
//        glm::vec2 deltaUV2 = uv2-uv0;
//
//        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
//        glm::vec3 tangent = (edge1 * deltaUV2.y   - edge2 * deltaUV1.y)*r;
//        glm::vec3 bitangent = (edge2 * deltaUV1.x   - edge1 * deltaUV2.x)*r;
//
//        // Set the same tangent for all three vertices of the triangle.
//        modelVertices[modelVertices.size()-2].tangent = tangent;
//        modelVertices[modelVertices.size()-1].tangent = tangent;
//        modelVertices[modelVertices.size()-0].tangent = tangent;
//
//        // Same thing for bitangents
//        modelVertices[modelVertices.size()-2].bitangent = bitangent;
//        modelVertices[modelVertices.size()-1].bitangent = bitangent;
//        modelVertices[modelVertices.size()-0].bitangent = bitangent;
//    
    }
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    // Vertex Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // Vertex Tangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(12 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    
    
    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}


GLuint MeshModel::GetVAO() const
{
    return vao;
}

const std::vector<Vertex>& MeshModel::GetModelVertices()
{
    return modelVertices;
}


MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
    return faces[index];
}

int MeshModel::GetFacesCount() const
{
    return faces.size();
}

const std::string& MeshModel::GetModelName() const
{
    return model_name;
}

const glm::vec3& MeshModel::GetVertices(int index) const
{
    return vertices[index];
}

const glm::vec3& MeshModel::GetNormals(int index) const
{
    return normals[index];
}


int MeshModel::GetVerticesSizes() const
{
    return vertices.size();
}


void MeshModel::set_model_color(const glm::vec4& color_) {
    this->color = color_;
}
const glm::vec3& MeshModel::get_model_color() {
    return color;
}

glm::mat4 MeshModel::NormolizeModel(){
    float minX, maxX,  minY, maxY, maxZ, minZ;
    minX = vertices[0].x;
    maxX = vertices[0].x;
    minY = vertices[0].y;
    maxY = vertices[0].y;
    minZ = vertices[0].z;
    maxZ = vertices[0].z;
    for(int i = 1; i < vertices.size(); i++){
        minX = std::min(minX, vertices[i].x);
        minY = std::min(minY, vertices[i].y);
        minZ = std::min(minY, vertices[i].z);
        maxX = std::max(maxX, vertices[i].x);
        maxY = std::max(maxY, vertices[i].y);
        maxZ = std::max(maxY, vertices[i].z);
    }
    float size = std::max(maxX-minX, maxY-minY);
    size = std::max(size, maxZ-minZ);
    float scale_factor = 100.f/size;
    return glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor,scale_factor,scale_factor));
}

void MeshModel::SetAxes(int i)
{
    if (display_axes[i])
    {
        display_axes[i] = false;
    }
    else
    {
        display_axes[i] = true;
    }
}

bool MeshModel::GetAxes(int i) const
{
    return display_axes[i];
}

void MeshModel::ResetAxes()
{
    for (int i = 0; i < 7; i++)
    {
        display_axes[i] = false;
    }
}

void MeshModel::SetModelBoundaries()
{
    float max_x;
    float max_y;
    float max_z;
    float min_x = max_x = GetVertices(0).x;
    float min_y = max_y = GetVertices(0).y;
    float min_z = max_z = GetVertices(0).z;

    //Get min and max bounding box values 
    for (int i = 0; i < GetVerticesSizes(); i++)
    {
        if (GetVertices(i).x < min_x)
        {
            min_x = GetVertices(i).x;
        }
        if (GetVertices(i).x > max_x)
        {
            max_x = GetVertices(i).x;
        }
        if (GetVertices(i).y < min_y)
        {
            min_y = GetVertices(i).y;
        }
        if (GetVertices(i).y > max_y)
        {
            max_y = GetVertices(i).y;
        }
        if (GetVertices(i).z < min_z)
        {
            min_z = GetVertices(i).z;
        }
        if (GetVertices(i).z > max_z)
        {
            max_z = GetVertices(i).z;
        }
    }
    model_boundaries[0] = min_x;
    model_boundaries[1] = max_x;
    model_boundaries[2] = min_y;
    model_boundaries[3] = max_y;
    model_boundaries[4] = min_z;
    model_boundaries[5] = max_z;
}

float MeshModel::GetModelBoundaries(int i) const 
{
    return model_boundaries[i];
}

void MeshModel::Color_faces(){
    //set random color
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    int count = faces.size();
    for(int i = 1; i < count; i++){
        color_faces.push_back({dis(gen), dis(gen), dis(gen)});
    }
}


const glm::vec3 MeshModel::Getcolor_faces(int i)
{
    return color_faces[i];
}


void MeshModel::SetMaterial(const std::string name, const Material& material)
{
    material_name = name;
    this->material.ambient = material.ambient;
    this->material.diffuse = material.diffuse;
    this->material.specular = material.specular;
    this->material.shininess = material.shininess;
}

float MeshModel::getRandomColor()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


