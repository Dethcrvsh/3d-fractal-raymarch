#version 150

uniform ivec2 in_ScreenSize;
uniform mat4 in_CamRotX;
uniform mat4 in_CamRotY;
uniform vec3 in_CamPosition;

// Distance Estimator
uniform float in_Scale;
uniform vec3 in_Offset;
uniform int in_DistEstIter;

// Ray Marcher
uniform float in_RayMarchMinDist;
uniform int in_RayMarchIterations;

// Color
uniform vec3 in_PrimaryColor;
uniform vec3 in_SecondaryColor;
uniform vec3 in_GlowColor;
uniform float in_Gradient;
uniform float in_GlowAmount;

out vec4 out_Color;

/* INSERT INPUT */

struct MarchResult
{
    bool did_hit;
    int number_of_iterations;
    vec3 hit_pos;
    float smallest_dist;
    float near_miss_angle;
};

void plane_fold(inout vec4 z, const vec3 n, const float d)
{
    z.xyz -= 2.0 * min(0.0, dot(z.xyz, n) - d) * n;
}

void box_fold(inout vec4 z, const float fold_limit)
{
    z.xyz = clamp(z.xyz, -fold_limit, fold_limit) * 2 - z.xyz;
}

void sphere_fold(inout vec4 z, inout float dz, const float fixedRadius) {
    float minRadius = 0.1;
    float r = dot(z,z);
    if (r<minRadius) { 
            // linear inner scaling
            float temp = (fixedRadius/minRadius);
            z *= temp;
            dz*= temp;
    } else if (r<fixedRadius) { 
            // this is the actual sphere inversion
            float temp =(fixedRadius/r);
            z *= temp;
            dz*= temp;
    }
}


void octahedral_symmetry_fold(inout vec4 point, const float limit)
{
    point = abs(point);
    if (point.x - point.y < limit)
    {
        point.xy = point.yx;
    }
    if (point.x - point.z < limit)
    {
        point.xz = point.zx;
    }
    if (point.y - point.z < limit)
    {
        point.yz = point.zy;
    }
}

void sierpinski_fold(inout vec4 z, const float limit) {
	z.xy -= min(z.x + z.y + limit, 0.0);
	z.xz -= min(z.x + z.z + limit, 0.0);
	z.yz -= min(z.y + z.z + limit, 0.0);
}

void mengerFold(inout vec4 z, const float limit) {
	float a = min(z.x - z.y + limit, 0.0);
	z.x -= a;
	z.y += a;
	a = min(z.x - z.z + limit, 0.0);
	z.x -= a;
	z.z += a;
	a = min(z.y - z.z + limit, 0.0);
	z.y -= a;
	z.z += a;
}

float estimate_distance(const vec3 p)
{
    vec4 z = vec4(p, 0);
    float dr = 1.0;

    for (int i = 0; i < in_DistEstIter; i++)
    {
        /* INSERT CODE */

        z = z * in_Scale + vec4(in_Offset, 0) * (in_Scale);
        dr = dr * abs(in_Scale);
    }

    return length(z) / abs(dr);
}

MarchResult ray_march(const vec3 start, const vec3 dir)
{
    const float MAX_DIST = 1000;

    float traveled_dist = 0;
    float smallest_dist = 1./0.; // Very large number
    bool did_hit = false;
    int i = 0;
    vec3 curr_pos = start;
    float near_miss_angle = 1.0;

    for (i; i < in_RayMarchIterations; i++)
    {
        curr_pos = start + dir * traveled_dist; 

        float closest_dist = estimate_distance(curr_pos);

		smallest_dist = min(smallest_dist, closest_dist);

        if (closest_dist < in_RayMarchMinDist)
        {
            did_hit = true;
            break;
        }

        if (closest_dist > in_RayMarchIterations)
        {
            break;
        }

        near_miss_angle = min(near_miss_angle, 8*closest_dist/traveled_dist);
        traveled_dist += closest_dist;
    }

    return MarchResult(did_hit, i, curr_pos, smallest_dist, near_miss_angle);
}

vec4 get_hit_color(const int iter, const vec3 curr_pos, const vec3 dir)
{
    // Calculate color from distance to origin
    float gradient = min(in_Gradient, length(curr_pos))/in_Gradient;
    vec3 color = in_PrimaryColor*(gradient) + in_SecondaryColor*(1 - gradient);

    // Calculate ambient occlusion
    vec4 amb_color = vec4((color) * max(0.3, pow(0.985, iter)), 1.0);
    //vec4 amb_color = vec4((color) * max(0.3, (1-float(iter)/float(500))) , 1.0);

    // Calculate color
	float STEP_LEN = in_RayMarchMinDist;
    const float REFLECTION_DIF = 0.7;
    const float REFLECTION_SPEC = 0.9;
    const vec3 LIGHT_DIR = normalize(vec3(0.8, 0.5, 1));
    const float INTENSITY_DIF = 1.0;
    const float INTENSITY_AMB = 0.8;
    const float INTENSITY_SPEC = 0.9;
    const float ALPHA = 12;

    // Normals
    vec3 pos = curr_pos - dir*in_RayMarchMinDist*2; // Backtrack to get better normals
    float x_norm = estimate_distance(pos + vec3(STEP_LEN,0,0)) - estimate_distance(pos - vec3(STEP_LEN,0,0));
    float y_norm = estimate_distance(pos + vec3(0,STEP_LEN,0)) - estimate_distance(pos - vec3(0,STEP_LEN,0));
    float z_norm = estimate_distance(pos + vec3(0,0,STEP_LEN)) - estimate_distance(pos - vec3(0,0,STEP_LEN));
	vec3 normal = normalize(vec3(x_norm, y_norm, z_norm));

    vec3 reflection_angle = 2*normal * dot(-LIGHT_DIR, normal) - (-LIGHT_DIR);

    // Light componenets
    float amb_light = REFLECTION_DIF * INTENSITY_AMB;
    float dif_light = REFLECTION_DIF * INTENSITY_DIF * max(0, dot(normal, LIGHT_DIR));
    float spec_light = REFLECTION_SPEC * INTENSITY_SPEC * pow(max(0, dot(reflection_angle, dir)), ALPHA);

    // Calculate shadows
    MarchResult result = ray_march(pos, LIGHT_DIR);
    float shadow = 0.5 + 0.5 * result.near_miss_angle;
    //spec_light = spec_light * result.near_miss_angle;
    if(result.did_hit){
        shadow = 0.5;
    }

	return amb_color * (amb_light + dif_light + spec_light) * shadow;
}

vec4 get_bg_color(const float closest_dist)
{
    float gradient = min(in_GlowAmount, closest_dist)/in_GlowAmount;
    vec3 color = vec3(0.0, 0.0, 0.0) * (gradient) + in_GlowColor * (1-gradient);
	return vec4(color, 1.0);
}

void main(void)
{
    vec2 uv;
    vec2 offset = vec2(-0.5 * in_ScreenSize.x/in_ScreenSize.y, -0.5);

    uv.x = gl_FragCoord.x/in_ScreenSize.y + offset.x;
    uv.y = gl_FragCoord.y/in_ScreenSize.y + offset.y; 

    vec3 ray_dir = normalize(vec3(in_CamRotY * in_CamRotX * vec4(uv.x, uv.y, -1.0, 1.0))); 
    MarchResult result = ray_march(in_CamPosition, ray_dir);
    vec4 color = vec4(0, 0, 0, 0);

    if (result.did_hit == true){
        color = get_hit_color(result.number_of_iterations, result.hit_pos, ray_dir);
    }
    else{
        color = get_bg_color(result.smallest_dist);
    }
    out_Color = color;
}
