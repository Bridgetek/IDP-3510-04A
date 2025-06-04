#include "ft8xx_gpu.h"
#include "about.h"

ft8xx_bitmap_info about_info = {
    .width = 23,
    .height = 23,
    .stride = 46,
    .size = 1058,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/about.bin",
};
