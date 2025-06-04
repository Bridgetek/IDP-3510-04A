#include "ft8xx_gpu.h"
#include "wifi_2.h"

ft8xx_bitmap_info wifi_2_info = {
    .width = 16,
    .height = 12,
    .stride = 32,
    .size = 384,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/wifi_2.bin",
};
