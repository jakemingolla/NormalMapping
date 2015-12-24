//fragment shader, create a file for it named fragment.frag
#version 120

// Make sure they are named exactly as they are in the cpp file
// images are of type 'uniform sampler2D' and then the texture name.

uniform sampler2D brick_image;
uniform sampler2D bump_image;
uniform vec3 lightVector;



void main()
{
	// Extract normals from the normal map
    vec4 bump = texture2D(bump_image, gl_TexCoord[0].xy);

	// Multiply lighting times original texture color to figure out how much light we receive
    vec4 brick = texture2D(brick_image, gl_TexCoord[0].xy);

    // Light vector passed in through uniform pipeline.
    vec4 light = vec4(lightVector, 0.0);

    // Dampens the dot product between the bump mapping and the light vector
    // in order to cap the maximum brightness values. Without this, the
    // color can too easily approach white (255, 255, 255) making the
    // normal mapping too difficult to see.
    float DAMPENING = 0.3;

    vec4 color = brick * dot(bump, light) * DAMPENING;

	// Find the final color
    gl_FragColor = color;

}






