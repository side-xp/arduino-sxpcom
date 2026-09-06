# Contributing

First of all, thank you for considering contributing to a *Sideways Experiments* project!

At Sideways Experiments, we hate doing the same thing more than once, and we love to share knowledge. Whether because you "just want to help" or share that generous laziness philosophy, you're welcome!

## Developer setup

### Install

`clang-format` and `cppcheck` are installed via `pip` ([Python 3](https://www.python.org) required) as prebuilt binaries:

```bash
pip install "clang-format==18.1.8" cppcheck
```

`arduino-lint` and `arduino-cli` don't have pip packages, but ship official install scripts that detect your OS/architecture automatically. They're installed into `./.tools` (gitignored):

```bash
mkdir -p ./.tools
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | BINDIR=./.tools sh
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=./.tools sh
```

Then install the AVR core, which is only needed once:

```bash
./.tools/arduino-cli core update-index
./.tools/arduino-cli core install arduino:avr
```

### Run the checks

This repository ships a utility script you can run directly to perform all the formatting and checks:

```bash
./scripts/check.sh
```

### Local use

The library itself if not an Arduino project. So for manual checks and actually use the library locally, you can create a symlink of this repository into an actual Arduino project.

```sh
export MSYS=winsymlinks:nativestrict
ln -s "/path/to/cloned/arduino-sxpcom" "/path/to/Arduino/project/libraries/SXPCom"
```

On Windows, you can also use CMD to avoid any access issues:

```sh
mklink /D "C:\path\to\Arduino\project\libraries\SXPCom" "C:\path\to\cloned\arduino-sxpcom"
```

Note that putting the library into a `/libraries` folder within your Arduino project is mandatory so Arduino IDE can actually use it like an actual library imported in the project.

## Get involved!

There are many ways to be involved in our open source projects, and there's absolutely no pressure to give more or less of your time. So whether you want to develop the core of a whole package or just post a comment in an issue, any help is appreciated!

So what can you do to help?

- **Discuss with the community**: join our [Discord server](https://discord.gg/G49RUZ9F2N), and start chatting with the community or the core team
- **Report bugs**: found a bug when using one of our packages? You can report it in the *Issues* tab on GitHub
- **Suggest improvements**: whether from the [Discord server](https://discord.gg/G49RUZ9F2N) or by creating an *Issue*, feel free to talk about your needs or current usage of our solutions, highlight what's missing or what could be better
- **Request new features**: again, you can use the [Discord server](https://discord.gg/G49RUZ9F2N) or create a new *Issue* to ask for something new, see if others may need it too, so we can consider modifying an existing package or even start a new project just for it
- **Address issues**: you can contribute directly to the codebase by resolving an issue, creating the required assets, or just implementing changes and create a Pull Request on *GitHub*

> By the way, we love to know how you use our tools, so we can make them better!

## Code of Conduct

*Sideways Experiments* has adopted the [*Contributor Covenant*](https://www.contributor-covenant.org/) as its Code of Conduct for its open source projects, and we expect contributors to adhere to it.

If you observe any unacceptable behavior or need to report a violation, please contact the *Sideways Experiments* core team directly to [contact@sideways-experiments.com](mailto:contact@sideways-experiments.com).

## Code syntax

Please refer to our [general Coding Style specification](https://github.com/side-xp/.github/blob/main/docs/coding-style/README.md) to learn more about the conventions used in this project.

## Submitting a Pull Request

To contribute code:

1. **Fork** this repository
2. Create a **new branch** off `develop`
3. Make your changes and commit them
   - **Signed commits** are required
   - Take care to follow our [commit message guidelines](https://github.com/side-xp/.github/blob/main/docs/coding-style/commit-messages.md)
4. Test your changes locally
5. Submit a **Pull Request targeting `develop` or `dev`**, not `master` or `main`
6. Describe your changes and reference any related issues
7. A member of the ***Sideways Experiments* core team** will review it

As mentioned in the [*Suggesting enhancements or features*](https://github.com/side-xp/.github/blob/main/SUPPORT.md#suggesting-enhancements-or-features) section of our support guidelines, please don't create Pull Requests for unsolicited work.

### Pull Request Requirements

- One PR per logical change (fix, feature, etc.)
- Must pass basic tests (if applicable)
- Must use **signed commits**
- All PRs must be reviewed by a *Sideways Experiments* core team member
- Assign the PR to the core team if not automatically assigned

## Releases

Releases are generated based on `vX.Y.Z` tags pushed to the repository.

Each release will include a compiled package and changelog.

We currently do **not** require contributors to handle release generation — this is done internally.

## License and Contributor Agreement

Most of our projects are licensed under the [MIT License](https://mit-license.org).

A `LICENSE.md` file is always included in our projects' repository root, and all contributions to a specific project will be licensed under the same terms as that file.

We will evaluate whether a Contributor License Agreement (CLA) is required in the future, as the projects go. For now, you can contribute freely under MIT.

## Maintainers

This project is maintained by the team at *Sideways Experiments*.

If you're unsure about any part of the contribution process, feel free to open a discussion on our [Discord server](https://discord.gg/G49RUZ9F2N), or by sending an email directly to [contact@sideways-experiments.com](mailto:contact@sideways-experiments.com).

---

Thank you again for being part of the project!