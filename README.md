# KDE Plasma Nature Wallpapers

Picture of the Day provider for KDE Plasma. It rotates through wallpapers daily. 

Pictures are hand-picked from Unsplash. They showcase the beauty of nature!

Help us reach 365 (one per day of the year) by opening a PR!

# Usage (From Source)

If you know how to package this binary, please let me know so I can package and upload it to GitHub.

Clone the repo and open a terminal in the root directory.

```bash
cd src
mkdir -p build
cd build
cmake ..
cmake --build .
sudo cmake --install .
```

Restart Plasma. Logging out and logging back in works, as does this:

`systemctl --user restart plasma-plasmashell.service`

Go into your wallpaper settings and you should see a Nature Wallpapers provider. Pick it.

# Thank You

Special thanks go to:

- **ChatGPT** for walking me through installation and coding of C++ and KDE libraries needed for this project
- **Unsplash** for building and then giving away a free, high-quality library of beautiful nature wallpapers.