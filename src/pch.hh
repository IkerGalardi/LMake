/*
 * This project is provided under the GNU GPL v2 license, more information can 
 * found on https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    - Iker Galardi
 */

#pragma once

// Library includes
#include <spdlog/spdlog.h>
#include <lua/lua.hpp>

// C++ Standard library includes
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <memory>

// C Standard library includes
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Own includes
#include "luavm.hh"
#include "utils.hh"
#include "os/filesystem.hh"
#include "os/process_management.hh"