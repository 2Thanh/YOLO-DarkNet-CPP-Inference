#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"  // Download from https://github.com/nlohmann/json
#include <opencv2/opencv.hpp>

using json = nlohmann::json;

struct Box {
    int x1,y1, x2, y2;
    
    Box(int x1, int y1, int x2, int y2) 
        : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

std::vector<Box> read_json(const std::string& filename);

bool isIntrusion(const std::vector<Detection>& detections, const std::vector<Box>& boxes, 
                 const cv::Size& image_size, float threshold);