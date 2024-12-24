load("//bazel:repository.bzl", "github_archive")

def _dependency():
    github_archive(
        name = "rules_cc_embed_data",
        commit = "baacf923f50ed17203a0ce05492ab0c959ea0988",
        repo_name = "davidzchen/rules_cc_embed_data",
    )

def _repository_impl(_ctx):
    _dependency()

rules_cc_embed_data_repository = module_extension(
    implementation = _repository_impl,
)
