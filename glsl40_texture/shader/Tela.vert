/*
	Transforma o vertice e passa a coordenada de textura para o FS.
*/
#version 400

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 VertexTexCoord;

out vec2 TexCoord;
out vec3 Position;
uniform mat4 MVP;

void main()
{
    TexCoord = VertexTexCoord;
	Position = VertexPosition;
	
	gl_Position = /*MVP */ vec4(VertexPosition,1.0);
}
