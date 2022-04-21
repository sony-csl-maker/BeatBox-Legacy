/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   build_make;
    const int            build_makeSize = 16784;

    extern const char*   cmake_clean_cmake;
    const int            cmake_clean_cmakeSize = 935;

    extern const char*   cmake_clean_target_cmake;
    const int            cmake_clean_target_cmakeSize = 36;

    extern const char*   compiler_depend_make;
    const int            compiler_depend_makeSize = 107;

    extern const char*   compiler_depend_ts;
    const int            compiler_depend_tsSize = 111;

    extern const char*   depend_make;
    const int            depend_makeSize = 88;

    extern const char*   DependInfo_cmake;
    const int            DependInfo_cmakeSize = 1737;

    extern const char*   flags_make;
    const int            flags_makeSize = 312;

    extern const char*   link_txt;
    const int            link_txtSize = 377;

    extern const char*   progress_make;
    const int            progress_makeSize = 190;

    extern const char*   CMakeDirectoryInformation_cmake;
    const int            CMakeDirectoryInformation_cmakeSize = 646;

    extern const char*   progress_marks;
    const int            progress_marksSize = 2;

    extern const char*   cmake_install_cmake;
    const int            cmake_install_cmakeSize = 1253;

    extern const char*   CMakeLists_txt;
    const int            CMakeLists_txtSize = 588;

    extern const char*   libGist_a;
    const int            libGist_aSize = 638066;

    extern const char*   Makefile;
    const int            MakefileSize = 12260;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 16;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
