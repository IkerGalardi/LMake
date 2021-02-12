function test_correct()
    local found = lmake_find("src/*.c")
    lmake_error(found)
end

function test_no_regex()
    local found = lmake_find("src/.c")
    lmake_error(found)
end

function test_no_path()
    local found = lmake_find("sr/*.c")
    lmake_error(found)
end