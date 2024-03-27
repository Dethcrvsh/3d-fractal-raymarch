#version 150

uniform ivec2 in_ScreenSize;
out vec4 out_Color;


float estimate_distance(const vec3 p)
{
    const vec3 center1 = vec3(0, 0, -30);
    const int radius1 = 10;

    const vec3 center2 = vec3(7, 8, -22);
    const int radius2 = 6;

    float dist1 = distance(p, center1) - radius1;
    float dist2 = distance(p, center2) - radius2;
 
    const float k = 6;
    float h = max(k - abs(dist1 - dist2), 0) / k;

    return min(dist1, dist2) - h*h*h*k*1/6.0;
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
    const float MIN_DIST = 0.00004;
    const float MAX_DIST = 1000;
    const int MAX_ITER = 512;

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

    const vec3 camera = vec3(0, 0, 2);
    vec3 ray_dir = normalize(vec3(uv, -1));

    out_Color = ray_march(camera, ray_dir);
}
