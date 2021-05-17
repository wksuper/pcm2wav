project(pcm2wav)
cmake_minimum_required(VERSION 3.16)

add_executable(pcm2wav
        pcm2wav.c
)

add_executable(combine
        combine.c
)
