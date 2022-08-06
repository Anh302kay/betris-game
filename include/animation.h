#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

//#include <json/json.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct animationFrame
{
    animationFrame() {}
    SDL_Rect frame = {0, 0, 0, 0};
    int duration = 1000; // duration of frame in milliseconds;
    void print()
    {
        std::cout << "Duration: " << duration << " Frame: " << frame.x << ", " << frame.y << ", " << frame.w << ", " << frame.h << "\n";
    }
};

struct animationSet
{
    animationSet() {}
    std::vector<animationFrame> animationFrames = {};
    unsigned int currentFrame = 0; // current frame in animationFrames
    float speed = 1.0f; //speed that the animation is played at. different from duration of animationFrame
    float delta = 0.0f;
};

//might be plagerised off of codergoper's code
// inline std::map<std::string, animationSet> loadAnimationFile(const char *filePath)
// {
//     Json::Value root;

//     std::ifstream ifs;
//     ifs.open(filePath);

//     Json::CharReaderBuilder builder;
//     builder["collectComments"] = true;
//     JSONCPP_STRING errs;

//     // load everything in the file into root
//     if (!parseFromStream(builder, ifs, &root, &errs))
//         std::cout << errs << "\n";

//     const Json::Value frames = root["frames"];

//     const Json::Value meta = root["meta"];

//     //we will store all the frames in this vector
//     std::vector<animationFrame> allAnimationFrames = {};

//     for(Uint8 i = 0; i < frames.size(); i++)
//     {
//         const Json::Value frame = frames[i];
//         const Json::Value rect = frame["frame"];

//         animationFrame tempFrame;
//         tempFrame.duration = std::stoi(frame["duration"].asString());
//         tempFrame.frame.x = std::stoi(rect["x"].asString());
//         tempFrame.frame.y = std::stoi(rect["y"].asString());
//         tempFrame.frame.w = std::stoi(rect["w"].asString());
//         tempFrame.frame.h = std::stoi(rect["h"].asString());
//         allAnimationFrames.push_back(tempFrame);
//     }

//     std::map<std::string, animationSet> animationCycle = {};

//     const Json::Value frameTags = meta["frameTags"];

//     for (Uint8 i = 0; i < frameTags.size(); i++)
//     {
//         animationSet tempSet;
//         const Json::Value tag = frameTags[i];
//         int begin = std::stoi(tag["from"].asString());
//         int end = std::stoi(tag["to"].asString());

//         for(int j = begin; j <= end; j++)
//         {
//             tempSet.animationFrames.push_back(allAnimationFrames[j]);
//         }

//         animationCycle.insert(std::pair<std::string, animationSet>(tag["name"].asString(), tempSet));
//     }

//     return animationCycle;
// }
