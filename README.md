# Balatro GBA
This is an attempt to recreate the game 'Balatro' as accurately as possible, including all of the visual effects that make Balatro feel satisfying to play.

This port is strictly limited in content to a minimal version of Balatro and will not recreate the full game.
### **Disclaimer: This project is NOT endorsed by or affiliated with Playstack or LocalThunk**
#### This is a non-profit community fan project solely aimed to recreate a minimal version of Balatro on the Game Boy Advance as a tribute to the full Balatro and is not meant to infringe or draw sales away from the full game's release or any of the established works by Playstack and LocalThunk. 
#### All rights are reserved to their respective holders. 
This version is meant for people who already own and know the official full game.

Buy the official full version

[Balatro on Steam](https://store.steampowered.com/app/2379780/Balatro/)

[Balatro on Google Play](https://play.google.com/store/apps/details?id=com.playstack.balatro.android)

[Balatro on Apple App Store](https://apps.apple.com/us/app/balatro/id6502453075)

[Balatro on Switch](https://www.nintendo.com/us/store/products/balatro-switch/)

[Balatro on PlayStation Store](https://store.playstation.com/en-us/concept/10010334)

[Balatro on Xbox](https://www.xbox.com/games/store/balatro/9PK087LNGJC5)

![Example](https://github.com/cellos51/balatro-gba/blob/main/example.gif)

# **Build Instructions:**

## **-Windows-**
1. Install `devkitPro` from https://github.com/devkitPro/installer by downloading the latest release.
2. Execute it and follow the installation instructions, you only need to install the tools for the GBA development so ensure you have it selected and install it to the default directory.
3. Go to the Start Menu and search for `MSys2` and open it.
4. Install `Git` by typing this command: `pacman -S git` if you don't have it already installed.
5. Navigate to where you want to clone the project such as your desktop or Documents folder. 
The easiest way to properly `cd` to the directory is to type `cd` and drag and drop the folder into the terminal window and press Enter. 
Example: `cd '/home/user/OneDrive/Documents/balatro-gba` or something like this.
6. Clone the project with `git clone --recursive https://github.com/cellos51/balatro-gba.git` in the terminal window.
7. Use `cd` and drag the newly generated folder into the window to add the file path and press Enter.
8. Type `make` into the window and press Enter to start building the game.
9. After it completes, navigate through the `build` directory in the project folder and look for `balatro-gba.gba` and load it on an emulator or flashcart.

## **-Linux-**
[No Instructions Available]

### **Common Issues:**
1. **When I drag the folder into the window and press enter, it doesn't work!**
-After dragging the folder, press the right or left arrow key until the text is no longer highlighted and then press enter.
2. **When I run `make` it errors out and won't compile!**
-Move the project folder to your desktop and then `cd` to it by dragging the folder into the terminal window after typing `cd` first.
3. **I can't find the build folder!**
-Look for a folder called `build` inside the project folder and then find `balatro-gba.gba`.
4. **The Game won't start!**
-Try a different emulator or if you are using original hardware, make sure the rom is not corrupted and try a different flashcart or SD Card. If this does not work, open an issue on the Github page because a recent commit may have broke the game. 
5. **It says I don't have `Git` or `Make` installed!**
-Use `pacman -S git` or `pacman -S make` although it should already be installed if you followed the instructions correctly.

