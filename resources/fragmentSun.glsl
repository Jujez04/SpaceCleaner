#version 330 core
out vec4 FragColor;

uniform float uTime;
// Uniform uResolution implicita (necessaria per gl_FragCoord)
// Uniform uColor non usata, ma puoi attivarla per la tinta

// Funzione di Rumore 1D (per il colore)
float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

// Funzione di Rumore 2D (simile al tuo)
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Fractional Brownian Motion (FBM)
// Usato per creare la struttura nuvolosa della nebulosa
float fbm(vec2 p) {
    float v = 0.0;
    float a = 0.5;
    mat2 rot = mat2(cos(0.5), -sin(0.5), sin(0.5), cos(0.5));
    for (int i = 0; i < 4; i++) {
        v += a * noise(p);
        p = rot * p * 2.0;
        a *= 0.5;
    }
    return v;
}

void main() {
    // Coordinate normalizzate (centrate e scalate)
    vec2 uv = (gl_FragCoord.xy / 600.0) * 2.0 - 1.0; // Assumo una risoluzione di base 800x600
    uv.x *= 800.0 / 600.0; // Correzione dell'aspetto (adatta questi valori alla tua risoluzione effettiva)
    
    vec3 finalColor = vec3(0.0);
    
    // 1. Distorsione e Movimento
    // Muove le coordinate UV nel tempo (effetto di scorrimento)
    vec2 p = uv * 3.0 + vec2(uTime * 0.1, uTime * 0.05); 
    
    // Distorce le UV usando il rumore FBM, creando un effetto "nuvola"
    float d = fbm(p);
    d = abs(d * d - 0.5); // Sottrae e prende il valore assoluto per creare bordi più definiti
    
    // 2. Struttura del Colore
    
    // Modulazione del colore usando il tempo per un effetto pulsante
    float hueShift = sin(uTime * 0.2) * 0.5 + 0.5; 
    
    // Combinazione di rumore FBM, distanza dal centro e tempo
    float nebulaDensity = fbm(uv * 1.5 + d * 0.5 + uTime * 0.3) * 0.7;
    
    // Rende il centro più luminoso
    nebulaDensity += pow(1.0 - length(uv), 3.0) * 0.2; 
    
    // 3. Mappatura del Colore (Gradienti Spaziali)
    
    vec3 colorA = vec3(0.0, 0.1, 0.4 + hueShift * 0.2);     // Blu scuro/Viola
    vec3 colorB = vec3(0.9, 0.2, 0.5 - hueShift * 0.2);     // Rosso acceso/Magenta
    vec3 colorC = vec3(1.0, 0.8, 0.4);                     // Giallo/Arancio
    
    // Miscela i colori in base alla densità
    vec3 nebulaColor = mix(colorA, colorB, nebulaDensity);
    nebulaColor = mix(nebulaColor, colorC, pow(nebulaDensity, 4.0)); // Aggiunge colore C alle zone più dense

    finalColor += nebulaColor * 1.5; // Amplifica il colore
    
    // 4. Aggiungi le Stelle (Sfondo)
    // Usa un rumore basico per le stelle lontane
    float starfield = step(0.998, noise(uv * 200.0));
    finalColor += vec3(starfield * 0.2);
    
    // 5. Output
    // Aggiusta la luminosità e l'esposizione
    finalColor = pow(finalColor, vec3(0.7)); 
    
    FragColor = vec4(finalColor, 1.0);
}