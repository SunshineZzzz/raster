#version 460 core
out vec4 FragColor;
in vec2 uv;
in vec3 normal;
uniform float near;
uniform float far;
void main()
{
	// 将深度值[0-1]映射成物体真实和摄像机的距离
	float Zndc = gl_FragCoord.z * 2.0 - 1.0;
	float linearDepth = 2.0 * near / (far + near - Zndc *(far - near));

	// 距离近，黑色，灰色，白色，最远
	vec3 finalColor = vec3(linearDepth);
 
	FragColor = vec4(finalColor, 1.0);
}