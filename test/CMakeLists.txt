# 変数設定 gtest, opencvのパス設定
set(GTEST_ROOT /Users/yhiro/workbench/googletest)
set(OPENCV_ROOT /usr/local/Cellar/opencv/4.1.0_2)
# C++17の設定
set(CMAKE_CXX_STANDARD 17)
# includeパスの設定
include_directories(
    ${GTEST_ROOT}/googletest/include
    ${OPENCV_ROOT}/include/opencv4
    )
# 全てのcppファイルを拾う
FILE(GLOB SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)

if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)

# ライブラリパスの指定
link_directories(${OPENCV_ROOT}/lib)

# test_pmsというバイナリに出力する
add_executable(test_pms ${SOURCES})
# リンカ設定
target_link_libraries(test_pms
    pthread
    ${GTEST_ROOT}/lib/libgtest.a
    ${GTEST_ROOT}/lib/libgtest_main.a
    opencv_core
    opencv_imgcodecs
    photometric_stereo
    )