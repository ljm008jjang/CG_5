#pragma once

#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#define GLFW_INCLUDE_GLU
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <vector>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Include this header for glm::value_ptr
#include <glm/gtx/string_cast.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Camera.h"
#include "Plane.h"
#include "Light.h"
#include "sphere_scene.cpp"

using namespace glm;

// -------------------------------------------------
// Global Variables
// -------------------------------------------------
int Width = 512;
int Height = 512;
std::vector<float> OutputImage;
// -------------------------------------------------
Camera camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), 45.0f, (float)Width / (float)Height, -0.1f, -1000.0f);
std::vector<float> DepthBuffer;

sphere_scene sphere;
std::vector<sphere_scene> sceneObjects;

void rasterize_triangle(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& normal, const vec3& lightDir) {
	auto edge = [](const vec2& a, const vec2& b, const vec2& c) {
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
		};

	vec2 p0 = v0.xy();
	vec2 p1 = v1.xy();
	vec2 p2 = v2.xy();

	float area = edge(p0, p1, p2);
	if (area == 0) return;

	int minX = std::max(0, (int)floor(min({ p0.x, p1.x, p2.x })));
	int maxX = std::min(Width - 1, (int)ceil(max({ p0.x, p1.x, p2.x })));
	int minY = std::max(0, (int)floor(min({ p0.y, p1.y, p2.y })));
	int maxY = std::min(Height - 1, (int)ceil(max({ p0.y, p1.y, p2.y })));

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			vec2 p(x + 0.5f, y + 0.5f);
			float w0 = edge(p1, p2, p);
			float w1 = edge(p2, p0, p);
			float w2 = edge(p0, p1, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
				w0 /= area;
				w1 /= area;
				w2 /= area;

				float z = w0 * v0.z + w1 * v1.z + w2 * v2.z;
				int idx = y * Width + x;
				if (z < DepthBuffer[idx]) {
					DepthBuffer[idx] = z;

					float brightness = 1;//std::max(0.0f, dot(normal, normalize(lightDir)));
					vec3 color = brightness * vec3(1.0f); // 흰색 Lambert

					OutputImage[idx * 3 + 0] = color.r;
					OutputImage[idx * 3 + 1] = color.g;
					OutputImage[idx * 3 + 2] = color.b;
				}
			}
		}
	}
}

void render()
{
	//Create our image. We don't want to do this in 
	//the main loop since this may be too slow and we 
	//want a responsive display of our beautiful image.
	//Instead we draw to another buffer and copy this to the 
	//framebuffer using glDrawPixels(...) every refresh
	OutputImage.clear();
	DepthBuffer.clear();
	sceneObjects.clear();

	OutputImage.assign(Width * Height * 3, 0.0f);
	DepthBuffer.assign(Width * Height, 1e9f);
	sceneObjects.push_back(sphere);

	for(sphere_scene& sceneObject : sceneObjects)
	{
		sceneObject.clear();
		sceneObject.create_scene();
	}

	// 1. Model Transform
	mat4 model = translate(mat4(1.0f), vec3(0, 0, 7)) * scale(mat4(1.0f), vec3(2.0f));

	// 2. View Transform (Identity)
	mat4 view = camera.getViewMatrix();

	// 3. Perspective Projection (사용자 정의)
	// 직접 만든 Perspective 행렬 (OpenGL 호환 버전)
	mat4 proj = camera.getProjectionMatrix();

	// 4. 최종 MVP
	mat4 MVP = proj * view * model;

	//적용X
	vec3 lightDir = normalize(vec3(1, 1, 1));

	for (sphere_scene& sceneObject : sceneObjects) {
		for (int i = 0; i < sceneObject.gNumTriangles; ++i) {
			vec3 v0;
			vec3 v1;
			vec3 v2;

			vec3 screen0;
			vec3 screen1;
			vec3 screen2;

			//벡터를	가져온다
			sceneObject.render(MVP, Width, Height, &v0, &v1, &v2, &screen0, &screen1, &screen2, i);

			//back face culling
			vec3 faceNormal = normalize(cross(v1 - v0, v2 - v0));
			vec3 center = (v0 + v1 + v2) / 3.0f;          // 삼각형 중심
			vec3 viewDir = normalize(camera.e - center);

			if (dot(faceNormal, viewDir) >= 0)
				continue; // back-face culling

			rasterize_triangle(screen0, screen1, screen2, normalize(cross(v1 - v0, v2 - v0)), lightDir);
		}
	}



	//for (int j = 0; j < Height; ++j) 
	//{
	//	for (int i = 0; i < Width; ++i) 
	//	{
	//		// ---------------------------------------------------
	//		// --- Implement your code here to generate the image
	//		// ---------------------------------------------------
	//	}
	//}
}


void resize_callback(GLFWwindow*, int nw, int nh) 
{
	//This is called in response to the window resizing.
	//The new width and height are passed in so we make 
	//any necessary changes:
	Width = nw;
	Height = nh;
	//Tell the viewport to use all of our screen estate
	glViewport(0, 0, nw, nh);

	//This is not necessary, we're just working in 2d so
	//why not let our spaces reflect it?
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, static_cast<double>(Width)
		, 0.0, static_cast<double>(Height)
		, 1.0, -1.0);

	//Reserve memory for our render so that we don't do 
	//excessive allocations and render the image
	OutputImage.reserve(Width * Height * 3);
	render();
}


int main(int argc, char* argv[])
{
	// -------------------------------------------------
	// Initialize Window
	// -------------------------------------------------

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//We have an opengl context now. Everything from here on out 
	//is just managing our window or opengl directly.

	//Tell the opengl state machine we don't want it to make 
	//any assumptions about how pixels are aligned in memory 
	//during transfers between host and device (like glDrawPixels(...) )
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	//We call our resize function once to set everything up initially
	//after registering it as a callback with glfw
	glfwSetFramebufferSizeCallback(window, resize_callback);
	resize_callback(NULL, Width, Height);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		//Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// -------------------------------------------------------------
		//Rendering begins!
		glDrawPixels(Width, Height, GL_RGB, GL_FLOAT, &OutputImage[0]);
		//and ends.
		// -------------------------------------------------------------

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		//Close when the user hits 'q' or escape
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
			|| glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
