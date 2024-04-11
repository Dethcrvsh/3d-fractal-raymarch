#version 150

uniform ivec2 in_ScreenSize;
uniform mat4 in_CamRotX;
uniform mat4 in_CamRotY;
uniform mat4 in_RotTest1;
uniform mat4 in_RotTest2;
uniform vec3 in_CamPosition;
out vec4 out_Color;

void plane_fold(inout vec4 z, const vec3 n, const float d)
{
    z.xyz -= 2.0 * min(0.0, dot(z.xyz, n) - d) * n;
}

void box_fold(inout vec4 z, const float fold_limit)
{
    z.xyz = clamp(z.xyz, -fold_limit, fold_limit) * 2 - z.xyz;
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
    const int MAX_ITER = 7;
    const float SCALE = 3;
    const float BAILOUT = 10000;

    vec4 z = vec4(p, 0);
    float dr = 1.0;
    vec4 offset = vec4(0.5, -0.5, 0.5, 0);

    for (int i = 0; i < MAX_ITER; i++)
    {
	box_fold(z, 1);
	z = z * in_RotTest1;
	z = z * in_RotTest2;
        z = z * SCALE + offset*SCALE;
        dr = dr * abs(SCALE) + 1.0;
    }

    return length(z) / abs(dr);
}

vec4 get_hit_color(const int iter)
{
    return vec4(vec3(1.0, 0.0, 1.0) * max(0.3, pow(0.97, iter)), 1.0);
}

vec4 get_bg_color()
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 ray_march(const vec3 start, const vec3 dir)
{
    const float MIN_DIST = 0.01;
    const float MAX_DIST = 1000;
    const int MAX_ITER = 64;

    float traveled_dist = 0;

    for (int i = 0; i < MAX_ITER; i++)
    {
        vec3 curr_pos = start + dir * traveled_dist; 

        float closest_dist = estimate_distance(curr_pos);

        if (closest_dist < MIN_DIST)
        {
            return get_hit_color(i);        
        }

        if (closest_dist > MAX_DIST)
        {
            return get_bg_color();
        }

        traveled_dist += closest_dist;
    }

    return get_bg_color();
}

void main(void)
{
    vec2 uv;
    const float OFFSET = -0.5;

    uv.x = gl_FragCoord.x/in_ScreenSize.x + OFFSET;
    uv.y = gl_FragCoord.y/in_ScreenSize.y + OFFSET;
    vec3 ray_dir = normalize(vec3(in_CamRotY * in_CamRotX * vec4(uv.x, uv.y, -1.0, 1.0))); 
    out_Color = ray_march(in_CamPosition, ray_dir);
}
