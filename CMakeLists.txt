cmake_minimum_required(VERSION 3.28)
project(Bookstore_lwz)

set(CMAKE_CXX_STANDARD 20)

add_executable(code
        main.cpp
        ./StanfordCPPLib/tokenscanner.cpp
        ./StanfordCPPLib/error.cpp
        ./StanfordCPPLib/strlib.cpp
        user_data.cpp
        book_data.cpp
        key_processor.cpp
        key_file.cpp
        book_name_processor.cpp
        author_processor.cpp
        finance.cpp
        worker.cpp
        worker.h)
