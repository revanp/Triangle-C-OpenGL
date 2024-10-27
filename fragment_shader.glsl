#version 120

varying vec3 fragGolor;

void main()
{
    gl_FragColor = vec4(fragGolor, 1.0);
}