//-----------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////ADVACED RENDERING AND REAL TIME GRAPHICS///////////////////////////////
//////////////////////////////////////////////2011-2012///////////////////////////////////////////////
///////////////////////////////////////////SHADERS TUTORIAL 1/////////////////////////////////////////
/////////////////////////////////////////USEING VERTEX SHADERS////////////////////////////////////////
/////////////////////////////////////////////JON PURDY////////////////////////////////////////////////
////////////////////////////////////////UNIVERSITY OF BRADFORD////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////-----------------


#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <strsafe.h>
//#include <D3DX9Shader.h>



struct D3DVERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;
};

#define D3DFVF_D3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D           = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice     = NULL; // Our rendering device

LPD3DXMESH              g_pMesh          = NULL; // Our mesh object in sysmem
D3DMATERIAL9*           g_pMeshMaterials = NULL; // Materials for our mesh
LPDIRECT3DTEXTURE9*     g_pMeshTextures  = NULL; // Textures for our mesh
DWORD                   g_dwNumMaterials = 0L;   // Number of mesh materials
LPDIRECT3DTEXTURE9				g_pTexture   = NULL; // New for texture part
LPDIRECT3DVERTEXSHADER9			g_pVertexShader = NULL;////VS TUTORIAL STUFF
LPDIRECT3DVERTEXBUFFER9			g_pMeshVB;
LPDIRECT3DINDEXBUFFER9			g_pMeshIB;
DWORD							g_dwNumVertices;
DWORD							g_dwNumFaces;

D3DXMATRIX g_matProj;  
D3DXMATRIX g_matView;
D3DXMATRIX g_matWorld;


//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    //Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
   //g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

  g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );


D3DVERTEXELEMENT9 decl[] = 
{
	{ 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
	{ 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
	D3DDECL_END()
};																							////VS TUTORIAL STUFF

LPD3DXBUFFER pCode;                  // Buffer with the assembled shader code				////VS TUTORIAL STUFF
LPD3DXBUFFER pErrorMsgs;             // Buffer with error messages							////VS TUTORIAL STUFF

if(FAILED(D3DXAssembleShaderFromFile( L"SimpleShaderA.vsh",  NULL, NULL, 0,					////VS TUTORIAL STUFF
                            &pCode, &pErrorMsgs )))
							MessageBox(hWnd,L"vertex assemble problem",NULL,NULL);			////VS TUTORIAL STUFF
g_pd3dDevice->CreateVertexShader((DWORD*)pCode->GetBufferPointer(),							////VS TUTORIAL STUFF
                               &g_pVertexShader);											////VS TUTORIAL STUFF
pCode->Release();		

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Load the mesh and build the material and texture arrays
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
	LPDIRECT3DVERTEXBUFFER9 pMeshSourceVB;
    LPDIRECT3DINDEXBUFFER9  pMeshSourceIB;
	D3DVERTEX*              pSrc;
    D3DVERTEX*              pDst;
    // Load the mesh from the specified file


	if( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"cartoonpallet.bmp",
                                       &g_pTexture ) ) )
    MessageBox(NULL, L"Texture Load Problem", NULL, NULL);



    if( FAILED( D3DXLoadMeshFromX( L"skull.x", D3DXMESH_SYSTEMMEM, 
                                   g_pd3dDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials, 
                                   &g_pMesh ) ) )
   
	g_pd3dDevice->SetFVF(D3DFVF_D3DVERTEX );
	g_dwNumVertices  = g_pMesh->GetNumVertices();
	g_dwNumFaces     = g_pMesh->GetNumFaces();

	//Clone the mesh to set the FVF
    LPD3DXMESH pTempSysMemMesh = NULL;
  
    if( FAILED( g_pMesh->CloneMeshFVF( D3DXMESH_SYSTEMMEM, D3DFVF_D3DVERTEX,
                                                 g_pd3dDevice, &pTempSysMemMesh ) ) )
									  MessageBox(NULL,L"Mesh clone problem",NULL,NULL);
	g_pMesh->Release();


     g_pMesh = pTempSysMemMesh;

     //Compute normals in case the meshes have them
    if( g_pMesh )
        D3DXComputeNormals( g_pMesh, NULL );


///Meshes cloned 

	 if( FAILED(g_pd3dDevice->CreateVertexBuffer( g_dwNumVertices * sizeof(D3DVERTEX),
                                      D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED,
                                      &g_pMeshVB, NULL )))
									  MessageBox(NULL,L"Vertex buffer create problem",NULL,NULL);

	 if( FAILED(g_pd3dDevice->CreateIndexBuffer( g_dwNumFaces * 3 * sizeof(WORD),
                                      D3DUSAGE_WRITEONLY,
                                      D3DFMT_INDEX16, D3DPOOL_MANAGED,
                                      &g_pMeshIB, NULL )))
									  MessageBox(NULL,L"Index buffer create problem",NULL,NULL);
	g_pMesh->GetVertexBuffer(&pMeshSourceVB);
    g_pMeshVB->Lock( 0, 0, (void**)&pDst, 0 );
    pMeshSourceVB->Lock( 0, 0, (void**)&pSrc, 0 );
    memcpy( pDst, pSrc, g_dwNumVertices * sizeof(D3DVERTEX) );
    g_pMeshVB->Unlock();
    pMeshSourceVB->Unlock();
    pMeshSourceVB->Release();

    g_pMesh->GetIndexBuffer(&pMeshSourceIB);
    g_pMeshIB->Lock( 0, 0, (void**)&pDst, 0 );
    pMeshSourceIB->Lock( 0, 0, (void**)&pSrc, 0 );
    memcpy( pDst, pSrc, g_dwNumFaces * 3 * sizeof(WORD));
    g_pMeshIB->Unlock();
    pMeshSourceIB->Unlock();
    pMeshSourceIB->Release();

   
    //// Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pMeshMaterials != NULL ) 
        delete[] g_pMeshMaterials;

    if( g_pMeshTextures )
    {
        for( DWORD i = 0; i < g_dwNumMaterials; i++ )
        {
            if( g_pMeshTextures[i] )
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if( g_pMesh != NULL )
        g_pMesh->Release();
    
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{
 // For our world matrix, we will just leave it as the identity
  D3DXMatrixIdentity( &g_matWorld );
  D3DXMatrixRotationYawPitchRoll( &g_matWorld, timeGetTime()/1000.0f, 
   timeGetTime()/700.0f, timeGetTime()/850.0f );
//D3DXMatrixRotationYawPitchRoll( &g_matWorld, 0.0f, 
 //  -0.2f, 0.0f );

   g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld );
  
  // Set up our view matrix. A view matrix can be defined given an eye point,
  // a point to lookat, and a direction for which way is up. Here, we set the
  // eye five units back along the z-axis and up three units, look at the
  // origin, and define "up" to be in the y-direction.
  D3DXMatrixLookAtLH( &g_matView, &D3DXVECTOR3( 0.0f, 0.0f,-20.0f ),
                                &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
   g_pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );
  
  // For the projection matrix, we set up a perspective transform (which
  // transforms geometry from 3D view space to 2D viewport space, with
  // a perspective divide making objects smaller in the distance). To build
  // a perpsective transform, we need the field of view (1/4 pi is common),
  // the aspect ratio, and the near and far clipping planes (which define at
  // what distances geometry should be no longer be rendered).
  D3DXMatrixPerspectiveFovLH( &g_matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_matProj );
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render(HWND hWnd)
{
    // Clear the backbuffer and the zbuffer
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );
    

 g_pd3dDevice->BeginScene();



// End the scene.
 g_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
SetupMatrices();

//   Render the vertex buffer contents 


  // Set up the vertex shader constants
 
    D3DXMATRIX mat;
	D3DXMATRIX mat1;
    D3DXMatrixMultiply( &mat, &g_matWorld, &g_matView );
	D3DXMatrixTranspose( &mat1, &mat );

    D3DXMatrixMultiply( &mat, &mat, &g_matProj );
    D3DXMatrixTranspose( &mat, &mat );

    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&mat,  4);


    // plug the diffuse color into constant register 4
    float color[4] = { 0.7f, 0.7f, 0.7f, 0.7f };
    g_pd3dDevice->SetVertexShaderConstantF(4, (float*)color, 1);
 
	float fLightPosition[3] = {0.0f, 0.0f, 2.0f};
	g_pd3dDevice->SetVertexShaderConstantF(5, (float*)fLightPosition,1);
    //Secular Lighting position and colour
    // plug the diffuse color into constant register 4
  


	g_pd3dDevice->SetTexture( 0, g_pTexture );


 ///Change back to get the sample back
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);



  POINT ptCursor;
        GetCursorPos( &ptCursor );
        ScreenToClient( hWnd, &ptCursor );
		D3DXVECTOR3 v;
        v.x =  ( ( ( 2.0f * ptCursor.x ) / 600 ) - 1 ) / g_matProj._11;
        v.y = -( ( ( 2.0f * ptCursor.y ) / 600 ) - 1 ) / g_matProj._22;
        v.z =  1.0f;

	


 g_pd3dDevice->SetFVF(D3DFVF_D3DVERTEX );
 g_pd3dDevice->SetStreamSource( 0, g_pMeshVB, 0, sizeof(D3DVERTEX) );
 g_pd3dDevice->SetVertexShader( g_pVertexShader); // set our vertex shader active
 g_pd3dDevice->SetIndices( g_pMeshIB);
 g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,0,
                                     0, g_dwNumVertices,
                                     0, g_dwNumFaces );
    g_pd3dDevice->EndScene();
    
    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

}



//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3D Tutorial", NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 06: Meshes", 
                              WS_OVERLAPPEDWINDOW, 100, 100, 600, 600,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    { 
        // Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg; 
            ZeroMemory( &msg, sizeof(msg) );
            while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                    Render(hWnd);
            }
        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}


