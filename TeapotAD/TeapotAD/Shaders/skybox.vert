#version 430


layout (location = 0) in vec3 vertex_position;

out vec3 texture_coordinates;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;


void main() 
{
	gl_Position = projection_matrix * view_matrix * vec4(vertex_position , 1.0);
	texture_coordinates = vertex_position;
}