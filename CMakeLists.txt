cmake_minimum_required(VERSION 3.6)

set(APP_NAME Trapcore)

project(${APP_NAME})

# Убедимся, что путь к Cocos2d правильно установлен
if(NOT DEFINED BUILD_ENGINE_DONE)  # to test Trapcore into root project
    set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# Использование GLOB для автоматического добавления всех .cpp и .h файлов
file(GLOB_RECURSE GAME_SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Classes/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Resources/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Scripts/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/*.cpp"
)
file(GLOB_RECURSE GAME_HEADER 
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Classes/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Resources/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Scripts/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/*.h"
)

# Ручное добавление обязательных файлов (например, AppDelegate.cpp и main.cpp)
list(APPEND GAME_SOURCE
    proj.win32/Classes/AppDelegate.cpp
    proj.win32/main.cpp
)

# Добавление ресурсов (например, картинок, шрифтов)
set(GAME_RES_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Resources")
file(GLOB GAME_RES_FILES 
    "${GAME_RES_FOLDER}/*.png"
    "${GAME_RES_FOLDER}/*.jpg"
    "${GAME_RES_FOLDER}/*.ttf"
    "${GAME_RES_FOLDER}/fonts/*.ttf"
    "${GAME_RES_FOLDER}/*.tmx"
    "${GAME_RES_FOLDER}/*.tsx"
    "${GAME_RES_FOLDER}/*.fnt"
)

# Добавление файлов из папки Scripts (если используются скрипты)
file(GLOB GAME_SCRIPTS 
    "${CMAKE_CURRENT_SOURCE_DIR}/proj.win32/Scripts/*.js"
)

# Для Windows добавляем файлы
if(WIN32)
    list(APPEND GAME_HEADER
        proj.win32/main.h
        proj.win32/resource.h
    )
endif()

# Сборка исполнимого файла для Windows
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)
add_executable(${APP_NAME} ${all_code_files})

# Связывание с библиотеками Cocos2d
target_link_libraries(${APP_NAME} cocos2d)

# Добавление путей для заголовочных файлов
target_include_directories(${APP_NAME}
    PRIVATE proj.win32/Classes
    PRIVATE proj.win32/Resources
    PRIVATE proj.win32/Scripts
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/

)

# Конфигурация приложения Cocos2d
setup_cocos_app_config(${APP_NAME})

# Копирование ресурсов для Windows
if(WIN32)
    cocos_copy_target_dll(${APP_NAME})
endif()

# Копирование ресурсов для всех платформ
if(UNIX OR WIN32)
    cocos_get_resource_path(APP_RES_DIR ${APP_NAME})
    cocos_copy_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()