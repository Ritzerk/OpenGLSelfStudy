#version 330 core
layout (location = 0) in vec3 aPos;	//this line sets the generic attribute index (attribindex/generic vertex attribute), which we use when binding and so on. 
layout (location = 2) in vec2 aTxtCoord;

out vec2 TxtCoord;

uniform mat4 transformation;

void main()
{
	gl_Position = transformation * vec4(aPos, 1.0);
	TxtCoord = vec2(aTxtCoord.x, aTxtCoord.y);
}