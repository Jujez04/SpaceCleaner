#version 330 core
out vec4 FragColor;

uniform vec3 uColor;       // Usato per il colore dominante del pianeta (es. arancio/marrone)
uniform vec2 uResolution;  // Risoluzione del viewport
uniform float uTime;       // Tempo trascorso

// Funzione di Rumore 2D
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Fractional Brownian Motion (FBM)
// Usato per creare le turbolenze e le bande nuvolose
float fbm(vec2 p) {
    float v = 0.0;
    float a = 0.5;
    mat2 rot = mat2(0.8, -0.6, 0.6, 0.8);
    for (int i = 0; i < 4; i++) {
        v += a * noise(p);
        p = rot * p * 2.0;
        a *= 0.5;
    }
    return v;
}

void main() {
    // 1. Coordinate
    vec2 R = uResolution.xy;
    vec2 uv = (gl_FragCoord.xy * 2.0 - R) / R.y; // Centrato e corretto l'aspetto
    
    vec3 finalColor = vec3(0.01, 0.0, 0.02); // Sfondo spazio molto scuro

    // 2. Geometria Sferica (Pianeta)
    float r = length(uv);
    float planetRadius = 0.55;
    float planetMask = smoothstep(planetRadius, planetRadius - 0.01, r); // Maschera del pianeta

    // 3. Texture Procedurale (Bande di Nuvole)
    
    // Deforma le coordinate UV (specialmente lungo Y per le bande)
    vec2 p = uv;
    p.y *= 8.0; // Alto fattore per creare bande orizzontali strette
    
    // Aggiunge movimento rotatorio (simula il pianeta che ruota velocemente)
    p.x += uTime * 0.5; 
    
    // Aggiunge turbolenza usando FBM
    p.x += fbm(uv * 10.0 + uTime * 0.1) * 0.1;
    
    // Turbolenza aggiuntiva per la banda
    float turbulence = fbm(p + uTime * 0.2);
    
    // Usa sin(p.y) per definire le bande e modula con la turbolenza
    float bandPattern = sin(p.y * 3.0);
    bandPattern = smoothstep(-0.5, 0.5, bandPattern);
    
    // Miscela il pattern con la turbolenza per un look "gassoso"
    float cloudDensity = mix(bandPattern, turbulence, 0.6); // 60% turbolenza
    cloudDensity = pow(cloudDensity, 1.5);

    // 4. Mappatura del Colore
    
    // Colore primario del pianeta (Arancio/Marrone) basato su uColor
    vec3 bandColor1 = uColor * 0.8; 
    // Colore secondario (Bianco/Giallo) per le nuvole chiare
    vec3 bandColor2 = vec3(1.0, 0.8, 0.5); 
    
    vec3 planetColor = mix(bandColor1, bandColor2, cloudDensity);

    // 5. Ombreggiatura (Semplice)
    
    // Simula una luce proveniente da sinistra in alto
    float lightDir = uv.x * 0.8 - uv.y * 0.2;
    float shadow = 1.0 - smoothstep(0.0, planetRadius, r); // Ombra al bordo
    float shading = pow(clamp(lightDir + 1.0, 0.0, 1.0) * shadow, 0.5);
    
    planetColor *= shading;

    // 6. Atmosfera (Semplice)
    
    float atmosphere = 1.0 - r / planetRadius; // Massima al centro, zero al bordo del pianeta
    atmosphere = pow(atmosphere, 10.0); // Concentra l'effetto sul bordo
    vec3 atmosphereColor = vec3(0.6, 0.9, 1.0) * atmosphere * 0.5; // Tinta bluastra

    // 7. Composizione Finale
    
    // Applica maschera e aggiunge atmosfera
    finalColor += planetColor * planetMask; 
    finalColor += atmosphereColor; 

    // Aggiungi stelle (stile semplice)
    float stars = step(0.997, noise(uv * 150.0 + uTime * 0.05));
    finalColor += vec3(stars * 0.5);
    
    FragColor = vec4(finalColor, 1.0);
}