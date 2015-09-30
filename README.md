# utki

C++ utility library. Stuff missing from std:: namespace.

# Installation

## Linux
For Debian based distributions it is easy, packages reside in *https://repo.fury.io/igagis/* repository.
- Add the repo to your sources (needs to be done once):

```
sudo echo "deb https://repo.fury.io/igagis/ /" > /etc/apt/sources.list.d/igagis.list
```

- Update apt packages

```
sudo apt-get update
```

- Install packages

```
sudo apt-get install libutki-dev libutki-doc
```

## MacOSX
Installation in MacOSX requires a *[brew](http://brew.sh/)* package manager.

- Add the homebrew tap:

```
sudo brew tap igagis/tap
```

- Update formulaes:

```
sudo brew update
```

- Install keg

```
sudo brew install libutki
```


#WiKi

[Main WiKi page](wiki/MainPage.md)
