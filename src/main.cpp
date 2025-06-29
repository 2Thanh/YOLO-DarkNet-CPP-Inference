#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "inference.h"

#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char** argv) {
    std::unordered_map<std::string, std::string> args;
    for (int i = 1; i + 1 < argc; i += 2) {
        args[argv[i]] = argv[i + 1];
    }

    // Required arguments
    if ((args.find("--image") == args.end() && args.find("--camera") == args.end()) ||
        args.find("--weights") == args.end() || args.find("--cfg") == args.end()) {
        std::cout << "Usage:\n"
                  << "  " << argv[0] << " --image <image_path>\n"
                  << "  OR\n"
                  << "  " << argv[0] << " --camera <camera_index>\n"
                  << "Required:\n"
                  << "  --weights <weights_path>\n"
                  << "  --cfg <config_path>\n"
                  << "Optional:\n"
                  << "  --names <class_names_path> (default: coco.names)\n"
                  << "  --conf <confidence_threshold> (default: 0.25)\n"
                  << "  --nms <nms_threshold> (default: 0.4)\n";
        return 1;
    }

    // Assign required paths
    std::string model_path = args["--weights"];
    std::string config_path = args["--cfg"];

    // Optional with defaults
    std::string class_names_path = args.count("--names") 
        ? args["--names"] 
        : "coco.names";
    
    float confidence_threshold = args.count("--conf") 
        ? std::stof(args["--conf"]) 
        : 0.25f;
    
    float nms_threshold = args.count("--nms") 
        ? std::stof(args["--nms"]) 
        : 0.4f;

    // Setup detector
    YOLOConfig config(model_path, config_path);
    config.class_names = YOLOUtils::loadClassNames(class_names_path);
    config.confidence_threshold = confidence_threshold;
    config.nms_threshold = nms_threshold;

    YOLODetector detector(config);

    // Run detection
    if (args.count("--image")) {
        run_image_inference(args["--image"], detector);
    } else if (args.count("--camera")) {
        int cam_idx = std::stoi(args["--camera"]);
        run_camera_inference(cam_idx, detector);
    }

    return 0;
}
