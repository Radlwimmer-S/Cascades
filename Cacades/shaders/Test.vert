#version 330 core

layout(location = 0) in vec3 inValue;
out vec3 geoValue;

void main()
{
	geoValue = sqrt(inValue);
}