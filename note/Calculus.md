- [函数](#函数)
    - [恒等函数](#恒等函数)
    - [二次函数](#二次函数)
    - [正弦函数](#正弦函数)
    - [反函数](#反函数)
    - [函数奇偶性](#函数奇偶性)
    - [线性函数](#线性函数)
    - [多项式函数](#多项式函数)
    - [指数函数](#指数函数)
    - [对数函数](#对数函数)
    - [对数函数](#对数函数)
    - [绝对值函数](#绝对值函数)
    - [三角函数](#三角函数)
        - [弧度](#弧度)
        - [三角函数图像](#三角函数图像)
        - [三角函数差角公式](#三角函数差角公式)
        - [三角函数和角公式](#三角函数和角公式)
        - [三角函数倍角公式](#三角函数倍角公式)
    - [极限](#极限)
        - [极限的直观理解](#极限的直观理解)
        - [极限的函数例子](#极限的函数例子)
        - [双侧极限不存在](#双侧极限不存在)
        - [极限不存在例子](#极限不存在例子)

## 函数

![alt text](img/calculus_function1.png)

![alt text](img/calculus_function2.png)

![alt text](img/calculus_function3.png)

![alt text](img/calculus_function7.png)

实数（Real Numbers）：所有可以在数轴（一条无限延伸的直线）上表示出来的数。看得见、摸得着，能对应现实世界中的物理量（如长度、温度、重量、时间）。例如：$3$ 米、$1.5$ 斤、$-5$ ℃。

虚数（Imaginary Numbers）：在现实中找不到直接对应的实物，是数学家为了解决特定方程（比如 $x^2 = -1$）而创造出来的“想象中的数”，用 $i$ 表示（其中 $i = \sqrt{-1}$）。

在数学的大家庭里，“实数”和“虚数”共同组成了更庞大的复数（Complex Numbers）。

![alt text](img/calculus_function8.png)

![alt text](img/calculus_function9.png)

![alt text](img/calculus_function10.png)

### 恒等函数

![alt text](img/calculus_function4.png)

### 二次函数

![alt text](img/calculus_function5.png)

### 正弦函数

![alt text](img/calculus_function6.png)

### 反函数

![alt text](img/calculus_function11.png)

![alt text](img/calculus_function12.png)

![alt text](img/calculus_function13.png)

![alt text](img/calculus_function14.png)

原函数图像是由无数个点 $(a, b)$ 组成的。

反函数图像是由对应的无数个点 $(b, a)$ 组成的。

每一对点 $(a, b)$ 与 $(b, a)$ 都关于直线 $y=x$ 对称。

![alt text](img/calculus_function15.png)

![alt text](img/calculus_function16.png)

![alt text](img/calculus_function17.png)

![alt text](img/calculus_function18.png)

![alt text](img/calculus_function19.png)

### 函数奇偶性

![alt text](img/calculus_function20.png)

![alt text](img/calculus_function21.png)

![alt text](img/calculus_function22.png)

![alt text](img/calculus_function23.png)

### 线性函数

![alt text](img/calculus_function24.png)

斜率 = 向右移动 1 个单位时，y 方向变化多少

斜率 = Δy / Δx

![alt text](img/calculus_function25.png)

![alt text](img/calculus_function26.png)

![alt text](img/calculus_function27.png)

![alt text](img/calculus_function28.png)

### 多项式函数

线性函数是多项式函数的一种特殊形式。你可以把多项式函数想象成一个“大家族”，而线性函数只是这个家族里最简单、最规矩的那一类。

![alt text](img/calculus_function29.png)

![alt text](img/calculus_function30.png)

### 有理函数

实数分成两大阵营：有理数和无理数。

有理数（Rational Numbers）：可以用分数形式 $\frac{p}{q}$（$p, q$ 为整数，且 $q \neq 0$）表示的数。

无理数（Irrational Numbers）：不能写成分数形式的数。它们是无限不循环小数——小数点后面有无穷无尽的数字，而且毫无循环规律。

圆周率 $\pi \approx 3.1415926...$

自然常数 $e \approx 2.71828...$

![alt text](img/calculus_function31.png)

![alt text](img/calculus_function32.png)

![alt text](img/calculus_function33.png)

![alt text](img/calculus_function34.png)

![alt text](img/calculus_function35.png)

![alt text](img/calculus_function36.png)

![alt text](img/calculus_function37.png)

### 指数函数

这两个约束条件 a > 0​ 和 a ≠ 1​ 缺一不可，各有其深刻的数学原因。

如果 $a < 0$（比如 $a = -2$）

此时函数为 $y = (-2)^x$。当自变量 $x$ 在实数集 $\mathbb{R}$ 内连续变化时：

如果 $x = 2$，那么 $y = (-2)^2 = 4$（有意义）

如果 $x = 3$，那么 $y = (-2)^3 = -8$（有意义）

如果 $x = \frac{1}{2}$，那么 $y = (-2)^{\frac{1}{2}} = \sqrt{-2}$。在实数范围内，负数不能开偶次方根，所以此时 $y$ 变成了虚数 $\sqrt{2}i$（$i = \sqrt{-1}$），在实数轴上根本找不到对应的点！

如果 $x = \frac{1}{3}$，那么 $y = (-2)^{\frac{1}{3}} = -\sqrt[3]{2}$（又是实数了）, $\sqrt[奇数]{-A} = -\sqrt[奇数]{A}$

由此可见，当 $a < 0$ 时，只要 $x$ 的分母是偶数（比如 $\frac{1}{2}, \frac{1}{4}, \frac{3}{8}$ 等），函数值就会瞬间脱离实数范围。这会导致函数的图像变成无数个孤立的点，根本无法连成一条连续的曲线，微积分等高级数学工具也就彻底失效了。



如果 $a = 0$

此时函数为 $y = 0^x$。

当 $x > 0$ 时， $y = 0$ 恒成立。

当 $x \le 0$ 时（比如 $x = -1$），$y = 0^{-1} = \frac{1}{0}$，分母为 $0$，数学上未定义。

这样半边有意义、半边无意义且毫无“指数爆炸增长”特征的函数，没有单独作为指数函数研究的必要。

结论：为了让 $y = a^x$ 在整个实数集 $\mathbb{R}$ 上都有定义，并且能画出一条连续、平滑的曲线，底数 $a$ 必须大于 $0$。



如果 $a = 1$

那么函数就变成了：$y = 1^x$

因为 $1$ 的任何次方都等于 $1$，所以无论 $x$ 是什么实数，$y$ 都永远等于 $1$。它的图像就是一条水平的直线 $y = 1$。

这本质上是一个常数函数（也是一种特殊的线性函数），它不具备指数函数“急剧增长”或“急剧衰减”的任何动力学特性。

更致命的是，因为 $y=1^x$ 的因变量永远是 $1$，它没有一一对应关系，因此无法定义它的反函数（也就是对数函数 $\log_1 x$ 将无从谈起，因为分母不能为 $\log 1 = 0$）。

为了不让指数函数退化成平平无奇的常数函数，数学家决定把 $a = 1$ 排除在外。

![alt text](img/calculus_function38.png)

### 对数函数

![alt text](img/calculus_function39.png)

![alt text](img/calculus_function40.png)

![alt text](img/calculus_function41.png)

![alt text](img/calculus_function42.png)

蓝色曲线（当 $a > 1$ 时）—— 指数增长

橙色曲线（当 $0 < a < 1$ 时）—— 指数衰减

![alt text](img/calculus_function43.png)

### 绝对值函数

![alt text](img/calculus_function44.png)

![alt text](img/calculus_function45.png)

![alt text](img/calculus_function46.png)

![alt text](img/calculus_function47.png)

![alt text](img/calculus_function48.png)

### 三角函数

#### 弧度

当一段弧的长度（弧长 $l$）正好等于圆的半径（$r$）时，它所对应的圆心角 $\theta$ 的大小，就是“1 弧度”。

![alt text](img/calculus_radian1.png)

![alt text](img/calculus_radian2.png)

在半径为 $1$ 的“单位圆”中，圆心角的弧度值和它所对应的弧长，在数值上是完全相等的（即 $l = \theta$）

![alt text](img/calculus_radian3.png)

![alt text](img/calculus_radian4.png)

#### 三角函数定义

![alt text](img/calculus_trigonometric1.png)

![alt text](img/calculus_trigonometric2.png)

![alt text](img/calculus_trigonometric3.png)

![alt text](img/calculus_trigonometric4.png)

在三角函数中，无论一个角 $\theta$ 有多大（在第二、三、四象限），它与 $x$ 轴 夹出来的那个 锐角（$0^\circ \sim 90^\circ$ 之间），就叫做这个角的参考角

![alt text](img/calculus_trigonometric5.png)

![alt text](img/calculus_trigonometric6.png)

![alt text](img/calculus_trigonometric7.png)

#### 三角函数图像

![alt text](img/calculus_trigonometric_cyclicity1.png)

![alt text](img/calculus_trigonometric_cyclicity2.png)

![alt text](img/calculus_trigonometric_cyclicity3.png)

![alt text](img/calculus_trigonometric_cyclicity4.png)

#### 三角函数诱导公式

![alt text](img/calculus_trigonometric_induction1.png)

![alt text](img/calculus_trigonometric_induction2.png)

![alt text](img/calculus_trigonometric_induction3.png)

![alt text](img/calculus_trigonometric_induction4.png)

#### 三角函数差角公式

![alt text](img/calculus_trigonometric_differenceformula1.png)

![alt text](img/calculus_trigonometric_differenceformula2.png)

![alt text](img/calculus_trigonometric_differenceformula3.png)

![alt text](img/calculus_trigonometric_differenceformula4.png)

![alt text](img/calculus_trigonometric_differenceformula5.png)

![alt text](img/calculus_trigonometric_differenceformula6.png)

![alt text](img/calculus_trigonometric_differenceformula7.png)

![alt text](img/calculus_trigonometric_differenceformula8.png)

#### 三角函数和角公式

![alt text](img/calculus_trigonometric_sumformula1.png)

![alt text](img/calculus_trigonometric_sumformula2.png)

![alt text](img/calculus_trigonometric_sumformula3.png)

![alt text](img/calculus_trigonometric_sumformula4.png)

![alt text](img/calculus_trigonometric_sumformula5.png)


#### 三角函数倍角公式

![alt text](img/calculus_trigonometric_doubleformula1.png)

![alt text](img/calculus_trigonometric_doubleformula2.png)

![alt text](img/calculus_trigonometric_doubleformula3.png)

![alt text](img/calculus_trigonometric_doubleformula4.png)

![alt text](img/calculus_trigonometric_doubleformula5.png)

![alt text](img/calculus_trigonometric_doubleformula6.png)

![alt text](img/calculus_trigonometric_doubleformula7.png)

![alt text](img/calculus_trigonometric_doubleformula8.png)

![alt text](img/calculus_trigonometric_doubleformula9.png)

## 极限

### 极限的直观理解

![alt text](img/calculus_limitIntuitive1.png)

![alt text](img/calculus_limitIntuitive2.png)

让P点在抛物线上滑动，不断的向Q点靠近，**割线PQ**的斜率在不断地变化，P距离Q越近，割线的斜率就越靠近一个特定的值，当P无限趋近Q时，割线的斜率就会无限趋近于Q点的切线的斜率。

**这个无限趋近但不等于的过程，就是极限的直观含义。**

![alt text](img/calculus_limitIntuitive3.png)

### 极限的函数例子

![alt text](img/calculus_limitfunctionexample1.png)

**极限的数学符号**

若当自变量 $x$ 趋近于常数 $x_0$ 时，函数 $f(x)$ 趋近于一个确定的常数 $L$，我们记作：

$$\lim_{x \to x_0} f(x) = L$$

或者写作：

$$f(x) \to L \quad (\text{当 } x \to x_0 \text{ 时})$$

$\lim$：是英文 Limit（极限）的缩写。

$x \to x_0$：表示自变量 $x$ 无限趋近于 $x_0$，但 $x \neq x_0$。

$L$：是函数在该点处的极限值。

![alt text](img/calculus_limitfunctionexample2.png)

![alt text](img/calculus_limitfunctionexample3.png)

![alt text](img/calculus_limitfunctionexample4.png)

![alt text](img/calculus_limitfunctionexample5.png)

左极限，右极限

![alt text](img/calculus_limitfunctionexample6.png)

![alt text](img/calculus_limitfunctionexample7.png)

### 双侧极限不存在

![alt text](img/calculus_limitfunctionexample8.png)

![alt text](img/calculus_limitfunctionexample9.png)

### 极限不存在例子

![alt text](img/calculus_nolimitexample1.png)

![alt text](img/calculus_nolimitexample2.png)

在图像中，左右两边最外侧的“波峰”（高度为 $1$）和“波谷”（高度为 $-1$）对应的 $x$ 坐标分别是：

$$\frac{1}{x} = \frac{\pi}{2} \implies x = \frac{2}{\pi} \approx 0.637$$

$$\frac{1}{x} = -\frac{\pi}{2} \implies x = -\frac{2}{\pi} \approx -0.637$$

在正实数范围内，要让正弦值等于 $1$，括号里的“角度” $\frac{1}{x}$ 最小也必须达到 $\frac{\pi}{2}$：

$$\frac{1}{x} \ge \frac{\pi}{2}$$

现在，我们把这个不等式两边取倒数（因为两边都是正数，取倒数要改变不等号方向）：

$$x \le \frac{2}{\pi}$$

这个简单的代数推导给出了一个硬性铁律：只有当 $x \le \frac{2}{\pi}$ 时，函数值才有机会达到 $1$。

![alt text](img/calculus_nolimitexample3.png)

![alt text](img/calculus_nolimitexample4.png)

$(k \in \mathbb{Z})$ 在数学语言里表示 “$k$ 是一个整数”。

我们希望找一堆特殊的 $x$ 坐标，让函数值在这些点上全部等于 1（波峰）。

1. 寻找角度公式

要让正弦值等于 $1$，角度必须满足：

$$\theta = \frac{\pi}{2} + 2k\pi \quad (k \in \mathbb{Z})$$

2. 对应到我们的自变量 $x$

因为函数是 $\sin(1/x)$，所以令括号里的角度 $\frac{1}{x} = \frac{\pi}{2} + 2k\pi$（这就是图片左上角第一步）：


$$\text{令 } \frac{1}{x} = \frac{\pi}{2} + 2k\pi$$

3. 解出这些特殊的 $x$ 坐标（记为 $x_k$）

把等式两边取倒数，得到第 $k$ 个波峰对应的 $x$ 坐标（图片左侧第二步）：


$$x_k = \frac{1}{\frac{\pi}{2} + 2k\pi}$$

4. 让这些点向 $0$ 靠拢

现在，我们让 $k$ 越来越大，大到无穷大（$k \to \infty$）。
因为分母上有 $2k\pi$，分母会爆炸式变大，所以这些坐标值 $x_k$ 必然无限趋近于 $0$（图片左下角第三步）：


$$\text{当 } k \to \infty \text{ 时}，x_k \to 0$$

5. 计算这一路上的函数值极限

无论 $k$ 是多少，由于我们选的 $x_k$ 刚好都是波峰位置，所以代入函数后，所有的值都雷打不动地等于 $1$（图片右侧公式）：


$$\sin\left(\frac{1}{x_k}\right) = \sin\left(\frac{\pi}{2} + 2k\pi\right) = 1 \quad (\forall k)$$

因此，当这群点 $x_k$ 趋近于 $0$ 时，它们的函数值极限也必然是 $1$：


$$\lim_{k \to \infty} \sin\left(\frac{1}{x_k}\right) = 1$$

![alt text](img/calculus_nolimitexample5.png)

![alt text](img/calculus_nolimitexample6.png)

左右极限是“左边一路人马，右边一路人马，看两路人马是否在终点相遇”。

子序列法则是在“单侧（甚至全在右边）派出多路人马，结果发现光是右边的队伍自己内部就打起来了，各执一词”。

![alt text](img/calculus_nolimitexample7.png)