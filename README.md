# 0) Overview
A Bézier curve is defined by a set of control points <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> to <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_n">, where <img src="https://render.githubusercontent.com/render/math?math=n"> is the order of the Bézier curve (<img src="https://render.githubusercontent.com/render/math?math=n = 1"> for linear
curves, <img src="https://render.githubusercontent.com/render/math?math=n = 2"> for quadratic curves, etc.). The first and last control points are always at the end of the curves, but intermediate control points are not constrained to the curve. 
## 0.0) Linear Bézier Curves
A linear Bézier curve is simply a straight line in which linear interpolation occurs between two points, <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> and <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">:  
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72015614-f3d95f80-3216-11ea-9714-4c9e70cde675.png" width="250" height="250"> <br>
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = \textbf{P}_0 %2B t(\textbf{P}_1 - \textbf{P}_0) = (1-t)\textbf{P}_0 %2B t\textbf{P}_1, 0 \leq t \leq 1">
</p>  

## 0.1) Quadratic Bézier Curves 
A quadratic Bézier curve is a path traced by the function <img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t)">, given points <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0">, <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">, and <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2">: 
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = (1 - t)[(1-t)\textbf{P}_0 %2B t\textbf{P}_1] %2B t[(1-t)\textbf{P}_1 %2B t\textbf{P}_2], 0 \leq t \leq 1">
</p>  

This is essentially the linear interpolant of corresponding points on the linear Bézier curves from <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> to <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1"> and from <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1"> to <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2">. We can rearrange the equation to be: 
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = (1-t)^2\textbf{P}_0 %2B 2(1-t)t\textbf{P}_1 %2B t^2\textbf{P}_2">
</p>  

As <img src="https://render.githubusercontent.com/render/math?math=t"> increases from 0 to 1, the curve departs from <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> in the direction of <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">, then bends to arrive at <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2"> from the direction of <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">. 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72016266-51ba7700-3218-11ea-93fb-578be7213b90.jpg"> 
</p> 

## 0.2) Cubic Bézier Curves
Four points  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0">,  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">,  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2">, and  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_3"> define a cubic Bézier curve. The curve starts at  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> going toward  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1"> and arrives at  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_3"> coming from  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2">. The curve usually does not pass through  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1"> and  <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_2"> as these points are only there to provide directional infomration. 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72017531-ba0a5800-321a-11ea-8608-edc9880e3132.jpg"> 
</p>  

The cubic Bézier curve can be defined as the combinaton of two quadratic Bézier curves:
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = (1-t)\textbf{B}_{\textbf{P}_0,\textbf{P}_1,\textbf{P}_2}(t) %2B t\textbf{B}_{\textbf{P}_1,\textbf{P}_2,\textbf{P}_3}(t), 0 \leq t \leq 1">
</p>  
This can be rewritten as:
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = (1-t)^3\textbf{P}_0 %2B 3(1-t)^2t\textbf{P}_1 %2B 3(1-t)(t^2\textbf{P}_2 %2B t^3\textbf{P}_3, 0 \leq t \leq 1">
</p>  
The constants that precede each point are the values of the binomial coefficient. 

## 0.3) Bézier Surface 
A Bézier surface of degree <img src="https://render.githubusercontent.com/render/math?math=(n, m)"> is defined by a set of <img src="https://render.githubusercontent.com/render/math?math=(n %2B 1)(m %2B 1)"> control points <img src="https://render.githubusercontent.com/render/math?math=\textbf{k}_{ij}">. A two-dimensional Bézier surface can be defined as a parametric surface where the position of a point <img src="https://render.githubusercontent.com/render/math?math=p"> is the function of the parametric coordinates <img src="https://render.githubusercontent.com/render/math?math=u, v">: 
<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\textbf{p}(u, v) = \sum_{i=0}^n \sum_{j=0}^m B_i^n(u) B_j^m(v) \textbf{k}_{ij}">
</p>  

<img src="https://render.githubusercontent.com/render/math?math=B_i^n(u) = n \choose i u^i(1-u)^{n-i}"> is the Bernstein polynomial and <img src="https://render.githubusercontent.com/render/math?math=n \choose i = \frac{n!}{i!(n-i)!}"> is the binomial coefficient. Typically, Bézier surfaces are constrained by the convex hull of its control points. Furthermore, the most common use of Bézier surfaces are as nets of **bicubic patches** (<img src="https://render.githubusercontent.com/render/math?math=m = n =3">); this means that a single bicubic patch consists of 16 control points. 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72022702-f42d2700-3225-11ea-8307-7f555e51aac1.gif">
</p>  

# 1) Implementation
## 1.0) Retrieving data from `data.txt`
The first line in `data.txt` represents the number of patches. Patches are formatted as such: 
```
(u,v)
Control point 0
Control point 1
...
Control point 15
```
Source for control points of Utah Teapot is [here](http://www.holmes3d.net/graphics/teapot/). 
## 1.1) Tessellating control patches 
The control points received from `data.txt` act as the skeletal structure for the patch, and in this portion of the implementation, we are constructing the surface of the patch by specifying the level of subdivisions we want and inputting the <img src="https://render.githubusercontent.com/render/math?math=(u, v)"> values into a function.  
  
If we have 0 levels of subdivision, then that means there are 0 subdivisions along the u-axis and 0 subdivisions along the v-axis, as shown in the diagram below: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72104550-88f45b00-32e0-11ea-9e79-8a9c8fa7f198.png" width="300" height="300">
</p>  

From here, we have 4 tessellated vertices to compute: <img src="https://render.githubusercontent.com/render/math?math=p(0, 0)">, <img src="https://render.githubusercontent.com/render/math?math=p(1, 0)">, <img src="https://render.githubusercontent.com/render/math?math=p(0, 1)">, and <img src="https://render.githubusercontent.com/render/math?math=p(1, 1)">.  
  
If we have 1 level of subdivision, then that means there are 1 subdivision along the u-axis and 1 subdivision along the v-axis, as shown in the diagram below: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72105867-323c5080-32e3-11ea-872d-13f05050ffca.png" width="300" height="300">
</p>  

From here, we have 9 tessellated vertices to compute: <img src="https://render.githubusercontent.com/render/math?math=p(0, 0)">, <img src="https://render.githubusercontent.com/render/math?math=p(0, \frac{1}{2})">, <img src="https://render.githubusercontent.com/render/math?math=p(0, 1)">, <img src="https://render.githubusercontent.com/render/math?math=p(\frac{1}{2}, 0)">, <img src="https://render.githubusercontent.com/render/math?math=p(\frac{1}{2}, \frac{1}{2})">, <img src="https://render.githubusercontent.com/render/math?math=p(\frac{1}{2}, 1)">, <img src="https://render.githubusercontent.com/render/math?math=p(1, 0)">, <img src="https://render.githubusercontent.com/render/math?math=p(1, \frac{1}{2})">, and <img src="https://render.githubusercontent.com/render/math?math=p(1, 1)">.  
  
Essentially, the number of subdivided polygons we receive is (level of subdivision + 1)<sup>2</sup>. Additionally, the number of tessellated vertices to compute is (level of subdivision + 2)<sup>2</sup>.  
  
Once we have the set of tessellated vertices, we need to compute each vertex using the parametric surface equation (section 0.3) via matrix multiplication: 
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72116169-3c1f7d00-32fe-11ea-92b7-9d927233f71c.png">
</p>  

To see how the matrix was computed, click 
[here](https://github.com/aaronkng/Tessellation/blob/master/Tessellation_Matrix.pdf).
  
## 1.2) Constructing triangle strips for each patch
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
In this scenario, the vertices of triangle 1 would occupy indices 0, 1, and 2; the vertices of triangle 2 would occupy indices 3, 4, and 5; and so on. We can optimize our data management by finding a way where the vertices of contiguous triangles are not stored twice into the vertex buffer object (i.e. the diagonal vertices of triangle 1 and 2 are stored twice). 

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
