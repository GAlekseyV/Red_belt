cmake_minimum_required(VERSION 3.12)
project(Red_belt)

set(CMAKE_CXX_STANDARD 17)

set(WEEKS
    Week_1
    Week_2
    Week_3
    Week_4
    Week_5
    Week_6
)

set(Week_1
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
        utility
        )

set(Week_5
        )

set(Week_6
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
    endforeach(TASK)
endforeach(WEEK)