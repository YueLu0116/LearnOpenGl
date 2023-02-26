#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec2 flipTexCoord = vec2(TexCoord.x, TexCoord.y);
	// linearly interpolate between both textures (80% texture1, 20% texture2)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, flipTexCoord), 0.2);
}