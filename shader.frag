#version 150

uniform ivec2 in_ScreenSize;
out vec4 out_Color;

vec3 box_fold(const vec3 z, const float fold_limit)
{
    return clamp(z, -fold_limit, fold_limit) * 2 - z;
}

float estimate_distance(const vec3 p)
{
    const int MAX_ITER = 20;
    const float SCALE = 3.7;

    vec3 z = p;
    float dr = 1.0;
    vec3 offset = p;

    for (int i = 0; i < MAX_ITER; i++)
    {
        z = box_fold(z, 1.9);
        z = z * SCALE + offset;
        dr = dr * abs(SCALE) + 1.0;
    }

    return length(z) / abs(dr);
}

vec4 get_hit_color(const int iter)
{
    return vec4(vec3(1.0, 0.0, 1.0) * pow(0.97, iter), 1.0);
}

vec4 get_bg_color()
{
    return vec4(0.0, 0.0, 0.0, 1.0);
}

vec4 ray_march(const vec3 start, const vec3 dir)
{
    const float MIN_DIST = 0.05;
    const float MAX_DIST = 1000;
    const int MAX_ITER = 1024;

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

    const vec3 camera = vec3(0, 0, 20);
    vec3 ray_dir = normalize(vec3(uv.x, uv.y, -1));

    out_Color = ray_march(camera, ray_dir);
}
