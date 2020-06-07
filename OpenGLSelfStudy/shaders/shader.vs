#version 330 core
layout (location = 0) in vec3 aPos;								//this line sets the generic attribute index (attribindex/generic vertex attribute), which we use when binding and so on. 
layout (location = 1) in vec3 aColour;
out vec3 ourColour;
void main()
{
	gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
	ourColour = aColour;
}