#version 330 core
out vec4 FragColor;

uniform float uTime;

float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

float fbm(vec2 p) {
    float v = 0.0;
    float a = 0.5;
    mat2 rot = mat2(0.8, -0.6, 0.6, 0.8);
    for (int i = 0; i < 5; i++) {
        v += a * noise(p);
        p = rot * p * 2.0;
        a *= 0.5;
    }
    return v;
}

void main() {
    vec2 uv = (gl_FragCoord.xy - vec2(400.0, 300.0)) / 300.0;
    float t = uTime * 0.1;

    float r = length(uv);
    float planetMask = smoothstep(0.5, 0.48, r);

    float angle = atan(uv.y, uv.x) + t * 0.3;
    float radius = length(uv);
    vec2 spherical = vec2(angle * 2.0, radius * 2.0);

    float continents = fbm(spherical * 1.5 + t);
    vec3 ocean = vec3(0.05, 0.3, 0.55);    
    vec3 land = vec3(0.2, 0.7, 0.5);  
    vec3 planetColor = mix(ocean, land, continents);
    planetColor *= planetMask;

    float atmosphere = smoothstep(0.5, 0.35, r);
    vec3 atmosphereColor = vec3(0.2, 0.5, 0.9) * pow(1.0 - r, 3.0);

    vec3 spaceColor = vec3(0.02, 0.05, 0.15);

    float stars = step(0.997, noise(uv * 100.0 + uTime * 0.05));
    vec3 starColor = vec3(1.0) * stars;

    vec3 finalColor = spaceColor + planetColor + atmosphereColor + starColor * 0.8;

    FragColor = vec4(finalColor, 1.0);
}
