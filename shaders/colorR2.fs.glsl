#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

in vec3 vColor;

out vec3 fFragColor;

void main()
{
    fFragColor = vColor * 0.8;
}
