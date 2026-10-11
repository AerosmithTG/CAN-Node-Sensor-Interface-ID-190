idf_component_register(SRCS "main.cpp" "framework.cpp" "adc.cpp"
                            "../ADC_task.cpp" "../CAN_task.cpp"
                            "MSM_CAN/MSM_CAN.cpp"
                       INCLUDE_DIRS "." "MSM_CAN"
                       PRIV_REQUIRES esp_driver_gpio esp_driver_spi esp_driver_twai esp_timer freertos)
