
      
vs.1.1						// version of shader

dcl_position v0             // fills register v0 with vertices (view matrix)
dcl_normal v4               // fills register v4 with transformation matrix (projection matrix)
dcl_texcoord0 v8            // v8 is texture coordinates

m4x4 oPos, v0, c0			// transform vertices by view/projection matrix
m3x3 r1, v4, c0

dp3 r1.w, r1, r1
rsq r1.w, r1.w
mul r1, r1, r1.w


dp3 r1, r1, -c5			// do a dot product with the light to get the angle between the light and the normal - lamberts term
mul r2, r1, c4			// multiply the light colour by the diffuse term
mul r2, c6, r2		    // multiply result by vertex colour


mov oD0, r1
mov oD0, r2			// output the colour of the vertex

mul r3,v0,c9.y
mul r4, v0, c9.y



mov oT0, r3
mov oT1, r4