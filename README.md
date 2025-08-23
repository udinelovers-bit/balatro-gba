# Balatro GBA
This is an attempt to recreate the game 'Balatro' as accurately as possible, including all of the visual effects that make Balatro feel satisfying to play.

This tech-demo/proof of concept is strictly limited in content to a minimal version of Balatro and will not recreate the full game.
### **Disclaimer: This project is NOT endorsed by or affiliated with Playstack or LocalThunk**
#### This is a non-profit community fan project solely aimed to recreate a minimal version of Balatro on the Game Boy Advance as a tribute to the full Balatro and is not meant to infringe or draw sales away from the full game's release or any of the established works by Playstack and LocalThunk. 
#### All rights are reserved to their respective holders. 
This version is meant for people who already own and know the official full game.

## Buy the official full version from these sources:

[![Balatro on Steam](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Steam-194c84?logo=steam&logoColor=fff)](https://store.steampowered.com/app/2379780/Balatro/)
[![Balatro on Google Play](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Google%20Play-414141?logo=Google-play&logoColor=fff)](https://play.google.com/store/apps/details?id=com.playstack.balatro.android)
[![Balatro on Apple App Store](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Apple%20App%20Store-0D96F6?logo=app-store&logoColor=fff)](https://apps.apple.com/us/app/balatro/id6502453075)
[![Balatro on Nintendo eShop](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Nintendo%20eShop-e60012?logo=nintendo&logoColor=fff)](https://www.nintendo.com/us/store/products/balatro-switch/)
[![Balatro on PlayStation Store](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20PlayStation%20Store-006FCD?logo=PlayStation&logoColor=fff)](https://store.playstation.com/en-us/concept/10010334)
<!-- I had to manually embed an Xbox Logo for it to display on the button. It was a real pain in the ass to figure out but it works. -->
[![Balatro on Xbox](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Xbox-107C10?logo=data:image/svg%2bxml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHN2ZyB2ZXJzaW9uPSIxLjEiIHZpZXdCb3g9IjAgMCAzNzIuMzcgMzcyLjU3IiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciPgo8dGl0bGU%2BWGJveCBMb2dvPC90aXRsZT4KPGcgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoLTEuNTcwNyAxMi4zNTcpIj4KPHBhdGggZD0ibTE2OS4xOSAzNTkuNDVjLTI4LjY4My0yLjc0NzEtNTcuNzIzLTEzLjA0OC04Mi42NjktMjkuMzI0LTIwLjkwNC0xMy42MzktMjUuNjI1LTE5LjI0Ni0yNS42MjUtMzAuNDM1IDAtMjIuNDc1IDI0LjcxMi02MS44NCA2Ni45OTItMTA2LjcyIDI0LjAxMi0yNS40ODYgNTcuNDYtNTUuMzYgNjEuMDc4LTU0LjU1MSA3LjAzMDkgMS41NzI0IDYzLjI1IDU2LjQxMSA4NC4yOTcgODIuMjI1IDMzLjI4MSA0MC44MjEgNDguNTgxIDc0LjI0NSA0MC44MDggODkuMTQ3LTUuOTA4NyAxMS4zMjgtNDIuNTcyIDMzLjQ2Ny02OS41MDggNDEuOTcyLTIyLjIgNy4wMTAxLTUxLjM1NSA5Ljk4MTMtNzUuMzcyIDcuNjgxMXptLTEzNi41My04My4xMjZjLTE3LjM3MS0yNi42NS0yNi4xNDctNTIuODg2LTMwLjM4NS05MC44MzItMS4zOTkyLTEyLjUzLTAuODk3NzgtMTkuNjk3IDMuMTc3Mi00NS40MTUgNS4wNzg4LTMyLjA1NCAyMy4zMzMtNjkuMTM2IDQ1LjI2Ny05MS45NTggOS4zNDE5LTkuNzE5NyAxMC4xNzYtOS45NTY1IDIxLjU2My02LjEyMDUgMTMuODI4IDQuNjU4NCAyOC41OTYgMTQuODU3IDUxLjQ5OCAzNS41NjdsMTMuMzYzIDEyLjA4My03LjI5NjkgOC45NjQzYy0zMy44NzIgNDEuNjEzLTY5LjYzIDEwMC42LTgzLjEwNSAxMzcuMDktNy4zMjYgMTkuODM5LTEwLjI4MSAzOS43NTMtNy4xMjg3IDQ4LjA0NCAyLjEyODIgNS41OTc1IDAuMTczMzkgMy41MTA5LTYuOTUyOC03LjQyMTV6bTMwNC45MiA0LjUzMjZjMS43MTYtOC4zNzcyLTAuNDU0NC0yMy43NjMtNS41NDEzLTM5LjI4LTExLjAxNy0zMy42MDYtNDcuODQtOTYuMTI0LTgxLjY1My0xMzguNjNsLTEwLjY0NC0xMy4zODEgMTEuNTE2LTEwLjU3NGMxNS4wMzYtMTMuODA3IDI1LjQ3Ni0yMi4wNzQgMzYuNzQtMjkuMDk1IDguODg4OC01LjU0MDIgMjEuNTkxLTEwLjQ0NSAyNy4wNTEtMTAuNDQ1IDMuMzY2MyAwIDE1LjIxNyAxMi4yOTkgMjQuNzg0IDI1LjcyMSAxNC44MTcgMjAuNzg4IDI1LjcxOCA0NS45ODcgMzEuMjQgNzIuMjIgMy41NjgzIDE2Ljk1IDMuODY1NyA1My4yMzEgMC41NzQ4NiA3MC4xMzktMi43MDA3IDEzLjg3Ni04LjQwMzEgMzEuODc1LTEzLjk2NiA0NC4wODItNC4xNjgyIDkuMTQ2Ni0xNC41MzUgMjYuOTEtMTkuMDc4IDMyLjY5MS0yLjMzNTcgMi45NzE4LTIuMzM3NiAyLjk2NTMtMS4wMjM5LTMuNDQ3N3ptLTE2NS4zMi0yNDcuNzVjLTE1LjYwMS03LjkyMjctMzkuNjctMTYuNDI3LTUyLjk2NS0xOC43MTUtNC42NjEtMC44MDIxMi0xMi42MTItMS4yNDk1LTE3LjY2OS0wLjk5NDExLTEwLjk3IDAuNTUzOTQtMTAuNDgtMC4wMTk3IDcuMTE3OC04LjMzMzcgMTQuNjMtNi45MTIxIDI2LjgzNC0xMC45NzcgNDMuNC0xNC40NTUgMTguNjM2LTMuOTEzMSA1My42NjYtMy45NTkgNzIuMDA1LTAuMDk0NCAxOS44MDggNC4xNzQxIDQzLjEzMyAxMi44NTQgNTYuMjc2IDIwLjk0MmwzLjkwNjMgMi40MDM5LTguOTYyNS0wLjQ1MjU4Yy0xNy44MS0wLjg5OTM3LTQzLjc2NiA2LjI5NTktNzEuNjMzIDE5Ljg1Ny04LjQwNTQgNC4wOTA1LTE1LjcxOCA3LjM1NzUtMTYuMjUgNy4yNi0wLjUzMjExLTAuMDk3NTQtNy4zODQzLTMuNDM1OS0xNS4yMjctNy40MTg2eiIgZmlsbD0iI2ZmZiIvPgo8L2c%2BCjwvc3ZnPgo=&logoColor=fff)](https://www.xbox.com/en-US/games/store/balatro/9PK087LNGJC5)
[![Balatro on Humble Bundle](https://img.shields.io/badge/Balatro%20on%20Humble%20Bundle-%23494F5C.svg?logo=HumbleBundle&logoColor=white)](https://www.humblebundle.com/store/balatro?srsltid=AfmBOoqS2De8T4kizzWxJS1pbvQosJ_bYCl4qvC6LA1YLPAh4sZ8vJqO)

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

