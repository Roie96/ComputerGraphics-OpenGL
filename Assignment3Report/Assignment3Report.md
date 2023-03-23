# Assignment 3 report 

## Renderer::InitOpenGLRendering() - what it does

1. Create a unique identifiers, one for OpenGL texture  store in `gl_screen_tex`, and other one for VAO, and store it `gl_screen_vtc`.
2. `gl_screen_vtc` is set to be the current vertex array.
3. Create a unique identifier for a buffer.
4. Make the VAO the current one, using `glBindVertexArray`.
5. Create 2 GLfloat arrays `vtc` - 2 triangles  and `tex` - corresponding texture UV coordinates.
6. Make the buffer the current one, using `glBindBuffer`.
7. Alloacte memory on the GPU using `glBufferData`, then copy vtc and tex to it using `glBufferSubData`.
8. Loads and compiles a sheder `vshader.glsl` and `fshader.glsl`.
9. Make this program the current one, using `glUseProgram`.
10. Define the two attributes `vPosition` for vertex position data, and `vTexCoord` texture position data that's how the shader will find them.
11. Define `GL_TEXTURE0` as the texture id.
 


## Vertex Shader
we wrote a vertex shader, the code below
```GLSL
#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
 
    gl_Position = projection * view *  model * vec4(pos, 1.0f);
}

```

## Fragment Shader
we wrote a fragment shader that only outputs a constant color, we did that to ensure the pipeline works.
```GLSL
#version 330 core

out vec4 frag_color;

void main()
{
    frag_color = vec4(0, 0, 0, 0);
}
```
## Renderer
we updated the all the relevant vertex attributes and uniforms in the renderer so now, we are able to view the mesh in our application.
the updated Render
```CPP
void Renderer::Render(const Scene& scene)
{
    Camera& camera = scene.GetActiveCamera();
    float half_width = viewport_width / 2.0f;
    float half_height = viewport_height / 2.0f;
    float DollyZoom;
    for(int i=0 ; i < scene.GetModelCount(); i++){
        MeshModel& model = scene.GetModel(i);
        // Activate the 'colorShader' program (vertex and fragment shaders)
        colorShader.use();
        glm::mat4 model1 = model.get_mat_transformation();
        glm::mat4 modelview = camera.GetCameraTransformation();
        glm::mat4 modelpro = camera.GetProjectionTransformation();

        colorShader.setUniform("model", model.get_mat_transformation());
        colorShader.setUniform("view", camera.GetCameraTransformation());
        colorShader.setUniform("projection", camera.GetProjectionTransformation());
        colorShader.setUniform("material.textureMap", 0);

        // Set 'texture1' as the active texture at slot #0
        texture1.bind(0);

        // Drag our model's faces (triangles) in fill mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
        
        // Unset 'texture1' as the active texture at slot #0
        texture1.unbind(0);
        glm::vec3 color = model.get_model_color();
        colorShader.setUniform("color", model.get_model_color());

        // Drag our model's faces (triangles) in line mode (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(model.GetVAO());
        glDrawArrays(GL_TRIANGLES, 0, model.GetModelVertices().size());
        glBindVertexArray(0);
    }
}
```
here some examples

![example 1](https://user-images.githubusercontent.com/100276577/219401652-8935d16d-f2a4-4b70-a045-d3d4e5b07a4e.png)
![example 2](https://user-images.githubusercontent.com/100276577/219401664-074149ab-d33f-4882-90bd-4ca50da72896.png)

## Phong shading
we've Implemented Phong shading in the fragment shader.
you can see the improved performance
![Screen Shot 2023-02-19 at 18 08 29](https://user-images.githubusercontent.com/100276577/219960913-cb699933-96e4-44d8-831e-58b8b675dad1.png)
![Screen Shot 2023-02-19 at 18 09 11](https://user-images.githubusercontent.com/100276577/219960919-6155b42c-1b24-4967-bf5e-bab50f30a8b3.png)
![Screen Shot 2023-02-19 at 18 17 55](https://user-images.githubusercontent.com/100276577/219961089-6f874ca1-2994-42bf-8e30-fa69abe10844.png)

![Screen Shot 2023-02-20 at 11 38 17](https://user-images.githubusercontent.com/100276577/220068750-7967e2f8-35ac-4b10-a625-be6f280cd9c0.png)
![Screen Shot 2023-02-20 at 11 38 25](https://user-images.githubusercontent.com/100276577/220068761-f353aca4-381b-4e4f-8480-d529d5f7c847.png)

![Screen Shot 2023-02-20 at 11 38 42](https://user-images.githubusercontent.com/100276577/220068768-bfe9e0a7-2f64-438e-b4c2-72cc6d229a38.png)

## Texture mapping
for this task we needed to implement 3 types of canonical project plane, cylinder and sphere.
we search online for a mesh with texture coordinates, for sanity check we got a grid (Checker) and use it as a texture.
here is the result
### Plane
![Screen Shot 2023-02-21 at 20 46 13](https://user-images.githubusercontent.com/100276577/220432593-57d5d21d-5c5d-47e2-92d7-87bab4c0cb3b.png)
![Screen Shot 2023-02-21 at 20 45 23](https://user-images.githubusercontent.com/100276577/220432601-0476cd34-43ea-4c4c-8a1f-c66e0bd12192.png)



### cylinder
![Screen Shot 2023-02-21 at 20 46 17](https://user-images.githubusercontent.com/100276577/220432700-fb86b013-f2b6-46f9-bc4d-df036516ed1f.png)

![Screen Shot 2023-02-21 at 20 45 13](https://user-images.githubusercontent.com/100276577/220432687-df3dc532-ff01-4334-947b-4c2d3f66ccd8.png)


### sphere
![Screen Shot 2023-02-21 at 20 46 21](https://user-images.githubusercontent.com/100276577/220432782-1f7acea2-d25c-45bd-a14b-52282da07dd5.png)
![Screen Shot 2023-02-21 at 20 45 18](https://user-images.githubusercontent.com/100276577/220432767-3fbaec05-64e2-41c5-a40d-aa17a5da5baf.png)


## Normal mapping
We found online a diffuse and normal map of a brick wall, then we used that to obtain normal from a normal map in range [0,1], then transform the normal vector to range [-1,1], then the rest of the code is silimar to phong shading.
```GLSL
  normal = texture(normalMap, fragTexCoords).rgb;
  normal = normalize(normal * 2.0 - 1.0);
  color = texture(diffuseMap, fragTexCoords).rgb;
```
### without
![Screen Shot 2023-02-23 at 16 20 19](https://user-images.githubusercontent.com/100276577/220933920-d3c70cee-1847-4d30-9d60-e775030439e3.png)


### with
![Screen Shot 2023-02-23 at 16 20 25](https://user-images.githubusercontent.com/100276577/220933931-49c341e3-9990-4b59-92ce-94ae29947658.png)




## Environment mapping
We used the ratio = 1.00 / 1.52 to get a glass-like effect.
```GLSL
 //environment mapping
    else {
        float ratio = 1.00 / 1.52;
        vec3 R = refract(viewDir, norm, ratio);
        frag_color = vec4(texture(material.textureMap, R.xy).rgb, 1.0);
```
https://user-images.githubusercontent.com/100276577/221400829-1a70d1c2-294f-4c6e-8680-2ff81ae3e3d6.mov


## Toon shading
We used 4 levels and lightColor and  shadowColor to get the Toon effect.
```GLSL
    //toon
    levels = 4.0;
    level = floor(diff * levels) / levels;

    baseColor = textureColor.rgb;
    lightColor = vec3(1.0, 1.0, 1.0); // light col 
    shadowColor = vec3(0.5, 0.5, 0.5); // shadow color
    finalColor = mix(shadowColor, mix(baseColor, lightColor, level), level);
    frag_color = vec4(finalColor * color, textureColor.x);
```
![Screen Shot 2023-02-21 at 21 51 41](https://user-images.githubusercontent.com/100276577/220446254-41fc5008-641f-477b-89b5-4946bb01d7b6.png)![Screen Shot 2023-02-21 at 19 44 32](https://user-images.githubusercontent.com/100276577/220446263-f3925318-a66a-403b-98a5-ab90c4c063c6.png)
![Screen Shot 2023-02-21 at 19 44 51](https://user-images.githubusercontent.com/100276577/220446268-7004b298-a0e1-42e2-b09a-a877bfcb0a98.png)




