#version 430 core
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) out float density;

in GS_OUT
{
	vec3 ws;
} fs_in;

struct Pillar
{
	vec2 position;
	float frequence;
	float weight;
};

struct Bound
{
	float weight;
};

struct Helix
{
	float offset;
	float frequence;
	float weight;
};

struct Shelf
{
	float offset;
	float frequence;
	float weight;
};

const int PILLAR_COUNT = 4;

uniform Pillar pillars[PILLAR_COUNT];
uniform Bound bound;
uniform Helix helix;
uniform Shelf shelf;

vec2 rotate(vec2 v, float a) 
{
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

float AddPillar(vec3 ws, Pillar pillar)
{
	float pillarAngle = pillar.frequence * ws.y * M_PI;
	vec2 position = rotate(pillar.position, pillarAngle);
	return pillar.weight * (1.0f / length(ws.xz - position) - 1.0f);
}

float AddBounds(vec3 ws, Bound bound)
{
	return bound.weight * pow(length(ws.xz), 3);
}

float AddHelix(vec3 ws, Helix helix)
{
	float helixAngle = helix.offset + helix.frequence * ws.y * M_PI;
	float sinLayer = sin(helixAngle);
	float cosLayer = cos(helixAngle);
	return helix.weight * dot(vec2(cosLayer, sinLayer), ws.xz);
}

float AddShelf(vec3 ws, Shelf shelf)
{
	float shelfAngle = shelf.offset + shelf.frequence * ws.y * M_PI;
	float cosLayer = cos(shelfAngle);
	return shelf.weight * cosLayer;
}

void main()
{
	density = 0;
	for (int p = 0; p < PILLAR_COUNT; ++p)
	{
		density += AddPillar(fs_in.ws, pillars[p]);
	}
	density += AddBounds(fs_in.ws, bound);
	density += AddHelix(fs_in.ws, helix);
	density += AddShelf(fs_in.ws, shelf);
}