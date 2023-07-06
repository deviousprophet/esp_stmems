idf_component_register(
    SRCS "esp_stmems.c"
    INCLUDE_DIRS "include"
    REQUIRES i2c_bus
)

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format" "-Wno-maybe-uninitialized" "-Wno-incompatible-pointer-types")
