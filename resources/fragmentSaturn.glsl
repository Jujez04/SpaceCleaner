#version 330 core
out vec4 FragColor;

uniform vec3 uColor;      // Colore sfondo (es. blu scuro o nero)
uniform vec2 uResolution; // Risoluzione viewport
uniform float uTime;      // Tempo trascorso

// Rumore semplice per posizionare le stelle
float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453);
}

// Sfondo stellato con scintillio casuale
vec3 stars(vec2 uv) {
    vec2 gv = floor(uv * 100.0);
    float seed = rand(gv);
    
    // Luminosità base molto bassa
    float star = seed * 0.02;

    // Solo alcune stelle scintillano
    if(seed < 0.03) { // circa 3% delle stelle
        // Effetto pulsante: oscillazione lenta casuale basata sul seed
        float flicker = sin(uTime * (2.0 + seed * 5.0) + seed * 10.0) * 0.5 + 0.5;
        // Picco di luminosità breve
        star += flicker * 0.08;
    }

    return vec3(star);
}

// Corpo di Saturno
vec3 saturn_body(vec2 uv, vec2 center, float radius) {
    float dist = length(uv - center);
    if(dist < radius) {
        float intensity = 1.0 - smoothstep(radius*0.9, radius, dist);
        return vec3(0.9,0.65,0.4) * intensity;
    }
    return vec3(0.0);
}

// Anelli di Saturno
vec3 saturn_rings(vec2 uv, vec2 center, float inner_radius, float outer_radius) {
    float dist = length(uv - center);
    if(dist > inner_radius && dist < outer_radius) {
        float ring_pattern = mod(dist*20.0,1.0);
        float ring_intensity = smoothstep(0.4,0.6,ring_pattern);
        return vec3(0.8,0.8,0.9) * ring_intensity * 0.3;
    }
    return vec3(0.0);
}

void main() {
    vec2 uv = (gl_FragCoord.xy / uResolution.xy) * 2.0 - 1.0;
    uv.x *= uResolution.x / uResolution.y;

    vec3 color = vec3(0.0);

    // Sfondo stellato con alcune stelle scintillanti
    color += stars(uv * 3.0);

    // Saturno
    vec2 saturn_center = vec2(0.5, 0.0);
    float saturn_radius = 0.25;
    float rings_outer = saturn_radius*2.0;
    float rings_inner = saturn_radius*1.1;

    // Oscillazione leggera del pianeta
    saturn_center.x += sin(uTime*0.05)*0.03;

    color += saturn_rings(uv, saturn_center, rings_inner, rings_outer);
    color += saturn_body(uv, saturn_center, saturn_radius);

    // Piccolo bagliore generale
    color += uColor * 0.02;

    FragColor = vec4(color,1.0);
}
