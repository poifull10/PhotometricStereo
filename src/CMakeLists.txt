# C++17適用
set(CMAKE_CXX_STANDARD 17)
# OpenCVのルートディレクトリの設定
set(OPENCV_ROOT /usr/local/Cellar/opencv/4.1.0_2)
set(GTEST_ROOT /Users/yhiro/workbench/googletest)
# cppを全て拾う
FILE(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# インクルードパスの設定
include_directories(
    ${GTEST_ROOT}/googletest/include
    ${OPENCV_ROOT}/include/opencv4
    )


# ライブラリパスの指定
link_directories(${OPENCV_ROOT}/lib)

    # 共有ライブラリとしてビルドする
add_library(photometric_stereo
    SHARED
    ${SOURCES}
  )

# 本ライブラリを使用するコードも同じincludeを参照する
target_include_directories(photometric_stereo
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )


# リンカ設定
target_link_libraries(photometric_stereo
    opencv_core
    )
    