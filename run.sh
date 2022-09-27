#!/bin/bash
source /home/ziheng/workspace/Github/mediapipe/kares.cfg
# conda activate actdet
# mkdir $mediapipe_landmarks_path

i=1

# timestamp(){
# 	date + "%T" # current time
# }

run_single_stream(){
	# # mkdir $i
	# start_time = "$(date -u +%s)"
	# echo "start_time: $start_time"
	# # GLOG_logtostderr=0 bazel-bin/mediapipe/examples/desktop/object_tracking/object_tracking_cpu   --calculator_graph_config_file=mediapipe/graphs/tracking/object_detection_tracking_desktop_live.pbtxt --input_video_path=videos/KidsYoga_1080P_30FPS.mp4&
	# end_time = "$(date -u +%s)"
	# echo "end_time: $end_time"
	# total_time = "$(($end_time-$start_time))"
	# echo "total_time: $total_time seconds"

	# mkdir $i
	local start=`date +%s`
	local start_time=$SECONDS
	echo "Task $i with start_time: $start"
	stream_url=""
	if [[ $i -eq 1 ]]
	then
		stream_url="rtsp://admin:Activity@Kares@10.0.1.201:554/cam/realmonitor?channel=1&subtype=1"
	else
		stream_url="rtsp://admin:Activity@Kares@10.0.1.202:554/cam/realmonitor?channel=1&subtype=1"
	fi

	if [[ $use_input_video -eq 1 ]]
	then
		GLOG_logtostderr=0 $mediapipe_bin_path  --calculator_graph_config_file=$mediapipe_graph_config_path --input_video_path=$stream_url --stream_number=$i&
	else
		GLOG_logtostderr=0 $mediapipe_bin_path  --calculator_graph_config_file=$mediapipe_graph_config_path --stream_number=$i&
	fi
	python3 $actdet_path/mediapipe_service.py --who Ziheng --stream_number $i
	local end_time=$SECONDS
	echo "Task $i with end_time: $end_time"
	local total_time=$(( end_time - start_time))
	echo "Task $i with total_time: $total_time seconds"
}


while [ $i -le $total_stream_number ]
do
	run_single_stream&
	i=$(( $i + 1))
done

