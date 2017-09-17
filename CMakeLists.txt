cmake_minimum_required(VERSION 2.8.3)
project(stitcher)
 
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  std_msgs
  usb_cam
  message_generation
)
 
find_package(OpenCV REQUIRED)
 
 add_message_files(
   FILES
   camera_poi.msg

 )

 generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES finddistance valve videoOCR transf  findcolor digit_recog #getImage transfUNI
   CATKIN_DEPENDS message_runtime roscpp std_msgs

   DEPENDS system_lib
)
 
include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)
add_executable(testcam src/main.cpp)
add_dependencies(testcam testcam_generate_messages_cpp)

add_library(valve include/${PROJECT_NAME}/valve.h include/${PROJECT_NAME}/valve.cpp)
add_library(videoOCR include/${PROJECT_NAME}/videoOCR.h include/${PROJECT_NAME}/videoOCR.cpp)
add_library(finddistance include/${PROJECT_NAME}/finddistance.h include/${PROJECT_NAME}/finddistance.cpp)
#add_library(transfUNI include/${PROJECT_NAME}/transfUNI.h include/${PROJECT_NAME}/transfUNI.cpp)
add_library(transf include/${PROJECT_NAME}/transf.h include/${PROJECT_NAME}/transf.cpp)
#add_library(getImage include/${PROJECT_NAME}/getImage.h include/${PROJECT_NAME}/getImage.cpp)
add_library(findcolor include/${PROJECT_NAME}/findcolor.h include/${PROJECT_NAME}/findcolor.cpp)
add_library(digit_recog include/${PROJECT_NAME}/digit_recog.h include/${PROJECT_NAME}/digit_recog.cpp)


target_link_libraries(testcam valve finddistance videoOCR  transf  findcolor digit_recog #getImage transfUNI
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
)

