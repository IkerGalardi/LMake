VAR = "variable name original"

function test()
    lmake_error(VAR)
end

function test_no_var()
    lmake_error(VARIABLE)
end