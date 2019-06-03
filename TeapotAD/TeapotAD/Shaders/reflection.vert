#version 430 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;


out vec3 normal; //Transformed normal
out vec3 lightPos; //Light position in eye coords
out vec3 Position;




uniform vec3 LightPosition; // Light position 

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
   Position = vec3(M * vec4(VertexPosition, 1.0));
   normal = mat3(transpose(inverse(M))) * VertexNormal;
   lightPos = vec3(V * M * vec4(LightPosition,1.0));  
   gl_Position = P * V * M * vec4(VertexPosition,1.0);
}