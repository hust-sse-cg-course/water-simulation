#version 330 core
out vec4 gl_FragColor;

const float IOR_AIR = 1.0; 
const float IOR_WATER = 1.333; 
const vec3 abovewaterColor = vec3(0.25, 1.0, 1.25);
const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);
const float poolHeight = 1.0;

uniform vec3 light;
uniform sampler2D water; 
uniform sampler2D tiles; 
uniform sampler2D causticTex;
uniform vec3 eye;

in vec3 position;

vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) 
{ 
	vec3 tMin = (cubeMin - origin) / ray;
	vec3 tMax = (cubeMax - origin) / ray;
	vec3 t1 = min(tMin, tMax); 
	vec3 t2 = max(tMin, tMax); 
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z); 
	return vec2(tNear, tFar); 
} 

vec3 getWallColor(vec3 point)
{
	float scale = 0.5; 
	vec3 wallColor;
	vec3 normal; 
	if (abs(point.x) > 0.999)
	{
		wallColor = texture2D(tiles, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
		normal = vec3(-point.x, 0.0, 0.0);
	} 
	else if (abs(point.z) > 0.999) 
	{
		wallColor = texture2D(tiles, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
		normal = vec3(0.0, 0.0, -point.z);
	}
	else 
	{ 
		wallColor = texture2D(tiles, point.xz * 0.5 + 0.5).rgb; 
		normal = vec3(0.0, 1.0, 0.0);
	}
	scale /= length(point); 
	/* pool ambient occlusion */ /* caustics */
	vec3 refractedLight = -refract(-light, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER); 
	float diffuse = max(0.0, dot(refractedLight, normal)); 
	vec4 info = texture2D(water, point.xz * 0.5 + 0.5); 
	if (point.y < info.r) 
	{
		vec4 caustic = texture2D(causticTex, 0.75 * (point.xz - point.y * refractedLight.xz / refractedLight.y) * 0.5 + 0.5);
		scale += diffuse * caustic.r * 2.0 * caustic.g;
	} 
	else 
	{ 
		/* shadow for the rim of the pool */ 
		vec2 t = intersectCube(point, refractedLight, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
		diffuse *= 1.0 / (1.0 + exp(-200.0 / (1.0 + 10.0 * (t.y - t.x)) * (point.y + refractedLight.y * t.y - 2.0 / 12.0))); 
		scale += diffuse * 0.5; 
	}
	return wallColor * scale;
}

vec3 getSurfaceRayColor(vec3 origin, vec3 ray, vec3 waterColor)
{ 
	vec3 color;
	vec2 t = intersectCube(origin, ray, vec3(-1.0, -poolHeight, -1.0), vec3(1.0, 2.0, 1.0));
	vec3 hit = origin + ray * t.y;
	if (ray.y < 0.0)	//光线往下
	{
		color = getWallColor(hit);
		color *= waterColor;
	} 
	else			//光线往上
	{ 
		if (hit.y < 2.0 / 12.0) 
		{ 
			color = getWallColor(hit);
		}
		else
		{
			ray = normalize(ray);
			color = vec3(pow(max(0.0, dot(normalize(light), ray)), 5000.0)) * vec3(10.0, 8.0, 6.0);
		} 
	} 
	return color;
} 
in vec4 temp;
void main() 
{ 
	vec2 coord = position.xz * 0.5 + 0.5;
	vec4 info = texture2D(water, coord);
	/* make water look more "peaked" */ 
	for (int i = 0; i < 5; i++) 
	{
		coord += info.ba * 0.005;
		info = texture2D(water, coord);
	} 

	vec3 normal = vec3(info.b, sqrt(1.0 - dot(info.ba, info.ba)), info.a);
	vec3 incomingRay = normalize(position - eye);
	
	vec3 outColor;
	if(gl_FrontFacing)
	{
		vec3 reflectedRay = reflect(incomingRay, normal);
		vec3 refractedRay = refract(incomingRay, normal, IOR_AIR / IOR_WATER);
		float fresnel = mix(0.25, 1.0, pow(1.0 - dot(normal, -incomingRay), 3.0));

		vec3 reflectedColor = getSurfaceRayColor(position, reflectedRay, abovewaterColor);
		vec3 refractedColor = getSurfaceRayColor(position, refractedRay, abovewaterColor);
		outColor = mix(refractedColor, reflectedColor, fresnel);
	}
	else
	{
		normal = -normal;
		vec3 reflectedRay = reflect(incomingRay, normal);
		vec3 refractedRay = refract(incomingRay, normal, IOR_WATER / IOR_AIR);
		float fresnel = mix(0.5, 1.0, pow(1.0 - dot(normal, -incomingRay), 3.0));
		vec3 reflectedColor = getSurfaceRayColor(position, reflectedRay, underwaterColor);
		vec3 refractedColor = getSurfaceRayColor(position, refractedRay, vec3(1.0)) * vec3(0.8, 1.0, 1.1);
		outColor = mix(reflectedColor, refractedColor, (1.0 - fresnel) * length(refractedRay));
	}
	//outColor /= (outColor + vec3(1.0));								//1
	//outColor = outColor / (outColor + vec3(1.0));					//2
	//outColor = vec3(1.0) - vec3(1.0) / (outColor + vec3(1.0));		//3
	gl_FragColor = vec4(outColor, 1.0);
}