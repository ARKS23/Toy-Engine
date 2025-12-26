#type vertex
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
    TexCoords = aTexCoords;
    gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
} 

#type fragment
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D u_Texture;

void main() {
    // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = texture(u_Texture, TexCoords);
}


