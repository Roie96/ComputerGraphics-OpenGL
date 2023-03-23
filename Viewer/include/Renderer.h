#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Material.h"
#include "Triangle.h"
#include "Texture2D.h"
#include "ShaderProgram.h"

class Renderer
{
public:
    
    void LoadShaders();
    void LoadTextures(string name);
    Renderer(int viewportWidth, int viewportHeight);
    Renderer();

    virtual ~Renderer();
    void Render(const Scene& scene);
//    void SwapBuffers();
//    void ClearColorBuffer(const glm::vec3& color);
    int GetViewportWidth() const;
    int GetViewportHeight() const;
//    void DrawCircle();
//    void PapperPlane();
//    void setDirections(float& delta, float& way);
//    void CenterPoint (glm::vec3& v);
    void SetViewportWidth(const int _viewport_width);
    void SetViewportHeight(const int _viewport_height);
//    void ColorBufferUpdate();
//    void InitZBuffer(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
//    void ClearZBuffer();
   
 
    //1c features
//    void DrawModleAxes(glm::mat4 transforedMat);
//    void DrawWorldAxes(glm::mat4 transforedMat, glm::mat4 transformation);
//    void DrawFacesNormals(MeshModel& model, Face face, glm::mat4 transforedMat);
//    void DrawVertexNormals(MeshModel& model, int i, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
//    void DrawModelBoundingBox(MeshModel& model, glm::mat4 transforedMat);
//    void DrawWorldBoundingBox(MeshModel& model, glm::mat4 transforedMat, int half_width, int half_height);
//    //2a features
//    void DrawFacesReactangles(MeshModel& model, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
//    void ShowZbuffer();
//
//    //2b features
//    void FillTriangleWithLight(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Light& light, const Scene& scene, MeshModel& model, const std::vector<glm::vec3> vertexNormals);
//    glm::vec3 interpolate(const glm::vec3& color1, const glm::vec3& color2, const glm::vec3& color3, const glm::vec3& barycentric);
//    glm::vec3 calculateNormal(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3);
//    glm::vec3 calculateColor(glm::vec3 normal, glm::vec3 lightDirection, glm::vec3 viewDirection,Material& material,
//        glm::vec3 ambientColor,
//        glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess);
//    void drawReflectionVector(glm::vec3 lightDirection, glm::vec3 normal, glm::vec3 center, glm::vec3 color);
//    std::vector<float> computeReflectionVector(glm::vec3 lightDirection, glm::vec3 normal);
//    glm::vec3 getTrianglesNormal(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3,
//        glm::vec2& point, const glm::vec3& normal1,const glm::vec3& normal2,const glm::vec3& normal3);
//    float getTrianglesArea(glm::vec3& p1, glm::vec3& p2, glm::vec2& p3);
//    
//    
//    
//private:
//    
//    void PutPixel(const int i, const int j, const glm::vec3& color);
//    void DrawLine( const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
//    void DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color);
//    
//    void CreateBuffers(int w, int h);
//    void CreateOpenglBuffer();
//    void InitOpenglRendering();
//    void FillTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, const glm::vec3& color_triangle);
//    void GetLineE(glm::vec3 p1, glm::vec3 p2, vector<float>& e);
//    void SwapVec(glm::vec3& p1, glm::vec3& p2);
//    void PutZ(int i, int j, const float max);
//    bool ComperAndSwap(int i, int j, float pz);
//    float CalculateZ(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y);
//    float CalculateArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec2& p3);
//    float* color_buffer;
//    float* z_buffer;
//    float maxZ = FLT_MIN;
//    float minZ = FLT_MAX;
    
    ShaderProgram lightShader;
    ShaderProgram colorShader;
    Texture2D texture1;
    Texture2D diffuseMap;
    Texture2D normalMap;
    
    int viewport_width;
    int viewport_height;
    GLuint gl_screen_tex;
    GLuint gl_screen_vtc;

};
