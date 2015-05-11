display = Object("display.c")
program = Object("program.c")

Program("program", [display, program], LIBS=["3d"])
