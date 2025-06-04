#include "ft8xx_gpu.h"
#include "wifi_3.h"

ft8xx_bitmap_info wifi_3_info = {
    .width = 16,
    .height = 12,
    .stride = 32,
    .size = 384,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/wifi_3.bin",
};
