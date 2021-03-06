#version 330


uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform float textureless;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	vec4 kd;
	vec4 ks;

	if(textureless==0)
	{
	kd = texture(textureMap0,iTexCoord0);
	ks = texture(textureMap1,iTexCoord0);
	}
	else
	{
	kd = vec4(0.75,0.75,0.75,1);
	ks = kd;
	}

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
	//pixelColor=ks;
	}