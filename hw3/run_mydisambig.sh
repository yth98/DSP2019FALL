#!/bin/bash

make
./mydisambig tmp/T_seg_example.txt ZhuYin-Big5.map U_bigram.lm ans/B_example.txt
./mydisambig tmp/T_seg_1.txt ZhuYin-Big5.map U_bigram.lm ans/B_1.txt
./mydisambig tmp/T_seg_2.txt ZhuYin-Big5.map U_bigram.lm ans/B_2.txt
./mydisambig tmp/T_seg_3.txt ZhuYin-Big5.map U_bigram.lm ans/B_3.txt
./mydisambig tmp/T_seg_4.txt ZhuYin-Big5.map U_bigram.lm ans/B_4.txt
./mydisambig tmp/T_seg_5.txt ZhuYin-Big5.map U_bigram.lm ans/B_5.txt
./mydisambig tmp/T_seg_6.txt ZhuYin-Big5.map U_bigram.lm ans/B_6.txt
./mydisambig tmp/T_seg_7.txt ZhuYin-Big5.map U_bigram.lm ans/B_7.txt
./mydisambig tmp/T_seg_8.txt ZhuYin-Big5.map U_bigram.lm ans/B_8.txt
./mydisambig tmp/T_seg_9.txt ZhuYin-Big5.map U_bigram.lm ans/B_9.txt
./mydisambig tmp/T_seg_10.txt ZhuYin-Big5.map U_bigram.lm ans/B_10.txt
