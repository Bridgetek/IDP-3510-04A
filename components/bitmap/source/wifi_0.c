#include "ft8xx_gpu.h"
#include "wifi_0.h"

ft8xx_bitmap_info wifi_0_info = {
    .width = 16,
    .height = 12,
    .stride = 32,
    .size = 384,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/wifi_0.bin",
};
