#version 140

// Task_2_2 - ToDo Begin

uniform mat4 vpi;
uniform mat4 transform;
// ...

in vec3 a_vertex;

out vec3 v_normal;
out vec3 v_eye;

void main()
{
	v_normal = a_vertex;

	// ToDo: Retrive the eye vector and pass to next stage

    v_eye = (vpi * vec4(a_vertex, 2.0)).xyz;
	//v_eye = ...;
	gl_Position = transform * vec4(a_vertex, 1.0);
}

// Task_2_2 - ToDo End.
