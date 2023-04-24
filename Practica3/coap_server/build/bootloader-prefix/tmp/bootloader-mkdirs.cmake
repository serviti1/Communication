# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ESP32_Repository/esp-idf/components/bootloader/subproject"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/tmp"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/src/bootloader-stamp"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/src"
  "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/ITESO/Sistemas de Comunicaciones/Esp32/coap_server/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
