//
//  sphere_scene.c
//  Rasterizer
//
//
#pragma once
#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifndef M_PI  
#define M_PI 3.14159265358979323846  
#endif
using namespace glm;

class sphere_scene
{

public:
    int     gNumVertices = 0;    // Number of 3D vertices.
    int     gNumTriangles = 0;    // Number of triangles.
    int* gIndexBuffer = NULL; // Vertex indices for the triangles.

    float* gVertexBuffer = NULL; // Vertex location for the triangles.

    void clear() {
        delete[] gVertexBuffer;
        delete[] gIndexBuffer;
        gVertexBuffer = nullptr;
        gIndexBuffer = nullptr;
    }

    void create_scene()
    {
        int width = 32;
        int height = 16;

        float theta, phi;
        int t;

        gNumVertices = (height - 2) * width + 2;
        gNumTriangles = (height - 2) * (width - 1) * 2;

        // TODO: Allocate an array for gNumVertices vertices.
        gVertexBuffer = new float[3 * gNumVertices];
        gIndexBuffer = new int[3 * gNumTriangles];

        t = 0;
        for (int j = 1; j < height - 1; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                theta = (float)j / (height - 1) * M_PI;
                phi = (float)i / (width - 1) * M_PI * 2;

                float   x = sinf(theta) * cosf(phi);
                float   y = cosf(theta);
                float   z = -sinf(theta) * sinf(phi);

                // TODO: Set vertex t in the vertex array to {x, y, z}.
                gVertexBuffer[3 * t] = x;
                gVertexBuffer[3 * t + 1] = y;
                gVertexBuffer[3 * t + 2] = z;
                t++;
            }
        }

        // TODO: Set vertex t in the vertex array to {0, 1, 0}.
        gVertexBuffer[3 * t] = 0;
        gVertexBuffer[3 * t + 1] = 1;
        gVertexBuffer[3 * t + 2] = 0;
        t++;

        // TODO: Set vertex t in the vertex array to {0, -1, 0}.
        gVertexBuffer[3 * t] = 0;
        gVertexBuffer[3 * t + 1] = -1;
        gVertexBuffer[3 * t + 2] = 0;
        t++;

        t = 0;
        for (int j = 0; j < height - 3; ++j)
        {
            for (int i = 0; i < width - 1; ++i)
            {
                gIndexBuffer[t++] = j * width + i;
                gIndexBuffer[t++] = (j + 1) * width + (i + 1);
                gIndexBuffer[t++] = j * width + (i + 1);
                gIndexBuffer[t++] = j * width + i;
                gIndexBuffer[t++] = (j + 1) * width + i;
                gIndexBuffer[t++] = (j + 1) * width + (i + 1);
            }
        }
        for (int i = 0; i < width - 1; ++i)
        {
            gIndexBuffer[t++] = (height - 2) * width;
            gIndexBuffer[t++] = i;
            gIndexBuffer[t++] = i + 1;
            gIndexBuffer[t++] = (height - 2) * width + 1;
            gIndexBuffer[t++] = (height - 3) * width + (i + 1);
            gIndexBuffer[t++] = (height - 3) * width + i;
        }

        // The index buffer has now been generated. Here's how to use to determine
        // the vertices of a triangle. Suppose you want to determine the vertices
        // of triangle i, with 0 <= i < gNumTriangles. Define:
        //
        // k0 = gIndexBuffer[3*i + 0]
        // k1 = gIndexBuffer[3*i + 1]
        // k2 = gIndexBuffer[3*i + 2]
        //
        // Now, the vertices of triangle i are at positions k0, k1, and k2 (in that
        // order) in the vertex array (which you should allocate yourself at line
        // 27).
        //
        // Note that this assumes 0-based indexing of arrays (as used in C/C++,
        // Java, etc.) If your language uses 1-based indexing, you will have to
        // add 1 to k0, k1, and k2.
    }

    void render(mat4 MVP,int Width, int Height,vec3* v0, vec3* v1, vec3* v2, vec3* screen0, vec3* screen1, vec3* screen2, int i) {

        int k0 = gIndexBuffer[3 * i + 0];
        int k1 = gIndexBuffer[3 * i + 1];
        int k2 = gIndexBuffer[3 * i + 2];

        *v0 = vec3(gVertexBuffer[3 * k0 + 0], gVertexBuffer[3 * k0 + 1], gVertexBuffer[3 * k0 + 2]);
        *v1 = vec3(gVertexBuffer[3 * k1 + 0], gVertexBuffer[3 * k1 + 1], gVertexBuffer[3 * k1 + 2]);
        *v2 = vec3(gVertexBuffer[3 * k2 + 0], gVertexBuffer[3 * k2 + 1], gVertexBuffer[3 * k2 + 2]);

        vec4 clip0 = MVP * vec4(*v0, 1.0f);
        vec4 clip1 = MVP * vec4(*v1, 1.0f);
        vec4 clip2 = MVP * vec4(*v2, 1.0f);

        vec3 ndc0 = vec3(clip0) / clip0.w;
        vec3 ndc1 = vec3(clip1) / clip1.w;
        vec3 ndc2 = vec3(clip2) / clip2.w;

        *screen0 = vec3((ndc0.x * 0.5f + 0.5f) * Width, (1 - (ndc0.y * 0.5f + 0.5f)) * Height, ndc0.z);
        *screen1 = vec3((ndc1.x * 0.5f + 0.5f) * Width, (1 - (ndc1.y * 0.5f + 0.5f)) * Height, ndc1.z);
        *screen2 = vec3((ndc2.x * 0.5f + 0.5f) * Width, (1 - (ndc2.y * 0.5f + 0.5f)) * Height, ndc2.z);

    }
    
};
