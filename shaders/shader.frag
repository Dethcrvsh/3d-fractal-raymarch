#version 150

uniform ivec2 in_ScreenSize;
uniform mat4 in_CamRotX;
uniform mat4 in_CamRotY;
uniform mat4 in_RotTest1;
uniform mat4 in_RotTest2;
uniform vec3 in_CamPosition;
uniform float in_Scale;
uniform vec3 in_Offset;
out vec4 out_Color;

const float MIN_DIST = 0.001;

struct MarchResult
{
    bool did_hit;
    int number_of_iterations;
    vec3 hit_pos;
    float smallest_dist;
};

void plane_fold(inout vec4 z, const vec3 n, const float d)
{
    z.xyz -= 2.0 * min(0.0, dot(z.xyz, n) - d) * n;
}

void box_fold(inout vec4 z, const float fold_limit)
{
    z.xyz = clamp(z.xyz, -fold_limit, fold_limit) * 2 - z.xyz;
}

void sphereFold(inout vec4 z, inout float dz) {
    float minRadius = 0.01;
    float fixedRadius = 2.0;
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

void sierpinskiFold(inout vec4 z, const float limit) {
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
    const int MAX_ITER = 20;
    const float BAILOUT = 10000;

    vec4 z = vec4(p, 0);
    float dr = 1.0;

    for (int i = 0; i < MAX_ITER; i++)
    {
	    box_fold(z, 1);
	    z = z * in_RotTest1;
        sierpinskiFold(z, 2);
	    z = z * in_RotTest2;
        sphereFold(z, dr);
        z = z * in_Scale + vec4(in_Offset, 0) *in_Scale;
        dr = dr * abs(in_Scale) + 1.0;
    }

    return length(z) / abs(dr);
}

MarchResult ray_march(const vec3 start, const vec3 dir)
{
    const float MAX_DIST = 1000;
    const int MAX_ITER = 200;

    float traveled_dist = 0;
    float smallest_dist = 1./0.; // Very large number
    bool did_hit = false;
    int i = 0;
    vec3 curr_pos = start;

    for (i; i < MAX_ITER; i++)
    {
        curr_pos = start + dir * traveled_dist; 

        float closest_dist = estimate_distance(curr_pos);

		smallest_dist = min(smallest_dist, closest_dist);

        if (closest_dist < MIN_DIST)
        {
            did_hit = true;
            break;
        }

        if (closest_dist > MAX_DIST)
        {
            break;
        }

        traveled_dist += closest_dist;
    }

    return MarchResult(did_hit, i, curr_pos, smallest_dist);
}

vec4 get_hit_color(const int iter, const vec3 curr_pos, const vec3 dir)
{
    // Calculate color from distance to origin
    float gradient = min(10, length(curr_pos))/10;
    vec3 color = vec3(0.7, 0.7, 0.7)*(gradient) + vec3(0.3, 0.3, 0.3)*(1 - gradient);

    // Calculate ambient occlusion
    vec4 amb_color = vec4((color) * max(0.3, pow(0.98, iter)), 1.0);
    //vec4 amb_color = vec4((color) * max(0.3, (1-float(iter)/float(500))) , 1.0);

    // Calculate color
	const float STEP_LEN = MIN_DIST;
    const float REFLECTION_DIF = 0.7;
    const float REFLECTION_SPEC = 0.9;
    const vec3 LIGHT_DIR = normalize(vec3(0.8, 0, 1));
    const float INTENSITY_DIF = 1.0;
    const float INTENSITY_AMB = 0.8;
    const float INTENSITY_SPEC = 1.0;
    const float ALPHA = 20;

    // Normals
    vec3 pos = curr_pos - dir*MIN_DIST*2; // Backtrack to get better normals
    float x_norm = estimate_distance(pos + vec3(STEP_LEN,0,0)) - estimate_distance(pos - vec3(STEP_LEN,0,0));
    float y_norm = estimate_distance(pos + vec3(0,STEP_LEN,0)) - estimate_distance(pos - vec3(0,STEP_LEN,0));
    float z_norm = estimate_distance(pos + vec3(0,0,STEP_LEN)) - estimate_distance(pos - vec3(0,0,STEP_LEN));
	vec3 normal = normalize(vec3(x_norm, y_norm, z_norm));

    vec3 reflection_angle = 2*normal * dot(-LIGHT_DIR, normal) - (-LIGHT_DIR);

    // Light componenets
    float amb_light = REFLECTION_DIF * INTENSITY_AMB;
    float dif_light = REFLECTION_DIF * INTENSITY_DIF * max(0, dot(normal, -LIGHT_DIR));
    float spec_light = REFLECTION_SPEC * INTENSITY_SPEC * pow(max(0, dot(reflection_angle, dir)), ALPHA);

    // Calculate shadows
    MarchResult result = ray_march(pos, LIGHT_DIR);
    float shadow = 1.0;
    if(result.did_hit){
        shadow = 0.6;
        //spec_light = 0.8;
    }

	return amb_color * (amb_light + dif_light + spec_light) * shadow;
}

vec4 get_bg_color(const float closest_dist)
{
    float gradient = min(0.5, closest_dist)/0.5;
    vec3 color = vec3(0.0, 0.0, 0.0) * (gradient) + vec3(0.6, 0.0, 0.6) * (1-gradient);
	return vec4(color, 1.0);
}

void main(void)
{
    vec2 uv;
    const float OFFSET = -0.5;

    uv.x = gl_FragCoord.x/in_ScreenSize.y + OFFSET;
    uv.y = gl_FragCoord.y/in_ScreenSize.y + OFFSET;

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
