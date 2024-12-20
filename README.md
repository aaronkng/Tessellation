# Goals of this project 
The objective of this project was to understand how Bézier surfaces operated and determine how their functionality can be used in surface tessellation. I developed a crude algorithm that 
would: 
- Take in the controls points from a text file
- Based on the how many subdivisions of tessellation, calculate surface points that goes along the Bézier surface using a series of matrix multiplications
- Use an algorithm that would organize the surface points into strips of triangle primitives, a format that the VBO reads so that the GPU can calculate the surfaces  <br>

This algorithm essentially takes in as input a text file of control points, and outputs a 3D object in which its smoothness is based on the number of subdivisions the user requested. 

# 0) Overview
A Bézier curve is defined by a set of control points $\textbf{P}_0$ to $\textbf{P}_n$, where $n$ is the order of the Bézier curve ($n = 1$ for linear
curves, $n = 2$ for quadratic curves, etc.). The first and last control points are always at the end of the curves, but intermediate control points are not constrained to the curve. 

<details><summary><b>Linear Bézier Curves</b></summary>
<p>
	
A linear Bézier curve is simply a straight line in which linear interpolation occurs between two control points, $\textbf{P}_0$ and $\textbf{P}_1$. <br>
- The equation for a linear Bézier curve can be written as: $\textbf{B}(t) = \textbf{P}_0 + t(\textbf{P}_1 - \textbf{P}_0)$, where $0 \leq t \leq 1$.
- This equation can be then  simplified to: $\textbf{B}(t) = (1-t)\textbf{P}_0 + (t)\textbf{P}_1$, where $0 \leq t \leq 1$. 
- If drawn on a graph, this is what a linear Bézier curve would look like on a graph:  
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72015614-f3d95f80-3216-11ea-9714-4c9e70cde675.png" width="250" height="250"> <br>
</p>
Source: https://en.wikipedia.org/wiki/Bézier_curve

</p>
</details>	

<details><summary><b>Quadratic Bézier Curves</b></summary>
<p>
	
A quadratic Bézier curve is a curve in which its path is constrained by three control points: $\textbf{P}_0$, $\textbf{P}_1$, and $\textbf{P}_2$. The image below describes what a
quadratic Bézier curve with three control points looks like: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72016266-51ba7700-3218-11ea-93fb-578be7213b90.jpg"> 
</p> 
This graph shows that a quadtratic Bézier curve is essentially an interpolation between two linear Bézier curves: $\textbf{B}_{\textbf{P}_0, \textbf{P}_1}$ 
and $\textbf{B}_{\textbf{P}_1, \textbf{P}_2}$. <br>

- A way to express this equation is $`\textbf{B}(t) = (1-t)[\textbf{B}_{\textbf{P}_0, \textbf{P}_1}(t)] + (t)[\textbf{B}_{\textbf{P}_1, \textbf{P}_2}(t)]`$, where $0 \leq t \leq 1$.
- The equation can be broken down to: $`\textbf{B}(t) = (1 - t)[(1-t)\textbf{P}_0 + (t)\textbf{P}_1] + (t)[(1-t)\textbf{P}_1 + (t)\textbf{P}_2]`$, where $0 \leq t \leq 1$.
- This equation can then be simplified to: $\textbf{B}(t) = (1-t)^2\textbf{P}_0 + 2(1-t)(t)\textbf{P}_1 + (t^2)\textbf{P}_2$, where $0 \leq t \leq 1$.  
  
Source: https://en.wikipedia.org/wiki/Bézier_curve
</p>
</details>

<details><summary><b>Cubic Bézier Curves</b></summary>
<p>

A cubic Bézier curve is a curve in which its path is constrained by four control points: $\textbf{P}_0$,  $\textbf{P}_1$,  $\textbf{P}_2$, and  $\textbf{P}_3$. The image below describes 
what a cubic Bézier curve with four control points looks like: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72017531-ba0a5800-321a-11ea-8608-edc9880e3132.jpg"> 
</p>  

The cubic Bézier curve is essentially an interpolation between two quadratic Bézier curves: $`\textbf{B}(t) = (1-t)[\textbf{B}_{\textbf{P}_0,\textbf{P}_1,\textbf{P}_2}(t)] + 
(t)[\textbf{B}_{\textbf{P}_1,\textbf{P}_2,\textbf{P}_3}(t)]`$, where $0 \leq t \leq 1$.
- The equation can be rewritten as: $\textbf{B}(t) = (1-t)^3\textbf{P}_0 + 3(1-t)^2(t)\textbf{P}_1 + 3(1-t)(t^2)\textbf{P}_2 + (t^3)\textbf{P}_3, 0 \leq t \leq 1$
- Note that in the quadratic and cubic Bézier curves, the coefficients in each polynomial patterns itself after the binomial coefficients. This means as that we increase the number of control points, we can intuit what the Bézier curve equation will be like.   
  
Source: https://en.wikipedia.org/wiki/Bézier_curve
</p>
</details>

<details><summary><b>Bézier Surface</b></summary>
<p> 
	
- A Bézier surface of degree $(n, m)$ is defined by a set of $(n + 1)(m + 1)$ control points $\textbf{k}_{ij}$, where $i = 0, ... , n$ and $j = 0, ..., m$.
- A two-dimensional Bézier surface can be defined as a parametric surface where the position of a point $p$ as the function of the parametric coordinates $u, v$ is given by: $\textbf{p}(u, v) = \sum_{i=0}^n \sum_{j=0}^m B_i^n(u) B_j^m(v) \textbf{k}_{ij}$. 
- $B_i^n(u) = \binom{n}{i} u^i(1-u)^{n-i}$ is the basis Bernstein polynomial and $\binom{n}{i} = \frac{n!}{i!(n-i)!}$ is the binomial coefficient.
- Typically, Bézier surfaces are constrained by the convex hull of its control points.
- Furthermore, the most common use of Bézier surfaces are as nets of **bicubic patches** ($m = n =3$); this means that a single bicubic patch consists of 16 control points. 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72022702-f42d2700-3225-11ea-8307-7f555e51aac1.gif">
</p>  
Source: https://en.wikipedia.org/wiki/Bézier_surface
</p>
</details>

# 1) Implementation
<details><summary><b>Retrieving data</b></summary>
<p>
	
The first line in `data.txt` represents the number of patches. Patches are formatted as such: 
```
(u,v)
Control point 0
Control point 1
...
Control point 15
```
Note that the control points are arranged as bicubic patches, meaning that the Bézier surface will 
consist of a matrix multiplication of two cubic Bézier curves. 
Source for control points of Utah Teapot is [here](http://www.holmes3d.net/graphics/teapot/). 

</p>
</details>

<details><summary><b>Tessellating Control Patches</b></summary>
<p>
	
The control points received from `data.txt` act as the skeletal structure for the patch, and in this portion of the implementation, we are constructing the surface of the patch by specifying the level of subdivisions we want and inputting the $(u, v)$ values into a function.  
  
If we have 0 levels of subdivision, then that means there are 0 subdivisions along the u-axis and 0 subdivisions along the v-axis, as shown in the diagram below: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72104550-88f45b00-32e0-11ea-9e79-8a9c8fa7f198.png" width="300" height="300">
</p>  

From here, we have 4 tessellated vertices to compute: $p(0, 0)$, $p(1, 0)$, $p(0, 1)$, and $p(1, 1)$.  
  
If we have 1 level of subdivision, then that means there are 1 subdivision along the u-axis and 1 subdivision along the v-axis, as shown in the diagram below: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72105867-323c5080-32e3-11ea-872d-13f05050ffca.png" width="300" height="300">
</p>  

From here, we have 9 tessellated vertices to compute: $p(0, 0)$, $p(0, \frac{1}{2})$, $p(0, 1)$, $p(\frac{1}{2}, 0)$, $p(\frac{1}{2}, \frac{1}{2})$, $p(\frac{1}{2}, 1)$, $p(1, 0)$, $p(1, \frac{1}{2})$, and $p(1, 1)$.  
  
Essentially, the number of subdivided polygons we receive is (level of subdivision + 1)<sup>2</sup>. Additionally, the number of tessellated vertices to compute is (level of subdivision + 2)<sup>2</sup>.  
  
Once we have the set of tessellated vertices, we need to compute each vertex using the parametric surface equation (section 0.3) via matrix multiplication: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72116169-3c1f7d00-32fe-11ea-92b7-9d927233f71c.png">
</p>  

Code Snippet: 
```
// Multiplication order: v * Bz_t * P * Bz * u
void formVertex(int patchID, int iter, int u, int v, glm::mat4 P_x, glm::mat4 P_y, glm::mat4 P_z)
{
	float u_value = (float)u / (float)LEVEL;
	float v_value = (float)v / (float)LEVEL;

	glm::vec4 u_vec = glm::vec4(1, u_value, u_value * u_value, u_value * u_value * u_value);
	glm::vec4 v_vec = glm::vec4(1, v_value, v_value * v_value, v_value * v_value * v_value);
	glm::mat4 Bz = glm::mat4(1, 0, 0, 0, -3, 3, 0, 0, 3, -6, 3, 0, -1, 3, -3, 1);

	glm::vec4 x_vec = glm::transpose(Bz) * P_x * Bz * u_vec;
	glm::vec4 y_vec = glm::transpose(Bz) * P_y * Bz * u_vec;
	glm::vec4 z_vec = glm::transpose(Bz) * P_z * Bz * u_vec;

	float x = v_vec.x * x_vec.x + v_vec.y * x_vec.y + v_vec.z * x_vec.z + v_vec.w * x_vec.w;
	float y = v_vec.x * y_vec.x + v_vec.y * y_vec.y + v_vec.z * y_vec.z + v_vec.w * y_vec.w;
	float z = v_vec.x * z_vec.x + v_vec.y * z_vec.y + v_vec.z * z_vec.z + v_vec.w * z_vec.w;

	vertices[0 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = x;
	vertices[1 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = y;
	vertices[2 + iter + patchID * LEVEL * LEVEL * NUM_TRIS_PER_SQUARE * NUM_VERTS_PER_TRI * NUM_FLOATS_PER_VEC3] = z;
}
```

To see how the matrix was computed, click 
[here](https://github.com/aaronkng/Tessellation/blob/master/Tessellation_Matrix.pdf).

</p>
</details>
  
<details><summary><b>Triangle Strip Construction</b></summary>
<p>
	
OpenGL renders objects onto the screen by taking data from the vertex buffer object and passing them through the graphics pipeline. We must indicate the primitive we intend to use to the graphics pipeline, and then, arrange our tessellated vertices so that it matches the drawing order of the primitive. Below show the drawing order of the primitive we are using, `GL_TRIANGLES`: 
```
Indices: 0 1 2 3 4 5 ...
Trianges:{0 1 2}
               {3 4 5}
```
The diagram below shows the order in which the triangles will be constructed for a patch of tessellated vertices that has one level of subdivision: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72211149-300cfa00-347b-11ea-805f-488d178b2fdd.png" width="300" height="300">
</p>  
In this scenario, the vertices of triangle 1 would occupy indices 0, 1, and 2; the vertices of triangle 2 would occupy indices 3, 4, and 5; and so on. 

Code Snippet: 
```
void generateVertices()
{
	// Get vertices from tessellating control points
	for (int patchID = 0; patchID < NUM_PATCHES; patchID++)
	{
		float xValues[NUM_CNTRL_PTS_PER_PATCH];
		float yValues[NUM_CNTRL_PTS_PER_PATCH];
		float zValues[NUM_CNTRL_PTS_PER_PATCH];
		for (int vertID = 0; vertID < NUM_CNTRL_PTS_PER_PATCH; vertID++)
		{
			xValues[vertID] = controlPoints[0 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
			yValues[vertID] = controlPoints[1 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
			zValues[vertID] = controlPoints[2 + vertID * NUM_FLOATS_PER_VEC3 + patchID * NUM_CNTRL_PTS_PER_PATCH * NUM_FLOATS_PER_VEC3];
		}

		glm::mat4 P_x = glm::make_mat4(xValues);
		glm::mat4 P_y = glm::make_mat4(yValues);
		glm::mat4 P_z = glm::make_mat4(zValues);

		int iter = 0;
		for (int k = 0; k < LEVEL; k++)
		{
			// Tri formation pattern:
			// Format: (u, v)
			// i = 0: (k, 0) (k, 1) (k + 1, 0)
			//	   +1 +1
			// i = 1: (k + 1, 1) (k, 1) (k + 1, 0)
			//				+0 +2 <- check if v value exceeds limit
			// i = 2: (k + 1, 1) (k, 1) (k + 1, 2)
			//	      -1 +1
			// i = 3: (k, 2) (k, 1) (k + 1, 2)
			//		  +0 +2 <- check if v value exceeds limit
			int u_0 = k;
			int v_0 = 0;
			int u_1 = k;
			int v_1 = 1;
			int u_2 = k + 1;
			int v_2 = 0;
			int counter = 0;
			while (v_1 <= LEVEL && v_2 <= LEVEL)
			{
				// Input vertices for first triangle
				formVertex(patchID, iter, u_0, v_0, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;
				// Input vertices for second triangle
				formVertex(patchID, iter, u_1, v_1, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;
				// Input vertices for third triangle
				formVertex(patchID, iter, u_2, v_2, P_x, P_y, P_z);
				iter += NUM_FLOATS_PER_VEC3;

				if (counter % 4 == 0)
				{
					u_0 += 1;
					v_0 += 1;
				}
				if (counter % 4 == 1)
				{
					v_2 += 2;
				}
				if (counter % 4 == 2)
				{
					u_0 -= 1;
					v_0 += 1;
				}
				if (counter % 4 == 3)
				{
					v_1 += 2;
				}
				counter++;
			}
		}
	}

	// Generate vertex array and buffers
	glGenVertexArrays(1, &verticesVAO);
	glGenBuffers(1, &verticesVBO);
	glBindVertexArray(verticesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, NUM_FLOATS_PER_VEC3, GL_FLOAT, GL_FALSE, NUM_FLOATS_PER_VEC3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
```

</p>
</details>

# 2) Results
## Control Points
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144225-6be9b700-4058-11ea-8d8d-6c12a8eafbbd.jpg" width="400" height="300">
</p>  

## Level 1 Tessellation
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144230-73a95b80-4058-11ea-807c-5a68d195bb39.jpg" width="400" height="300">
</p>  

## Level 2 Tessellation
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144232-7c9a2d00-4058-11ea-810f-be3a0aad3ac6.jpg" width="400" height="300">
</p>  

## Level 5 Tessellation
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144236-858afe80-4058-11ea-99b8-3863a229a47f.jpg" width="400" height="300">
</p>  

## Level 10 Tessellation 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144239-8c197600-4058-11ea-84eb-48111b7f32f4.jpg" width="400" height="300">
</p>  

## Bonus: Some camera work
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/73144538-96893f00-405b-11ea-9f13-6b050037832b.gif" >
</p>  

# Post-Mortem 
- Understood how a pipline works and breaking down the development process into discrete chunks (i.e. data processing, tessellating vertices, constructing triangles)
- Carefully arranged data in the VBO so that the visual output matches expcetations 
- Rigorously tested the functionality of a function before moving onto the next one in order to make the debugging process simpler
