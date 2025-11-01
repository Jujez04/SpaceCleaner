#version 330 core
out vec4 FragColor;

// Riceve le coordinate del quad dal Vertex Shader
in vec2 vUV; 

uniform vec4 uColor; 
uniform float uTime; 
uniform vec2 uResolution;


float hash21(vec2 p) {
    p = fract(p * 0.3134597);
    p += dot(p, p + 3.13);
    return fract(p.x * p.y);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);

    vec2 u = f * f * (3.0 - 2.0 * f);

    float a = hash21(i);
    float b = hash21(i + vec2(1.0, 0.0));
    float c = hash21(i + vec2(0.0, 1.0));
    float d = hash21(i + vec2(1.0, 1.0));

    return mix(mix(a, b, u.x), 
               mix(c, d, u.x), u.y);
}

// --- Funzione Principale ---

void main()
{
    // Normalizza le coordinate del frammento per la generazione di rumore
    // Si basa sul World Space (vUV) e aggiunge un aspetto del World Space (Aspect Ratio)
    vec2 uv = vUV;
    uv.x *= uResolution.x / uResolution.y; // Applica l'Aspect Ratio

    vec2 p = uv * 3.0; // Scala il rumore per la nebulosa
    
    // Muove il campo di rumore nel tempo (effetto di gas che scorre)
    p.x += uTime * 0.05; 
    p.y += uTime * 0.03;

    // Applica due strati di rumore a scale diverse (per complessità)
    float n = noise(p);
    n += 0.5 * noise(p * 2.0); // Scala più piccola
    n += 0.25 * noise(p * 4.0); // Scala ancora più piccola

    // Normalizza il rumore in un intervallo di colore più visibile
    n = pow(n * 0.8, 1.5); 
    
    // Mescola il rumore con un colore viola/ciano
    vec3 nebulaColor = mix(vec3(0.0, 0.1, 0.4), vec3(0.4, 0.1, 0.5), n);
    

    vec2 st = gl_FragCoord.xy; // Usiamo le coordinate schermo per la densità stellare

    // Aggiunge un offset di tempo per far sfarfallare le stelle
    st += uTime * 10.0; 
    
    // Generazione del pattern stellare casuale
    float starHash = fract(sin(st.x * 12.9898 + st.y * 78.233) * 43758.5453);
    
    float star = step(0.999, starHash); // Solo 1 su 1000 pixel sono stelle
    
    vec3 starColor = vec3(1.0, 0.95, 0.8);

    // Sfondo di base (uColor) + Nebulosa + Stelle
    vec3 finalColor = uColor.rgb * 0.5; // Scuro
    finalColor = mix(finalColor, nebulaColor, 0.8); // Aggiungi la Nebulosa
    finalColor = mix(finalColor, starColor, star); // Aggiungi le Stelle

    FragColor = vec4(finalColor, 1.0);
}