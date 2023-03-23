#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "iostream"

#include "Renderer.h"
#include "InitShader.h"


#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))


Renderer::Renderer(int viewport_width, int viewport_height) :
    viewport_width(viewport_width),
    viewport_height(viewport_height)
{
    LoadShaders();
}

Renderer::~Renderer()
{
//    delete[] color_buffer;
//    delete[] z_buffer;
}


void Renderer::Render(const Scene& scene)
{
    Camera& camera = scene.GetActiveCamera();
    float half_width = viewport_width / 2.0f;
    float half_height = viewport_height / 2.0f;
    float DollyZoom;
    for(int i=0 ; i < scene.GetModelCount(); i++){
        MeshModel& model = scene.GetModel(i);
        
        LoadTextures(model.GetModelName());

        // Activate the 'colorShader' program (vertex and fragment shaders)
        colorShader.use();
    
    
        glm::mat4 model1 = model.get_mat_transformation();
        glm::mat4 modelview = camera.GetCameraTransformation();
        glm::mat4 modelpro = camera.GetProjectionTransformation();
        
        // Set the uniform variables
        colorShader.setUniform("model", model.get_mat_transformation());
        colorShader.setUniform("view", camera.GetCameraTransformation());
        colorShader.setUniform("projection", camera.GetProjectionTransformation());
        colorShader.setUniform("material.textureMap", 0);
        
        colorShader.setInt("diffuseMap", 0);
        colorShader.setInt("normalMap", 1);
        
        //Set  material properties
        colorShader.setUniform("material.ambient", glm::vec3(model.material.ambient));
        colorShader.setUniform("material.diffuse", glm::vec3(model.material.diffuse));
        colorShader.setUniform("material.specular", glm::vec3(model.material.specular));
        colorShader.setUniform("material.shininess", model.material.shininess);
        
        colorShader.setUniform("cameraPos", camera.GetCameraPosition());
        
        colorShader.setUniform("canonicalProject", model.canonicalProjection);
        
        colorShader.setUniform("mapTex", model.mapTex);


        // Set up the light sources
        colorShader.setUniform("num_lights", scene.GetLightCount());
        for (int i = 0; i < scene.GetLightCount(); i++){
            Light& light = scene.GetLightAtIndex(i);
            std::string lightName = "lights[" + std::to_string(i) + "].";
            colorShader.setUniform((lightName+"ambient").c_str(), glm::vec3(light.ambient));
            colorShader.setUniform((lightName+"diffuse").c_str(), glm::vec3(light.diffuse));
            colorShader.setUniform((lightName+"specular").c_str(), glm::vec3(light.specular));
            colorShader.setUniform((lightName+"position").c_str(), glm::vec3(light.GetPosition()));
        }
        
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 1);
        
        
        // Set 'texture1' as the active texture at slot #0
        texture1.bind(0);
     

        // Drag our model's faces (triangles) in fill mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
        // Unset 'texture1' as the active texture at slot #0
        texture1.unbind(0);
    
  
    }
}

void Renderer::LoadShaders()
{
    colorShader.loadShaders("vshader.glsl", "fshader.glsl");
}

void Renderer::LoadTextures(string name)
{
    string path = "/Users/roieshahar/Desktop/grahics_new/computer-graphics-2023-roie-yuval/Data/" + name + ".jpg";   //texture1.loadTexture("/Users/roieshahar/Desktop/grahics_new/computer-graphics-2023-roie-yuval/Data/spot_texture.png", true);
    //texture1.loadTexture("/Users/roieshahar/Desktop/grahics_new/computer-graphics-2023-roie-yuval/Data/Chess_Board.png", true);
    
//    //texture1.loadTexture(path, true);
    texture1.loadTexture("/Users/roieshahar/Desktop/grahics_new/computer-graphics-2023-roie-yuval/Data/normal.jpeg", true);
   texture1.loadTexture("/Users/roieshahar/Desktop/grahics_new/computer-graphics-2023-roie-yuval/Data/diffuse.jpeg", true);

}

int Renderer::GetViewportWidth() const
{
    return viewport_width;
}

int Renderer::GetViewportHeight() const
{
    return viewport_height;
}

void Renderer::SetViewportWidth(const int _viewport_width)
{
    this->viewport_width = _viewport_width;
}

void Renderer::SetViewportHeight(const int _viewport_height)
{
    this->viewport_height = _viewport_height;
}

//Renderer::Renderer()
//{
//    LoadShaders();
//    LoadTextures();
////    InitOpenglRendering();
////    CreateBuffers(viewport_width, viewport_height);
//}

//
//void Renderer::PutPixel(int i, int j, const glm::vec3& color)
//{
//    if (i < 0) return; if (i >= viewport_width) return;
//    if (j < 0) return; if (j >= viewport_height) return;
//
//    color_buffer[INDEX(viewport_width, i, j, 0)] = color.x;
//    color_buffer[INDEX(viewport_width, i, j, 1)] = color.y;
//    color_buffer[INDEX(viewport_width, i, j, 2)] = color.z;
//}
//
//
//void Renderer::PutZ(int i, int j, const float z)
//{
//    if (i < 0) return; if (i >= viewport_width) return;
//    if (j < 0) return; if (j >= viewport_height) return;
//
//    z_buffer[Z_INDEX(viewport_width, i, j)] = z;
//}
//
//float Renderer::CalculateArea(const glm::vec3& p1, const glm::vec3& p2, const glm::vec2& p3){
//    float a = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y)+ p3.x * (p1.y - p2.y);
//    if (a > 0)
//        return 0.5 * a;
//    else
//        return -0.5 * a;
//}
//
//float Renderer::CalculateZ(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, int x, int y)
//{
//    float vec23 = CalculateArea(p2, p3, {x,y});
//    float vec13 = CalculateArea(p1, p3, {x,y});
//    float vec21 = CalculateArea(p2, p1, {x,y});
//    float a = vec23 + vec13 + vec21;
//    return p1.z * (vec23 / a) + p2.z * (vec13 / a) + p3.z* (vec21 / a);
//
////    float distanse, pixelDis, pro, slope;
////    int delx, dely;
////    if (p1.y > p2.y) SwapVec(p1, p2); // p2 will have the y max
////
////
////    if (p1.x == p2.x && p1.y == p2.y) // the vertcies are at the same pixel, return the max z coordinate
////        return std::max(p1.z,p2.z);
////
////    delx = p2.x - p1.x;
////    dely = p2.y - p1.y;
////
////    if (delx == 0){   //will run on y axis
////        distanse = p2.y - p1.y;
////        pixelDis = p2.y - p3.y;
////    }
////    else{
////        slope = (float)dely / delx;
////        if (slope < 1 && slope > -1){
////            if (p1.x > p2.x)  SwapVec(p1, p2);  // p2 will have the x max
////            distanse = p2.x - p1.x;               //will run on x axis
////            pixelDis = p2.x - p3.x;
////        }
////        else{
////            distanse = p2.y - p1.y;  //will run on y axis
////            pixelDis = p2.y - p3.y;
////        }
////        }
////    pro = (float)pixelDis/distanse;
////    return (float)(((1 - pro) * p2.z) + (pro * p1.z)); //Expected value
//}
//
//bool Renderer::ComperAndSwap(int i, int j, float pz)
//{
//    if (i < 0) return false; if (i >= viewport_width) return false;
//    if (j < 0) return false; if (j >= viewport_height) return false;
//
//    float z = z_buffer[Z_INDEX(viewport_width, i, j)];
//    if (pz < z){
//        z_buffer[Z_INDEX(viewport_width, i, j)] = pz;
//        maxZ = glm::max(pz, maxZ);
//        minZ = glm::min(pz, minZ);
//        return true;
//    }
//    return false;
//}
//
//
//void Renderer::setDirections(float& delta, float& way){
//    if(delta > 0)
//        way = 1;
//    else if (delta < 0){
//        way = -1;
//        delta = -delta;
//    }
//    else
//        way = 0;
//}
//
//void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
//{
//    // TODO: Implement bresenham algorithm
//    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
//    float delx = p2.x - p1.x;
//    float dely = p2.y - p1.y;
//    float switchFlag = 0; // Decide if needs to switch x and y
//    float x_way, y_way;
//
//    if (dely == 0 && delx == 0){
////        if(ComperAndSwap((int)p1.x, (int)p1.y, z))
//            PutPixel((int)p1.x, (int)p1.y, color);
//        return;
//    }
//
//    // Decide from where to start
//    //line goes from left to right <-- or line goes from right to left -->
//    setDirections(delx, x_way);
//    setDirections(dely, y_way);
//
//    if (dely > delx){
//        swap(delx, dely);
//        switchFlag = 1;
//    }
//
//    float di = 2*(dely - delx);
//    float xn = p1.x, yn = p1.y;
//
//    for (int i = 0; i < delx; i++) {
////        if(ComperAndSwap((int)xn, (int)yn, z))
//            PutPixel((int)xn, (int)yn, color);
//        if (di > 0) {
//            di = di -(2*delx);
//            xn = xn + x_way;
//            yn = yn + y_way;
//        }
//        else {
//            if (switchFlag){
//                yn = yn + y_way;
//            }
//            else
//                xn = xn + x_way;
//        }
//        di = di + 2*dely;
//    }
//}
//
//
//void Renderer::CreateBuffers(int w, int h)
//{
//    CreateOpenglBuffer(); //Do not remove this line.
//    //create z buffer;
//    z_buffer = new float[w * h];
//
////    InitZBuffer();
//    color_buffer = new float[3 * w * h];
//    ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
//
//}

////##############################
////##OpenGL stuff. Don't touch.##
////##############################
//
//// Basic tutorial on how opengl works:
//// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
//// don't linger here for now, we will have a few tutorials about opengl later.
//void Renderer::InitOpenglRendering()
//{
//    // Creates a unique identifier for an opengl texture.
//    glGenTextures(1, &gl_screen_tex);
//
//    // Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
//    glGenVertexArrays(1, &gl_screen_vtc);
//
//    GLuint buffer;
//
//    // Makes this VAO the current one.
//    glBindVertexArray(gl_screen_vtc);
//
//    // Creates a unique identifier for a buffer.
//    glGenBuffers(1, &buffer);
//
//    // (-1, 1)____(1, 1)
//    //         |\  |
//    //         | \ | <--- The exture is drawn over two triangles that stretch over the screen.
//    //         |__\|
//    // (-1,-1)    (1,-1)
//    const GLfloat vtc[]={
//        -1, -1,
//         1, -1,
//        -1,  1,
//        -1,  1,
//         1, -1,
//         1,  1
//    };
//
//    const GLfloat tex[]={
//        0,0,
//        1,0,
//        0,1,
//        0,1,
//        1,0,
//        1,1};
//
//    // Makes this buffer the current one.
//    glBindBuffer(GL_ARRAY_BUFFER, buffer);
//
//    // This is the opengl way for doing malloc on the gpu.
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
//
//    // memcopy vtc to buffer[0,sizeof(vtc)-1]
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
//
//    // memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
//    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
//
//    // Loads and compiles a sheder.
//    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
//
//    // Make this program the current one.
//    glUseProgram(program);
//
//    // Tells the shader where to look for the vertex position data, and the data dimensions.
//    GLint  vPosition = glGetAttribLocation( program, "vPosition" );
//    glEnableVertexAttribArray( vPosition );
//    glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );
//
//    // Same for texture coordinates data.
//    GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
//    glEnableVertexAttribArray( vTexCoord );
//    glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );
//
//    //glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
//
//    // Tells the shader to use GL_TEXTURE0 as the texture id.
//    glUniform1i(glGetUniformLocation(program, "texture"),0);
//}
//
//void Renderer::CreateOpenglBuffer()
//{
//    // Makes GL_TEXTURE0 the current active texture unit
//    glActiveTexture(GL_TEXTURE0);
//
//    // Makes glScreenTex (which was allocated earlier) the current texture.
//    glBindTexture(GL_TEXTURE_2D, gl_screen_tex);
//
//    // malloc for a texture on the gpu.
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width, viewport_height, 0, GL_RGB, GL_FLOAT, NULL);
//    (0, 0, viewport_width, viewport_height);
//}
//
//void Renderer::SwapBuffers()
//{
//    // Makes GL_TEXTURE0 the current active texture unit
//    glActiveTexture(GL_TEXTURE0);
//
//    // Makes glScreenTex (which was allocated earlier) the current texture.
//    glBindTexture(GL_TEXTURE_2D, gl_screen_tex);
//
//    // memcopy's colorBuffer into the gpu.
//    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width, viewport_height, GL_RGB, GL_FLOAT, color_buffer);
//
//    // Tells opengl to use mipmapping
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // Make glScreenVtc current VAO
//    glBindVertexArray(gl_screen_vtc);
//
//    // Finally renders the data.
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//}
//
//void Renderer::ClearColorBuffer(const glm::vec3& color)
//{
//    for (int i = 0; i < viewport_width; i++)
//    {
//        for (int j = 0; j < viewport_height; j++)
//        {
//            PutPixel(i, j, color);
//            PutZ(i, j, FLT_MAX);
//        }
//    }
//    maxZ = FLT_MIN;
////    ClearZBuffer();
//}


//void Renderer::ClearZBuffer()
//{
//    for (int i = 0; i < viewport_width; i++)
//    {
//        for (int j = 0; j < viewport_height; j++)
//        {
//            PutZ(i, j, FLT_MAX);
//        }
//    }
//}





//        for (int i = 0; i < model.GetFacesCount(); i++) {
//            //get vertex normals
//            std::vector<glm::vec3> vertexNormals;
//            glm::vec4 vn1(model.GetNormals(model.GetFace(i).GetNormalIndex(0) - 1), 1.0f);
//            glm::vec4 vn2(model.GetNormals(model.GetFace(i).GetNormalIndex(1) - 1), 1.0f);
//            glm::vec4 vn3(model.GetNormals(model.GetFace(i).GetNormalIndex(2) - 1), 1.0f);
//
//
//            vertexNormals.push_back(vn1);
//            vertexNormals.push_back(vn2);
//            vertexNormals.push_back(vn3);
//
//            auto face = model.GetFace(i);
//            glm::vec4 p1_vec4(model.GetVertices(face.GetVertexIndex(0) - 1), 1.0f);
//            glm::vec4 p2_vec4(model.GetVertices(face.GetVertexIndex(1) - 1), 1.0f);
//            glm::vec4 p3_vec4(model.GetVertices(face.GetVertexIndex(2) - 1), 1.0f);
//
//            p1_vec4 = transforedMat * p1_vec4;
//            p2_vec4 = transforedMat * p2_vec4;
//            p3_vec4 = transforedMat * p3_vec4;
//
//            p1_vec4 /= p1_vec4.w;
//            p2_vec4 /= p2_vec4.w;
//            p3_vec4 /= p3_vec4.w;
//
//            glm::vec3 p1(p1_vec4);
//            glm::vec3 p2(p2_vec4);
//            glm::vec3 p3(p3_vec4);
//
//
//            CenterPoint(p1);
//            CenterPoint(p2);
//            CenterPoint(p3);
//
//
//
//           /* if (model.GetAxes(8))
//            {
//                DrawTriangle(p1, p2, p3, color);
//            }*/
//            // draw faces with random colors
//            if (model.GetAxes(7))
//            {
//                FillTriangle(p1, p2, p3, model.Getcolor_faces(i));
//            }
//
//            // draw faces bounding
//            if (model.GetAxes(6))
//            {
//                DrawFacesReactangles(model, p1, p2, p3);
//            }
//            // draw faces normals
//            if (model.GetAxes(4)) {
//                DrawFacesNormals(model, face, transforedMat);
//            }
//            //draw vertces normals
//            if (model.GetAxes(5)) {
//                DrawVertexNormals(model, i, p1, p2, p3);
//            }
//
//            auto lights_count = scene.GetLightCount();
//            for (size_t i = 0; i < lights_count; i++)
//            {
//                auto& light = scene.GetLightAtIndex(i);
//                FillTriangleWithLight(p1, p2, p3, light,scene,model,vertexNormals);
//
//                if (model.GetAxes(8))
//                {
//                    //Draw light vector
//                    glm::vec3 center((p1 + p2 + p3) / 3.0f);
//                    glm::vec2 p;
//                    float m;
//                    if ((center.x - light.GetPosition().x) != 0)
//                    {
//                        m = (center.y - light.GetPosition().y) / (center.x - light.GetPosition().x);
//                        float c = center.y - m * center.x;
//                        p = { center.x - 20, (center.x - 20) * m + c };
//                    }
//                    else
//                    {
//                        p = { center.x - 1, center.y - 1 };
//                    }
//                    glm::vec3 silver(192, 192, 192);
//                    DrawLine(p, center, silver);
//
//                    glm::vec3 lightDirection = glm::normalize(light.GetPosition() - center);
//                    //Draw reflection vectors
//                    glm::vec3 faceNorm = calculateNormal(p1, p2, p3);
//                    glm::vec3 gold(255, 223, 0);
//                    drawReflectionVector(lightDirection, faceNorm,center,gold);
//                }
//            }
//        }
//            //Draw model axis
//            if (model.GetAxes(0))
//            {
//                DrawModleAxes(transforedMat);
//            }
//                //Draw world axis
//            if (model.GetAxes(1))
//            {
//                DrawWorldAxes(transforedMat, model.get_mat_transformation());
//            }
//            //Draw model bounding box
//            if (model.GetAxes(2))
//            {
//                DrawModelBoundingBox(model, transforedMat);
//            }
//            //Draw world bounding box
//            if (model.GetAxes(3))
//            {
//                DrawWorldBoundingBox(model, transforedMat,half_width,half_height);
//            }
//        // grey scale
//        if (model.GetAxes(9))
//        {
//            ShowZbuffer();
//        }
        
        
//    }
//
//
//}

//
//
//void Renderer::DrawTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& color){
//    DrawLine(p1, p2, color);
//    DrawLine(p2, p3, color);
//    DrawLine(p3, p1, color);
//}
//
//void Renderer::SwapVec(glm::vec3& p1, glm::vec3& p2){
//     glm::vec3 temp = p1;
//     p1 = p2;
//     p2 = temp;
// }
//
//void Renderer::FillTriangleWithLight(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Light& light,const Scene& scene ,MeshModel& model, const std::vector<glm::vec3> vertexNormals) {
//    //edge walking
//    Camera& camera = scene.GetActiveCamera();
//    Material& material = model.material;
//    float x1, x2, z;
//    glm::vec2 p_1, p_2;
//    glm::vec3 normal = calculateNormal(p1, p2, p3);
//    glm::vec3 lightDirection;
//    vector<float> lineE12, lineE13, lineE23;
//
//    // p1 has the max Y p3 has min y
//    if (p1.y < p2.y) SwapVec(p1, p2);
//    if (p2.y < p3.y) SwapVec(p2, p3);
//    if (p1.y < p2.y) SwapVec(p2, p1);
//
//    if (p1.y == p3.y) return;
//
//    GetLineE(p1, p2, lineE12);
//    GetLineE(p1, p3, lineE13);
//
//    // fill the triangle
//    for (int y = p1.y; y > p2.y; y--) {
//        if (p1.x != p2.x)   //avoid divide by 0
//            x1 = (y - lineE12[1]) / lineE12[2];
//        else  x1 = p1.x;
//        if (p1.x != p3.x)   //avoid divide by 0
//            x2 = (y - lineE13[1]) / lineE13[2];
//        else x2 = p1.x;
//
//        if (x1 < x2)  //x1 will be max
//            swap(x1, x2);
//        for (float x = x2; x <= x1; x++) {
//            z = CalculateZ(p1, p2, p3, x, y);
//            if (ComperAndSwap(x, y, z)) {
//                float xNormal = ((vertexNormals[0].x + vertexNormals[1].x + vertexNormals[2].x) / 3);
//                float yNormal = ((vertexNormals[0].y + vertexNormals[1].y + vertexNormals[2].y) / 3);
//                float zNormal = ((vertexNormals[0].z + vertexNormals[1].z + vertexNormals[2].z) / 3);
//
//                float xAvg = (p1.x + p2.x + p3.x) / 3;
//                float yAvg = (p1.y + p2.y + p3.y) / 3;
//                float zAvg = (p1.z + p2.z + p3.z) / 3;
//
//                glm::vec3 center(xAvg, yAvg, zAvg);
//                glm::vec2 point(x, y);
//                glm::vec3 faceCenter(xNormal, yNormal, zNormal);
//                glm::vec3 to(glm::cross(p1 - p2, p1 - p3));
//                glm::vec3 faceCenterNormal(faceCenter + glm::normalize(to));
//                glm::vec3 pointNormal = getTrianglesNormal(p1, p2, p3, point, vertexNormals[0], vertexNormals[1], vertexNormals[2]);
//                glm::vec3 position(x, y, z);
//
//                glm::vec3 lightDirection = glm::normalize(light.GetPosition() - faceCenter);
//                glm::vec3 viewDirection = glm::normalize(camera.GetCameraPosition() - position);
//                glm::vec3 color;
//                if (scene.shading_type() == "FLAT")
//                {
//                    color = calculateColor(normal, lightDirection, viewDirection, model.material, light.ambient, light.diffuse,
//                        light.specular, 10.0f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//                else if (scene.shading_type() == "GOURAUD")
//                {
//                    lightDirection = glm::normalize(light.GetPosition() - pointNormal);
//                    color = calculateColor(pointNormal, lightDirection, viewDirection, model.material, light.ambient, light.diffuse, light.specular, 10.0f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//                if (scene.shading_type() == "PHONG")
//                {
//                    glm::vec3 color = material.ambient * light.ambient;
//                    glm::vec3 lightDirection = glm::normalize(light.GetPosition() - position);
//                    glm::vec3 viewDirection = glm::normalize(camera.GetCameraPosition() - position);
//                    color += light.diffuse * material.diffuse * glm::max(glm::dot(lightDirection, pointNormal), 0.0f);
//                    glm::vec3 R = glm::normalize(2.0f * glm::dot(lightDirection, pointNormal) * pointNormal - lightDirection);
//                    color += light.specular * material.specular * pow(glm::max(glm::dot(viewDirection, R), 0.0f), 30.f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//            }
//        }
//    }
//    GetLineE(p2, p3, lineE23);
//    for (int y = p2.y; y > p3.y; y--) {
//        if (p1.x != p3.x)   //avoid divede by 0
//            x1 = (y - lineE13[1]) / lineE13[2];
//        else x1 = p1.x;
//        if (p2.x != p3.x)   //avoid divede by 0
//            x2 = (y - lineE23[1]) / lineE23[2];
//        else x2 = p2.x;
//        if (x1 < x2)
//            swap(x1, x2); //x1 will be max
//        for (float x = x2; x <= x1; x++) {
//            z = CalculateZ(p1, p2, p3, x, y);
//            if (ComperAndSwap(x, y, z)) {
//                float xNormal = ((vertexNormals[0].x + vertexNormals[1].x + vertexNormals[2].x) / 3);
//                float yNormal = ((vertexNormals[0].y + vertexNormals[1].y + vertexNormals[2].y) / 3);
//                float zNormal = ((vertexNormals[0].z + vertexNormals[1].z + vertexNormals[2].z) / 3);
//
//                float xAvg = (p1.x + p2.x + p3.x) / 3;
//                float yAvg = (p1.y + p2.y + p3.y) / 3;
//                float zAvg = (p1.z + p2.z + p3.z) / 3;
//
//                glm::vec3 center(xAvg, yAvg, zAvg);
//                glm::vec2 point(x, y);
//                glm::vec3 faceCenter(xNormal, yNormal, zNormal);
//                glm::vec3 to(glm::cross(p1 - p2, p1 - p3));
//                glm::vec3 faceCenterNormal(faceCenter + glm::normalize(to));
//                glm::vec3 pointNormal = getTrianglesNormal(p1, p2, p3, point, vertexNormals[0], vertexNormals[1], vertexNormals[2]);
//                glm::vec3 position(x, y, z);
//                glm::vec3 lightDirection = glm::normalize(light.GetPosition() - faceCenter);
//                glm::vec3 viewDirection = glm::normalize(camera.GetCameraPosition() - position);
//                glm::vec3 color;
//
//                if (scene.shading_type() == "FLAT")
//                {
//                    color = calculateColor(normal, lightDirection, viewDirection, model.material, light.ambient, light.diffuse,
//                        light.specular, 10.0f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//                else if (scene.shading_type() == "GOURAUD")
//                {
//                    lightDirection = glm::normalize(light.GetPosition() - pointNormal);
//                    color=calculateColor(pointNormal, lightDirection, viewDirection, model.material, light.ambient, light.diffuse, light.specular, 10.0f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//                if (scene.shading_type() == "PHONG")
//                {
//                    glm::vec3 color = material.ambient * light.ambient;
//                    glm::vec3 lightDirection = glm::normalize(light.GetPosition() - position);
//                    glm::vec3 viewDirection = glm::normalize(camera.GetCameraPosition() - position);
//                    color += light.diffuse * material.diffuse * glm::max(glm::dot(lightDirection, pointNormal), 0.0f);
//                    glm::vec3 R = glm::normalize(2.0f * glm::dot(lightDirection, pointNormal) * pointNormal - lightDirection);
//                    color += light.specular * material.specular * pow(glm::max(glm::dot(viewDirection, R), 0.0f), 30.f);
//                    PutPixel(x, y, color);
//                    PutZ(x, y, z);
//                }
//            }
//        }
//    }
//}
//
//
//void Renderer::FillTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, const glm::vec3& color_triangle){
//    //egde walking
//    float x1, x2, z;
//    glm::vec2 p_1, p_2;
//    vector<float> lineE12, lineE13, lineE23;
//
//    // p1 has the max Y p3 has min y
//    if (p1.y < p2.y) SwapVec(p1, p2);
//    if (p2.y < p3.y) SwapVec(p2, p3);
//    if (p1.y < p2.y) SwapVec(p2, p1);
//
//   if (p1.y == p3.y) return;
//
//    GetLineE(p1, p2, lineE12);
//    GetLineE(p1, p3, lineE13);
//
//    // fill the triangle
//    for(int y = p1.y; y > p2.y; y--){
//        if (p1.x != p2.x)   //avoid divede by 0
//            x1 = (y - lineE12[1]) / lineE12[2];
//        else  x1 = p1.x;
//        if(p1.x != p3.x)   //avoid divede by 0
//            x2 = (y - lineE13[1]) / lineE13[2];
//        else x2 = p1.x;
//
//        if(x1 < x2)  //x1 will be max
//            swap(x1, x2);
//        for(float x = x2; x <= x1; x++){
//            z = CalculateZ(p1, p2, p3, x, y);
//            if (ComperAndSwap(x, y, z)){
//                    PutPixel(x, y, color_triangle);
//                    PutZ(x, y, z);
//            }
//        }
//    }
//
//   GetLineE(p2, p3, lineE23);
//   for(int y = p2.y; y > p3.y; y--){
//        if(p1.x != p3.x)   //avoid divede by 0
//            x1 = (y - lineE13[1]) / lineE13[2];
//        else x1 = p1.x;
//        if(p2.x != p3.x)   //avoid divede by 0
//            x2 = (y - lineE23[1]) / lineE23[2];
//        else x2 = p2.x;
//        if(x1 < x2)
//            swap(x1, x2); //x1 will be max
//        for(float x = x2; x <= x1; x++){
//            z = CalculateZ(p1, p2, p3, x, y);
//            if (ComperAndSwap(x, y, z)){
//                PutPixel(x, y, color_triangle);
//                PutZ(x, y, z);
//            }
//        }
//   }
//
// }
//
//void Renderer::GetLineE(glm::vec3 p1, glm::vec3 p2, vector<float>& e){
//    float m, n;
//     m = (p1.y - p2.y) / (p1.x - p2.x);
//     n = p1.y - (m*p1.x);
//     if(!e.empty())
//         e.clear();
//     e.push_back(p1.y);
//     e.push_back(n);
//     e.push_back(m);
// }
//
//

//
//void Renderer::ColorBufferUpdate(){
//    delete[] color_buffer;
//    delete[] z_buffer;
//    InitOpenglRendering();
//    CreateBuffers(viewport_width, viewport_height);
//}
//
//
//void Renderer::DrawCircle()
//{
//// // TODO: Replace this code with real scene rendering code
//    int half_width = viewport_width / 2;
//    int half_height = viewport_height / 2;
//    // draw circle
//    int x0 = half_width, y0 = half_height, r =300;
//    int a=1000;
//    glm::ivec2 center(x0, y0);
//    glm::vec3 black(0, 0, 0);
//    for(int i =0; i < a; i++ ){
//        glm::ivec2 line(x0 + r*sin((2*M_PI*i)/a), y0 + r*cos((2*M_PI*i)/a));
//        DrawLine(center,line ,black);
//    }
//}
//
//
//void Renderer::PapperPlane()
//{
//    int half_width = viewport_width / 2;
//    int half_height = viewport_height / 2;
//    int x0 = half_width, y0 = half_height, r =300;
//    glm::ivec2 center(x0, y0);
//    glm::vec3 black(0, 0, 0);
//
//
//    glm::ivec2 p1(x0+13, y0+19);
//    glm::ivec2 p2(x0+20, y0+2);
//    glm::ivec2 p3(x0+46, y0-68);
//    glm::ivec2 p4(x0+5, y0+45);
//    glm::ivec2 p5(x0-50, y0+74);
//    glm::ivec2 p6(x0+211, y0+72);
//
//    DrawLine(center, p1 ,black);
//    DrawLine(center, p2 ,black);
//    DrawLine(center, p4 ,black);
//
//    DrawLine(p1, p3 ,black);
//    DrawLine(p1, p6 ,black);
//    DrawLine(p3, p6 ,black);
//    DrawLine(p4, p6 ,black);
//    DrawLine(p4, p5 ,black);
//    DrawLine(p5, p6 ,black);
//
//    //lines
//    DrawLine(glm::ivec2(x0-5, y0+22), glm::ivec2(x0-24, y0+19) ,black);
//    DrawLine(glm::ivec2(x0-36, y0+19), glm::ivec2(x0-52, y0+16) ,black);
//    DrawLine(glm::ivec2(x0-68, y0+14), glm::ivec2(x0-85, y0+10) ,black);
//}
//
//
//void Renderer::CenterPoint(glm::vec3& v){
//    v[0] = (v[0] + 1.f) * (((float)viewport_width) / 2.f );
//    v[1] = (v[1] + 1.f) * (((float)viewport_height) / 2.f );
//    v[2] = (v[2]+ 1.f)/ 2.0f;
//}
//
//void Renderer::DrawModleAxes(glm::mat4 transforedMat) {
//    glm::vec3 model_axes_color[] = { glm::vec3(0.8, 0.5, 0.5),  glm::vec3(0.5, 0.70, 0.5),  glm::vec3(0.5, 0.5, 0.93) };
//    glm::vec4 o_model(0.0f, 0.0f, 0.0f, 1.0f);
//    glm::vec4 x_model(viewport_width / 4, 0.0f, 0.0f, 1.0f);
//    glm::vec4 y_model(0.0f, viewport_height / 4, 0.0f, 1.0f);
//    glm::vec4 z_model(-viewport_width / 8, -viewport_width / 8, 0.0f, 1.0f);
//
//    o_model = transforedMat * o_model;
//    x_model = transforedMat * x_model;
//    y_model = transforedMat * y_model;
//    z_model = transforedMat * z_model;
//
//    glm::vec3 o(o_model);
//    glm::vec3 x(x_model);
//    glm::vec3 y(y_model);
//    glm::vec3 z(z_model);
//
//    CenterPoint(o);
//    CenterPoint(x);
//    CenterPoint(y);
//    CenterPoint(z);
//
//    DrawLine(o, x, model_axes_color[0]);
//    DrawLine(o, y, model_axes_color[1]);
//    DrawLine(o, z, model_axes_color[2]);
//}
//
//void Renderer::DrawWorldAxes(glm::mat4 transforedMat,glm::mat4 transformation) {
//    glm::vec3 world_axes_color[] = { glm::vec3(0.0, 0.0, 0.0),  glm::vec3(0.0, 0.0, 0.0),  glm::vec3(0.0, 0.0, 0.0) };
//    glm::vec4 o_world(0.0f, 0.0f, 0.0f, 0.0f);
//    glm::vec4 x_world(viewport_width, 0.0f, 0.0f, 0.0f);
//    glm::vec4 y_world(0.0f, viewport_height, 0.0f, 0.0f);
//    glm::vec4 z_world(-viewport_width, -viewport_height, 0.0f, 0.0f);
//
//    o_world = transforedMat / transformation * o_world;
//    x_world = transforedMat / transformation * x_world;
//    y_world = transforedMat / transformation * y_world;
//    z_world = transforedMat / transformation * z_world;
//
//
//    glm::vec3 o(o_world);
//    glm::vec3 x(x_world);
//    glm::vec3 y(y_world);
//    glm::vec3 z(z_world);
//
//    CenterPoint(o);
//    CenterPoint(x);
//    CenterPoint(y);
//    CenterPoint(z);
//
//
//    DrawLine(o, x, world_axes_color[0]);
//    DrawLine(o, y, world_axes_color[1]);
//    DrawLine(o, z, world_axes_color[2]);
//}
//
//void Renderer::DrawFacesNormals(MeshModel& model, Face face, glm::mat4 transforedMat)
//{
//    glm::vec4 v1(model.GetVertices(face.GetVertexIndex(0) - 1), 1.0f);
//    glm::vec4 v2(model.GetVertices(face.GetVertexIndex(1) - 1), 1.0f);
//    glm::vec4 v3(model.GetVertices(face.GetVertexIndex(2) - 1), 1.0f);
//
//    v1 = transforedMat * v1;
//    v2 = transforedMat * v2;
//    v3 = transforedMat * v3;
//
//    v1 /= v1[3];
//    v2 /= v2[3];
//    v3 /= v3[3];
//
//    glm::vec3 v1_3(v1);
//    glm::vec3 v2_3(v2);
//    glm::vec3 v3_3(v3);
//
//    CenterPoint(v1_3);
//    CenterPoint(v2_3);
//    CenterPoint(v3_3);
//
//    glm::vec3 start_point((v1_3 + v2_3 + v3_3) / 3.0f);
//    glm::vec3 end_point(start_point + model.get_scale_factor_local() * 50.0f * glm::normalize(glm::cross(v1_3 - v3_3, v1_3 - v2_3)));
//    DrawLine(glm::vec2(start_point.x, start_point.y), glm::vec2(end_point.x, end_point.y), glm::vec3(0.5, 0.5, 0.93));
//}
//
//void Renderer::DrawVertexNormals(MeshModel& model,int i, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
//{
//    glm::vec4 vn1(model.GetNormals(model.GetFace(i).GetNormalIndex(0) - 1), 1.0f);
//    glm::vec4 vn2(model.GetNormals(model.GetFace(i).GetNormalIndex(1) - 1), 1.0f);
//    glm::vec4 vn3(model.GetNormals(model.GetFace(i).GetNormalIndex(2) - 1), 1.0f);
//
//
//    vn1 = model.get_rotate_world_mat() * model.get_rotate_local_mat() * vn1;
//    vn2 = model.get_rotate_world_mat() * model.get_rotate_local_mat() * vn2;
//    vn3 = model.get_rotate_world_mat() * model.get_rotate_local_mat() * vn3;
//
//    vn1 /= vn1[3];
//    vn2 /= vn2[3];
//    vn3 /= vn3[3];
//
//    glm::vec3 vn1_(vn1);
//    glm::vec3 vn2_(vn2);
//    glm::vec3 vn3_(vn3);
//
//    DrawLine(p1, p1 + vn1_ * model.get_scale_factor_local() * 50.0f, glm::vec3(0.8, 0.5, 0.5));
//    DrawLine(p2, p2 + vn2_ * model.get_scale_factor_local() * 50.0f, glm::vec3(0.8, 0.5, 0.5));
//    DrawLine(p3, p3 + vn3_ * model.get_scale_factor_local() * 50.0f, glm::vec3(0.8, 0.5, 0.5));
//}
//
//void Renderer::DrawModelBoundingBox(MeshModel& model, glm::mat4 transforedMat)
//{
//    glm::vec4 model_box_color(1, 1, 1, 1);
//
//    float min_x = model.GetModelBoundaries(0);
//    float max_x = model.GetModelBoundaries(1);
//    float min_y = model.GetModelBoundaries(2);
//    float max_y = model.GetModelBoundaries(3);
//    float min_z = model.GetModelBoundaries(4);
//    float max_z = model.GetModelBoundaries(5);
//
//
//    glm::vec4 p1(min_x, min_y, min_z, 1.0f);
//    glm::vec4 p2(max_x, min_y, min_z, 1.0f);
//    glm::vec4 p3(min_x, max_y, min_z, 1.0f);
//    glm::vec4 p4(max_x, max_y, min_z, 1.0f);
//    glm::vec4 p5(min_x, min_y, max_z, 1.0f);
//    glm::vec4 p6(max_x, min_y, max_z, 1.0f);
//    glm::vec4 p7(min_x, max_y, max_z, 1.0f);
//    glm::vec4 p8(max_x, max_y, max_z, 1.0f);
//
//
//    p1 = transforedMat * p1;
//    p2 = transforedMat * p2;
//    p3 = transforedMat * p3;
//    p4 = transforedMat * p4;
//    p5 = transforedMat * p5;
//    p6 = transforedMat * p6;
//    p7 = transforedMat * p7;
//    p8 = transforedMat * p8;
//
//    glm::vec3 p1_model_box(p1);
//    glm::vec3 p2_model_box(p2);
//    glm::vec3 p3_model_box(p3);
//    glm::vec3 p4_model_box(p4);
//    glm::vec3 p5_model_box(p5);
//    glm::vec3 p6_model_box(p6);
//    glm::vec3 p7_model_box(p7);
//    glm::vec3 p8_model_box(p8);
//
//    CenterPoint(p1_model_box);
//    CenterPoint(p2_model_box);
//    CenterPoint(p3_model_box);
//    CenterPoint(p4_model_box);
//    CenterPoint(p5_model_box);
//    CenterPoint(p6_model_box);
//    CenterPoint(p7_model_box);
//    CenterPoint(p8_model_box);
//
//    DrawLine(p1_model_box, p2_model_box, model_box_color);
//    DrawLine(p1_model_box, p3_model_box, model_box_color);
//    DrawLine(p2_model_box, p4_model_box, model_box_color);
//    DrawLine(p3_model_box, p4_model_box, model_box_color);
//    DrawLine(p5_model_box, p6_model_box, model_box_color);
//    DrawLine(p5_model_box, p7_model_box, model_box_color);
//    DrawLine(p6_model_box, p8_model_box, model_box_color);
//    DrawLine(p7_model_box, p8_model_box, model_box_color);
//    DrawLine(p1_model_box, p5_model_box, model_box_color);
//    DrawLine(p2_model_box, p6_model_box, model_box_color);
//    DrawLine(p3_model_box, p7_model_box, model_box_color);
//    DrawLine(p4_model_box, p8_model_box, model_box_color);
//}
//
//void Renderer::DrawWorldBoundingBox(MeshModel& model, glm::mat4 transforedMat, int half_width, int half_height)
//{
//    glm::vec4 world_box_color(0, 0, 0, 0);
//    glm::vec4 p1(0.0f, 0.0f, 0.0f, 0.0f);
//    glm::vec4 p2(viewport_width, 0.0f, 0.0f, 0.0f);
//    glm::vec4 p3(0.f, viewport_height, 0.0f, 0.0f);
//    glm::vec4 p4(viewport_width, viewport_height, 0.0f, 0.0f);
//    glm::vec4 p5(-viewport_width, -viewport_height, 0.0f, 0.0f);
//    glm::vec4 p6(viewport_width, -viewport_height, 0.0f, 0.0f);
//    glm::vec4 p7(-viewport_width, viewport_height, 0.0f, 0.0f);
//    glm::vec4 p8(viewport_width + viewport_height, viewport_height, 0.0f, 0.0f);
//
//    p1 = transforedMat / model.get_mat_transformation() * p1;
//    p2 = transforedMat / model.get_mat_transformation() * p2;
//    p3 = transforedMat / model.get_mat_transformation() * p3;
//    p4 = transforedMat / model.get_mat_transformation() * p4;
//    p5 = transforedMat / model.get_mat_transformation() * p5;
//    p6 = transforedMat / model.get_mat_transformation() * p6;
//    p7 = transforedMat / model.get_mat_transformation() * p7;
//    p8 = transforedMat / model.get_mat_transformation() * p8;
//
//    glm::vec3 p1_world_box(p1);
//    glm::vec3 p2_world_box(p2);
//    glm::vec3 p3_world_box(p3);
//    glm::vec3 p4_world_box(p4);
//    glm::vec3 p5_world_box(p5);
//    glm::vec3 p6_world_box(p6);
//    glm::vec3 p7_world_box(p7);
//    glm::vec3 p8_world_box(p8);
//
//    CenterPoint(p1_world_box);
//    CenterPoint(p2_world_box);
//    CenterPoint(p3_world_box);
//    CenterPoint(p4_world_box);
//    CenterPoint(p5_world_box);
//    CenterPoint(p6_world_box);
//    CenterPoint(p7_world_box);
//    CenterPoint(p8_world_box);
//
//
//    DrawLine(p1_world_box, p2_world_box, world_box_color);
//    DrawLine(p1_world_box, p3_world_box, world_box_color);
//    DrawLine(p1_world_box, p5_world_box, world_box_color);
//    DrawLine(p5_world_box, p6_world_box, world_box_color);
//    DrawLine(p5_world_box, p7_world_box, world_box_color);
//    DrawLine(p7_world_box, p3_world_box, world_box_color);
//    DrawLine(p6_world_box, p8_world_box, world_box_color);
//    DrawLine(p7_world_box, p8_world_box, world_box_color);
//    DrawLine(p8_world_box, p4_world_box, world_box_color);
//    DrawLine(p3_world_box, p4_world_box, world_box_color);
//    DrawLine(p6_world_box, p2_world_box, world_box_color);
//    DrawLine(p2_world_box, p4_world_box, world_box_color);
//}
//
//void Renderer::DrawFacesReactangles(MeshModel& model, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
//{
//    float min_x, min_y, min_z, max_x, max_y, max_z, rec_color;
//
//    max_x = max(p1.x, p2.x);
//    max_x = max(max_x, p3.x);
//    min_x = min(p1.x, p2.x);
//    min_x = min(min_x, p3.x);
//    max_y = max(p1.y, p2.y);
//    max_y = max(max_y, p3.y);
//    min_y = min(p1.y, p2.y);
//    min_y = min(min_y, p3.y);
//    max_z = max(p1.z, p2.z);
//    max_z = max(max_z, p3.z);
//    min_z = min(p1.z, p2.z);
//    min_z = min(min_z, p3.z);
//
//
//
//    glm::vec3 p1_rec(min_x, min_y, 1.0f);
//    glm::vec3 p2_rec(max_x, min_y, 1.0f);
//    glm::vec3 p3_rec(min_x, max_y, 1.0f);
//    glm::vec3 p4_rec(max_x, max_y, 1.0f);
//
//    rec_color = (max_z - min_z) / (model.get_scale_factor_local() + model.get_scale_factor_world());
//
//    glm::vec3 color(rec_color*1.f, rec_color*0.4f ,rec_color*0.2f);
//
//
//    DrawLine(p1_rec, p2_rec, color);
//    DrawLine(p1_rec, p3_rec, color);
//    DrawLine(p2_rec, p4_rec, color);
//    DrawLine(p3_rec, p4_rec, color);
//
//}
//
//void Renderer::ShowZbuffer()
//{
//    for(int i=0;i<viewport_width;i++){
//        for(int j=0;j<viewport_height;j++){
//            float z = z_buffer[Z_INDEX(viewport_width, i, j)];
//            if(z < FLT_MAX){
//                float c =  (float) 1 - (z - minZ) / (maxZ-minZ);
//                PutPixel(i, j, {c,c,c});
//            }
//        }
//    }
//}
//
//glm::vec3 Renderer::calculateNormal(const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
//{
//    glm::vec3 edge1 = p2 - p1;
//    glm::vec3 edge2 = p3 - p1;
//    glm::vec3 normal = cross(edge1, edge2);
//    normal = normalize(normal);
//    return normal;
//}
//
//glm::vec3 Renderer::calculateColor(glm::vec3 normal, glm::vec3 lightDirection, glm::vec3 viewDirection,Material& material,
//    glm::vec3 ambientColor,
//    glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess) {
//    glm::vec3 color;
//    float diffuseIntensity = max(dot(normal, lightDirection), 0.0f);
//    glm::vec3 diffuseTerm = diffuseIntensity * diffuseColor * material.diffuse;
//    glm::vec3 reflectTerm = reflect(-lightDirection, normal);
//    float specularIntensity = pow(max(dot(reflectTerm, viewDirection), 0.0f), shininess);
//    glm::vec3 specularTerm = specularIntensity * specularColor * material.specular;
//
//    color = (ambientColor * material.ambient) + diffuseTerm + specularTerm;
//    return color;
//}
//
//
//
//glm::vec3 Renderer::interpolate(const glm::vec3& color1, const glm::vec3& color2, const glm::vec3& color3, const glm::vec3& barycentric) {
//    return color1 * barycentric.x + color2 * barycentric.y + color3 * barycentric.z;
//}
//
//void Renderer::drawReflectionVector(glm::vec3 lightDirection, glm::vec3 normal, glm::vec3 center, glm::vec3 color) {
//    std::vector<float> reflectionVector = computeReflectionVector(lightDirection, normal);
//    glm::ivec2 start = glm::ivec2(center.x, center.y);
//    glm::ivec2 end = glm::ivec2(center.x + reflectionVector[0] * 50.0f, center.y + reflectionVector[1] * 50.0f);
//    DrawLine(start, end, color);
//}
//
//std::vector<float> Renderer::computeReflectionVector(glm::vec3 lightDirection, glm::vec3 normal) {
//    float dotProduct = 0;
//    for (int i = 0; i < 3; i++) {
//        dotProduct += lightDirection[i] * normal[i];
//    }
//    std::vector<float> reflectionVector(3);
//    for (int i = 0; i < 3; i++) {
//        reflectionVector[i] = 2 * dotProduct * normal[i] - lightDirection[i];
//    }
//    return reflectionVector;
//}
//
//glm::vec3 Renderer::getTrianglesNormal(glm::vec3& p1, glm::vec3& p2, glm::vec3& p3,
//    glm::vec2& point, const glm::vec3& normal1, const glm::vec3& normal2,const glm::vec3& normal3)
//{
//    float a1 = getTrianglesArea(p2, p3, point);
//    float a2 = getTrianglesArea(p1, p3, point);
//    float a3 = getTrianglesArea(p1, p2, point);
//
//    float sum = a1 + a2 + a3;
//    return normal1 * (a1 / sum) + normal1 * (a2 / sum) + normal2 * (a3 / sum);
//}
//float Renderer::getTrianglesArea(glm::vec3& p1, glm::vec3& p2, glm::vec2& p3)
//{
//    float a = p3.x * (p1.y - p2.y) + p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y);
//    return (a > 0) ? (0.5 * a) : (-0.5 * a);
//}
//
//

