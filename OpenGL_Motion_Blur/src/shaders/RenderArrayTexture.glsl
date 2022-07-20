#version 330 core

in vec2 vs_textureCoord;

uniform sampler2DArray u_texture;

uniform int u_blurLevel;

out vec4 color;

void main(void)
{
    color = vec4( 0.0f );

    for( int layer = u_blurLevel - 1; layer >= 0; layer-- )
    {
        color += (u_blurLevel - layer) * texture( u_texture, vec3( vs_textureCoord, layer ) );
    }

    color /= color.a;
}