load("//tools:repository.bzl", "github_archive")

def gxl_dependency():
    github_archive(
        name = "gxl",
        commit = "9054f670640ccd9142c4b9257a22c4dc617a0730",
        repo_name = "davidzchen/gxl",
    )

def _gxl_repository_impl(_ctx):
    gxl_dependency()

gxl_repository = module_extension(
    implementation = _gxl_repository_impl,
)
