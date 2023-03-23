# Assignment 2 report - part 2
On this assignment we have completed the renderer by adding lights and display shaded model
## 1.   Add Point
In this section we have added the option to add a light to the scene.\
Using edge walking algorithem we were able to fill the actived model with added light which computed with chosen matriel of the model.\
We have computed the lightning for ambient light and demonstrate it by the following evidences:

As we can see white ambient light displayed the model by its ambient color - red
![image](https://user-images.githubusercontent.com/100403800/214162854-ebf7d4fa-d78f-48ca-838a-ca51ace6322b.png)

On this evidence we change the ambient light to blue which causes the red model become black due to ambient vecs product.
![image](https://user-images.githubusercontent.com/100403800/214169481-7523ff81-86c0-43f7-bca0-098494e14561.png)

## 2.  Diffuse component with flat shading 
We have computed the diffuse component by taking the dot product of the model normals and the light vectors.

![image](https://user-images.githubusercontent.com/100403800/214171016-8e07da67-8653-405c-86c9-d95d5bc38c61.png)

![image](https://user-images.githubusercontent.com/100403800/214171611-1e697f51-8226-4e9e-beaa-3d539f07d8e9.png)

On those examanations we can see the light is reflected off the model in a diffuse manner - scattered equally in all directions.

## 3. Gouraud shading 
On this section we have computed gouraud shading using vertexes normals instead of faces normals.\
As we can see in the evidense below, compare to flat shading model rendering became more realistic.

![image](https://user-images.githubusercontent.com/100403800/214331940-2b24dbe0-4e3f-4e24-b61e-83a1ccbd3886.png)

This shade wont provide the smoothest rendering but it look way too realistic in compare to flat shade as we can see:

![image](https://user-images.githubusercontent.com/100403800/214332328-d1363b49-7367-4b03-ad5b-16e731bd708c.png)

## 4. Light and reflection directions
On this section we provided a renderer feature to display light vectors that going from light source to each face center (siler vecs).\
The each face has 3 reflections vector that reflect accordignly. (light source in left down side of the screen)

![image](https://user-images.githubusercontent.com/100403800/214361371-772de410-a6b1-46a0-b7cc-bc62381207e1.png)

Translate light source on Y axe change the direction of the lights as we can see in this evidence (light source in now on left up side of the screen):
![image](https://user-images.githubusercontent.com/100403800/214362470-12fee93e-12a6-46a1-96d4-be5d98ef39a0.png)

## 5. Compute the specular light with Phong shading
On this section we calculated specular light using phong shading by computing light direction for each vertex.\
With this rendering method we got smoother image.

![image](https://user-images.githubusercontent.com/100403800/214424106-e0763dbf-976f-413d-85db-414c922b8713.png)

In this evidence we can see mush smoother model than in FLAT shading for comparance:

![image](https://user-images.githubusercontent.com/100403800/214424625-2c6970d1-b920-4402-8922-56741a28e0dc.png)

## 6. Different models, lights and shadings
Bunny model with flat shading and white light.

![image](https://user-images.githubusercontent.com/100403800/214425940-fc244702-3c0e-41ff-a133-041ef0eb2e1f.png)

Cow model with red ambient light lightend by yellow light - gouraud shading.
![image](https://user-images.githubusercontent.com/100403800/214426575-547a510d-ef1a-4c94-8618-b4ef22409738.png)

Blue ambient camera lightned with green diffuse ,red ambient colors - phong shading.
![image](https://user-images.githubusercontent.com/100403800/214427081-813b055e-e633-4781-ab95-5ce181c565c5.png)



