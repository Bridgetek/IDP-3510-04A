#include "ft8xx_gpu.h"
#include "setting.h"

ft8xx_bitmap_info setting_info = {
    .width = 23,
    .height = 23,
    .stride = 46,
    .size = 1058,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/setting.bin",
};
