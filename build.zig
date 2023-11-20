const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{ .preferred_optimize_mode = .ReleaseFast });
    const exe = b.addExecutable(.{
        .name = "blogbench",
        .target = target,
        .optimize = optimize,
    });
    exe.linkLibC();
    exe.single_threaded = false;
    const source_files = &.{
        "src/blogbench.c",
        "src/commenter.c",
        "src/helpers.c",
        "src/process.c",
        "src/reader.c",
        "src/rewriter.c",
        "src/writer.c",
    };
    if (@hasDecl(std.Build.Step.Compile, "AddCSourceFilesOptions")) {
        exe.addCSourceFiles(.{ .files = source_files });
    } else {
        exe.addCSourceFiles(source_files, &.{});
    }
    exe.defineCMacro("PACKAGE_STRING", "\"blogbench 1.2\"");
    exe.defineCMacro("PACKAGE_BUGREPORT", "\"https://github.com/jedisct1/Blogbench\"");
    exe.defineCMacro("HAVE_ALLOCA", "1");
    exe.defineCMacro("HAVE_FCNTL_H", "1");
    exe.defineCMacro("HAVE_GETOPT_LONG", "1");
    exe.defineCMacro("HAVE_SYNC", "1");
    exe.defineCMacro("HAVE_STRTOULL", "1");
    exe.defineCMacro("HAVE_SYS_PARAM_H", "1");
    exe.defineCMacro("HAVE_SYS_TIME_H", "1");

    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
