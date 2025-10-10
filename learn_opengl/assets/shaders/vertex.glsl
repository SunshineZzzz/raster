#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float time;
out vec3 color;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   // 根据每一帧的传入的时间，计算出一个缩放因子，缩放因子*RGB，从而实现颜色饱和度变化
   color = aColor * (cos(time) + 1.0) / 2.0;
}