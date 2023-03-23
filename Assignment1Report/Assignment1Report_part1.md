## Bresenham’s algorithm
```cpp
void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int delx = abs(p2.x - p1.x);
	int dely = abs(p2.y - p1.y); 
	int switchFlag = 0; // Decide if needs to switch x and y

	
	if (dely > delx){		
		int temp  = delx;
		delx = dely;
		dely = temp;
		switchFlag = 1;
	} 	
	int di = 2*(dely - delx);

	// Decide from where to start
	//line goes from left to right <-- or line goes from right to left -->	
	int x_way = (p1.x>p2.x)?-1:1;
	int y_way = (p1.y>p2.y)?-1:1;
	
	int xn = p1.x, yn = p1.y;
	for (int i = 0; i < abs(delx); i++) {
		PutPixel(xn, yn, color);
		if (di >= -180) {
			di = di + 2*dely - 2*delx;
			xn = xn + x_way;
			yn = yn + y_way;
		}
		else { 
			di = di + 2*dely;
			if (switchFlag){
				yn = yn + y_way;
			}
			else
				xn = xn + x_way;
		}
	}
}
```

## Sanity check
<img width="355" alt="image" src="https://user-images.githubusercontent.com/100403800/200528811-715c774c-107c-4de3-bd49-c25a2dd9750b.png">

## Papper  Plane
![Papper plane](https://user-images.githubusercontent.com/100276577/199712147-afcb1a87-4b5c-4800-bc6a-b7354ed2c33a.png)




