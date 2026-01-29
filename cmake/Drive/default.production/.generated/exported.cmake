set(DEPENDENT_MP_BIN2HEXDrive_default_YqUtQ9dd "c:/Program Files/Microchip/xc16/v2.10/bin/xc16-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_YqUtQ9dd "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Drive/production/default-production.elf")
set(DEPENDENT_TARGET_DIRDrive_default_YqUtQ9dd "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Drive/production")
set(DEPENDENT_BYPRODUCTSDrive_default_YqUtQ9dd ${DEPENDENT_TARGET_DIRDrive_default_YqUtQ9dd}/${sourceFileNameDrive_default_YqUtQ9dd}.s)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRDrive_default_YqUtQ9dd}/${sourceFileNameDrive_default_YqUtQ9dd}.s
    COMMAND ${DEPENDENT_MP_BIN2HEXDrive_default_YqUtQ9dd} ${DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_YqUtQ9dd} --image ${sourceFileNameDrive_default_YqUtQ9dd} ${addressDrive_default_YqUtQ9dd} ${modeDrive_default_YqUtQ9dd} -mdfp=C:/Users/fkren/.mchp_packs/Microchip/dsPIC33CK-MC_DFP/1.8.299/xc16 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRDrive_default_YqUtQ9dd}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFDrive_default_YqUtQ9dd})
add_custom_target(
    dependent_produced_source_artifactDrive_default_YqUtQ9dd 
    DEPENDS ${DEPENDENT_TARGET_DIRDrive_default_YqUtQ9dd}/${sourceFileNameDrive_default_YqUtQ9dd}.s
    )
set(loadable_varDrive_default_YqUtQ9dd "${CMAKE_CURRENT_LIST_DIR}/../../../../out/Drive/production/default-production.hex")
