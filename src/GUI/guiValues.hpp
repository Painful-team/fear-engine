#ifndef FEARENGINE_CORE_GUIVALUES_H__
#define	FEARENGINE_CORE_GUIVALUES_H__

namespace FearEngine::Gui::Values
{
// Main windows trackers
static bool ShowScene = true;
static bool ShowHierarchy = true;
static bool ShowProject = true;
static bool ShowInspector = true;
static bool ShowHelp = false;

// Scene window start, pause, stop trackers
static bool isSceneStarted = false;
static bool isScenePaused = false;

// Inspector components flags
static bool InspectorTransformComponent = true;
static bool InspectorColorComponent = true;
static bool InspectorMeshComponent = true;
static bool InspectorInteractionComponent = true;
static bool InspectorShaderComponent = true;

const int nInspectorComponents = 5;

// 0 - Transform, 1 - Color, 2 - Mesh, 3 - Interaction, 4 - Shader
static bool InspectorComponents[nInspectorComponents] = { true, true, true, true, true };
const char* ComponentsNames[nInspectorComponents] = { "Transform", "Color", "Mesh", "Interaction", "Shader" };

// Stubs for Inspector showing simple GUI logic
static bool ShowPickedInspectorObject = false;
static char ObjectInspectorName[32] = "Cube";

static float ObjectInspectorPositionX = 0.0f;
static float ObjectInspectorPositionY = 0.0f;
static float ObjectInspectorPositionZ = 0.0f;

static float ObjectInspectorRotationX = 0.0f;
static float ObjectInspectorRotationY = 0.0f;
static float ObjectInspectorRotationZ = 0.0f;

static float ObjectInspectorScaleX = 0.0f;
static float ObjectInspectorScaleY = 0.0f;
static float ObjectInspectorScaleZ = 0.0f;

static float ObjectInspectorColor[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
}

#endif
