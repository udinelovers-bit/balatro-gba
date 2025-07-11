# Balatro GBA
This is my attempt to recreate the game 'Balatro' as accurately as possible, including all of the visual effects that make Balatro feel satisfying to play.

This port is strictly limited to the content that was available in the Balatro Steam demo.

![Example](https://github.com/cellos51/balatro-gba/blob/main/example.gif)

# **Build Instructions:**

## **-Windows-**
1. Install `devkitPro` from https://devkitpro.org/ and navigate to `Click Here for instructions`
2. Navigate down until you find the `Download the latest version of the graphical installer` and download the latest release from the Github page.
3. Execute it and follow the installation instructions, you only need to install the tools for the GBA development so ensure you have it selected and install it to the default directory.
4. Go to the Start Menu and search for `MSys2` and open it.
5. Install `Git` by typing this command: `pacman -S git`
6. Navigate to where you want to clone the project such as your desktop or Documents folder. 
The easiest way to properly `cd` to the directory is to type `cd` and drag and drop the folder into the terminal window and press Enter. 
Example: `cd '/home/user/OneDrive/Documents/Folder` or something like this.
7. Clone the project with `git clone --recursive https://github.com/cellos51/balatro-gba.git` in the terminal
8. Now you want to `cd` to the new folder that was created. Use `cd` and drag the new folder into the window to add the file path and press Enter.
9. Run the `make` command in the window to start building.
10. After it completes, navigate through the `build` directory in the project folder and look for `balatro-gba.gba` and load it on an emulator or flashcart.

## **-Linux-**
[Undocumented]

### **Common Issues:**
1. **When I drag the folder into the window and press enter, it doesn't work!**
-After dragging the folder, press the right or left arrow key until the text is no longer highlighted and then press enter.
2. **When I run `make` it errors out and won't compile!**
-Move the project folder to your desktop and then `cd` to it by dragging the folder into the terminal window after typing `cd` first.
3. **I can't find the build folder!**
-Look for a folder called `build` inside the project folder and then find `balatro-gba.gba`.
4. **The Game won't start!**
-Try a different emulator or if you are using original hardware, make sure the rom is not corrupted and try a different flashcart or SD Card. If this does not work, open an issue on the Github page because a recent commit may have broke the game. 
