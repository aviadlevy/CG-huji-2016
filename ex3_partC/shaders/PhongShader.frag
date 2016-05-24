#version 330
#define PI 3.14159

uniform float specExp = 200.0;
uniform int textMode;
uniform sampler2D my_colormap;
uniform sampler2D my_specmap;
uniform float textScale;
uniform float turbMagn;

uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform vec3 specularColor = vec3(1.0, 1.0, 1.0);
uniform vec3 lightColor1 = vec3(1.0, 0.9, 0.7);
uniform vec3 lightColor2 = vec3(0.6, 0.6, 1.0);

uniform vec3 light_position1 = vec3(3.0, 2.0, 1.0);
uniform vec3 light_position2 = vec3(-3.0, 0.0, 1.0);

vec3 ka = vec3(0.1, 0.1, 0.1);
vec3 kd = vec3(0.3, 0.3, 0.3);
vec3 ks = vec3(0.3, 0.3, 0.3);

in vec3 nTemp;
in vec3 shadingPosition;
in vec3 myPosition;

out vec4 outColor;

vec3 n = normalize(nTemp);

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}


float snoise(vec3 v)
{
	const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
	const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);
	
	// First corner
	vec3 i  = floor(v + dot(v, C.yyy) );
	vec3 x0 =   v - i + dot(i, C.xxx) ;
	
	// Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0 - g;
	vec3 i1 = min( g.xyz, l.zxy );
	vec3 i2 = max( g.xyz, l.zxy );
	
	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y
	
	// Permutations
	i = mod289(i);
	vec4 p = permute( permute( permute(
									   i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
							  + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
					 + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));
	
	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;
	
	vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)
	
	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)
	
	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;
	vec4 h = 1.0 - abs(x) - abs(y);
	
	vec4 b0 = vec4( x.xy, y.xy );
	vec4 b1 = vec4( x.zw, y.zw );
	
	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	vec4 s0 = floor(b0)*2.0 + 1.0;
	vec4 s1 = floor(b1)*2.0 + 1.0;
	vec4 sh = -step(h, vec4(0.0));
	
	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;
	
	vec3 p0 = vec3(a0.xy,h.x);
	vec3 p1 = vec3(a0.zw,h.y);
	vec3 p2 = vec3(a1.xy,h.z);
	vec3 p3 = vec3(a1.zw,h.w);
	
	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;
	
	// Mix final noise value
	vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;
	return 42.0 * dot( m*m, vec4(dot(p0,x0), dot(p1,x1),
								 dot(p2,x2), dot(p3,x3) ) );
}

float turb(vec3 v)
{
	const float base_freq = 0.2; // SPIDER
	
	vec4 noisevec;
	noisevec.x = snoise(v * base_freq*1.0) * 8.0;
	noisevec.y = snoise(v * base_freq*2.0) * 4.0;
	noisevec.z = snoise(v * base_freq*4.0) * 2.0;
	noisevec.w = snoise(v * base_freq*8.0) * 1.0;
	// noisevec = (noisevec / 8.0 + 1.0) / 2.0;
	noisevec = noisevec / 8.0;
	// noisevec = noisevec * noisevec;
	
	float intensity = abs(noisevec[0] - 0.20) +
	abs(noisevec[1] - 0.10) +
	abs(noisevec[2] - 0.05) +
	abs(noisevec[3] - 0.025);
	return intensity;
}

void phong(float texture_spec)
{
	vec3 ambient = ka * ambientColor;
	vec3 l1 = normalize(light_position1 - shadingPosition);
	vec3 l2 = normalize(light_position2 - shadingPosition);
	float dist1 = distance(shadingPosition.xyz, light_position1);
	float dist2 = distance(shadingPosition.xyz, light_position2);
	vec3 diffuse1 = lightColor1 * kd * max(0.0, dot(l1, n));
	vec3 diffuse2 = lightColor2 * kd * max(0.0, dot(l2, n));
	vec3 v = normalize(vec3(0,0,-3) - shadingPosition);
	vec3 r1 = normalize(reflect(l1, n));
	vec3 spec1 = ks * specularColor * pow(max(dot(v, r1), 0.0001), specExp);
	vec3 r2 = normalize(reflect(l2, n));
	vec3 spec2 = ks * specularColor * pow(max(dot(v, r2), 0.0001), specExp);
	vec3 shade;
	shade += ambient;
	shade += (diffuse1 + diffuse2);
	shade += (spec1 + spec2) * texture_spec;
	outColor =  vec4(shade, 1.0);
}

void marble(float tr)
{
	float t = (pow(sin(2 * PI * (myPosition.x * textScale + tr)),3) + 1) / 2;
	kd = vec3(t,t,t);
}

void wood(float tr)
{
	float t;
	float d = sqrt(myPosition.y * myPosition.y + myPosition.z * myPosition.z) * textScale + 1 * tr;
	t = (abs(cos(2 * PI * (d - floor(d)))) + 1) / 2;
	kd = mix(vec3(6.0/32, 3.0/32, 0.0), vec3(6.0/8 , 3.0/8 , 0.0), t);
}

void mirror()
{
	vec2 fragTexCoord;
	vec3 toEye = normalize(vec3(0.0, 0.0, 0.0) - shadingPosition);
	vec3 posOnSphere = normalize(reflect(-toEye, n));
	float theta = atan(posOnSphere.x , posOnSphere.z);
	float phi = atan(posOnSphere.y , length(posOnSphere.xz));
	float r = length(posOnSphere.xyz);
	float du = (theta + PI) / (2 * PI);
	float dv = (phi + PI/2)  / PI;
	fragTexCoord.x = du;
	fragTexCoord.y = 1.0 - dv;
	vec4 diffuse = texture(my_colormap, fragTexCoord);
	outColor = vec4(diffuse.xyz, 1.0);
}

void brick()
{
	vec2 fragCoord;
    vec3 texturePos = (myPosition + 1) / 2;
	if (1 - abs(myPosition.x) < 0.001)
	{
		fragCoord = texturePos.yz;
	}
	if (1 - abs(myPosition.y) < 0.001)
	{
		fragCoord = texturePos.xz;
	}
	if (1 - abs(myPosition.z) < 0.001)
	{
		fragCoord = texturePos.xy;
	}
	float val = texture(my_specmap, fragCoord).x;
	float du = texture(my_specmap, vec2(fragCoord.x + 1.0/512, fragCoord.y)).x - val;
    float dv = texture(my_specmap, vec2(fragCoord.x, fragCoord.y + 1.0/512)).x  - val;
    n = normalize(nTemp - textScale * vec3(du, dv , 0.0));
	kd = texture(my_colormap, fragCoord).xyz;
}


void main()
{

	float tr = turb(turbMagn * myPosition * sqrt(textScale));
	float texture_spec = 1.0;

	if(textMode == 0){
		marble(tr);
	}
	else if(textMode == 1){
		wood(tr);
		texture_spec = 0.1;
	}
	else if(textMode == 2){
		mirror();
	}
	else if(textMode == 3){
		brick();
		texture_spec = 0.0;
	}

	if (textMode != 2)
	{
		phong(texture_spec);
	}
}