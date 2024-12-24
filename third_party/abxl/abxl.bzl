load("//bazel:repository.bzl", "github_archive")

def abxl_dependency():
    github_archive(
        name = "abxl",
        commit = "021cc69ac994ae965a7eab15cdba9cd00e7b6908",
        repo_name = "davidzchen/abxl",
    )

def _abxl_repository_impl(_ctx):
    abxl_dependency()

abxl_repository = module_extension(
    implementation = _abxl_repository_impl,
)
