# raster

图形学学习仓库：从手写软光栅到 OpenGL 实时渲染。

## 结构

Visual Studio 解决方案 `raster.sln`，分成两组工程：

### 软光栅（`raster`）

Win32 窗口 + 自绘帧缓冲（DIB），核心为 `Raster` / `Math` / `Image`，按项目递进：

| 工程 | 内容 |
|------|------|
| `base` | 点、线、三角形、矩形；位图绘制与 alpha |
| `uv` | 顶点 UV / 颜色插值，纹理采样 |
| `3d` | 三维顶点与投影光栅化 |
| `camera` | 相机、鼠标旋转/平移、射线与地面交互 |

### OpenGL（`opengl`）

工程 `learn_opengl`：SDL3 + OpenGL + glm + ImGui + Assimp，包含场景与网格、多种材质（Phong、环境贴图、深度、透明度、屏幕后处理等）、点光/聚光、实例化渲染（含草地）、轨道球与游戏相机控制等。

### 其它目录

- `3rd`：第三方依赖（如 SDL3、glm）
- `res`：软光栅示例资源
- `note`：学习笔记，对应 Markdown：

  | 文件 | 内容 |
  |------|------|
  | [`Essense_of_Linear_Algebra.md`](note/Essense_of_Linear_Algebra.md) | 3Blue1Brown《线性代数的本质》笔记 |
  | [`Calculus.md`](note/Calculus.md) | 微积分（函数、极限、导数等） |
  | [`math_misc.md`](note/math_misc.md) | 数学杂记（e、偏导、链式法则、梯度、泰勒展开等） |
  | [`statistic.md`](note/statistic.md) | 统计（回归、相关系数、协方差） |
  | [`algorithm.md`](note/algorithm.md) | 算法（如线性插值） |
  | [`OpenGL.md`](note/OpenGL.md) | OpenGL / ES、窗口与数学库、坐标系、渲染管线等 |

## 构建

使用 Visual Studio 打开 `raster.sln`，选择对应工程编译运行（建议 x64）。
