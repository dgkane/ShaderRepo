
      
vs.1.1						// version instruction

dcl_position v0             // declare register data
							// v0 is position of vertices (in model)
dcl_normal v4               // v4 is normal of vertices (in v0)
dcl_texcoord0 v8            // v8 is texture coordinates

m4x4 oPos, v0, c0			// Transforms position of vertices in register v0 using transformation matrix c0 (defined in Shade Lab.cpp line "g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&mat,  4)" ), outputs to oPos (position output)
m4x4 r0, v0, c10			// Transforms position of vertices in v0 using matrix c10, stores back to calculation register r0

mul r0, r0, c4				// Multiplies transformed vertices r0 by light colour constant c4 (ambient light).

m3x3 r1, v4, c0				// Transforms normal of vertices in register v4 using transformation matrix c0, stores in temporary vertex register r1.



// Normalises surface normal
//
dp3 r1.w, r1, r1			
rsq r1.w, r1.w
mul r1, r1, r1.w
//

// dp3 r5, r1, -c5				// Dot product of vector normal and light position (diffuse component of light) stored to register r5.
							// c5 is negated so light is in view direction and front side of viewport is lit.

add r2, r0, -c14			// Adds vector between r0 (vertices) and c14 (eye position - note c14 is subtracted from r0) to temporary register r2

// Normalises eye vactor
//
dp3 r2.w, r2, r2			
rsq r2.w, r2.w
mul r2, r2, r2.w
//

add r3, r2, -c5				// Adds vector between r2 (vertices -> eye) and c5 (light position), stores in temp r3 (this calculates L+V)

// Normalises this vector
//
dp3 r3.w, r3, r3			
rsq r3.w, r3.w
mul r3, r3, r3.w
//



dp3 r4, r3, r1				// Dot product of r3 and r1, calculates N.H

mul r4, r4, r4              // Multiplies N.H by itself.
mul r4, r4, r4				// The more times this is performed the greater the level of specularity (N.B. (I * N.H)^n).
mul r4, r4, r4
mul r4, r4, r4
mul r4, r4, r4

mul r4, r4, c4


dp3 r1, r1, -c5				// Dot product of vector normal (r1) and light position (-c5) (diffuse component of light) overwrites register r1.
							// c5 is negated so light is in view direction and front side of viewport is lit.

mul r5, r1, c4				// Multiplies temporary vertex register r1 by light colour constant c4, stores back to temporary register r1.

dp3 r1, r1, c7

mul r1, r1, c6




add r5, r4, r1				// Adds specular component in r4 with diffuse component already calculated in r1.
							// Global illumination is expresses as a combination of ambient, diffuse and specular components.

// mov oT0, v8

mov oD0, r0					// Outputs vertices r0 to first colour output (ambient light)

mov oD1, r5					// Outputs vertices r5 to the other colour output.
							// r5 is a combination of specular and diffuse components.