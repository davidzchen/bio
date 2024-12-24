load("//tools:repository.bzl", "github_archive")

def abxl_dependency():
    github_archive(
        name = "abxl",
        commit = "73767dbeac1fb5fd28943905fb3cd46dbb3fb01c",
        repo_name = "davidzchen/abxl",
    )

def _abxl_repository_impl(_ctx):
    abxl_dependency()

abxl_repository = module_extension(
    implementation = _abxl_repository_impl,
)
