#version 330 core
layout (location = 0) in vec3 aPos;								//this line sets the generic attribute index (attribindex/generic vertex attribute), which we use when binding and so on. 
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTxtCoord;

out vec3 ourColour;
out vec2 TxtCoord;

void main()
{
	
	gl_Position = vec4(aPos, 1.0);
	ourColour = aColour;	
	TxtCoord = aTxtCoord;
	//ourColour = aColour;
}