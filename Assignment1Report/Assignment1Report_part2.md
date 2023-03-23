# Assignment 1 report - part 2
On this assignment we've asked to load a mesh model, display it and it's transformations. 

## 1. Load Mesh Model
### print vertices and faces
```cpp
Vertices: 14
v0: 0 0 0
v1: 2 0 0
v2: 2 2 0
v3: 0 2 0
v4: 0 0 -2
v5: 2 0 -2
v6: 2 2 -2
v7: 0 2 -2
v8: -0.6 1 -1
v9: 1 -0.6 -1
v10: 2.6 1 -1
v11: 1 2.6 -1
v12: 1 1 0.6
v13: 1 1 -2.6
Faces: 24
f0:
v0: 1 vt0: 0 vn0: 1
v1: 13 vt1: 0 vn1: 2
v2: 4 vt2: 0 vn2: 1
f1:
v0: 4 vt0: 0 vn0: 3
v1: 13 vt1: 0 vn1: 4
v2: 3 vt2: 0 vn2: 3
f2:
v0: 3 vt0: 0 vn0: 5
v1: 13 vt1: 0 vn1: 6
v2: 2 vt2: 0 vn2: 5
f3:
v0: 2 vt0: 0 vn0: 7
v1: 13 vt1: 0 vn1: 8
v2: 1 vt2: 0 vn2: 7
f4:
v0: 4 vt0: 0 vn0: 1
v1: 9 vt1: 0 vn1: 9
v2: 1 vt2: 0 vn2: 1
f5:
v0: 8 vt0: 0 vn0: 10
v1: 9 vt1: 0 vn1: 11
v2: 4 vt2: 0 vn2: 10
f6:
v0: 5 vt0: 0 vn0: 12
v1: 9 vt1: 0 vn1: 13
v2: 8 vt2: 0 vn2: 12
f7:
v0: 1 vt0: 0 vn0: 14
v1: 9 vt1: 0 vn1: 15
v2: 5 vt2: 0 vn2: 14
f8:
v0: 5 vt0: 0 vn0: 16
v1: 14 vt1: 0 vn1: 17
v2: 6 vt2: 0 vn2: 16
f9:
v0: 8 vt0: 0 vn0: 12
v1: 14 vt1: 0 vn1: 18
v2: 5 vt2: 0 vn2: 12
f10:
v0: 7 vt0: 0 vn0: 19
v1: 14 vt1: 0 vn1: 20
v2: 8 vt2: 0 vn2: 19
f11:
v0: 6 vt0: 0 vn0: 21
v1: 14 vt1: 0 vn1: 22
v2: 7 vt2: 0 vn2: 21
f12:
v0: 3 vt0: 0 vn0: 23
v1: 11 vt1: 0 vn1: 24
v2: 7 vt2: 0 vn2: 23
f13:
v0: 2 vt0: 0 vn0: 5
v1: 11 vt1: 0 vn1: 25
v2: 3 vt2: 0 vn2: 5
f14:
v0: 6 vt0: 0 vn0: 26
v1: 11 vt1: 0 vn1: 27
v2: 2 vt2: 0 vn2: 26
f15:
v0: 7 vt0: 0 vn0: 21
v1: 11 vt1: 0 vn1: 28
v2: 6 vt2: 0 vn2: 21
f16:
v0: 3 vt0: 0 vn0: 3
v1: 12 vt1: 0 vn1: 29
v2: 4 vt2: 0 vn2: 3
f17:
v0: 4 vt0: 0 vn0: 10
v1: 12 vt1: 0 vn1: 30
v2: 8 vt2: 0 vn2: 10
f18:
v0: 8 vt0: 0 vn0: 19
v1: 12 vt1: 0 vn1: 31
v2: 7 vt2: 0 vn2: 19
f19:
v0: 7 vt0: 0 vn0: 23
v1: 12 vt1: 0 vn1: 32
v2: 3 vt2: 0 vn2: 23
f20:
v0: 1 vt0: 0 vn0: 7
v1: 10 vt1: 0 vn1: 33
v2: 2 vt2: 0 vn2: 7
f21:
v0: 2 vt0: 0 vn0: 26
v1: 10 vt1: 0 vn1: 34
v2: 6 vt2: 0 vn2: 26
f22:
v0: 6 vt0: 0 vn0: 16
v1: 10 vt1: 0 vn1: 35
v2: 5 vt2: 0 vn2: 16
f23:
v0: 5 vt0: 0 vn0: 14
v1: 10 vt1: 0 vn1: 36
v2: 1 vt2: 0 vn2: 14
```

## 2. Scale and Translate
We created two matrices, one for translate and one for scale, then we multiply those to create an object matrix (in that order: transMat * scaleMat), for every vertex we multiply with our new matrix, for that we had a function that get int scale, int x, int y, int z.
Then according to the display we changed the scale and moved the cordinates to answer the requirements. 

## 3. Banana object

<img width="1278" alt="Screen Shot 2022-11-29 at 12 47 02" src="https://user-images.githubusercontent.com/100276577/204510518-b5e831ec-1ecc-431d-ad33-ddbf138d825a.png">


## 4. GUI

<img width="1272" alt="Screen Shot 2022-11-29 at 12 54 51" src="https://user-images.githubusercontent.com/100276577/204511406-400d10ad-d075-44c0-b232-2ee663852374.png">

For the gui we have created two matrices for each model - local and world.
The computed order is :

```cpp
  local_matrix = transMat * scaleMat * rotateMat;
  world_matrix = transMat * scaleMat * rotateMat;
  transformation_matrix = world_matrix * local_matrix;
  vertex_[i] = transformation_matrix*vertex[i];
```

## 5. Loacl & World
### Local translate and world rotate

https://user-images.githubusercontent.com/100276577/204516117-d9002ead-9767-4954-9ca3-65bae1e1e567.mov

### World translate and local rotate

https://user-images.githubusercontent.com/100276577/204517120-3c338f49-2058-4a0f-969d-4d62cffed8fa.mov

## 6. Multiple models
https://user-images.githubusercontent.com/100276577/204518308-c2a91530-5568-405c-b5af-a655e59eaf3e.mov


## 7. Keybord and Mouse utility 
### Mouse 
	By clicking on left side, the model will zoom in, and for zoom out click on the right side.
### Keyboard
	- using the arrows you can translate the model (local).   
		right arrow = x+;                                  
		left arrow  = x-; 
		up arrow    = y+;
		down arrow  = y-;
		
	  - using 'W' ,'S', 'D', 'A' you will rotate the model (local).
	  	'D'  = x+;                                  
		'A'  = x-; 
		'W'  = y+;
		'S'  = y-;
		
