#include "pch.h"
#include "Hot3dxDrawnObject.h"
#include "Hot3dxObjectGeometry.h"

using namespace DirectX;
using namespace DirectX::DXTKXAML12;
using namespace std;
using Microsoft::WRL::ComPtr;
using namespace Hot3dxRotoDraw;
namespace DirectX
{
    namespace DXTKXAML12
    {

        const float SQRT2 = 1.41421356237309504880f;
        const float SQRT3 = 1.73205080756887729352f;
        const float SQRT6 = 2.44948974278317809820f;

        inline void CheckIndexOverflowHot3dx(size_t value)
        {
            // Use >=, not > comparison, because some D3D level 9_x hardware does not support 0xFFFF index values.
            if (value >= USHRT_MAX)
                throw std::exception("Index value out of range: cannot tesselate primitive so finely");
        }


        // Collection types used when generating the geometry.
        inline void index_push_backHot3dx(IndexCollectionColor& indices, size_t value)
        {
            CheckIndexOverflowHot3dx(value);
            indices.push_back(static_cast<uint16_t>(value));
        }


        // Helper for flipping winding of geometric primitives for LH vs. RH coords
        inline void ReverseWindingHot3dx(IndexCollectionColor& indices, VertexCollectionColor& vertices)
        {
            assert((indices.size() % 3) == 0);
            for (auto it = indices.begin(); it != indices.end(); it += 3)
            {
                std::swap(*it, *(it + 2));
            }

            for (auto it = vertices.begin(); it != vertices.end(); ++it)
            {
                // it->position.x = (1.f - it->position.x);
            }
        }

        // Helper for flipping winding of geometric primitives for LH vs. RH coords
        inline void ReverseWindingHot3dx(IndexCollectionColor& indices, VertexCollectionPositionNormalTextureTangent& vertices)
        {
            assert((indices.size() % 3) == 0);
            for (auto it = indices.begin(); it != indices.end(); it += 3)
            {
                std::swap(*it, *(it + 2));
            }

            for (auto it = vertices.begin(); it != vertices.end(); ++it)
            {
                // it->position.x = (1.f - it->position.x);
            }
        }

        // Helper for flipping winding of geometric primitives for LH vs. RH coords
        inline void ReverseWindingHot3dx(IndexCollectionColor& indices, VertexCollectionDualTexture& vertices)
        {
            assert((indices.size() % 3) == 0);
            for (auto it = indices.begin(); it != indices.end(); it += 3)
            {
                std::swap(*it, *(it + 2));
            }

            for (auto it = vertices.begin(); it != vertices.end(); ++it)
            {
                // it->position.x = (1.f - it->position.x);
            }
        }

        // Helper for inverting normals of geometric primitives for 'inside' vs. 'outside' viewing
        inline void InvertNormalsHot3dxPositionNormalTexture(VertexCollectionPositionNormalTexture& vertices)
        {
            for (auto it = vertices.begin(); it != vertices.end(); ++it)
            {
                it->normal.x = -it->normal.x;
                it->normal.y = -it->normal.y;
                it->normal.z = -it->normal.z;
            }
        }
        // Helper for inverting normals of geometric primitives for 'inside' vs. 'outside' viewing
        inline void InvertNormalsHot3dxPositionNormalColorTexture(VertexCollectionPositionNormalColorTexture& vertices)
        {
            for (auto it = vertices.begin(); it != vertices.end(); ++it)
            {
                it->normal.x = -it->normal.x;
                it->normal.y = -it->normal.y;
                it->normal.z = -it->normal.z;
            }
        }

        // Helper computes a point on a unit circle, aligned to the x/z plane and centered on the origin.
        inline XMVECTOR GetCircleVectorColor(size_t i, size_t tessellation)
        {
            float angle = i * XM_2PI / tessellation;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, angle);

            XMVECTORF32 v = { { { dx, 0, dz, 0 } } };
            return v;
        }

        inline XMVECTOR GetCircleTangentColor(size_t i, size_t tessellation)
        {
            float angle = (i * XM_2PI / tessellation) + XM_PIDIV2;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, angle);

            XMVECTORF32 v = { { { dx, 0, dz, 0 } } };
            return v;
        }

        // Helper creates a triangle fan to close the end of a cylinder / cone
       inline  // Helper creates a triangle fan to close the end of a cylinder / cone
           void CreateCylinderCapColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, size_t tessellation, float height, float radius, bool isTop)
       {
           XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

           // Create cap indices.
           for (size_t i = 0; i < tessellation - 2; i++)
           {
               size_t i1 = (i + 1) % tessellation;
               size_t i2 = (i + 2) % tessellation;

               if (isTop)
               {
                   std::swap(i1, i2);
               }

               size_t vbase = vertices.size();
               index_push_backHot3dx(indices, vbase);
               index_push_backHot3dx(indices, vbase + i1);
               index_push_backHot3dx(indices, vbase + i2);
           }

           // Which end of the cylinder is this?
           XMVECTOR normal = g_XMIdentityR1;
           XMVECTOR textureScale = g_XMNegativeOneHalf;

           if (!isTop)
           {
               normal = XMVectorNegate(normal);
               textureScale = XMVectorMultiply(textureScale, g_XMNegateX);
           }

           // Create cap vertices.
           for (size_t i = 0; i < tessellation; i++)
           {
               XMVECTOR circleVector = GetCircleVectorColor(i, tessellation);

               XMVECTOR position = XMVectorAdd(XMVectorScale(circleVector, radius), XMVectorScale(normal, height));

               XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

               vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));
           }
       }

       // Helper creates a triangle fan to close the end of a cylinder / cone
       inline  // Helper creates a triangle fan to close the end of a cylinder / cone
           void CreateCylinderCapPositionNormalTextureTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, size_t tessellation, float height, float radius, bool isTop)
       {
           XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

           // Create cap indices.
           for (size_t i = 0; i < tessellation - 2; i++)
           {
               size_t i1 = (i + 1) % tessellation;
               size_t i2 = (i + 2) % tessellation;

               if (isTop)
               {
                   std::swap(i1, i2);
               }

               size_t vbase = vertices.size();
               index_push_backHot3dx(indices, vbase);
               index_push_backHot3dx(indices, vbase + i1);
               index_push_backHot3dx(indices, vbase + i2);
           }

           // Which end of the cylinder is this?
           XMVECTOR normal = g_XMIdentityR1;
           XMVECTOR textureScale = g_XMNegativeOneHalf;

           if (!isTop)
           {
               normal = XMVectorNegate(normal);
               textureScale = XMVectorMultiply(textureScale, g_XMNegateX);
           }

           // Create cap vertices.
           for (size_t i = 0; i < tessellation; i++)
           {
               XMVECTOR circleVector = GetCircleVectorColor(i, tessellation);

               XMVECTOR position = XMVectorAdd(XMVectorScale(circleVector, radius), XMVectorScale(normal, height));
               float  x = XMVectorGetX(position);
               float  y = XMVectorGetY(position);
               float  z = XMVectorGetZ(position);

               XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

               XMVECTOR incident = position;
               float a = XMVectorGetX(normal) * cosf(x);
               float b = XMVectorGetY(normal) * sinf(y);
               XMVECTOR t2 = { a, b, XMVectorGetZ(normal) * tanf(z) };
               XMVECTOR tan = XMVector3Cross(incident, normal);
               Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt =
                   Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinate, tan);
               vertices.push_back(vpntt);
           }
       }

       // Helper creates a triangle fan to close the end of a cylinder / cone
       inline  // Helper creates a triangle fan to close the end of a cylinder / cone
           void CreateCylinderCapDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, size_t tessellation, float height, float radius, bool isTop)
       {
           XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

           // Create cap indices.
           for (size_t i = 0; i < tessellation - 2; i++)
           {
               size_t i1 = (i + 1) % tessellation;
               size_t i2 = (i + 2) % tessellation;

               if (isTop)
               {
                   std::swap(i1, i2);
               }

               size_t vbase = vertices.size();
               index_push_backHot3dx(indices, vbase);
               index_push_backHot3dx(indices, vbase + i1);
               index_push_backHot3dx(indices, vbase + i2);
           }

           // Which end of the cylinder is this?
           XMVECTOR normal = g_XMIdentityR1;
           XMVECTOR textureScale = g_XMNegativeOneHalf;

           if (!isTop)
           {
               normal = XMVectorNegate(normal);
               textureScale = XMVectorMultiply(textureScale, g_XMNegateX);
           }

           // Create cap vertices.
           for (size_t i = 0; i < tessellation; i++)
           {
               XMVECTOR circleVector = GetCircleVectorColor(i, tessellation);

               XMVECTOR position = XMVectorAdd(XMVectorScale(circleVector, radius), XMVectorScale(normal, height));

               XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

               vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinate, textureCoordinate));
           }
       }


void ComputeBoxColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, const XMFLOAT3& size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    // A box has six faces, each one pointing in a different direction.
    const int FaceCount = 6;

    const XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    FXMVECTOR colorv = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    static const XMVECTORF32 faceNormals[FaceCount] =
    {
        { { {  0,  0,  1, 0 } } },
        { { {  0,  0, -1, 0 } } },
        { { {  1,  0,  0, 0 } } },
        { { { -1,  0,  0, 0 } } },
        { { {  0,  1,  0, 0 } } },
        { { {  0, -1,  0, 0 } } }
    };

    static const XMVECTORF32 textureCoordinates[4] =
    {
        { { { 1, 0, 0, 0 } } },
        { { { 1, 1, 0, 0 } } },
        { { { 0, 1, 0, 0 } } },
        { { { 0, 0, 0, 0 } } }
    };

    XMVECTOR tsize = XMLoadFloat3(&size);
    tsize = XMVectorDivide(tsize, g_XMTwo);

    // Create each face in turn.
    for (int i = 0; i < FaceCount; i++)
    {
        XMVECTOR normal = faceNormals[i];

        // Get two vectors perpendicular both to the face normal and to each other.
        XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

        XMVECTOR side1 = XMVector3Cross(normal, basis);
        XMVECTOR side2 = XMVector3Cross(normal, side1);

        // Six indices (two triangles) per face.
        size_t vbase = vertices.size();
        index_push_backHot3dx(indices, vbase + 0);
        index_push_backHot3dx(indices, vbase + 1);
        index_push_backHot3dx(indices, vbase + 2);

        index_push_backHot3dx(indices, vbase + 0);
        index_push_backHot3dx(indices, vbase + 2);
        index_push_backHot3dx(indices, vbase + 3);

       
        // Four vertices per face.
        // (normal - side1 - side2) * tsize // normal // t0
        FXMVECTOR t = XMVectorMultiply(XMVectorSubtract(XMVectorSubtract(normal, side1), side2), tsize);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(t, colorv));

        // (normal - side1 + side2) * tsize // normal // t1
        FXMVECTOR t1 = XMVectorMultiply(XMVectorAdd(XMVectorSubtract(normal, side1), side2), tsize);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(t1, colorv));

        // (normal + side1 + side2) * tsize // normal // t2
        FXMVECTOR t2 = XMVectorMultiply(XMVectorAdd(normal, XMVectorAdd(side1, side2)), tsize);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(t2, colorv));

        // (normal + side1 - side2) * tsize // normal // t3
        FXMVECTOR t3 = XMVectorMultiply(XMVectorSubtract(XMVectorAdd(normal, side1), side2), tsize);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(t3, colorv));
    }

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeSphereColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();
    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    FXMVECTOR colorv = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    float radius = diameter / 2;

    // Create rings of vertices at progressively higher latitudes.
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - float(i) / verticalSegments;

        float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
        float dy, dxz;

        XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of vertices at this latitude.
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = float(j) / horizontalSegments;

            float longitude = j * XM_2PI / horizontalSegments;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            FXMVECTOR t = XMVectorScale(normal, radius);
            vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(t, colorv));
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = (j + 1) % stride;

            index_push_backHot3dx(indices, i * stride + j);
            index_push_backHot3dx(indices, nextI * stride + j);
            index_push_backHot3dx(indices, i * stride + nextJ);

            index_push_backHot3dx(indices, i * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + j);
            index_push_backHot3dx(indices, nextI * stride + nextJ);
        }
    }

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeGeoSphereColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
    // Becuse this edge is undirected, (a,b) is the same as (b,a).
    typedef std::pair<uint16_t, uint16_t> UndirectedEdge;

    // Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
    // we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
    auto makeUndirectedEdge = [](uint16_t a, uint16_t b)
    {
        return std::make_pair(max(a, b), min(a, b));
    };

    // Key: an edge
    // Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
    // This map is used to avoid duplicating vertices when subdividing triangles along edges.
    typedef std::map<UndirectedEdge, uint16_t> EdgeSubdivisionMap;


    static const XMFLOAT3 OctahedronVertices[] =
    {
        // when looking down the negative z-axis (into the screen)
        XMFLOAT3(0,  1,  0), // 0 top
        XMFLOAT3(0,  0, -1), // 1 front
        XMFLOAT3(1,  0,  0), // 2 right
        XMFLOAT3(0,  0,  1), // 3 back
        XMFLOAT3(-1,  0,  0), // 4 left
        XMFLOAT3(0, -1,  0) // 5 bottom
    };
    static const uint16_t OctahedronIndices[] =
    {
        0, 1, 2, // top front-right face
        0, 2, 3, // top back-right face
        0, 3, 4, // top back-left face
        0, 4, 1, // top front-left face
        5, 1, 4, // bottom front-left face
        5, 4, 3, // bottom back-left face
        5, 3, 2, // bottom back-right face
        5, 2, 1 // bottom front-right face
    };

    const float radius = diameter / 2.0f;

    // Start with an octahedron; copy the data into the vertex/index collection.

    std::vector<XMFLOAT3> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));

    indices.insert(indices.begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));

    // We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
    // about to go on, these values aren't ever going to change because the vertices don't move around in the array.
    // We'll need these values later on to fix the singularities that show up at the poles.
    const uint16_t northPoleIndex = 0;
    const uint16_t southPoleIndex = 5;

    for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
    {
        assert(indices.size() % 3 == 0); // sanity

        // We use this to keep track of which edges have already been subdivided.
        EdgeSubdivisionMap subdividedEdges;

        // The new index collection after subdivision.
        IndexCollectionColor newIndices;

        const size_t triangleCount = indices.size() / 3;
        for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
        {
            // For each edge on this triangle, create a new vertex in the middle of that edge.
            // The winding order of the triangles we output are the same as the winding order of the inputs.

            // Indices of the vertices making up this triangle
            uint16_t iv0 = indices[iTriangle * 3 + 0];
            uint16_t iv1 = indices[iTriangle * 3 + 1];
            uint16_t iv2 = indices[iTriangle * 3 + 2];

            // Get the new vertices
            XMFLOAT3 v01; // vertex on the midpoint of v0 and v1
            XMFLOAT3 v12; // ditto v1 and v2
            XMFLOAT3 v20; // ditto v2 and v0
            uint16_t iv01; // index of v01
            uint16_t iv12; // index of v12
            uint16_t iv20; // index of v20

            // Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
            auto divideEdge = [&](uint16_t i0, uint16_t i1, XMFLOAT3& outVertex, uint16_t& outIndex)
            {
                const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

                // Check to see if we've already generated this vertex
                auto it = subdividedEdges.find(edge);
                if (it != subdividedEdges.end())
                {
                    // We've already generated this vertex before
                    outIndex = it->second; // the index of this vertex
                    outVertex = vertexPositions[outIndex]; // and the vertex itself
                }
                else
                {
                    // Haven't generated this vertex before: so add it now

                    // outVertex = (vertices[i0] + vertices[i1]) / 2
                    XMStoreFloat3(
                        &outVertex,
                        XMVectorScale(
                            XMVectorAdd(XMLoadFloat3(&vertexPositions[i0]), XMLoadFloat3(&vertexPositions[i1])),
                            0.5f
                        )
                    );

                    outIndex = static_cast<uint16_t>(vertexPositions.size());
                    CheckIndexOverflowHot3dx(outIndex);
                    vertexPositions.push_back(outVertex);

                    // Now add it to the map.
                    auto entry = std::make_pair(edge, outIndex);
                    subdividedEdges.insert(entry);
                }
            };

            // Add/get new vertices and their indices
            divideEdge(iv0, iv1, v01, iv01);
            divideEdge(iv1, iv2, v12, iv12);
            divideEdge(iv0, iv2, v20, iv20);

            // Add the new indices. We have four new triangles from our original one:
            //        v0
            //        o
            //       /a\
            //  v20 o---o v01
            //     /b\c/d\
            // v2 o---o---o v1
            //       v12
            const uint16_t indicesToAdd[] =
            {
                 iv0, iv01, iv20, // a
                iv20, iv12,  iv2, // b
                iv20, iv01, iv12, // c
                iv01,  iv1, iv12, // d
            };
            newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
        }

        indices = std::move(newIndices);
    }

    // Now that we've completed subdivision, fill in the final vertex collection
    vertices.reserve(vertexPositions.size());
    for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it)
    {
        auto& vertexValue = *it;

        auto normal = XMVector3Normalize(XMLoadFloat3(&vertexValue));
        auto pos = XMVectorScale(normal, radius);

        XMFLOAT3 normalFloat3;
        XMStoreFloat3(&normalFloat3, normal);

        // calculate texture coordinates for this vertex
        float longitude = atan2(normalFloat3.x, -normalFloat3.z);
        float latitude = acos(normalFloat3.y);

        float u = longitude / XM_2PI + 0.5f;
        float v = latitude / XM_PI;

        auto texcoord = XMVectorSet(1.0f - u, v, 0.0f, 0.0f);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)), color));
    }

    // There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
    // a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
    // occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
    // right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
    // 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
    // 
    // Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
    // completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
    // y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
    // need to duplicate our vertices - and provide the correct texture coordinates.
    size_t preFixupVertexCount = vertices.size();
    for (size_t i = 0; i < preFixupVertexCount; ++i)
    {
        // This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
        bool isOnPrimeMeridian = XMVector2NearEqual(
            XMVectorSet(vertices[i].position.x, vertices[i].position.x, 0.0f, 0.0f),
            XMVectorZero(),
            XMVectorSplatEpsilon());

        if (isOnPrimeMeridian)
        {
            size_t newIndex = vertices.size(); // the index of this vertex that we're about to add
            CheckIndexOverflowHot3dx(newIndex);

            // copy this vertex, correct the texture coordinate, and add the vertex
            DirectX::DXTKXAML12::VertexPositionColor v = vertices[i];
            //v.textureCoordinate.x = 1.0f;
            vertices.push_back(v);

            // Now find all the triangles which contain this vertex and update them if necessary
            for (size_t j = 0; j < indices.size(); j += 3)
            {
                uint16_t* triIndex0 = &indices[j + 0];
                uint16_t* triIndex1 = &indices[j + 1];
                uint16_t* triIndex2 = &indices[j + 2];

                if (*triIndex0 == i)
                {
                    // nothing; just keep going
                }
                else if (*triIndex1 == i)
                {
                    std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
                }
                else if (*triIndex2 == i)
                {
                    std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
                }
                else
                {
                    // this triangle doesn't use the vertex we're interested in
                    continue;
                }

                // If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
                assert(*triIndex0 == i);
                assert(*triIndex1 != i && *triIndex2 != i); // assume no degenerate triangles

                const DirectX::DXTKXAML12::VertexPositionColor& v0 = vertices[*triIndex0];
                const DirectX::DXTKXAML12::VertexPositionColor& v1 = vertices[*triIndex1];
                const DirectX::DXTKXAML12::VertexPositionColor& v2 = vertices[*triIndex2];

                // check the other two vertices to see if we might need to fix this triangle

                if (abs(v0.position.x - v1.position.x) > 0.5f ||
                    abs(v0.position.x - v2.position.x) > 0.5f)
                {
                    // yep; replace the specified index to point to the new, corrected vertex
                    *triIndex0 = static_cast<uint16_t>(newIndex);
                }
            }
        }
    }

    // And one last fix we need to do: the poles. A common use-case of a sphere mesh is to map a rectangular texture onto
    // it. If that happens, then the poles become singularities which map the entire top and bottom rows of the texture
    // onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
    // spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
    // poles, but reduce stretching.
    auto fixPole = [&](size_t poleIndex)
    {
        auto& poleVertex = vertices[poleIndex];
        bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            // These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
            // entry in the index array which represents the pole index, and the other two pointers point to the other
            // two indices making up this triangle.
            uint16_t* pPoleIndex;
            uint16_t* pOtherIndex0;
            uint16_t* pOtherIndex1;
            if (indices[i + 0] == poleIndex)
            {
                pPoleIndex = &indices[i + 0];
                pOtherIndex0 = &indices[i + 1];
                pOtherIndex1 = &indices[i + 2];
            }
            else if (indices[i + 1] == poleIndex)
            {
                pPoleIndex = &indices[i + 1];
                pOtherIndex0 = &indices[i + 2];
                pOtherIndex1 = &indices[i + 0];
            }
            else if (indices[i + 2] == poleIndex)
            {
                pPoleIndex = &indices[i + 2];
                pOtherIndex0 = &indices[i + 0];
                pOtherIndex1 = &indices[i + 1];
            }
            else
            {
                continue;
            }

            const auto& otherVertex0 = vertices[*pOtherIndex0];
            const auto& otherVertex1 = vertices[*pOtherIndex1];

            // Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
            DirectX::DXTKXAML12::VertexPositionColor newPoleVertex = poleVertex;
            newPoleVertex.position.x = (otherVertex0.position.x + otherVertex1.position.x) / 2;
            newPoleVertex.position.y = poleVertex.position.y;

            if (!overwrittenPoleVertex)
            {
                vertices[poleIndex] = newPoleVertex;
                overwrittenPoleVertex = true;
            }
            else
            {
                CheckIndexOverflowHot3dx(vertices.size());

                *pPoleIndex = static_cast<uint16_t>(vertices.size());
                vertices.push_back(newPoleVertex);
            }
        }
    };

    fixPole(northPoleIndex);
    fixPole(southPoleIndex);

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

//--------------------------------------------------------------------------------------
// Cylinder / Cone
//--------------------------------------------------------------------------------------



   

void ComputeCylinderColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    height /= 2;

    XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cylinder.
    for (size_t i = 0; i <= tessellation; i++)
    {
        XMVECTOR normal = GetCircleVectorColor(i, tessellation);

        XMVECTOR sideOffset = XMVectorScale(normal, radius);

        float u = float(i) / tessellation;

        XMVECTOR textureCoordinate = XMLoadFloat(&u);

        XMVECTOR t = XMVectorAdd(sideOffset, topOffset);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t)), color));
        t = XMVectorSubtract(sideOffset, topOffset);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t)), color));

        index_push_backHot3dx(indices, i * 2);
        index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
        index_push_backHot3dx(indices, i * 2 + 1);

        index_push_backHot3dx(indices, i * 2 + 1);
        index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
        index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the top and bottom.
    DirectX::DXTKXAML12::CreateCylinderCapColor(vertices, indices, tessellation, height, radius, true);
    DirectX::DXTKXAML12::CreateCylinderCapColor(vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeConeColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    height /= 2;

    XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cone.
    for (size_t i = 0; i <= tessellation; i++)
    {
        XMVECTOR circlevec = GetCircleVectorColor(i, tessellation);

        XMVECTOR sideOffset = XMVectorScale(circlevec, radius);

        float u = float(i) / tessellation;

        XMVECTOR textureCoordinate = XMLoadFloat(&u);

        XMVECTOR pt = XMVectorSubtract(sideOffset, topOffset);

        XMVECTOR normal = XMVector3Cross(
            GetCircleTangentColor(i, tessellation),
            XMVectorSubtract(topOffset, pt));
        normal = XMVector3Normalize(normal);

        // Duplicate the top vertex for distinct normals
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(topOffset), XMVectorGetY(topOffset), XMVectorGetZ(topOffset)), color));
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(pt), XMVectorGetY(pt), XMVectorGetZ(pt)), color));

        index_push_backHot3dx(indices, i * 2);
        index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
        index_push_backHot3dx(indices, (i * 2 + 1) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the bottom.
    DirectX::DXTKXAML12::CreateCylinderCapColor(vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeTorusColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t stride = tessellation + 1;

    // First we loop around the main ring of the torus.
    for (size_t i = 0; i <= tessellation; i++)
    {
        float u = float(i) / tessellation;

        float outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

        // Create a transform matrix that will align geometry to
        // slice perpendicularly though the current ring position.
        XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

        // Now we loop along the other axis, around the side of the tube.
        for (size_t j = 0; j <= tessellation; j++)
        {
            float v = 1 - float(j) / tessellation;

            float innerAngle = j * XM_2PI / tessellation + XM_PI;
            float dx, dy;

            XMScalarSinCos(&dy, &dx, innerAngle);

            // Create a vertex.
            XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
            XMVECTOR position = XMVectorScale(normal, thickness / 2);
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            position = XMVector3Transform(position, transform);
            normal = XMVector3TransformNormal(normal, transform);

            vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));

            // And create indices for two triangles.
            size_t nextI = (i + 1) % stride;
            size_t nextJ = (j + 1) % stride;

            index_push_backHot3dx(indices, i * stride + j);
            index_push_backHot3dx(indices, i * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + j);

            index_push_backHot3dx(indices, i * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + j);
        }
    }

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeTetrahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const XMVECTORF32 verts[4] =
    {
        { { {              0.f,          0.f,        1.f, 0 } } },
        { { {  2.f * SQRT2 / 3.f,          0.f, -1.f / 3.f, 0 } } },
        { { {     -SQRT2 / 3.f,  SQRT6 / 3.f, -1.f / 3.f, 0 } } },
        { { {     -SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f, 0 } } }
    };

    static const uint32_t faces[4 * 3] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,
        1, 3, 2
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 4 * 3);
    assert(indices.size() == 4 * 3);

}

void ComputeOctahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const XMVECTORF32 verts[6] =
    {
        { { {  1,  0,  0, 0 } } },
        { { { -1,  0,  0, 0 } } },
        { { {  0,  1,  0, 0 } } },
        { { {  0, -1,  0, 0 } } },
        { { {  0,  0,  1, 0 } } },
        { { {  0,  0, -1, 0 } } }
    };

    static const uint32_t faces[8 * 3] =
    {
        4, 0, 2,
        4, 2, 1,
        4, 1, 3,
        4, 3, 0,
        5, 2, 0,
        5, 1, 2,
        5, 3, 1,
        5, 0, 3
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 8 * 3);
    assert(indices.size() == 8 * 3);

}

void ComputeDodecahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const float a = 1.f / SQRT3;
    static const float b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
    static const float c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

    static const XMVECTORF32 verts[20] =
    {
        { { {  a,  a,  a, 0 } } },
        { { {  a,  a, -a, 0 } } },
        { { {  a, -a,  a, 0 } } },
        { { {  a, -a, -a, 0 } } },
        { { { -a,  a,  a, 0 } } },
        { { { -a,  a, -a, 0 } } },
        { { { -a, -a,  a, 0 } } },
        { { { -a, -a, -a, 0 } } },
        { { {  b,  c,  0, 0 } } },
        { { { -b,  c,  0, 0 } } },
        { { {  b, -c,  0, 0 } } },
        { { { -b, -c,  0, 0 } } },
        { { {  c,  0,  b, 0 } } },
        { { {  c,  0, -b, 0 } } },
        { { { -c,  0,  b, 0 } } },
        { { { -c,  0, -b, 0 } } },
        { { {  0,  b,  c, 0 } } },
        { { {  0, -b,  c, 0 } } },
        { { {  0,  b, -c, 0 } } },
        { { {  0, -b, -c, 0 } } }
    };

    static const uint32_t faces[12 * 5] =
    {
        0, 8, 9, 4, 16,
        0, 16, 17, 2, 12,
        12, 2, 10, 3, 13,
        9, 5, 15, 14, 4,
        3, 19, 18, 1, 13,
        7, 11, 6, 14, 15,
        0, 12, 13, 1, 8,
        8, 1, 18, 5, 9,
        16, 4, 14, 6, 17,
        6, 11, 10, 2, 17,
        7, 15, 5, 18, 19,
        7, 19, 3, 10, 11
    };

    static const XMVECTORF32 textureCoordinates[5] =
    {
        { { {  0.654508f, 0.0244717f, 0, 0 } } },
        { { { 0.0954915f,  0.206107f, 0, 0 } } },
        { { { 0.0954915f,  0.793893f, 0, 0 } } },
        { { {  0.654508f,  0.975528f, 0, 0 } } },
        { { {        1.f,       0.5f, 0, 0 } } }
    };

    static const uint32_t textureIndex[12][5] =
    {
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 2, 3, 4, 0, 1 },
        { 0, 1, 2, 3, 4 },
        { 1, 2, 3, 4, 0 },
        { 4, 0, 1, 2, 3 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 }
    };

    size_t t = 0;
    for (size_t j = 0; j < _countof(faces); j += 5, ++t)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];
        uint32_t v3 = faces[j + 3];
        uint32_t v4 = faces[j + 4];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 2);
        index_push_backHot3dx(indices, base + 3);

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 3);
        index_push_backHot3dx(indices, base + 4);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v3], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v4], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 12 * 5);
    assert(indices.size() == 12 * 3 * 3);
}

void ComputeIcosahedronColor(VertexCollectionColor& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const float  t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
    static const float t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

    static const XMVECTORF32 verts[12] =
    {
        { { {    t / t2,  1.f / t2,       0, 0 } } },
        { { {   -t / t2,  1.f / t2,       0, 0 } } },
        { { {    t / t2, -1.f / t2,       0, 0 } } },
        { { {   -t / t2, -1.f / t2,       0, 0 } } },
        { { {  1.f / t2,       0,    t / t2, 0 } } },
        { { {  1.f / t2,       0,   -t / t2, 0 } } },
        { { { -1.f / t2,       0,    t / t2, 0 } } },
        { { { -1.f / t2,       0,   -t / t2, 0 } } },
        { { {       0,    t / t2,  1.f / t2, 0 }  } },
        { { {       0,   -t / t2,  1.f / t2, 0 } } },
        { { {       0,    t / t2, -1.f / t2, 0 } } },
        { { {       0,   -t / t2, -1.f / t2, 0 } } }
    };

    static const uint32_t faces[20 * 3] =
    {
        0, 8, 4,
        0, 5, 10,
        2, 4, 9,
        2, 11, 5,
        1, 6, 8,
        1, 10, 7,
        3, 9, 6,
        3, 7, 11,
        0, 10, 8,
        1, 8, 10,
        2, 9, 11,
        3, 11, 9,
        4, 2, 0,
        5, 0, 2,
        6, 1, 3,
        7, 3, 1,
        8, 6, 4,
        9, 4, 6,
        10, 5, 7,
        11, 7, 5
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionColor(XMFLOAT3(XMVectorGetX(position), XMVectorGetY(position), XMVectorGetZ(position)), color));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 20 * 3);
    assert(indices.size() == 20 * 3);
}

void ComputeBoxTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, const XMFLOAT3& size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    // A box has six faces, each one pointing in a different direction.
    const int FaceCount = 6;

    XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

    static const XMVECTORF32 faceNormals[FaceCount] =
    {
        { { {  0,  0,  1, 0 } } },
        { { {  0,  0, -1, 0 } } },
        { { {  1,  0,  0, 0 } } },
        { { { -1,  0,  0, 0 } } },
        { { {  0,  1,  0, 0 } } },
        { { {  0, -1,  0, 0 } } }
    };

    static const XMVECTORF32 textureCoordinates[4] =
    {
        { { { 1, 0, 0, 0 } } },
        { { { 1, 1, 0, 0 } } },
        { { { 0, 1, 0, 0 } } },
        { { { 0, 0, 0, 0 } } }
    };

    XMVECTOR tsize = XMLoadFloat3(&size);
    tsize = XMVectorDivide(tsize, g_XMTwo);

    // Create each face in turn.
    for (int i = 0; i < FaceCount; i++)
    {
        XMVECTOR normal = faceNormals[i];

        // Get two vectors perpendicular both to the face normal and to each other.
        XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

        XMVECTOR side1 = XMVector3Cross(normal, basis);
        XMVECTOR side2 = XMVector3Cross(normal, side1);

        // Six indices (two triangles) per face.
        size_t vbase = vertices.size();
        index_push_backHot3dx(indices, vbase + 0);
        index_push_backHot3dx(indices, vbase + 1);
        index_push_backHot3dx(indices, vbase + 2);

        index_push_backHot3dx(indices, vbase + 0);
        index_push_backHot3dx(indices, vbase + 2);
        index_push_backHot3dx(indices, vbase + 3);
        
        // Four vertices per face.
        // (normal - side1 - side2) * tsize // normal // t0
        vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent((DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(normal, side1), side2), tsize),
            normal, textureCoordinates[0], side1));
       

        // (normal - side1 + side2) * tsize // normal // t1
        vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent((DirectX::XMVectorMultiply(DirectX::XMVectorAdd(DirectX::XMVectorSubtract(normal, side1), side2), tsize)),
            normal, textureCoordinates[1], side1));

        // (normal + side1 + side2) * tsize // normal // t2
        vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent((DirectX::XMVectorMultiply(DirectX::XMVectorAdd(normal, DirectX::XMVectorAdd(side1, side2)), tsize)), normal, textureCoordinates[2], side2));

        // (normal + side1 - side2) * tsize // normal // t3
        vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent((DirectX::XMVectorMultiply(DirectX::XMVectorSubtract(DirectX::XMVectorAdd(normal, side1), side2), tsize)), normal, textureCoordinates[3], side2));
    }

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}


void ComputeSphereTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();
    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
    FXMVECTOR colorv = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    float radius = diameter / 2;

    // Create rings of vertices at progressively higher latitudes.
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - float(i) / verticalSegments;

        float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
        float dy, dxz;

        XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of vertices at this latitude.
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = float(j) / horizontalSegments;

            float longitude = j * XM_2PI / horizontalSegments;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            FXMVECTOR t = XMVectorScale(normal, radius);
            float  x = XMVectorGetX(t);
            float  y = XMVectorGetY(t);
            float  z = XMVectorGetZ(t);

            XMVECTOR incident = DirectX::XMVECTOR(XMVectorSet(x, y, z, 0.0f));
            float a = XMVectorGetX(normal) * cosf(x);
            float b = XMVectorGetY(normal) * sinf(y);
            XMVECTOR t2 = { a, b, XMVectorGetZ(normal) * tanf(z) };
            XMVECTOR tan = XMVector3Cross(incident, normal);


            Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt =
                Hot3dxRotoDraw::VertexPositionNormalTextureTangent(
                    XMFLOAT3(x, y, z),
                    XMFLOAT3(XMVectorGetX(normal), XMVectorGetY(normal), XMVectorGetZ(normal)),
                    XMFLOAT2(XMVectorGetX(textureCoordinate), XMVectorGetY(textureCoordinate)),
                    XMFLOAT4(XMVectorGetX(tan), XMVectorGetY(tan), XMVectorGetZ(tan), XMVectorGetW(tan)
                    )
                );
            vertices.push_back(vpntt);
        }

        // Fill the index buffer with triangles joining each pair of latitude rings.
        size_t stride = horizontalSegments + 1;

        for (size_t l = 0; l < verticalSegments; l++)
        {
            for (size_t j = 0; j <= horizontalSegments; j++)
            {
                size_t nextI = l + 1;
                size_t nextJ = (j + 1) % stride;

                index_push_backHot3dx(indices, l * stride + j);
                index_push_backHot3dx(indices, nextI * stride + j);
                index_push_backHot3dx(indices, l * stride + nextJ);

                index_push_backHot3dx(indices, l * stride + nextJ);
                index_push_backHot3dx(indices, nextI * stride + j);
                index_push_backHot3dx(indices, nextI * stride + nextJ);
            }
        }

        // Build RH above
        if (!rhcoords) {
            ReverseWindingHot3dx(indices, vertices);
        }
    }
}
    
void ComputeGeoSphereTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
    // Becuse this edge is undirected, (a,b) is the same as (b,a).
    typedef std::pair<uint16_t, uint16_t> UndirectedEdge;

    // Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
    // we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
    auto makeUndirectedEdge = [](uint16_t a, uint16_t b)
    {
        return std::make_pair(max(a, b), min(a, b));
    };

    // Key: an edge
    // Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
    // This map is used to avoid duplicating vertices when subdividing triangles along edges.
    typedef std::map<UndirectedEdge, uint16_t> EdgeSubdivisionMap;


    static const XMFLOAT3 OctahedronVertices[] =
    {
        // when looking down the negative z-axis (into the screen)
        XMFLOAT3(0,  1,  0), // 0 top
        XMFLOAT3(0,  0, -1), // 1 front
        XMFLOAT3(1,  0,  0), // 2 right
        XMFLOAT3(0,  0,  1), // 3 back
        XMFLOAT3(-1,  0,  0), // 4 left
        XMFLOAT3(0, -1,  0) // 5 bottom
    };
    static const uint16_t OctahedronIndices[] =
    {
        0, 1, 2, // top front-right face
        0, 2, 3, // top back-right face
        0, 3, 4, // top back-left face
        0, 4, 1, // top front-left face
        5, 1, 4, // bottom front-left face
        5, 4, 3, // bottom back-left face
        5, 3, 2, // bottom back-right face
        5, 2, 1 // bottom front-right face
    };

    const float radius = diameter / 2.0f;

    // Start with an octahedron; copy the data into the vertex/index collection.

    std::vector<XMFLOAT3> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));

    indices.insert(indices.begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));

    // We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
    // about to go on, these values aren't ever going to change because the vertices don't move around in the array.
    // We'll need these values later on to fix the singularities that show up at the poles.
    const uint16_t northPoleIndex = 0;
    const uint16_t southPoleIndex = 5;

    for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
    {
        assert(indices.size() % 3 == 0); // sanity

        // We use this to keep track of which edges have already been subdivided.
        EdgeSubdivisionMap subdividedEdges;

        // The new index collection after subdivision.
        IndexCollectionColor newIndices;

        const size_t triangleCount = indices.size() / 3;
        for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
        {
            // For each edge on this triangle, create a new vertex in the middle of that edge.
            // The winding order of the triangles we output are the same as the winding order of the inputs.

            // Indices of the vertices making up this triangle
            uint16_t iv0 = indices[iTriangle * 3 + 0];
            uint16_t iv1 = indices[iTriangle * 3 + 1];
            uint16_t iv2 = indices[iTriangle * 3 + 2];

            // Get the new vertices
            XMFLOAT3 v01; // vertex on the midpoint of v0 and v1
            XMFLOAT3 v12; // ditto v1 and v2
            XMFLOAT3 v20; // ditto v2 and v0
            uint16_t iv01; // index of v01
            uint16_t iv12; // index of v12
            uint16_t iv20; // index of v20

            // Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
            auto divideEdge = [&](uint16_t i0, uint16_t i1, XMFLOAT3& outVertex, uint16_t& outIndex)
            {
                const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

                // Check to see if we've already generated this vertex
                auto it = subdividedEdges.find(edge);
                if (it != subdividedEdges.end())
                {
                    // We've already generated this vertex before
                    outIndex = it->second; // the index of this vertex
                    outVertex = vertexPositions[outIndex]; // and the vertex itself
                }
                else
                {
                    // Haven't generated this vertex before: so add it now

                    // outVertex = (vertices[i0] + vertices[i1]) / 2
                    XMStoreFloat3(
                        &outVertex,
                        XMVectorScale(
                            XMVectorAdd(XMLoadFloat3(&vertexPositions[i0]), XMLoadFloat3(&vertexPositions[i1])),
                            0.5f
                        )
                    );

                    outIndex = static_cast<uint16_t>(vertexPositions.size());
                    CheckIndexOverflowHot3dx(outIndex);
                    vertexPositions.push_back(outVertex);

                    // Now add it to the map.
                    auto entry = std::make_pair(edge, outIndex);
                    subdividedEdges.insert(entry);
                }
            };

            // Add/get new vertices and their indices
            divideEdge(iv0, iv1, v01, iv01);
            divideEdge(iv1, iv2, v12, iv12);
            divideEdge(iv0, iv2, v20, iv20);

            // Add the new indices. We have four new triangles from our original one:
            //        v0
            //        o
            //       /a\
            //  v20 o---o v01
            //     /b\c/d\
            // v2 o---o---o v1
            //       v12
            const uint16_t indicesToAdd[] =
            {
                 iv0, iv01, iv20, // a
                iv20, iv12,  iv2, // b
                iv20, iv01, iv12, // c
                iv01,  iv1, iv12, // d
            };
            newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
        }

        indices = std::move(newIndices);
    }

    // Now that we've completed subdivision, fill in the final vertex collection
    vertices.reserve(vertexPositions.size());
    for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it)
    {
        auto& vertexValue = *it;

        auto normal = XMVector3Normalize(XMLoadFloat3(&vertexValue));
        auto pos = XMVectorScale(normal, radius);

        XMFLOAT3 normalFloat3;
        XMStoreFloat3(&normalFloat3, normal);

        // calculate texture coordinates for this vertex
        float longitude = atan2(normalFloat3.x, -normalFloat3.z);
        float latitude = acos(normalFloat3.y);

        float u = longitude / XM_2PI + 0.5f;
        float v = latitude / XM_PI;

        auto texcoord = XMVectorSet(1.0f - u, v, 0.0f, 0.0f);

        FXMVECTOR t = XMVectorScale(normal, radius);
        float  x = XMVectorGetX(t);
        float  y = XMVectorGetY(t);
        float  z = XMVectorGetZ(t);

        XMVECTOR incident = DirectX::XMVECTOR(XMVectorSet(x, y, z, 0.0f));
        float a = XMVectorGetX(normal) * cosf(x);
        float b = XMVectorGetY(normal) * sinf(y);
        XMVECTOR t2 = { a, b, XMVectorGetZ(normal) * tanf(z) };
        XMVECTOR tan = XMVector3Cross(incident, normal);

        Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt = 
        Hot3dxRotoDraw::VertexPositionNormalTextureTangent(
            XMFLOAT3(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)), 
            XMFLOAT3(normalFloat3.x, normalFloat3.y, normalFloat3.z), 
            XMFLOAT2(1.0f - u, v), 
            XMFLOAT4(XMVectorGetX(tan), XMVectorGetY(tan), XMVectorGetZ(tan), XMVectorGetW(tan))
        );
        vertices.push_back(vpntt);

    }

    // There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
    // a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
    // occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
    // right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
    // 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
    // 
    // Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
    // completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
    // y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
    // need to duplicate our vertices - and provide the correct texture coordinates.
    size_t preFixupVertexCount = vertices.size();
    for (size_t i = 0; i < preFixupVertexCount; ++i)
    {
        // This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
        bool isOnPrimeMeridian = XMVector2NearEqual(
            XMVectorSet(vertices[i].position.x, vertices[i].position.x, 0.0f, 0.0f),
            XMVectorZero(),
            XMVectorSplatEpsilon());

        if (isOnPrimeMeridian)
        {
            size_t newIndex = vertices.size(); // the index of this vertex that we're about to add
            CheckIndexOverflowHot3dx(newIndex);

            // copy this vertex, correct the texture coordinate, and add the vertex
            
            Hot3dxRotoDraw::VertexPositionNormalTextureTangent v = vertices[i];
            //v.textureCoordinate.x = 1.0f;
            vertices.push_back(v);

            // Now find all the triangles which contain this vertex and update them if necessary
            for (size_t j = 0; j < indices.size(); j += 3)
            {
                uint16_t* triIndex0 = &indices[j + 0];
                uint16_t* triIndex1 = &indices[j + 1];
                uint16_t* triIndex2 = &indices[j + 2];

                if (*triIndex0 == i)
                {
                    // nothing; just keep going
                }
                else if (*triIndex1 == i)
                {
                    std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
                }
                else if (*triIndex2 == i)
                {
                    std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
                }
                else
                {
                    // this triangle doesn't use the vertex we're interested in
                    continue;
                }

                // If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
                assert(*triIndex0 == i);
                assert(*triIndex1 != i && *triIndex2 != i); // assume no degenerate triangles

                const Hot3dxRotoDraw::VertexPositionNormalTextureTangent& v0 = vertices[*triIndex0];
                const Hot3dxRotoDraw::VertexPositionNormalTextureTangent& v1 = vertices[*triIndex1];
                const Hot3dxRotoDraw::VertexPositionNormalTextureTangent& v2 = vertices[*triIndex2];

                // check the other two vertices to see if we might need to fix this triangle

                if (abs(v0.position.x - v1.position.x) > 0.5f ||
                    abs(v0.position.x - v2.position.x) > 0.5f)
                {
                    // yep; replace the specified index to point to the new, corrected vertex
                    *triIndex0 = static_cast<uint16_t>(newIndex);
                }
            }
        }
    }

    // And one last fix we need to do: the poles. A common use-case of a sphere mesh is to map a rectangular texture onto
    // it. If that happens, then the poles become singularities which map the entire top and bottom rows of the texture
    // onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
    // spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
    // poles, but reduce stretching.
    auto fixPole = [&](size_t poleIndex)
    {
        auto& poleVertex = vertices[poleIndex];
        bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            // These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
            // entry in the index array which represents the pole index, and the other two pointers point to the other
            // two indices making up this triangle.
            uint16_t* pPoleIndex;
            uint16_t* pOtherIndex0;
            uint16_t* pOtherIndex1;
            if (indices[i + 0] == poleIndex)
            {
                pPoleIndex = &indices[i + 0];
                pOtherIndex0 = &indices[i + 1];
                pOtherIndex1 = &indices[i + 2];
            }
            else if (indices[i + 1] == poleIndex)
            {
                pPoleIndex = &indices[i + 1];
                pOtherIndex0 = &indices[i + 2];
                pOtherIndex1 = &indices[i + 0];
            }
            else if (indices[i + 2] == poleIndex)
            {
                pPoleIndex = &indices[i + 2];
                pOtherIndex0 = &indices[i + 0];
                pOtherIndex1 = &indices[i + 1];
            }
            else
            {
                continue;
            }

            const auto& otherVertex0 = vertices[*pOtherIndex0];
            const auto& otherVertex1 = vertices[*pOtherIndex1];

            // Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
            Hot3dxRotoDraw::VertexPositionNormalTextureTangent newPoleVertex = poleVertex;
            newPoleVertex.position.x = (otherVertex0.position.x + otherVertex1.position.x) / 2;
            newPoleVertex.position.y = poleVertex.position.y;

            if (!overwrittenPoleVertex)
            {
                vertices[poleIndex] = newPoleVertex;
                overwrittenPoleVertex = true;
            }
            else
            {
                CheckIndexOverflowHot3dx(vertices.size());

                *pPoleIndex = static_cast<uint16_t>(vertices.size());
                vertices.push_back(newPoleVertex);
            }
        }
    };
    fixPole(northPoleIndex);
    fixPole(southPoleIndex);
    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

    void ComputeCylinderTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        if (tessellation < 3)
            throw std::out_of_range("tesselation parameter out of range");

        height /= 2;

        XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

        float radius = diameter / 2;
        size_t stride = tessellation + 1;

        // Create a ring of triangles around the outside of the cylinder.
        for (size_t i = 0; i <= tessellation; i++)
        {
            XMVECTOR normal = GetCircleVectorColor(i, tessellation);

            XMVECTOR sideOffset = XMVectorScale(normal, radius);

            float u = float(i) / tessellation;

            XMVECTOR textureCoordinate = XMLoadFloat(&u);

            XMVECTOR t = XMVectorAdd(sideOffset, topOffset);

            Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt = 
                Hot3dxRotoDraw::VertexPositionNormalTextureTangent(XMFLOAT3(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t)), 
                    XMFLOAT3(XMVectorGetX(normal), XMVectorGetY(normal), XMVectorGetZ(normal)),
                    XMFLOAT2(XMVectorGetX(textureCoordinate), XMVectorGetY(textureCoordinate)),
                    XMFLOAT4()
                );
            vertices.push_back(vpntt);
            t = XMVectorSubtract(sideOffset, topOffset);
            Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt1 =
                Hot3dxRotoDraw::VertexPositionNormalTextureTangent(XMFLOAT3(XMVectorGetX(t), XMVectorGetY(t), XMVectorGetZ(t)), 
                    XMFLOAT3(XMVectorGetX(normal), XMVectorGetY(normal), XMVectorGetZ(normal)), 
                    XMFLOAT2(XMVectorGetX(textureCoordinate), XMVectorGetY(textureCoordinate)),
                    XMFLOAT4()
                );
            vertices.push_back(vpntt1);

            index_push_backHot3dx(indices, i * 2);
            index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
            index_push_backHot3dx(indices, i * 2 + 1);

            index_push_backHot3dx(indices, i * 2 + 1);
            index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
            index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
        }

        // Create flat triangle fan caps to seal the top and bottom.
        DirectX::DXTKXAML12::CreateCylinderCapPositionNormalTextureTangent(vertices, indices, tessellation, height, radius, true);
        DirectX::DXTKXAML12::CreateCylinderCapPositionNormalTextureTangent(vertices, indices, tessellation, height, radius, false);

        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }

    void ComputeConeTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        if (tessellation < 3)
            throw std::out_of_range("tesselation parameter out of range");

        height /= 2;

        XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

        float radius = diameter / 2;
        size_t stride = tessellation + 1;

        // Create a ring of triangles around the outside of the cone.
        for (size_t i = 0; i <= tessellation; i++)
        {
            XMVECTOR circlevec = GetCircleVectorColor(i, tessellation);

            XMVECTOR sideOffset = XMVectorScale(circlevec, radius);

            float u = float(i) / tessellation;

            XMVECTOR textureCoordinate = XMLoadFloat(&u);

            XMVECTOR pt = XMVectorSubtract(sideOffset, topOffset);

            XMVECTOR normal = XMVector3Cross(
                GetCircleTangentColor(i, tessellation),
                XMVectorSubtract(topOffset, pt));
            normal = XMVector3Normalize(normal);

            // Duplicate the top vertex for distinct normals
            Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt =
                Hot3dxRotoDraw::VertexPositionNormalTextureTangent(topOffset, normal, textureCoordinate, GetCircleTangentColor(i, tessellation));
            vertices.push_back(vpntt);
            Hot3dxRotoDraw::VertexPositionNormalTextureTangent vpntt1 =
                Hot3dxRotoDraw::VertexPositionNormalTextureTangent(pt, normal, textureCoordinate, GetCircleTangentColor(i, tessellation));
            vertices.push_back(vpntt1);

            index_push_backHot3dx(indices, i * 2);
            index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
            index_push_backHot3dx(indices, (i * 2 + 1) % (stride * 2));
        }

        // Create flat triangle fan caps to seal the bottom.
        DirectX::DXTKXAML12::CreateCylinderCapPositionNormalTextureTangent(vertices, indices, tessellation, height, radius, true); (vertices, indices, tessellation, height, radius, false);

        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }

    void ComputeTorusTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        if (tessellation < 3)
            throw std::out_of_range("tesselation parameter out of range");

        size_t stride = tessellation + 1;

        // First we loop around the main ring of the torus.
        for (size_t i = 0; i <= tessellation; i++)
        {
            float u = float(i) / tessellation;

            float outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

            // Create a transform matrix that will align geometry to
            // slice perpendicularly though the current ring position.
            XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

            // Now we loop along the other axis, around the side of the tube.
            for (size_t j = 0; j <= tessellation; j++)
            {
                float v = 1 - float(j) / tessellation;

                float innerAngle = j * XM_2PI / tessellation + XM_PI;
                float dx, dy;

                XMScalarSinCos(&dy, &dx, innerAngle);

                // Create a vertex.
                XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
                XMVECTOR position = XMVectorScale(normal, thickness / 2);
                XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

                position = XMVector3Transform(position, transform);
                normal = XMVector3TransformNormal(normal, transform);

                
                vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinate, GetCircleTangentColor(j, tessellation)));

                // And create indices for two triangles.
                size_t nextI = (i + 1) % stride;
                size_t nextJ = (j + 1) % stride;

                index_push_backHot3dx(indices, i * stride + j);
                index_push_backHot3dx(indices, i * stride + nextJ);
                index_push_backHot3dx(indices, nextI * stride + j);

                index_push_backHot3dx(indices, i * stride + nextJ);
                index_push_backHot3dx(indices, nextI * stride + nextJ);
                index_push_backHot3dx(indices, nextI * stride + j);
            }
        }

        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }
 }

    void ComputeTetrahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        static const XMVECTORF32 verts[4] =
        {
            { { {              0.f,          0.f,        1.f, 0 } } },
            { { {  2.f * SQRT2 / 3.f,          0.f, -1.f / 3.f, 0 } } },
            { { {     -SQRT2 / 3.f,  SQRT6 / 3.f, -1.f / 3.f, 0 } } },
            { { {     -SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f, 0 } } }
        };

        static const uint32_t faces[4 * 3] =
        {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            1, 3, 2
        };

        for (size_t j = 0; j < _countof(faces); j += 3)
        {
            uint32_t v0 = faces[j];
            uint32_t v1 = faces[j + 1];
            uint32_t v2 = faces[j + 2];

            XMVECTOR normal = XMVector3Cross(
                XMVectorSubtract(verts[v1].v, verts[v0].v),
                XMVectorSubtract(verts[v2].v, verts[v0].v));
            normal = XMVector3Normalize(normal);

            size_t base = vertices.size();
            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 1);
            index_push_backHot3dx(indices, base + 2);

            // Duplicate vertices to use face normals
            XMVECTOR position = XMVectorScale(verts[v0], size);
            XMVECTOR tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[0], tan));


            position = XMVectorScale(verts[v1], size);
            tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[1], tan));


            position = XMVectorScale(verts[v2], size);
            tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[2], tan));

        }

        // Built LH above
        if (rhcoords)
            ReverseWindingHot3dx(indices, vertices);

        assert(vertices.size() == 4 * 3);
        assert(indices.size() == 4 * 3);

    }
    void ComputeOctahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        static const XMVECTORF32 verts[6] =
        {
            { { {  1,  0,  0, 0 } } },
            { { { -1,  0,  0, 0 } } },
            { { {  0,  1,  0, 0 } } },
            { { {  0, -1,  0, 0 } } },
            { { {  0,  0,  1, 0 } } },
            { { {  0,  0, -1, 0 } } }
        };

        static const uint32_t faces[8 * 3] =
        {
            4, 0, 2,
            4, 2, 1,
            4, 1, 3,
            4, 3, 0,
            5, 2, 0,
            5, 1, 2,
            5, 3, 1,
            5, 0, 3
        };

        for (size_t j = 0; j < _countof(faces); j += 3)
        {
            uint32_t v0 = faces[j];
            uint32_t v1 = faces[j + 1];
            uint32_t v2 = faces[j + 2];

            XMVECTOR normal = XMVector3Cross(
                XMVectorSubtract(verts[v1].v, verts[v0].v),
                XMVectorSubtract(verts[v2].v, verts[v0].v));
            normal = XMVector3Normalize(normal);

            size_t base = vertices.size();
            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 1);
            index_push_backHot3dx(indices, base + 2);

            // Duplicate vertices to use face normals
            XMVECTOR position = XMVectorScale(verts[v0], size);
            XMVECTOR tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[v0], tan));


            position = XMVectorScale(verts[v1], size);
            tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[v1], tan));


            position = XMVectorScale(verts[v2], size);
            tan = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, verts[v2], tan));

        }

        // Built LH above
        if (rhcoords)
            ReverseWindingHot3dx(indices, vertices);

        assert(vertices.size() == 8 * 3);
        assert(indices.size() == 8 * 3);
    }

    void ComputeDodecahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        static const float a = 1.f / SQRT3;
        static const float b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
        static const float c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

        static const XMVECTORF32 verts[20] =
        {
            { { {  a,  a,  a, 0 } } },
            { { {  a,  a, -a, 0 } } },
            { { {  a, -a,  a, 0 } } },
            { { {  a, -a, -a, 0 } } },
            { { { -a,  a,  a, 0 } } },
            { { { -a,  a, -a, 0 } } },
            { { { -a, -a,  a, 0 } } },
            { { { -a, -a, -a, 0 } } },
            { { {  b,  c,  0, 0 } } },
            { { { -b,  c,  0, 0 } } },
            { { {  b, -c,  0, 0 } } },
            { { { -b, -c,  0, 0 } } },
            { { {  c,  0,  b, 0 } } },
            { { {  c,  0, -b, 0 } } },
            { { { -c,  0,  b, 0 } } },
            { { { -c,  0, -b, 0 } } },
            { { {  0,  b,  c, 0 } } },
            { { {  0, -b,  c, 0 } } },
            { { {  0,  b, -c, 0 } } },
            { { {  0, -b, -c, 0 } } }
        };

        static const uint32_t faces[12 * 5] =
        {
            0, 8, 9, 4, 16,
            0, 16, 17, 2, 12,
            12, 2, 10, 3, 13,
            9, 5, 15, 14, 4,
            3, 19, 18, 1, 13,
            7, 11, 6, 14, 15,
            0, 12, 13, 1, 8,
            8, 1, 18, 5, 9,
            16, 4, 14, 6, 17,
            6, 11, 10, 2, 17,
            7, 15, 5, 18, 19,
            7, 19, 3, 10, 11
        };

        static const XMVECTORF32 textureCoordinates[5] =
        {
            { { {  0.654508f, 0.0244717f, 0, 0 } } },
            { { { 0.0954915f,  0.206107f, 0, 0 } } },
            { { { 0.0954915f,  0.793893f, 0, 0 } } },
            { { {  0.654508f,  0.975528f, 0, 0 } } },
            { { {        1.f,       0.5f, 0, 0 } } }
        };

        static const uint32_t textureIndex[12][5] =
        {
            { 0, 1, 2, 3, 4 },
            { 2, 3, 4, 0, 1 },
            { 4, 0, 1, 2, 3 },
            { 1, 2, 3, 4, 0 },
            { 2, 3, 4, 0, 1 },
            { 0, 1, 2, 3, 4 },
            { 1, 2, 3, 4, 0 },
            { 4, 0, 1, 2, 3 },
            { 4, 0, 1, 2, 3 },
            { 1, 2, 3, 4, 0 },
            { 0, 1, 2, 3, 4 },
            { 2, 3, 4, 0, 1 }
        };

        size_t t = 0;
        for (size_t j = 0; j < _countof(faces); j += 5, ++t)
        {
            uint32_t v0 = faces[j];
            uint32_t v1 = faces[j + 1];
            uint32_t v2 = faces[j + 2];
            uint32_t v3 = faces[j + 3];
            uint32_t v4 = faces[j + 4];

            XMVECTOR normal = XMVector3Cross(
                XMVectorSubtract(verts[v1].v, verts[v0].v),
                XMVectorSubtract(verts[v2].v, verts[v0].v));
            normal = XMVector3Normalize(normal);

            size_t base = vertices.size();

            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 1);
            index_push_backHot3dx(indices, base + 2);

            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 2);
            index_push_backHot3dx(indices, base + 3);

            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 3);
            index_push_backHot3dx(indices, base + 4);

            // Duplicate vertices to use face normals
            XMVECTOR position = XMVectorScale(verts[v0], size);
            XMVECTOR tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[0], tangent));


            position = XMVectorScale(verts[v1], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[1], tangent));


            position = XMVectorScale(verts[v2], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[2], tangent));


            position = XMVectorScale(verts[v3], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[3], tangent));


            position = XMVectorScale(verts[v4], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[4], tangent));

        }

        // Built LH above
        if (rhcoords)
            ReverseWindingHot3dx(indices, vertices);

        assert(vertices.size() == 12 * 5);
        assert(indices.size() == 12 * 3 * 3);
    }

    void ComputeIcosahedronTangent(VertexCollectionPositionNormalTextureTangent& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

        static const float  t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
        static const float t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

        static const XMVECTORF32 verts[12] =
        {
            { { {    t / t2,  1.f / t2,       0, 0 } } },
            { { {   -t / t2,  1.f / t2,       0, 0 } } },
            { { {    t / t2, -1.f / t2,       0, 0 } } },
            { { {   -t / t2, -1.f / t2,       0, 0 } } },
            { { {  1.f / t2,       0,    t / t2, 0 } } },
            { { {  1.f / t2,       0,   -t / t2, 0 } } },
            { { { -1.f / t2,       0,    t / t2, 0 } } },
            { { { -1.f / t2,       0,   -t / t2, 0 } } },
            { { {       0,    t / t2,  1.f / t2, 0 }  } },
            { { {       0,   -t / t2,  1.f / t2, 0 } } },
            { { {       0,    t / t2, -1.f / t2, 0 } } },
            { { {       0,   -t / t2, -1.f / t2, 0 } } }
        };

        static const XMVECTORF32 textureCoordinates[3] =
        {
            { { {  0.654508f, 0.0244717f, 0, 0 } } },
            { { { 0.0954915f,  0.793893f, 0, 0 } } },
            { { {        1.f,       0.5f, 0, 0 } } }
        };

        static const uint32_t faces[20 * 3] =
        {
            0, 8, 4,
            0, 5, 10,
            2, 4, 9,
            2, 11, 5,
            1, 6, 8,
            1, 10, 7,
            3, 9, 6,
            3, 7, 11,
            0, 10, 8,
            1, 8, 10,
            2, 9, 11,
            3, 11, 9,
            4, 2, 0,
            5, 0, 2,
            6, 1, 3,
            7, 3, 1,
            8, 6, 4,
            9, 4, 6,
            10, 5, 7,
            11, 7, 5
        };

        for (size_t j = 0; j < _countof(faces); j += 3)
        {
            uint32_t v0 = faces[j];
            uint32_t v1 = faces[j + 1];
            uint32_t v2 = faces[j + 2];

            XMVECTOR normal = XMVector3Cross(
                XMVectorSubtract(verts[v1].v, verts[v0].v),
                XMVectorSubtract(verts[v2].v, verts[v0].v));
            normal = XMVector3Normalize(normal);

            size_t base = vertices.size();
            index_push_backHot3dx(indices, base);
            index_push_backHot3dx(indices, base + 1);
            index_push_backHot3dx(indices, base + 2);

            // Duplicate vertices to use face normals
            XMVECTOR position = XMVectorScale(verts[v0], size);
            XMVECTOR tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[0], tangent));


            position = XMVectorScale(verts[v1], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[1], tangent));


            position = XMVectorScale(verts[v2], size);
            tangent = XMVector3Cross(normal, position);
            vertices.push_back(Hot3dxRotoDraw::VertexPositionNormalTextureTangent(position, normal, textureCoordinates[2], tangent));

        }

        // Built LH above
        if (rhcoords)
            ReverseWindingHot3dx(indices, vertices);

        assert(vertices.size() == 20 * 3);
        assert(indices.size() == 20 * 3);
        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }

    void ComputeBoxDualTexture(VertexCollectionDualTexture & vertices, IndexCollectionColor & indices, const XMFLOAT3 & size, bool rhcoords)
    {
        vertices.clear();
        indices.clear();

        // A box has six faces, each one pointing in a different direction.
        const int FaceCount = 6;

        XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

        static const XMVECTORF32 faceNormals[FaceCount] =
        {
            { { {  0,  0,  1, 0 } } },
            { { {  0,  0, -1, 0 } } },
            { { {  1,  0,  0, 0 } } },
            { { { -1,  0,  0, 0 } } },
            { { {  0,  1,  0, 0 } } },
            { { {  0, -1,  0, 0 } } }
        };

        static const XMVECTORF32 textureCoordinates[4] =
        {
            { { { 1, 0, 0, 0 } } },
            { { { 1, 1, 0, 0 } } },
            { { { 0, 1, 0, 0 } } },
            { { { 0, 0, 0, 0 } } }
        };

        XMVECTOR tsize = XMLoadFloat3(&size);
        tsize = XMVectorDivide(tsize, g_XMTwo);

        // Create each face in turn.
        for (int i = 0; i < FaceCount; i++)
        {
            XMVECTOR normal = faceNormals[i];

            // Get two vectors perpendicular both to the face normal and to each other.
            XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

            XMVECTOR side1 = XMVector3Cross(normal, basis);
            XMVECTOR side2 = XMVector3Cross(normal, side1);

            // Six indices (two triangles) per face.
            size_t vbase = vertices.size();
            index_push_backHot3dx(indices, vbase + 0);
            index_push_backHot3dx(indices, vbase + 1);
            index_push_backHot3dx(indices, vbase + 2);

            index_push_backHot3dx(indices, vbase + 0);
            index_push_backHot3dx(indices, vbase + 2);
            index_push_backHot3dx(indices, vbase + 3);


            // Four vertices per face.
            // (normal - side1 - side2) * tsize // normal // t0
            XMVECTOR t = DirectX::XMVectorMultiply(DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(normal, side1), side2), tsize);

            vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(DirectX::XMVectorSet(DirectX::XMVectorGetX(t), DirectX::XMVectorGetY(t), DirectX::XMVectorGetZ(t), DirectX::XMVectorGetW(t)), textureCoordinates[0], textureCoordinates[0]));

            // (normal - side1 + side2) * tsize // normal // t1
            t = XMVectorMultiply(XMVectorAdd(XMVectorSubtract(normal, side1), side2), tsize);
            vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(DirectX::XMVectorSet(XMVectorGetX(t), DirectX::XMVectorGetY(t), DirectX::XMVectorGetZ(t), DirectX::XMVectorGetW(t)), textureCoordinates[1], textureCoordinates[1]));

            // (normal + side1 + side2) * tsize // normal // t2
            t = XMVectorMultiply(XMVectorAdd(normal, XMVectorAdd(side1, side2)), tsize);
            vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(DirectX::XMVectorSet(DirectX::XMVectorGetX(t), DirectX::XMVectorGetY(t), DirectX::XMVectorGetZ(t), DirectX::XMVectorGetW(t)), textureCoordinates[2], textureCoordinates[2]));

            // (normal + side1 - side2) * tsize // normal // t3
            t = XMVectorMultiply(XMVectorSubtract(XMVectorAdd(normal, side1), side2), tsize);
            vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(DirectX::XMVectorSet(DirectX::XMVectorGetX(t), DirectX::XMVectorGetY(t), DirectX::XMVectorGetZ(t), DirectX::XMVectorGetW(t)), textureCoordinates[3], textureCoordinates[3]));
        }

        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }

    void ComputeSphereDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
    {
        vertices.clear();
        indices.clear();
        XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
        FXMVECTOR colorv = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
        if (tessellation < 3)
            throw std::out_of_range("tesselation parameter out of range");

        size_t verticalSegments = tessellation;
        size_t horizontalSegments = tessellation * 2;

        float radius = diameter / 2;

        // Create rings of vertices at progressively higher latitudes.
        for (size_t i = 0; i <= verticalSegments; i++)
        {
            float v = 1 - float(i) / verticalSegments;

            float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
            float dy, dxz;

            XMScalarSinCos(&dy, &dxz, latitude);

            // Create a single ring of vertices at this latitude.
            for (size_t j = 0; j <= horizontalSegments; j++)
            {
                float u = float(j) / horizontalSegments;

                float longitude = j * XM_2PI / horizontalSegments;
                float dx, dz;

                XMScalarSinCos(&dx, &dz, longitude);

                dx *= dxz;
                dz *= dxz;

                XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
                XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

                FXMVECTOR t = XMVectorScale(normal, radius);

                vertices.push_back(
                    DirectX::DXTKXAML12::VertexPositionDualTexture(XMFLOAT3(DirectX::XMVectorGetX(t), DirectX::XMVectorGetY(t), DirectX::XMVectorGetZ(t)), XMFLOAT2(u, v), XMFLOAT2(u, v))
                );
            }
        }

        // Fill the index buffer with triangles joining each pair of latitude rings.
        size_t stride = horizontalSegments + 1;

        for (size_t i = 0; i < verticalSegments; i++)
        {
            for (size_t j = 0; j <= horizontalSegments; j++)
            {
                size_t nextI = i + 1;
                size_t nextJ = (j + 1) % stride;

                index_push_backHot3dx(indices, i * stride + j);
                index_push_backHot3dx(indices, nextI * stride + j);
                index_push_backHot3dx(indices, i * stride + nextJ);

                index_push_backHot3dx(indices, i * stride + nextJ);
                index_push_backHot3dx(indices, nextI * stride + j);
                index_push_backHot3dx(indices, nextI * stride + nextJ);
            }
        }

        // Build RH above
        if (!rhcoords)
            ReverseWindingHot3dx(indices, vertices);
    }

void ComputeGeoSphereDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    // An undirected edge between two vertices, represented by a pair of indexes into a vertex array.
    // Becuse this edge is undirected, (a,b) is the same as (b,a).
    typedef std::pair<uint16_t, uint16_t> UndirectedEdge;

    // Makes an undirected edge. Rather than overloading comparison operators to give us the (a,b)==(b,a) property,
    // we'll just ensure that the larger of the two goes first. This'll simplify things greatly.
    auto makeUndirectedEdge = [](uint16_t a, uint16_t b)
    {
        return std::make_pair(max(a, b), min(a, b));
    };

    // Key: an edge
    // Value: the index of the vertex which lies midway between the two vertices pointed to by the key value
    // This map is used to avoid duplicating vertices when subdividing triangles along edges.
    typedef std::map<UndirectedEdge, uint16_t> EdgeSubdivisionMap;


    static const XMFLOAT3 OctahedronVertices[] =
    {
        // when looking down the negative z-axis (into the screen)
        XMFLOAT3(0,  1,  0), // 0 top
        XMFLOAT3(0,  0, -1), // 1 front
        XMFLOAT3(1,  0,  0), // 2 right
        XMFLOAT3(0,  0,  1), // 3 back
        XMFLOAT3(-1,  0,  0), // 4 left
        XMFLOAT3(0, -1,  0) // 5 bottom
    };
    static const uint16_t OctahedronIndices[] =
    {
        0, 1, 2, // top front-right face
        0, 2, 3, // top back-right face
        0, 3, 4, // top back-left face
        0, 4, 1, // top front-left face
        5, 1, 4, // bottom front-left face
        5, 4, 3, // bottom back-left face
        5, 3, 2, // bottom back-right face
        5, 2, 1 // bottom front-right face
    };

    const float radius = diameter / 2.0f;

    // Start with an octahedron; copy the data into the vertex/index collection.

    std::vector<XMFLOAT3> vertexPositions(std::begin(OctahedronVertices), std::end(OctahedronVertices));

    indices.insert(indices.begin(), std::begin(OctahedronIndices), std::end(OctahedronIndices));

    // We know these values by looking at the above index list for the octahedron. Despite the subdivisions that are
    // about to go on, these values aren't ever going to change because the vertices don't move around in the array.
    // We'll need these values later on to fix the singularities that show up at the poles.
    const uint16_t northPoleIndex = 0;
    const uint16_t southPoleIndex = 5;

    for (size_t iSubdivision = 0; iSubdivision < tessellation; ++iSubdivision)
    {
        assert(indices.size() % 3 == 0); // sanity

        // We use this to keep track of which edges have already been subdivided.
        EdgeSubdivisionMap subdividedEdges;

        // The new index collection after subdivision.
        IndexCollectionColor newIndices;

        const size_t triangleCount = indices.size() / 3;
        for (size_t iTriangle = 0; iTriangle < triangleCount; ++iTriangle)
        {
            // For each edge on this triangle, create a new vertex in the middle of that edge.
            // The winding order of the triangles we output are the same as the winding order of the inputs.

            // Indices of the vertices making up this triangle
            uint16_t iv0 = indices[iTriangle * 3 + 0];
            uint16_t iv1 = indices[iTriangle * 3 + 1];
            uint16_t iv2 = indices[iTriangle * 3 + 2];

            // Get the new vertices
            XMFLOAT3 v01; // vertex on the midpoint of v0 and v1
            XMFLOAT3 v12; // ditto v1 and v2
            XMFLOAT3 v20; // ditto v2 and v0
            uint16_t iv01; // index of v01
            uint16_t iv12; // index of v12
            uint16_t iv20; // index of v20

            // Function that, when given the index of two vertices, creates a new vertex at the midpoint of those vertices.
            auto divideEdge = [&](uint16_t i0, uint16_t i1, XMFLOAT3& outVertex, uint16_t& outIndex)
            {
                const UndirectedEdge edge = makeUndirectedEdge(i0, i1);

                // Check to see if we've already generated this vertex
                auto it = subdividedEdges.find(edge);
                if (it != subdividedEdges.end())
                {
                    // We've already generated this vertex before
                    outIndex = it->second; // the index of this vertex
                    outVertex = vertexPositions[outIndex]; // and the vertex itself
                }
                else
                {
                    // Haven't generated this vertex before: so add it now

                    // outVertex = (vertices[i0] + vertices[i1]) / 2
                    XMStoreFloat3(
                        &outVertex,
                        XMVectorScale(
                            XMVectorAdd(XMLoadFloat3(&vertexPositions[i0]), XMLoadFloat3(&vertexPositions[i1])),
                            0.5f
                        )
                    );

                    outIndex = static_cast<uint16_t>(vertexPositions.size());
                    CheckIndexOverflowHot3dx(outIndex);
                    vertexPositions.push_back(outVertex);

                    // Now add it to the map.
                    auto entry = std::make_pair(edge, outIndex);
                    subdividedEdges.insert(entry);
                }
            };

            // Add/get new vertices and their indices
            divideEdge(iv0, iv1, v01, iv01);
            divideEdge(iv1, iv2, v12, iv12);
            divideEdge(iv0, iv2, v20, iv20);

            // Add the new indices. We have four new triangles from our original one:
            //        v0
            //        o
            //       /a\
            //  v20 o---o v01
            //     /b\c/d\
            // v2 o---o---o v1
            //       v12
            const uint16_t indicesToAdd[] =
            {
                 iv0, iv01, iv20, // a
                iv20, iv12,  iv2, // b
                iv20, iv01, iv12, // c
                iv01,  iv1, iv12, // d
            };
            newIndices.insert(newIndices.end(), std::begin(indicesToAdd), std::end(indicesToAdd));
        }

        indices = std::move(newIndices);
    }

    // Now that we've completed subdivision, fill in the final vertex collection
    vertices.reserve(vertexPositions.size());
    for (auto it = vertexPositions.begin(); it != vertexPositions.end(); ++it)
    {
        auto& vertexValue = *it;

        auto normal = XMVector3Normalize(XMLoadFloat3(&vertexValue));
        auto pos = XMVectorScale(normal, radius);

        XMFLOAT3 normalFloat3;
        XMStoreFloat3(&normalFloat3, normal);

        // calculate texture coordinates for this vertex
        float longitude = atan2(normalFloat3.x, -normalFloat3.z);
        float latitude = acos(normalFloat3.y);

        float u = longitude / XM_2PI + 0.5f;
        float v = latitude / XM_PI;

        auto texcoord = XMVectorSet(1.0f - u, v, 0.0f, 0.0f);
        XMFLOAT2 tex = { 1.0f - u, v };
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(XMFLOAT3(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos)), tex, tex));
    }

    // There are a couple of fixes to do. One is a texture coordinate wraparound fixup. At some point, there will be
    // a set of triangles somewhere in the mesh with texture coordinates such that the wraparound across 0.0/1.0
    // occurs across that triangle. Eg. when the left hand side of the triangle has a U coordinate of 0.98 and the
    // right hand side has a U coordinate of 0.0. The intent is that such a triangle should render with a U of 0.98 to
    // 1.0, not 0.98 to 0.0. If we don't do this fixup, there will be a visible seam across one side of the sphere.
    // 
    // Luckily this is relatively easy to fix. There is a straight edge which runs down the prime meridian of the
    // completed sphere. If you imagine the vertices along that edge, they circumscribe a semicircular arc starting at
    // y=1 and ending at y=-1, and sweeping across the range of z=0 to z=1. x stays zero. It's along this edge that we
    // need to duplicate our vertices - and provide the correct texture coordinates.
    size_t preFixupVertexCount = vertices.size();
    for (size_t i = 0; i < preFixupVertexCount; ++i)
    {
        // This vertex is on the prime meridian if position.x and texcoord.u are both zero (allowing for small epsilon).
        bool isOnPrimeMeridian = XMVector2NearEqual(
            XMVectorSet(vertices[i].position.x, vertices[i].position.x, 0.0f, 0.0f),
            XMVectorZero(),
            XMVectorSplatEpsilon());

        if (isOnPrimeMeridian)
        {
            size_t newIndex = vertices.size(); // the index of this vertex that we're about to add
            CheckIndexOverflowHot3dx(newIndex);

            // copy this vertex, correct the texture coordinate, and add the vertex
            DirectX::DXTKXAML12::VertexPositionDualTexture v = vertices[i];
            //v.textureCoordinate.x = 1.0f;
            vertices.push_back(v);

            // Now find all the triangles which contain this vertex and update them if necessary
            for (size_t j = 0; j < indices.size(); j += 3)
            {
                uint16_t* triIndex0 = &indices[j + 0];
                uint16_t* triIndex1 = &indices[j + 1];
                uint16_t* triIndex2 = &indices[j + 2];

                if (*triIndex0 == i)
                {
                    // nothing; just keep going
                }
                else if (*triIndex1 == i)
                {
                    std::swap(triIndex0, triIndex1); // swap the pointers (not the values)
                }
                else if (*triIndex2 == i)
                {
                    std::swap(triIndex0, triIndex2); // swap the pointers (not the values)
                }
                else
                {
                    // this triangle doesn't use the vertex we're interested in
                    continue;
                }

                // If we got to this point then triIndex0 is the pointer to the index to the vertex we're looking at
                assert(*triIndex0 == i);
                assert(*triIndex1 != i && *triIndex2 != i); // assume no degenerate triangles

                const DirectX::DXTKXAML12::VertexPositionDualTexture& v0 = vertices[*triIndex0];
                const DirectX::DXTKXAML12::VertexPositionDualTexture& v1 = vertices[*triIndex1];
                const DirectX::DXTKXAML12::VertexPositionDualTexture& v2 = vertices[*triIndex2];

                // check the other two vertices to see if we might need to fix this triangle

                if (abs(v0.position.x - v1.position.x) > 0.5f ||
                    abs(v0.position.x - v2.position.x) > 0.5f)
                {
                    // yep; replace the specified index to point to the new, corrected vertex
                    *triIndex0 = static_cast<uint16_t>(newIndex);
                }
            }
        }
    }

    // And one last fix we need to do: the poles. A common use-case of a sphere mesh is to map a rectangular texture onto
    // it. If that happens, then the poles become singularities which map the entire top and bottom rows of the texture
    // onto a single point. In general there's no real way to do that right. But to match the behavior of non-geodesic
    // spheres, we need to duplicate the pole vertex for every triangle that uses it. This will introduce seams near the
    // poles, but reduce stretching.
    auto fixPole = [&](size_t poleIndex)
    {
        auto& poleVertex = vertices[poleIndex];
        bool overwrittenPoleVertex = false; // overwriting the original pole vertex saves us one vertex

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            // These pointers point to the three indices which make up this triangle. pPoleIndex is the pointer to the
            // entry in the index array which represents the pole index, and the other two pointers point to the other
            // two indices making up this triangle.
            uint16_t* pPoleIndex;
            uint16_t* pOtherIndex0;
            uint16_t* pOtherIndex1;
            if (indices[i + 0] == poleIndex)
            {
                pPoleIndex = &indices[i + 0];
                pOtherIndex0 = &indices[i + 1];
                pOtherIndex1 = &indices[i + 2];
            }
            else if (indices[i + 1] == poleIndex)
            {
                pPoleIndex = &indices[i + 1];
                pOtherIndex0 = &indices[i + 2];
                pOtherIndex1 = &indices[i + 0];
            }
            else if (indices[i + 2] == poleIndex)
            {
                pPoleIndex = &indices[i + 2];
                pOtherIndex0 = &indices[i + 0];
                pOtherIndex1 = &indices[i + 1];
            }
            else
            {
                continue;
            }

            const auto& otherVertex0 = vertices[*pOtherIndex0];
            const auto& otherVertex1 = vertices[*pOtherIndex1];

            // Calculate the texcoords for the new pole vertex, add it to the vertices and update the index
            DirectX::DXTKXAML12::VertexPositionDualTexture newPoleVertex = poleVertex;
            newPoleVertex.position.x = (otherVertex0.position.x + otherVertex1.position.x) / 2;
            newPoleVertex.position.y = poleVertex.position.y;

            if (!overwrittenPoleVertex)
            {
                vertices[poleIndex] = newPoleVertex;
                overwrittenPoleVertex = true;
            }
            else
            {
                CheckIndexOverflowHot3dx(vertices.size());

                *pPoleIndex = static_cast<uint16_t>(vertices.size());
                vertices.push_back(newPoleVertex);
            }
        }
    };
    fixPole(northPoleIndex);
    fixPole(southPoleIndex);
    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}
void ComputeCylinderDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float height, float diameter, size_t tessellation, bool rhcoords)
{
    
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    height /= 2;

    XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cylinder.
    for (size_t i = 0; i <= tessellation; i++)
    {
        XMVECTOR normal = GetCircleVectorColor(i, tessellation);

        XMVECTOR sideOffset = XMVectorScale(normal, radius);

        float u = float(i) / tessellation;

        XMVECTOR textureCoordinate = XMLoadFloat(&u);

        XMVECTOR t = XMVectorAdd(sideOffset, topOffset);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(t, textureCoordinate, textureCoordinate));
        t = XMVectorSubtract(sideOffset, topOffset);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(t, textureCoordinate, textureCoordinate));

        index_push_backHot3dx(indices, i * 2);
        index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
        index_push_backHot3dx(indices, i * 2 + 1);

        index_push_backHot3dx(indices, i * 2 + 1);
        index_push_backHot3dx(indices, (i * 2 + 2) % (stride * 2));
        index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the top and bottom.
    DirectX::DXTKXAML12::CreateCylinderCapDualTexture(vertices, indices, tessellation, height, radius, true);
    DirectX::DXTKXAML12::CreateCylinderCapDualTexture(vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
    
}

void ComputeConeDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, float height, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    height /= 2;

    XMVECTOR topOffset = XMVectorScale(g_XMIdentityR1, height);

    float radius = diameter / 2;
    size_t stride = tessellation + 1;

    // Create a ring of triangles around the outside of the cone.
    for (size_t i = 0; i <= tessellation; i++)
    {
        XMVECTOR circlevec = GetCircleVectorColor(i, tessellation);

        XMVECTOR sideOffset = XMVectorScale(circlevec, radius);

        float u = float(i) / tessellation;

        XMVECTOR textureCoordinate = XMLoadFloat(&u);

        XMVECTOR pt = XMVectorSubtract(sideOffset, topOffset);

        XMVECTOR normal = XMVector3Cross(
            GetCircleTangentColor(i, tessellation),
            XMVectorSubtract(topOffset, pt));
        normal = XMVector3Normalize(normal);

        // Duplicate the top vertex for distinct normals
        DirectX::DXTKXAML12::VertexPositionDualTexture vpntt =
            DirectX::DXTKXAML12::VertexPositionDualTexture(topOffset, textureCoordinate, textureCoordinate);
        vertices.push_back(vpntt);
        DirectX::DXTKXAML12::VertexPositionDualTexture vpntt1 =
            DirectX::DXTKXAML12::VertexPositionDualTexture(pt, textureCoordinate, textureCoordinate);
        vertices.push_back(vpntt1);

        index_push_backHot3dx(indices, i * 2);
        index_push_backHot3dx(indices, (i * 2 + 3) % (stride * 2));
        index_push_backHot3dx(indices, (i * 2 + 1) % (stride * 2));
    }

    // Create flat triangle fan caps to seal the bottom.
    DirectX::DXTKXAML12::CreateCylinderCapDualTexture(vertices, indices, tessellation, height, radius, true); (vertices, indices, tessellation, height, radius, false);

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
}

void ComputeTorusDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float diameter, float thickness, size_t tessellation, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    if (tessellation < 3)
        throw std::out_of_range("tesselation parameter out of range");

    size_t stride = tessellation + 1;

    // First we loop around the main ring of the torus.
    for (size_t i = 0; i <= tessellation; i++)
    {
        float u = float(i) / tessellation;

        float outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

        // Create a transform matrix that will align geometry to
        // slice perpendicularly though the current ring position.
        XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

        // Now we loop along the other axis, around the side of the tube.
        for (size_t j = 0; j <= tessellation; j++)
        {
            float v = 1 - float(j) / tessellation;

            float innerAngle = j * XM_2PI / tessellation + XM_PI;
            float dx, dy;

            XMScalarSinCos(&dy, &dx, innerAngle);

            // Create a vertex.
            XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
            XMVECTOR position = XMVectorScale(normal, thickness / 2);
            XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

            position = XMVector3Transform(position, transform);
            normal = XMVector3TransformNormal(normal, transform);


            vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position,textureCoordinate, textureCoordinate));

            // And create indices for two triangles.
            size_t nextI = (i + 1) % stride;
            size_t nextJ = (j + 1) % stride;

            index_push_backHot3dx(indices, i * stride + j);
            index_push_backHot3dx(indices, i * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + j);

            index_push_backHot3dx(indices, i * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + nextJ);
            index_push_backHot3dx(indices, nextI * stride + j);
        }
    }

    // Build RH above
    if (!rhcoords)
        ReverseWindingHot3dx(indices, vertices);
   }


void ComputeTetrahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const XMVECTORF32 verts[4] =
    {
        { { {              0.f,          0.f,        1.f, 0 } } },
        { { {  2.f * SQRT2 / 3.f,          0.f, -1.f / 3.f, 0 } } },
        { { {     -SQRT2 / 3.f,  SQRT6 / 3.f, -1.f / 3.f, 0 } } },
        { { {     -SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f, 0 } } }
    };

    static const uint32_t faces[4 * 3] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 1,
        1, 3, 2
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        XMVECTOR tan = XMVector3Cross(normal, position);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, verts[0], verts[0]));


        position = XMVectorScale(verts[v1], size);
        tan = XMVector3Cross(normal, position); 
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, verts[1], verts[1]));


        position = XMVectorScale(verts[v2], size);
        tan = XMVector3Cross(normal, position); 
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, verts[2], verts[2]));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 4 * 3);
    assert(indices.size() == 4 * 3);

}

void ComputeOctahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, size_t tessellation, bool rhcoords, bool isTop)
{
    
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const XMVECTORF32 verts[6] =
    {
        { { {  1,  0,  0, 0 } } },
        { { { -1,  0,  0, 0 } } },
        { { {  0,  1,  0, 0 } } },
        { { {  0, -1,  0, 0 } } },
        { { {  0,  0,  1, 0 } } },
        { { {  0,  0, -1, 0 } } }
    };

    static const uint32_t faces[8 * 3] =
    {
        4, 0, 2,
        4, 2, 1,
        4, 1, 3,
        4, 3, 0,
        5, 2, 0,
        5, 1, 2,
        5, 3, 1,
        5, 0, 3
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);


        // Which end of the cylinder is this?
        XMVECTOR normal1 = g_XMIdentityR1;
        XMVECTOR textureScale = g_XMNegativeOneHalf;

        if (!isTop)
        {
            normal = XMVectorNegate(normal1);
            textureScale = XMVectorMultiply(textureScale, g_XMNegateX);
        }

        XMVECTOR circleVector = GetCircleVectorColor(j, tessellation);

            
        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinate, textureCoordinate));


        position = XMVectorScale(verts[v1], size);
        textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinate, textureCoordinate));


        position = XMVectorScale(verts[v2], size);
        textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinate, textureCoordinate));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 8 * 3);
    assert(indices.size() == 8 * 3);
}

void ComputeDodecahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{

    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const float a = 1.f / SQRT3;
    static const float b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
    static const float c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

    static const XMVECTORF32 verts[20] =
    {
        { { {  a,  a,  a, 0 } } },
        { { {  a,  a, -a, 0 } } },
        { { {  a, -a,  a, 0 } } },
        { { {  a, -a, -a, 0 } } },
        { { { -a,  a,  a, 0 } } },
        { { { -a,  a, -a, 0 } } },
        { { { -a, -a,  a, 0 } } },
        { { { -a, -a, -a, 0 } } },
        { { {  b,  c,  0, 0 } } },
        { { { -b,  c,  0, 0 } } },
        { { {  b, -c,  0, 0 } } },
        { { { -b, -c,  0, 0 } } },
        { { {  c,  0,  b, 0 } } },
        { { {  c,  0, -b, 0 } } },
        { { { -c,  0,  b, 0 } } },
        { { { -c,  0, -b, 0 } } },
        { { {  0,  b,  c, 0 } } },
        { { {  0, -b,  c, 0 } } },
        { { {  0,  b, -c, 0 } } },
        { { {  0, -b, -c, 0 } } }
    };

    static const uint32_t faces[12 * 5] =
    {
        0, 8, 9, 4, 16,
        0, 16, 17, 2, 12,
        12, 2, 10, 3, 13,
        9, 5, 15, 14, 4,
        3, 19, 18, 1, 13,
        7, 11, 6, 14, 15,
        0, 12, 13, 1, 8,
        8, 1, 18, 5, 9,
        16, 4, 14, 6, 17,
        6, 11, 10, 2, 17,
        7, 15, 5, 18, 19,
        7, 19, 3, 10, 11
    };

    static const XMVECTORF32 textureCoordinates[5] =
    {
        { { {  0.654508f, 0.0244717f, 0, 0 } } },
        { { { 0.0954915f,  0.206107f, 0, 0 } } },
        { { { 0.0954915f,  0.793893f, 0, 0 } } },
        { { {  0.654508f,  0.975528f, 0, 0 } } },
        { { {        1.f,       0.5f, 0, 0 } } }
    };

    static const uint32_t textureIndex[12][5] =
    {
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 2, 3, 4, 0, 1 },
        { 0, 1, 2, 3, 4 },
        { 1, 2, 3, 4, 0 },
        { 4, 0, 1, 2, 3 },
        { 4, 0, 1, 2, 3 },
        { 1, 2, 3, 4, 0 },
        { 0, 1, 2, 3, 4 },
        { 2, 3, 4, 0, 1 }
    };

    size_t t = 0;
    for (size_t j = 0; j < _countof(faces); j += 5, ++t)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];
        uint32_t v3 = faces[j + 3];
        uint32_t v4 = faces[j + 4];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 2);
        index_push_backHot3dx(indices, base + 3);

        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 3);
        index_push_backHot3dx(indices, base + 4);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[0], textureCoordinates[0]));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[1], textureCoordinates[1]));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[2], textureCoordinates[2]));


        position = XMVectorScale(verts[v3], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[3], textureCoordinates[3]));


        position = XMVectorScale(verts[v4], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[4], textureCoordinates[4]));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 12 * 5);
    assert(indices.size() == 12 * 3 * 3);
}

void ComputeIcosahedronDualTexture(VertexCollectionDualTexture& vertices, IndexCollectionColor& indices, float size, bool rhcoords)
{
    vertices.clear();
    indices.clear();

    XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

    static const float  t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
    static const float t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

    static const XMVECTORF32 verts[12] =
    {
        { { {    t / t2,  1.f / t2,       0, 0 } } },
        { { {   -t / t2,  1.f / t2,       0, 0 } } },
        { { {    t / t2, -1.f / t2,       0, 0 } } },
        { { {   -t / t2, -1.f / t2,       0, 0 } } },
        { { {  1.f / t2,       0,    t / t2, 0 } } },
        { { {  1.f / t2,       0,   -t / t2, 0 } } },
        { { { -1.f / t2,       0,    t / t2, 0 } } },
        { { { -1.f / t2,       0,   -t / t2, 0 } } },
        { { {       0,    t / t2,  1.f / t2, 0 }  } },
        { { {       0,   -t / t2,  1.f / t2, 0 } } },
        { { {       0,    t / t2, -1.f / t2, 0 } } },
        { { {       0,   -t / t2, -1.f / t2, 0 } } }
    };

    static const XMVECTORF32 textureCoordinates[3] =
    {
        { { {  0.654508f, 0.0244717f, 0, 0 } } },
        { { { 0.0954915f,  0.793893f, 0, 0 } } },
        { { {        1.f,       0.5f, 0, 0 } } }
    };

    static const uint32_t faces[20 * 3] =
    {
        0, 8, 4,
        0, 5, 10,
        2, 4, 9,
        2, 11, 5,
        1, 6, 8,
        1, 10, 7,
        3, 9, 6,
        3, 7, 11,
        0, 10, 8,
        1, 8, 10,
        2, 9, 11,
        3, 11, 9,
        4, 2, 0,
        5, 0, 2,
        6, 1, 3,
        7, 3, 1,
        8, 6, 4,
        9, 4, 6,
        10, 5, 7,
        11, 7, 5
    };

    for (size_t j = 0; j < _countof(faces); j += 3)
    {
        uint32_t v0 = faces[j];
        uint32_t v1 = faces[j + 1];
        uint32_t v2 = faces[j + 2];

        XMVECTOR normal = XMVector3Cross(
            XMVectorSubtract(verts[v1].v, verts[v0].v),
            XMVectorSubtract(verts[v2].v, verts[v0].v));
        normal = XMVector3Normalize(normal);

        size_t base = vertices.size();
        index_push_backHot3dx(indices, base);
        index_push_backHot3dx(indices, base + 1);
        index_push_backHot3dx(indices, base + 2);

        // Duplicate vertices to use face normals
        XMVECTOR position = XMVectorScale(verts[v0], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[0], textureCoordinates[0]));


        position = XMVectorScale(verts[v1], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[1], textureCoordinates[1]));


        position = XMVectorScale(verts[v2], size);
        vertices.push_back(DirectX::DXTKXAML12::VertexPositionDualTexture(position, textureCoordinates[2], textureCoordinates[2]));

    }

    // Built LH above
    if (rhcoords)
        ReverseWindingHot3dx(indices, vertices);

    assert(vertices.size() == 20 * 3);
    assert(indices.size() == 20 * 3);
}


} // Eo Namespace DirectX Namespace
