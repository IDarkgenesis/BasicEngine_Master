#pragma once
#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#define GLOG(format, ...) glog(__FILE__, __LINE__, format, __VA_ARGS__);

void glog(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }


// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define HFOV 90