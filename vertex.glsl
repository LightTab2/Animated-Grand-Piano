#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float textureless;
uniform vec4 ls1;
uniform vec4 ls2;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 ic;
out vec4 l1;
out vec4 l2;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

void main(void)
{
    vec4 lp1 = ls1, lp2 = ls2;                           //pozcyja światła, przestrzeń świata
    l1 = normalize(V * lp1 - V * M * vertex);    //wektor do światła w przestrzeni oka  
    l2 = normalize(V * lp2 - V * M * vertex);    //wektor do światła w przestrzeni oka                                  
    v = normalize(-V * M * vertex);         //wektor normalny w przestrzeni oka
    n = normalize(V * M * normal);          //wektor normalny w przestrzeni oka

    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}
