# YOLO-DarkNet-CPP-Inference

A high-performance C++ implementation for running YOLO (You Only Look Once) Darknet models. This project enables fast and efficient object detection using pre-trained Darknet models, leveraging C++ for optimal speed and resource usage.

## Features

* ✅ **High Performance:** Optimized C++ code for real-time inference.
* ✅ **Darknet Model Support:** Load and run YOLO models trained with Darknet.
* ✅ **Easy Integration:** Simple API for integrating object detection into your C++ applications.
* ✅ **Flexible Input:** Supports both image and live camera input.
* ✅ **Flexible Output:** Save results as annotated images or GIF/video format.
* ✅ **Customizable Parameters:** Set confidence threshold and NMS threshold from CLI.

---

## Requirements

* C++17 or later
* OpenCV (for image/video processing)
* CMake (for building)
* Darknet model files:

  * `.cfg` (network configuration)
  * `.weights` (trained weights)
  * `.names` (class labels)

---
## Download Pretrained Models
| Model       | Weights (.weights)                                                                           | Config (.cfg)                                                                            |
| ----------- | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| YOLOv4      | [Download](https://github.com/AlexeyAB/darknet/releases/download/yolov4/yolov4.weights)      | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov4.cfg)               |
| YOLOv4-Tiny | [Download](https://github.com/AlexeyAB/darknet/releases/download/yolov4/yolov4-tiny.weights) | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov4-tiny.cfg)          |
| YOLOv7      | [Download](https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7.weights)       | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov7.cfg)      |
| YOLOv7-Tiny | [Download](https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7-tiny.weights)  | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov7-tiny.cfg) |

## Build Instructions

```bash
git clone https://github.com/yourusername/YOLO-DarkNet-CPP-Inference.git
cd YOLO-DarkNet-CPP-Inference
mkdir build && cd build
cmake ..
make
```

---

## Usage

```bash
./cv_app (--image <image_path> | --camera <camera_index>) --weights <weights_file> --cfg <cfg_file> [options]
```

### Required Arguments

* `--image <path>`: Path to the input image.
* `--camera <index>`: Index of camera to use (e.g. 0 for default webcam).
* `--weights <path>`: Path to the YOLO `.weights` file.
* `--cfg <path>`: Path to the YOLO `.cfg` file.

### Optional Arguments

* `--names <path>`: Path to the `.names` file for class labels (default: `./models/coco.names`)
* `--conf <float>`: Confidence threshold for detections (default: `0.25`)
* `--nms <float>`: Non-maximum suppression threshold (default: `0.4`)
* `--out <filename>`: Save output image or video/GIF (format based on extension)

---

## Examples

### Run on a Static Image

```bash
./cv_app --image ../samples/zidane.jpg --weights ../models/yolov7.weights --cfg ../models/yolov7.cfg --names ../models/coco.names --conf 0.3 --nms 0.4 --out result.jpg
```

### Run on Live Camera (Webcam Index 0)

```bash
./cv_app --camera 0 --weights ../models/yolov7.weights --cfg ../models/yolov7.cfg --names ../models/coco.names --conf 0.3 --nms 0.4 --out result.gif
```

---

## Output 
* **GIF/Video**:
![Output with camera mode](assets/video.gif)

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgements

* [Darknet](https://pjreddie.com/darknet/) by Joseph Redmon
* [YOLO](https://pjreddie.com/darknet/yolo/)
* [OpenCV](https://opencv.org/) for image processing

---
