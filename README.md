# Overview
A Bézier curve is defined by a set of control points <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> to <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_n">, where <img src="https://render.githubusercontent.com/render/math?math=n"> is the order of the Bézier curve (<img src="https://render.githubusercontent.com/render/math?math=n = 1"> for linear
curves, <img src="https://render.githubusercontent.com/render/math?math=n = 2"> for quadratic curves, etc.). The first and last control points are always at the end of the curves, but intermediate control points are not constrained to the curve. 
## Linear Bézier Curves
A linear Bézier curve is simply a straight line in which linear interpolation occurs between two points, <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> and <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">:  
<p align="center">
<img src="https://user-images.githubusercontent.com/34965351/72015614-f3d95f80-3216-11ea-9714-4c9e70cde675.png" width="250" height="250"> <br>
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = \textbf{P}_0 %2B t(\textbf{P}_1 - \textbf{P}_0) = (1-t)\textbf{P}_0 %2B t\textbf{P}_1, 0 \leq t \leq 1">
</p>  

## Quadratic Bézier Curves 
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

