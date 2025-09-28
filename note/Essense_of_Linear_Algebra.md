3Blue1Brown视频[《线性代数的本质》](https://www.bilibili.com/video/BV1ys411472E)的笔记。从空间变化理解线性代数。

- [向量](#向量)
  - [向量的几何意义](#向量的几何意义)
  - [向量的加法](#向量的加法)
  - [向量的数乘](#向量的数乘)
  - [向量的线性组合](#向量的线性组合)
  - [向量张成的空间](#向量张成的空间)
  - [线性变换](#线性变换)
- [矩阵](#矩阵)
  - [复合变换](#复合变换)
  - [矩阵乘法](#矩阵乘法)

### 向量

三种不同视角看待向量：
1. 物理学视角：空间中的**箭头**，由长度和方向决定一个向量(arrows pointing in space)，向量平移不变
2. 数学视角：向量(a vector can be anything)，只要能保证相加和与数字相乘是有意义的即可，太抽象了
3. 计算机视角：向量等同于有序的数字**列表**(ordered list of numbers)
   
#### 向量的几何意义

和物理专业的看法有一定出入的是，线性代数中的**向量往往以坐标原点起始**。

![alt text](img/vector_sense1.png)

#### 向量的加法

![alt text](img/vector_plus1.png)

![alt text](img/vector_plus2.png)

以$\vec{v} + \vec{w}$为例，即将$\vec{v}$平移，使其起点对准$\vec{w}$的终点，最终画一条从$\vec{v}$起点指向$\vec{w}$终点的向量

$\left[\begin{array}{c} 1 \\ 2 \end{array}\right] + \left[\begin{array}{c} 3 \\ -1 \end{array}\right] = \left[\begin{array}{c} 4 \\ 1 \end{array}\right]$

#### 向量的数乘

![alt text](img/vector_mul1.png)

![alt text](img/vector_mul2.png)

$2 \cdot \left[\begin{array}{c} 3 \\ 1 \end{array}\right] = \left[\begin{array}{c} 6 \\ 2 \end{array}\right]$

#### 向量的线性组合

![alt text](img/vector_linear_combination2.png)

在xy坐标系中有两个特殊含义的向量

1. x轴上的单位向量$\hat{i}$ ，也被叫做i帽(i-hat)      
2. y轴上的单位向量$\hat{j}$ ，也被叫做j帽(j-hat)
  
这些单位向量被叫做坐标系的基向量

![alt text](img/vector_linear_combination1.png)

![alt text](img/vector_linear_combination3.png)

把向量看成是基向量**缩放后相加**的结果

$\left[\begin{array}{c} 3 \\ -2 \end{array}\right] = 3 \cdot \hat{i} - 2 \cdot \hat{j}$

这种向量的**缩放后相加**实际上就是**线性组合**(Linear Combination)。也就是说，向量可以理解成基向量的线性组合。

#### 向量张成的空间

![alt text](img/vector_linear_span1.png)

1. 一组基向量的线性组合所能到达的点的集合就是该组基向量**张成的空间**(Span)。
2. 在二维情况下，只要基向量不共线，它们张成的空间就覆盖平面上所有的点。
3. 如果基向量共线，它们张成的空间就只有一条线；
4. 如果基向量都是零向量，它们张成的空间就只有一个点。

#### 线性变换

线性变换(Linear Transformation): 接受一个向量并输出一个向量的变换。
1. 线性(Linear)指空间中的所有直线在变换后仍然是直线，且原点的位置没有发生改变，并且网格线平行且等距分布。
2. 变换(Transformation)本质上是函数(function)，但与函数不同的是，变换暗示你可以用可视化的运动来思考这个过程。
   
比如下面就不是:

![alt text](img/linear_transformation1.png)

在线性变换中，只要确定基向量的变换后位置，就能确定其他所有向量的变换后位置。

![alt text](img/linear_transformation2.png)

举例来说，对于向量

$
\vec{v} = \left[\begin{array}{c} x \\ y \end{array}\right] = x \cdot \hat{i} + y \cdot \hat{j}
$

变化后

$
\vec{v'} = x \cdot \hat{i'} + y \cdot \hat{j'}
$

例如:

$
\hat{i'} = \begin{bmatrix} 1 \\ -2 \end{bmatrix}, \quad \hat{j'} = \begin{bmatrix} 3 \\ 0 \end{bmatrix}
$

得出:

$
\begin{aligned}
\vec{v'} &= x \cdot \hat{i'} + y \cdot \hat{j'} \\
&= x \begin{bmatrix} 1 \\ -2 \end{bmatrix} + y \begin{bmatrix} 3 \\ 0 \end{bmatrix} \\
&= \begin{bmatrix} x + 3y \\ -2x \end{bmatrix}
\end{aligned}
$

把基向量变换后的向量作为列排成 $2 \times 2$ 的矩阵 $\begin{bmatrix} 1 & 3 \\ -2 & 0 \end{bmatrix}$，此时矩阵乘以向量 $\vec{v} = \begin{bmatrix} x \\ y \end{bmatrix}$，就是

$
\begin{bmatrix} 1 & 3 \\ -2 & 0 \end{bmatrix}
\begin{bmatrix} x \\ y \end{bmatrix}
= \begin{bmatrix} x + 3y \\ -2x \end{bmatrix}
= \vec{v'}
$

**矩阵本质上是对空间操纵的描述，用于定义一个线性变换的函数**。

**矩阵中的各列依次代表线性变换中，各基向量在变换后的结果(位置)**。矩阵对应的线性变换将**原基向量张成的空间**映射(求函数)到**变换后基向量张成的空间**。

### 矩阵

#### 复合变换

矩阵是对空间线性变换的描述函数，依次两个线性变换称为**复合变换**(Composition of Transformations)。

一个向量依次经历两个矩阵的线性变换得到的结果，等价于其经过一个复合变换的结果，如下图所示:

![alt text](img/compose_transformation1.png)

![alt text](img/compose_transformation2.png)

#### 矩阵乘法

我的理解，i帽和j帽经过M1线性变化后对应的是M1矩阵第一列和M2矩阵线性变化第二列，同理，再次经过M2线性变化，i帽撇为:

![alt text](img/matrix_multiplication1.png)

j帽撇为:

![alt text](img/matrix_multiplication2.png)

性质:

1. 矩阵乘法不满足交换律，即 $AB \neq BA$

![alt text](img/matrix_multiplication3.png)

![alt text](img/matrix_multiplication4.png)

2. 矩阵乘法满足结合律，即 $(AB)C = A(BC)$
  
**只从线性变换的角度上思考，两个公式本质上都描述了先进行C变换，再进行B变换，最后进行A变换，因此没有区别**。

![alt text](img/matrix_multiplication5.png)

