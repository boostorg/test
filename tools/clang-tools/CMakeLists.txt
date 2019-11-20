# blablabla clang tool
cmake_minimum_required(VERSION 3.14)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

project(clang-parser)

find_package(Clang)

add_executable(clang_tool main.cpp)

target_link_libraries(clang_tool PRIVATE
    #clangHandleCXX
    #clangTooling
    #clangBasic
    #clangASTMatchers
    #clangFrontend
#
    ##clangDoc
    ##clangTooling
    ##clangTidy
    #
    #clangAST
#
    #clangFrontendTool
    #clangFrontend
    #clangDriver
    #clangSerialization
    #clangCodeGen
    #clangParse
    #clangSema
    #clangStaticAnalyzerFrontend
    #clangStaticAnalyzerCheckers
    #clangStaticAnalyzerCore
    #clangAnalysis
    #clangIndex
    #clangRewrite
    #clangLex
    #clangBasic
#
#
    clangTooling
    clangFrontendTool
    clangFrontend
    clangDriver
    clangSerialization
    clangCodeGen
    clangParse
    clangSema
    clangStaticAnalyzerFrontend
    clangStaticAnalyzerCheckers
    clangStaticAnalyzerCore
    clangAnalysis
    clangARCMigrate
    clangRewrite
    clangRewriteFrontend
    clangEdit
    clangAST
    clangASTMatchers
    clangLex
    clangBasic
    clang
)

target_include_directories(clang_tool PRIVATE
${CLANG_INCLUDE_DIRS}

)
set_property(TARGET clang_tool PROPERTY CXX_STANDARD 14)
