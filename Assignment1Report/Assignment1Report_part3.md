# Assignment 1 report - part 3

## 1. Orthographic Projection
we've implement Orthographic Projection and with GUI the user is able to change the view volume
### The initial view volume

![task 1 before 14 52 53](https://user-images.githubusercontent.com/100276577/208239611-9d1a36b1-472f-4033-869b-2f43bc652e13.png)

### The mesh model after altering view volume
![task 1 after 14 52 53](https://user-images.githubusercontent.com/100276577/208239620-9e494464-d49a-45ba-89a9-bad7b6f0f76b.png)


## 2. Camera frame & World frame
like in mesh model the camera has two matriecs, so we've created a class called MatrixTransform that has 2 matriecs and has function that cumpute the matrix tranformion, now camera and modelmesh are inherting for MatrixTransform class because they have the same implemetion.

### Camera frame

https://user-images.githubusercontent.com/100276577/208239628-d106969b-29b1-477e-a8e6-e57f6f9eed5c.mov

### World frame

https://user-images.githubusercontent.com/100276577/208239633-68041544-ce60-4ed0-b06a-3f941bfc84ba.mov



## 3. View volume automatically based on the window’s size
for this task we needed to change the view volume automatically based on the window’s size, so every time the user changed the window’s size we updated them in the renderer and changing the color buffer by deleteing him and creating new one based on the new window's height and width.

```cpp
void Renderer::ColorBufferUpdate(){
    delete[] color_buffer;
    InitOpenglRendering();
    CreateBuffers(viewport_width, viewport_height);
}
```

for the santiy check we change the window’s size and see if the proportions of the drawn object remain the same.

### demonstration with several window’s size
<img width="1274" alt="task 3 origin" src="https://user-images.githubusercontent.com/100276577/208239649-2b053155-820e-4d23-9d63-e739278fd574.png">

<img width="1049" alt="task3" src="https://user-images.githubusercontent.com/100276577/208239658-769c6c49-24d3-421c-81fc-dd5e11c9453d.png">

<img width="467" alt="task 3" src="https://user-images.githubusercontent.com/100276577/208239659-47fcbce4-67d4-4210-8b37-dc7d76e8792c.png">


## 4. Draw axes feature
We provided a feture via Renderer which display model and world axis.
Once performing world tansformations, world axes will move accordignly.

### Transforming in the model frame 

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208444977-72707713-21cf-4be9-b1e6-568823b3e103.png">

### Transforming in the world frame 

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208443906-e8f29599-4c25-455e-af29-2ec90dfcb9ea.png">

## 5. Non-commuting transformations
On this section we've defined two non commuting transformations:\
**T1** = translate to y by ~ 77deg.\
**T2** =  rotate by ~ 152 deg to x.

### T1 via model frame, T2 via world frame:

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208449686-f522c037-6d1a-41c4-ad8a-8e643165687e.jpg">

### T1 via world frame, T2 via model frame:

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208449867-5daa8de6-6a54-4733-a63b-8da781957223.jpg">

As we can see in the first examination, rotation performed according to world origin point as in the second examination rotation peformed according to model origin point.

## 6. Object bounding box
We have created bounding box based on model min and max coordinates.\
As in section 5 transformations were different between model and world frame.\
World frame transformation performed according to world origin point as model transformations according to model origin point.

### Transformation bounding box via model frame:

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208503377-4592d942-e51e-4a27-bf99-19ae6f93a7b7.png">

### Transformation bounding box via world frame:

<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208504298-d16cbf47-295b-44af-81e0-62092411c418.png">

## 7. Display face & vertex normals
On this section we have created renderer feature to display model vertex and faces normals.\
We have calculated the normals of each face and vertex and transformed it with the model so normals will stay fixed. 

### Dispaly face normals
<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208755993-58a28583-4365-4cf2-a769-5fc555decdbf.png">

### Dispaly vertex normals
<img width="803" alt="task 4" src="https://user-images.githubusercontent.com/100403800/208755693-3d92fff1-54cb-44b1-b8ed-76f80216731f.png">

For Sanity check we chose 2 models where we can clearly see same axes normals parallels to each other and vertical to faces and vertics.  

## 8. Perspective projection vs Orthographic projection
we've implement perspective projection, for sanity check we checked if the axis Z is effecting on the model, as we know in orthographic projection we do not have depht, hense it will not effect the model, but in perspective projection we should see the model getting bigger/smaller.

### Orthographic projection
<img width="1278" alt="task 8 orho" src="https://user-images.githubusercontent.com/100276577/208239673-b79f368f-b2f9-4920-acfb-4a3b73763bd4.png">

### Perspective projection 
<img width="1273" alt="task 8 pers" src="https://user-images.githubusercontent.com/100276577/208239674-9c129f5b-3c62-4876-ade0-d0e0c4ff11f9.png">


### Sanity check for Orthographic projection
<img width="1277" alt="task 8 ortho sanity" src="https://user-images.githubusercontent.com/100276577/208239677-f274ba71-914b-4542-ba33-dc922ad752d5.png">

### Sanity check for Perspective projection

<img width="1273" alt="task 8 persp sanity" src="https://user-images.githubusercontent.com/100276577/208239681-58b7fbf6-d369-4f53-b409-d66f3296e1e7.png">




## 9. Zoom in Perspective projection and getting closer 
in the video below we show the difference between changing the zoom (by changing the size of the frustum) and moving closer or farther from the model. 


https://user-images.githubusercontent.com/100276577/208239700-f8b1f4fc-4c74-420f-bcc5-11b88d0cd704.mov



## 10. LookAt
we've asked to set the camera to position to (a, a, a), we choosed a = 2300.
so fisrt we nove the camera to the select position, then we moived it back to the original position.
the video below demonstrates the process.


https://user-images.githubusercontent.com/100276577/208239705-405e7fa7-0eb6-461c-a900-1c6b5cd8aefb.mov

## 11. Dolly Zoom
for Dolly Zoom we needed to update the fovy and the camera position respectively, if you move the camera away from the meah, then we decrease the fovy, and on the contrary when the camera is closer to the mesh we increase the fovy
the code below:
```cpp
void Camera::SetDollyZoom(const float i)
{
    this->dollyZoom = i;
    // update the fovy
    Set_view_vol_p(0, 45.0f + (0.5*i));
     // update the eye.z
    Set_eye(2, 1000 -(i*10.0f));
}

```

https://user-images.githubusercontent.com/100276577/208293389-d4d98ffb-45a3-463d-98f6-80be73246ae6.mov





## 12. Multiple Models and Cameras on Screen


https://user-images.githubusercontent.com/100276577/208239754-7ce812dd-9449-4fbe-8c72-76151e98859f.mov



## 13. GUI
![Screen Shot 2022-12-20 at 23 43 47](https://user-images.githubusercontent.com/100276577/208773916-cbcfaf7d-a866-4f1d-8b8e-69bd2bf28d4d.png)
![Screen Shot 2022-12-20 at 23 43 20](https://user-images.githubusercontent.com/100276577/208773925-36373ee1-a75c-4eab-ad0c-3764df624054.png)
![Screen Shot 2022-12-20 at 23 43 38](https://user-images.githubusercontent.com/100276577/208773930-bab20c18-865d-4ca0-83f1-56c315e50841.png)



## 14. new features 
we've added a reset button that return the camera and model to the original position(you can see in GUI), and added a centring trick that makes the model to appear in the middle of the screen.

```cpp
void MeshModel::NormolizeModel(){
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
    float scale_factor = 700.f/size;
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale_factor,scale_factor,scale_factor));
    for(int i=0; i<vertices.size(); i++){
        glm::vec4 v(vertices[i], 1.f);
        v = scaleMat * v;
        vertices[i] = v;
    }
}
```


