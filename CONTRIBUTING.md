# Contributing to Buddhabrot

Buddhabrot is an open source community project, and everyone is welcome to contribute. This short guide will assist you with the workflow required in successfully having your features and bug fixes merged into the project.

# Table of Contents

- [How to Contribute Code](#how-to-contribute-code)
  - [Creating a Fork](#creating-a-fork)
  - [Keeping Your Fork Up to Date](#keeping-your-fork-up-to-date)
  - [Creating a Development Branch](#creating-a-development-branch)
  - [Submitting a Pull Request](#submitting-a-pull-request)
- [How to Submit a Bug Report](#how-to-submit-a-bug-report)

## How to Contribute Code

### Creating a Fork

The best approach to contribute code to Buddhabrot is to fork the [main repository](https://github.com/salindersidhu/Buddhabrot) on GitHub, then submit a pull request.

1. [Create an account](https://github.com/join) on GitHub if you do not have one.

2. Fork the repository by clicking on the 'Fork' button near the top-right section of the page. This creates a copy of the code under your account on GitHub.

3. Clone your fork to your local machine:

```bash
git clone https://github.com/USERNAME/Buddhabrot.git

# If you have an SSH key setup with GitHub
git clone git@github.com:USERNAME/Buddhabrot.git

# Navigate to your cloned fork
cd Buddhabrot
```

### Keeping Your Fork Up to Date

You will want to make sure you keep your fork synced with the original repository by tracking the original "upstream" repository that you forked.

1. Add "upstream" to the list of remotes:

```bash
# You will only need to do this once
git remote add upstream https://github.com/salindersidhu/Buddhabrot.git
```

2. Fetch the upstream repository's latest commits and branches:

```bash
git fetch upstream
```

3. Checkout the main branch and merge in the changes from the upstream repository's main branch:

```bash
git checkout main
git merge upstream/main
```

### Creating a Development Branch

A new branch is required when working on a new feature or bug fix. This ensures that your changes are kept seperate from the main branch making it easier to manage multiple pull requests for every task you complete.

1. Checkout the main branch as the starting point for the development branch:

```bash
git checkout main
```

2. Create a new development branch on your local machine and switch to it:

```bash
git checkout -b {prefix}/branch-name origin/main
```

_Prefix is either `hotfix`, `feature` or `experiment` depending on the type of development work._

### Submitting a Pull Request

Prior to submitting a pull request, you must ensure that you have rebased your
development branch so that merging it into the original repository is a simple
fast-forward free of merge conflicts.

1. [Ensure that your fork is up to date.](#keeping-your-fork-up-to-date)

2. Rebase your development branch:

```bash
git checkout {prefix}/branch-name
git rebase main
```

Once you have committed and pushed all of your changes on your development branch to GitHub, go to the page for your fork on GitHub, select your development branch and click the pull request button. Once a pull request has been made, you can continue to make changes to the development branch and your pull request will automatically track the changes and update.

## How to Submit a Bug Report

If you find a bug in the code, please submit a ticket to the [Bug Tracker](https://github.com/salindersidhu/Buddhabrot/issues).

Before submitting your bug report, please ensure that your ticket contains the following:

- A short summary of the bug, typically a couple of sentences.
- A screenshot showing the bug.
- The version of Buddhabrot you are using.
