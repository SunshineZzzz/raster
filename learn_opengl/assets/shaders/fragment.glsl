#version 460 core
out vec4 FragColor;
in vec3 color;
void main()
{
    vec3 finalColor = color;
    if (length(color) < 0.001) 
    {
        finalColor = vec3(1.0f, 0.0f, 1.0f); 
    }
    FragColor = vec4(finalColor, 1.0f);
}