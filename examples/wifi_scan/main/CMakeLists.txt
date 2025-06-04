idf_component_register(SRCS "main.c" "ui.c"
                    INCLUDE_DIRS ".")
# Note: you must have a partition named the first argument (here it's "littlefs")
# in your partition table csv file.
littlefs_create_partition_image(storage ../../../assets FLASH_IN_PROJECT)
