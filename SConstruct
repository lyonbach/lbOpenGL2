
CPPPATH = "./include:./thirdParty/include"

NAME = "lbOpengl2"
SOURCES = [Glob("./thirdParty/src/glad/*.c"), ]
SOURCES.append(Glob(f"./source/*.cpp"))


env=Environment(
    CPPPATH=CPPPATH,
    LIBPATH="./thirdParty/lib/GLFW",
    LIBS=["glfw3", "GL", "X11", "pthread", "Xrandr", "dl"],
    SCONS_CXX_STANDARD="c++20",
    )
env.Program(NAME, SOURCES)
