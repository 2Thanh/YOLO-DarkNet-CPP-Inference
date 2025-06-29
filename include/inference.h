#ifndef YOLO_DETECTOR_H
#define YOLO_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>

// Detection result structure
struct Detection {
    int class_id;
    float confidence;
    cv::Rect bbox;
    std::string class_name;
    
    Detection(int id, float conf, cv::Rect box, const std::string& name = "");
};

// YOLO model configuration
struct YOLOConfig {
    std::string model_path;
    std::string config_path;
    std::vector<std::string> class_names;
    float confidence_threshold;
    float nms_threshold;
    cv::Size input_size;
    cv::Scalar mean;
    double scale_factor;
    bool swap_rb;
    
    YOLOConfig(const std::string& model, const std::string& config = "");
};

// Main YOLO detector class
class YOLODetector {
private:
    cv::dnn::Net net;
    YOLOConfig config;
    std::vector<std::string> output_layer_names;
    
    // Pre-allocated containers for efficiency
    cv::Mat blob;
    std::vector<cv::Mat> outputs;
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<int> indices;
    
    void parseOutputs(const std::vector<cv::Mat>& outputs, cv::Size image_size);
    
public:
    YOLODetector(const YOLOConfig& cfg);
    std::vector<Detection> detect(const cv::Mat& image);
};

// Utility functions namespace
namespace YOLOUtils {
    std::vector<std::string> loadClassNames(const std::string& filename);
    void drawDetections(cv::Mat& image, const std::vector<Detection>& detections);
    void drawFPS(cv::Mat& image, double fps);
}

// Function to run inference on an image
void run_image_inference(const std::string& image_path, YOLODetector& detector);

// Function to run inference on a camera stream
void run_camera_inference(int camera_index, YOLODetector& detector);

#endif // YOLO_DETECTOR_H