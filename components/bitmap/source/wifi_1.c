#include "ft8xx_gpu.h"
#include "wifi_1.h"

ft8xx_bitmap_info wifi_1_info = {
    .width = 16,
    .height = 12,
    .stride = 32,
    .size = 384,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/wifi_1.bin",
};
