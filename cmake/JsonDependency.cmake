# Download source into the build cache; never execute downloaded code.
set(VANISHER_JSON_INCLUDE_DIR "${CMAKE_BINARY_DIR}/dependencies")
set(VANISHER_JSON_HEADER "${VANISHER_JSON_INCLUDE_DIR}/nlohmann/json.hpp")
set(VANISHER_JSON_HASH "aaf127c04cb31c406e5b04a63f1ae89369fccde6d8fa7cdda1ed4f32dfc5de63")
set(VANISHER_JSON_CACHED FALSE)
if(EXISTS "${VANISHER_JSON_HEADER}")
    file(SHA256 "${VANISHER_JSON_HEADER}" VANISHER_JSON_CACHED_HASH)
    if(VANISHER_JSON_CACHED_HASH STREQUAL VANISHER_JSON_HASH)
        set(VANISHER_JSON_CACHED TRUE)
    endif()
endif()

if(NOT VANISHER_JSON_CACHED)
    file(MAKE_DIRECTORY "${VANISHER_JSON_INCLUDE_DIR}/nlohmann")
    set(VANISHER_JSON_TEMP "${VANISHER_JSON_HEADER}.download")
    message(STATUS "Downloading the pinned JSON source header")
    file(DOWNLOAD
        "https://raw.githubusercontent.com/nlohmann/json/v3.12.0/single_include/nlohmann/json.hpp"
        "${VANISHER_JSON_TEMP}"
        TLS_VERIFY ON TIMEOUT 60 INACTIVITY_TIMEOUT 20
        STATUS VANISHER_JSON_DOWNLOAD_STATUS
    )
    list(GET VANISHER_JSON_DOWNLOAD_STATUS 0 VANISHER_JSON_DOWNLOAD_CODE)
    if(NOT VANISHER_JSON_DOWNLOAD_CODE EQUAL 0)
        # Some Windows TLS environments require Git's OpenSSL backend.
        # Git is optional when the direct HTTPS download works.
        find_package(Git QUIET)
        if(NOT GIT_FOUND)
            message(FATAL_ERROR "Secure JSON download failed. Check internet access, or install Git for Windows to enable the secure fallback. Certificate checks remain enabled.")
        endif()
        message(STATUS "Using Git with verified HTTPS as the source-download fallback")
        set(VANISHER_JSON_REPOSITORY "${CMAKE_BINARY_DIR}/json-source")
        set(VANISHER_JSON_GIT_COMMAND "${CMAKE_COMMAND}" -E env GIT_TERMINAL_PROMPT=0 GCM_INTERACTIVE=Never
            "${GIT_EXECUTABLE}" -c http.sslBackend=openssl -c http.sslVerify=true -c core.hooksPath=NUL)
        if(NOT EXISTS "${VANISHER_JSON_REPOSITORY}/.git")
            execute_process(COMMAND ${VANISHER_JSON_GIT_COMMAND} clone
                --filter=blob:none --no-checkout --depth 1 --branch v3.12.0
                "https://github.com/nlohmann/json.git" "${VANISHER_JSON_REPOSITORY}"
                RESULT_VARIABLE VANISHER_JSON_GIT_RESULT OUTPUT_QUIET ERROR_VARIABLE VANISHER_JSON_GIT_ERROR TIMEOUT 60)
            if(NOT VANISHER_JSON_GIT_RESULT EQUAL 0)
                message(FATAL_ERROR "Could not fetch JSON source securely. Check internet access to github.com. ${VANISHER_JSON_GIT_ERROR}")
            endif()
        endif()
        execute_process(COMMAND ${VANISHER_JSON_GIT_COMMAND} -C "${VANISHER_JSON_REPOSITORY}"
            show HEAD:single_include/nlohmann/json.hpp
            OUTPUT_FILE "${VANISHER_JSON_TEMP}" RESULT_VARIABLE VANISHER_JSON_GIT_RESULT
            ERROR_VARIABLE VANISHER_JSON_GIT_ERROR TIMEOUT 60)
        if(NOT VANISHER_JSON_GIT_RESULT EQUAL 0)
            message(FATAL_ERROR "Could not read the pinned JSON header. ${VANISHER_JSON_GIT_ERROR}")
        endif()
    endif()
    # Both download methods must produce the exact reviewed header before use.
    file(SHA256 "${VANISHER_JSON_TEMP}" VANISHER_JSON_DOWNLOADED_HASH)
    if(NOT VANISHER_JSON_DOWNLOADED_HASH STREQUAL VANISHER_JSON_HASH)
        message(FATAL_ERROR "JSON source SHA256 verification failed. The downloaded header will not be used.")
    endif()
    file(RENAME "${VANISHER_JSON_TEMP}" "${VANISHER_JSON_HEADER}")
    message(STATUS "JSON source SHA256 verified")
endif()
