############################################################################
# Authors:
#  2015 -       Manuel Penschuck
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
############################################################################

enable_testing()

include_directories(${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include)

file(GLOB SRCS Test*.cpp)

add_executable(tests ${SRCS})

target_link_libraries(tests
       gtest_main gtest
       ${STXXL_LIBRARIES}
)

add_test(tests tests)

add_executable(im_bfs main_im_bfs.cpp)
target_link_libraries(im_bfs ${STXXL_LIBRARIES})
