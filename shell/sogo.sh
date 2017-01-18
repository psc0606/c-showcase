#!/bin/sh
#搜狗输入重启
pidof fcitx | xargs kill
pidof sogou-qimpanel | xargs kill
nohup fcitx  1>/dev/null 2>/dev/null &
nohup sogou-qimpanel  1>/dev/null 2>/dev/null &
