#version 330 core
out vec4 FragColour;

in vec3 ourColour;
in vec2 TxtCoord;

uniform sampler2D texture1;	//built in sampler for what we need
uniform sampler2D texture2;

void main()
{
	//FragColour = texture(ourTexture, TxtCoord) * vec4(ourColour, 1.0); //texture function links texture coordinates to the sampler
	FragColour = mix(texture(texture1, TxtCoord), texture(texture2, TxtCoord), 0.2);
}