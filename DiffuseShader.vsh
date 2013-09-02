
vs.1.1						// version instruction

dcl_position v0             // declare register data
							// v0 is position of vertices (in model)
dcl_normal v4               // v4 is normal of vertices (in v0)
dcl_texcoord0 v8            // v8 is texture coordinates

m4x4 oPos, v0, c0			// Transforms position of vertices in register v0 using transformation matrix c0 (defined in Shade Lab.cpp line "g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&mat,  4)" ), outputs to oPos (position output)
m3x3 r1, v4, c0				// Transforms normal of vertices in register v4 using transformation matrix c0, stores in temporary vertex register r1.

dp3 r1.w, r1, r1			// This routine performs normalisation of a vector (in this case the normals).
rsq r1.w, r1.w
mul r1, r1, r1.w


dp3 r1, r1, -c5				// Dot product of vector normal and light position (diffuse component of light) stored back to register r1.
mul r2, r1, c4				// multiply the light colour by the diffuse term

mov oD1, r2					// output the colour of the vertex
