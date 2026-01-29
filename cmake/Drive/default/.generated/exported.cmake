set(DEPENDENT_MP_BIN2HEXDrive_default_DYIqDzYj "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_DYIqDzYj "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Drive/default.elf")
set(DEPENDENT_TARGET_DIRDrive_default_DYIqDzYj "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Drive")
set(DEPENDENT_BYPRODUCTSDrive_default_DYIqDzYj ${DEPENDENT_TARGET_DIRDrive_default_DYIqDzYj}/${sourceFileNameDrive_default_DYIqDzYj}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRDrive_default_DYIqDzYj}/${sourceFileNameDrive_default_DYIqDzYj}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXDrive_default_DYIqDzYj} ${DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_DYIqDzYj} --image ${sourceFileNameDrive_default_DYIqDzYj} ${addressDrive_default_DYIqDzYj} ${modeDrive_default_DYIqDzYj} -mdfp=C:/Users/fkren/.mchp_packs/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRDrive_default_DYIqDzYj}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_DYIqDzYj})
add_custom_target(
    dependent_produced_source_artifactDrive_default_DYIqDzYj 
    DEPENDS ${DEPENDENT_TARGET_DIRDrive_default_DYIqDzYj}/${sourceFileNameDrive_default_DYIqDzYj}.s
    )
