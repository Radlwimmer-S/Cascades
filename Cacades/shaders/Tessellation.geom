#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 tePosition[3];
in vec3 tePatchDistance[3];
out vec3 gPosition;
out vec3 gFacetNormal;
out vec3 gPatchDistance;
out vec3 gTriDistance;

uniform mat4 model;

void main()
{
    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = - normalize(cross(A, B));

    gPatchDistance = tePatchDistance[0];
    gTriDistance = vec3(1, 0, 0);
    gPosition = vec3(model * vec4(tePosition[0], 1.0f));
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gTriDistance = vec3(0, 1, 0);
    gPosition = vec3(model * vec4(tePosition[1], 1.0f));
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gTriDistance = vec3(0, 0, 1);
    gPosition = vec3(model * vec4(tePosition[2], 1.0f));
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}
