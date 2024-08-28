#version 330
layout (location =0) in vec3 pos;
out vec4 vColor;
uniform mat4 model;
uniform mat4 projection;
void main()
{
	gl_Position=projection*model*vec4(pos,1.0f);
	//vColor=vec4(color,1.0f);
	//Por el momento no calculamos color
	//vColor=vec4(clamp(pos,0.0f,1.0f),1.0f);
}