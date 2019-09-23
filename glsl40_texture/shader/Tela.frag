/*
	Recebe a coordenada de textura do VS (note que ela foi interpolada).
	 
	 Ao inves de utilizar layout (binding = 0), pode-se tambem utilizar
	 shader.setUniform("Texture1", 0). Porem a ultima demanda que chame
	 bind na textura em cada render (demo de tessellation usa a segunda maneira)
*/
#version 420 //  layout (binding = XXX) demanda >= 420

in vec2 TexCoord; // Coordenada de textura enviada pelo FS
in vec3 Position;
layout (binding = 2) uniform sampler2D camera;
layout (binding = 3) uniform sampler2D mira;
uniform bool zoom;

void main() 
{
	//Amostra as texturas. TexCoord foi enviado pelo VS explicitamente
	// e interpolado para cada fragmento automaticamente.
	vec4 col1;
	vec4 col2 = texture(mira, TexCoord);
	if(zoom){
		if (Position.x*Position.x+Position.y*Position.y < 0.25*0.25){
			gl_FragColor = col2;
		}
		else{
			int qnt=0;
			vec2 newPos;
			for(float x = -0.1; x < 0.1; x+=0.01){
				for(float y = -0.1; y < 0.1; y+=0.01){
					newPos.x = TexCoord.x+x;
					newPos.y = TexCoord.y+y;
					col1 += texture(camera, newPos);
					qnt++;
				}
			}
			col1/=qnt;
			gl_FragColor = col1;
		}
	}
	else{
	    vec4 col1 = texture(camera, TexCoord);
		gl_FragColor = col1;
	}

}
