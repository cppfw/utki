# Installation in Linux

## Using Debian packet manager
- Add the repo to your `sources.list` (needs to be done once):
  ```
  deb http://dl.bintray.com/igagis/<distro> <release> main
  ```
  where
  - `<distro>` is `debian` or `ubuntu`
  - `<release>` is `stretch`, `buster`, `xenial`, `bionic` etc.
  

- Import APT key

  ```
  sudo apt install dirmngr
  sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 379CE192D401AB61
  ```

- Update apt packages

  ```
  sudo apt update
  ```

- Install **prorab** package

  ```
  sudo apt install libutki-dev
  ```
