// --------------------------
// Vertex Shader
// --------------------------
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;     // 这里的 float 会被传给片段着色器转为 int
layout(location = 4) in float a_TilingFactor; // 纹理重复平铺次数
layout(location = 5) in int a_EntityID;       // 编辑器用的实体 ID

// 对应 C++ 中的 CameraUniformBuffer
layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

// 输出到片段着色器的结构体
struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat float v_TexIndex; // flat 表示不插值，但这在 float 上有时会有精度问题，通常直接传 float 也没事
layout(location = 4) out flat int v_EntityID;   // 传给片段着色器输出到第2个附件

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TilingFactor = a_TilingFactor;
	v_TexIndex = a_TexIndex;
	v_EntityID = a_EntityID;

	// 计算裁剪空间坐标
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

// --------------------------
// Fragment Shader
// --------------------------
#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID; // 输出到第二个颜色附件（用于鼠标拾取）

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat float v_TexIndex;
layout(location = 4) in flat int v_EntityID;

// 对应 C++ 里的 s_Data.TextureSlots
// binding = 0 意味着它会绑定到纹理单元 0-31
layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = vec4(1.0);

	// 核心逻辑：根据 v_TexIndex (0, 1, 2...) 从数组中采样对应的纹理
	texColor *= texture(u_Textures[int(v_TexIndex)], Input.TexCoord * Input.TilingFactor);

	// 最终颜色 = 纹理采样颜色 * 顶点颜色(tint)
	o_Color = texColor * Input.Color;

	// 输出实体 ID 到第二个附件 (EntityID attachment)
	// 只有当你配置了两个颜色附件的 Framebuffer 时这里才有效，否则它会被丢弃，不会报错
	o_EntityID = v_EntityID;
}