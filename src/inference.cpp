#include "inference.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// Detection struct implementation
Detection::Detection(int id, float conf, cv::Rect box, const std::string& name)
    : class_id(id), confidence(conf), bbox(box), class_name(name) {}

// YOLOConfig struct implementation
YOLOConfig::YOLOConfig(const std::string& model, const std::string& config)
    : model_path(model), config_path(config), confidence_threshold(0.5f), 
      nms_threshold(0.4f), input_size(cv::Size(640, 640)), 
      mean(cv::Scalar(0, 0, 0)), scale_factor(1.0/255.0), swap_rb(true) {}

// YOLODetector class implementation
YOLODetector::YOLODetector(const YOLOConfig& cfg) : config(cfg) {
    // Load the network
    if (config.config_path.empty()) {
        net = cv::dnn::readNetFromDarknet(config.model_path);
    } else {
        net = cv::dnn::readNetFromDarknet(config.config_path, config.model_path);
    }
    
    // Get output layer names
    output_layer_names = net.getUnconnectedOutLayersNames();
    
    // Set backend and target
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::vector<Detection> YOLODetector::detect(const cv::Mat& image) {
    // Clear previous results
    class_ids.clear();
    confidences.clear();
    boxes.clear();
    
    // Create blob from image
    cv::dnn::blobFromImage(image, blob, config.scale_factor, 
                          config.input_size, config.mean, 
                          config.swap_rb, false);
    
    // Set input to the network
    net.setInput(blob);
    
    // Run forward pass
    net.forward(outputs, output_layer_names);
    
    // Parse outputs
    parseOutputs(outputs, image.size());
    
    // Apply Non-Maximum Suppression
    cv::dnn::NMSBoxes(boxes, confidences, config.confidence_threshold, 
                     config.nms_threshold, indices);
    
    // Build final detection results
    std::vector<Detection> detections;
    for (int idx : indices) {
        std::string class_name = (class_ids[idx] < config.class_names.size()) ? 
                               config.class_names[class_ids[idx]] : 
                               "Unknown";
        detections.emplace_back(class_ids[idx], confidences[idx], 
                              boxes[idx], class_name);
    }
    
    return detections;
}

void YOLODetector::parseOutputs(const std::vector<cv::Mat>& outputs, cv::Size image_size) {
    float x_factor = static_cast<float>(image_size.width) / config.input_size.width;
    float y_factor = static_cast<float>(image_size.height) / config.input_size.height;
    
    for (const auto& output : outputs) {
        // Handle different output dimensions (2D vs 3D)
        cv::Mat flat_output;
        if (output.dims == 3) {
            // 3D output: reshape to 2D
            flat_output = output.reshape(1, output.size[1]);
        } else if (output.dims == 2) {
            // Already 2D
            flat_output = output;
        } else {
            std::cerr << "Unsupported output dimensions: " << output.dims << std::endl;
            continue;
        }
        
        const int rows = flat_output.rows;
        const int cols = flat_output.cols;
        
        // Verify minimum columns (x, y, w, h, confidence + at least 1 class)
        if (cols < 6) {
            std::cerr << "Output has insufficient columns: " << cols << std::endl;
            continue;
        }
        
        for (int i = 0; i < rows; ++i) {
            const float* data = flat_output.ptr<float>(i);
            
            // Extract coordinates and confidence
            float cx = data[0];
            float cy = data[1];
            float w = data[2];
            float h = data[3];
            float confidence = data[4];
            
            if (confidence >= config.confidence_threshold) {
                // Find best class
                float max_class_score = 0;
                int best_class_id = 0;
                
                for (int j = 5; j < cols; ++j) {
                    if (data[j] > max_class_score) {
                        max_class_score = data[j];
                        best_class_id = j - 5;
                    }
                }
                
                if (max_class_score > config.confidence_threshold) {
                    // Convert normalized coordinates to pixel coordinates
                    // cx, cy, w, h are in range [0, 1] relative to input size
                    float pixel_cx = cx * image_size.width;
                    float pixel_cy = cy * image_size.height;
                    float pixel_w = w * image_size.width;
                    float pixel_h = h * image_size.height;
                    
                    // Convert center coordinates to top-left corner
                    int left = static_cast<int>(pixel_cx - 0.5 * pixel_w);
                    int top = static_cast<int>(pixel_cy - 0.5 * pixel_h);
                    int width = static_cast<int>(pixel_w);
                    int height = static_cast<int>(pixel_h);
                    
                    // Ensure bounding box is within image bounds
                    left = std::max(0, left);
                    top = std::max(0, top);
                    width = std::min(width, image_size.width - left);
                    height = std::min(height, image_size.height - top);
                    
                    if (width > 0 && height > 0) {
                        boxes.emplace_back(left, top, width, height);
                        confidences.push_back(confidence);
                        class_ids.push_back(best_class_id);
                    }
                }
            }
        }
    }
}

// YOLOUtils namespace implementation
namespace YOLOUtils {
    std::vector<std::string> loadClassNames(const std::string& filename) {
        std::vector<std::string> class_names;
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            class_names.push_back(line);
        }
        return class_names;
    }
    
    void drawDetections(cv::Mat& image, const std::vector<Detection>& detections) {
        for (const auto& detection : detections) {
            // Draw bounding box
            cv::rectangle(image, detection.bbox, cv::Scalar(0, 255, 0), 2);
            
            // Draw label
            std::string label = detection.class_name + ": " + 
                              std::to_string(detection.confidence);
            int baseline;
            cv::Size text_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 
                                               0.5, 1, &baseline);
            cv::Point text_origin(detection.bbox.x, 
                                detection.bbox.y - text_size.height);
            
            cv::rectangle(image, text_origin + cv::Point(0, baseline),
                         text_origin + cv::Point(text_size.width, -text_size.height),
                         cv::Scalar(0, 255, 0), cv::FILLED);
            cv::putText(image, label, text_origin, cv::FONT_HERSHEY_SIMPLEX, 
                       0.5, cv::Scalar(0, 0, 0), 1);
        }
    }
    
    void drawFPS(cv::Mat& image, double fps) {
        std::string fps_text = "FPS: " + std::to_string(static_cast<int>(fps));
        cv::putText(image, fps_text, cv::Point(10, 30), 
                   cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    }
}

// Function to run inference on an image
void run_image_inference(const std::string& image_path, YOLODetector& detector)
{
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        std::cerr << "Could not read image: " << image_path << std::endl;
        return;
    }
    
    // Perform detection
    std::vector<Detection> detections = detector.detect(img);
    
    // Draw detections
    YOLOUtils::drawDetections(img, detections);
    
    // Show result
    cv::imshow("Inference Result", img);
    cv::waitKey(0);
}

void run_camera_inference(int camera_index, YOLODetector& detector, int num_skipped_frames) {
    cv::VideoCapture cap(camera_index);
    if (!cap.isOpened()) {
        std::cerr << "Could not open camera: " << camera_index << std::endl;
        return;
    }
    cv::Mat frame;
    int frame_count = 0;
    while (true) {
        if (num_skipped_frames > 0 && ++frame_count % num_skipped_frames == 0) {
            continue; // Skip frames for performance
        }
        cap >> frame;
        if (frame.empty()) break;
        // Replace with your actual inference function
        std::vector<Detection> detections = detector.detect(frame);
        YOLOUtils::drawDetections(frame, detections);
        cv::imshow("Realtime Inference", frame);
        if (cv::waitKey(1) == 27) break; // ESC to exit
    }
    cap.release();
    cv::destroyAllWindows();
}
void run_rtsp_inference(const std::string& rtsp_url, YOLODetector& detector, int num_skipped_frames) {
    cv::VideoCapture cap(rtsp_url);
    if (!cap.isOpened()) {
        std::cerr << "Could not open RTSP stream: " << rtsp_url << std::endl;
        return;
    }
    cv::Mat frame;
    int frame_count = 0;
    while (true) {
        if (num_skipped_frames > 0 && ++frame_count % num_skipped_frames == 0) {
            continue; // Skip frames for performance
        }
        cap >> frame;
        if (frame.empty()) break;
        // Replace with your actual inference function
        std::vector<Detection> detections = detector.detect(frame);
        YOLOUtils::drawDetections(frame, detections);
        cv::imshow("RTSP Inference", frame);
        if (cv::waitKey(1) == 27) break; // ESC to exit
    }
    cap.release();
    cv::destroyAllWindows();
}