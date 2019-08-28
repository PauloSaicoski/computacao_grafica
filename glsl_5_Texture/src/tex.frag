//Autor: Cesar Tadeu Pozzer

#define M_PI		3.14159265358979323846
uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     brilho;
uniform float     uAngle;
varying vec2      coord;

void main()
{
   vec2 position = gl_TexCoord[0].st;
   vec3 cor;
   float localAngle = atan(coord.y,coord.x);
   float angleInverso = (uAngle> 0) ? uAngle-M_PI: uAngle+M_PI;
   bool lado;
   if(uAngle > 0){
    lado = (localAngle > 0) ? (localAngle < uAngle) : (localAngle > angleInverso);
   }
   else{
    lado = (localAngle < 0) ? (localAngle < uAngle) : (localAngle > angleInverso);
   }
   if(lado)
      cor = texture2D(texture_0, position).rgb; //nao pega o alpha
   else
      cor = texture2D(texture_1, position).rgb; //nao pega o alpha
   //cor*=brilho;
   cor = clamp(cor, 0.0, 1.0); //os parametros 0.0 e 1.0 devem ser float. Nao podem ser inteiros

   //gl_FragColor = texture2D(texture, position).rgba;
   gl_FragColor = vec4( cor, 1);
}
