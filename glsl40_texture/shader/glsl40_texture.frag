/*
	Recebe a coordenada de textura do VS (note que ela foi interpolada).
	 
	 Ao inves de utilizar layout (binding = 0), pode-se tambem utilizar
	 shader.setUniform("Texture1", 0). Porem a ultima demanda que chame
	 bind na textura em cada render (demo de tessellation usa a segunda maneira)
*/
#version 420 //  layout (binding = XXX) demanda >= 420

in vec2 TexCoord; // Coordenada de textura enviada pelo FS
in vec3 Position;
uniform bool fire;
uniform vec3 pos;
uniform vec3 dir;
layout (binding = 0) uniform sampler2D Texture1; // binding = 0 eh equivalente a GL_TEXTURE0.
layout (binding = 1) uniform sampler2D Texture2; // binding = 1 eh equivalente a GL_TEXTURE1.

void main() 
{
	//Amostra as texturas. TexCoord foi enviado pelo VS explicitamente
	// e interpolado para cada fragmento automaticamente.
	vec4 col1 = texture(Texture2, TexCoord);
	vec4 col2 = texture(Texture1, TexCoord);
	if (fire){
		float theta=acos(dot(normalize(dir), normalize(Position-pos))/(length(normalize(dir))*length(normalize(Position-pos))));
	    //vec3 dif = atan(dir - normalize(Position-pos));
		if(/*atan((normalize(Position-pos)).x - normalize(dir).x ,(normalize(Position-pos).z) - normalize(dir).z) < 0.025*/ theta < 0.1){
			col1 = vec4 (1, 0, 0, 0);
			col2 = vec4 (1, 0, 0, 0);
		}
	}

	//Interpola linearmente as duas cores utilizando a coordenada X da textura
	//ou seja, linearmente da esquerda para a direita.
    gl_FragColor = mix(col1, col2, TexCoord.x);
}
//(Position.x+pos.y+front.z)+(Position.y+pos.z+front.x)+(Position.z+pos.x+front.y)-((Position.x+pos.z+front.y)+(Position.y+pos.x+front.z)+(Position.z+pos.y+front.x))==0