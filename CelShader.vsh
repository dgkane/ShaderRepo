
      
vs.1.1						

dcl_position v0             
dcl_normal v4               
dcl_texcoord0 v8            

m4x4 oPos, v0, c0			
m3x3 r0, v4, c0				


dp3 r1, r0, -c11			// get dot product (angle between) of normal (r0) and view angle (c11, negated)
mul r1, r1, c4				// multiplies angle by light colour - seems to give a better effect

mov oT0, r1					// r1 moved to texture ouptut, so is used as a reference to a point on the texture.
							// Texture consists of four grayscale colours.