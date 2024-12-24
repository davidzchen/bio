load("//tools:repository.bzl", "github_archive")

def primrose_dependency():
    github_archive(
        name = "primrose",
        commit = "e2dcf54a653def6e754b969c5b7153f7d7f41752",
        repo_name = "davidzchen/primrose",
    )

def _primrose_repository_impl(_ctx):
    primrose_dependency()

primrose_repository = module_extension(
    implementation = _primrose_repository_impl,
)
