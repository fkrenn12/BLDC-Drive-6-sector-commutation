# The following functions contains all the flags passed to the different build stages.

set(PACK_REPO_PATH "C:/Users/fkren/.mchp_packs" CACHE PATH "Path to the root of a pack repository.")

function(Drive_default_default_XC16_assemble_rule target)
    set(options
        "-g"
        "-mcpu=33CK64MC105"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(Drive_default_default_XC16_assemblePreproc_rule target)
    set(options
        "-x"
        "assembler-with-cpp"
        "-g"
        "-mcpu=33CK64MC105"
        "-Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-g,--no-relax"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=1"
        PRIVATE "XPRJ_default=default")
endfunction()
function(Drive_default_default_XC16_compile_rule target)
    set(options
        "-g"
        "-mcpu=33CK64MC105"
        "-ffunction-sections"
        "-fno-short-double"
        "-O0"
        "-msmart-io=1"
        "-Wall"
        "-msfr-warn=off"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG"
        PRIVATE "XPRJ_default=default")
endfunction()
function(Drive_default_dependentObject_rule target)
    set(options
        "-c"
        "-mcpu=33CK64MC105"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16")
    list(REMOVE_ITEM options "")
    target_compile_options(${target} PRIVATE "${options}")
endfunction()
function(Drive_default_link_rule target)
    set(options
        "-g"
        "-mcpu=33CK64MC105"
        "-mreserve=data@0x1000:0x101b"
        "-mreserve=data@0x101c:0x101d"
        "-mreserve=data@0x101e:0x101f"
        "-mreserve=data@0x1020:0x1021"
        "-mreserve=data@0x1022:0x1023"
        "-mreserve=data@0x1024:0x1027"
        "-mreserve=data@0x1028:0x104f"
        "-Wl,--script=p33CK64MC105.gld,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--heap=0,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,memoryfile.xml"
        "-mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16")
    list(REMOVE_ITEM options "")
    target_link_options(${target} PRIVATE "${options}")
    target_compile_definitions(${target}
        PRIVATE "__DEBUG=__DEBUG"
        PRIVATE "XPRJ_default=default")
endfunction()
function(Drive_default_bin2hex_rule target)
    add_custom_target(
        Drive_default_Bin2Hex ALL
        COMMAND ${MP_BIN2HEX} ${Drive_default_image_name} -a -mdfp=${PACK_REPO_PATH}/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16
        WORKING_DIRECTORY ${Drive_default_output_dir}
        BYPRODUCTS "${Drive_default_output_dir}/${Drive_default_image_base_name}.hex"
        COMMENT "Convert build file to .hex")
    add_dependencies(Drive_default_Bin2Hex ${target})
endfunction()
