idf_component_register(SRC_DIRS "source"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_spi
                                     esp_driver_gpio
                                     bsp)
