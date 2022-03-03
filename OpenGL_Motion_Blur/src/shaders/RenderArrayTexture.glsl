#version 330 core

#define FRAME_LAYERS 16

in vec2 vs_textureCoord;

uniform sampler2DArray u_texture;

out vec4 color;

void main(void)
{
    color = vec4( 0.0f );

    for( int layer = FRAME_LAYERS - 1; layer >= 0; layer-- )
    {
        color += (FRAME_LAYERS - layer) * texture( u_texture, vec3( vs_textureCoord, layer ) );
        //color = texture( u_texture, vec3( vs_textureCoord, layer ) );
    }

    color /= color.a;
}