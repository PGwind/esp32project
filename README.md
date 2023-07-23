## 前言

此仓库用来记录学习 ESP32 期间的各种Demo，Demo非原创

ESP32代码记录：[ESP32基础教程 – Echo (liveout.cn)](https://www.liveout.cn/55-2/)



## 1. 智能(感应)垃圾桶

### 流程

超声波模块每隔200ms发出一次信号进行测距，如果测量到的物体距离在范围内，则信号为 `open_semaphore`。

舵机旋转打开盖子，板载灯变亮，串口打印相关信息。

当打开盖子时，记录打开时间，并启动计时器进行定时检测，即每隔500ms进行检测。

如果检测到盖子关闭时间超过了阈值，则重置打开时间，并设置二值信号量状态为关闭。

得到关闭 `close_semaphore`  信号后，舵机转动进行关盖。

### 相关链接

Demo教程视频：[ ESP32之智能垃圾桶制作讲解—哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1yG4y1g74j/?spm_id_from=333.337.top_right_bar_window_history.content.click&vd_source=a1234589a3616351986bc6d13bcbd8f8)

Demo文章详解：[ESP32Demo智能垃圾桶 – Echo (liveout.cn)](https://www.liveout.cn/56-2/)



## 2. 四足机器人

### 流程

通过WiFi连接到网络，并提供了一个简单的网页界面，通过点击 按钮来控制舵机的运动。通过访问特定的URL，用户可以控制四足机器人的前进、后退、左转、右转等动作。

### 相关链接

Demo文章详解：[60元成本打造esp8266四足机器人CSDN博客](https://blog.csdn.net/qq_66813359/article/details/130633027?spm=1001.2014.3001.5506)

Demo视频演示：[60元成本打造esp8266四足机器人_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1Fo4y147Uh/?spm_id_from=888.80997.embed_other.whitelist&t=41)