#version 330 core
#define PI 3.1415926538
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;


// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int canonicalProject;



// These outputs will be available in the fragment shader as inputs
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;
out vec3 orig_fragPos;


void main()
{
    // Apply the model transformation to the 'position' and 'normal' properties of the vertex,
    // so the interpolated values of these properties will be available for usi n the fragment shader
    fragPos = vec3(model * vec4(pos, 1.0f));
    fragNormal = mat3(view * model) * normal;
    orig_fragPos = vec3(vec4(pos, 1.0f));
    // Pass the vertex texture coordinates property as it is. Its interpolated value
    // will be avilable for us in the fragment shader
    
    switch (canonicalProject){
          // plane
          case 0:
            fragTexCoords = texCoords;
            break;
          // cylinder
          case 1:
            fragTexCoords = vec2(atan(orig_fragPos.z, orig_fragPos.x), orig_fragPos.y);
            break;
          // sphere
          case 2:
           fragTexCoords.x = 0.5 + atan(orig_fragPos.z, orig_fragPos.x);
           fragTexCoords.y = 0.5 - atan(-orig_fragPos.y, length(orig_fragPos.xz));
            break;
    }
    
    // This is an internal OpenGL variable, we must set a value to this variable
    gl_Position = projection * view *  model  * vec4(pos, 1.0f);
    
}







