here's a playlist with videos showcasing the product during it's development (by me!) along with some other random recordings of things I've done or made over the years:

https://youtube.com/playlist?list=PLfdxX3m7zNp0zJZA8BhKIpgp9tjVOIGHG&si=wtmgOZPBpbyCYf1-

I've written a detailed description for each video explaining what can be seen in it, i reccommend reading them

**ABOUT THIS REPOSITORY**

this repository is for a device I built a few years ago from scratch. The project consists of the device itself, which is an ESP32 arduino whose code I wrote in C++ with little knowledge about the language.
This device also has wifi access and communicates with a firebase database that I set up. A python script checks for changes to this database,
and when the device updates the database with a request to kick a user from a discord voice channel, the python script interfaces with a simple discord bot that I wrote to kick the user.

While I didn't really know C++ at the time (the code on the arduino is in C++), I did know Java, and at that level of code complexity in this device I haven't encountered any significant differences between java and c++ since the syntax is similar. i also havent encountered problems with memory allocation so far, although I did take some simple measures to minimize the program's memory use on the arduino.

I built this device so that I would have a way to kick my friends out of Discord voice channels by pressing a physical button on my desk. the idea was to make it funny and easy for me to kick my friend with the press of a button.

I used an ESP32 Arduino, Firebase, and Python code. When the button is pressed, the device (which is connected to WiFi) updates Firebase with a request to kick the user, and this change is detected by a Python script that monitors these updates in Firebase. After that, the Python bot uses the Discord API to make the Discord bot kick the specific user from the voice chat in Discord.

My friends found my project funny , so I ordered a few more of the components I used with the intention of building a few of these units and selling them to some of my friends as a ready-made and working product.
the code in this repo is incomplete. unfortunately i cant find the main file with the code for the arduino, although i have a few arduinos with the full functioning code on them, but recovering the code from the onboard memory on the arduinos themselves would be very difficult because it has already been compiled to machine code, so this will take hours of reverse engineering to restore the source code for it. I'd be better off coding it from scratch, which I actually want to do but haven't gotten around to it yet. I'll hopefully do it sometime before i finish highschool.

if you have any questions youre more than welcome to contact me at yo7na7@gmail.com, id be happy to help you set up your own one of these with your friends. i can also upload the 3d print files for the case if somebody wants.
