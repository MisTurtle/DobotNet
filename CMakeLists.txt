cmake_minimum_required(VERSION 3.23)
project(DobotNet)

set(CMAKE_CXX_STANDARD 14)


add_executable(DobotNet src/RingBuffer.cpp src/RingBuffer.h src/ProtocolID.h src/ProtocolDef.h src/FlexiTimer2.h src/FlexiTimer2.cpp src/SerialInterface.h src/DobotInstance.cpp src/DobotInstance.h src/DobotNet.h src/DobotNet.cpp src/Protocol.cpp src/Protocol.h src/Message.h src/Message.cpp examples/SingleDemo.ino examples/NetworkDemo.ino)
