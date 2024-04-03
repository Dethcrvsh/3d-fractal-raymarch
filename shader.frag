#version 150

uniform ivec2 in_ScreenSize;
uniform mat4 in_CamRotX;
uniform mat4 in_CamRotY;
uniform mat4 in_RotTest1;
uniform mat4 in_RotTest2;
uniform vec3 in_CamPosition;
out vec4 out_Color;

vec3 box_fold(const vec3 z, const float fold_limit)
{
    return clamp(z, -fold_limit, fold_limit) * 2 - z;
}

vec3 octahedral_symmetry_fold(vec3 point)
{
    if (point.x + point.y < 0)
    {
        point.xy = -point.yx;
    }
    if (point.x + point.z < 0)
    {
        point.xz = -point.zx;
    }
    if (point.y + point.z < 0)
    {
        point.yz = -point.zy;
    }

    return point;
}

float estimate_distance(const vec3 p)
{
    const int MAX_ITER = 10;
    const float SCALE = 2;
    const float BAILOUT = 10000;

    vec3 z = p;
    float dr = 1.0;
    vec3 offset = vec3(0, 0, 0);

    for (int i = 0; i < MAX_ITER; i++)
    {
	z = octahedral_symmetry_fold(z);
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
