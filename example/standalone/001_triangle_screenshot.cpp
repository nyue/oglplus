/**
 *  @example standalone/001_triangle_scereenshot.cpp
 *  @brief Shows the basic usage of OGLplus with EGLplus
 *
 *  @oglplus_screenshot{001_triangle}
 *
 *  Copyright 2008-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <cassert>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <oglplus/all.hpp>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#define EGL_CONTEXT_MINOR_VERSION_KHR 0x30FB
#include <eglplus/all.hpp>

void render_frame(void)
{
	using namespace oglplus;

	Context gl;


	VertexShader vs;
	vs.Source(" \
		#version 330\n \
		in vec3 Position; \
		void main(void) \
		{ \
			gl_Position = vec4(Position, 1.0); \
		} \
	");
	vs.Compile();

	FragmentShader fs;
	fs.Source(" \
		#version 330\n \
		out vec4 fragColor; \
		void main(void) \
		{ \
			fragColor = vec4(1.0, 0.0, 0.0, 1.0); \
		} \
	");
	fs.Compile();

	Program prog;
	prog.AttachShader(vs);
	prog.AttachShader(fs);
	prog.Link();
	prog.Use();

	VertexArray triangle;
	triangle.Bind();

	GLfloat triangle_verts[9] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	oglplus::Buffer verts;
	verts.Bind(Buffer::Target::Array);
	Buffer::Data(
		Buffer::Target::Array,
		9,
		triangle_verts
	);

	VertexAttribArray vert_attr(prog, "Position");
	vert_attr.Setup(3, DataType::Float);
	vert_attr.Enable();

	gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gl.ClearDepth(1.0f);

	gl.Clear().ColorBuffer().DepthBuffer();

	gl.DrawArrays(PrimitiveType::Triangles, 0, 3);
}

void save_frame(unsigned width, unsigned height, const char* screenshot_path)
{
	std::vector<char> pixels(width * height * 3);
	glReadPixels(
		0, 0,
		width,
		height,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
	);
	std::ofstream output(screenshot_path);
	output.write(pixels.data(), pixels.size());
	std::cout << screenshot_path << std::endl;
}

void make_screenshot(unsigned width, unsigned height, const char* screenshot_path)
{
	using namespace eglplus;

	eglplus::Display display;
	LibEGL egl(display);

	Configs configs(
		display,
		ConfigAttribs()
			.Add(ConfigAttrib::RedSize, 8)
			.Add(ConfigAttrib::GreenSize, 8)
			.Add(ConfigAttrib::BlueSize, 8)
			.Add(ConfigAttrib::DepthSize, 24)
			.Add(eglplus::ConfigAttrib::StencilSize, 8)
			.Finish()
	);

	Config config = configs.First();

	Surface surface = Surface::PBuffer(
		display,
		config,
		SurfaceAttribs()
			.Add(SurfaceAttrib::Width, int(width))
			.Add(SurfaceAttrib::Height, int(height))
			.Finish()
	);

	BindAPI(RenderingAPI::OpenGL);
	Context context(
		display,
		config,
		ContextAttribs()
			.Add(ContextAttrib::ClientVersion, 3)
			.Add(ContextAttrib::MinorVersion, 0)
			.Finish()
	);

	context.MakeCurrent(surface);

	GLenum err = glewInit();
	if(err == GLEW_OK)
	{
		render_frame();

		context.WaitGL();

		save_frame(width, height, screenshot_path);
	}
	else throw std::runtime_error(
		std::string("GLEW initialization error '")+
		std::string((const char*)glewGetErrorString(err))+
		std::string("'")
	);
}

int main(int argc, char* argv[])
{
	try
	{
		glGetError();
		make_screenshot(
			800,
			600,
			(argc>1)?argv[1]:"screenshot.rgb"
		);
		return 0;
	}
	catch(oglplus::Error& oe)
	{
		std::cerr <<
			"OGLplus error (in " << oe.GLSymbol() << ", " <<
			oe.ClassName() << ": '" <<
			oe.ObjectDescription() << "'): " <<
			oe.what() <<
			" [" << oe.File() << ":" << oe.Line() << "] ";
		std::cerr << std::endl;
		oe.Cleanup();
	}
	catch(eglplus::Error& ee)
	{
		std::cerr <<
			"EGLplus error (in " << ee.EGLSymbol() << ") " <<
			ee.what() <<
			" [" << ee.File() << ":" << ee.Line() << "] ";
		std::cerr << std::endl;
		ee.Cleanup();
	}
	catch(std::exception& se)
	{
		std::cerr << "Error: " << se.what();
		std::cerr << std::endl;
	}
	return 1;
}

