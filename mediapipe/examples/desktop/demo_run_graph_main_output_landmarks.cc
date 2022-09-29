// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.

#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <nlohmann/json.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>



using json = nlohmann::json;


// #include <grpcpp/ext/proto_server_reflection_plugin.h>
// #include <grpcpp/grpcpp.h>
// #include <grpcpp/health_check_service_interface.h>


#include <cstdlib>
// #include <fstream>
#include <cstdio>
#include <iostream>

// #include <filesystem>


#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/calculators/util/landmarks_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/calculators/util/rect_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/calculators/util/detections_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/location_data.pb.h"

constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";
constexpr char kWindowName[] = "MediaPipe";
constexpr char kLandmarksStream[] = "unfiltered_pose_landmarks";
constexpr char kDetectionsStream[] = "filtered_tracked_detections";
constexpr char kFaceLandmarksStream[] = "multi_face_landmarks";


ABSL_FLAG(std::string, calculator_graph_config_file, "",
          "Name of file containing text format CalculatorGraphConfig proto.");
ABSL_FLAG(std::string, input_video_path, "",
          "Full path of video to load. "
          "If not provided, attempt to use a webcam.");
ABSL_FLAG(std::string, output_video_path, "",
          "Full path of where to save result (.mp4 only). "
          "If not provided, show result in a window.");
ABSL_FLAG(std::string, stream_number, "",
          "ith stream out of total number of n streams. "
          "If not provided, default is 1. ");

int frame_count = 0;
json json_output = {};
char hrminsec_start[100];
char hrminsec_processed[100];
char hrminsec_current_run[100];
std::vector<double> detections_vec_output;
std::vector<std::vector<double>> face_landmarks_vec_output;



struct FrameOutput
{
  std::string ts_frame_start;
  std::string ts_frame_processed;
  int frame_count;
  int person_id;
  std::vector<std::vector<double>> landmarks; // 33 body keypoints by 3 dimensions
  std::vector<double> detections;
  std::vector<std::vector<double>> face_landmarks; // 478 keypoints by 3 dimensions
};

// void to_json(json& j, const FrameOutput& outputFrame){
//   j = json{
//     {"ts_frame_started", outputFrame.ts_frame_start},
//     {"ts_frame_processed", outputFrame.ts_frame},
//     {"frame_count", outputFrame.frame_count},
//     {"person", {
//         {"person_id", outputFrame.person_id},
//         {"landmarks", outputFrame.landmarks}
//       }
//     }
//   };
// }
void to_json(json& j, const FrameOutput& outputFrame){
  j = json{
    {"ts_frame", outputFrame.ts_frame_start},
    {"ts_frame_processed", outputFrame.ts_frame_processed},
    {"frame_count", outputFrame.frame_count},
    {"person", {
        {"person_id", outputFrame.person_id},
        {"landmarks", outputFrame.landmarks},
        {"detections", outputFrame.detections},
        {"face_landmarks", outputFrame.face_landmarks}
      }
    }
  };
}



absl::Status RunMPPGraph() {
  std::string calculator_graph_config_contents;
  MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
      absl::GetFlag(FLAGS_calculator_graph_config_file),
      &calculator_graph_config_contents));
  LOG(INFO) << "Get calculator graph config contents: "
            << calculator_graph_config_contents;
  mediapipe::CalculatorGraphConfig config =
      mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
          calculator_graph_config_contents);

  LOG(INFO) << "Initialize the calculator graph.";
  mediapipe::CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));

  LOG(INFO) << "Initialize the camera or load the video.";
  cv::VideoCapture capture;
  const bool load_video = !absl::GetFlag(FLAGS_input_video_path).empty();
  if (load_video) {
    capture.open(absl::GetFlag(FLAGS_input_video_path));
  } else {
    capture.open(0);
    // capture.open("rtsp://admin:Activity@Kares@10.0.1.201:554/cam/realmonitor?channel=1&subtype=1");
  }
  RET_CHECK(capture.isOpened());

  cv::VideoWriter writer;
  const bool save_video = !absl::GetFlag(FLAGS_output_video_path).empty();
  if (!save_video) {
    /*
      FIXME: disable the opencv frame Display, need add FLAG to control the frame display
      */
   // cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
#if (CV_MAJOR_VERSION >= 3) && (CV_MINOR_VERSION >= 2)
    //capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    //capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    //capture.set(cv::CAP_PROP_FPS, 30);
#endif
  }

  LOG(INFO) << "Start running the calculator graph.";
  ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                   graph.AddOutputStreamPoller(kOutputStream));
  // Added streampoller for Stream Landmarks
  // ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller_landmarks,
  //                  graph.AddOutputStreamPoller(kLandmarksStream)); // TODO check syntax
  // std::ofstream log_output_landmarks("log_output_landmarks.txt");
  
  MP_RETURN_IF_ERROR(
    graph.ObserveOutputStream(kLandmarksStream, [&graph](const mediapipe::Packet& packet) -> ::mediapipe::Status {
    auto landmarksListVec = packet.Get<std::vector<mediapipe::NormalizedLandmarkList>>();
    // LOG(INFO) << "NormalizedLandmarkList.size(): " << landmarksListVec.size();
    
    // frame_count++;
    // if (frame_count >= 30)
    // {
    //   frame_count = 0;
    // }
    // std::cout << frame_count << std::endl;
    // json_output.push_back({"Frame", frame_count});
    auto current = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(current);

    tm* curr_tm;
    // char hrminsec[100];
    curr_tm = localtime(&current_time);
    std::strftime(hrminsec_processed, 50, "%T", curr_tm);
    // std::cout << hrminsec << " ";
    // json_output.push_back({"timestamp_frame", hrminsec});
    json json_person_output = {};
    // std::cout << "*********************** Frame: " << frame_count << "\n";
    for (int lmIdx = 0; lmIdx < landmarksListVec.size(); ++lmIdx) {
      // std::cout << "Person tracked ID: " << lmIdx << "\n";
      // log_output_landmarks << "Person tracked ID: " << lmIdx << "\n";
      

      // std::cout << j.dump() << "\n";
      std::vector<std::vector<double>> landmarks_vec_output;
      const auto& landmarksList = landmarksListVec[lmIdx];
      for (int i = 0; i < landmarksList.landmark_size(); ++i) {
        // std::cout << "Body Landmarks: " << i 
        // << " with x: " << landmarksList.landmark(i).x() 
        // << " with y: " << landmarksList.landmark(i).y() 
        // << " with z: " << landmarksList.landmark(i).z() << "\n";
        std::vector<double> xyz;
        xyz.push_back(landmarksList.landmark(i).x());
        xyz.push_back(landmarksList.landmark(i).y());
        xyz.push_back(landmarksList.landmark(i).z());
        landmarks_vec_output.push_back(xyz);



        // auto current = std::chrono::system_clock::now();
        // std::time_t current_time = std::chrono::system_clock::to_time_t(current);

        // tm* curr_tm;
        // char hrminsec[100];
        // curr_tm = localtime(&current_time);
        // std::strftime(hrminsec, 50, "%T", curr_tm);
        // // std::cout << hrminsec << " ";
        // json_output.push_back({"timestamp_frame", hrminsec});
        // std::cout << "Person " << lmIdx << " Landmarks: " << i 
        // << " (" << landmarksList.landmark(i).x() 
        // << " ," << landmarksList.landmark(i).y() 
        // << " ," << landmarksList.landmark(i).z() << ")\n";




        // log_output_landmarks << "Body Landmarks: " << i << " with x: " << landmarksList.landmark(i).x() << " with y: " << landmarksList.landmark(i).y() << " with z: "  << landmarksList.landmark(i).z() << "\n";
      }

      const FrameOutput fo{
        hrminsec_start,
        hrminsec_processed,
        frame_count,
        lmIdx,
        landmarks_vec_output,
        detections_vec_output,
        face_landmarks_vec_output
      };
      json j_frameoutput {fo};
      // json_output.push_back(j_frameoutput);
      // std::cout << j_frameoutput.dump() << std::endl;
      json_person_output.push_back(j_frameoutput);


    }
    std::string str_hr_min_sec_start = std::string(hrminsec_start);
    // std::string str_frame_count = std::to_string(frame_count);
    std::stringstream stream_frame_count;
    stream_frame_count << std::setw(3) << std::setfill('0') << frame_count;
    std::string str_frame_count = stream_frame_count.str();

    std::stringstream stream_num;
    stream_num << std::setw(3) << std::setfill('0') << absl::GetFlag(FLAGS_stream_number);
    std::string str_stream_num = stream_num.str();

    if (mkdir(("/tmp/mediapipe_landmarks/"+std::string(hrminsec_current_run)+ '/' +str_stream_num).c_str(), 0777) == -1){
      // std::cout << "Unable to make folder...\n";
      // std::cout << "/tmp/mediapipe_landmarks/"+std::string(hrminsec_current_run)+ '/' +str_stream_num << std::endl;
    }
    std::ofstream file("/tmp/mediapipe_landmarks/"+ std::string(hrminsec_current_run) + '/' + str_stream_num + '/' +str_hr_min_sec_start+"_frame_"+str_frame_count+".json");
    // std::ofstream file("/tmp/mediapipe_landmarks/"+ std::string(hrminsec_current_run) + '/' + str_stream_num + '/' +str_hr_min_sec_start+"_frame_"+str_frame_count+ "_stream_" + str_stream_num + ".json");
    file << json_person_output;
    return mediapipe::OkStatus();
  }));



  MP_RETURN_IF_ERROR(
    graph.ObserveOutputStream(kFaceLandmarksStream, [&graph](const mediapipe::Packet& packet) -> ::mediapipe::Status {
    auto facelandmarksListVec = packet.Get<std::vector<mediapipe::NormalizedLandmarkList>>();
    // LOG(INFO) << "NormalizedLandmarkList.size(): " << landmarksListVec.size();
    

    for (int lmIdx = 0; lmIdx < facelandmarksListVec.size(); ++lmIdx) {
      face_landmarks_vec_output.clear();
      const auto& facelandmarksList = facelandmarksListVec[lmIdx];
      for (int i = 0; i < facelandmarksList.landmark_size(); ++i) {
        // std::cout << "Face Landmarks: " << i 
        // << " with x: " << facelandmarksList.landmark(i).x() 
        // << " with y: " << facelandmarksList.landmark(i).y() 
        // << " with z: " << facelandmarksList.landmark(i).z() << "\n";
        std::vector<double> face_xyz;
        face_xyz.push_back(facelandmarksList.landmark(i).x());
        face_xyz.push_back(facelandmarksList.landmark(i).y());
        face_xyz.push_back(facelandmarksList.landmark(i).z());
        face_landmarks_vec_output.push_back(face_xyz);
       }
    }
    return mediapipe::OkStatus();
  }));


  ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller_detections,
                   graph.AddOutputStreamPoller(kDetectionsStream)); // TODO check syntax

  MP_RETURN_IF_ERROR(graph.StartRun({}));

  LOG(INFO) << "Start grabbing and processing frames.";
  bool grab_frames = true;
  while (grab_frames) {
    frame_count++;
    if (frame_count >= 30)
    {
      frame_count = 0;
    }
    // std::cout << frame_count << std::endl;

    auto current_start = std::chrono::system_clock::now();
    std::time_t current_time_start = std::chrono::system_clock::to_time_t(current_start);

    tm* curr_tm_start;
    curr_tm_start = localtime(&current_time_start);
    std::strftime(hrminsec_start, 50, "%T", curr_tm_start);


    LOG(INFO) << "grab_frames is true";
    // Capture opencv camera or video frame.
    cv::Mat camera_frame_raw;
    capture >> camera_frame_raw;
    if (camera_frame_raw.empty()) {
      if (!load_video) {
        LOG(INFO) << "Ignore empty frames from camera.";
        continue;
      }
      LOG(INFO) << "Empty frame, end of video reached.";
      break;
    }
    cv::Mat camera_frame;
    cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
    if (!load_video) {
      cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);
    }

    // Wrap Mat into an ImageFrame.
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
        mediapipe::ImageFrame::kDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    camera_frame.copyTo(input_frame_mat);

    // Send image packet into the graph.
    size_t frame_timestamp_us =
        (double)cv::getTickCount() / (double)cv::getTickFrequency() * 1e6;
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
        kInputStream, mediapipe::Adopt(input_frame.release())
                          .At(mediapipe::Timestamp(frame_timestamp_us))));

    // Get the graph result packet, or stop if that fails.
    mediapipe::Packet packet;
    mediapipe::Packet landmarks_packet;
    mediapipe::Packet detections_packet;

    if (!poller.Next(&packet)) break;
    auto& output_frame = packet.Get<mediapipe::ImageFrame>();


    // LOG(INFO) << "\n\n\n";
    // LOG(INFO) << "111 About to get landmarks packet";
    // if (!poller_landmarks.Next(&landmarks_packet)) break; // hang in Next
    // LOG(INFO) << "222 Finished getting landmarks packet";
    // auto& output_landmarks = landmarks_packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();
    // LOG(INFO) << "output_landmarks: " << output_landmarks;
    // for (const auto &normalizedlandmarkList : output_landmarks){
    //   for (const auto &m_landmark : normalizedlandmarkList){
    //     LOG(INFO) << "x coordinate: " << m_landmark.x(); 
    //     LOG(INFO) << "y coordinate: " << m_landmark.y(); 
    //     LOG(INFO) << "z coordinate: " << m_landmark.z();
    //   }
    // }

    // LOG(INFO) << "\n\n\n";
    // LOG(INFO) << "111 About to get detections packet";
    if (!poller_detections.Next(&detections_packet)) break; // hang in Next
    // LOG(INFO) << "222 Finished getting detections packet";
    auto& output_detections = detections_packet.Get<std::vector<::mediapipe::Detection>>();
    for (const ::mediapipe::Detection& detection : output_detections) {
        // std::string detection_string = detection.DebugString();
        // std::cout << detection_string.find("xmin") << std::endl;
        // std::cout << detection_string.find("ymin") << std::endl;
        // std::cout << detection_string;
        const auto& location_data = detection.location_data();
        detections_vec_output.clear();
        detections_vec_output.push_back(location_data.relative_bounding_box().xmin());
        detections_vec_output.push_back(location_data.relative_bounding_box().ymin());
        detections_vec_output.push_back(location_data.relative_bounding_box().width());
        detections_vec_output.push_back(location_data.relative_bounding_box().height());
        // std::cout << "xmin: " << location_data.relative_bounding_box().xmin() << std::endl;
        // std::cout << "ymin: " << location_data.relative_bounding_box().ymin() << std::endl;
        // std::cout << "width: " << location_data.relative_bounding_box().width() << std::endl;
        // std::cout << "height: " << location_data.relative_bounding_box().height() << std::endl;
    }


    
    // Convert back to opencv for display or saving.
    cv::Mat output_frame_mat = mediapipe::formats::MatView(&output_frame);
    // LOG(INFO) << "output_frame_mat: " << output_frame_mat;
    cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);
    if (save_video) {
      if (!writer.isOpened()) {
        LOG(INFO) << "Prepare video writer.";
        writer.open(absl::GetFlag(FLAGS_output_video_path),
                    mediapipe::fourcc('a', 'v', 'c', '1'),  // .mp4
                    capture.get(cv::CAP_PROP_FPS), output_frame_mat.size());
        RET_CHECK(writer.isOpened());
      }
      writer.write(output_frame_mat);
    } else {
      /*
      FIXME: disable the opencv frame Display, need add FLAG to control the frame display
        cv::imshow(kWindowName, output_frame_mat);
      */
      // Press any key to exit.
      const int pressed_key = cv::waitKey(5);
      if (pressed_key >= 0 && pressed_key != 255) grab_frames = false;
    }



  }

  LOG(INFO) << "Shutting down.";
  if (writer.isOpened()) writer.release();
  MP_RETURN_IF_ERROR(graph.CloseInputStream(kInputStream));
  return graph.WaitUntilDone();
}

int main(int argc, char** argv) {
  // std::freopen("/home/ziheng/workspace/Github/mediapipe/landmarks_and_detections.txt", "w", stdout);
  // std::cout << "Root Path is " << std::filesystem::current_path().root_path() << "\n";
  // std::freopen("/tmp/mediapipe_landmarks/landmarks_and_detections.txt", "w", stdout);
  google::InitGoogleLogging(argv[0]);
  absl::ParseCommandLine(argc, argv);
  // run_grpc_server();

  auto current_run = std::chrono::system_clock::now();
  std::time_t current_time_run = std::chrono::system_clock::to_time_t(current_run);

  tm* curr_tm_run;
  // char hrminsec[100];
  curr_tm_run = localtime(&current_time_run);
  std::strftime(hrminsec_current_run, 50, "%T", curr_tm_run);

  if (mkdir(("/tmp/mediapipe_landmarks/"+std::string(hrminsec_current_run)).c_str(), 0777) == -1){
    // std::cout << "Unable to make folder...\n";
    // std::cout << "/tmp/mediapipe_landmarks/"+std::string(hrminsec_current_run) << std::endl;
  }



  absl::Status run_status = RunMPPGraph();
  // std::cout << json_output.dump() << std::endl;
  if (!run_status.ok()) {
    LOG(ERROR) << "Failed to run the graph: " << run_status.message();
    return EXIT_FAILURE;
  } else {
    LOG(INFO) << "Success!";
  }
  return EXIT_SUCCESS;
}
