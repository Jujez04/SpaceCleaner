#version 330 core
#ifdef GL_ES
precision highp float;
#endif

uniform vec3 uColor;       // Colore base (ad esempio, per la luce o l'atmosfera)
uniform vec2 uResolution;  // Risoluzione del viewport
uniform float uTime;       // Tempo trascorso in secondi

// Funzione di rumore (per le stelle)
float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

// Funzione per generare un campo stellare
vec3 stars(vec2 uv, float time) {
    vec2 p = floor(uv * 100.0);
    float star = rand(p) * 0.9;
    
    // Aggiunge un effetto scintillante basato sul tempo
    star *= (sin(rand(p) * 20.0 + time * 5.0) * 0.5 + 0.5);
    
    // Rende alcune stelle più luminose
    if (rand(p * 2.0) < 0.01) {
        star = 1.0 + sin(time) * 0.1;
    }
    
    return vec3(star * 0.5);
}

// Funzione per disegnare il cerchio di Saturno
vec3 saturn_body(vec2 uv, vec2 center, float radius) {
    float dist = length(uv - center);
    
    // Corpo del pianeta (semplice cerchio)
    if (dist < radius) {
        // Colore arancione-giallo tenue
        float intensity = 1.0 - smoothstep(radius * 0.9, radius, dist);
        return vec3(0.9, 0.65, 0.4) * intensity;
    }
    return vec3(0.0);
}

// Funzione per disegnare gli anelli di Saturno
vec3 saturn_rings(vec2 uv, vec2 center, float inner_radius, float outer_radius) {
    float dist = length(uv - center);
    
    // Gli anelli
    if (dist > inner_radius && dist < outer_radius) {
        // Simula la divisione degli anelli usando il modulo della distanza
        float ring_pattern = mod(dist * 20.0, 1.0); 
        float ring_color = smoothstep(0.4, 0.6, ring_pattern);
        
        // Colore grigio chiaro per gli anelli
        return vec3(0.8, 0.8, 0.9) * ring_color * 0.5;
    }
    return vec3(0.0);
}


void main() {
    // Coordinate normalizzate (da -1 a 1)
    vec2 uv = (gl_FragCoord.xy / uResolution.xy) * 2.0 - 1.0;
    uv.x *= uResolution.x / uResolution.y; // Correzione del rapporto d'aspetto
    
    vec3 final_color = vec3(0.0);
    
    // 1. Sfondo stellato
    final_color += stars(uv * 5.0, uTime); 
    
    // 2. Posizione di Saturno
    vec2 saturn_center = vec2(0.5, 0.0);
    float saturn_radius = 0.3;
    float rings_outer_radius = saturn_radius * 2.0;
    float rings_inner_radius = saturn_radius * 1.1;

    // Applica una piccola rotazione/oscillazione per effetto dinamico
    saturn_center.x += sin(uTime * 0.1) * 0.1;

    // 3. Disegna Saturno e gli Anelli (gli anelli devono essere disegnati prima)
    vec3 rings_color = saturn_rings(uv, saturn_center, rings_inner_radius, rings_outer_radius);
    
    // Simula l'ombra del corpo sugli anelli
    float shadow_factor = 1.0;
    if (length(uv - saturn_center) > saturn_radius * 0.9) {
        shadow_factor = smoothstep(saturn_radius, rings_outer_radius, length(uv - saturn_center));
    }
    rings_color *= shadow_factor;
    
    vec3 body_color = saturn_body(uv, saturn_center, saturn_radius);
    
    // Combina i colori di sfondo, anelli e corpo
    final_color += rings_color;
    final_color += body_color;
    
    // 4. Aggiungi un tocco di colore (uColor) - ad esempio, come luce atmosferica
    final_color += uColor * 0.05; 

    // Saturazione per un look più vivo
    gl_FragColor = vec4(final_color, 1.0);
}