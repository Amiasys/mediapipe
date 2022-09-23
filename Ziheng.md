bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/multipose_tracking:multipose_tracking_cpu


GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt   --input_video_path=ziheng_multiperson_8fps.mp4 --output_video_path=single_pose_8fps.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt   --input_video_path=basketball_1280x720_30fps.mp4 --output_video_path=multipose_bball_30fps.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt   --input_video_path=liu_1280x720_30fps.mp4 --output_video_path=multipose_liu_30fps.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt   --input_video_path=martialarts_1280x720_30fps.mp4 --output_video_path=multipose_ma_30fps.mp4


GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/multipose_tracking/multipose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/multipose_tracking/multipose_tracking_cpu.pbtxt   --input_video_path=videos/martialarts_1280x720_30fps.mp4 --output_video_path=results/multipose_ma_30fps.mp4

--input_video_path=
--output_video_path=


bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/pose_tracking:pose_tracking_cpu
GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/pose_tracking/pose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/pose_tracking/pose_tracking_cpu.pbtxt --input_video_path=videos/basketball_1280x720_30fps.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/pose_tracking/pose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/pose_tracking/pose_tracking_cpu.pbtxt --input_video_path=videos/Back_Street_Boy-720P.mov




bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 mediapipe/examples/desktop/object_tracking:object_tracking_gpu
GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/tracking/object_detection_tracking_desktop_live.pbtxt --input_video_path=videos/Back_Street_Boy-720P.mp4
GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/tracking/object_detection_tracking_desktop_live.pbtxt --input_video_path=videos/Snow-Street-Walk.mp4 --output_video_path=results/snow_30fps.mp4



bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 mediapipe/examples/desktop/object_detection:object_detection_cpu

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_detection/object_detection_cpu   --calculator_graph_config_file=mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=videos/Snow-Street-Walk.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_detection/object_detection_cpu   --calculator_graph_config_file=mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=videos/Snow-Street-Walk.mp4 --output_video_path=results/snow_30fps.mp4


GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_detection/object_detection_cpu   --calculator_graph_config_file=mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=videos/Back_Street_Boy-480P.mp4




bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/object_detection:object_detection_cpu
bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/object_tracking:object_tracking_cpu


bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/pose_tracking:pose_tracking_cpu

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_detection/object_detection_cpu   --calculator_graph_config_file=mediapipe/graphs/object_detection/object_detection_desktop_live.pbtxt --input_video_path=videos/Snow-Street-Walk-480P.mp4

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/tracking/object_detection_tracking_desktop_live.pbtxt --input_video_path=videos/Snow-Street-Walk-480P.mp4


bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 mediapipe/examples/desktop/object_tracking:object_tracking_gpu

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_gpu   --calculator_graph_config_file=mediapipe/graphs/tracking/object_detection_tracking_mobile_gpu.pbtxt --input_video_path=videos/Snow-Street-Walk-480P.mp4


bazel build -c opt --define MEDIAPIPE_DISABLE_GPU=1 mediapipe/examples/desktop/pose_tracking:pose_tracking_cpu
GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/pose_tracking/pose_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/pose_tracking/pose_tracking_cpu.pbtxt --input_video_path=videos/Snow-Street-Walk-480P.mp4
bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 mediapipe/examples/desktop/pose_tracking:pose_tracking_gpu
GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/pose_tracking/pose_tracking_gpu   --calculator_graph_config_file=mediapipe/graphs/pose_tracking/pose_tracking_gpu.pbtxt --input_video_path=videos/Snow-Street-Walk-480P.mp4







```
bazel build -c opt --copt -DMESA_EGL_NO_X11_HEADERS --copt -DEGL_NO_X11 mediapipe/examples/desktop/kares_multipose_tracking:kares_multipose_tracking

GLOG_logtostderr=1 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/kares_multipose_tracking/object_detection_tracking_desktop_live.pbtxt --input_video_path=videos/KidsYoga_360P_10FPS.mp4
```