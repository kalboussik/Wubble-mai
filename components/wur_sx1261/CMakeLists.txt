idf_component_register(SRCS "wur_sx1261.c" "sx126x.c" "sx126x_lr_fhss.c" "ra01s.c" "lr_fhss_mac.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver esp_timer)
