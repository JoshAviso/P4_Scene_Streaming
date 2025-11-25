#version 330 core

in vec3 fragPos;
in vec3 normCoord;
in vec2 texCoord; 

uniform vec3 base_color;

out vec4 FragColor; 

void main() {
	FragColor = vec4(base_color, 1.0);
}