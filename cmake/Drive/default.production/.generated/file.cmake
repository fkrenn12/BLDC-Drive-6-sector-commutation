# The following variables contains the files used by the different stages of the build process.
set(Drive_default_default_XC16_FILE_TYPE_assemble
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/dmt_asm.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/where_was_i.s")
set_source_files_properties(${Drive_default_default_XC16_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Drive_default_default_XC16_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Drive_default_default_XC16_FILE_TYPE_assemblePreproc)
set_source_files_properties(${Drive_default_default_XC16_FILE_TYPE_assemblePreproc} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Drive_default_default_XC16_FILE_TYPE_assemblePreproc})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Drive_default_default_XC16_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/adc-current-measurement-and-control.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/debug.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/drive.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/fletuino/fletuino.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/fletuino/json/json_decoder.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/fletuino/json/lexer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/fletuino/json/mem_model.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/fletuino/json/parser.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/global.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/gui.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/lib/fixed_point.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/lib/pi-controller.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/lib/ramp.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/lib/ringbuffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/adc/src/adc1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/clock.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/config_bits.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/dmt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/interrupt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/pins.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/reset.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/system.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/mcc_generated_files/system/src/traps.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/peripheral/pwm.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/peripheral/timer1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/peripheral/uart1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/peripheral/uart2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/pwm-sector-detection-commutation-speed-measurement.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Drive/mcc/serial-command-interpreter.c")
set_source_files_properties(${Drive_default_default_XC16_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Drive_default_default_XC16_FILE_TYPE_link)
set(Drive_default_default_XC16_FILE_TYPE_bin2hex)
set(Drive_default_image_name "default-production.elf")
set(Drive_default_image_base_name "default-production")

# The output directory of the final image.
set(Drive_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Drive/production")

# The full path to the final image.
set(Drive_default_full_path_to_image ${Drive_default_output_dir}/${Drive_default_image_name})
