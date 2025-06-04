idf_component_register(SRC_DIRS "source"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES ft81x
                                     esp_driver_gpio
                                     esp_timer
                                     littlefs
                                     esp_wifi)
