#!/bin/bash
g++ image-effect.cpp -o ImageEffect -std=c++11  `pkg-config --cflags --libs opencv4` -lpthread
./ImageEffect 720bg.jpg 720fg.jpg 720out_1.jpg 1
./ImageEffect 720bg.jpg 720fg.jpg 720out_2.jpg 2
./ImageEffect 720bg.jpg 720fg.jpg 720out_4.jpg 4
./ImageEffect 720bg.jpg 720fg.jpg 720out_8.jpg 8
./ImageEffect 720bg.jpg 720fg.jpg 720out_16.jpg 16

./ImageEffect 1080bg.jpg 1080fg.jpg 1080out_1.jpg 1
./ImageEffect 1080bg.jpg 1080fg.jpg 1080out_2.jpg 2
./ImageEffect 1080bg.jpg 1080fg.jpg 1080out_4.jpg 4
./ImageEffect 1080bg.jpg 1080fg.jpg 1080out_8.jpg 8
./ImageEffect 1080bg.jpg 1080fg.jpg 1080out_16.jpg 16

./ImageEffect 4kbg.jpg 4kfg.jpg 4kout_1.jpg 1
./ImageEffect 4kbg.jpg 4kfg.jpg 4kout_2.jpg 2
./ImageEffect 4kbg.jpg 4kfg.jpg 4kout_4.jpg 4
./ImageEffect 4kbg.jpg 4kfg.jpg 4kout_8.jpg 8
./ImageEffect 4kbg.jpg 4kfg.jpg 4kout_16.jpg 16
