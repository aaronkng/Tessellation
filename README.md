# Overview
A Bézier curve is defined by a set of control points <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> to <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_n">, where <img src="https://render.githubusercontent.com/render/math?math=n"> is the order of the Bézier curve (<img src="https://render.githubusercontent.com/render/math?math=n = 1"> for linear
curves, <img src="https://render.githubusercontent.com/render/math?math=n = 2"> for quadratic curves, etc.). The first and last control points are always at the end of the curves, but intermediate control points are not constrained to the curve. 
## Linear Bézier Curves
A linear Bézier curve is simply a straight line in which linear interpolation occurs between <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_0"> and <img src="https://render.githubusercontent.com/render/math?math=\textbf{P}_1">:  
<img src="https://render.githubusercontent.com/render/math?math=\textbf{B}(t) = \textbf{P}_0 %2B t(\textbf{P}_1 - \textbf{P}_0) = (1-t)\textbf{P}_0 %2B t\textbf{P}_1, 0 \leq t \leq 1">
