#version 150

uniform ivec2 in_ScreenSize;
out vec4 out_Color;

void main(void)
{
    vec2 uv;
    const float OFFSET = 0;

    uv.x = gl_FragCoord.x/in_ScreenSize.x - OFFSET;
    uv.y = gl_FragCoord.y/in_ScreenSize.y - OFFSET;

    out_Color = vec4(uv.x, uv.y, 0.0, 1.0);
}
