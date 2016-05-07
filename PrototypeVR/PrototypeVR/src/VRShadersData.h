#ifndef SHADERSOURCE
#define SHADERSOURCE

#include "VRManager.h"
#include <gl\glew.h>
#include <gl\GLU.h>
#include <gl\gl.h>

#if VR_ENABLED
extern VRManager			vrManager;
#endif

//////////////////OpenGL Helper Class///////////////
static void gl_uniform_2f	(const char* varname, float a, float b) { GLuint varid = glGetUniformLocation(vrManager.GetShaderProgramID(), varname); glUniform2f(varid, a, b); }
static void gl_uniform_4f	 (const char* varname, float a, float b, float c, float d) { GLuint varid = glGetUniformLocation(vrManager.GetShaderProgramID(), varname); glUniform4f(varid, a, b, c, d); }
static void gl_uniform_1i	 (const char* varname, int value) { GLuint varid = glGetUniformLocation(vrManager.GetShaderProgramID(), varname); glUniform1i(varid, value); }

////////////////////Shaders Source Code/////////////
static const char* vertex_shader_src = 
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 Position;\n"
"layout(location = 1) in vec2 TexCoord;\n"
"out vec2 oTexCoord;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(Position, 1);\n"
"   oTexCoord = TexCoord;\n"
"};\n";

static const char* fragment_shader_src = 
"#version 330\n"
"\n"
"uniform vec2 LensCenter;\n"
"uniform vec2 ScreenCenter;\n"
"uniform vec2 Scale;\n"
"uniform vec2 ScaleIn;\n"
"uniform vec4 HmdWarpParam;\n"
"uniform sampler2D texture0;\n"
"varying vec2 oTexCoord;\n"
"out vec4 outcolor;\n"
"\n"
"vec2 HmdWarp(vec2 in01)\n"
"{\n"
"   vec2  theta = (in01 - LensCenter) * ScaleIn; // Scales to [-1, 1]\n"
"   float rSq = theta.x * theta.x + theta.y * theta.y;\n"
"   vec2  theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq + \n"
"                           HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);\n"
"   return LensCenter + Scale * theta1;\n"
"}\n"
"void main()\n"
"{\n"
"   vec2 tc = HmdWarp(oTexCoord);\n"
"   if (!all(equal(clamp(tc, ScreenCenter-vec2(0.25,0.5), ScreenCenter+vec2(0.25,0.5)), tc)))\n"
"       outcolor = vec4(0);\n"
"   else\n"
"	   outcolor = texture2D(texture0, tc);\n"
"};\n";

#endif