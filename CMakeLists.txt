cmake_minimum_required(VERSION 3.12)
project(Red_belt)

set(CMAKE_CXX_STANDARD 17)

MESSAGE(STATUS "Compiler flags:" ${CMAKE_CXX_COMPILE_FLAGS})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_RELEASE})

set(WEEKS
    Week_1
    Week_2
    Week_3
    Week_4
    Week_5
    Week_6
)

set(Week_1
        task_0_max_element_if
        task_1_log
        task_2_comps
        task_3_update_field
        task_4_bad_macros
        task_5_uniq_id
        task_6_table
        task_7_deque
        task_8_paginator
        Lecture_example
)

set(Week_2
        task_1_sort_students
        task_2_learner
        task_3_expresses
        task_4_ebook
        task_5_booking
        utility
)

set(Week_3
        task_1_objectPool
        task_2_SortSwapCope
        task_3_vector
        task_4_list
        )

set(Week_4
        stringSet
        task_1_sportsmen
        task_2_stackVector
        task_3_translator
        task_4_airportCounter
        task_5_textEditor
        task_6_webServer
        utility
        )

set(Week_5
        task_1_josephus
        task_2_groupStrings
        task_3_simpleVector
        task_4_splitIntoSentences
        task_5_simpleVector2
        task_6_mergeSort3
        task_7_priority_container
        task_8_matrix_sum
        task_9_blog
        task_10_synchronized
        task_11_ConcurrentMap
        Account
        samples
        )

set(Week_6
        part_1
        )

# create a project file per tutorial
foreach(WEEK ${WEEKS})
    foreach(TASK ${${WEEK}})
        file(GLOB SOURCE
                "${WEEK}/${TASK}/*.h"
                "${WEEK}/${TASK}/*.cpp"
                )
        set(NAME "${WEEK}_${TASK}")
        add_executable(${NAME} ${SOURCE})
        target_include_directories(${NAME} PRIVATE "test_framework/")
    endforeach(TASK)
endforeach(WEEK)