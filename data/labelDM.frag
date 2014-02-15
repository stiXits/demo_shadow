#version 140

uniform sampler2D label;

out vec4 fragColor;
in vec2 v_uv;

float aastep(float threshold, float value)
{

    float diff = clamp(dFdy(value), 0.001, 0.0) + clamp(dFdx(value), 0.001, 0.0) + clamp(dFdy(-value), 0.001, 0.0) + clamp(dFdx(-value), 0.001, 0.0);
    if(value < 0.5 - threshold - diff)
        discard;
    float color = smoothstep(threshold + diff, threshold - diff, value);

	return color;

	// Task_3_2 - ToDo End
}

void main()
{
	// Task_3_2 - ToDo Begin

	float a = texture(label, v_uv).r;
	a = aastep(0.5, a);
	fragColor = vec4(vec3(0.0), -a+1.0);

	// Task_3_2 - ToDo End
}
