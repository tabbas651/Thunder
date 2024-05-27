# If not stated otherwise in this file or this component's license file the
# following copyright and licenses apply:
#
# Copyright 2020 Metrological
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# https://stackoverflow.com/questions/60211516/programmatically-get-all-targets-in-a-cmake-project

# Collect all currently added targets in all subdirectories
#
# Parameters:
# - _result the list containing all found targets
# - _dir root directory to start looking from

# Set some defaults

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_DEFAULT 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-std=c++14" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "--std=c++14" CACHE STRING "")

