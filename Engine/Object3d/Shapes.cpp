#include "Shapes.h"

using namespace DirectX;

Model* Shapes::CreateTriangle(const XMFLOAT2& vertices_1, const XMFLOAT2& vertices_2, const XMFLOAT2& vertices_3, const std::string& textureName)
{
    Model* triangleModel = nullptr;
    Model::VertexPosNormalUv verticesTriangle[] = {
        {{vertices_1.x, vertices_1.y, 0.0f}, {0, 0, 1}, {0, 1}},
        {{vertices_2.x, vertices_2.y, 0.0f}, {0, 0, 1}, {0, 0}},
        {{vertices_3.x, vertices_3.y, 0.0f}, {0, 0, 1}, {1, 1}},
    };

    unsigned short indicesTriangle[] = {
        0, 1, 2,
    };

    std::vector<Model::VertexPosNormalUv> vertices;
    std::vector<unsigned short> indices;
    for (int32_t i = 0; i < 3; i++) {
        vertices.emplace_back(verticesTriangle[i]);
        indices.emplace_back(indicesTriangle[i]);
    }

    triangleModel = Model::CreateShapeModel(vertices, indices, textureName);

    return triangleModel;
}

Model* Shapes::CreateSquare(XMFLOAT2 upperLeft, XMFLOAT2 lowerBottom, const std::string& textureName)
{
    Model* squareModel = nullptr;
    Model::VertexPosNormalUv verticesSquare[] = {
        {{upperLeft.x, lowerBottom.y, 0.0f}, {0, 0, 1}, {0, 1}},  //¶‰º
        {{upperLeft.x, upperLeft.y, 0.0f}, {0, 0, 1}, {0, 0}},    //¶ã
        {{lowerBottom.x, lowerBottom.y, 0.0f}, {0, 0, 1}, {1, 1}},//‰E‰º
        {{lowerBottom.x, upperLeft.y, 0.0f}, {0, 0, 1}, {1, 0}},  //‰Eã
    };

    unsigned short indicesSquare[] = {
        0, 1, 2,
        2, 1, 3,
    };

    std::vector<Model::VertexPosNormalUv> vertices;
    for (int32_t i = 0; i < 4; i++) {
        vertices.emplace_back(verticesSquare[i]);
    }

    std::vector<unsigned short> indices;
    for (int32_t i = 0; i < 6; i++) {
        indices.emplace_back(indicesSquare[i]);
    }

    squareModel = Model::CreateShapeModel(vertices, indices, textureName);

    return squareModel;
}
