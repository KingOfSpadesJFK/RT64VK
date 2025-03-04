//
// RT64
//

// #define RT64_MINIMAL
#ifndef RT64_H_INCLUDED
#define RT64_H_INCLUDED

#if defined(_WIN32) && !defined(__WIN32__)
	#define __WIN32__
#endif

#ifdef __WIN32__
	#include <iostream>
	#include <Windows.h>
#else
	// This version of RT64 will rely on GLFW for the window
	#define GLFW_INCLUDE_VULKAN
    #include <dlfcn.h>
#endif
#include <stdio.h>


// Material constants.
#define RT64_MATERIAL_FILTER_POINT				0
#define RT64_MATERIAL_FILTER_LINEAR				1
#define RT64_MATERIAL_ADDR_WRAP					0
#define RT64_MATERIAL_ADDR_MIRROR				1
#define RT64_MATERIAL_ADDR_CLAMP				2
#define RT64_MATERIAL_CC_SHADER_0				0
#define RT64_MATERIAL_CC_SHADER_INPUT_1			1
#define RT64_MATERIAL_CC_SHADER_INPUT_2			2
#define RT64_MATERIAL_CC_SHADER_INPUT_3			3
#define RT64_MATERIAL_CC_SHADER_INPUT_4			4
#define RT64_MATERIAL_CC_SHADER_TEXEL0			5
#define RT64_MATERIAL_CC_SHADER_TEXEL0A			6
#define RT64_MATERIAL_CC_SHADER_TEXEL1			7

// Material attributes.
#define RT64_ATTRIBUTE_NONE							0x0000
#define RT64_ATTRIBUTE_IGNORE_NORMAL_FACTOR			0x0001
#define RT64_ATTRIBUTE_UV_DETAIL_SCALE				0x0002
#define RT64_ATTRIBUTE_REFLECTION_FACTOR			0x0004
#define RT64_ATTRIBUTE_REFLECTION_FRESNEL_FACTOR	0x0008
#define RT64_ATTRIBUTE_REFLECTION_SHINE_FACTOR		0x0010
#define RT64_ATTRIBUTE_REFRACTION_FACTOR			0x0020
#define RT64_ATTRIBUTE_SPECULAR_COLOR				0x0040
#define RT64_ATTRIBUTE_SPECULAR_EXPONENT			0x0080
#define RT64_ATTRIBUTE_SOLID_ALPHA_MULTIPLIER		0x0100
#define RT64_ATTRIBUTE_SHADOW_ALPHA_MULTIPLIER		0x0200
#define RT64_ATTRIBUTE_DEPTH_BIAS					0x0400
#define RT64_ATTRIBUTE_SHADOW_RAY_BIAS				0x0800
#define RT64_ATTRIBUTE_SELF_LIGHT					0x1000
#define RT64_ATTRIBUTE_LIGHT_GROUP_MASK_BITS		0x2000
#define RT64_ATTRIBUTE_DIFFUSE_COLOR_MIX			0x4000

// Mesh flags.
#define RT64_MESH_RAYTRACE_ENABLED				0x1
#define RT64_MESH_RAYTRACE_UPDATABLE			0x2
#define RT64_MESH_RAYTRACE_COMPACT				0x4
#define RT64_MESH_RAYTRACE_FAST_TRACE			0x8

// Shader flags.
#define RT64_SHADER_FILTER_POINT				0x0
#define RT64_SHADER_FILTER_LINEAR				0x1
//
#define RT64_SHADER_ADDRESSING_WRAP				0x0
#define RT64_SHADER_ADDRESSING_MIRROR			0x1
#define RT64_SHADER_ADDRESSING_CLAMP			0x2
//
#define RT64_SHADER_RASTER_ENABLED				0x01
#define RT64_SHADER_RAYTRACE_ENABLED			0x02
#define RT64_SHADER_NORMAL_MAP_ENABLED			0x04
#define RT64_SHADER_SPECULAR_MAP_ENABLED		0x08
/* * * * *  Experimental  * * * * */
#define RT64_SHADER_RASTER_TRANSFORMS_ENABLED	0x10	// Use this if the raster shader should have 3D perspective and transformations enabled

// Instance flags.
#define RT64_INSTANCE_RASTER_BACKGROUND			0x1
#define RT64_INSTANCE_DISABLE_BACKFACE_CULLING	0x2

// Light flags.
#define RT64_LIGHT_GROUP_MASK_ALL				0xFFFFFFFF
#define RT64_LIGHT_GROUP_DEFAULT				0x1
#define RT64_LIGHT_MAX_SAMPLES					128

// View attributes.
#define RT64_UPSCALER_OFF						0x0
#define RT64_UPSCALER_AUTO						0x1
#define RT64_UPSCALER_DLSS						0x2
#define RT64_UPSCALER_FSR						0x3
#define RT64_UPSCALER_XESS						0x4
#define RT64_UPSCALER_MODE_AUTO					0x0
#define RT64_UPSCALER_MODE_ULTRA_PERFORMANCE	0x1
#define RT64_UPSCALER_MODE_PERFORMANCE			0x2
#define RT64_UPSCALER_MODE_BALANCED				0x3
#define RT64_UPSCALER_MODE_QUALITY				0x4
#define RT64_UPSCALER_MODE_ULTRA_QUALITY		0x5
#define RT64_UPSCALER_MODE_NATIVE				0x6

// Texture formats.
#define RT64_TEXTURE_FORMAT_RGBA8				0x1
#define RT64_TEXTURE_FORMAT_DDS					0x2

// Forward declaration of types.
typedef struct RT64_DEVICE RT64_DEVICE;
typedef struct RT64_VIEW RT64_VIEW;
typedef struct RT64_SCENE RT64_SCENE;
typedef struct RT64_INSTANCE RT64_INSTANCE;
typedef struct RT64_MESH RT64_MESH;
typedef struct RT64_TEXTURE RT64_TEXTURE;
typedef struct RT64_SHADER RT64_SHADER;
typedef struct RT64_INSPECTOR RT64_INSPECTOR;

typedef struct {
	float x, y;
} RT64_VECTOR2;

typedef struct {
	float x, y, z;
} RT64_VECTOR3;

typedef struct {
	float x, y, z, w;
} RT64_VECTOR4;

typedef struct {
	float m[4][4];
} RT64_MATRIX4;

typedef struct {
	int x, y, w, h;
} RT64_RECT;

typedef struct {
	int diffuseTexIndex;
	int normalTexIndex;
	int specularTexIndex;
	float ignoreNormalFactor;
	float uvDetailScale;
	float reflectionFactor;
	float reflectionFresnelFactor;
	float reflectionShineFactor;
	float refractionFactor;
	RT64_VECTOR3 specularColor;
	float specularExponent;
	float solidAlphaMultiplier;
	float shadowAlphaMultiplier;
	float depthBias;
	float shadowRayBias;
	RT64_VECTOR3 selfLight;
	unsigned int lightGroupMaskBits;
	RT64_VECTOR3 fogColor;
	RT64_VECTOR4 diffuseColorMix;
	float fogMul;
	float fogOffset;
	unsigned int fogEnabled;
	float lockMask;

	// Flag containing all attributes that are actually used by this material.
	int enabledAttributes;
} RT64_MATERIAL;

// Light
typedef struct {
	RT64_VECTOR3 position;
	RT64_VECTOR3 diffuseColor;
	float attenuationRadius;
	float pointRadius;
	RT64_VECTOR3 specularColor;
	float shadowOffset;
	float attenuationExponent;
	float flickerIntensity;
	unsigned int groupBits;
} RT64_LIGHT;

typedef struct {
	RT64_VECTOR3 ambientBaseColor;
	RT64_VECTOR3 ambientNoGIColor;
	RT64_VECTOR3 eyeLightDiffuseColor;
	RT64_VECTOR3 eyeLightSpecularColor;
	RT64_VECTOR3 skyDiffuseMultiplier;
	RT64_VECTOR3 skyHSLModifier;
	float skyYawOffset;
	float giDiffuseStrength;
	float giSkyStrength;
} RT64_SCENE_DESC;

typedef struct {
	float 			resolutionScale;
	float 			motionBlurStrength;
	unsigned int 	diSamples;
	unsigned int 	giSamples;
	unsigned int 	maxLights;
	unsigned char 	upscaler;
	unsigned char 	upscalerMode;
	float 			upscalerSharpness;
	bool 			denoiserEnabled;
} RT64_VIEW_DESC;

#ifdef RT64_ENABLE_EXPERIMENTAL
typedef struct {
	int 			tonemapMode;
	float 			tonemapExposure;
	float 			tonemapBlack;
	float 			tonemapWhite;
	float 			tonemapGamma;
} RT64_POST_FX_DESC;
#endif

typedef struct {
	RT64_MESH *mesh;
	RT64_MATRIX4 transform;
	RT64_MATRIX4 previousTransform;
	RT64_TEXTURE *diffuseTexture;
	RT64_TEXTURE *normalTexture;
	RT64_TEXTURE *specularTexture;
	RT64_SHADER *shader;
	RT64_MATERIAL material;
	RT64_RECT scissorRect;
	RT64_RECT viewportRect;
	unsigned int flags;
} RT64_INSTANCE_DESC;

typedef struct {
	void *bytes;
	int byteCount;
	int format;
	int width;
	int height;
	int rowPitch;
} RT64_TEXTURE_DESC;

inline void RT64_ApplyMaterialAttributes(RT64_MATERIAL *dst, RT64_MATERIAL *src) {
	if (src->enabledAttributes & RT64_ATTRIBUTE_IGNORE_NORMAL_FACTOR) {
		dst->ignoreNormalFactor = src->ignoreNormalFactor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_UV_DETAIL_SCALE) {
		dst->uvDetailScale = src->uvDetailScale;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_REFLECTION_FACTOR) {
		dst->reflectionFactor = src->reflectionFactor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_REFLECTION_FRESNEL_FACTOR) {
		dst->reflectionFresnelFactor = src->reflectionFresnelFactor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_REFLECTION_SHINE_FACTOR) {
		dst->reflectionShineFactor = src->reflectionShineFactor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_REFRACTION_FACTOR) {
		dst->refractionFactor = src->refractionFactor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SPECULAR_COLOR) {
		dst->specularColor = src->specularColor;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SPECULAR_EXPONENT) {
		dst->specularExponent = src->specularExponent;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SOLID_ALPHA_MULTIPLIER) {
		dst->solidAlphaMultiplier = src->solidAlphaMultiplier;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SHADOW_ALPHA_MULTIPLIER) {
		dst->shadowAlphaMultiplier = src->shadowAlphaMultiplier;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_DEPTH_BIAS) {
		dst->depthBias = src->depthBias;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SHADOW_RAY_BIAS) {
		dst->shadowRayBias = src->shadowRayBias;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_SELF_LIGHT) {
		dst->selfLight = src->selfLight;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_LIGHT_GROUP_MASK_BITS) {
		dst->lightGroupMaskBits = src->lightGroupMaskBits;
	}

	if (src->enabledAttributes & RT64_ATTRIBUTE_DIFFUSE_COLOR_MIX) {
		dst->diffuseColorMix = src->diffuseColorMix;
	}
}

// Internal function pointer types.
typedef const char *(*GetLastErrorPtr)();
typedef RT64_DEVICE* (*CreateDevicePtr)(void* window);
typedef void (*DestroyDevicePtr)(RT64_DEVICE* device);
typedef void (*DrawDevicePtr)(RT64_DEVICE *device, int vsyncInterval, float delta);
typedef RT64_VIEW* (*CreateViewPtr)(RT64_SCENE* scenePtr);
typedef void (*SetViewPerspectivePtr)(RT64_VIEW *viewPtr, RT64_MATRIX4 viewMatrix, float fovRadians, float nearDist, float farDist, bool canReproject);
typedef void (*SetViewDescriptionPtr)(RT64_VIEW *viewPtr, RT64_VIEW_DESC viewDesc);
typedef void (*SetViewSkyPlanePtr)(RT64_VIEW *viewPtr, RT64_TEXTURE *texturePtr);
typedef RT64_INSTANCE* (*GetViewRaytracedInstanceAtPtr)(RT64_VIEW *viewPtr, int x, int y);
typedef bool (*GetViewUpscalerSupportPtr)(RT64_VIEW* viewPtr, char upscaler);
typedef void (*DestroyViewPtr)(RT64_VIEW* viewPtr);
typedef RT64_SCENE* (*CreateScenePtr)(RT64_DEVICE* devicePtr);
typedef void (*SetSceneDescriptionPtr)(RT64_SCENE* scenePtr, RT64_SCENE_DESC sceneDesc);
typedef void (*SetSceneLightsPtr)(RT64_SCENE* scenePtr, RT64_LIGHT* lightArray, int lightCount);
typedef void (*DestroyScenePtr)(RT64_SCENE* scenePtr);
typedef RT64_MESH* (*CreateMeshPtr)(RT64_DEVICE* devicePtr, int flags);
typedef void (*SetMeshPtr)(RT64_MESH* meshPtr, void* vertexArray, int vertexCount, int vertexStride, unsigned int* indexArray, int indexCount);
typedef void (*DestroyMeshPtr)(RT64_MESH* meshPtr);
typedef RT64_SHADER *(*CreateShaderPtr)(RT64_DEVICE *devicePtr, unsigned int shaderId, unsigned int filter, unsigned int hAddr, unsigned int vAddr, int flags);
typedef void (*DestroyShaderPtr)(RT64_SHADER *shaderPtr);
typedef RT64_INSTANCE* (*CreateInstancePtr)(RT64_SCENE* scenePtr);
typedef void (*SetInstanceDescriptionPtr)(RT64_INSTANCE* instancePtr, RT64_INSTANCE_DESC instanceDesc);
typedef void (*DestroyInstancePtr)(RT64_INSTANCE* instancePtr);
typedef RT64_TEXTURE* (*CreateTexturePtr)(RT64_DEVICE* devicePtr, RT64_TEXTURE_DESC textureDesc);
typedef void (*DestroyTexturePtr)(RT64_TEXTURE* texture);
typedef RT64_INSPECTOR* (*CreateInspectorPtr)(RT64_DEVICE* devicePtr);
#ifdef __WIN32__
typedef bool (*HandleMessageInspectorPtr)(RT64_INSPECTOR* inspectorPtr, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
typedef void (*SetSceneInspectorPtr)(RT64_INSPECTOR* inspectorPtr, RT64_SCENE_DESC* sceneDesc);
typedef void (*SetMaterialInspectorPtr)(RT64_INSPECTOR* inspectorPtr, RT64_MATERIAL* material, const char *materialName);
typedef void (*SetLightsInspectorPtr)(RT64_INSPECTOR* inspectorPtr, RT64_LIGHT* lights, int *lightCount, int maxLightCount);
typedef void (*PrintClearInspectorPtr)(RT64_INSPECTOR *inspectorPtr);
typedef void (*PrintMessageInspectorPtr)(RT64_INSPECTOR* inspectorPtr, const char* message);
typedef void (*DestroyInspectorPtr)(RT64_INSPECTOR* inspectorPtr);
// Experimental
#ifdef RT64_ENABLE_EXPERIMENTAL
typedef void (*SetPostEffectsPtr)(RT64_VIEW *viewPtr, RT64_POST_FX_DESC postDesc);
#endif

// Stores all the function pointers used in the RT64 library.
typedef struct {
#ifdef __WIN32__
	HMODULE handle;
#else
	void* handle;
#endif
	GetLastErrorPtr GetLastError;
	CreateDevicePtr CreateDevice;
	DestroyDevicePtr DestroyDevice;
#ifndef RT64_MINIMAL
	DrawDevicePtr DrawDevice;
	CreateViewPtr CreateView;
	SetViewPerspectivePtr SetViewPerspective;
	SetViewDescriptionPtr SetViewDescription;
	SetViewSkyPlanePtr SetViewSkyPlane;
	GetViewRaytracedInstanceAtPtr GetViewRaytracedInstanceAt;
	GetViewUpscalerSupportPtr GetViewUpscalerSupport;
	DestroyViewPtr DestroyView;
	CreateScenePtr CreateScene;
	SetSceneDescriptionPtr SetSceneDescription;
	SetSceneLightsPtr SetSceneLights;
	DestroyScenePtr DestroyScene;
	CreateMeshPtr CreateMesh;
	SetMeshPtr SetMesh;
	DestroyMeshPtr DestroyMesh;
	CreateShaderPtr CreateShader;
	DestroyShaderPtr DestroyShader;
	CreateInstancePtr CreateInstance;
	SetInstanceDescriptionPtr SetInstanceDescription;
	DestroyInstancePtr DestroyInstance;
	CreateTexturePtr CreateTexture;
	DestroyTexturePtr DestroyTexture;
	CreateInspectorPtr CreateInspector;
#ifdef __WIN32__
	HandleMessageInspectorPtr HandleMessageInspector;
#endif
	PrintClearInspectorPtr PrintClearInspector;
	PrintMessageInspectorPtr PrintMessageInspector;
	SetSceneInspectorPtr SetSceneInspector;
	SetMaterialInspectorPtr SetMaterialInspector;
	SetLightsInspectorPtr SetLightsInspector;
	DestroyInspectorPtr DestroyInspector;
#ifdef RT64_ENABLE_EXPERIMENTAL
	SetPostEffectsPtr SetPostEffects;
#endif
#endif
} RT64_LIBRARY;

#ifdef __WIN32__
#define RT64_GetProcAddress GetProcAddress
#else
#define RT64_GetProcAddress dlsym
#endif


// Define RT64_DEBUG for loading the debug DLL.
inline RT64_LIBRARY RT64_LoadLibrary() {
	RT64_LIBRARY lib;

#ifdef __WIN32__
    //#if defined(RT64_MINIMAL)
    //    lib.handle = LoadLibrary(TEXT("rt64vkm.dll"));
    //#elif defined(RT64_DEBUG)
    //    lib.handle = LoadLibrary(TEXT("rt64vkd.dll"));
    //#else
        lib.handle = LoadLibrary(TEXT("rt64vk.dll"));
    //#endif
#else
    // #if defined(RT64_MINIMAL)
    //     lib.handle = dlopen("./librt64vkm.so", RTLD_LAZY | RTLD_DEEPBIND);
    // #elif defined(RT64_DEBUG)
    //     lib.handle = dlopen("./librt64vkd.so", RTLD_LAZY | RTLD_DEEPBIND);
    // #else
    //     lib.handle = dlopen("./librt64vk.so", RTLD_LAZY | RTLD_DEEPBIND);
    // #endif
        lib.handle = dlopen("./librt64vk.so", RTLD_NOW | RTLD_GLOBAL | RTLD_DEEPBIND);
#endif

	if (lib.handle != 0) {
		lib.GetLastError = (GetLastErrorPtr)(RT64_GetProcAddress(lib.handle, "RT64_GetLastError"));
		lib.CreateDevice = (CreateDevicePtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateDevice"));
		lib.DestroyDevice = (DestroyDevicePtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyDevice"));

#ifndef RT64_MINIMAL
		lib.DrawDevice = (DrawDevicePtr)(RT64_GetProcAddress(lib.handle, "RT64_DrawDevice"));
		lib.CreateView = (CreateViewPtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateView"));
		lib.SetViewPerspective = (SetViewPerspectivePtr)(RT64_GetProcAddress(lib.handle, "RT64_SetViewPerspective"));
		lib.SetViewDescription = (SetViewDescriptionPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetViewDescription"));
		lib.SetViewSkyPlane = (SetViewSkyPlanePtr)(RT64_GetProcAddress(lib.handle, "RT64_SetViewSkyPlane"));
		lib.GetViewRaytracedInstanceAt = (GetViewRaytracedInstanceAtPtr)(RT64_GetProcAddress(lib.handle, "RT64_GetViewRaytracedInstanceAt"));
		lib.GetViewUpscalerSupport = (GetViewUpscalerSupportPtr)(RT64_GetProcAddress(lib.handle, "RT64_GetViewUpscalerSupport"));
		lib.DestroyView = (DestroyViewPtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyView"));
		lib.CreateScene = (CreateScenePtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateScene"));
		lib.SetSceneDescription = (SetSceneDescriptionPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetSceneDescription"));
		lib.SetSceneLights = (SetSceneLightsPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetSceneLights"));
		lib.DestroyScene = (DestroyScenePtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyScene"));
		lib.CreateMesh = (CreateMeshPtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateMesh"));
		lib.SetMesh = (SetMeshPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetMesh"));
		lib.DestroyMesh = (DestroyMeshPtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyMesh"));
		lib.CreateShader = (CreateShaderPtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateShader"));
		lib.DestroyShader = (DestroyShaderPtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyShader"));
		lib.CreateInstance = (CreateInstancePtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateInstance"));
		lib.SetInstanceDescription = (SetInstanceDescriptionPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetInstanceDescription"));
		lib.DestroyInstance = (DestroyInstancePtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyInstance"));
		lib.CreateTexture = (CreateTexturePtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateTexture"));
		lib.DestroyTexture = (DestroyTexturePtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyTexture"));
		lib.CreateInspector = (CreateInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_CreateInspector"));
	#ifdef __WIN32__
		lib.HandleMessageInspector = (HandleMessageInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_HandleMessageInspector"));
	#endif
		lib.SetSceneInspector = (SetSceneInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetSceneInspector"));
		lib.SetMaterialInspector = (SetMaterialInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetMaterialInspector"));
		lib.SetLightsInspector = (SetLightsInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetLightsInspector"));
		lib.PrintClearInspector = (PrintClearInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_PrintClearInspector"));
		lib.PrintMessageInspector = (PrintMessageInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_PrintMessageInspector"));
		lib.DestroyInspector = (DestroyInspectorPtr)(RT64_GetProcAddress(lib.handle, "RT64_DestroyInspector"));
	#ifdef RT64_ENABLE_EXPERIMENTAL
		lib.SetPostEffects = (SetPostEffectsPtr)(RT64_GetProcAddress(lib.handle, "RT64_SetPostEffects"));
	#endif
#endif
	}
	else {
#ifdef __WIN32__
		char errorMessage[256];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), errorMessage, sizeof(errorMessage), NULL);
		fprintf(stderr, "Error when loading library: %s\n", errorMessage);
#else
		fprintf(stderr, "Error when loading library: %s\n", dlerror());
#endif
	}

	return lib;
}

inline void RT64_UnloadLibrary(RT64_LIBRARY lib) {
#ifdef __WIN32__
	FreeLibrary(lib.handle);
#else
	dlclose(lib.handle);
#endif
}

#endif