#version 150 core

in float inValue;
out float geoValue;

void main()
{
	geoValue = sqrt(inValue);
}