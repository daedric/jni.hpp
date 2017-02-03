cmake_minimum_required (VERSION 3.1)

project (
    examples
    C CXX
)

set (CMAKE_CXX_STANDARD 14)

find_package (Java REQUIRED)
find_package (JNI REQUIRED)

include(UseJava)

add_library (hello SHARED "examples/hello.cpp")
add_library (peer SHARED "examples/native_peer.cpp")
add_executable (unit_tests_low
    "test/low_level.cpp"
)
add_executable (unit_tests_high
    "test/high_level.cpp"
)

include_directories (
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
            "${JNI_INCLUDE_DIRS}"
)

add_jar (hello_jar "examples/Hello.java"
    OUTPUT_NAME "Hello"
    ENTRY_POINT "Hello"
)
add_jar (peer_jar "examples/NativePeer.java"
    OUTPUT_NAME "NativePeer"
    ENTRY_POINT "NativePeer"
)


include (CTest)
add_test (
	NAME hello_test
	COMMAND java -Xcheck:jni
                 -Djava.library.path=$<TARGET_FILE_DIR:hello>
                 -jar $<TARGET_PROPERTY:hello_jar,JAR_FILE>
                 "Me!"
)
add_test (
	NAME peer_test
	COMMAND java -Xcheck:jni
                 -Djava.library.path=$<TARGET_FILE_DIR:peer>
                 -jar $<TARGET_PROPERTY:peer_jar,JAR_FILE>
)
add_test (
	NAME test_suite_low
	COMMAND $<TARGET_FILE:unit_tests_low>
)
add_test (
	NAME test_suite_high
	COMMAND $<TARGET_FILE:unit_tests_high>
)