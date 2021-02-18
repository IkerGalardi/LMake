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