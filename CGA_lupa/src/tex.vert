
//Autor: Cesar Tadeu Pozzer

//uniform sampler2D texture;
varying vec2 coord;

void main()
{
   coord = gl_Vertex.xy;
   gl_Position = ftransform();

   //gl_TexCoord[0]     --> built-in varying variable
   //gl_MultiTexCoord0  --> built-in attribute variable
   gl_TexCoord[0] = gl_MultiTexCoord0;
}
