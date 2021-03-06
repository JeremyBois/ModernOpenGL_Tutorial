#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float uvScale;

out vec4 FragColor;

void main()
{
    vec4 t0 = texture2D(tex0, texCoord * uvScale);
    vec4 t1 = texture2D(tex1, texCoord);

    // Multiplication then alpha blending
    FragColor = mix(vertexColor * t0, t1, 0.01 * t1.a);

    // Gamma correction
    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}
