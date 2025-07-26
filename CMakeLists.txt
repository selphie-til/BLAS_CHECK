cmake_minimum_required(VERSION 3.5)

# AOCCコンパイラの設定
set(CMAKE_CXX_COMPILER /opt/amd/aocc-compiler-5.0.0/bin/clang++)

project(BLAS_CHECK)

set(CMAKE_CXX_STANDARD 20)

# BLASライブラリの選択オプション（デフォルトはBLIS）
set(BLAS_LIBRARY "BLIS" CACHE STRING "Choose BLAS library: BLIS or OpenBLAS")
set_property(CACHE BLAS_LIBRARY PROPERTY STRINGS "BLIS" "OpenBLAS")

# PkgConfigモジュールを見つける
find_package(PkgConfig REQUIRED)

# 実行ファイルの設定
add_executable(BLAS_CHECK main.cpp)

# ライブラリ選択に応じた設定
if(BLAS_LIBRARY STREQUAL "BLIS")
    message(STATUS "Using BLIS library")
    pkg_check_modules(BLAS_LIB REQUIRED blis)
    target_compile_definitions(BLAS_CHECK PRIVATE USE_BLIS)
    
elseif(BLAS_LIBRARY STREQUAL "OpenBLAS")
    message(STATUS "Using OpenBLAS library")
    pkg_check_modules(BLAS_LIB REQUIRED openblas)
    target_compile_definitions(BLAS_CHECK PRIVATE USE_OPENBLAS)
    
else()
    message(FATAL_ERROR "Invalid BLAS_LIBRARY option: ${BLAS_LIBRARY}. Choose BLAS or OpenBLAS")
endif()

# 選択されたBLASライブラリの設定を適用
target_include_directories(BLAS_CHECK PRIVATE ${BLAS_LIB_INCLUDE_DIRS})
target_compile_options(BLAS_CHECK PRIVATE ${BLAS_LIB_CFLAGS_OTHER})
target_link_libraries(BLAS_CHECK PRIVATE ${BLAS_LIB_LIBRARIES})
target_link_directories(BLAS_CHECK PRIVATE ${BLAS_LIB_LIBRARY_DIRS})

# libc++を明示的にリンク（-stdlib=libc++を使用する場合）
target_link_libraries(BLAS_CHECK PRIVATE c++)

# コンパイラフラグの設定
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(BLAS_CHECK PRIVATE -O3)
endif()
