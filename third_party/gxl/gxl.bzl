load("//tools:repository.bzl", "github_archive")

def gxl_dependency():
    github_archive(
        name = "gxl",
        commit = "3d401da08d1b6253fc42012e2744f29e7022734a",
        repo_name = "davidzchen/gxl",
    )

def _gxl_repository_impl(_ctx):
    gxl_dependency()

gxl_repository = module_extension(
    implementation = _gxl_repository_impl,
)
