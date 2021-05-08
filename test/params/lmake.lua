function test_noparams()
    lmake_compatibility_version()
    
    -- Test compilation functions
    lmake_set_compiler()
    lmake_set_compiler_flags()
    lmake_set_compiler_out()
    lmake_compile()
    
    -- Test linking functions
    lmake_set_linker()
    lmake_set_linker_flags()
    lmake_set_linker_out()
    lmake_link()

    -- Test directory function
    lmake_chdir()
    lmake_last_dir()

    lmake_exec()

    lmake_error()

    lmake_find()
end