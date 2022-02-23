#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>
#include <typeinfo>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "d3dx11effect.h"
#include "d3dcompiler.h"
#include "DirectXTex.h"

#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include "DirectXTK/Effects.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"


#include <vector>
#include <map>
#include <list>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace DirectX;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif // _DEBUG

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"


using namespace Engine;
