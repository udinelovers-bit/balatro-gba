# Balatro-GBA

[![Build Status](https://img.shields.io/github/actions/workflow/status/cellos51/balatro-gba/build_ci_workflow.yml?style=flat&logo=github&branch=main&label=Builds&labelColor=gray&color=default&v=1)](https://github.com/cellos51/balatro-gba/actions)
[![Open Issues](https://custom-icon-badges.demolab.com/github/issues/cellos51/balatro-gba?logo=bug&style=flat&label=Issues&labelColor=gray&color=red)](https://github.com/cellos51/balatro-gba/issues)
[![Pull Requests](https://custom-icon-badges.demolab.com/github/issues-pr/cellos51/balatro-gba?logo=git-pull-request&style=flat&label=Pull%20Requests&labelColor=gray&color=indigo&v=1)](https://github.com/cellos51/balatro-gba/pulls)
[![Forks](https://custom-icon-badges.demolab.com/github/forks/cellos51/balatro-gba?logo=repo-forked&style=flat&label=Forks&labelColor=gray&color=blue)](https://github.com/cellos51/balatro-gba/network/members)
[![Watchers](https://custom-icon-badges.demolab.com/github/watchers/cellos51/balatro-gba?logo=eye&style=flat&label=Watchers&labelColor=gray&color=lightgray&v=1)](https://github.com/cellos51/balatro-gba/watchers)
[![Stars](https://custom-icon-badges.demolab.com/github/stars/cellos51/balatro-gba?logo=star&style=flat&labelColor=gray&color=yellow&label=Stars&v=1)](https://github.com/cellos51/balatro-gba)

This is an attempt to recreate the game **'Balatro'** as accurately as possible, including all of the visual effects that make Balatro feel satisfying to play.

This **tech-demo/proof of concept** is strictly limited in content to a minimal version of Balatro and will **NOT** recreate the full game. **This version is intended for people who already own and know how the official full game works.** Please refer to the Balatro Wiki for any information regarding Joker effects and such.

## Disclaimer: This project is NOT endorsed by or affiliated with Playstack or LocalThunk
### This is a non-profit community fan project solely aimed to recreate a minimal version of Balatro on the Game Boy Advance as a tribute to the full Balatro and is not intended to infringe or draw sales away from the full game's release or any of the established works by Playstack and LocalThunk. 
### All rights are reserved to their respective holders. 

## Buy the official full version from these sources below:

<!-- I had to figure out some actual coding shit to get the Xbox logo to show up, hopefully it works and to understand this comment, look at the previous commit -->

[![Balatro on Steam](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Steam-194c84?logo=steam&logoColor=fff)](https://store.steampowered.com/app/2379780/Balatro/)
[![Balatro on Google Play](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Google%20Play-414141?logo=Google-play&logoColor=fff)](https://play.google.com/store/apps/details?id=com.playstack.balatro.android)
[![Balatro on Apple App Store](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Apple%20App%20Store-0D96F6?logo=app-store&logoColor=fff)](https://apps.apple.com/us/app/balatro/id6502453075)
[![Balatro on Nintendo eShop](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Nintendo%20eShop-e60012?logo=nintendo&logoColor=fff)](https://www.nintendo.com/us/store/products/balatro-switch/)
[![Balatro on PlayStation Store](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20PlayStation%20Store-006FCD?logo=PlayStation&logoColor=fff)](https://store.playstation.com/en-us/concept/10010334)
[![Balatro on Xbox](https://custom-icon-badges.demolab.com/badge/Balatro%20on%20Xbox-107C10.svg?logo=xbox&logoColor=white)](https://www.xbox.com/en-US/games/store/balatro/9PK087LNGJC5)
[![Balatro on Humble Bundle](https://img.shields.io/badge/Balatro%20on%20Humble%20Bundle-%23494F5C.svg?logo=HumbleBundle&logoColor=white)](https://www.humblebundle.com/store/balatro?srsltid=AfmBOoqS2De8T4kizzWxJS1pbvQosJ_bYCl4qvC6LA1YLPAh4sZ8vJqO)

![Example](https://github.com/cellos51/balatro-gba/blob/main/example.gif) 

<!---Gif Needs Updated--->
### Controls: 
(A: Pick Card/Make Selections)

(B: Deselect All Cards) 

(L: Sell Joker)

(R: Sort Suit/Rank)

(D-Pad: Navigation) 

# **Build Instructions:**

## **-Windows-**
1. Install `devkitPro` from https://github.com/devkitPro/installer by downloading the latest release.

2. Execute it and follow the installation instructions, you only need to install the tools for the GBA development so ensure you have it selected and install it to the default directory.

3. Go to the Start Menu and search for `MSys2` and open it.

4. Install `Git` by typing this command: `pacman -S git` if you don't have it already installed.

5. Navigate to where you want to clone the project such as your desktop or Documents folder. 
The easiest way to properly `cd` to the directory is to type `cd` and drag and drop the folder into the terminal window and press Enter. 
- Example: `cd '/home/user/OneDrive/Documents/balatro-gba` or something like this.

6. Clone the project with `git clone --recursive https://github.com/cellos51/balatro-gba.git` in the terminal window.

7. Use `cd` and drag the newly generated folder into the window to add the file path and press Enter.

8. Type `make` into the window and press Enter to start building the game.

9. After it completes, navigate through the `build` directory in the project folder and look for `balatro-gba.gba` and load it on an emulator or flashcart.

### **Common Issues:**
1. **When I drag the folder into the window and press enter, it doesn't work!**
- After dragging the folder, nake sure to click back into the window and press enter

2. **When I run `make` it errors out and won't compile!**
- Move the project folder to your desktop and then `cd` to it by dragging the folder into the terminal window after typing `cd` first.
3. **I can't find the compiled rom!**
- Look for a folder called `build` inside the project folder and then find `balatro-gba.gba`.

4. **The Game won't start!**
- Try a different emulator or if you are using original hardware, make sure the rom is not corrupted and try a different flashcart or SD Card. If this does not work, open an issue on the Github page because a recent commit may have broke the game.

5. **It says I don't have `Git` or `Make` installed!**
- Use `pacman -S git` or `pacman -S make` although it should already be installed if you followed the instructions correctly.
