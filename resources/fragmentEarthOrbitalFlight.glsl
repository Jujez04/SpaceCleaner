#version 330 core
out vec4 FragColor;

uniform float uTime;

float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Funzione per generare "continenti" proceduralmente
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
    vec2 uv = (gl_FragCoord.xy - vec2(400.0, 300.0)) / 300.0; // centro schermo circa
    float t = uTime * 0.2;

    // Pianeta
    float r = length(uv);
    float planet = smoothstep(0.5, 0.48, r);

    // Coordinate "ruotate" per simulare rotazione pianeta
    float angle = atan(uv.y, uv.x) + t;
    float radius = length(uv);
    vec2 spherical = vec2(angle * 2.0, radius * 2.0);

    // Continenti e oceani
    float continents = fbm(spherical * 1.5 + t);
    vec3 landColor = mix(vec3(0.0, 0.2, 0.6), vec3(0.1, 0.5, 0.1), continents);
    vec3 planetColor = landColor * planet;

    // Atmosfera e glow
    float glow = smoothstep(0.5, 0.4, r);
    vec3 atmosphere = vec3(0.2, 0.4, 0.8) * pow(1.0 - r, 3.0);

    // Sfondo spaziale (gradiente e stelle)
    float stars = step(0.995, noise(uv * 100.0 + uTime * 0.1));
    vec3 spaceColor = mix(vec3(0.02, 0.02, 0.05), vec3(0.0, 0.0, 0.1), r);
    vec3 starColor = vec3(1.0) * stars;

    vec3 finalColor = spaceColor + starColor + planetColor + atmosphere;

    FragColor = vec4(finalColor, 1.0);
}
