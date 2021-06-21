#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform float textureless;
uniform int pink;
uniform int l1on;
uniform int l2on;
uniform int seed;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec2 iTexCoord0;

float rand(vec2 co) 
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

bool isRozowy(vec4 color)
{
	float r = color.x, g = color.y, b = color.z;
	if (color.z > color.x)
	{
		r = color.z;
		b = color.x;
	}
	if ((r + b) > 0.196078431373f && b != 0.f && (r / b) < 3.f && (r / b) > 1.f && (r - b) < (r + b - 2.21f * g)) //Dobrałem parametry eksperymentalnie, tak żeby usunąć odcienie
		return true;
	return false;
}

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml1 = normalize(l1);
	vec4 ml2 = normalize(l2);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	//Parametry powierzchni
	vec4 kd;
	vec4 ks;

	if (textureless==0)
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
	//Wektor odbity
	vec4 mr1 = reflect(-ml1, mn);
	vec4 mr2 = reflect(-ml2, mn);
	float nl = clamp(max(dot(mn, ml1), dot(mn, ml2)), 0.f, 1.f);
	float rv1 = pow(clamp(dot(mr1, mv), 0.f, 1.f), 25);
	float rv2 = pow(clamp(dot(mr2, mv), 0.f, 1.f), 25);

	vec4 color;
	if ((l1on == 0) && (l2on == 0))
	{
		nl = 1.f;
		rv1 = rv2 = 0.f;
		color = vec4((1.f - kd.rgb) * 0.5f, kd.a);
	}
	else if (l1on == 0)
	{
		nl = clamp(dot(mn, ml2), 0.f, 1.f);
		color = vec4(kd.rgb * (nl + rv2), kd.a);
	}
	else if (l2on == 0)
	{
		nl = clamp(dot(mn, ml1), 0.f, 1.f);
		color = vec4(kd.rgb * (nl + rv1), kd.a);
	}
	else color = vec4(kd.rgb * (nl + rv1 + rv2), kd.a);
	if (pink == 0)
		for (int i = seed; isRozowy(color); ++i)
			color = vec4(rand(color.xz + i), rand(color.xy + i), rand(color.yz + i), 1.f);
	pixelColor = color;
	}