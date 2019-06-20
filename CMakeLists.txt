# cmake version設定
cmake_minimum_required(VERSION 3.14)
# プロジェクト名の設定
project(PHOTOMETRIC_STEREO)
# サブディレクトリの読み取り
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(main)