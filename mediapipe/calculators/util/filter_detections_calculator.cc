// Copyright 2021 The MediaPipe Authors.
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

#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "absl/memory/memory.h"
#include "mediapipe/calculators/util/filter_detections_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

const char kInputDetectionsTag[] = "INPUT_DETECTIONS";
const char kOutputDetectionsTag[] = "OUTPUT_DETECTIONS";

//
// Calculator to filter out detections that do not meet the criteria specified
// in options.
//
class FilterDetectionsCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag(kInputDetectionsTag));
    RET_CHECK(cc->Outputs().HasTag(kOutputDetectionsTag));

    cc->Inputs().Tag(kInputDetectionsTag).Set<std::vector<Detection>>();
    cc->Outputs().Tag(kOutputDetectionsTag).Set<std::vector<Detection>>();

    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(TimestampDiff(0));
    options_ = cc->Options<mediapipe::FilterDetectionsCalculatorOptions>();

    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) final {
    const auto& input_detections =
        cc->Inputs().Tag(kInputDetectionsTag).Get<std::vector<Detection>>();

    // LOG(INFO) << "\n"; // mod
    // LOG(INFO) << "[Detections size to FilterDetection: " << input_detections.size(); // mod
    auto output_detections = absl::make_unique<std::vector<Detection>>();
    int count = 0;
    for (const Detection& detection : input_detections) {
      RET_CHECK_GT(detection.score_size(), 0);
      // Note: only score at index 0 supported.
      // LOG(INFO) << "Start Filtering";
      // LOG(INFO) << "label detected: " << detection.label(0);
      if (detection.label(0) == options_.class_id() && detection.score(0) >= options_.min_score()) {
        // LOG(INFO) << "Valid Detection";
        output_detections->push_back(detection);
        count++;
        if (count == options_.max_num()){break;}
      }
    }
    // LOG(INFO) << "Finish Filtering";
    cc->Outputs()
        .Tag(kOutputDetectionsTag)
        .Add(output_detections.release(), cc->InputTimestamp());
    // LOG(INFO) << "Detections size from FilterDetection: " << count << "]"; // mod
    return absl::OkStatus();
  }

 private:
  mediapipe::FilterDetectionsCalculatorOptions options_;
};

REGISTER_CALCULATOR(FilterDetectionsCalculator);

}  // namespace mediapipe
