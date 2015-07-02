//
//  JSONPacker.cpp
//  Tetrominos
//
//  Created by ryutamaki on 2015/07/01.
//
//

#include "JSONPacker.h"
#include "Coordinate.h"

#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;

namespace JSONPacker
{
    TetrominoState unpackTetrominoJSON(std::string json, TetrominoType type)
    {
        rapidjson::Document document;
        document.Parse<0>(json.c_str());

        const char* typeStr;

        switch (type)
        {
            case TetrominoType::I: typeStr = "I"; break;
            case TetrominoType::T: typeStr = "T"; break;
            case TetrominoType::J: typeStr = "J"; break;
            case TetrominoType::L: typeStr = "L"; break;
            case TetrominoType::O: typeStr = "O"; break;
            case TetrominoType::S: typeStr = "S"; break;
            case TetrominoType::Z: typeStr = "Z"; break;
        }

        rapidjson::Value& tetrominoDoc = document[typeStr];

        rapidjson::Value& colorDoc = tetrominoDoc["color"];
        Color3B color;
        color.r = colorDoc["r"].GetInt();
        color.g = colorDoc["g"].GetInt();
        color.b = colorDoc["b"].GetInt();

        rapidjson::Value& rotationsDoc = tetrominoDoc["rotations"];
        std::vector<std::vector<Coordinate>> rotations;
        for (rapidjson::SizeType rotationIndex = 0; rotationIndex < rotationsDoc.Size(); rotationIndex++) {
            rapidjson::Value& rotationDoc = rotationsDoc[rotationIndex];
            std::vector<Coordinate> rotation;
            for (rapidjson::SizeType coordinateIndex = 0; coordinateIndex < rotationDoc.Size(); coordinateIndex++) {
                Coordinate coordinate = Coordinate(rotationDoc[coordinateIndex]["x"].GetInt(), rotationDoc[coordinateIndex]["y"].GetInt());
                rotation.push_back(coordinate);
            }
            rotations.push_back(rotation);
        }

        TetrominoState state;
        state.color = color;
        state.rotations = rotations;
        return state;
    }
}