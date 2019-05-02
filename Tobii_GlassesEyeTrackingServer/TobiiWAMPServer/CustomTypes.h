#pragma once

#include <array>
#include <map>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/locale/util.hpp>
#include <boost/foreach.hpp>


using boost::asio::ip::udp;
using boost::property_tree::ptree;
using namespace std;

struct TobiiControlUnitInfo {
	ptree discoveryResponse;
	udp::endpoint info;
};

struct TobiiEyeSample {
	long gidx;
	int status; //0 means OK, different values mean not OK
	long timestamp;
	int left_confidence;
	int right_confidence;
	std::array<double, 3> left_pupil_center;
	std::array<double, 3> right_pupil_center;
	double left_pupil_diameter;
	double right_pupil_diameter;
	std::array<double, 3> left_gaze_direction;
	std::array<double, 3> right_gaze_direction;
	std::array<double, 2> gaze_position; //normalized gaze position, (0, 0) at top left corner, (1, 1) at bottom right corner
	std::array<double, 3> gaze_position3d;
	TobiiEyeSample() {
		left_confidence = 1;
		right_confidence = 1;

		left_pupil_center[0] = -1;
		left_pupil_center[1] = -1;
		left_pupil_center[2] = -1;

		right_pupil_center[0] = -1;
		right_pupil_center[1] = -1;
		right_pupil_center[2] = -1;

		left_gaze_direction[0] = -1;
		left_gaze_direction[1] = -1;
		left_gaze_direction[2] = -1;

		right_gaze_direction[0] = -1;
		right_gaze_direction[1] = -1;
		right_gaze_direction[2] = -1;

		gaze_position3d[0] = -1;
		gaze_position3d[1] = -1;
		gaze_position3d[2] = -1;

		gaze_position[0] = -1;
		gaze_position[1] = -1;

		gidx = -1;
	}
};