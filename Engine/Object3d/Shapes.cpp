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

Model* Shapes::CreateSquare(XMFLOAT2 upperLeft, XMFLOAT2 lowerBottom, const std::string& textureName, const XMFLOAT2& imageSize, const DirectX::XMFLOAT2& anchorpoint, const DirectX::XMFLOAT2& texBase, const DirectX::XMFLOAT2& texSize, bool isFlip)
{

    //UV設定
    XMFLOAT2 uvLT, uvLB, uvRT, uvRB;

    uvLT = { 0, 0 };
    uvLB = { 0, 1 };
    uvRT = { 1, 0 };
    uvRB = { 1, 1 };

    Model* squareModel = nullptr;
    Model::VertexPosNormalUv verticesSquare[] = {
        {{-upperLeft.x, -lowerBottom.y, 0.0f}, {0, 0, 1}, uvLB},  //左下
        {{-upperLeft.x, -upperLeft.y, 0.0f}, {0, 0, 1}, uvLT},    //左上
        {{-lowerBottom.x, -lowerBottom.y, 0.0f}, {0, 0, 1}, uvRB},//右下
        {{-lowerBottom.x, -upperLeft.y, 0.0f}, {0, 0, 1}, uvRT},  //右上
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
    if (imageSize.x != 0.0f && imageSize.y != 0.0f) {
        squareModel->SetSize(imageSize);
    }
    squareModel->SetAnchorpoint(anchorpoint);
    if (texSize.x != 0.0f && texSize.y != 0.0f) {
        squareModel->SetTexRect(texBase, texSize);
    }

    if (isFlip) {
        squareModel->SetIsFlip();
    }

    return squareModel;
}
