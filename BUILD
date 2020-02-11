package(default_visibility = ["//visibility:public"])

cc_library(
    name = "pond_solver",
    srcs = ["pond_solver.cc"],
    hdrs = ["pond_solver.h"],
)

cc_binary(
    name = "pond_solver_main",
    srcs = ["pond_solver_main.cc"],
    deps = [
        ":pond_solver",
    ],
)
