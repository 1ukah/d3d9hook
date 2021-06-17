#include "Includes.h"

LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS g_d3dPP = {};
IDirect3DPixelShader9* shaderBack;
IDirect3DPixelShader9* shaderFront;

UINT Stride;
D3DVERTEXELEMENT9 declaration[MAXD3DDECLLENGTH];
UINT numElements, mStartRegister, mVectorCount, vSize, pSize;
IDirect3DVertexShader9* vShader;
IDirect3DPixelShader9* pShader;

#define _CSS
//#define _CSGO

#ifdef _CSS
#define TR ((Stride == 32) && (PHOENIX || ELITE || ARCTIC || GUERILLA))
#define CT ((Stride == 32) && (SEAL || GSG || SAS || GIGN))

#define PHOENIX ((numVertices == 3265) && (primCount == 5015))
#define ELITE ((numVertices == 3087) && (primCount == 4911))
#define ARCTIC ((numVertices == 3210) && (primCount == 4503))
#define GUERILLA ((numVertices == 3561) && (primCount == 5066))

#define SEAL ((numVertices == 3887) && (primCount == 4974))
#define GSG ((numVertices == 3206) && (primCount == 4872))
#define SAS ((numVertices == 3417) && (primCount == 5030))
#define GIGN ((numVertices == 3304) && (primCount == 5003))
#endif // _CSS

#ifdef _CSGO
#define ct_idf_legs ((Stride==32) && (numVertices==2118) && (primCount==3354))
#define ct_idf_head1 ((Stride==32) && (numVertices==1761) && (primCount==2681))
#define ct_idf_head2 ((Stride==32) && (numVertices==1677) && (primCount==2713))
#define ct_idf_body ((Stride==32) && (numVertices==4523) && (primCount==6700))

#define ct_idf ct_idf_legs || ct_idf_head1 || ct_idf_head2 || ct_idf_body


//SAS
#define ct_sas_legs ((Stride==32) && (numVertices==4442) && (primCount==5475))
#define ct_sas_head ((Stride==32) && (numVertices==1215) && (primCount==1749))
#define ct_sas_body ((Stride==32) && (numVertices==5443) && (primCount==8008))

//New d3d model rec for SAS(12 / 13 / 2016 csgo update) :
#define new_sas_head ((Stride==32) && (numVertices == 5299) && (primCount == 7167))//head -
#define new_sas_eyes ((Stride==32) && (numVertices == 136) && (primCount == 204))//eyes -
#define new_sas_chest ((Stride==32) && (numVertices == 1967) && (primCount == 2720))//chest -
#define new_sas_hands ((Stride==32) && (numVertices == 2052) && (primCount == 2966))//hands -
#define new_sas_legs ((Stride==32) && (numVertices == 2482) && (primCount == 3414))//legs -

#define ct_sas ct_sas_legs || ct_sas_head || ct_sas_body || new_sas_head || new_sas_eyes || new_sas_chest || new_sas_legs


//FBI
#define ct_fbi_legs ((Stride==32) && (numVertices==1646) && (primCount==2714))
#define ct_fbi_body ((Stride==32) && (numVertices==5395) && (primCount==8657))
#define ct_fbi_head1 ((Stride==32) && (numVertices==1459) && (primCount==2566))
#define ct_fbi_head2 ((Stride==32) && (numVertices==2223) && (primCount==3672))
#define ct_fbi_head3 ((Stride==32) && (numVertices==2778) && (primCount==4244))
#define ct_fbi_head4 ((Stride==32) && (numVertices==993) && (primCount==1722))
#define ct_fbi_head5 ((Stride==32) && (numVertices==2957) && (primCount==4234))

#define ct_fbi ct_fbi_legs || ct_fbi_body || ct_fbi_head1 || ct_fbi_head2 || ct_fbi_head3 || ct_fbi_head4 || ct_fbi_head5


//SWAT
#define ct_swat_legs ((Stride==32) && (numVertices==3650) && (primCount==4030))
#define ct_swat_body1 ((Stride==32) && (numVertices==6016) && (primCount==8022))
#define ct_swat_body2 ((Stride==32) && (numVertices==5675) && (primCount==7713))
#define ct_swat_body3 ((Stride==32) && (numVertices==6112) && (primCount==8261))
#define ct_swat_body4 ((Stride==32) && (numVertices==6777) && (primCount==8930))
#define ct_swat_body5 ((Stride==32) && (numVertices==6087) && (primCount==8226))
#define ct_swat_head1 ((Stride==32) && (numVertices==1883) && (primCount==2986))
#define ct_swat_head2 ((Stride==32) && (numVertices==1917) && (primCount==3004))
#define ct_swat_head3 ((Stride==32) && (numVertices==1835) && (primCount==2898))
#define ct_swat_head4 ((Stride==32) && (numVertices==1980) && (primCount==3074))
#define ct_swat_head5 ((Stride==32) && (numVertices==1834) && (primCount==2898))

#define ct_swat ct_swat_legs || ct_swat_body1 || ct_swat_body2 || ct_swat_body3 || ct_swat_body4 || ct_swat_body5 || ct_swat_head1 || ct_swat_head2 || ct_swat_head3 || ct_swat_head4 || ct_swat_head5


//GSG9
#define ct_gsg9_legs ((Stride==32) && (numVertices==1410) && (primCount==2352))
#define ct_gsg9_body ((Stride==32) && (numVertices==3301) && (primCount==5424))
#define ct_gsg9_head1 ((Stride==32) && (numVertices==2113) && (primCount==3391))
#define ct_gsg9_head2 ((Stride==32) && (numVertices==2157) && (primCount==3461))
#define ct_gsg9_head3 ((Stride==32) && (numVertices==2151) && (primCount==3459))

#define ct_gsg9 ct_gsg9_legs || ct_gsg9_body || ct_gsg9_head1 || ct_gsg9_head2 || ct_gsg9_head3


//GIGN
#define ct_gign_legs ((Stride==32) && (numVertices==1258) && (primCount==1950))
#define ct_gign_body ((Stride==32) && (numVertices==4854) && (primCount==7410))
#define ct_gign_head1 ((Stride==32) && (numVertices==1645) && (primCount==2568))
#define ct_gign_head2 ((Stride==32) && (numVertices==1611) && (primCount==2522))
#define ct_gign_head3 ((Stride==32) && (numVertices==1601) && (primCount==2502))

#define ct_gign ct_gign_legs || ct_gign_body || ct_gign_head1 || ct_gign_head2 || ct_gign_head3


//SEAL
#define ct_seal_legs1 ((Stride==32) && (numVertices==2924) && (primCount==3540))
#define ct_seal_legs2 ((Stride==32) && (numVertices==2923) && (primCount==3540))
#define ct_seal_body1 ((Stride==32) && (numVertices==5547) && (primCount==8003))
#define ct_seal_body2 ((Stride==32) && (numVertices==5524) && (primCount==8106))
#define ct_seal_body3 ((Stride==32) && (numVertices==5680) && (primCount==8166))
#define ct_seal_body4 ((Stride==32) && (numVertices==5334) && (primCount==7910))
#define ct_seal_body5 ((Stride==32) && (numVertices==5436) && (primCount==8003))
#define ct_seal_head1 ((Stride==32) && (numVertices==2539) && (primCount==3527))
#define ct_seal_head2 ((Stride==32) && (numVertices==3137) && (primCount==3948))
#define ct_seal_head3 ((Stride==32) && (numVertices==3245) && (primCount==4158))
#define ct_seal_head4 ((Stride==32) && (numVertices==2963) && (primCount==3792))
#define ct_seal_head5 ((Stride==32) && (numVertices==4084) && (primCount==4697))

#define ct_seal ct_seal_legs1 || ct_seal_legs2 || ct_seal_body1 || ct_seal_body2 || ct_seal_body3 || ct_seal_body4 || ct_seal_body5 || ct_seal_head1 || ct_seal_head2 || ct_seal_head3 || ct_seal_head4 || ct_seal_head5

//CT Models
#define CT_Models (ct_idf || ct_sas || ct_fbi || ct_swat || ct_gsg9 || ct_gign || ct_seal)

//CT Heads
#define CT_Heads (ct_idf_head1 || ct_idf_head2 || ct_sas_head || ct_fbi_head1 || ct_fbi_head2 || ct_fbi_head3 || ct_fbi_head4 || ct_fbi_head5 || ct_swat_head1 || ct_swat_head2 || ct_swat_head3 || ct_swat_head4 || ct_swat_head5 || ct_gsg9_head1 || ct_gsg9_head2 || ct_gsg9_head3 || ct_gign_head1 || ct_gign_head2 || ct_gign_head3 || ct_seal_head1 || ct_seal_head2 || ct_seal_head3 || ct_seal_head4 || ct_seal_head5)

//CT Legs
#define CT_LEGS (ct_idf_legs||ct_sas_legs||new_sas_legs||ct_fbi_legs||ct_swat_legs||ct_gsg9_legs||ct_gign_legs||ct_seal_legs1||ct_seal_legs2)

//////////////////////
// T //
//////////////////////


//ELITE CREW
#define t_elite1_legs ((Stride==32) && (numVertices==2531) && (primCount==3888))
#define t_elite1_head ((Stride==32) && (numVertices==1134) && (primCount==2024))
#define t_elite1_body ((Stride==32) && (numVertices==3606) && (primCount==5856))
#define t_elite2_legs ((Stride==32) && (numVertices==1889) && (primCount==3088))
#define t_elite2_head ((Stride==32) && (numVertices==1310) && (primCount==2302))
#define t_elite2_body ((Stride==32) && (numVertices==3816) && (primCount==5930))
#define t_elite3_legs ((Stride==32) && (numVertices==2416) && (primCount==3778))
#define t_elite3_head ((Stride==32) && (numVertices==1430) && (primCount==2422))
#define t_elite3_body ((Stride==32) && (numVertices==2587) && (primCount==4334))
#define t_elite4_legs ((Stride==32) && (numVertices==2138) && (primCount==3424))
#define t_elite4_head ((Stride==32) && (numVertices==1352) && (primCount==2268))
#define t_elite5_legs ((Stride==32) && (numVertices==1986) && (primCount==3044))
#define t_elite5_head ((Stride==32) && (numVertices==1372) && (primCount==2286))
#define t_elite5_body ((Stride==32) && (numVertices== 3162) && (primCount==5182))


#define t_elite t_elite1_legs || t_elite1_head || t_elite1_body || t_elite2_legs || t_elite2_head || t_elite2_body || t_elite3_legs || t_elite3_head || t_elite3_body || t_elite4_legs || t_elite4_head || t_elite5_legs || t_elite5_head || t_elite5_body


//Separatist
#define t_separatist_legs ((Stride==32) && (numVertices==2536) && (primCount==4196))
#define t_separatist_head ((Stride==32) && (numVertices==1106) && (primCount==2018))
#define t_separatist_body ((Stride==32) && (numVertices==3210) && (primCount==5382))

#define t_separatist t_separatist_legs || t_separatist_head || t_separatist_body


//Anarchist
#define t_anarchist_legs ((Stride==32) && (numVertices==1671) && (primCount==2876))
#define t_anarchist_head ((Stride==32) && (numVertices==1925) && (primCount==3314))
#define t_anarchist_body ((Stride==32) && (numVertices==3881) && (primCount==6774))

#define t_anarchist t_anarchist_legs || t_anarchist_head || t_anarchist_body


//Professional
#define t_professional_legs ((Stride==32) && (numVertices==2056) && (primCount==3436))
#define t_professional_body ((Stride==32) && (numVertices==5116) && (primCount==8253))
#define t_professional_head1 ((Stride==32) && (numVertices==927) && (primCount==1626))
#define t_professional_head2 ((Stride==32) && (numVertices==646) && (primCount==1248))
#define t_professional_head3 ((Stride==32) && (numVertices==1053) && (primCount==1968))
#define t_professional_head4 ((Stride==32) && (numVertices==1197) && (primCount==2256))
#define t_professional_head5 ((Stride==32) && (numVertices==924) && (primCount==1632))
#define t_professional_glasses1 ((Stride==32) && (numVertices==266) && (primCount==348))
#define t_professional_glasses2 ((Stride==32) && (numVertices==200) && (primCount==276))

#define t_professional t_professional_legs || t_professional_body || t_professional_head1 || t_professional_head2 || t_professional_head3 || t_professional_head4 || t_professional_head5 || t_professional_glasses1 || t_professional_glasses2


//Phoenix
#define t_phoenix_legs ((Stride==32) && (numVertices==2599) && (primCount==4097))
#define t_phoenix_body ((Stride==32) && (numVertices==2944) && (primCount==4916))
#define t_phoenix_head ((Stride==32) && (numVertices==876) && (primCount==1630))

#define t_phoenix t_phoenix_legs || t_phoenix_body || t_phoenix_head


//Balkan
#define t_balkan_legs1 ((Stride==32) && (numVertices==1525) && (primCount==2525))
#define t_balkan_legs2 ((Stride==32) && (numVertices==1273) && (primCount==2113))
#define t_balkan_body ((Stride==32) && (numVertices==5215) && (primCount==8130))
#define t_balkan_head1 ((Stride==32) && (numVertices==1214) && (primCount==2292))
#define t_balkan_head2 ((Stride==32) && (numVertices==1215) && (primCount==2286))
#define t_balkan_head3 ((Stride==32) && (numVertices==1291) && (primCount==2437))
#define t_balkan_head4 ((Stride==32) && (numVertices==1345) && (primCount==2488))
#define t_balkan_head5 ((Stride==32) && (numVertices==1624) && (primCount==2936))

#define t_balkan t_balkan_legs1 || t_balkan_legs2 || t_balkan_body || t_balkan_head1 || t_balkan_head2 || t_balkan_head3 || t_balkan_head4 || t_balkan_head5


//Pirate
#define t_pirate_legs1 ((Stride==32) && (numVertices==934) && (primCount==1525))
#define t_pirate_legs2 ((Stride==32) && (numVertices==991) && (primCount==1525))
#define t_pirate_body ((Stride==32) && (numVertices==7900) && (primCount==9248))
#define t_pirate_head1 ((Stride==32) && (numVertices==1996) && (primCount==2906))
#define t_pirate_head2 ((Stride==32) && (numVertices==1183) && (primCount==2045))
#define t_pirate_head3 ((Stride==32) && (numVertices==1380) && (primCount==2493))
#define t_pirate_head4 ((Stride==32) && (numVertices==1314) && (primCount==2367))

#define t_pirate t_pirate_legs1 || t_pirate_legs2 || t_pirate_body || t_pirate_head1 || t_pirate_head2 || t_pirate_head3 || t_pirate_head4

//T Models
#define T_Models (t_elite || t_separatist || t_anarchist || t_professional || t_phoenix || t_balkan || t_pirate)

//T Heads
#define T_Heads (t_elite1_head || t_elite2_head|| t_elite3_head||t_elite4_head||t_elite5_head || t_separatist_head || t_anarchist_head || t_professional_head1 || t_professional_head2 || t_professional_head3 || t_professional_head4 || t_professional_head5 || t_phoenix_head || t_balkan_head1 || t_balkan_head2 || t_balkan_head3 || t_balkan_head4 || t_balkan_head5 || t_pirate_head1 || t_pirate_head2 || t_pirate_head3 || t_pirate_head4)

//T Legs
#define T_LEGS (t_elite1_legs||t_elite2_legs||t_elite3_legs||t_elite4_legs||t_elite5_legs||t_separatist_legs||t_anarchist_legs||t_professional_legs||t_phoenix_legs||t_balkan_legs1||t_balkan_legs2||t_pirate_legs1||t_pirate_legs2)

#define CT_HANDS (Stride == 32 && decl->Type == 2 && numElements >= 10 && mStartregister == 58 && mVectorCount == 150)// && mStage == 0)
#define T_HANDS ((Stride == 32 && decl->Type == 2 && numElements >= 10 && mStartregister == 58)&&(mVectorCount == 108||mVectorCount == 144||mVectorCount == 126))
#endif // _CSGO

HWND window = NULL;
WNDPROC wndProc_Orig = NULL;

bool showMenu = false;
bool Chams = true;

typedef HRESULT(_stdcall* EndScene)(LPDIRECT3DDEVICE9 pDevice);
HRESULT _stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice);
EndScene oEndScene;

typedef HRESULT(APIENTRY* SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource oSetStreamSource = 0;

typedef HRESULT(APIENTRY* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration oSetVertexDeclaration = 0;

typedef HRESULT(APIENTRY* SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF oSetVertexShaderConstantF = 0;

typedef HRESULT(APIENTRY* SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
SetVertexShader oSetVertexShader = 0;

typedef HRESULT(APIENTRY* SetPixelShader)(LPDIRECT3DDEVICE9, IDirect3DPixelShader9*);
SetPixelShader oSetPixelShader = 0;

typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive oDrawIndexedPrimitive = 0;

HRESULT GenerateShader(IDirect3DDevice9* pD3Ddev, IDirect3DPixelShader9** pShader, float r, float g, float b)
{
	char szShader[256];
	ID3DXBuffer* pShaderBuf = NULL;
	sprintf_s(szShader, "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", r, g, b, 1.0f);
	D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, NULL, &pShaderBuf, NULL);
	if (FAILED(pD3Ddev->CreatePixelShader((const DWORD*)pShaderBuf->GetBufferPointer(), pShader)))return E_FAIL;
	return D3D_OK;
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcID;
	GetWindowThreadProcessId(handle, &wndProcID);

	if (GetCurrentProcessId() != wndProcID)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (showMenu && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	return CallWindowProc(wndProc_Orig, hWnd, msg, wParam, lParam);
}

bool GetD3D9Device(void** pTable, size_t size)
{
	if (!pTable)
		return false;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!g_pD3D)
		return false;

	D3DPRESENT_PARAMETERS d3dPP = {};
	d3dPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dPP.hDeviceWindow = GetProcessWindow();
	d3dPP.Windowed = false;

	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dPP.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dPP, &g_pd3dDevice);

	if (!g_pd3dDevice)
	{
		d3dPP.Windowed = !d3dPP.Windowed;
		g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dPP.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dPP, &g_pd3dDevice);

		if (!g_pd3dDevice)
		{
			g_pD3D->Release();
			return false;
		}

		memcpy(pTable, *reinterpret_cast<void***>(g_pd3dDevice), size);

		g_pd3dDevice->Release();
		g_pD3D->Release();
		return true;
	}
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

HRESULT __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	static bool init = false;

	if (GetAsyncKeyState(VK_INSERT))
		showMenu = !showMenu;

	if (!init)
	{
		wndProc_Orig = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(pDevice);

		if (!shaderBack)
			GenerateShader(pDevice, &shaderBack, 221.0f / 255.0f, 177.0f / 255.0f, 31.0f / 255.0f);

		if (!shaderFront)
			GenerateShader(pDevice, &shaderFront, 31.0f / 255.0f, 99.0f / 255.0f, 155.0f / 255.0f);

		init = true;
	}

	if (init)
	{
		if (showMenu)
		{
			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX9_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Menu", &showMenu);
				ImGui::Checkbox("Test", &Chams);

			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}
	}

	return oEndScene(pDevice);
}

HRESULT APIENTRY SetStreamSource_Hook(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride)
{
	if (StreamNumber == 0)
		Stride = sStride;

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
}

HRESULT APIENTRY SetVertexDeclaration_Hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pDeclaration)
{
	if (pDeclaration != NULL)
		pDeclaration->GetDeclaration(declaration, &numElements);

	return oSetVertexDeclaration(pDevice, pDeclaration);
}

HRESULT APIENTRY SetVertexShaderConstantF_Hook(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
{
	if (pConstantData != NULL)
	{
		mStartRegister = StartRegister;
		mVectorCount = Vector4fCount;
	}

	return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
}

HRESULT APIENTRY SetVertexShader_Hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9* veShader)
{
	if (veShader != NULL)
	{
		vShader = veShader;
		vShader->GetFunction(NULL, &vSize);
	}

	return oSetVertexShader(pDevice, veShader);
}

HRESULT APIENTRY SetPixelShader_Hook(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9* piShader)
{
	if (piShader != NULL)
	{
		pShader = piShader;
		pShader->GetFunction(NULL, &pSize);
	}

	return oSetPixelShader(pDevice, piShader);
}

HRESULT APIENTRY DrawIndexedPrimitive_Hook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT baseVertexIndex, UINT minVertexIndex, UINT numVertices, UINT startIndex, UINT primCount)
{
	if (Chams)
	{
#ifdef _CSS
		if (TR || CT)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetPixelShader(shaderBack);
			oDrawIndexedPrimitive(pDevice, Type, baseVertexIndex, minVertexIndex, numVertices, startIndex, primCount);
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetPixelShader(shaderFront);
		}
#endif // _CSS

#ifdef _CSGO
		if (T_Models || CT_Models)
		{
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetPixelShader(shaderBack);
			oDrawIndexedPrimitive(pDevice, Type, baseVertexIndex, minVertexIndex, numVertices, startIndex, primCount);
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetPixelShader(shaderFront);
		}
#endif // _CSGO
	}

	return oDrawIndexedPrimitive(pDevice, Type, baseVertexIndex, minVertexIndex, numVertices, startIndex, primCount);
}

DWORD WINAPI mainThread(PVOID base)
{
	void* d3d9Device[119];
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		oEndScene = (EndScene)Detours::X86::DetourFunction((uintptr_t)d3d9Device[42], (uintptr_t)hkEndScene);
		oSetStreamSource = (SetStreamSource)Detours::X86::DetourFunction((uintptr_t)d3d9Device[100], (uintptr_t)SetStreamSource_Hook);
		oSetVertexDeclaration = (SetVertexDeclaration)Detours::X86::DetourFunction((uintptr_t)d3d9Device[87], (uintptr_t)SetVertexDeclaration_Hook);
		oSetVertexShaderConstantF = (SetVertexShaderConstantF)Detours::X86::DetourFunction((uintptr_t)d3d9Device[94], (uintptr_t)SetVertexShaderConstantF_Hook);
		oSetVertexShader = (SetVertexShader)Detours::X86::DetourFunction((uintptr_t)d3d9Device[92], (uintptr_t)SetVertexShader_Hook);
		oSetPixelShader = (SetPixelShader)Detours::X86::DetourFunction((uintptr_t)d3d9Device[107], (uintptr_t)SetPixelShader_Hook);
		oDrawIndexedPrimitive = (DrawIndexedPrimitive)Detours::X86::DetourFunction((uintptr_t)d3d9Device[82], (uintptr_t)DrawIndexedPrimitive_Hook);

		while (true)
		{
			if (GetAsyncKeyState(VK_F10))
			{
				CleanupDeviceD3D();
				FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
			}
		}
	}
	FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(nullptr, NULL, mainThread, hModule, NULL, nullptr);
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}