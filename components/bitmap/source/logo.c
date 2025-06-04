#include "ft8xx_gpu.h"
#include "logo.h"

ft8xx_bitmap_info logo_info = {
    .width = 280,
    .height = 85,
    .stride = 560,
    .size = 47600,
    .format = ARGB4,
    .compressed = true,
    .file_path = "/littlefs/logo.bin",
};
