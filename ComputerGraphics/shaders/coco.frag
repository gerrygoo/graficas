void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = fragCoord / iResolution.xy;
    
    vec2 q = p - vec2(0.7, 0.5);
    
    vec3 color = mix(
    	vec3(0.8, 0.6, 0.0),
        vec3(0.0, 0.0, 0.8),
        sqrt(p.y)
    );
    
    float r = 0.2 + 0.1 * cos(
        atan(q.y, q.x) * 10.0 + 20.0 * ( 0.75 + 0.25 * cos(iTime) ) * q.x + 1.0);
    
    color *= smoothstep(r, r + 0.01, length(q));
   	
    r = 0.015;
    r += 0.002 * cos(100.0 * q.y);
    r += exp(-70.0 * p.y);
    color *= 1.0 -
        	(1.0 - smoothstep(r, r + 0.002, abs(q.x - 0.25 * sin(4.0 * q.y))))*
         	(1.0 - smoothstep(0.0, 0.1, q.y));
    
    
    fragColor = vec4(color,1.0);
}