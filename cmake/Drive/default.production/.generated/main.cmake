include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Drive_default_library_list )

# Handle files with suffix s, for group default-XC16
if(Drive_default_default_XC16_FILE_TYPE_assemble)
add_library(Drive_default_default_XC16_assemble OBJECT ${Drive_default_default_XC16_FILE_TYPE_assemble})
    Drive_default_default_XC16_assemble_rule(Drive_default_default_XC16_assemble)
    list(APPEND Drive_default_library_list "$<TARGET_OBJECTS:Drive_default_default_XC16_assemble>")

endif()

# Handle files with suffix S, for group default-XC16
if(Drive_default_default_XC16_FILE_TYPE_assemblePreproc)
add_library(Drive_default_default_XC16_assemblePreproc OBJECT ${Drive_default_default_XC16_FILE_TYPE_assemblePreproc})
    Drive_default_default_XC16_assemblePreproc_rule(Drive_default_default_XC16_assemblePreproc)
    list(APPEND Drive_default_library_list "$<TARGET_OBJECTS:Drive_default_default_XC16_assemblePreproc>")

endif()

# Handle files with suffix c, for group default-XC16
if(Drive_default_default_XC16_FILE_TYPE_compile)
add_library(Drive_default_default_XC16_compile OBJECT ${Drive_default_default_XC16_FILE_TYPE_compile})
    Drive_default_default_XC16_compile_rule(Drive_default_default_XC16_compile)
    list(APPEND Drive_default_library_list "$<TARGET_OBJECTS:Drive_default_default_XC16_compile>")

endif()

# Handle files with suffix s, for group default-XC16
if(Drive_default_default_XC16_FILE_TYPE_dependentObject)
add_library(Drive_default_default_XC16_dependentObject OBJECT ${Drive_default_default_XC16_FILE_TYPE_dependentObject})
    Drive_default_default_XC16_dependentObject_rule(Drive_default_default_XC16_dependentObject)
    list(APPEND Drive_default_library_list "$<TARGET_OBJECTS:Drive_default_default_XC16_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC16
if(Drive_default_default_XC16_FILE_TYPE_bin2hex)
add_library(Drive_default_default_XC16_bin2hex OBJECT ${Drive_default_default_XC16_FILE_TYPE_bin2hex})
    Drive_default_default_XC16_bin2hex_rule(Drive_default_default_XC16_bin2hex)
    list(APPEND Drive_default_library_list "$<TARGET_OBJECTS:Drive_default_default_XC16_bin2hex>")

endif()


# Main target for this project
add_executable(Drive_default_image_YqUtQ9dd ${Drive_default_library_list})

set_target_properties(Drive_default_image_YqUtQ9dd PROPERTIES
    OUTPUT_NAME "default-production"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${Drive_default_output_dir}")
target_link_libraries(Drive_default_image_YqUtQ9dd PRIVATE ${Drive_default_default_XC16_FILE_TYPE_link})

# Add the link options from the rule file.
Drive_default_link_rule( Drive_default_image_YqUtQ9dd)

# Call bin2hex function from the rule file
Drive_default_bin2hex_rule(Drive_default_image_YqUtQ9dd)

