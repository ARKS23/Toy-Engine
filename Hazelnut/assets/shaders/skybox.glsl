#type vertex
#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TexCoords = aPos; // 用于立方体贴图采样
    vec4 pos = u_Projection * mat4(mat3(u_View)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // 天空盒深度永远1.0
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube u_SkyboxSampler;

void main()
{    
    FragColor = texture(u_SkyboxSampler, TexCoords);
}

