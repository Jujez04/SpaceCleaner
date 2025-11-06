#version 330 core
out vec4 FragColor;

uniform vec3 uColor;       // Usato per una tinta o un colore di accento
uniform vec2 uResolution;  // Risoluzione del viewport
uniform float uTime;       // Tempo trascorso

// Funzione di Rumore 2D (la stessa per coerenza)
float noise(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453123);
}

// Funzione Hash 1D -> 1D per generare numeri casuali in base a una posizione
float hash11(float p) {
    return fract(sin(p * 54.5312) * 43758.5453123);
}

void main() {
    // 1. Coordinate e Preparazione
    vec2 R = uResolution.xy;
    // Centrato e corretto l'aspetto
    vec2 uv = (gl_FragCoord.xy * 2.0 - R) / R.y; 
    
    // Distanza dal centro (r) e angolo (a) - Coordinate Polari
    float r = length(uv);
    float a = atan(uv.y, uv.x);

    vec3 finalColor = vec3(0.0);
    
    // 2. Parametri dell'effetto
    
    // Modifica il tempo per il movimento in avanti
    float speed = uTime * 0.15; 
    // Curvatura/Vortice (aggiunge una rotazione al tunnel)
    float warp = sin(uTime * 0.5) * 0.2; 
    
    // 3. Iterazione e Posizionamento Stelle
    
    // Numero di anelli di stelle
    for (int i = 0; i < 30; i++) {
        // Ogni stella ha una posizione radiale e angolare unica
        // i -> Distanza radiale di base
        float starDist = float(i) * 0.05 + 0.1; 
        
        // Hash per una posizione angolare casuale iniziale
        float starAngleOffset = hash11(float(i)) * 6.28318; // 2*PI
        
        // Posizione angolare animata: Angolo + Rotazione + Movimento vorticoso
        float starAngle = a + warp * starDist;
        
        // Distanza (r) che si muove in avanti nel tempo (starDist)
        // La divisione per starDist fa sì che le stelle più lontane (starDist alto) si muovano più lentamente
        float zPos = mod(starDist - speed / starDist, 3.0); 
        
        // Posizione radiale basata su Z: le stelle "vicine" (zPos basso) sono più lontane dal centro
        float currentR = (3.0 - zPos) * 0.3; // Mappa da [0, 3] a [0.9, 0]
        
        // Posizione finale sulla schermata (raggio e angolo)
        vec2 starPos = vec2(cos(starAngle) * currentR, sin(starAngle) * currentR);

        // 4. Disegno e Colore delle Stelle
        
        // Calcola la distanza tra il frammento corrente (uv) e la posizione della stella
        float d = length(uv - starPos);

        // Luminosità/Dimensione: le stelle "vicine" (zPos basso) sono più grandi e luminose
        float starSize = 0.0005 * (3.0 - zPos);
        
        // Aggiunge la luce della stella con una funzione liscia e concentrata (smoothstep o pow)
        float starBrightness = max(0.0, starSize / d);
        starBrightness = pow(starBrightness, 2.0); // Concentra il punto luminoso
        
        // Colore: mescola un colore di base con il colore dominante dell'uniform (uColor)
        vec3 starColor = mix(vec3(1.0, 0.9, 0.7), uColor, 0.5);
        
        // Aggiunge la luce della stella al colore finale (aggiunta cumulativa)
        finalColor += starColor * starBrightness;
    }

    // 5. Composizione Finale
    
    // Aggiunge un tocco di colore (bloom) al centro per l'effetto tunnel
    float centerGlow = 0.005 / pow(r, 2.0); 
    finalColor += uColor * centerGlow;
    
    // Tono generale scuro e leggermente colorato
    finalColor += vec3(0.01, 0.01, 0.05) * r * 0.5;

    // Esposizione (per evitare che i bianchi siano troppo piatti)
    finalColor = pow(finalColor, vec3(0.6));
    
    FragColor = vec4(finalColor, 1.0);
}