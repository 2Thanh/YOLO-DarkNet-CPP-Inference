# YOLO-DarkNet-CPP-Inference

A high-performance C++ application for running YOLO (You Only Look Once) Darknet models using OpenCV. Supports real-time object detection on images, video files, RTSP streams, and live camera input.

---

## Features

- 🚀 Real-time inference with YOLOv4, YOLOv7, and Tiny models
- 🎥 Supports images, video files, RTSP streams, and webcams
- 🖼️ Saves annotated output as images, GIF, or video
- ⚙️ Configurable thresholds and model parameters via CLI
- 📦 Easy to build with CMake

---

## Requirements

- C++17 or later
- OpenCV (tested with OpenCV 4.x)
- CMake
- Darknet model files: `.cfg`, `.weights`, `.names`

---

## Download Pretrained Models

| Model       | Weights (.weights)                                                                           | Config (.cfg)                                                                            |
| ----------- | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| YOLOv4      | [Download](https://github.com/AlexeyAB/darknet/releases/download/yolov4/yolov4.weights)      | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov4.cfg)               |
| YOLOv4-Tiny | [Download](https://github.com/AlexeyAB/darknet/releases/download/yolov4/yolov4-tiny.weights) | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov4-tiny.cfg)          |
| YOLOv7      | [Download](https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7.weights)       | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov7.cfg)               |
| YOLOv7-Tiny | [Download](https://github.com/WongKinYiu/yolov7/releases/download/v0.1/yolov7-tiny.weights)  | [Download](https://github.com/AlexeyAB/darknet/blob/master/cfg/yolov7-tiny.cfg)          |

---

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
./cv_app (--image <image_path> | --camera <camera_index> | --video <video_path> | --rtsp_url <rtsp_url>) \
  --weights <weights_file> --cfg <cfg_file> [options]
```

### Required Arguments

- `--image <path>`: Path to the input image.
- `--camera <index>`: Index of camera to use (e.g. 0 for default webcam).
- `--video <path>`: Path to a video file.
- `--rtsp_url <url>`: RTSP stream URL.
- `--weights <path>`: Path to the YOLO `.weights` file.
- `--cfg <path>`: Path to the YOLO `.cfg` file.

### Optional Arguments

- `--names <path>`: Path to the `.names` file for class labels (default: `./models/coco.names`)
- `--conf <float>`: Confidence threshold for detections (default: `0.25`)
- `--nms <float>`: Non-maximum suppression threshold (default: `0.4`)
- `--out <filename>`: Save output image or video/GIF (format based on extension)
- `--intrusion`: Enable intrusion detection (requires boxes.json, you can generate by using drawing_intrusion.py)

---

## Examples

### Run on a Static Image

```bash
./cv_app --image ../samples/zidane.jpg --weights ../models/yolov7.weights --cfg ../models/yolov7.cfg --names ../models/coco.names --conf 0.3 --nms 0.4 
```

### Run on Live Camera (Webcam Index 0)

```bash
./cv_app --camera 0 --weights ../models/yolov7.weights --cfg ../models/yolov7.cfg --names ../models/coco.names --conf 0.3 --nms 0.4
```

### Run on a Video File

```bash
./cv_app --rtsp_url ../samples/videoplayback.mp4 --weights ../models/yolov4-tiny.weights --cfg ../models/yolov4-tiny.cfg --names ../models/coco.names --conf 0.3 --nms 0.4
```

### Run on RTSP Stream

```bash
./cv_app --rtsp_url rtsp://your_stream_url --weights ../models/yolov4-tiny.weights --cfg ../models/yolov4-tiny.cfg --names ../models/coco.names --conf 0.3 --nms 0.4
```

---

## Output

- **GIF/Video**:  
  ![Output with camera mode](assets/video.gif)

---

## Intrusion result
  **GIF/Video**:  
  ![Output with camera mode](assets/intrusion.gif)
## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Acknowledgements

- [Darknet](https://pjreddie.com/darknet/) by Joseph Redmon
- [YOLO](https://pjreddie.com/darknet/yolo/)
- [OpenCV](https://opencv.org/) for image processing

---