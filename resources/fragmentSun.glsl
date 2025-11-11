#version 330 core
out vec4 FragColor;

uniform vec3 uColor;      // Tinta dello sfondo/bagliore esterno (impostalo su blu scuro/nero, e.g., vec3(0.01, 0.01, 0.05))
uniform vec2 uResolution; // Risoluzione del viewport
uniform float uTime;      // Tempo trascorso

// Funzione di Rumore 2D
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    // 1. Coordinate e Preparazione
    vec2 R = uResolution.xy;
    vec2 uv = (gl_FragCoord.xy * 2.0 - R) / R.y;
    
    // Distanza dal centro (r) e angolo (a)
    float r = length(uv);
    float a = atan(uv.y, uv.x);

    vec3 finalColor = vec3(0.0);
    
    // --- 2. GIGANTE BAGLIORE CENTRALE (Buchi Nero / Stella Lontana) ---
    
    // Il fenomeno è molto più grande, usiamo un decadimento meno ripido (es. pow(r, 1.0) o pow(r, 1.5))
    // La scala di r è modificata per rendere il fenomeno visivamente più grande.
    float scaledR = r * 0.5; // Rende il bagliore circa il doppio più grande
    
    // Nucleo (Rosso/Arancio per contrasto)
    float coreGlow = 0.05 / pow(scaledR, 2.0);
    vec3 coreColor = vec3(0.7, 0.2, 0.1) * coreGlow; // Rosso scuro/Arancio

    // Aggiungi un piccolo punto bianco al centro per l'aberrazione
    coreColor += vec3(1.0, 1.0, 1.0) * (0.00001 / pow(r, 4.0));
    
    finalColor += coreColor;

    // Bagliore Esterno/Disco di Accrescimento (Tinta Blu/Viola - uColor)
    float coronaGlow = 0.01 / pow(scaledR, 1.0); // Decadimento lento
    
    // Usa il rumore per simulare vortici o gas
    float vortexNoise = noise(uv * 2.0 + uTime * 0.1) * 0.5 + 0.5;
    
    vec3 coronaColor = mix(uColor * 2.0, vec3(0.3, 0.1, 0.6), vortexNoise); // Tinta blu-viola
    
    finalColor += coronaColor * coronaGlow;
    
    // --- 3. RAGGI SOTTILI E LUNGHI ---
    
    // Raggi molto più sottili e distesi che sembrano estendersi fuori dall'inquadratura.
    
    // Applica una distorsione angolare costante per l'effetto raggio
    float rays = pow(sin(a * 5.0) * 0.5 + 0.5, 5.0); // 5 raggi (più sottili grazie all'esponente 5.0)
    
    // Il raggio svanisce più lentamente (pow(r, 0.8)) per estendersi lontano dal centro
    float rayFade = 0.001 / pow(r, 0.8);

    // Tinta dei raggi (Blu Scuro)
    finalColor += uColor * rays * rayFade * 0.5;

    // --- 4. Sfondo: Spazio Profondo e Stelle Lontane ---

    // Lo sfondo è dominato dal nero/blu scuro (uColor)
    vec3 spaceColor = uColor * 0.1; // Scurisci l'uniforme
    
    // Aggiungi un leggero rumore per le nebulose lontane (appena percettibile)
    float deepSpaceNoise = noise(uv * 10.0 + uTime * 0.01) * 0.02;
    
    finalColor += spaceColor + deepSpaceNoise;

    // --- 5. Composizione Finale ---
    
    // Esposizione (per mantenere i colori scuri)
    finalColor = pow(finalColor, vec3(0.7)); 
    
    // Saturazione generale bassa per l'effetto "spazio freddo"
    finalColor = mix(finalColor, dot(finalColor, vec3(0.333)) * vec3(1.0), 0.2); // Desaturazione leggera
    
    FragColor = vec4(finalColor, 1.0);
}