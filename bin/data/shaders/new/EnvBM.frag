// This fragment shader applies a bump map to the object, and
// then uses an environment map to produce a reflective surface. 

/* Use of the precision keyword is a GLES requirement */
precision mediump float;

/* X and Y axis unit vectors */
const vec3 XUnitVec = vec3(1.0, 0.0, 0.0);
const vec3 YUnitVec = vec3(0.0, -1.0, 0.0);

/* Environment map sampler */
uniform sampler2D Texture;

/* Base colour of the object */
uniform vec3 surfaceColour;

/* Mix ratio between base colour and reflection */
uniform float mixRatio;

/* Level of ambient lighting */
uniform float ambientlev;

/* Number of bumps per unit area */
uniform float bumpDensity;

/* Diameter of bumps */
uniform float bumpSize;

/* Specular highlight size */
uniform float specularFactor;

/* Interpolated normal from vertex shader */
varying vec3 normal;

/* Interpolated diffuse lighting intensity */
varying float lightIntensity;

/* Interpolated direction to the eye in tangent space */
varying vec3 eyeDir;

/* Interpolated modelview direction of the eye */
varying vec3 eyeDirB;

/* Interpolated texture co-ordinates from vertex shader */
varying float x;
varying float y;

/* Interpolated direction of the light in tangent space */
varying vec3 lightDir;

/* Fragment shader entry point */
void main(void)
{
	vec3 normalDelta;
	vec3 litColour;
	vec3 reflectDir;

	/* First, compute the bump mapping */

	/* Scale the bump texture according to density */
	vec2 c = bumpDensity * vec2(x,y);
	
	/* Calculate the perturbation vector as the distance
	 * center of the bump.
	 */
	vec2 p = fract(c) - vec2(0.5);
	float d, f;	
	d = p.x * p.x + p.y * p.y;
	
	/* Calculate the normalization factor for the 
	 * perturbation normal.
	 */
	f = 1.0 / sqrt(d + 1.0);

	/* Test if the fragment is in a bump, and if not, flatten
	 * the surface.
	 */
	if (d >= bumpSize) {
		/* Outside the bump area, use the original normal */
		normalDelta = normal; 
	} else {
		/* Normalize the perturbation vector */
		normalDelta = vec3(p.x, p.y, 1.0) * f;
	}
	
	/* Compute the specular and diffuse lighting components using the
	 * perturbed "bumpy" normals.
	 */
	litColour = surfaceColour * max(dot(normalDelta, lightDir), 0.0);
	reflectDir = reflect(lightDir, normalDelta);
	float spec = max(dot(eyeDir, reflectDir), 0.0);
	spec = pow(spec, 6.0);
	spec *= specularFactor;
	litColour = min(litColour + spec, vec3(1.0));


	
	/* Compute the environment mapping, using the perturbed bumpy 
	 * normal. The reflections seen in the bumps are faked.
	 */
	if(d >= bumpSize) {
		reflectDir = reflect(eyeDirB, normal);
	} else {
		reflectDir = reflect(eyeDir, normalDelta);
	}
	vec2 index;
	
	index.t = dot(normalize(reflectDir), YUnitVec);
	reflectDir.y = 0.0;
	index.s = dot(normalize(reflectDir), XUnitVec) * 0.5;

	/* Test whether reflection is towards the front or back */	
	if (reflectDir.z >= 0.0)
	{
		/* Towards the front - index the texture from 0.25 to 0.75 */
		index = (index + 1.0) * 0.5;
		index.t = 1.0 - index.t;
	}
	else
	{
		/* Towards the back - index the texture from 0.75 to 1.25
		 * using the GL_REPEAT texturing mode to map 1.0 - 1.25 to
		 * 0 - 0.25.
		 */
		index.t = 1.0 - (index.t + 1.0) * 0.5;
		index.s = (-index.s) * 0.5 + 1.0;
	}
	
	/* Look up a color from the environment map based on the
	 * index vector.
	 */
	vec3 envColour = vec3(texture2D(Texture, index));
	
	/* Mix the colouring from the environment map with the base
	 * surface color.
	 */
	envColour = mix(envColour, surfaceColour, mixRatio);
	
	/* Light the resulting surface */ 
	envColour = lightIntensity * envColour + ambientlev * envColour;
	
	/* Assign the lit surface and environment color to the fragment */
    gl_FragColor = mix(vec4(litColour, 1.0), vec4(envColour, 1.0), 0.6);
}