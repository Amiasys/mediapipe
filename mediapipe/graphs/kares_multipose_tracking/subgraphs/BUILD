# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load(
    "//mediapipe/framework/tool:mediapipe_graph.bzl",
    "mediapipe_simple_subgraph",
)

licenses(["notice"])

package(default_visibility = ["//visibility:public"])

mediapipe_simple_subgraph(
    name = "box_tracking_gpu",
    graph = "box_tracking_gpu.pbtxt",
    register_as = "BoxTrackingSubgraphGpu",
    deps = [
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/video:box_tracker_calculator",
        "//mediapipe/calculators/video:flow_packager_calculator",
        "//mediapipe/calculators/video:motion_analysis_calculator",
        "//mediapipe/framework/stream_handler:immediate_input_stream_handler",
        "//mediapipe/framework/stream_handler:sync_set_input_stream_handler",
        "//mediapipe/gpu:gpu_buffer_to_image_frame_calculator",
        "//mediapipe/gpu:image_frame_to_gpu_buffer_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "box_tracking_cpu",
    graph = "box_tracking_cpu.pbtxt",
    register_as = "BoxTrackingSubgraphCpu",
    deps = [
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/video:box_tracker_calculator",
        "//mediapipe/calculators/video:flow_packager_calculator",
        "//mediapipe/calculators/video:motion_analysis_calculator",
        "//mediapipe/framework/stream_handler:immediate_input_stream_handler",
        "//mediapipe/framework/stream_handler:sync_set_input_stream_handler",
        "//mediapipe/gpu:image_frame_to_gpu_buffer_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "object_tracking_gpu",
    graph = "object_tracking_gpu.pbtxt",
    register_as = "ObjectTrackingSubgraphGpu",
    deps = [
        "//mediapipe/calculators/util:detection_unique_id_calculator",
        "//mediapipe/calculators/util:detections_to_timed_box_list_calculator",
        "//mediapipe/calculators/video:tracked_detection_manager_calculator",
        "//mediapipe/framework/stream_handler:sync_set_input_stream_handler",
        "//mediapipe/graphs/tracking/subgraphs:box_tracking_gpu",
    ],
)

mediapipe_simple_subgraph(
    name = "object_tracking_cpu",
    graph = "object_tracking_cpu.pbtxt",
    register_as = "ObjectTrackingSubgraphCpu",
    deps = [
        "//mediapipe/calculators/util:detection_unique_id_calculator",
        "//mediapipe/calculators/util:detections_to_timed_box_list_calculator",
        "//mediapipe/calculators/video:tracked_detection_manager_calculator",
        "//mediapipe/framework/stream_handler:sync_set_input_stream_handler",
        "//mediapipe/graphs/tracking/subgraphs:box_tracking_gpu",
    ],
)

mediapipe_simple_subgraph(
    name = "object_detection_gpu",
    graph = "object_detection_gpu.pbtxt",
    register_as = "ObjectDetectionSubgraphGpu",
    deps = [
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/tflite:ssd_anchors_calculator",
        "//mediapipe/calculators/tflite:tflite_converter_calculator",
        "//mediapipe/calculators/tflite:tflite_inference_calculator",
        "//mediapipe/calculators/tflite:tflite_tensors_to_detections_calculator",
        "//mediapipe/calculators/util:detection_label_id_to_text_calculator",
        "//mediapipe/calculators/util:non_max_suppression_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "object_detection_cpu",
    graph = "object_detection_cpu.pbtxt",
    register_as = "ObjectDetectionSubgraphCpu",
    deps = [
        "//mediapipe/calculators/image:image_transformation_calculator",
        "//mediapipe/calculators/tflite:ssd_anchors_calculator",
        "//mediapipe/calculators/tflite:tflite_converter_calculator",
        "//mediapipe/calculators/tflite:tflite_inference_calculator",
        "//mediapipe/calculators/tflite:tflite_tensors_to_detections_calculator",
        "//mediapipe/calculators/util:detection_label_id_to_text_calculator",
        "//mediapipe/calculators/util:non_max_suppression_calculator",
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
        "//mediapipe/calculators/tensor:tensors_to_detections_calculator",
        "//mediapipe/calculators/util:detection_letterbox_removal_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "renderer_gpu",
    graph = "renderer_gpu.pbtxt",
    register_as = "RendererSubgraphGpu",
    deps = [
        "//mediapipe/calculators/util:annotation_overlay_calculator",
        "//mediapipe/calculators/util:detections_to_render_data_calculator",
        "//mediapipe/calculators/util:rect_to_render_data_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "renderer_cpu",
    graph = "renderer_cpu.pbtxt",
    register_as = "RendererSubgraphCpu",
    deps = [
        "//mediapipe/calculators/util:annotation_overlay_calculator",
        "//mediapipe/calculators/util:detections_to_render_data_calculator",
        "//mediapipe/calculators/util:rect_to_render_data_calculator",
        "//mediapipe/calculators/core:split_vector_calculator",
        "//mediapipe/calculators/core:begin_loop_calculator",
        "//mediapipe/calculators/core:end_loop_calculator",
        "//mediapipe/calculators/util:landmarks_to_render_data_calculator",
        "//mediapipe/calculators/util:collection_has_min_size_calculator",
    ],
)


mediapipe_simple_subgraph(
    name = "detections_to_rois",
    graph = "detections_to_rois.pbtxt",
    register_as = "DetectionsToRoisSubgraphCpu",
    deps = [
        "//mediapipe/calculators/util:alignment_points_to_rects_calculator",
        "//mediapipe/calculators/util:rect_transformation_calculator",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/core:begin_loop_calculator",
        "//mediapipe/calculators/core:end_loop_calculator",
        "//mediapipe/calculators/util:detections_to_rects_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_landmark_by_roi_cpu",
    graph = "pose_landmark_by_roi_cpu.pbtxt",
    register_as = "PoseLandmarkByRoiCpu",
    deps = [
        ":pose_landmark_model_loader",
        ":pose_landmarks_and_segmentation_inverse_projection",
        ":tensors_to_pose_landmarks_and_segmentation",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_landmark_by_roi_gpu",
    graph = "pose_landmark_by_roi_gpu.pbtxt",
    register_as = "PoseLandmarkByRoiGpu",
    deps = [
        ":pose_landmark_model_loader",
        ":pose_landmarks_and_segmentation_inverse_projection",
        ":tensors_to_pose_landmarks_and_segmentation",
        "//mediapipe/calculators/image:image_properties_calculator",
        "//mediapipe/calculators/tensor:image_to_tensor_calculator",
        "//mediapipe/calculators/tensor:inference_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_landmark_model_loader",
    graph = "pose_landmark_model_loader.pbtxt",
    register_as = "PoseLandmarkModelLoader",
    deps = [
        "//mediapipe/calculators/core:constant_side_packet_calculator",
        "//mediapipe/calculators/tflite:tflite_model_calculator",
        "//mediapipe/calculators/util:local_file_contents_calculator",
        "//mediapipe/framework/tool:switch_container",
    ],
)

mediapipe_simple_subgraph(
    name = "pose_landmarks_and_segmentation_inverse_projection",
    graph = "pose_landmarks_and_segmentation_inverse_projection.pbtxt",
    register_as = "PoseLandmarksAndSegmentationInverseProjection",
    deps = [
        "//mediapipe/calculators/image:warp_affine_calculator",
        "//mediapipe/calculators/util:inverse_matrix_calculator",
        "//mediapipe/calculators/util:landmark_letterbox_removal_calculator",
        "//mediapipe/calculators/util:landmark_projection_calculator",
        "//mediapipe/calculators/util:world_landmark_projection_calculator",
    ],
)

mediapipe_simple_subgraph(
    name = "tensors_to_pose_landmarks_and_segmentation",
    graph = "tensors_to_pose_landmarks_and_segmentation.pbtxt",
    register_as = "TensorsToPoseLandmarksAndSegmentation",
    deps = [
        "//mediapipe/calculators/core:gate_calculator",
        "//mediapipe/calculators/core:split_proto_list_calculator",
        "//mediapipe/calculators/core:split_vector_calculator",
        "//mediapipe/calculators/tensor:tensors_to_floats_calculator",
        "//mediapipe/calculators/tensor:tensors_to_landmarks_calculator",
        "//mediapipe/calculators/tensor:tensors_to_segmentation_calculator",
        "//mediapipe/calculators/util:refine_landmarks_from_heatmap_calculator",
        "//mediapipe/calculators/util:thresholding_calculator",
        "//mediapipe/calculators/util:visibility_copy_calculator",
    ],
)