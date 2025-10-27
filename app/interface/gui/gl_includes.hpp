#ifdef _WIN32 
#define NOMINMAX
#include <windows.h> 
#include <GL/gl.h> 
#elif __linux__ 
#include <GL/gl.h> 
#elif __APPLE__ 
#include <OpenGL/gl.h> 
#endif