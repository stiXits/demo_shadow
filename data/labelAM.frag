#version 140

uniform sampler2D label;

out vec4 fragColor;

in vec2 v_uv;

void main()
{
	// Task_3_1 - ToDo Begin

	// Apply Alpha Mapping to separate the font (white)
	// parts within the label texture from the background.
	// Feel free to modify the color as you like and do
	// not forget to discard fragments that will not be
	// visible at all.

	vec4 tex = texture(label, v_uv);
	if(tex.x < 0.2)
        discard;
	fragColor = vec4(vec3(0.0), tex.x);

	// Task_3_1 - ToDo End
}
