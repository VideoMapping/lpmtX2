// This vertex shader calculates light and eye vectors for 
// a bump and environment mapping effect.

/* Set the default floating point precision */
precision mediump float;

/* The vertex co-ordinates */
attribute vec4 coord;

/* The normal for the vertex */
attribute vec4 normals;

/* The texture co-ordinates for the vertex */
attribute vec2 texcoord;

/* The tangent vector at the vertex */
attribute vec4 tangent;

/* The projection matrix */
uniform mat4 proj;

/* The modelview matrix */
uniform mat4 modelview;

/* Normal matrix */
uniform mat3 normalMatrix;

/* Position of the light source */
uniform vec3 lightPosition;

/* Direction of the eye in tangent space */
varying vec3 eyeDir;

/* Modelview direction of the eye */
varying vec3 eyeDirB;

/* Diffuse lighting intensity */
varying float lightIntensity;

/* Interpolated normal passed to the fragment shader */
varying vec3 normal;

/* Interpolated texture co-ordinates passed to the fragment shader */
varying float x;
varying float y;

/* Direction of the light in tangent space */
varying vec3 lightDir;

void main() 
{
	/* Assign the texture co-ordinates to our new vertices */
	x = texcoord.x;
	y = texcoord.y;

	/* Calculate the eye direction for the vertex by applying
	 * the camera matrix.
	 */	
	eyeDir = (modelview * coord).xyz;

	/* Calculate basis vectors for tangent space */
	vec3 n = normalize(normalMatrix * normals.xyz);
	vec3 t = normalize(normalMatrix * tangent.xyz);
	vec3 b = cross(n, t);

	/* Project light vector into tangent space */	
	vec3 v;
	v.x = dot(lightPosition, t);
	v.y = dot(lightPosition, b);
	v.z = dot(lightPosition, n);
	lightDir = normalize(v);

	/* Project eye vector into tangent space */
	v.x = dot(eyeDir, t);
	v.y = dot(eyeDir, b);
	v.z = dot(eyeDir, n);
	eyeDir = normalize(v);

	/* Apply the camera matrix */
	gl_Position = proj * modelview * coord;
	
	/* Transform the normals into camera space */
	normal = normalize(normalMatrix * normals.xyz);
	
	/* Calculate the eye direction */
	vec4 pos = modelview * coord;
	eyeDirB = pos.xyz;
	
	/* Compute the diffuse lighting at the vertex */
	lightIntensity = max(dot(normalize(lightPosition - eyeDirB), normal), 0.0);	
}
