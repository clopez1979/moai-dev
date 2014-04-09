// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-spine/headers.h>

#include <spine/extension.h>
#include <moai-spine/host.h>
#include <moai-spine/headers.h>

#include <moai-spine/MOAISpine.h>
#include <moai-spine/MOAISpineBone.h>
#include <moai-spine/MOAISpineSkeletonData.h>
#include <moai-spine/MOAISpineSkeleton.h>
#include <moai-spine/MOAISpineSlot.h>

//================================================================//
// aku-util
//================================================================//

static bool sIsInitialized = false;

//================================================================//
// Required spine extension functions
//================================================================//
int spAtlasFilterToGL( spAtlasFilter filter ) {
	switch( filter ) {
		case ATLAS_NEAREST:
			return ZGL_SAMPLE_LINEAR;
		case ATLAS_LINEAR:
			return ZGL_SAMPLE_LINEAR;
		case ATLAS_MIPMAP:
			return ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR;
		case ATLAS_MIPMAP_NEAREST_NEAREST:
			return ZGL_SAMPLE_NEAREST_MIPMAP_NEAREST;
		case ATLAS_MIPMAP_LINEAR_NEAREST:
			return ZGL_SAMPLE_LINEAR_MIPMAP_NEAREST;
		case ATLAS_MIPMAP_NEAREST_LINEAR:
			return ZGL_SAMPLE_NEAREST_MIPMAP_LINEAR;
		case ATLAS_MIPMAP_LINEAR_LINEAR:
			return ZGL_SAMPLE_LINEAR_MIPMAP_LINEAR;
	}
	return ZGL_SAMPLE_LINEAR;
}

void _spAtlasPage_createTexture ( spAtlasPage* self, const char* path ) {
	
	MOAILuaRef& createTextureRef = MOAISpine::Get ().GetCreateTextureRef();
	MOAITexture* texture;
	
	if ( MOAILuaRuntime::IsValid ()) {
		
		if ( createTextureRef ) {
			MOAIScopedLuaState state = createTextureRef.GetSelf ();
			
			state.Push ( path );
			state.Push ( spAtlasFilterToGL(self->minFilter) );
			state.Push ( spAtlasFilterToGL(self->magFilter) );
			state.DebugCall ( 3, 1 );
			
			texture = state.GetLuaObject < MOAITexture >( -1, true );
		}
		else {
			
			texture = new MOAITexture();
			texture->Init ( path, MOAIImageTransform::TRUECOLOR | MOAIImageTransform::PREMULTIPLY_ALPHA );
			texture->SetFilter ( spAtlasFilterToGL (self->minFilter), spAtlasFilterToGL (self->magFilter) );
		}
	}
	
	self->rendererObject = texture;
	self->width = texture->GetWidth ();
	self->height = texture->GetHeight ();
	texture->Retain ();
}

//----------------------------------------------------------------//
void _spAtlasPage_disposeTexture ( spAtlasPage* self ) {
	
	MOAITexture* texture = (MOAITexture*)self->rendererObject;
	texture->Release ();
}

//----------------------------------------------------------------//
char* _spUtil_readFile (const char* path, int* length) {
	
	MOAILuaRef& readFileRef = MOAISpine::Get ().GetReadFileRef();
	
	if ( MOAILuaRuntime::IsValid()) {
		
		if ( readFileRef ) {
			MOAIScopedLuaState state = readFileRef.GetSelf ();
			
			state.Push ( path );
			state.DebugCall ( 1, 1 );
			
			path = state.GetValue ( -1, path );
			return _readFile ( path, length );
		}
	}
	return _readFile ( path, length );
}

//----------------------------------------------------------------//
void AKUSpineFinalize () {
	
	if ( !sIsInitialized ) return;
	sIsInitialized = false;
}

//----------------------------------------------------------------//
void AKUSpineAppInitialize () {
}

//----------------------------------------------------------------//
void AKUSpineContextInitialize () {

	if ( !sIsInitialized ) {
		sIsInitialized = true;
	}

	REGISTER_LUA_CLASS ( MOAISpine )
	REGISTER_LUA_CLASS ( MOAISpineBone )
	REGISTER_LUA_CLASS ( MOAISpineSkeleton )
	REGISTER_LUA_CLASS ( MOAISpineSkeletonData )
	REGISTER_LUA_CLASS ( MOAISpineSlot )
}
